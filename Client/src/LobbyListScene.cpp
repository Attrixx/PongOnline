#include "LobbyListScene.h"

#include <iostream>

void LobbyListScene::InitLobbiesList(std::vector<LobbyStruct> lobbies)
{
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
