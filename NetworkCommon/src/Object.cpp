#include "pch.h"
#include "Object.h"
#include <vector>


Object::Object(Vector2 position, Vector2 direction, float speed)
	: m_position(position), m_direction(direction), m_speed(speed)
{
}
