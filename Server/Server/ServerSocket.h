#pragma once
#include "stdafx.h"
#include <winsock2.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib" )
#include <ctime>

#define EXIT		"EXIT"
#define SERV_LOGIN	"USER"
#define SERV_USER	"LOGIN_USER"
#define SERV_ADMIN	"LOGIN_ADMIN"
#define SERV_WAIT	"WHAT"
#define SERV_SHUT	"SHUTDOWN"
#define CLIE_LOGIN	"LOG"
#define CLIE_SIGNIN	"REG"

#define PATH_ACCOUNT	"..\\Data\\account.csv"
#define $Log			"..\\Data\\Log\\lastest.log"
#define SEP				"\t"		// use "\t" to split packet data

#define RUNNING		1
#define SHUTDOWN	0

unsigned int __stdcall ServClient(void* data);
unsigned int __stdcall ServAccept(void* data);

namespace sock {
	class ServerSocket {
		WSADATA wsaData;
		sockaddr_in addr;
		SOCKET sock, client;

	public: 
		static int Status;
		static bool newLog;

	public:
		ServerSocket();
		void Start();
		void End();
	};

	void newLog(const char* text, int num = -1);

	bool is(const char* input, const char* cmd = EXIT, int flags = 0);
	void Send(SOCKET& s, const char* Msg, int flags = 0);
	void Recv(SOCKET& s, char*& buff, int flags = 0);

	bool Login(const char* data, int& Position);
	bool Register(const char* data);

	/// Q:questtion(send) - A:answer(recv, send back)
	bool QConnect(SOCKET& s);
	bool QLogin(SOCKET& s);

	int Handle(SOCKET& s);
}



