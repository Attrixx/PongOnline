#include <cstdlib>
#include <iostream>

#include "ClientApp.h"

int main()
{
	SetThreadDescription(GetCurrentThread(), L"Client Main");

	I(ClientApp)->Run();

	return EXIT_SUCCESS;
}
