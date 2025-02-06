#pragma once
#include "Object.h"
#include <raylib.h>

class Entity : public Object
{
public:
	Entity();

	void Init(Vector2 position, Vector2 direction, float speed, const char* spritePath, Color color);

	virtual void Update(float deltaTime);
	virtual void Render();

	void SetPendingDestroy(bool isPendingDestroy) { m_isPendingDestroy = isPendingDestroy; }
	bool IsPendingDestroy() const { return m_isPendingDestroy; }

private:
	bool m_isPendingDestroy;
	Texture2D m_texture;
protected:
	Color m_color;
};

