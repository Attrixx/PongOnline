#include "TimeManager.h"

void TimeManager::Update()
{
	auto currentTime = std::chrono::high_resolution_clock::now();

	deltaTime = std::chrono::duration<float>(currentTime - previousTime).count();
	previousTime = currentTime;
}
