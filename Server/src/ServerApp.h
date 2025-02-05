#pragma once

#include "Singleton.h"
#include "UDPNetwork.h"
#include "User.h"
#include "Lobby.h"

#include <unordered_map>
#include <atomic>
#include <mutex>

class ServerHandler : public NetworkHandler
{
public:
	void HandleMessage(const Message& message) override;
};


class Ball;
class Paddle;

class ServerApp : public Singleton<ServerApp>
{
	friend class Singleton<ServerApp>;

public:
	ServerApp();
	virtual ~ServerApp();

	void Run();
	void OnBallOutOfScreen(bool isOutOnLeftSide);

	void RegisterUser(const std::string& name, u_short port);
	void UnregisterUser(int id);

	int CreateLobby(int userId, const std::string& name);
	void RemoveLobby(int lobbyId);

	void JoinLobby(int userId, int lobbyId);
	void LeaveLobby(int userId);

private:
	void InitNetwork();
	void InitRound();

	User* GetUser(int id);
	Lobby* GetLobby(int id);

	void Update(float deltaTime);


private:
	UDPNetwork m_udpServer;

	std::unordered_map<int, User*> m_users;
	std::unordered_map<int, Lobby*> m_lobbies;
	std::mutex m_usersMutex;
	std::mutex m_lobbiesMutex;

	std::atomic_int m_userIdCounter = -1;
	std::atomic_int m_lobbyIdCounter = -1;

	Ball* m_ball;
	Paddle* m_paddleLeft;
	Paddle* m_paddleRight;

	int m_healthPoints;

};

