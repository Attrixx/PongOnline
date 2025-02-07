#include "LobbyScene.h"
#include "Button.h"
#include "ClientGameConsts.h"
#include "CommonGameConsts.h"
#include "Message.h"
#include "ClientApp.h"
#include "LobbyListScene.h"

void LobbyScene::OnInitialize()
{
	Button* PlayButton = CreateUIElement<Button>();
	PlayButton->SetPosition({ WINDOW_WIDTH * 0.5f - BUTTON_WIDTH - 25.f, 75.f });
	PlayButton->SetText("Play");
	auto onPlayButtonClicked = [&]() {
		Message message = Message::CreateMessage(MessageType::START_GAME, {});
		message.content["id"] = I(ClientApp)->GetClientId();
		I(ClientApp)->SendMessage(message);
		};
	PlayButton->BindOnClickFunction(onPlayButtonClicked);


	Button* QuitButton = CreateUIElement<Button>();
	QuitButton->SetPosition({ WINDOW_WIDTH * 0.5f + BUTTON_WIDTH * 2, 75.f });
	QuitButton->SetText("Quit");
	auto OnQuitButtonClicked = [&]() {
		I(ClientApp)->LoadScene<LobbyListScene>();
		Message message = Message::CreateMessage(MessageType::LEAVE_LOBBY, {});
		message.content["id"] = I(ClientApp)->GetClientId();
		I(ClientApp)->SendMessage(message);
		};
	QuitButton->BindOnClickFunction(OnQuitButtonClicked);
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
	float yOffset = 0;
	for (std::string name : playerNames)
	{
		Button* PlayerButton = CreateUIElement<Button>();
		PlayerButton->SetPosition({ BUTTON_WIDTH * 2, 75.f + yOffset });
		PlayerButton->SetText(name);
		yOffset += BUTTON_HEIGHT * 1.5f;
	}
}
