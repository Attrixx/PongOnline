#pragma once

#include "UDPNetwork.h"
#include "User.h"

#include <vector>

class ServerApp
{
public:
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

