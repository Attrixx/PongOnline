#pragma once

#include "UIElement.h"

#include <string>
#include <raylib.h>

enum class HorizontalAlignment : unsigned char
{
	LEFT,
	CENTER,
	RIGHT
};

enum class VerticalAlignment : unsigned char
{
	TOP,
	CENTER,
	BOTTOM
};

class BasicText : public UIElement
{
public:

	BasicText();
	BasicText(const std::string& text, const Vector2& position, HorizontalAlignment horizontalAlignment = HorizontalAlignment::LEFT, VerticalAlignment verticalAlignment = VerticalAlignment::TOP);

	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	void SetText(const std::string& text) { m_text = text; };
	void SetFontSize(float size) { m_fontSize = size; };
	void SetFont(const char* filename);
	void SetFont(Font& font) { m_font = font; };
	void SetTextColor(const Color& color) { m_textColor = color; };

	void SetPosition(const Vector2& position) { m_position = position; };
	void SetHorizontalAlignment(HorizontalAlignment alignment) { m_horizontalAlignment = alignment; };
	void SetVerticalAlignment(VerticalAlignment alignment) { m_verticalAlignment = alignment; };

private:

	std::string m_text;
	float m_fontSize;
	Font m_font;
	Color m_textColor;

	HorizontalAlignment m_horizontalAlignment;
	VerticalAlignment m_verticalAlignment;

	Vector2 m_position;

};

