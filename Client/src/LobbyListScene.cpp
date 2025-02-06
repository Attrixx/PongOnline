#include "LobbyListScene.h"
#include "LobbyScene.h"
#include "CommonGameConsts.h"
#include "ClientGameConsts.h"
#include "Message.h"
#include "Button.h"
#include "ClientApp.h"
#include <iostream>

void LobbyListScene::InitLobbiesList(std::vector<LobbyStruct> lobbies)
{
	for (int i = 0; i < lobbies.size(); i++)
	{
		Button* lobbyButton = CreateUIElement<Button>();
		lobbyButton->SetPosition({ float(GetScreenWidth() / 2), (BUTTON_HEIGHT * 1.5f) * i });
		lobbyButton->SetText(lobbies[i].name + " " + std::to_string(lobbies[i].currentSize) + "/" + std::to_string(lobbies[i].maxSize));
		int lobbyId = lobbies[i].id;
		auto onLobbyButtonClicked = [&, lobbyId]() {
			Message message = Message::CreateMessage(MessageType::JOIN_LOBBY, {
				{"lobbyId", lobbyId}
				});
			message.content["id"] = I(ClientApp)->GetClientId();
			I(ClientApp)->SendMessage(message);
			};
		lobbyButton->BindOnClickFunction(onLobbyButtonClicked);
	}
}

void LobbyListScene::OnInitialize()
{
	m_inputLobbyName = CreateUIElement<InputText>();
	m_inputLobbyName->SetPosition({ float(GetScreenWidth() - BUTTON_WIDTH), float(GetScreenHeight() / 2 + BUTTON_HEIGHT * 2) });

	Button* createLobbyButton = CreateUIElement<Button>();
	createLobbyButton->SetPosition({ float(GetScreenWidth() - BUTTON_WIDTH), float(GetScreenHeight() / 2) });
	createLobbyButton->SetText("Create Lobby");
	auto onCreateLobbyButtonClicked = [&]() {
		Message message = Message::CreateMessage(MessageType::CREATE_LOBBY, {
			{"name", m_inputLobbyName->GetValue()}
			});
		message.content["id"] = I(ClientApp)->GetClientId();
		I(ClientApp)->SendMessage(message);
		};
	createLobbyButton->BindOnClickFunction(onCreateLobbyButtonClicked);
}

void LobbyListScene::OnUpdate(float deltaTime)
{
}

void LobbyListScene::OnRender()
{
}

void LobbyListScene::OnUninitialize()
{
}

void LobbyListScene::OnJoinLobby(bool canJoin, const std::string& message)
{
	if (canJoin)
	{
		I(ClientApp)->LoadScene<LobbyScene>();
	}
	else
	{
		std::cerr << message << std::endl;
	}
}
