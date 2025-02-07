#pragma once

#include "CommonGameConsts.h"

#include <unordered_map>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

class User;
class Ball;
class Paddle;
class Message;
enum PaddlePosition;

constexpr int UPDATE_TICKRATE = 30;

class Lobby
{
public:
	Lobby(int id, const std::string& inName);
	~Lobby();

	void Run();
	void Stop();

	void StartGame();
	void Update(float deltaTime);

	int GetId() const { return lobbyId; }
	const std::string& GetName() const { return name; }

	bool IsEmpty() const { return m_users.empty(); }
	bool IsFull() const { return m_users.size() >= m_capacity; }
	int GetCapactity() const { return m_capacity; }
	int GetUserAmount() const { return static_cast<int>(m_users.size()); }

	void Rename(const std::string& inName) { name = inName; }

	void AddUser(User* user);
	void RemoveUser(int id);
	const std::unordered_map<int, User*>& GetUsers() { return m_users; }
	void TransferOwnership();

	void InitRound();
	void OnBallOutOfScreen(bool isOutOnLeftSide);
	void OnBallCollideWithPaddle();
	void UpdatePaddleDirection(PaddlePosition paddlePosition, int dirY);

	// Send message to specific user if id is specified, everyone otherwise (id = -1)
	void SendMessage(Message& message, int id = -1);

private:
	std::thread m_thread;
	std::atomic_bool m_running = false;

	int lobbyId;
	std::string name;

	int m_capacity;
	std::unordered_map<int, User*> m_users;

	int m_healthPoints;
	int m_score;

	Ball* m_ball;
	Paddle* m_paddleLeft;
	std::mutex m_paddleLeftMutex;

	Paddle* m_paddleRight;
	std::mutex m_paddleRightMutex;
};

