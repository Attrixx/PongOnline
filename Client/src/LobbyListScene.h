#pragma once
#include "Scene.h"
class LobbyListScene : public Scene
{
	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnUninitialize() override;


};

