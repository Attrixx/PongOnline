#pragma once

#include "winimports.h"

constexpr int DEFAULT_PORT = 8888;

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

private:
	SOCKET m_socket;
};