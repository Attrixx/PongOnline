#pragma once

// This warning was triggered because Raylib is included statically
// Shouldn't cause any issues
#pragma warning(disable: 4098)

#include <raylib.h>
#include <vector>
#include <atomic>

#include "UDPNetwork.h"
#include "Scene.h"
#include "Singleton.h"

template<typename T>
concept IsScene = std::is_base_of<Scene, T>::value;

enum PaddleDirection
{
	NONE,
	UP,
	DOWN
};

class ClientHandler : public NetworkHandler
{
public:
	void HandleMessage(const Message& message) override;
};

class ClientApp : public Singleton<ClientApp>
{
	friend class Singleton<ClientApp>;

public:
	ClientApp();
	virtual ~ClientApp();

	void Run();

	template<IsScene T>
	void LoadScene();
	Scene* GetLoadedScene() const { return m_loadedScene; }

	void SetServerHostname(const std::string& address) { m_serverHostname = address; }
	void SetClientId(int clientId) { m_clientId = clientId; }
	int GetClientId() const { return m_clientId; }
	void SendMessage(Message& message);
	UDPNetwork& GetUdpClient() { return m_udpClient; }

private:
	// Window
	void HandleEvents();
	void Update(float deltaTime);
	void Render();

	// Networking
	void InitNetwork();

	void SetIsInGame(bool isInGame) { m_isInGame = isInGame; }

private:
	UDPNetwork m_udpClient;
	std::string m_serverHostname = "127.0.0.1";
	std::atomic_int m_clientId = -1;

	Scene* m_loadedScene;
	Scene* m_newScene;

	bool m_isInGame;
	PaddleDirection m_paddleDirection = NONE;

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
