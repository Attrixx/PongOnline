#pragma once 

#include <string>

class Vector2Float
{
public:
	Vector2Float(float x, float y) : x(x), y(y) {};

	float x;
	float y;
};

class Object
{
public:
	Object(Vector2Float position, Vector2Float direction, float speed);

private:
	Vector2Float m_position;
	Vector2Float m_direction;
	float m_speed;
};