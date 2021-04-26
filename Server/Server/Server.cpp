#include "stdafx.h"
#include <winsock2.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib" )

unsigned int __stdcall  ServClient(void *data);

int _tmain(int argc, _TCHAR* argv[]) {

	WSADATA wsaData;
	sockaddr_in addr;
	SOCKET sock, client;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(8612);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");


	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {	//2.2
		printf("WSA startup failed");
		return 0;
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		printf("Invalid socket");
		return 0;
	}

	if (bind(sock, (sockaddr*)&addr, sizeof(sockaddr_in))) {
		printf("bind failed %u", GetLastError());
		return 0;
	}

	if (listen(sock, SOMAXCONN)) {
		printf("iResult failed %u", GetLastError());
		return 0;
	}

	while (client = accept(sock, 0, 0)) {
		if (client == INVALID_SOCKET) {
			printf("invalid client socket", GetLastError());
			continue;
		}

		_beginthreadex(0, 0, ServClient, (void*)&client, 0, 0);
	}

	closesocket(sock);
	return 0;
}

unsigned int __stdcall ServClient(void* data) {
	SOCKET* client = (SOCKET*)data;
	SOCKET Client = *client;
	DWORD ClientID = GetCurrentThreadId();
	printf("Client connected: %d\n", ClientID);

	bool isLogin = 0;
	int MsgSize = 0;
	char ServerMsg[100];
	char* tmp;

	while (isLogin == FALSE) {
		///CHECK CONNECTION TO CLIENT
		MsgSize = strlen("CONNECT ?");
		send(Client, (char*)&MsgSize, sizeof(int), 0);
		send(Client, "CONNECT ?", MsgSize, 0);

		recv(Client, (char*)&MsgSize, sizeof(int), 0);
		tmp = new char[MsgSize + 1];
		recv(Client, tmp, MsgSize, 0);
		tmp[MsgSize] = '\0';

		if (strcmp(tmp, "YES!\0")) break;
		delete tmp;
		///!CHECK CONNECTION TO CLIENT

		strcpy(ServerMsg, "USER");
		MsgSize = strlen(ServerMsg);
		send(Client, (char*)&MsgSize, sizeof(int), 0);
		send(Client, ServerMsg, MsgSize, 0);

		recv(Client, (char*)&MsgSize, sizeof(int), 0);
		tmp = new char[MsgSize + 1];
		recv(Client, tmp, MsgSize, 0);
		tmp[MsgSize] = '\0';

		if (strcmp(tmp, "LOG\0") == 0) {

		}
		else if (strcmp(tmp, "REG\0") == 0) {
			
		}


		printf("%s \t %d\n", tmp, ClientID);
		delete tmp;
	}

	closesocket(Client);
	return 0;
}