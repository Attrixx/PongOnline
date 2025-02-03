#include "ServerApp.h"
#include "UDPNetwork.h"

void ServerApp::Run()
{
	UDPNetwork udpServer;
	if (!udpServer.Init() || udpServer.CreateSocket() || udpServer.BindSocket(8888))
	{
		std::cerr << "Server network initialization failed." << std::endl;
		return;
	}
}
