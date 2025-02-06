#include "ServerApp.h"
#include "Ball.h"
#include "Paddle.h"
#include "CommonGameConsts.h"

#include <iostream>
#include <cmath>
#include <ctime>

ServerApp::ServerApp()
	: m_healthPoints(3)
	, m_udpServer(new ServerHandler())
{
}

ServerApp::~ServerApp()
{
}

void ServerApp::Run()
{
	srand(static_cast<unsigned int>(time(NULL)));

	// TODO: Get server IP
	// Server has id 0
	RegisterUser("Server", SERVER_PORT, "127.0.0.1");

	InitNetwork();

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void ServerApp::SendMessage(const char* address, u_short port, Message& message)
{
	std::string finalMessage = message.toString(); // Ensure thread safety
	std::vector<char> messageBuffer(finalMessage.begin(), finalMessage.end());

	if (!m_udpServer.SendTo(address, port, messageBuffer.data(), static_cast<int>(messageBuffer.size())))
	{
		std::cerr << "Failed to send message to the client." << std::endl;
	}
}

void ServerApp::InitNetwork()
{
	if (!m_udpServer.Init())
	{
		std::cerr << "Server network initialization failed." << std::endl;
		return;
	}

	if (!m_udpServer.CreateSocket())
	{
		std::cerr << "Server socket creation failed." << std::endl;
		return;
	}

	if (!m_udpServer.BindSocket(SERVER_PORT))
	{
		std::cerr << "Server socket bind failed." << std::endl;
		return;
	}

	std::cout << "Server Initialized successfully on port " << m_udpServer.GetLocalPort() << std::endl;
	m_udpServer.StartListening();
}

void ServerApp::Update(float deltaTime)
{
}


int ServerApp::RegisterUser(const std::string& name, u_short port, const std::string& address)
{
	int userId = m_userIdCounter.fetch_add(1, std::memory_order_relaxed);

	{
		std::lock_guard<std::mutex> lock(m_usersMutex);
		m_users.insert({ userId, new User(userId, name, port, address) });
	}

	std::cout << "User " << name << " connected" << std::endl;
	return userId;
}

void ServerApp::UnregisterUser(int id)
{
	{
		std::lock_guard<std::mutex> lock(m_usersMutex);
		User* user = GetUser(id);
		if (user)
		{
			m_users.erase(id);
		}
	}
}

int ServerApp::CreateLobby(int userId, const std::string& name)
{
	User* user = GetUser(userId);
	if (!user)
	{
		std::cerr << "User not found." << std::endl;
		return -1;
	}

	int lobbyId = m_lobbyIdCounter.fetch_add(1, std::memory_order_relaxed);

	{
		std::lock_guard<std::mutex> lock(m_lobbiesMutex);
		std::lock_guard<std::mutex> lock2(m_usersMutex);

		m_lobbies.insert({ lobbyId, new Lobby(lobbyId, name) });
		user->SetIsOwner(true);
	}

	return lobbyId;
}

void ServerApp::RemoveLobby(int lobbyId)
{
	{
		std::lock_guard<std::mutex> lock(m_lobbiesMutex);
		Lobby* lobby = GetLobby(lobbyId);
		if (lobby)
		{
			m_lobbies.erase(lobbyId);
		}
	}
}

void ServerApp::JoinLobby(int userId, int lobbyId)
{
	{
		std::lock_guard<std::mutex> lock(m_lobbiesMutex);
		std::lock_guard<std::mutex> lock2(m_usersMutex);

		Lobby* lobby = GetLobby(lobbyId);
		User* user = GetUser(userId);

		json responseData;
		bool joined = false;

		if (!user)
		{
			std::cerr << "User not found." << std::endl;
			return;
		}

		if (!lobby)
		{
			responseData = {
				{"canJoin", joined},
				{"message", "Lobby does not exist."}
			};
		}

		else if (lobby->IsFull())
		{
			responseData = {
				{"canJoin", joined},
				{"message", "Lobby is full."}
			};
		}

		else
		{
			joined = true;
			responseData = {
				{"canJoin", joined},
				{"message", ""}
			};
		}

		Message response = Message::CreateMessage(MessageType::JOIN_LOBBY_RESPONSE, responseData);
		response.content["id"] = userId;

		SendMessage(user->GetPublicIpAddress().c_str(), user->GetPort(), response);

		if (joined)
		{
			lobby->AddUser(user);
			user->SetLobby(lobby);
		}
	}
}

void ServerApp::LeaveLobby(int userId)
{
	{
		std::lock_guard<std::mutex> lock(m_lobbiesMutex);
		std::lock_guard<std::mutex> lock2(m_usersMutex);

		User* user = GetUser(userId);
		Lobby* lobby = user->GetLobby();

		if (user && lobby)
		{
			lobby->RemoveUser(user->GetId());
			user->SetLobby(nullptr);

			if (lobby->IsEmpty())
			{
				RemoveLobby(lobby->GetId());
			}
		}
	}
}

void ServerApp::StartLobbyByOwner(int userId)
{
	User* user = GetUser(userId);
	if (user && user->IsOwner())
	{
		Lobby* lobby = user->GetLobby();
		if (lobby)
		{
			lobby->StartGame();
		}
	}
}

void ServerHandler::HandleMessage(const Message& message)
{
	json content = message.content;
	json data = content["data"];
	int userId = content["id"];

	MessageType type = content["messageType"];
	switch (type)
	{
	case MessageType::CONNECT:
	{
		std::string name = data["name"];
		u_short port = data["port"];
		std::string address = data["address"];

		std::cout << "Client address: " << address << std::endl;

		int attributedUserId = I(ServerApp)->RegisterUser(name, port, address);

		// Send User Id
		Message response = Message::CreateMessage(MessageType::CONNECT, {});
		response.content["id"] = attributedUserId;

		I(ServerApp)->SendMessage(address.c_str(), port, response);
	}
	break;
	case MessageType::LOBBIES_LIST:
	{
		u_short port = data["port"];
		std::string address = data["address"];

		std::cout << "Client address: " << address << std::endl;

		// Send User Id
		Message response = Message::CreateMessage(MessageType::LOBBIES_LIST, {});
		response.content["id"] = userId;

		json jsonLobbies = json::array();

		for (auto lobby : I(ServerApp)->GetLobbies())
		{
			json lStruct = json::object();
			lStruct["name"] = lobby.second->GetName();
			lStruct["id"] = lobby.second->GetId();
			lStruct["capacity"] = lobby.second->GetCapactity();
			lStruct["userAmount"] = lobby.second->GetUserAmount();
			jsonLobbies.push_back(lStruct);
		}

		response.content["data"] = jsonLobbies;

		I(ServerApp)->SendMessage(address.c_str(), port, response);
	}
	break;
	case MessageType::DISCONNECT:
	{
		I(ServerApp)->UnregisterUser(userId);
	}
	break;
	case MessageType::CREATE_LOBBY:
	{
		std::string name = data["name"];

		ServerApp* app = I(ServerApp);
		int lobbyId = app->CreateLobby(userId, name);
		app->JoinLobby(userId, lobbyId);
	}
	break;
	case MessageType::JOIN_LOBBY:
	{
		int lobbyId = data["lobbyId"];
		I(ServerApp)->JoinLobby(userId, lobbyId);
	}
	break;
	case MessageType::LEAVE_LOBBY:
	{
		I(ServerApp)->LeaveLobby(userId);
	}
	break;
	case MessageType::START_GAME:
	{
		I(ServerApp)->StartLobbyByOwner(userId);
	}
	break;
	case MessageType::PLAY:
	{
		int dirY = data["movedPaddle"]["dirY"];
		// Get lobby with user Id
		// Get user paddle
		// Move paddle
	}
	break;
	default:
	{

	}
	break;
	}
}

User* ServerApp::GetUser(int id)
{
	auto it = m_users.find(id);
	if (it != m_users.end())
	{
		return it->second;
	}
	return nullptr;
}

Lobby* ServerApp::GetLobby(int id)
{
	auto it = m_lobbies.find(id);
	if (it != m_lobbies.end())
	{
		return it->second;
	}
	return nullptr;
}