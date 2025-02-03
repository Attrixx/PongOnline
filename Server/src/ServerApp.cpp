#include "ServerApp.h"

#include <iostream>

void ServerApp::Run()
{
	InitNetwork();
}

void ServerApp::InitNetwork()
{
	if (!m_udpServer.Init())
	{
		std::cerr << "Server network initialization failed." << std::endl;
		return;
	}

	if (!m_udpServer.CreateSocket())
	{
		std::cerr << "Server socket creation failed." << std::endl;
		return;
	}

	if (!m_udpServer.BindSocket(DEFAULT_PORT))
	{
		std::cerr << "Server socket bind failed." << std::endl;
		return;
	}

	std::cout << "Server Initialized successfully" << std::endl;
}
