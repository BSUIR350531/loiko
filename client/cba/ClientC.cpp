#include "ClientC.h"
#include "..\..\server\abc\ServerC.h"


DWORD WINAPI UpServer()
{
	printf("Server disconected...Transform into server");
	Sleep(5000);
	system("cls");
	Server Upserver;
	Upserver.BindServer();
	Upserver.StartListen();

	return 0;
}



Client::Client()
{
	WSAStartup(version, &wsaData);
	
	ZeroMemory(&chints, sizeof(chints));
	chints.ai_family = AF_INET;
	chints.ai_socktype = SOCK_STREAM;
	chints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(NULL, "10060", &chints, &cresult);

	event = WSACreateEvent();
}

Client::~Client()
{
	closesocket(my_socket); 
	WSACleanup();
}

void Client::ConnectToServer()
{
	my_socket = socket(cresult->ai_family, cresult->ai_socktype, cresult->ai_protocol);
	connect(my_socket, cresult->ai_addr, int(cresult->ai_addrlen));
}

void Client::ChatMessages()
{
	WSAEventSelect(my_socket, event, FD_READ | FD_WRITE);
	while (1)
	{
		WSAWaitForMultipleEvents(1, &event, FALSE, INFINITE, FALSE);
		WSAEnumNetworkEvents(my_socket, event, &hProcessEvent);

		if (hProcessEvent.lNetworkEvents & FD_READ)
		{
			memset(acc_buf, 0, sizeof(acc_buf));
			int bytes = recv(my_socket, acc_buf, 1024, FALSE);
			printf(acc_buf);
			printf("Enter the message: ");
			gets(send_buf);
			check = send(my_socket, send_buf, 1024, NULL);
			if (check == SOCKET_ERROR && WSAGetLastError() == 10054)
			{
				int bytes = recv(my_socket, acc_buf, 1024, FALSE);
				if (bytes)
				{
					HANDLE hl = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UpServer, NULL, 0, NULL);
					WaitForSingleObject(hl, INFINITE);
				}
				else
				{
					Sleep(10000);
					Client::ConnectToServer();
				}
			}
		}
	}
}