#include "LobbyScene.h"
#include "Button.h"
#include "ClientGameConsts.h"
#include "CommonGameConsts.h"
#include "Message.h"
#include "ClientApp.h"
#include "LobbyListScene.h"
#include "BasicText.h"

void LobbyScene::OnInitialize()
{
	Button* PlayButton = CreateUIElement<Button>();
	PlayButton->SetPosition({ WINDOW_WIDTH * 0.5f - BUTTON_WIDTH - 25.f, WINDOW_HEIGHT - 75.f });
	PlayButton->SetText("Play");
	auto onPlayButtonClicked = [&]() {
		Message message = Message::CreateMessage(MessageType::START_GAME, {});
		message.content["id"] = I(ClientApp)->GetClientId();
		I(ClientApp)->SendMessage(message);
		};
	PlayButton->BindOnClickFunction(onPlayButtonClicked);


	Button* QuitButton = CreateUIElement<Button>();
	QuitButton->SetPosition({ WINDOW_WIDTH * 0.5f ,  WINDOW_HEIGHT - 75.f });
	QuitButton->SetText("Quit");
	auto OnQuitButtonClicked = [&]() {
		I(ClientApp)->LoadScene<LobbyListScene>();
		Message message = Message::CreateMessage(MessageType::LEAVE_LOBBY, {});
		message.content["id"] = I(ClientApp)->GetClientId();
		I(ClientApp)->SendMessage(message);
		};
	QuitButton->BindOnClickFunction(OnQuitButtonClicked);

	Message message = Message::CreateMessage(MessageType::LOBBY_PLAYERS, {});
	message.content["id"] = I(ClientApp)->GetClientId();
	I(ClientApp)->SendMessage(message);
}

void LobbyScene::OnUpdate(float deltaTime)
{
}

void LobbyScene::OnRender()
{
}

void LobbyScene::OnUninitialize()
{
}

void LobbyScene::RefreshPlayers(std::vector<std::string> playerNames)
{
	for (int i = 0; i < m_playerNames.size(); i++)
	{
		m_uiElements.erase(find(m_uiElements.begin(), m_uiElements.end(), m_playerNames[i]));
	}
	m_playerNames.clear();

	float yOffset = 0;
	for (std::string name : playerNames)
	{
		BasicText* player = CreateUIElement<BasicText>();
		player->SetPosition({ BUTTON_WIDTH * 2, 75.f + yOffset });
		player->SetText(name);
		yOffset += BUTTON_HEIGHT * 1.5f;
		m_playerNames.push_back(player);
	}
}
