#pragma once

#include <chrono>

class TimeManager
{
public:
	TimeManager() : previousTime(std::chrono::high_resolution_clock::now()) {}
	~TimeManager() = default;

	void Update();
	float GetDeltaTime() const { return deltaTime; }

private:
	float deltaTime = 0.f;
	std::chrono::steady_clock::time_point previousTime;
};

