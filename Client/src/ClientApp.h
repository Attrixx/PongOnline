#pragma once

#include <raylib.h>
#include <vector>

#include "UDPNetwork.h"
#include "Scene.h"

template<typename T>
concept IsScene = std::is_base_of<Scene, T>::value;

class ClientApp
{
public:
	ClientApp();
	virtual ~ClientApp();

	void Run();

private:
	// Window
	void Update(float deltaTime);
	void Render();

	// Networking
	void InitNetwork();
	template<IsScene T>
	void LoadScene();

private:
	UDPNetwork m_udpClient;

	Scene* m_loadedScene;
	Scene* m_newScene;

};

template<IsScene T>
void ClientApp::LoadScene()
{
	//static_assert(std::is_base_of<Scene, T>::value, "T must be derived from Scene");

	if (m_loadedScene)
	{
		m_loadedScene->Uninitialize();
	}

	m_newScene = new T();
	m_newScene->Initialize();
}
