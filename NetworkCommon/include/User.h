#pragma once

#include <string>
#include "winimports.h"

class User
{
public:
	User(int id, std::string& name, std::string& address, u_short port) : m_id(id), m_name(name), m_address(address), m_port(port) {}

	int GetId() const { return m_id; }
	const std::string& GetName() const { return m_name; }
	const std::string& GetAddress() const { return m_address; }
	u_short GetPort() const { return m_port; }

private:
	int m_id;
	std::string m_name;
	std::string m_address;
	u_short m_port;
};

