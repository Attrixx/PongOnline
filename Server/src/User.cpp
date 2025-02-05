#include "User.h"
#include "Lobby.h"

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
