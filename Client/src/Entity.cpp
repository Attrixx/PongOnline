#include "Entity.h"

Entity::Entity()
	: m_isPendingDestroy(false), m_color(WHITE), m_texture(Texture2D())
{
}

void Entity::Init(Vector2 position, Vector2 direction, float speed, const char* spritePath, Color color)
{
	m_position = Vector2Float(position.x, position.y);
	m_direction = Vector2Float(direction.x, direction.y);
	m_speed = speed;
	m_color = color;
	//Image image = LoadImage(spritePath);
	//m_texture = LoadTextureFromImage(image);
}

void Entity::Update(float deltaTime)
{
}

void Entity::Render()
{
	DrawRectangle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), 25, 25, m_color);
	//DrawTexture(m_texture, static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_color);
}
