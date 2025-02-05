#pragma once

#include "Scene.h"

class InputText;

class MainMenuScene : public Scene
{
public:

	virtual void OnInitialize() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnUninitialize() override;

private:

	InputText* m_inputIp;
	InputText* m_inputUsername;

};
