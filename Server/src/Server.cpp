#include <cstdlib>
#include <iostream>

#include "ServerApp.h"

int main()
{
	SetThreadDescription(GetCurrentThread(), L"Server Main");

	I(ServerApp)->Run();

	return EXIT_SUCCESS;
}