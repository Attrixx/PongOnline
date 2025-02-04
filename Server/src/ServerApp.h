#pragma once

#include "Singleton.h"
#include "UDPNetwork.h"
#include "User.h"

#include <vector>

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

private:
	void InitNetwork();
	void InitRound();

	void Update(float deltaTime);


private:
	UDPNetwork m_udpServer;
	std::vector<User> m_users;

	int m_userIdCounter = 0;

	Ball* m_ball;
	Paddle* m_paddleLeft;
	Paddle* m_paddleRight;

	int m_healthPoints;

};

