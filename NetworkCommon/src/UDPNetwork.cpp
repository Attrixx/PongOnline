#include "pch.h"
#include "UDPNetwork.h"
#include "Message.h"

#include <iostream>
#include <unordered_map>

UDPNetwork::UDPNetwork() : m_socket(INVALID_SOCKET), m_localPort(0) {}

UDPNetwork::~UDPNetwork()
{
	CleanUp();
}

bool UDPNetwork::Init()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		std::cerr << "WSAStartup failed: " << result << std::endl;
		return false;
	}
	return true;
}

void UDPNetwork::CleanUp()
{
	StopListening();
	if (m_socket != INVALID_SOCKET)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	WSACleanup();
}

bool UDPNetwork::CreateSocket()
{
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}

bool UDPNetwork::BindSocket(u_short port)
{
	sockaddr_in serverAddr = {};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(port == 0 ? 0 : port);

	if (bind(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
		return false;
	}

	if (port == 0)
	{
		sockaddr_in sockAddr;
		int addrSize = sizeof(sockAddr);
		if (getsockname(m_socket, (sockaddr*)&sockAddr, &addrSize) == 0)
		{
			m_localPort = ntohs(sockAddr.sin_port);  // Store the assigned local port
		}
		else
		{
			std::cerr << "Failed to retrieve local port: " << WSAGetLastError() << std::endl;
			return false;
		}
	}
	else
	{
		m_localPort = port;  // If a port was specified, use that one
	}

	return true;
}

bool UDPNetwork::SendTo(const char* address, u_short port, const char* data, int dataSize)
{
	sockaddr_in destAddr = {};
	destAddr.sin_family = AF_INET;
	inet_pton(AF_INET, address, &destAddr.sin_addr);
	destAddr.sin_port = htons(port);

	const int payloadSize = BUFFER_SIZE - sizeof(MessageHeader);
	int packetCount = (dataSize + payloadSize - 1) / payloadSize;

	for (int packetIndex = 0; packetIndex < packetCount; ++packetIndex)
	{
		MessageHeader header;
		header.totalSize = dataSize;
		header.packetIndex = packetIndex;
		header.packetCount = packetCount;

		int offset = packetIndex * payloadSize;
		int packetSize = std::min(payloadSize, dataSize - offset);

		std::vector<char> packet(sizeof(MessageHeader) + packetSize);
		memcpy(packet.data(), &header, sizeof(MessageHeader));
		memcpy(packet.data() + sizeof(MessageHeader), data + offset, packetSize);

		std::cout << "Sending packet " << packetIndex + 1 << "/" << packetCount
			<< " | offset: " << offset
			<< " | size: " << packetSize << std::endl;

		int bytesSent = sendto(m_socket, packet.data(), static_cast<int>(packet.size()), 0, (sockaddr*)&destAddr, sizeof(destAddr));
		if (bytesSent == SOCKET_ERROR)
		{
			std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
			return false;
		}
	}

	return true;
}

int UDPNetwork::ReceiveFrom(char* buffer, int bufferSize, sockaddr_in& senderAddr)
{
	int senderAddrSize = sizeof(senderAddr);
	int bytesReceived = recvfrom(m_socket, buffer, bufferSize, 0, (sockaddr*)&senderAddr, &senderAddrSize);
	if (bytesReceived == SOCKET_ERROR)
	{
		std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
		return 0;
	}
	buffer[bytesReceived] = '\0'; // Null-terminate the buffer
	return bytesReceived;
}

void UDPNetwork::StartListening()
{
	m_running = true;
	m_processing = true;

	m_listenThread = std::thread(&UDPNetwork::Listen, this);
	m_interpreterThread = std::thread(&UDPNetwork::Interpret, this);
}

void UDPNetwork::StopListening()
{
	m_running = false;
	m_processing = false;

	if (m_listenThread.joinable())
		m_listenThread.join();

	m_messageQueueCondition.notify_all();
	if (m_interpreterThread.joinable())
		m_interpreterThread.join();
}

void UDPNetwork::Listen()
{
	std::unordered_map<std::string, MessageBuffer> partialMessages;
	char buffer[BUFFER_SIZE];
	sockaddr_in senderAddr = {};

	while (m_running)
	{
		int receivedBytes = ReceiveFrom(buffer, BUFFER_SIZE, senderAddr);
		if (receivedBytes <= sizeof(MessageHeader))
		{
			std::cerr << "Received packet is too small, ignoring packet, size: " << receivedBytes << std::endl;
			continue;
		}

		MessageHeader* header = reinterpret_cast<MessageHeader*>(buffer);
		if (header->signature != SIGNATURE)
		{
			std::cerr << "Invalid signature, ignoring packet" << std::endl;
			continue;
		}

		std::string senderKey = std::to_string(senderAddr.sin_addr.S_un.S_addr) + ":" + std::to_string(senderAddr.sin_port);
		int dataSize = receivedBytes - sizeof(MessageHeader);

		MessageBuffer& msgBuffer = partialMessages[senderKey];
		msgBuffer.expectedPackets = header->packetCount;
		msgBuffer.receivedPackets++;

		msgBuffer.data.insert(msgBuffer.data.end(), buffer + sizeof(MessageHeader), buffer + sizeof(MessageHeader) + dataSize);

		// Message is complete
		if (msgBuffer.receivedPackets >= msgBuffer.expectedPackets)
		{
			std::vector<char> completeMessage = std::move(msgBuffer.data);
			partialMessages.erase(senderKey);

			std::lock_guard<std::mutex> lock(m_messageQueueMutex);
			m_messageQueue.push(std::move(completeMessage));
			m_messageQueueCondition.notify_one();
		}
	}
}

void UDPNetwork::Interpret()
{
	while (m_processing)
	{
		std::vector<char> messageChar;
		{
			std::unique_lock<std::mutex> lock(m_messageQueueMutex);
			m_messageQueueCondition.wait(lock, [this] { return !m_messageQueue.empty(); });

			if (!m_processing)
			{
				break;
			}

			messageChar = std::move(m_messageQueue.front());
			m_messageQueue.pop();
		}

		std::string messageStr(messageChar.begin(), messageChar.end());
		Message message = Message::toMessage(messageStr.c_str());

		std::cout << "Received message: " << message.toString() << std::endl;
	}
}
