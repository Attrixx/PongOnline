#pragma once

#include <vector>

#include "Scene.h"

class App
{
public:
	App();
	virtual ~App();

	void Run();

private:
	void HandleEvents();
	void Update(float deltaTime);
	void Render();

	template<typename T>
	void LoadScene();

private:

	Scene* m_loadedScene;
	Scene* m_newScene;

};

template<typename T>
void App::LoadScene()
{
	static_assert(std::is_base_of<Scene, T>::value, "T must be derived from Scene");

	if (m_loadedScene)
	{
		m_loadedScene->Uninitialize();
	}

	m_newScene = new T();
	m_newScene->Initialize();
}
