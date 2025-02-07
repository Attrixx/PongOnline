#include "Button.h"
#include "ClientGameConsts.h"

#include <iostream>

Button::Button()
	: m_text("")
	, m_fontSize(FONT_SIZE)
	, m_textColor(BUTTON_TEXT_COLOR)
	, m_font(GetFontDefault())
	, m_backgroundColor(UI_ELEMENT_BACKGROUND_COLOR)
	, m_hoverColor(UI_ELEMENT_HOVER_COLOR)
	, m_rectangle(Rectangle(0, 0, BUTTON_WIDTH, BUTTON_HEIGHT))
	, m_isMouseHovering(false)
{
}

Button::Button(const std::string& text, const Vector2& position, const Vector2& size)
	: m_text(text)
	, m_fontSize(FONT_SIZE)
	, m_textColor(BUTTON_TEXT_COLOR)
	, m_font(GetFontDefault())
	, m_backgroundColor(UI_ELEMENT_BACKGROUND_COLOR)
	, m_hoverColor(UI_ELEMENT_HOVER_COLOR)
	, m_rectangle(Rectangle(position.x, position.y, size.x, size.y))
	, m_isMouseHovering(false)
{
}

void Button::SetFont(const char* filename)
{
	m_font = LoadFont(filename);
}

void Button::SetFont(Font& font)
{
	m_font = font;
}

void Button::SetPosition(const Vector2& position)
{
	m_rectangle.x = position.x;
	m_rectangle.y = position.y;
}

void Button::SetSize(const Vector2& size)
{
	m_rectangle.width = size.x;
	m_rectangle.height = size.y;
}

void Button::Update(float deltaTime)
{
	const Vector2 mousePosition = GetMousePosition();
	bool isMouseOnButton = CheckCollisionPointRec(mousePosition, m_rectangle);
	if (isMouseOnButton)
	{
		if (!m_isMouseHovering)
		{
			m_isMouseHovering = true;
			SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
		}
	}
	else if (m_isMouseHovering)
	{
		m_isMouseHovering = false;
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	}

	if (m_isMouseHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		m_onClickFunction();
	}
}

void Button::Render()
{
	DrawRectangleRec(m_rectangle, m_isMouseHovering ? m_hoverColor : m_backgroundColor);
	Vector2 textSize = MeasureTextEx(m_font, m_text.c_str(), m_fontSize, 1.f);
	DrawTextEx(m_font, m_text.c_str(), Vector2(m_rectangle.x + m_rectangle.width * 0.5f - textSize.x * 0.5f, m_rectangle.y + m_rectangle.height * 0.5f - textSize.y * 0.5f), m_fontSize, 1.f, m_textColor);
}
