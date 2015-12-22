#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>


class Client
{
public:
	Client::Client();
	Client::~Client();
	
	void ConnectToServer();
	void ChatMessages();

private:
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 2);
	SOCKET my_socket;
	WSANETWORKEVENTS hProcessEvent;
	struct addrinfo chints, *cresult;
	WSAEVENT event;
	char send_buf[1024], acc_buf[1024];
	int check;
};