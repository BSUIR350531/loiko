#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include <vector>


SOCKET Client, Server;
STARTUPINFO sa;
PROCESS_INFORMATION pi;
HANDLE th;
char acc_buf[256];
WSAEVENT NewEvent, NewEvent2, pEvents[10];
struct addrinfo hints, *result;

DWORD WINAPI UpServer()
{
	struct addrinfo hints_ser;
	struct addrinfo * result_ser;

	ZeroMemory(&hints_ser, sizeof(hints_ser));

	hints_ser.ai_family = AF_INET;
	hints_ser.ai_flags = AI_PASSIVE;
	hints_ser.ai_socktype = SOCK_STREAM;
	hints_ser.ai_protocol = IPPROTO_TCP;

	getaddrinfo(NULL, "10060", &hints_ser, &result_ser);

	Server = socket(result_ser->ai_family, result_ser->ai_socktype, result_ser->ai_protocol);
	if (Server == INVALID_SOCKET)
	{
		printf("Error create socket\n");
	}
	int bi = bind(Server, result_ser->ai_addr, result_ser->ai_addrlen);
	if (bi == SOCKET_ERROR)
	{
		printf("Error bind socket\n");
	}
	int li = listen(Server, SOMAXCONN);
	if (li == SOCKET_ERROR)
	{
		printf("Error listen socket\n");
	}
	closesocket(Client);
	Client = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	int co = connect(Client, result->ai_addr, int(result->ai_addrlen));
	if (co == SOCKET_ERROR)
	{
		printf("Error connecting socket: %d\n", WSAGetLastError());
	}

	return 0;
}

int main()
{
	WSADATA wsaData;
	FD_SET WriteSet;
	FD_SET ReadSet;
	int res;
	int sw;
	char send_buf[1024] = "Hi";
	char m_connect[] = "Connect...";
	size_t size;
	int oi;


	
	ZeroMemory(&sa, sizeof(sa));
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	sa.cb = sizeof(sa);

	res = WSAStartup(MAKEWORD(2, 2), &wsaData);

	getaddrinfo(NULL, "10060", &hints, &result);

	std::cout << "#####################" << std::endl;
	std::cout << "###Welcome to Chat###" << std::endl;
	std::cout << "#####################" << std::endl;

	NewEvent = WSACreateEvent();
	NewEvent2 = WSACreateEvent();
	WSANETWORKEVENTS hProcessEvent;
	WSANETWORKEVENTS hProcessEvent2;
	Client = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	connect(Client, result->ai_addr, int(result->ai_addrlen));
	WSAEventSelect(Client, NewEvent, FD_READ | FD_WRITE );
	WSAEventSelect(Server, NewEvent2, FD_CONNECT);
	pEvents[0] = NewEvent;
	pEvents[1] = NewEvent2;




	while (1)
	{
		WSAWaitForMultipleEvents(2, pEvents, FALSE, INFINITE, FALSE);
		WSAEnumNetworkEvents(Client, NewEvent, &hProcessEvent);
		WSAEnumNetworkEvents(Server, NewEvent2, &hProcessEvent2);

		if (hProcessEvent.lNetworkEvents & FD_READ)
		{
			memset(acc_buf, 0, sizeof(acc_buf));
			int bytes = recv(Client, acc_buf, 1024, FALSE);
			printf(acc_buf);
		}

		if (hProcessEvent2.lNetworkEvents & FD_CONNECT)
		{
			accept(Client, NULL, NULL);
			send(Client, m_connect, strlen(m_connect), NULL);
		}
		else
		{
			printf("Enter the message: ");
			gets(send_buf);
			res = send(Client, send_buf, 1024, NULL);
			if (res == SOCKET_ERROR && WSAGetLastError() == 10054)
			{
				int bytes = recv(Client, acc_buf, 1024, FALSE);
				if (bytes)
				{
					HANDLE hl = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UpServer, NULL, 0, NULL);
					WaitForSingleObject(hl, INFINITE);
				}
			}

		}
	}
}