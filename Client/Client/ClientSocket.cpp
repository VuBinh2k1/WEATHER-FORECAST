#include "stdafx.h"
#include "ClientSocket.h"

/// Thread function /////////////////////////////////////////////////////////
unsigned int __stdcall ClientRecv(void* data) {
	SOCKET* client = (SOCKET*)data;
	SOCKET Client = *client;

	char* tmp = nullptr;

	while (1) {
		sock::Recv(Client, tmp);
		printf("Server: %s\n", tmp);

		if (sock::is(tmp, SERV_SHUT)) {
			sock::ClientSocket::Status = DISCONNECT;
			delete tmp;
			return 0;
		}

		if (sock::is(tmp, "CONNECT ?")) {
			sock::Send(Client, "YES!");
			delete tmp;
			continue;
		}

		if (sock::is(tmp, SERV_LOGIN)) {
			sock::ClientSocket::cmd_to_gui = SERV_LOGIN;
			delete tmp;

			/// Waiting DATA from GUI
			while (sock::ClientSocket::cmd_from_gui.empty()) Sleep(500);

			std::string ClientMsg = sock::ClientSocket::cmd_from_gui;
			sock::ClientSocket::cmd_from_gui.clear();
			printf("Data: %s\n", ClientMsg.c_str());
			sock::Send(Client, ClientMsg.c_str());

			continue;
			/// LOGIN GUI
			//return ALogin(Client, tmp);
		}

		if (sock::is(tmp, SERV_USER) || sock::is(tmp, SERV_ADMIN)) {
			sock::ClientSocket::cmd_to_gui = tmp;
			delete tmp;
			continue;
		}

		if (sock::is(tmp, "WHAT")) {
			sock::ClientSocket::cmd_to_gui = tmp;

			/// Waiting DATA from GUI
			while (sock::ClientSocket::cmd_from_gui.empty()) Sleep(500);
			std::string ClientMsg = sock::ClientSocket::cmd_from_gui;
			sock::ClientSocket::cmd_from_gui.clear();

			Sleep(3000);

			sock::Send(Client, ClientMsg.c_str());
			continue;
			// USER:
			// LIST [date]
			// CITY <cityID>
			// 
			// ADMIN:
			// UPDATE_CITY <cityID> <cityName>
			// UPDATE_DATA <cityID> <date> <weather info>
		}

		break;
	}
}

/// Class /////////////////////////////////////////////////////////
int sock::ClientSocket::Status = DISCONNECT;
std::string sock::ClientSocket::cmd_to_gui = "";
std::string sock::ClientSocket::cmd_from_gui = "";

sock::ClientSocket::ClientSocket() {
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8612);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		printf("WSA startup failed");
		return;
	}

	Connect();
}

sock::ClientSocket::~ClientSocket() {
	closesocket(sock);
}

bool sock::ClientSocket::Connect() {
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		printf("Invalid socket");
		return 0;
	}

	if (connect(sock, (SOCKADDR*)&addr, sizeof(sockaddr_in))) {
		printf("Connect failed %u", WSAGetLastError());
		return 0;
	}
	
	Status = CONNECTING;
	_beginthreadex(0, 0, ClientRecv, (void*)&sock, 0, 0);
}

/// Function /////////////////////////////////////////////////////////
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

/// HANDLING FUNCTION
bool ALogin(SOCKET& s, char* data) {
	printf("Server: %s\n", data);
	delete data;

	char ClientMsg[100];

	printf("Client: ");

	///////////////////////////////////
	std::cin.getline(ClientMsg, 100);
	
	if (sock::is(ClientMsg, "EXIT")) return 0;

	if (sock::is(ClientMsg, "LOG")) {
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

	if (sock::is(ClientMsg, "REG")) {
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

	sock::Send(s, ClientMsg);
	return 1;
}
/// !HANDLING FUNCTION

int sock::Handle(SOCKET& s) {
	

	return 0;
}
