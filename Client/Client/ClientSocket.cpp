#include "stdafx.h"
#include "ClientSocket.h"

/// Thread function /////////////////////////////////////////////////////////
unsigned int __stdcall ClientRecv(void* data) {
	SOCKET* client = (SOCKET*)data;
	SOCKET Client = *client;

	char* tmp = nullptr;

	sock::Send(Client, "ARE_U_SERVER_FORECAST");
	sock::Recv(Client, tmp);
	if (!sock::is(tmp, "YES!")) {
		sock::ClientSocket::Status = DISCONNECT;
		return 0;
	}

	while (1) {
		sock::Recv(Client, tmp);
		//printf("Server: %s\n", tmp);

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
			
			sock::Send(Client, ClientMsg.c_str());
			continue;
		}

		if (sock::is(tmp, SERV_USER) || sock::is(tmp, SERV_ADMIN)) {
			sock::ClientSocket::cmd_to_gui = tmp;
			delete tmp;
			continue;
		}

		if (sock::is(tmp, SERV_WAIT)) {
			sock::ClientSocket::cmd_to_gui = SERV_WAIT;

			/// Waiting DATA from GUI
			while (sock::ClientSocket::cmd_from_gui.empty()) Sleep(500);

			std::string ClientMsg = sock::ClientSocket::cmd_from_gui;
			sock::ClientSocket::cmd_from_gui.clear();

			sock::Send(Client, ClientMsg.c_str());
			continue;
			// ADMIN:
			// UPDATE_CITY <cityID> <cityName>
			// UPDATE_DATA <cityID> <date> <weather info>
		}

		if (sock::is(tmp, "CITY")) {
			sock::ClientSocket::cmd_to_gui = tmp;

			/// Time for GUI build data
			while (sock::ClientSocket::cmd_from_gui.empty()) Sleep(500);
			sock::ClientSocket::cmd_from_gui.clear();

			continue;
		}

		break;
	}

	sock::ClientSocket::Status = DISCONNECT;
	return 0;
}

/// Class /////////////////////////////////////////////////////////
int sock::ClientSocket::Status = DISCONNECT;
std::string sock::ClientSocket::serv_addr = $SERV_ADDR;
std::string sock::ClientSocket::cmd_to_gui = "";
std::string sock::ClientSocket::cmd_from_gui = "";

sock::ClientSocket::ClientSocket() {
	addr.sin_family = AF_INET;
	addr.sin_port = htons($SERV_PORT);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		printf("WSA startup failed\n");
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
		printf("Invalid socket\n");
		return 0;
	}

	addr.sin_addr.S_un.S_addr = inet_addr(serv_addr.c_str());

	if (connect(sock, (SOCKADDR*)&addr, sizeof(sockaddr_in))) {
		Status = DISCONNECT;
		printf("Connect failed %u\n", WSAGetLastError());
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
	printf("Client: %s\n", Msg);
}

void sock::Recv(SOCKET& s, char*& buff, int flags) {
	int MsgSize = 0;
	recv(s, (char*)&MsgSize, sizeof(int), flags);

	buff = new char[MsgSize + 1];
	recv(s, buff, MsgSize, flags);

	buff[MsgSize] = '\0';
	printf("Server: %s\n", buff);
}

int sock::Handle(SOCKET& s) {
	

	return 0;
}
