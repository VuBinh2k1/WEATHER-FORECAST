#include "stdafx.h"
#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include "ClientSocket.h"

using namespace sock;

int _tmain(int argc, _TCHAR* argv[]) {

	WSADATA wsaData;
	sockaddr_in addr;
	SOCKET sock, client;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(8612);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");


	if(WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		printf("WSA startup failed");
		return 0;
	}


	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock == INVALID_SOCKET) {
		printf("Invalid socket");
		return 0;
	}

	if(connect(sock, (SOCKADDR*)&addr, sizeof(sockaddr_in))) {
		printf("Connect failed %u",WSAGetLastError());
		return 0;
	}

	char ClientMsg[100];
	char* tmp = nullptr;

	while (1) {
		Recv(sock, tmp);

		///CHECK CONNECTION FROM SERVER
		if (is(tmp, "CONNECT ?")) {
			Send(sock, "YES!");
			continue;
		}
		///!CHECK CONNECTION FROM SERVER

		if (is(tmp, "USER")) {
			printf("Server: %s\n", tmp);
			delete tmp;

			printf("Client: ");
			std::cin.getline(ClientMsg, 100);
			if (is(ClientMsg, "EXIT")) break;

			if (is(ClientMsg, "LOG")) {
				char username[50], password[50];
				printf(" - USERNAME: ");
				std::cin.getline(username, 100);
				printf(" - PASSWORD: ");
				std::cin.getline(password, 100);

				strcat(ClientMsg, SEP);
				strcat(ClientMsg, username); 
				strcat(ClientMsg, SEP);
				strcat(ClientMsg, password);
			}

			if (is(ClientMsg, "REG")) {
				char username[50], password[50], confpass[50];
				printf(" - USERNAME: ");
				std::cin.getline(username, 100);
				printf(" - PASSWORD: ");
				std::cin.getline(password, 100);
				printf(" - CONFIRM PASSWORD: ");
				std::cin.getline(confpass, 100);

				strcat(ClientMsg, SEP);
				strcat(ClientMsg, username);
				strcat(ClientMsg, SEP);
				strcat(ClientMsg, password);
				strcat(ClientMsg, SEP);
				strcat(ClientMsg, confpass);
			}

			Send(sock, ClientMsg);
			continue;
		}

		if (is(tmp, "LOGIN_SUCCESS")) {
			printf("Server: Login success!\n");
			delete tmp;
			continue;
		}

		break;
	}

	closesocket(sock);
	return 0;
}