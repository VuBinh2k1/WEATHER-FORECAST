#pragma once
#include "stdafx.h"
#include <winsock.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib" )
#include <string>

#define EXIT		"EXIT"
#define SERV_LOGIN	"USER"
#define SERV_USER	"LOGIN_USER"
#define SERV_ADMIN	"LOGIN_ADMIN"
#define SERV_WAIT	"WHAT ?"
#define SERV_SHUT	"SHUTDOWN"

#define CONNECTING	1
#define DISCONNECT	0

#define SEP "\t"

unsigned int __stdcall ClientRecv(void* data);

namespace sock {
	class ClientSocket {
		WSADATA wsaData;
		sockaddr_in addr;
		SOCKET sock, client;

		

	public:
		static int Status;
		static std::string cmd_to_gui;
		static std::string cmd_from_gui;

	public:
		ClientSocket();
		~ClientSocket();

		bool Connect();
	};

	bool is(const char* input, const char* cmd = "EXIT", int flags = 0);
	void Send(SOCKET& s, const char* Msg, int flags = 0);
	void Recv(SOCKET& s, char*& buff, int flags = 0);

	/// Q:questtion(send) - A:answer(recv, send back)

	int Handle(SOCKET& s);
}