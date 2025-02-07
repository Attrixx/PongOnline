#pragma once
#include "Scene.h"
#include "vector"
#include "iostream"

class LobbyScene : public Scene
{
	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnUninitialize() override;

public:
	void RefreshPlayers(std::vector<std::string> playerNames);

private:
	std::vector<class UIElement*> m_playerNames;
};
