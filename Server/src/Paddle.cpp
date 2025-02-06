#include "Paddle.h"
#include "CommonGameConsts.h"

Paddle::Paddle()
	: m_size(Vector2Float(PADDLE_WIDTH, PADDLE_HEIGHT))
{
	m_speed = PADDLE_SPEED;
	m_direction = Vector2Float(0.f, 0.f);
}

Paddle::Paddle(const Vector2Float& position, const Vector2Float& direction, float speed, const Vector2Float& size)
	: m_size(size)
{
	m_position = position;
	m_direction = direction;
	m_speed = speed;
}

void Paddle::Update(float deltaTime)
{
	Object::Update(deltaTime);
	if (m_position.y < 0)
	{
		m_position.y = 0;
	}
	if (m_position.y + m_size.y > WINDOW_HEIGHT)
	{
		m_position.y = WINDOW_HEIGHT - m_size.y;
	}
}
