#include "ServerC.h"

SOCKET connections[10];
const char endline = '\n';
SOCKET server_socket, client_socket;

void SendMessageToClient(int ID)
{
	char* buffer = new char[1024];
	for (;; Sleep(75))
	{
		memset(buffer, 0, sizeof(buffer));
		if (recv(connections[ID], buffer, 1024, NULL))
		{
			size_t size = strlen(buffer);
			buffer[size + 1] = endline;
			printf(buffer);
			for (int i = 0; i <= ID; i++)
			{
				send(connections[i], buffer, strlen(buffer), NULL);
			}

		}
	}
	delete buffer;
}

static BOOL CtrlHandler(DWORD fdwCtrlType)
{
	{
		switch (fdwCtrlType)
		{
		case CTRL_C_EVENT:
			send(connections[0], "Shutdown", strlen("Shutdown"), NULL);
			printf("Send before die...\n");
			closesocket(server_socket);
			exit(0);
		case CTRL_CLOSE_EVENT:
			send(connections[0], "Shutdown", strlen("Shutdown"), NULL);
			printf("Send before close...\n");
			closesocket(server_socket);
			exit(0);
		}
	}
}

Server::Server()
{
	WSAStartup(version, &data);

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(NULL, "10060", &hints, &result);

	server_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
}

Server::~Server()
{
	closesocket(server_socket);
	WSACleanup();
}

void Server::BindServer()
{
	check = bind(server_socket, result->ai_addr, result->ai_addrlen);
	if (check == SOCKET_ERROR)
	{
		PrintError();
	}
}

void Server::StartListen()
{
	listen(server_socket, SOMAXCONN);

	freeaddrinfo(result);

	printf("Start server...\n");
	char m_connect[] = "Connect...\n";
	for (;; Sleep(75))
	{
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
		if (client_socket = accept(server_socket, NULL, NULL))
		{
			printf("Client connect...\n");
			connections[client_count] = client_socket;
			send(connections[client_count], m_connect, strlen(m_connect), NULL);
			client_count++;
			hd = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendMessageToClient, (LPVOID)(client_count - 1), NULL, NULL);
			WaitForSingleObject(hd, INFINITE);
		}
	}


}

void Server::PrintError()
{
	printf("Error occured: %d", WSAGetLastError());
	exit(1);
}



