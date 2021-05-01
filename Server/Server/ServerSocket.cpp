#include "stdafx.h"
#include "ServerSocket.h"
#include "Tokenizer.h"
#include "csv.h"
#include "sha256.h"

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

	/// Clent checking...
	char* tmp = nullptr;
	sock::Recv(Client, tmp);
	if (sock::is(tmp, "ARE_U_SERVER_FORECAST")) {
		sock::Send(Client, "YES!");
	}
	delete tmp;

	/// Login...
	do {
		if (!sock::QConnect(Client)) {
			sock::newLog("Client disconnected: ", ClientID);
			closesocket(Client);
			return 0;
		}
	} while (!sock::QLogin(Client));

	/// 
	do {
		if (!sock::QConnect(Client)) {
			sock::newLog("Client disconnected: ", ClientID);
			closesocket(Client);
			return 0;
		}
	} while (sock::Handle(Client));

	sock::newLog("Client disconnected: ", ClientID);
	closesocket(Client);
	return 0;
}

/// Class /////////////////////////////////////////////////////////
int sock::ServerSocket::Status = SHUTDOWN;
bool sock::ServerSocket::newLog = 0;

sock::ServerSocket::ServerSocket() {
	addr.sin_family = AF_INET;
	addr.sin_port = htons($SERV_PORT);
	addr.sin_addr.S_un.S_addr = inet_addr($SERV_ADDR);

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

	//*//log << "ok\n";
	log << "[" << now->tm_year + 1900 << "/";
	if (now->tm_mon < 9)
		log << "0" << now->tm_mon + 1 << "/";
	else log << now->tm_mon + 1 << "/";
	if (now->tm_mday < 10)
		log << "0" << now->tm_mday << "][";
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
	//*/
	//log << "1111/11/11][11:11:11] 222222222222222222";
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
	printf("Server: %s\n", Msg);
}

void sock::Recv(SOCKET& s, char*& buff, int flags) {
	int MsgSize = 0;
	recv(s, (char*)&MsgSize, sizeof(int), flags);

	buff = new char[MsgSize + 1];
	recv(s, buff, MsgSize, flags);

	buff[MsgSize] = '\0';
	printf("Client: %s\n", buff);
}

bool sock::Login(const char* data, int& Position) {
	vector<string> vdata = Tokenizer::split(data, SEP);
	if (vdata.size() != 3) return 0;

	if (!file::exists(PATH_ACCOUNT)) return 0;
	file::csv dataAccount(PATH_ACCOUNT);

	//SHA256 funtion
	string sh256password = sha256(vdata[2]);

	int puser = -1;

	if ((puser = file::finduser(dataAccount, vdata[1].c_str(), sh256password.c_str())) != -1) {

		Position = is(dataAccount.data[puser].pdata[3], "admin");
		return 1;
	}

	if ((puser = file::finduser(dataAccount, vdata[1].c_str(), "0")) != -1) {
		//Password reseted, confirm new password
		file::update(PATH_ACCOUNT, puser, 2, sh256password.c_str());

		Position = is(dataAccount.data[puser].pdata[3], "admin");
		return 1;
	}

	return 0;
}

bool sock::Register(const char* data)
{
	vector<string> vdata = Tokenizer::split(data, SEP);
	if (vdata.size() != 4)return false;
	if (!file::exists(PATH_ACCOUNT))return false;
	if (vdata[3] != vdata[2])return false;
	file::csv dataAccount(PATH_ACCOUNT);

	if (file::finduser(dataAccount, vdata[1].c_str()) != -1)
	{
		return false;
	}
	//string sha256pass = sha256(vdata[2]);
	std::ofstream out(PATH_ACCOUNT, std::ios::app);
	out << "1," << vdata[1] << "," << sha256(vdata[2]) << ",user\n";
	out.close();

	return 1;
}

std::string ctime_to_string(std::tm* date) {
	std::string data;
	if (date->tm_mday < 10)
		data = (std::string)"0" + std::to_string(date->tm_mday) + "/";
	else data = std::to_string(date->tm_mday) + "/";


	if (date->tm_mon < 9)
		data += (std::string)"0" + std::to_string(date->tm_mon + 1) + "/";
	else data += std::to_string(date->tm_mon + 1) + "/";

	data += std::to_string(date->tm_year + 1900);

	return data;
}

std::string sock::City(const char* data) {
	vector<string> vdata = Tokenizer::split(data, SEP);
	if (vdata.size() != 3) return SERV_SHUT;

	if (!file::exists(PATH_WEATHER)) return SERV_SHUT;
	if (!file::exists(PATH_CITY)) return SERV_SHUT;
	file::csv dataWeather(PATH_WEATHER);
	file::csv dataCity(PATH_CITY);

	int row = -1;
	std::string cmd = "CITY\t";
	if (vdata[2] == "next 7 days") {
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		for (int i = 0; i < 7; ++i) {
			while (1) {
				std::string date = ctime_to_string(now);

				if (vdata[1] == "ALL") {
					row = file::findWeather(dataWeather, row + 1, date.c_str());
				}
				else {
					row = file::findWeather(dataWeather, row + 1, date.c_str(), vdata[1].c_str());
				}
				if (row == -1) break;

				file::csv::line* tmp = &dataWeather.data[row];

				cmd += file::findCity(dataCity, tmp->pdata[0]) + ',';
				cmd += (string)tmp->pdata[1] + ',';
				cmd += (string)tmp->pdata[2] + ',';
				cmd += (string)tmp->pdata[3] + ',';
				cmd += (string)tmp->pdata[4] + '\t';
			}
			now->tm_mday++;
			mktime(now);
		}
	}
	else while (1) {
		if (vdata[1] == "ALL") {
			row = file::findWeather(dataWeather, row + 1, vdata[2].c_str());
		}
		else {
			row = file::findWeather(dataWeather, row + 1, vdata[2].c_str(), vdata[1].c_str());
		}
		if (row == -1) break;

		file::csv::line* tmp = &dataWeather.data[row];
		
		cmd += file::findCity(dataCity, tmp->pdata[0]) + ',';
		cmd += (string)tmp->pdata[1] + ',';
		cmd += (string)tmp->pdata[2] + ',';
		cmd += (string)tmp->pdata[3] + ',';
		cmd += (string)tmp->pdata[4] + '\t';
	}
	cmd.pop_back();
	return cmd;
}

void sock::City_ADD(const char* data) {
	vector<string> vdata = Tokenizer::split(data, SEP);
	if (vdata.size() != 3) return;

	if (!file::exists(PATH_CITY)) return;
	file::csv dataCity(PATH_CITY);

	if (file::findCity(dataCity, vdata[1].c_str()) == "Unknow") {
		std::ofstream out(PATH_CITY, std::ios::app);
		out << vdata[1] << "," << vdata[2] << "\n";
		out.close();
	}
}

void sock::City_UPDATE(const char* data) {
	vector<string> vdata = Tokenizer::split(data, SEP);

	if (!file::exists(PATH_CITY)) return;
	if (!file::exists(PATH_WEATHER)) return;
	file::csv dataCity(PATH_CITY);
	file::csv dataWeather(PATH_WEATHER);

	

	for (int i = 1; i < vdata.size(); ++i) {
		vector<string> tmp = Tokenizer::split(vdata[i], ",");
		if (file::findCity(dataCity, tmp[0].c_str()) == "Unknow") return;

		int row = file::findWeather(dataWeather, 0, tmp[1].c_str(), tmp[0].c_str());
		if (row != -1) {
			for (int j = 2; j < max(5, min(5, tmp.size())); ++j) {
				if (tmp[j] == "#") continue;
				file::update(PATH_WEATHER, row, j, tmp[j].c_str());
			}
		}
		else {
			if (tmp[2] == "#" && tmp[3] == "#" && tmp[4] == "#") continue;

			std::ofstream out(PATH_WEATHER, std::ios::app);
			for (int j = 0; j < max(5, min(5, tmp.size())); ++j) {
				out << tmp[j];
				if (j < min(5, tmp.size() - 1)) out << ",";
				else out << "\n";
			}
			out.close();
		}
	}
}

bool sock::QConnect(SOCKET& s) {
	if (sock::ServerSocket::Status == SHUTDOWN) {
		sock::Send(s, SERV_SHUT);
		return 0;
	}

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

	Send(s, SERV_LOGIN);
	Recv(s, tmp);

	if (sock::ServerSocket::Status == SHUTDOWN) {
		return 0;
	}

	if (is(tmp, "LOG")) {
		//printf("Login request: ");
		if (Login(tmp, Position)) {
			if (Position) {
				Send(s, SERV_ADMIN);
			}
			else {
				Send(s, SERV_USER);
			}
			delete tmp;
			return 1;
		}
		
		//printf("failed!\n");
		delete tmp;
		return 0;
	}
	if (is(tmp, "REG")) {
		// password unconfirm, username is used
		printf("Register request: ");
		if (Register(tmp)) {

			Send(s, SERV_USER);
			delete tmp;
			return 1;
		}

		printf("failed!\n");
		delete tmp;
		return 0;
	}

	//printf("%s\n", tmp);
	delete tmp;
	return 0;
}

int sock::Handle(SOCKET& s) {
	char* tmp = nullptr;

	Send(s, SERV_WAIT);
	Recv(s, tmp);

	if (sock::ServerSocket::Status == SHUTDOWN) {
		return 0;
	}

	if (is(tmp, "CITY")) {
		Send(s, City(tmp).c_str());
		delete tmp;
		return 1;
	}

	if (is(tmp, "CITY_NEW")) {
		City_ADD(tmp);
		delete tmp;
		return 1;
	}

	if (is(tmp, "CITY_UPDATE")) {
		City_UPDATE(tmp);
		delete tmp;
		return 1;
	}

	delete tmp;
	return 0;
}

