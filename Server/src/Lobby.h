#pragma once

#include "CommonGameConsts.h"

#include <unordered_map>
#include <string>
#include <thread>

class User;
class Ball;
class Paddle;

class Lobby
{
public:
	Lobby(int id, const std::string& inName);
	~Lobby();

	void StartGame();
	void Update(float deltaTime);

	int GetId() const { return lobbyId; }
	const std::string& GetName() const { return name; }

	bool IsEmpty() const { return m_users.empty(); }
	bool IsFull() const { return m_users.size() >= m_capacity; }

	void Rename(const std::string& inName) { name = inName; }
	
	void AddUser(User* user);
	void RemoveUser(int id);

	void InitRound();
	void OnBallOutOfScreen(bool isOutOnLeftSide);

private:
	std::thread m_thread;

	int lobbyId;
	std::string name;

	int m_capacity;
	std::unordered_map<int, User*> m_users;

	int m_healthPoints;

	Ball* m_ball;
	Paddle* m_paddleLeft;
	Paddle* m_paddleRight;

};

