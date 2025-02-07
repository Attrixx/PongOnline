#pragma once

#include "UIElement.h"
#include "InputText.h"
#include "BasicText.h"
#include "Button.h"

class LobbyEntry : public UIElement
{
public:

	LobbyEntry();
	virtual ~LobbyEntry();

	void SetData(const std::string& partyName, int playerCount, int maxPlayers, int lobbyId);
	void SetPosition(const Vector2 position);
	void SetTextColor(const Color& color) { m_textColor = color; }

	Vector2 GetSize() const;

	virtual void Update(float deltaTime) override;
	virtual void Render() override;

protected:

	Vector2 m_position;

	Font m_font;
	float m_fontSize;
	Color m_textColor;

	std::string m_partyName;
	int m_playerCount;
	int m_maxPlayers;

	int m_lobbyId;

	Button* m_joinButton;

};
