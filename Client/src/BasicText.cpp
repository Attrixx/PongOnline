#include "BasicText.h"
#include "ClientGameConsts.h"

BasicText::BasicText()
	: m_text("")
	, m_fontSize(FONT_SIZE)
	, m_font(GetFontDefault())
	, m_textColor(TEXT_COLOR)
	, m_horizontalAlignment(HorizontalAlignment::LEFT)
	, m_verticalAlignment(VerticalAlignment::TOP)
	, m_position(Vector2(0.f, 0.f))
{
}

BasicText::BasicText(const std::string& text, const Vector2& position, HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment)
	: m_text(text)
	, m_fontSize(FONT_SIZE)
	, m_font(GetFontDefault())
	, m_textColor(TEXT_COLOR)
	, m_horizontalAlignment(horizontalAlignment)
	, m_verticalAlignment(verticalAlignment)
	, m_position(position)
{
}

void BasicText::Update(float deltaTime)
{
}

void BasicText::Render()
{
	DrawTextEx(m_font, m_text.c_str(), m_position, m_fontSize, 1.f, m_textColor);
}

void BasicText::SetFont(const char* filename)
{
	m_font = LoadFont(filename);
}