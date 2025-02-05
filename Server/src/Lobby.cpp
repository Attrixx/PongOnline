#include "Lobby.h"
#include "User.h"

Lobby::~Lobby()
{
	for (auto& user : m_users)
	{
		user.second->SetLobby(nullptr);
	}
}

void Lobby::AddUser(User* user)
{
	m_users.insert({ user->GetId(), user });
}

void Lobby::RemoveUser(int id)
{
	auto it = m_users.find(id);
	if (it != m_users.end())
	{
		m_users.erase(it);
	}
}
