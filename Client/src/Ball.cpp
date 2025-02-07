#include "Ball.h"
#include "CommonGameConsts.h"

void Ball::Render()
{
	DrawCircle(static_cast<int>(m_position.x + BALL_RADIUS), static_cast<int>(m_position.y + BALL_RADIUS), BALL_RADIUS, m_color);
}
