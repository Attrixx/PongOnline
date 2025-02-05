#include "MainMenuScene.h"
#include "Button.h"
#include "InputText.h"
#include "Entity.h"
#include "ClientGameConsts.h"
#include "CommonGameConsts.h"
#include "Message.h"
#include "ClientApp.h"

#include <iostream>

void MainMenuScene::OnInitialize()
{
	InputText* inputUsername = CreateUIElement<InputText>();
	inputUsername->SetPosition({ WINDOW_WIDTH * 0.5f - INPUT_WIDTH - 25.f, 25.f });

	InputText* inputIP = CreateUIElement<InputText>();
	inputIP->SetPosition({ WINDOW_WIDTH * 0.5f + 25.f, 25.f });

	Button* buttonCreateGame = CreateUIElement<Button>();
	buttonCreateGame->SetPosition({ WINDOW_WIDTH * 0.5f - BUTTON_WIDTH - 25.f, 75.f });
	buttonCreateGame->SetText("Create game");
	auto onCreateGameButtonClicked = [&inputIP]() {
		Message message = Message::CreateMessage(MessageType::CONNECT, {
		{"name", "Client"},
		{"port", I(ClientApp)->GetUdpClient().GetLocalPort()}
			});
		I(ClientApp)->SetServerHostname(inputIP->GetValue());
		I(ClientApp)->SendMessage(message);
	};
	buttonCreateGame->BindOnClickFunction(onCreateGameButtonClicked);

	Button* buttonJoinGame = CreateUIElement<Button>();
	buttonJoinGame->SetPosition({ WINDOW_WIDTH * 0.5f + 25.f, 75.f });
	buttonJoinGame->SetText("Join game");
	auto onJoinGameButtonClicked = []() { std::cout << "JOIN GAME BUTTON CLICKED" << std::endl; };
	buttonJoinGame->BindOnClickFunction(onJoinGameButtonClicked);
}

void MainMenuScene::OnUpdate(float deltaTime)
{
}

void MainMenuScene::OnRender()
{
}

void MainMenuScene::OnUninitialize()
{
}
