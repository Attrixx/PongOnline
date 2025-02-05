#include <cstdlib>
#include <iostream>

#include "ClientApp.h"

int main()
{
	SetThreadDescription(GetCurrentThread(), L"Client Main");

	ClientApp app;
	app.Run();

	return EXIT_SUCCESS;
}
