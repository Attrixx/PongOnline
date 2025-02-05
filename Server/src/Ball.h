#pragma once

#include "Object.h"

class Paddle;
class Lobby;

class Ball : public Object
{
public:

	Ball(Lobby* lobby);
	Ball(Lobby* lobby, const Vector2Float& position, const Vector2Float& direction, float speed, float radius);

	virtual void Update(float deltaTime) override;

	void CheckCollision(Paddle* target);

	void SetRadius(float radius) { m_radius = radius; }

private:

	float m_radius;

	Lobby* m_lobby;

};

