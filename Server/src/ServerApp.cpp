#include "ServerApp.h"
#include "UDPNetwork.h"

#include <iostream>

void ServerApp::Run()
{
	UDPNetwork udpServer;
	if (!udpServer.Init())
	{
		std::cerr << "Server network initialization failed." << std::endl;
		return;
	}

	if (!udpServer.CreateSocket())
	{
		std::cerr << "Server socket creation failed." << std::endl;
		return;
	}

	if (!udpServer.BindSocket(DEFAULT_PORT))
	{
		std::cerr << "Server socket bind failed." << std::endl;
		return;
	}

	std::cout << "Server Initialized successfully" << std::endl;
}
