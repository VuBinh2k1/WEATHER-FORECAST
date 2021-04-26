#include "stdafx.h"
#include <winsock2.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib" )
#include "ServerSocket.h"
#include "csv.h"

using namespace sock;

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

	while (client = accept(sock, NULL, NULL)) {
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

	char ServerMsg[100];
	char* tmp = nullptr;

	while (1) {
		///CHECK CONNECTION TO CLIENT
		Send(Client, "CONNECT ?");
		Recv(Client, tmp);
		if (!is(tmp, "YES!")) {
			printf("Client disconnected: %d\n", ClientID);
			delete tmp;
			break;
		}
		delete tmp;
		///!CHECK CONNECTION TO CLIENT


		Send(Client, "USER");
		Recv(Client, tmp);

		if (is(tmp, "LOG")) {
			printf("Login request: ");
			if (Login(tmp)) {
				//<LOGIN SUCCESS .. BREAK WHILE>
				printf("success!\n");
				Send(Client, "LOGIN_SUCCESS");

				delete tmp;
				break;		
			}
			printf("failed!\n");
			//continue;
		}
		if (is(tmp, "REG")) {
			printf("Register request: ");


			printf("failed!\n");
			//continue;
		}


		printf("%s \t %d\n", tmp, ClientID);
		delete tmp;
	}

	closesocket(Client);
	return 0;
}