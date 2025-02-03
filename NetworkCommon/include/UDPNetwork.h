#pragma once

#include "winimports.h"

#include <atomic>
#include <thread>

constexpr int BUFFER_SIZE = 1024;

class UDPNetwork
{
public:
	UDPNetwork();
	~UDPNetwork();

	bool Init();
	void CleanUp();

	bool CreateSocket();
	bool BindSocket(u_short port);
	bool SendTo(const char* address, u_short port, const char* data, int dataSize);
	bool ReceiveFrom(char* buffer, int bufferSize, sockaddr_in& senderAddr);

	void StartListening();
	void StopListening();

private:
	void Listen();

private:
	SOCKET m_socket;
	std::thread m_listenThread;
	std::atomic_bool m_running;
};