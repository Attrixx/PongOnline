#pragma once

#include <raylib.h>
#include <vector>

#include "UDPNetwork.h"
#include "Scene.h"

class ClientApp
{
public:
	ClientApp();
	virtual ~ClientApp();

	void Run();

private:
	// Window
	void HandleEvents();
	void Update(float deltaTime);
	void Render();

	// Networking
	void InitNetwork();
	template<typename T>
	void LoadScene();

private:
	UDPNetwork m_udpClient;

	Scene* m_loadedScene;
	Scene* m_newScene;

};

template<typename T>
void ClientApp::LoadScene()
{
	static_assert(std::is_base_of<Scene, T>::value, "T must be derived from Scene");

	if (m_loadedScene)
	{
		m_loadedScene->Uninitialize();
	}

	m_newScene = new T();
	m_newScene->Initialize();
}
