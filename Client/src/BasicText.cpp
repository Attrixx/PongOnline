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
	Vector2 textSize = GetTextSize();
	Vector2 targetPosition = m_position;
	switch (m_verticalAlignment)
	{
	case VerticalAlignment::CENTER:
	{
		targetPosition.y -= textSize.y * 0.5f;
	}
	break;
	case VerticalAlignment::BOTTOM:
	{
		targetPosition.y -= textSize.y;
	}
	break;
	default:
		break;
	}

	switch (m_horizontalAlignment)
	{
	case HorizontalAlignment::CENTER:
	{
		targetPosition.x -= textSize.x * 0.5f;
	}
	break;
	case HorizontalAlignment::RIGHT:
	{
		targetPosition.x -= textSize.x;
	}
	break;
	}
	DrawTextEx(m_font, m_text.c_str(), targetPosition, m_fontSize, 1.f, m_textColor);
}

void BasicText::SetFont(const char* filename)
{
	m_font = LoadFont(filename);
}

Vector2 BasicText::GetTextSize() const
{
	return MeasureTextEx(m_font, m_text.c_str(), m_fontSize, 1.f);
}
