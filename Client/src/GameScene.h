#pragma once

#include "Scene.h"
#include "Object.h"

#include <raylib.h>
#include <mutex>

class UIElement;
class BasicText;
class Entity;
class Ball;

struct ReplicationData
{
	Vector2Float BallPosition;
	Vector2Float BallDirection;
	float BallSpeed;

	Vector2Float PaddleLeftPosition;
	Vector2Float PaddleLeftDirection;

	Vector2Float PaddleRightPosition;
	Vector2Float PaddleRightDirection;
};

class GameScene : public Scene
{
public:

	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnUninitialize() override;

	void OnGameDataReplication(const ReplicationData& replicationData);
	void OnScoreReplication(int score, int healthPoints);

private:

	std::mutex m_scoreMutex;
	std::mutex m_healthPointsMutex;
	std::mutex m_ballMutex;
	std::mutex m_paddleLeftMutex;
	std::mutex m_paddleRightMutex;

	BasicText* m_score;
	BasicText* m_healthPoints;
	Ball* m_ball;
	Entity* m_paddleLeft;
	Entity* m_paddleRight;

};
