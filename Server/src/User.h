#pragma once

#include <string>
#include "winimports.h"

enum UserState
{
	NOLOBBY = 0,
	INLOBBY,
	INGAME
};

enum PaddlePosition
{
	DEFAULT_POSITION = 0,
	LEFT,
	RIGHT
};

class Lobby;

class User
{
public:
	User(int id, const std::string& name, u_short port, const std::string& address);
	~User();

	int GetId() const { return m_id; }
	const std::string& GetName() const { return m_name; }
	u_short GetPort() const { return m_port; }
	UserState GetState() const { return m_state; }
	PaddlePosition GetPaddlePosition() const { return m_paddlePosition; }
	Lobby* GetLobby() const { return m_lobby; }
	const std::string& GetPublicIpAddress() const { return m_publicIpAddress; }
	bool IsOwner() const { return m_isOwner; }

	void SetLobby(Lobby* lobby);
	void SetIsOwner(bool isOwner) { m_isOwner = isOwner; }

private:
	int m_id;
	std::string m_name;
	std::string m_publicIpAddress = "127.0.0.1";
	u_short m_port;

	Lobby* m_lobby = nullptr;
	UserState m_state = NOLOBBY;
	bool m_isOwner = false;

	PaddlePosition m_paddlePosition = DEFAULT_POSITION;
};