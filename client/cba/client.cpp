#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>


SOCKET Client;
STARTUPINFO sa;
PROCESS_INFORMATION pi;

int main()
{
	WSADATA wsaData;
	int res;
	int sw;
	char send_buf[1024] = "Hi";
	char acc_buf[1024];
	size_t size;
	int oi;


	struct addrinfo hints, *result;
	ZeroMemory(&sa, sizeof(sa));
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	sa.cb = sizeof(sa);

	res = WSAStartup(MAKEWORD(2, 2), &wsaData);

	getaddrinfo(NULL, "10061", &hints, &result);

	std::cout << "#####################" << std::endl;
	std::cout << "###Welcome to Chat###" << std::endl;
	std::cout << "#####################" << std::endl;

	std::cout << "Select the options: " << std::endl;
	std::cout << "1) Start Server" << std::endl;
	std::cout << "2) Connect as a client" << std::endl;
	std::cout << "Your option: ";
	std::cin >> sw;
	switch (sw)
	{
	case 1:

		if (!CreateProcess(TEXT("D:\\Kyrsach\\loiko\\client\\Debug\\server.exe"), NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &sa, &pi));
		{
			printf("Failed (%d)", GetLastError());
		}

	case 2:
		Client = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		connect(Client, result->ai_addr, int(result->ai_addrlen));
		for (;; Sleep(10))
		{
			memset(&acc_buf, 0, strlen(acc_buf));
			memset(&send_buf, 0, strlen(send_buf));
			recv(Client, acc_buf, 1024, NULL);
			size = strlen(acc_buf);
			for (int i = 0; i < size; i++)
			{
				if (acc_buf[i] == '$')
					printf("\n");
				else
					printf("%c", acc_buf[i]);
			}
			printf("\n");
			std::cout << "Enter the message: ";
			gets(send_buf);
			send(Client, send_buf, sizeof(send_buf), NULL);
		}
	}
}