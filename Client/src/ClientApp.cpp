#include "ClientApp.h"

#include "GameConsts.h"
#include "TimeManager.h"
#include "UDPNetwork.h"

#include "raylib.h"

#include <iostream>

// This warning was triggered because Raylib is included statically
// Shouldn't cause any issues
#pragma warning(disable: 4098)

void ClientApp::Run()
{
	InitNetwork();

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

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
	UDPNetwork udpClient;
	if (!udpClient.Init())
	{
		std::cerr << "Client network initialization failed." << std::endl;
		return;
	}

	if (!udpClient.CreateSocket())
	{
		std::cerr << "Client socket creation failed." << std::endl;
		return;
	}

	if (!udpClient.BindSocket(DEFAULT_PORT))
	{
		std::cerr << "Client socket bind failed." << std::endl;
		return;
	}

	std::cout << "Client Initialized successfully" << std::endl;
}
