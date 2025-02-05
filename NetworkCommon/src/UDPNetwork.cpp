#include "pch.h"
#include "UDPNetwork.h"

#include <iostream>
#include <unordered_map>

UDPNetwork::UDPNetwork(NetworkHandler* handler) : m_socket(INVALID_SOCKET), m_localPort(0), m_networkHandler(handler) {}

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
	SetThreadDescription(m_listenThread.native_handle(), L"UDPNetwork Listen");

	m_interpreterThread = std::thread(&UDPNetwork::Interpret, this);
	SetThreadDescription(m_interpreterThread.native_handle(), L"UDPNetwork Interpret");
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
			m_messageQueueCondition.wait(lock, [this] { return !m_messageQueue.empty() || !m_processing; });

			if (!m_processing)
			{
				break;
			}

			messageChar = std::move(m_messageQueue.front());
			m_messageQueue.pop();
		}

		std::string messageStr(messageChar.begin(), messageChar.end());
		Message message = Message::toMessage(messageStr.c_str());

		try
		{
			m_networkHandler->HandleMessage(message);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Wrong message format detected: " << e.what() << std::endl;
		}
	}
}

std::string UDPNetwork::GetPublicIPAddress() const
{
	std::string ipAddress = "127.0.0.1"; // Default fallback
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80); // Arbitrary external port
	inet_pton(AF_INET, "3.215.250.207", &addr.sin_addr); // Check ip amazonaws

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << "socket() failed with error: " << WSAGetLastError() << std::endl;
		return ipAddress;
	}

	if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		std::cerr << "connect() failed with error: " << WSAGetLastError() << std::endl;
		closesocket(sock);
		return ipAddress;
	}

	char request[] = "GET / HTTP/1.1\r\nHost: checkip.amazonaws.com\r\nConnection: close\r\n\r\n";
	send(sock, request, static_cast<int>(strlen(request)), 0);

	char buffer[1024] = { 0 };
	int recv_size = recv(sock, buffer, sizeof(buffer) - 1, 0);

	closesocket(sock);

	if (recv_size == SOCKET_ERROR) {
		std::cerr << "recv() failed with error: " << WSAGetLastError() << std::endl;
		return ipAddress;
	}

	buffer[recv_size] = '\0';

	std::string response(buffer);
	size_t pos = response.rfind("\r\n");
	if (pos != std::string::npos) {
		return response.substr(pos + 2);
	}
	
	return ipAddress;
}

