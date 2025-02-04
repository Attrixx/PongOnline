#include "ClientApp.h"

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
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

	TimeManager timeManager;
	timeManager.Update();

	LoadScene<MainMenuScene>();

	while (!WindowShouldClose())
	{
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
	// Replace old scene by the new one on frame start
	if (m_newScene)
	{
		m_loadedScene = m_newScene;
		m_newScene = nullptr;
	}
	// TODO: Update game logic here
	// Call Entity::Update(deltaTime) for each entity
	if (m_loadedScene)
	{
		m_loadedScene->Update(deltaTime);
	}
}

void ClientApp::Render()
{
	BeginDrawing();
	ClearBackground(WHITE);

	// TODO: Draw game objects here
	// Draw associated shape/sprite to each entity

	if (m_loadedScene)
	{
		m_loadedScene->Render();
	}

	EndDrawing();
}
