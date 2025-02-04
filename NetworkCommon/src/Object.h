#pragma once 

#include <string>

class Vector2
{
public:
	Vector2(float x, float y) : x(x), y(y) {};

	float x;
	float y;
};

class Object
{
public:
	Object(Vector2 position, Vector2 direction, float speed);

	static const Object& FromString(char* input);
	const std::string ToString(const Object& input);

private:
	Vector2 m_position;
	Vector2 m_direction;
	float m_speed;
};