#include "App.h"

#include "GameConsts.h"
#include "TimeManager.h"

#include <raylib.h>

// This warning was triggered because Raylib is included statically
// Shouldn't cause any issues
#pragma warning(disable: 4098)

void App::Run()
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

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

void App::HandleEvents()
{
	// TODO: Handle player input events here
}

void App::Update(float deltaTime)
{
	// TODO: Update game logic here
	// Call Entity::Update(deltaTime) for each entity
}

void App::Render()
{
	BeginDrawing();
	ClearBackground(WHITE);

	// TODO: Draw game objects here
	// Draw associated shape/sprite to each entity

	EndDrawing();
}
