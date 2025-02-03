#include "ClientApp.h"

#include <iostream>
#include <string>

#include "GameConsts.h"
#include "TimeManager.h"

// This warning was triggered because Raylib is included statically
// Shouldn't cause any issues
#pragma warning(disable: 4098)

void ClientApp::Run()
{
	InitNetwork();

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

	TimeManager timeManager;
	timeManager.Update();

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

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
		HandleEvents();

		timeManager.Update();
		Update(timeManager.GetDeltaTime());
		
		Render();
	}

	CloseWindow();
}

void ClientApp::HandleEvents()
{
	// TODO: Handle player input events here
}

void ClientApp::Update(float deltaTime)
{
	// TODO: Update game logic here
	// Call Entity::Update(deltaTime) for each entity
}

void ClientApp::Render()
{
	BeginDrawing();
	ClearBackground(WHITE);

	// TODO: Draw game objects here
	// Draw associated shape/sprite to each entity

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
