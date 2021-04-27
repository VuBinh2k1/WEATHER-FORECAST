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

	do {
		
	} while (Handle(sock));

	closesocket(sock);
	return 0;
}