#pragma once

#include <vector>

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
};

