#pragma once

#include "UIElement.h"

#include <string>
#include <raylib.h>
#include <functional>

class Button : public UIElement
{
public:

	Button();
	Button(const std::string& text, const Vector2& position, const Vector2& size);
	virtual ~Button() = default;

	void SetText(const std::string& text) { m_text = text; };
	void SetFontSize(float size) { m_fontSize = size; };
	void SetFont(const char* filename);
	void SetFont(Font& font);
	void SetTextColor(const Color& color) { m_textColor = color; };

	void SetBackgroundColor(const Color& color) { m_backgroundColor = color; };
	void SetHoverColor(const Color& color) { m_hoverColor = color; };

	void SetPosition(const Vector2& position);
	void SetSize(const Vector2& size);

	void BindOnClickFunction(const std::function<void()>& callback) { m_onClickFunction = callback; }

	void Update(float deltaTime);
	void Render();

protected:

	std::string m_text;
	float m_fontSize;
	Color m_textColor;
	Font m_font;

	Color m_backgroundColor;
	Color m_hoverColor;

	Rectangle m_rectangle;

	bool m_isMouseHovering;

	std::function<void()> m_onClickFunction;

};

