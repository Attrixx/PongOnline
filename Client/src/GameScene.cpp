#include "GameScene.h"
#include "CommonGameConsts.h"
#include "Entity.h"

void GameScene::OnInitialize()
{
	m_ball = CreateEntity<Entity>();
	m_ball->Init(Vector2(0, 0), Vector2(1, 0), 100, "res/ball.png", PURPLE);

	m_paddleRight = CreateEntity<Entity>();
	m_paddleRight->Init(Vector2(WINDOW_WIDTH - PADDLE_MARGIN, 0), Vector2(0, 0), 100, "res/paddle.png", BLUE);

	m_paddleLeft = CreateEntity<Entity>();
	m_paddleLeft->Init(Vector2(PADDLE_MARGIN, 0), Vector2(0, 0), 100, "res/paddle.png", RED);
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

void GameScene::OnReplication(const ReplicationData& replicationData)
{
}
