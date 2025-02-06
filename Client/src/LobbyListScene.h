#pragma once
#include "Scene.h"
#include <vector>
#include <iostream>

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
};

