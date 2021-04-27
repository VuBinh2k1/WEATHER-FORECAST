#include "stdafx.h"
#include "ClientSocket.h"

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
	char* tmp = nullptr;

	Recv(s, tmp);

	if (is(tmp, "CONNECT ?")) {
		Send(s, "YES!");
		delete tmp;
		return 1;
	}

	if (is(tmp, "USER")) {
		/// LOGIN GUI
		return ALogin(s, tmp);
	}

	if (is(tmp, "LOGIN_SUCCESS")) {
		printf("Server: %s Login success!\n", tmp + 14);
		if (is(tmp + 14, "USER")) {
			/// PROGRAM GUI: NORMAL USER

		}

		if (is(tmp + 14, "ADMIN")) {
			/// PROGRAM GUI: ADMIN
		}
		delete tmp;
		return 1;
	}

	if (is(tmp, "WHAT")) {
		// USER:
		// LIST [date]
		// CITY <cityID>
		// 
		// ADMIN:
		// UPDATE_CITY <cityID> <cityName>
		// UPDATE_DATA <cityID> <date> <weather info>
		Send(s, "LIST");
	}

	return 0;
}
