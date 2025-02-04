#pragma once

#include "UIElement.h"

#include <string>
#include <raylib.h>

class InputText : public UIElement
{
public:

	InputText();
	InputText(const Vector2& position, const Vector2& size);
	virtual ~InputText() = default;

	void SetValue(const std::string& text) { m_text = text; };
	std::string GetValue() const { return m_text; };

	void SetFontSize(float size) { m_fontSize = size; };
	void SetFont(const char* filename);
	void SetFont(Font& font);
	void SetTextColor(const Color& color) { m_textColor = color; };

	void SetBackgroundColor(const Color& color) { m_backgroundColor = color; };
	void SetHoverColor(const Color& color) { m_hoverColor = color; };
	void SetBorderColor(const Color& color) { m_borderColor = color; };
	void SetBorderSize(float size) { m_borderSize = size; }
	void SetFocusColor(const Color& color) { m_focusColor = color; };

	void SetPosition(const Vector2& position);
	void SetSize(const Vector2& size);

	virtual void Update(float deltaTime) override;
	virtual void Render() override;

protected:

	std::string m_text;
	Font m_font;
	float m_fontSize;
	Color m_textColor;

	Color m_backgroundColor;
	Color m_hoverColor;
	Color m_borderColor;
	float m_borderSize;
	Color m_focusColor;

	Rectangle m_rectangle;

	bool m_isMouseHovering;
	bool m_hasFocus;

};

