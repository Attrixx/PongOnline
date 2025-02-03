#pragma once

#include "UDPNetwork.h"

class ServerApp
{
public:
	void Run();

private:
	void InitNetwork();

private:
	UDPNetwork m_udpServer;
};

