#include "ClientApp.h"

#include <iostream>
#include <string>

#include "GameConsts.h"
#include "TimeManager.h"
#include "Scene.h"
#include "MainMenuScene.h"

#include <raylib.h>

// This warning was triggered because Raylib is included statically
// Shouldn't cause any issues
#pragma warning(disable: 4098)

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

	TimeManager timeManager;
	timeManager.Update();

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	LoadScene<MainMenuScene>();

	std::string message = "signature:2344575663312;type:CONNECT;user:{name:Attrix;IP:127.0.0.1;Port:" +
		std::to_string(m_udpClient.GetLocalPort()) + "}";
	const char* finalMessage = message.c_str();
	if (!m_udpClient.SendTo("127.0.0.1", SERVER_PORT, finalMessage, static_cast<int>(strlen(finalMessage))))
	{
		std::cerr << "Failed to send message to the server." << std::endl;
		return;
	}

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
