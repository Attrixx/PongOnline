#include "LobbyEntry.h"
#include "Message.h"
#include "ClientApp.h"
#include "ClientGameConsts.h"

#include <string>

LobbyEntry::LobbyEntry()
	: m_position({ 0.f, 0.f })
	, m_font(GetFontDefault())
	, m_fontSize(FONT_SIZE)
	, m_textColor(TEXT_COLOR)
	, m_partyName("DEFAULT PARTY NAME")
	, m_playerCount(0)
	, m_maxPlayers(0)
	, m_lobbyId(-1)
	, m_joinButton(nullptr)
{
	m_joinButton = new Button();
	m_joinButton->SetFont(m_font);
	m_joinButton->SetFontSize(m_fontSize);
	m_joinButton->SetText("Join");
	m_joinButton->SetSize({ 100.f, BUTTON_HEIGHT });

	auto onJoinButtonClicked = [&]() {
		Message message = Message::CreateMessage(MessageType::JOIN_LOBBY, {
			{"lobbyId", m_lobbyId}
			});
		message.content["id"] = I(ClientApp)->GetClientId();
		I(ClientApp)->SendMessage(message);
		};
	m_joinButton->BindOnClickFunction(onJoinButtonClicked);
}

LobbyEntry::~LobbyEntry()
{
	delete m_joinButton;
}

void LobbyEntry::SetData(const std::string& partyName, int playerCount, int maxPlayers, int lobbyId)
{
	m_partyName = partyName;
	m_playerCount = playerCount;
	m_maxPlayers = maxPlayers;
	m_lobbyId = lobbyId;
}

void LobbyEntry::SetPosition(const Vector2 position)
{
	m_position = position;
	m_joinButton->SetPosition({ m_position.x + 2.f * PARTY_ENTY_ABS_MARGIN, m_position.y });
}

Vector2 LobbyEntry::GetSize() const
{
	Vector2 textSize = MeasureTextEx(m_font, m_partyName.c_str(), m_fontSize, 1.f);
	return Vector2(
		2.f * PARTY_ENTY_ABS_MARGIN + 100.f,
		BUTTON_HEIGHT > textSize.y ? BUTTON_HEIGHT : textSize.y
	);
}

void LobbyEntry::Update(float deltaTime)
{
	m_joinButton->Update(deltaTime);
}

void LobbyEntry::Render()
{
	Vector2 textSize = MeasureTextEx(m_font, m_partyName.c_str(), m_fontSize, 1.f);
	DrawTextEx(m_font, m_partyName.c_str(), { m_position.x, m_position.y + (BUTTON_HEIGHT - textSize.y) * 0.5f }, m_fontSize, 1.f, m_textColor);
	Vector2 targetPosition = m_position;
	targetPosition.x += PARTY_ENTY_ABS_MARGIN;
	targetPosition.y += (BUTTON_HEIGHT - textSize.y) * 0.5f;
	DrawTextEx(m_font, (std::to_string(m_playerCount) + "/" + std::to_string(m_maxPlayers)).c_str(), targetPosition, m_fontSize, 1.f, m_textColor);

	m_joinButton->Render();
}
