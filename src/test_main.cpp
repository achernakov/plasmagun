#include "stdafx.h"
#include "net/socket.h"
#include "net/connection.h"


int main (int argc, char ** argv) {
	Socket net;
	net.connect("172.16.13.238", "5025");
	//net.sendString("*RST\n");
	net.sendString("*IDN?\n");
	std::string resp;
	net.recvString(resp);
	std::cout << "[" << resp << "]" << std::endl;
	net.disconnect();
	return 0;
}

