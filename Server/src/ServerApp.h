#pragma once

#include "UDPNetwork.h"
#include "User.h"

#include <vector>

class ServerHandler : public NetworkHandler
{
public:
	void HandleMessage(const Message& message) override;
};

class ServerApp
{
public:
	ServerApp();

	void Run();

private:
	void InitNetwork();

	void RegisterUser(const std::string& name, u_short port);
	void UnregisterUser(int id);

private:
	UDPNetwork m_udpServer;
	std::vector<User> m_users;

	int m_userIdCounter = 0;
};

