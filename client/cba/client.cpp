#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>


SOCKET Client;


int main()
{
	WSADATA wsaData;
	int res;
	char send_buf[1024] = "Hi";
	char acc_buf[1024];
	size_t size;
	


	struct addrinfo hints, *result;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	res = WSAStartup(MAKEWORD(2, 2), &wsaData);

	getaddrinfo(NULL, "10061", &hints, &result);

	
	Client = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	connect(Client, result->ai_addr, int(result->ai_addrlen));
	for (;; Sleep(10))
	{
		memset(&acc_buf, 0, strlen(acc_buf));
		memset(&send_buf, 0, strlen(send_buf));
		recv(Client, acc_buf, 1024 ,NULL);
		size = strlen(acc_buf);
		for (int i = 0; i < size; i++)
		{
			if (acc_buf[i] == '$')
				printf("\n");
			else
				printf("%c",acc_buf[i]);
		}
		printf("\n");
		std::cout << "Enter the message: ";
		gets(send_buf);
		send(Client, send_buf, sizeof(send_buf), NULL);
	}

}