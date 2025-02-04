#pragma once 

#include <string>
#include <cmath>

class Vector2Float
{
public:
	Vector2Float(float x, float y) : x(x), y(y) {};
	Vector2Float() : x(0), y(0) {};

	float x;
	float y;

	Vector2Float Normalize()
	{
		float length = std::sqrt(x * x + y * y);
		return length > 0 ? Vector2Float(x / length, y / length) : Vector2Float(0, 0);
	}
};

class Object
{
public:
	Object();
	Object(const Vector2Float& position, const Vector2Float& direction, float speed);

	virtual void Update(float deltaTime);

	void SetPosition(const Vector2Float& position) { m_position = position; }
	Vector2Float GetPosition() const { return m_position; }
	void SetDirection(const Vector2Float& direction) { m_direction = direction; }
	void SetSpeed(float speed) { m_speed = speed; }

protected:
	Vector2Float m_position;
	Vector2Float m_direction;
	float m_speed;
};