#pragma once
#include "Scene.h"
#include "InputText.h"
#include "LobbyEntry.h"

#include <vector>
#include <iostream>
#include <string>

struct LobbyStruct
{
	std::string name;
	int id;
	int maxSize;
	int currentSize;
};

class LobbyListScene : public Scene
{
public:
	void InitLobbiesList(std::vector<LobbyStruct> lobbies);
	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnUninitialize() override;

	void OnJoinLobby(bool canJoin, const std::string& message);
private:
	InputText* m_inputLobbyName;
	std::vector<LobbyEntry*> m_lobbyEntries;
};

