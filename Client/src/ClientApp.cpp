#include "ClientApp.h"

#include <iostream>
#include <string>

#include "ClientGameConsts.h"
#include "CommonGameConsts.h"
#include "TimeManager.h"
#include "Scene.h"
#include "MainMenuScene.h"
#include "Message.h"

ClientApp::ClientApp()
	: m_loadedScene(nullptr)
	, m_newScene(nullptr)
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

	Message message;
	message.content["id"] = -1;
	message.content["messageType"] = static_cast<int>(MessageType::CONNECT);
	message.content["data"] = message.connectData;
	message.content["data"]["name"] = "Client";
	message.content["data"]["port"] = m_udpClient.GetLocalPort();

	std::string finalMessage = message.toString(); // Ensure thread safety
	std::vector<char> messageBuffer(finalMessage.begin(), finalMessage.end());
	std::cout << "Sending message: " << finalMessage << std::endl;

	if (!m_udpClient.SendTo("127.0.0.1", SERVER_PORT, messageBuffer.data(), static_cast<int>(messageBuffer.size())))
	{
		std::cerr << "Failed to send message to the server." << std::endl;
		return;
	}

	TimeManager timeManager;
	timeManager.Update();

	while (!WindowShouldClose())
	{
		timeManager.Update();
		Update(timeManager.GetDeltaTime());
		
		Render();
	}

	CloseWindow();
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
