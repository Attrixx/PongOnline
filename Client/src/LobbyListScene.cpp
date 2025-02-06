#include "LobbyListScene.h"
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
		auto onLobbyButtonClicked = [&]() {
			Message message = Message::CreateMessage(MessageType::JOIN_LOBBY, {
				{"lobbyId", lobbies[i].id}
				});
			message.content["id"] = I(ClientApp)->GetClientId();
			I(ClientApp)->SendMessage(message);
			};
		lobbyButton->BindOnClickFunction(onLobbyButtonClicked);
	}
}

void LobbyListScene::OnInitialize()
{
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
	// Load the game scene if the player can join the lobby
	if (canJoin)
	{
		// Load LobbyScene
	}
	else
	{
		std::cerr << message << std::endl;
	}
}
