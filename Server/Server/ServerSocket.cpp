#include "stdafx.h"
#include "ServerSocket.h"
#include "Tokenizer.h"
#include "csv.h"
#include "sha256.h"

#define PATH_ACCOUNT "..\\Data\\account.csv"

bool sock::is(const char* input, const char* cmd, int flags) {
	if (flags == 1) return strcmp(input, cmd);
	if (strlen(input) < strlen(cmd)) return 0;

	int n = strlen(cmd);
	if (input[n] != '\0' && input[n] != '\n' && input[n] != '\t') return 0;

	for (int i = 0; i < strlen(cmd); ++i) 
		if (input[i] != cmd[i]) return 0;
	
	return true;
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

bool sock::Login(const char* data) {
	vector<string> vdata = Tokenizer::split(data, SEP);
	if (vdata.size() != 3) return false;

	if (!file::exists(PATH_ACCOUNT)) return 0;
	file::csv dataAccount(PATH_ACCOUNT);

	//SHA256 funtion
	string sh256password = sha256(vdata[2]);

	if (file::find(dataAccount, vdata[1].c_str(), sh256password.c_str())) return true;

	if (file::find(dataAccount, vdata[1].c_str(), "0")) {
		//Password reseted, confirm new password

		return true;
	}

	return false;
}
