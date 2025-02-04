#pragma once

#include <string>
#include "winimports.h"

class User
{
public:
	User(int id, const std::string& name, u_short port) : m_id(id), m_name(name), m_port(port) {}

	int GetId() const { return m_id; }
	const std::string& GetName() const { return m_name; }
	u_short GetPort() const { return m_port; }

private:
	int m_id;
	std::string m_name;
	u_short m_port;
};

