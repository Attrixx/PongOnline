#pragma once

#include <list>

class Entity;
class UIElement;

template<typename T>
concept IsEntity = std::is_base_of<Entity, T>::value;

template<typename T>
concept IsUIElement = std::is_base_of<UIElement, T>::value;

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

	template<IsEntity T>
	T* CreateEntity();

	template<IsUIElement T>
	T* CreateUIElement();

protected:

	std::list<Entity*> m_entities;
	std::list<UIElement*> m_uiElements;

};

template<IsEntity T>
T* Scene::CreateEntity()
{
	T* newEntity = new T();

	m_entities.push_back(newEntity);

	return newEntity;
}

template<IsUIElement T>
T* Scene::CreateUIElement()
{
	T* newElement = new T();

	m_uiElements.push_back(newElement);

	return newElement;
}