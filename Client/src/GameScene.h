#pragma once

#include "Scene.h"
#include <raylib.h>

class Entity;

struct ReplicationData
{
	Vector2 BallPosition;
	Vector2 BallDirection;
	float BallSpeed;

	Vector2 PaddleLeftPosition;
	Vector2 PaddleLeftDirection;

	Vector2 PaddleRightPosition;
	Vector2 PaddleRightDirection;
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

	Entity* m_ball;
	Entity* m_paddleLeft;
	Entity* m_paddleRight;

};
