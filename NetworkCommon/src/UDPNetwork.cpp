#include "pch.h"
#include "UDPNetwork.h"

#include <iostream>

UDPNetwork::UDPNetwork() : m_socket(INVALID_SOCKET) {}

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
    serverAddr.sin_port = htons(port);

    if (bind(m_socket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

bool UDPNetwork::SendTo(const char* address, u_short port, const char* data, int dataSize)
{
    sockaddr_in destAddr = {};
    destAddr.sin_family = AF_INET;
    inet_pton(AF_INET, address, &destAddr.sin_addr);
    destAddr.sin_port = htons(port);

    int bytesSent = sendto(m_socket, data, dataSize, 0, (sockaddr*)&destAddr, sizeof(destAddr));
    if (bytesSent == SOCKET_ERROR)
    {
        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

bool UDPNetwork::ReceiveFrom(char* buffer, int bufferSize, sockaddr_in& senderAddr)
{
    int senderAddrSize = sizeof(senderAddr);
    int bytesReceived = recvfrom(m_socket, buffer, bufferSize, 0, (sockaddr*)&senderAddr, &senderAddrSize);
    if (bytesReceived == SOCKET_ERROR)
    {
        std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    buffer[bytesReceived] = '\0'; // Null-terminate the buffer
    return true;
}

void UDPNetwork::StartListening()
{
	if (m_running)
	{
		return;
	}

	m_running = true;
	m_listenThread = std::thread(&UDPNetwork::Listen, this);
}

void UDPNetwork::StopListening()
{
	if (!m_running)
	{
		return;
	}

	m_running = false;

    if (m_listenThread.joinable())
    {
		m_listenThread.join();
    }
}

void UDPNetwork::Listen()
{
	char buffer[BUFFER_SIZE];
	sockaddr_in senderAddr = {};

    while (m_running)
    {
        if (ReceiveFrom(buffer, BUFFER_SIZE, senderAddr))
        {
            std::cout << "Received: " << buffer << std::endl;
        }
    }
}
