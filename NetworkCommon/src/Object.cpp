#include "pch.h"
#include "Object.h"
#include <vector>


Object::Object()
{
}

Object::Object(Vector2Float position, Vector2Float direction, float speed)
	: m_position(position), m_direction(direction), m_speed(speed)
{
}
