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
	m_inputUsername = CreateUIElement<InputText>();
	m_inputUsername->SetPosition({ WINDOW_WIDTH * 0.5f - INPUT_WIDTH - 25.f, 25.f });

	m_inputIp = CreateUIElement<InputText>();
	m_inputIp->SetPosition({ WINDOW_WIDTH * 0.5f + 25.f, 25.f });

	Button* buttonCreateGame = CreateUIElement<Button>();
	buttonCreateGame->SetPosition({ WINDOW_WIDTH * 0.5f - BUTTON_WIDTH - 25.f, 75.f });
	buttonCreateGame->SetText("Create game");
	auto onCreateGameButtonClicked = [&]() {
		Message message = Message::CreateMessage(MessageType::CONNECT, {
			{"name", "Client"},
			{"port", I(ClientApp)->GetUdpClient().GetLocalPort()},
			{"address", I(ClientApp)->GetUdpClient().GetPublicIPAddress()}
		});
		I(ClientApp)->SetServerHostname(m_inputIp->GetValue());
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
