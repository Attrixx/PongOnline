#pragma once

#include <vector>

class App
{
public:
	void Run();

private:
	void HandleEvents();
	void Update(float deltaTime);
	void Render();
};

