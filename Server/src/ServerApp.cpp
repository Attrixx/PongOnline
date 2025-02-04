#include "ServerApp.h"

#include <iostream>

void ServerApp::Run()
{
	// TODO: Get server IP
	// Server has id 0
	RegisterUser("Server", SERVER_PORT);

	InitNetwork();

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
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

	if (!m_udpServer.BindSocket(SERVER_PORT))
	{
		std::cerr << "Server socket bind failed." << std::endl;
		return;
	}

	std::cout << "Server Initialized successfully on port " << m_udpServer.GetLocalPort() << std::endl;
	m_udpServer.StartListening();
}

void ServerApp::RegisterUser(const std::string& name, u_short port)
{
	m_users.emplace_back(m_userIdCounter, name, port);
	m_userIdCounter++;
}

void ServerApp::UnregisterUser(int id)
{
	auto it = std::find_if(m_users.begin(), m_users.end(), [id](const User& user) { return user.GetId() == id; });
	if (it != m_users.end())
	{
		m_users.erase(it);
	}
}
