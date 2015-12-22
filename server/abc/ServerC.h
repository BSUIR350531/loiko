#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>

class Server
{
public:
	Server::Server();
	Server::~Server();
	void BindServer();
	void StartListen();
	void PrintError();

private:
	WSAData data;
	WORD version = MAKEWORD(2, 2);
	HANDLE hd;
	struct addrinfo hints, * result;
	int client_count = 0;
	int check;


};