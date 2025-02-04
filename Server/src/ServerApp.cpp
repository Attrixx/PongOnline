#include "ServerApp.h"
#include "Ball.h"
#include "Paddle.h"
#include "CommonGameConsts.h"

#include <iostream>
#include <cmath>
#include <ctime>

ServerApp::ServerApp()
	: m_ball(nullptr)
	, m_paddleLeft(nullptr)
	, m_paddleRight(nullptr)
	, m_healthPoints(3)
	, m_udpServer(new ServerHandler())
{
}

ServerApp::~ServerApp()
{
	delete m_ball;
	delete m_paddleLeft;
	delete m_paddleRight;
}

void ServerApp::Run()
{
	srand(time(NULL));

	// TODO: Get server IP
	// Server has id 0
	RegisterUser("Server", SERVER_PORT);

	m_ball = new Ball();
	m_paddleLeft = new Paddle();
	m_paddleRight = new Paddle();

	InitNetwork();

	InitRound();

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void ServerApp::InitNetwork()
{
	if (!m_udpServer.Init())
	{
		std::cerr << "Server network initialization failed." << std::endl;
		return;
	}

	if (!m_udpServer.CreateSocket())
	{
		std::cerr << "Server socket creation failed." << std::endl;
		return;
	}

	if (!m_udpServer.BindSocket(SERVER_PORT))
	{
		std::cerr << "Server socket bind failed." << std::endl;
		return;
	}

	std::cout << "Server Initialized successfully on port " << m_udpServer.GetLocalPort() << std::endl;
	m_udpServer.StartListening();
}

void ServerApp::Update(float deltaTime)
{
	// Update objects
	m_ball->Update(deltaTime);
	m_paddleLeft->Update(deltaTime);
	m_paddleRight->Update(deltaTime);

	// Check collisions
	m_ball->CheckCollision(m_paddleLeft);
	m_ball->CheckCollision(m_paddleRight);
}

void ServerApp::RegisterUser(const std::string& name, u_short port)
{
	m_users.emplace_back(m_userIdCounter, name, port);
	m_userIdCounter++;
	std::cout << "User " << name << " connected" << std::endl;
}

void ServerApp::UnregisterUser(int id)
{
	auto it = std::find_if(m_users.begin(), m_users.end(), [id](const User& user) { return user.GetId() == id; });
	if (it != m_users.end())
	{
		m_users.erase(it);
	}
}

void ServerHandler::HandleMessage(const Message& message)
{
	json content = message.content;
	json data = content["data"];

	MessageType type = content["messageType"];
	switch (type)
	{
	case MessageType::CONNECT:
	{
		std::string name = data["name"];
		u_short port = data["port"];
		ServerApp::GetInstance()->RegisterUser(name, port);
	}
	break;
	}
}

void ServerApp::InitRound()
{
	m_ball->SetPosition(Vector2Float(WINDOW_WIDTH * 0.5f - BALL_RADIUS, WINDOW_HEIGHT * 0.5f - BALL_RADIUS));
	m_ball->SetSpeed(BALL_INITIAL_SPEED);

	bool leftSize = rand() / RAND_MAX > 0.5f;
	float minAngle = leftSize ? -BALL_INITIAL_MAX_HALF_ANGLE : -BALL_INITIAL_MAX_HALF_ANGLE + (M_PI * 0.5f);
	float maxAngle = minAngle + (BALL_INITIAL_MAX_HALF_ANGLE * 2.f);
	float angle = (minAngle + (rand() / (float)RAND_MAX) * (maxAngle - minAngle));
	m_ball->SetDirection(Vector2Float(cos(angle), sin(angle)));

	m_paddleLeft->SetPosition(Vector2Float(PADDLE_MARGIN, WINDOW_HEIGHT * 0.5f - PADDLE_HEIGHT));
	m_paddleLeft->SetDirection(Vector2Float(0.f, 1.f));
	m_paddleLeft->SetSpeed(0.f);

	m_paddleRight->SetPosition(Vector2Float(WINDOW_WIDTH - PADDLE_MARGIN - PADDLE_WIDTH, WINDOW_HEIGHT * 0.5f - PADDLE_HEIGHT));
	m_paddleRight->SetDirection(Vector2Float(0.f, 1.f));
	m_paddleRight->SetSpeed(0.f);
}

void ServerApp::OnBallOutOfScreen(bool isOutOnLeftSide)
{
	if (m_healthPoints > 0)
	{
		--m_healthPoints;
	}

	// Send scores
}
