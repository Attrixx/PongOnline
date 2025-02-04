#pragma once

#include <list>

class Entity;
class UIElement;

class Scene
{
public:

	void Initialize();
	void Update(float deltaTime);
	void Render();
	void Uninitialize();

	virtual void OnInitialize() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRender() = 0;
	virtual void OnUninitialize() = 0;

	template<typename T>
	T* CreateEntity();

	template<typename T>
	T* CreateUIElement();

protected:

	std::list<Entity*> m_entities;
	std::list<UIElement*> m_uiElements;

};

template<typename T>
T* Scene::CreateEntity()
{
	static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");

	T* newEntity = new T();

	m_entities.push_back(newEntity);

	return newEntity;
}

template<typename T>
T* Scene::CreateUIElement()
{
	static_assert(std::is_base_of<UIElement, T>::value, "T must be derived from UIElement");

	T* newElement = new T();

	m_uiElements.push_back(newElement);

	return newElement;
}