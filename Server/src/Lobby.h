#pragma once

#include "User.h"

#include <unordered_map>

class Lobby
{
public:


private:
	int lobbyId;
	std::unordered_map<int, User> m_users;
};

