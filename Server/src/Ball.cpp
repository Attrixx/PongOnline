#include "Ball.h"
#include "Paddle.h"
#include "CommonGameConsts.h"
#include "ServerApp.h"
#include "Lobby.h"

Ball::Ball(Lobby* lobby)
	: m_radius(0.f)
	, m_lobby(lobby)
{
}

Ball::Ball(Lobby* lobby, const Vector2Float& position, const Vector2Float& direction, float speed, float radius)
	: m_radius(radius)
	, m_lobby(lobby)
{
	m_position = position;
	m_direction = direction;
	m_speed = speed;
}

void Ball::Update(float deltaTime)
{
	Object::Update(deltaTime);

	if (m_position.y < 0)
	{
		m_direction.y = abs(m_direction.y);
	}

	else if (m_position.y + m_radius > WINDOW_HEIGHT)
	{
		m_direction.y = -1.f * abs(m_direction.y);
	}

	if (m_position.x < -m_radius && m_lobby)
	{
		m_lobby->OnBallOutOfScreen(true);
	}
	else if (m_position.x > WINDOW_WIDTH && m_lobby)
	{
		m_lobby->OnBallOutOfScreen(false);
	}
}

void Ball::CheckCollision(Paddle* target)
{
	const Vector2Float targetPosition = target->GetPosition();
	const Vector2Float targetSize = target->GetSize();
	if (m_position.x < targetPosition.x + targetSize.x && m_position.x + (m_radius * 2.f) > targetPosition.x &&
		m_position.y < targetPosition.y + targetSize.y && m_position.y + (m_radius * 2.f) > targetPosition.y)
	{
		m_direction.x *= -1.f;
		m_speed *= BALL_COLLISION_SPEED_COEFF;
	}
}
