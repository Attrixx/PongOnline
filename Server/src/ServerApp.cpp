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
	srand(static_cast<unsigned int>(time(NULL)));

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

void ServerApp::SendMessage(const char* address, u_short port, Message& message)
{
	std::string finalMessage = message.toString(); // Ensure thread safety
	std::vector<char> messageBuffer(finalMessage.begin(), finalMessage.end());

	if (!m_udpServer.SendTo(address, port, messageBuffer.data(), static_cast<int>(messageBuffer.size())))
	{
		std::cerr << "Failed to send message to the client." << std::endl;
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

int ServerApp::RegisterUser(const std::string& name, u_short port)
{
	int userId = m_userIdCounter.fetch_add(1, std::memory_order_relaxed);

	{
		std::lock_guard<std::mutex> lock(m_usersMutex);
		m_users.insert({ userId, new User(userId, name, port) });
	}

	std::cout << "User " << name << " connected" << std::endl;
	return userId;
}

void ServerApp::UnregisterUser(int id)
{
	{
		std::lock_guard<std::mutex> lock(m_usersMutex);
		User* user = GetUser(id);
		if (user)
		{
			m_users.erase(id);
		}
	}
}

int ServerApp::CreateLobby(int userId, const std::string& name)
{
	int lobbyId = m_lobbyIdCounter.fetch_add(1, std::memory_order_relaxed);

	{
		std::lock_guard<std::mutex> lock(m_lobbiesMutex);

		m_lobbies.insert({ lobbyId, new Lobby(userId, name) });
	}

	return lobbyId;
}

void ServerApp::RemoveLobby(int lobbyId)
{
	{
		std::lock_guard<std::mutex> lock(m_lobbiesMutex);
		Lobby* lobby = GetLobby(lobbyId);
		if (lobby)
		{
			m_lobbies.erase(lobbyId);
		}
	}
}

void ServerApp::JoinLobby(int userId, int lobbyId)
{
	{
		std::lock_guard<std::mutex> lock(m_lobbiesMutex);
		std::lock_guard<std::mutex> lock2(m_usersMutex);

		Lobby* lobby = GetLobby(lobbyId);
		User* user = GetUser(userId);

		if (lobby && user)
		{
			lobby->AddUser(user);
			user->SetLobby(lobby);
		}
	}
}

void ServerApp::LeaveLobby(int userId)
{
	{
		std::lock_guard<std::mutex> lock(m_lobbiesMutex);
		std::lock_guard<std::mutex> lock2(m_usersMutex);

		User* user = GetUser(userId);
		Lobby* lobby = user->GetLobby();

		if (user && lobby)
		{
			lobby->RemoveUser(user->GetId());
			user->SetLobby(nullptr);

			if (lobby->IsEmpty())
			{
				RemoveLobby(lobby->GetId());
			}
		}
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
		std::string address = data["address"];

		std::cout << "Client address: " << address << std::endl;

		int userId = I(ServerApp)->RegisterUser(name, port);

		// Send User Id
		Message response = Message::CreateMessage(MessageType::CONNECT, {});
		response.content["id"] = userId;

		I(ServerApp)->SendMessage(address.c_str(), port, response);
	}
	break;
	case MessageType::DISCONNECT:
	{
		int id = data["id"];
		I(ServerApp)->UnregisterUser(id);
	}
	break;
	case MessageType::CREATE_LOBBY:
	{
		int userId = data["id"];
		std::string name = data["name"];

		ServerApp* app = I(ServerApp);
		int lobbyId = app->CreateLobby(userId, name);
		app->JoinLobby(userId, lobbyId);
	}
	break;
	case MessageType::JOIN_LOBBY:
	{
		int userId = data["id"];
		int lobbyId = data["lobbyId"];
		I(ServerApp)->JoinLobby(userId, lobbyId);
	}
	break;
	case MessageType::LEAVE_LOBBY:
	{
		int userId = data["id"];
		I(ServerApp)->LeaveLobby(userId);
	}
	break;
	case MessageType::PLAY:
	{
		int dirY = data["movedPaddle"]["dirY"];
		// Get lobby with user Id
		// Get user paddle
		// Move paddle
	}
	break;
	default:
	{

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

User* ServerApp::GetUser(int id)
{
	auto it = m_users.find(id);
	if (it != m_users.end())
	{
		return it->second;
	}
	return nullptr;
}

Lobby* ServerApp::GetLobby(int id)
{
	auto it = m_lobbies.find(id);
	if (it != m_lobbies.end())
	{
		return it->second;
	}
	return nullptr;
}

void ServerApp::OnBallOutOfScreen(bool isOutOnLeftSide)
{
	if (m_healthPoints > 0)
	{
		--m_healthPoints;
	}

	// Send scores
}
