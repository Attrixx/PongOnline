#pragma once

#include "Scene.h"

#include <string>

class InputText;

class MainMenuScene : public Scene
{
public:

	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnUninitialize() override;

};
