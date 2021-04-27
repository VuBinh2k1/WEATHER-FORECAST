#pragma once
#include "stdafx.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib" )

#define SEP "\t"

namespace sock {
	class ClientSocket {

	};

	bool is(const char* input, const char* cmd = "EXIT", int flags = 0);
	void Send(SOCKET& s, const char* Msg, int flags = 0);
	void Recv(SOCKET& s, char*& buff, int flags = 0);

	/// Q:questtion(send) - A:answer(recv, send back)

	int Handle(SOCKET& s);
}