#include "GameScene.h"
#include "Entity.h"

void GameScene::OnInitialize()
{
	Entity* ball = CreateEntity<Entity>();
	ball->Init(Vector2(0, 0), Vector2(1, 0), 100, "res/ball.png", BLACK);

	Entity* paddleRight = CreateEntity<Entity>();
	paddleRight->Init(Vector2(100, 0), Vector2(0, 0), 100, "res/paddle.png", BLUE);

	Entity* paddleLeft = CreateEntity<Entity>();
	paddleLeft->Init(Vector2(-100, 0), Vector2(0, 0), 100, "res/paddle.png", RED);
}

void GameScene::OnUpdate(float deltaTime)
{
}

void GameScene::OnRender()
{
}

void GameScene::OnUninitialize()
{
}
