#pragma once
class UIElement
{
public:

	UIElement();

	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

	void SetPendingDestroy(bool isPendingDestroy) { m_isPendingDestroy = isPendingDestroy; }
	bool IsPendingDestroy() const { return m_isPendingDestroy; }

protected:

	bool m_isPendingDestroy;

};
