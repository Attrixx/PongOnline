#include "User.h"
#include "Lobby.h"

User::User(int id, const std::string& name, u_short port, const std::string& address) 
	: m_id(id)
	, m_name(name)
	, m_port(port)
	, m_publicIpAddress(address)
{}

User::~User()
{
	if (m_lobby)
	{
		m_lobby->RemoveUser(m_id);
	}
}

void User::SetLobby(Lobby* lobby)
{
	m_lobby = lobby; 
}
