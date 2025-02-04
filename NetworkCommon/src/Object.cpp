#include "pch.h"
#include "Object.h"
#include <vector>


Object::Object()
	: m_position(Vector2Float(0.f, 0.f))
	, m_direction(Vector2Float(0.f, 0.f))
	, m_speed(0.f)
{
}

Object::Object(const Vector2Float& position, const Vector2Float& direction, float speed)
	: m_position(position), m_direction(direction), m_speed(speed)
{
}

void Object::Update(float deltaTime)
{
	Vector2Float normalizedDirection = m_direction.Normalize();
	m_position.x += normalizedDirection.x * m_speed * deltaTime;
	m_position.y += normalizedDirection.y * m_speed * deltaTime;
}
