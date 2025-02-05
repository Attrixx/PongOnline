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
	User(int id, const std::string& name, u_short port) : m_id(id), m_name(name), m_port(port) {}
	~User();

	int GetId() const { return m_id; }
	const std::string& GetName() const { return m_name; }
	u_short GetPort() const { return m_port; }
	UserState GetState() const { return m_state; }
	PaddlePosition GetPaddlePosition() const { return m_paddlePosition; }
	Lobby* GetLobby() const { return m_lobby; }

	void SetLobby(Lobby* lobby);

private:
	int m_id;
	std::string m_name;
	u_short m_port;

	UserState m_state = NOLOBBY;
	PaddlePosition m_paddlePosition = DEFAULT_POSITION;
	Lobby* m_lobby = nullptr;
};