#pragma once

#include <unordered_map>
#include <string>

class User;

class Lobby
{
public:
	Lobby(int id, const std::string& inName) : lobbyId(id), name(inName) {}
	~Lobby();

	int GetId() const { return lobbyId; }
	const std::string& GetName() const { return name; }

	bool IsEmpty() const { return m_users.empty(); }

	void Rename(const std::string& inName) { name = inName; }
	
	void AddUser(User* user);
	void RemoveUser(int id);

private:
	int lobbyId;
	std::string name;

	std::unordered_map<int, User*> m_users;

};

