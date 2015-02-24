#include "stdafx.h"
#include "infiniium/net.h"

int main (int argc, char ** argv) {
	NetClnt net;
	net.connect("172.16.13.238", "5025");
	//net.sendString("*RST\n");
	net.sendString("*IDN?\n");
	std::string resp;
	net.recvString(resp);
	std::cout << "[" << resp << "]" << std::endl;
	net.disconnect();
	return 0;
}

