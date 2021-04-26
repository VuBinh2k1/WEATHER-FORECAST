#include "stdafx.h"
#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")


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

	int MsgSize = 0;
	char ClientMsg[100];
	char* tmp;

	int max = 5;
	while (strcmp(ClientMsg, "EXIT")) {
		recv(sock, (char*)&MsgSize, sizeof(int), 0);
		tmp = new char[MsgSize + 1];
		recv(sock, tmp, MsgSize, 0);
		tmp[MsgSize] = '\0';

		if (strcmp(tmp, "CONNECT ?\0") == 0) {
			strcpy(ClientMsg, "YES!");
		}
		else if (strcmp(tmp, "USER\0") == 0) {
			printf("Server: %s\n", tmp);

			printf("Client: ");
			std::cin.getline(ClientMsg, 100);
			if (strcmp(ClientMsg, "EXIT") == 0) break;
		}
		else {
			break;
		}

		MsgSize = strlen(ClientMsg);
		send(sock, (char*)&MsgSize, sizeof(int), 0);
		send(sock, ClientMsg, MsgSize, 0);
		delete tmp;
	}

	closesocket(sock);
	return 0;
}