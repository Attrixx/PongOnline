#include "Lobby.h"
#include "User.h"
#include "Ball.h"
#include "Paddle.h"
#include "Message.h"
#include "ServerApp.h"

#include <winimports.h>
#include <stringapiset.h>

Lobby::Lobby(int id, const std::string& inName)
	: lobbyId(id)
	, name(inName)
	, m_healthPoints(HEALTH_POINTS)
	, m_capacity(2)
{
	m_ball = new Ball(this);
	m_paddleLeft = new Paddle();
	m_paddleRight = new Paddle();
}

Lobby::~Lobby()
{
	for (auto& user : m_users)
	{
		user.second->SetLobby(nullptr);
	}

	delete m_ball;
	delete m_paddleLeft;
	delete m_paddleRight;
}

void Lobby::Run()
{
	// TODO: Send message to all clients to notify game has started
	Message message = Message::CreateMessage(MessageType::START_GAME, {});

	InitRound();

	const auto tickDuration = std::chrono::milliseconds(1000 / UPDATE_TICKRATE);
	auto lastTime = std::chrono::steady_clock::now();
	auto nextTick = lastTime + tickDuration;
	while (m_running)
	{
		auto now = std::chrono::steady_clock::now();
		if (m_healthPoints <= 0)
		{
			m_running = false;
			break;
		}

		float deltaTime = std::chrono::duration<float>(now - lastTime).count();
		lastTime = now;

		Update(deltaTime);

		nextTick += tickDuration;
		std::this_thread::sleep_until(nextTick);
	}
}

std::wstring ConvertToWideString(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

void Lobby::StartGame()
{
	m_running = true;
    m_thread = std::thread(&Lobby::Run, this);
    std::string threadName = "Runner Lobby " + std::to_string(lobbyId);
    std::wstring wideThreadName = ConvertToWideString(threadName);
    SetThreadDescription(m_thread.native_handle(), wideThreadName.c_str());
}

void Lobby::Update(float deltaTime)
{
	// Update entities
	m_ball->Update(deltaTime);
	m_paddleLeft->Update(deltaTime);
	m_paddleRight->Update(deltaTime);

	// Check collision
	m_ball->CheckCollision(m_paddleLeft);
	m_ball->CheckCollision(m_paddleRight);

	// Send data to clients
	Message message = Message::CreateMessage(MessageType::LOGIC, {
		{"ball", {{"posX", m_ball->GetPosition().x}, {"posY", m_ball->GetPosition().y}, {"dirX", m_ball->GetDirection().x}, {"dirY", m_ball->GetDirection().y}, {"speed", m_ball->GetSpeed()}}},
		{"paddleRight", {{"posX", m_paddleRight->GetPosition().x}, {"posY", m_paddleRight->GetPosition().y}, {"dirY", m_paddleRight->GetDirection().y}}},
		{"paddleLeft", {{"posX", m_paddleLeft->GetPosition().x}, {"posY", m_paddleLeft->GetPosition().y}, {"dirY", m_paddleLeft->GetDirection().y}}}
	});
	SendMessage(message);
}

void Lobby::AddUser(User* user)
{
	m_users.insert({ user->GetId(), user });
}

void Lobby::RemoveUser(int id)
{
	auto it = m_users.find(id);
	if (it != m_users.end())
	{
		m_users.erase(it);
	}
}

void Lobby::InitRound()
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

void Lobby::OnBallOutOfScreen(bool isOutOnLeftSide)
{
	if (m_healthPoints > 0)
	{
		--m_healthPoints;
		InitRound();
	}

	Message message = Message::CreateMessage(MessageType::SCORE, {
		{"score", m_healthPoints}
		});
	SendMessage(message);
}

void Lobby::UpdatePaddleDirection(PaddlePosition paddlePosition, int dirY)
{
	switch (paddlePosition)
	{
	case PaddlePosition::LEFT:
	{
		m_paddleLeft->SetDirection(Vector2Float(0, dirY * 1.f));
	}
	break;
	case PaddlePosition::RIGHT:
	{
		m_paddleRight->SetDirection(Vector2Float(0, dirY * 1.f));
	}
	break;
	default:
		break;
	}
}

void Lobby::SendMessage(Message& message, int id)
{
	if (id < 0)
	{
		for (auto it = m_users.begin(); it != m_users.end(); ++it)
		{
			I(ServerApp)->SendMessage(it->second->GetPublicIpAddress().c_str(), it->second->GetPort(), message);
		}
	}
	else
	{
		auto it = m_users.find(id);
		if (it != m_users.end())
		{
			I(ServerApp)->SendMessage(it->second->GetPublicIpAddress().c_str(), it->second->GetPort(), message);
		}
	}
}
