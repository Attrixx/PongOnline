#pragma once

#include "Object.h"


class Paddle : public Object
{
public:

	Paddle();
	Paddle(const Vector2Float& position, const Vector2Float& direction, float speed, const Vector2Float& size);

	virtual void Update(float deltaTime) override;

	void SetSize(const Vector2Float& size) { m_size = size; }
	Vector2Float GetSize() const { return m_size; }

private:

	Vector2Float m_size;

};

