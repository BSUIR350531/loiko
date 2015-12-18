#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>

SOCKET Connect;
SOCKET* Connections;
SOCKET Listen;
char endline = '$';
int ClientCount = 0;
std::string str;

void SendMessageToClient(int ID)
{
	char* buffer = new char[1024];
	for (;; Sleep(75))
	{
		memset(buffer, 0, sizeof(buffer));
		if (recv(Connections[ID], buffer, 1024, NULL))
		{
			size_t size = strlen(buffer);
			buffer[size] = endline;
			printf(buffer);
			printf("\n");
			for (int i = 0; i <= ClientCount; i++)
			{
				send(Connections[i], buffer, strlen(buffer), NULL);
			}
		}
	}
	delete buffer;
}

int main()
{ 
	setlocale(LC_ALL, "russian");
	WSAData data;
	WORD version = MAKEWORD(2, 2);
	int res = WSAStartup(version, &data);
	if (res != 0)
	{
		return 0;
	}

	struct addrinfo hints;
	struct addrinfo * result;

	Connections = (SOCKET*)calloc(64, sizeof(SOCKET));
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(NULL, "10061", &hints, &result);

	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(Listen, result->ai_addr, result->ai_addrlen);
	listen(Listen, SOMAXCONN);

	freeaddrinfo(result);

	printf("Start server...");
	char m_connect[] = "Connect...";
	for (;; Sleep(75))
	{
		if (Connect = accept(Listen, NULL, NULL))
		{
			printf("Client connect...\n");
			Connections[ClientCount] = Connect;
			send(Connections[ClientCount], m_connect, strlen(m_connect), NULL);
			ClientCount++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendMessageToClient, (LPVOID)(ClientCount - 1), NULL, NULL);
	//		WaitForSingleObject(tr, INFINITE);
	//		printf("End thread");
		}
	}
	return 0;
}

