#pragma comment(lib, "Ws2_32.lib")
#include "ClientC.h"




int main()
{
	Client cl;
	cl.ConnectToServer();
	cl.ChatMessages();

	return 0;

}


	
	