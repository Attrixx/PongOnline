#pragma once

#include <raylib.h>

#include "UDPNetwork.h"

class ClientApp
{
public:
	void Run();

private:
	// Window
	void HandleEvents();
	void Update(float deltaTime);
	void Render();

	// Networking
	void InitNetwork();

private:
	UDPNetwork m_udpClient;
};

