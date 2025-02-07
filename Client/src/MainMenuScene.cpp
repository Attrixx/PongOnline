#include "MainMenuScene.h"
#include "Button.h"
#include "InputText.h"
#include "BasicText.h"
#include "Entity.h"
#include "ClientGameConsts.h"
#include "CommonGameConsts.h"
#include "Message.h"
#include "ClientApp.h"

#include <iostream>

void MainMenuScene::OnInitialize()
{
	const float winCenterH = WINDOW_WIDTH * 0.5f;
	const float winCenterV = WINDOW_HEIGHT * 0.5f;

	BasicText* labelUsername = CreateUIElement<BasicText>();
	labelUsername->SetText("Username");
	labelUsername->SetPosition({ winCenterH - INPUT_WIDTH - 50.f, winCenterV - 60.f});
	labelUsername->SetTextColor(BLACK);
	labelUsername->SetVerticalAlignment(VerticalAlignment::BOTTOM);

	m_inputUsername = CreateUIElement<InputText>();
	m_inputUsername->SetPosition({ winCenterH - INPUT_WIDTH - 50.f, winCenterV - 60.f });

	BasicText* labelIP = CreateUIElement<BasicText>();
	labelIP->SetText("Server address");
	labelIP->SetPosition({ winCenterH + 50.f, winCenterV - 60.f});
	labelIP->SetTextColor(BLACK);
	labelIP->SetVerticalAlignment(VerticalAlignment::BOTTOM);

	m_inputIp = CreateUIElement<InputText>();
	m_inputIp->SetPosition({ winCenterH + 50.f, winCenterV - 60.f });

	Button* buttonCreateGame = CreateUIElement<Button>();
	buttonCreateGame->SetPosition({ winCenterH - BUTTON_WIDTH * 0.5f, winCenterV + 20.f });
	buttonCreateGame->SetText("Connect to server");
	auto onCreateGameButtonClicked = [&]() {
		Message message = Message::CreateMessage(MessageType::CONNECT, {
			{"name", m_inputUsername->GetValue()},
			{"port", I(ClientApp)->GetUdpClient().GetLocalPort()},
			{"address", I(ClientApp)->GetUdpClient().GetPublicIPAddress()}
		});
		I(ClientApp)->SetServerHostname(m_inputIp->GetValue());
		I(ClientApp)->SendMessage(message);
	};
	buttonCreateGame->BindOnClickFunction(onCreateGameButtonClicked);
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
