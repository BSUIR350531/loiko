#pragma comment(lib, "Ws2_32.lib")
#include "ServerC.h"


int main()
{
	setlocale(LC_ALL, "russian");
	
	Server NewServer;
	
	NewServer.BindServer();
	NewServer.StartListen();
	
	return 0;
}

