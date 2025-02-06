#include "Ball.h"
#include "CommonGameConsts.h"

void Ball::Render()
{
	DrawCircle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), BALL_RADIUS, m_color);
}
