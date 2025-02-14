#pragma once

#include "Singleton.h"
#include "UDPNetwork.h"
#include "User.h"
#include "Lobby.h"

#include <unordered_map>
#include <atomic>
#include <mutex>
#include <condition_variable>

class ServerHandler : public NetworkHandler
{
public:
	ServerHandler() {}
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
	void WakeUpMain(int lobbyId);

	void SendMessage(const char* address, u_short port, Message& message);

	int RegisterUser(const std::string& name, u_short port, const std::string& address);
	void UnregisterUser(int id);

	int CreateLobby(int userId, const std::string& name);
	void RemoveLobby(int lobbyId);
	void StopLobby(int lobbyId);

	void JoinLobby(int userId, int lobbyId);
	void LeaveLobby(int userId);
	void StartLobbyByOwner(int userId);
	void SendLobbies(int userId);
	void SendLobbyPlayers(int lobbyId);

	void OnPaddleDirectionChanged(int userId, int dirY);

	std::unordered_map<int, Lobby*> GetLobbies() { return m_lobbies; }
	UDPNetwork& GetUdpServer() { return m_udpServer; }

private:
	void InitNetwork();

	User* GetUser(int id);
	Lobby* GetLobby(int id);

	void Update(float deltaTime);

private:
	UDPNetwork m_udpServer;
	std::condition_variable m_cv;
	std::mutex m_mutex;

	std::unordered_map<int, User*> m_users;
	std::unordered_map<int, Lobby*> m_lobbies;
	std::mutex m_usersMutex;
	std::mutex m_lobbiesMutex;

	std::atomic_int m_userIdCounter = 0;
	std::atomic_int m_lobbyIdCounter = 0;

	int m_lobbyToStop = -1;
};

