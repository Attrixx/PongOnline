#include "InputText.h"
#include "GameConsts.h"

#include <iostream>

InputText::InputText()
	: m_text("")
	, m_font(GetFontDefault())
	, m_fontSize(FONT_SIZE)
	, m_textColor(TEXT_COLOR)
	, m_backgroundColor(UI_ELEMENT_BACKGROUND_COLOR)
	, m_hoverColor(UI_ELEMENT_HOVER_COLOR)
	, m_borderColor(UI_ELEMENT_BORDER_COLOR)
	, m_borderSize(UI_ELEMENT_BORDER_SIZE)
	, m_focusColor(UI_ELEMENT_FOCUS_COLOR)
	, m_rectangle(Rectangle(0, 0, INPUT_WIDTH, INPUT_HEIGHT))
	, m_isMouseHovering(false)
	, m_hasFocus(false)
{
}

InputText::InputText(const Vector2& position, const Vector2& size)
	: m_text("")
	, m_font(GetFontDefault())
	, m_fontSize(FONT_SIZE)
	, m_textColor(TEXT_COLOR)
	, m_backgroundColor(UI_ELEMENT_BACKGROUND_COLOR)
	, m_hoverColor(UI_ELEMENT_HOVER_COLOR)
	, m_borderColor(UI_ELEMENT_BORDER_COLOR)
	, m_borderSize(UI_ELEMENT_BORDER_SIZE)
	, m_focusColor(UI_ELEMENT_FOCUS_COLOR)
	, m_rectangle(Rectangle(position.x, position.y, size.x, size.y))
	, m_isMouseHovering(false)
	, m_hasFocus(false)
{
}

void InputText::SetFont(const char* filename)
{
	m_font = LoadFont(filename);
}

void InputText::SetFont(Font& font)
{
	m_font = font;
}

void InputText::SetPosition(const Vector2& position)
{
	m_rectangle.x = position.x;
	m_rectangle.y = position.y;
}

void InputText::SetSize(const Vector2& size)
{
	m_rectangle.width = size.x;
	m_rectangle.height = size.y;
}

void InputText::Update(float deltaTime)
{
	const Vector2 mousePosition = GetMousePosition();
	m_isMouseHovering = CheckCollisionPointRec(mousePosition, m_rectangle);
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		m_hasFocus = m_isMouseHovering;
	}

	if (m_hasFocus)
	{
		int keyPressed = GetCharPressed();
		while (keyPressed > 0)
		{
			m_text += keyPressed;

			keyPressed = GetCharPressed();
		}
	}

	if (IsKeyPressed(KEY_BACKSPACE))
	{
		if(!m_text.empty()) m_text.pop_back();
	}
}

void InputText::Render()
{
	DrawRectangleRec(m_rectangle, m_isMouseHovering ? m_hoverColor : m_hasFocus ? m_focusColor : m_backgroundColor);
	DrawRectangleLinesEx(m_rectangle, m_borderSize, m_borderColor);
	Vector2 textSize = MeasureTextEx(m_font, m_text.c_str(), m_fontSize, 1.f);
	std::string displayedText = m_text;
	while (textSize.x > m_rectangle.width - m_borderSize * 2.f - 4.f)
	{
		if (displayedText.empty()) break;

		displayedText.erase(0, 1);
		textSize = MeasureTextEx(m_font, displayedText.c_str(), m_fontSize, 1.f);
	}
	DrawTextEx(m_font, displayedText.c_str(), Vector2(m_rectangle.x + m_borderSize + 2.f, m_rectangle.y + m_rectangle.height * 0.5f - textSize.y * 0.5f), m_fontSize, 1.f, m_textColor);
}
