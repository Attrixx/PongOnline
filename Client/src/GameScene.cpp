#include "GameScene.h"
#include "CommonGameConsts.h"
#include "UIElement.h"
#include "BasicText.h"
#include "Entity.h"
#include "Ball.h"
#include "ClientApp.h"

#include <string>

void GameScene::OnInitialize()
{
	m_score = CreateUIElement<BasicText>();
	m_score->SetText("0");
	m_score->SetTextColor(BLACK);
	m_score->SetPosition({ WINDOW_WIDTH * 0.5f, 10.f });

	m_ball = CreateEntity<Ball>();
	m_ball->Init(Vector2(0, 0), Vector2(1, 0), 100, "res/ball.png", PURPLE);

	m_paddleRight = CreateEntity<Entity>();
	m_paddleRight->Init(Vector2(WINDOW_WIDTH - PADDLE_MARGIN, 0), Vector2(0, 0), 100, "res/paddle.png", BLUE);

	m_paddleLeft = CreateEntity<Entity>();
	m_paddleLeft->Init(Vector2(PADDLE_MARGIN, 0), Vector2(0, 0), 100, "res/paddle.png", RED);

	I(ClientApp)->SetIsInGame(true);
}

void GameScene::OnUpdate(float deltaTime)
{
	{
		std::lock_guard<std::mutex> lock(m_ballMutex);
		// Update for prediction
		//m_ball->Update(deltaTime);
	}

	{
		std::lock_guard<std::mutex> lock(m_paddleLeftMutex);
		// Update for prediction
		//m_paddleLeft->Update(deltaTime);
	}

	{
		std::lock_guard<std::mutex> lock(m_paddleRightMutex);
		// Update for prediction
		//m_paddleRight->Update(deltaTime);
	}
}

void GameScene::OnRender()
{
}

void GameScene::OnUninitialize()
{
	I(ClientApp)->SetIsInGame(false);
}

void GameScene::OnGameDataReplication(const ReplicationData& replicationData)
{
	{
		std::scoped_lock lock(m_ballMutex, m_paddleLeftMutex, m_paddleRightMutex);
		m_ball->SetPosition(replicationData.BallPosition);
		m_ball->SetDirection(replicationData.BallDirection);
		m_ball->SetSpeed(replicationData.BallSpeed);

		m_paddleLeft->SetPosition(replicationData.PaddleLeftPosition);
		m_paddleLeft->SetDirection(replicationData.PaddleLeftDirection);

		m_paddleRight->SetPosition(replicationData.PaddleRightPosition);
		m_paddleRight->SetDirection(replicationData.PaddleRightDirection);
	}
}

void GameScene::OnScoreReplication(int score, int healthPoints)
{
	{
		std::scoped_lock lock(m_scoreMutex, m_healthPointsMutex);
		m_score->SetText(std::to_string(score));
	}
}
