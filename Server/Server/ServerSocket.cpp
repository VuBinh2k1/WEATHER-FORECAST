#include "stdafx.h"
#include "ServerSocket.h"
#include "Tokenizer.h"
#include "csv.h"
#include "sha256.h"

#define PATH_ACCOUNT "..\\Data\\account.csv"

/// Thread function /////////////////////////////////////////////////////////
unsigned int __stdcall ServAccept(void* data) {
	SOCKET* server = (SOCKET*)data;
	SOCKET Server = *server, Client;

	while (Client = accept(Server, NULL, NULL)) {
		if (sock::ServerSocket::Status == SHUTDOWN) {
			return 0;
		}
		if (Client == INVALID_SOCKET) {
			//sock::newLog("invalid client socket ", GetLastError());
			return 0;
		}

		_beginthreadex(0, 0, ServClient, (void*)&Client, 0, 0);
	}

	return 0;
}

unsigned int __stdcall ServClient(void* data) {
	SOCKET* client = (SOCKET*)data;
	SOCKET Client = *client;
	DWORD ClientID = GetCurrentThreadId();
	sock::newLog("Client connected: ", ClientID);

	/// Login...
	do {
		if (sock::ServerSocket::Status == SHUTDOWN || !sock::QConnect(Client)) {
			sock::newLog("Client disconnected: ", ClientID);
			closesocket(Client);
			return 0;
		}
	} while (!sock::QLogin(Client));

	/// 
	do {
		if (sock::ServerSocket::Status == SHUTDOWN || !sock::QConnect(Client)) {
			sock::newLog("Client disconnected: ", ClientID);
			closesocket(Client);
			return 0;
		}
	} while (1);

	closesocket(Client);
	return 0;
}

/// Class /////////////////////////////////////////////////////////
int sock::ServerSocket::Status = SHUTDOWN;
bool sock::ServerSocket::newLog = 0;

sock::ServerSocket::ServerSocket() {
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8612);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");


	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {	//2.2
		sock::newLog("WSA startup failed");
		return;
	}

}

void sock::ServerSocket::Start() {
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		sock::newLog("Invalid socket");
		return;
	}

	if (bind(sock, (sockaddr*)&addr, sizeof(sockaddr_in))) {
		sock::newLog("bind failed ", GetLastError());
		return;
	}

	if (listen(sock, SOMAXCONN)) {
		sock::newLog("iResult failed ", GetLastError());
		return;
	}

	sock::newLog("Server Start!");
	Status = RUNNING;
	_beginthreadex(0, 0, ServAccept, (void*)&sock, 0, 0);
}

void sock::ServerSocket::End() {
	sock::newLog("Server End!");
	Status = SHUTDOWN;
	closesocket(sock);
	sock = INVALID_SOCKET;
}

/// Function /////////////////////////////////////////////////////////
void sock::newLog(const char* text, int num) {
	std::ofstream log($Log, std::ios::app);

	std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);

	log << "[" << now->tm_year + 1900 << "/";
	if (now->tm_mon < 9)
		log << "0" << now->tm_mon + 1 << "/";
	else log << now->tm_mon + 1 << "/";
	if (now->tm_mday < 10)
		log << "0" << now->tm_mday << "/";
	else log << now->tm_mday << "][";

	if (now->tm_hour < 10) 
		log << "0" << now->tm_hour << ":";
	else log << now->tm_hour << ":";
	if (now->tm_min < 10) 
		log << "0" << now->tm_min << ":";
	else log << now->tm_min << ":";
	if (now->tm_sec < 10)
		log << "0" << now->tm_sec << "]: ";
	else log << now->tm_sec << "]: ";

	log << text;
	if (num != -1) {
		log << num;
	}
	
	log << "\n";
	log.close();

	sock::ServerSocket::newLog = 1;
}

bool sock::is(const char* input, const char* cmd, int flags) {
	if (flags == 1) return strcmp(input, cmd);
	if (strlen(input) < strlen(cmd)) return 0;

	int n = strlen(cmd);
	if (input[n] != '\0' && input[n] != '\n' && input[n] != '\t') return 0;

	for (int i = 0; i < strlen(cmd); ++i) 
		if (input[i] != cmd[i]) return 0;
	
	return 1;
}

void sock::Send(SOCKET& s, const char* Msg, int flags) {
	int MsgSize = strlen(Msg);
	send(s, (char*)&MsgSize, sizeof(int), flags);
	send(s, Msg, MsgSize, flags);
}

void sock::Recv(SOCKET& s, char*& buff, int flags) {
	int MsgSize = 0;
	recv(s, (char*)&MsgSize, sizeof(int), flags);

	buff = new char[MsgSize + 1];
	recv(s, buff, MsgSize, flags);

	buff[MsgSize] = '\0';
}

bool sock::Login(const char* data, int& Position) {
	vector<string> vdata = Tokenizer::split(data, SEP);
	if (vdata.size() != 3) return 0;

	if (!file::exists(PATH_ACCOUNT)) return 0;
	file::csv dataAccount(PATH_ACCOUNT);

	//SHA256 funtion
	string sh256password = sha256(vdata[2]);

	file::csv::line* puser = nullptr;

	if (puser = file::find(dataAccount, vdata[1].c_str(), sh256password.c_str())) {

		Position = is(puser->pdata[3], "admin");
		return 1;
	}

	if (puser = file::find(dataAccount, vdata[1].c_str(), "0")) {
		//Password reseted, confirm new password

		Position = is(puser->pdata[3], "admin");
		return 1;
	}

	return 0;
}

bool sock::QConnect(SOCKET& s) {
	char* tmp = nullptr;

	Send(s, "CONNECT ?");
	Recv(s, tmp);

	bool status = is(tmp, "YES!");
	delete tmp;

	return status;
}

bool sock::QLogin(SOCKET& s) {
	char* tmp = nullptr;
	int Position = 0;	//0: user, 1: admin

	Send(s, "USER");
	Recv(s, tmp);

	if (is(tmp, "LOG")) {
		printf("Login request: ");
		if (Login(tmp, Position)) {
			if (Position) {
				printf("ADMIN success!\n");
				Send(s, "LOGIN_SUCCESS\tADMIN");
			}
			else {
				printf("USER success!\n");
				Send(s, "LOGIN_SUCCESS\tUSER");
			}
			delete tmp;
			return 1;
		}
		
		printf("failed!\n");
		delete tmp;
		return 0;
	}
	if (is(tmp, "REG")) {
		// password unconfirm, username is used
		printf("Register request: ");


		printf("failed!\n");
		delete tmp;
		return 0;
	}

	printf("%s\n", tmp);
	delete tmp;
	return 0;
}

int sock::Handle(SOCKET& s) {
	char* tmp = nullptr;
	int Position = 0;	//0: user, 1: admin

	Send(s, "WHAT ?");
	Recv(s, tmp);

	return 0;
}

