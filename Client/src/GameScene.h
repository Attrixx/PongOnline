#pragma once

#include "Scene.h"

#include <raylib.h>
#include <mutex>

class Entity;

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

	void OnReplication(const ReplicationData& replicationData);

private:

	std::mutex m_ballMutex;
	std::mutex m_paddleLeftMutex;
	std::mutex m_paddleRightMutex;

	Entity* m_ball;
	Entity* m_paddleLeft;
	Entity* m_paddleRight;

};
