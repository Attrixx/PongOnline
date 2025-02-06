#pragma once

#include "CommonGameConsts.h"

#include <unordered_map>
#include <string>

class User;
class Ball;
class Paddle;
class Message;

class Lobby
{
public:
	Lobby(int id, const std::string& inName);
	~Lobby();

	void Update(float deltaTime);

	int GetId() const { return lobbyId; }
	const std::string& GetName() const { return name; }

	bool IsEmpty() const { return m_users.empty(); }

	void Rename(const std::string& inName) { name = inName; }
	
	void AddUser(User* user);
	void RemoveUser(int id);

	void InitRound();
	void OnBallOutOfScreen(bool isOutOnLeftSide);

	// Send message to specific user if id is specified, everyone otherwise (id = -1)
	void SendMessage(Message& message, int id = -1);

private:
	int lobbyId;
	std::string name;

	std::unordered_map<int, User*> m_users;

	int m_healthPoints;

	Ball* m_ball;
	Paddle* m_paddleLeft;
	Paddle* m_paddleRight;

};

