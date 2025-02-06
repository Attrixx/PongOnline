#include "Scene.h"
#include "Entity.h"
#include "UIElement.h"

void Scene::Initialize()
{
	OnInitialize();
}

void Scene::Update(float deltaTime)
{
	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}
	for (auto it = m_uiElements.begin(); it != m_uiElements.end(); ++it)
	{
		(*it)->Update(deltaTime);
	}

	OnUpdate(deltaTime);
}

void Scene::Render()
{
	for (auto it = m_entities.begin(); it != m_entities.end();)
	{
		if ((*it)->IsPendingDestroy())
		{
			it = m_entities.erase(it);
		}
		else
		{
			(*it)->Render();
			++it;
		}
	}
	for (auto it = m_uiElements.begin(); it != m_uiElements.end();)
	{
		if ((*it)->IsPendingDestroy())
		{
			it = m_uiElements.erase(it);
		}
		else
		{
			(*it)->Render();
			++it;
		}
	}

	OnRender();
}

void Scene::Uninitialize()
{
	for (auto it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		(*it)->SetPendingDestroy(true);
	}
	for (auto it = m_uiElements.begin(); it != m_uiElements.end(); ++it)
	{
		(*it)->SetPendingDestroy(true);
	}
	OnUninitialize();
}
