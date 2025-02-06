#include "ClientApp.h"

#include <iostream>
#include <string>

#include "ClientGameConsts.h"
#include "CommonGameConsts.h"
#include "TimeManager.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "LobbyListScene.h"
#include "LobbyScene.h"
#include "Message.h"

ClientApp::ClientApp()
	: m_loadedScene(nullptr)
	, m_newScene(nullptr)
	, m_udpClient(new ClientHandler())
	, m_isInGame(false)
{
}

ClientApp::~ClientApp()
{
	delete m_loadedScene;
	delete m_newScene;
}

void ClientApp::Run()
{
	InitNetwork();

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	LoadScene<MainMenuScene>();

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	TimeManager timeManager;
	timeManager.Update();

	while (!WindowShouldClose())
	{
		HandleEvents();

		timeManager.Update();
		Update(timeManager.GetDeltaTime());

		Render();
	}

	CloseWindow();
}

void ClientApp::HandleEvents()
{
	if (m_isInGame)
	{
		if (IsKeyDown(KEY_UP))
		{
			if (m_paddleDirection != PaddleDirection::UP)
			{
				m_paddleDirection = PaddleDirection::UP;
				Message m = Message::CreateMessage(MessageType::PLAY, {
					{"movedPaddle", {"dirY", -1}}
					});
				SendMessage(m);
			}
		}
		else if (IsKeyDown(KEY_DOWN))
		{
			if (m_paddleDirection != PaddleDirection::DOWN)
			{
				m_paddleDirection = PaddleDirection::DOWN;
				Message m = Message::CreateMessage(MessageType::PLAY, {
					{"movedPaddle", {"dirY", 1}}
					});
				SendMessage(m);
			}
		}
		else if (m_paddleDirection != PaddleDirection::NONE)
		{
			m_paddleDirection = PaddleDirection::NONE;
			Message m = Message::CreateMessage(MessageType::PLAY, {
				{"movedPaddle", {"dirY", 0}}
				});
			SendMessage(m);
		}
	}
}

void ClientApp::Update(float deltaTime)
{
	// Replace old scene by the new one on frame start
	if (m_newScene)
	{
		m_loadedScene = m_newScene;
		m_newScene = nullptr;
	}

	if (m_loadedScene)
	{
		m_loadedScene->Update(deltaTime);
	}
}

void ClientApp::Render()
{
	BeginDrawing();
	ClearBackground(WHITE);

	if (m_loadedScene)
	{
		m_loadedScene->Render();
	}

	EndDrawing();
}

void ClientApp::InitNetwork()
{
	if (!m_udpClient.Init())
	{
		std::cerr << "Client network initialization failed." << std::endl;
		return;
	}

	if (!m_udpClient.CreateSocket())
	{
		std::cerr << "Client socket creation failed." << std::endl;
		return;
	}

	if (!m_udpClient.BindSocket(0)) // Let the OS choose an available port for us
	{
		std::cerr << "Client socket bind failed." << std::endl;
		return;
	}

	std::cout << "Client Initialized successfully on port " << m_udpClient.GetLocalPort() << std::endl;
	m_udpClient.StartListening();
}

void ClientApp::SendMessage(Message& message)
{
	if (m_serverHostname.empty())
	{
		std::cerr << "Can't send message to server : no hostname" << std::endl;
		return;
	}

	std::string finalMessage = message.toString(); // Ensure thread safety
	std::vector<char> messageBuffer(finalMessage.begin(), finalMessage.end());

	std::vector<char> hostname(m_serverHostname.begin(), m_serverHostname.end());

	if (!m_udpClient.SendTo(hostname.data(), SERVER_PORT, messageBuffer.data(), static_cast<int>(messageBuffer.size())))
	{
		std::cerr << "Failed to send message to the server." << std::endl;
		return;
	}
}

void ClientHandler::HandleMessage(const Message& message)
{
	switch (static_cast<int>(message.content["messageType"]))
	{
	case MessageType::CONNECT:
	{
		I(ClientApp)->LoadScene<LobbyListScene>();
		I(ClientApp)->SetClientId(message.content["id"]);

		Message message = Message::CreateMessage(MessageType::LOBBIES_LIST, {});
		message.content["id"] = I(ClientApp)->GetClientId();
		I(ClientApp)->SendMessage(message);
	}
	break;
	case MessageType::LOBBIES_LIST:
	{
		if (LobbyListScene* scene = dynamic_cast<LobbyListScene*>(I(ClientApp)->GetLoadedScene()))
		{
			std::vector<LobbyStruct> lobbies;
			for (auto lobby : message.content["data"])
			{
				LobbyStruct lStruct;
				lStruct.name = message.content["name"];
				lStruct.id = message.content["id"];
				lStruct.maxSize = message.content["capacity"];
				lStruct.currentSize = message.content["userAmount"];
				lobbies.push_back(lStruct);
			}
			
			scene->InitLobbiesList(lobbies);
		}
	}
	break;
	case MessageType::DISCONNECT:
	{
		I(ClientApp)->LoadScene<MainMenuScene>();
	}
	break;
	case MessageType::LOGIC:
	{
		if (GameScene* scene = dynamic_cast<GameScene*>(I(ClientApp)->GetLoadedScene()))
		{
			ReplicationData data;
			data.BallPosition = Vector2Float(message.content["ball"]["posX"], message.content["ball"]["posY"]);
			data.BallDirection = Vector2Float(message.content["ball"]["dirX"], message.content["ball"]["dirY"]);
			data.BallSpeed = message.content["ball"]["speed"];

			data.PaddleLeftPosition = Vector2Float(message.content["paddleLeft"]["posX"], message.content["paddleLeft"]["posY"]);
			data.PaddleLeftDirection = Vector2Float(message.content["paddleLeft"]["dirX"], message.content["paddleLeft"]["dirY"]);

			data.PaddleRightPosition = Vector2Float(message.content["paddleRight"]["posX"], message.content["paddleRight"]["posY"]);
			data.PaddleRightDirection = Vector2Float(message.content["paddleRight"]["dirX"], message.content["paddleRight"]["dirY"]);
			scene->OnReplication(data);
		}
	}
	break;
	case MessageType::JOIN_LOBBY_RESPONSE:
	{
		if (LobbyListScene* scene = dynamic_cast<LobbyListScene*>(I(ClientApp)->GetLoadedScene()))
		{
			scene->OnJoinLobby(message.content["canJoin"], message.content["message"]);
		}
	}
	break;
	default:
		break;
	}
}
