#include "../stdafx.h"
#include "net.h"

#define DEFAULT_NET_BUF 1024 * 10

NetClnt::NetClnt () :
	m_sock(-1) {
}

NetClnt::NetClnt (int sock) :
	m_sock(sock) {
}

NetClnt::NetClnt (const NetClnt & oth) :
	m_sock(oth.m_sock) {
}

NetClnt::~NetClnt () {
}

void NetClnt::connect (const std::string & addr, const std::string & port) {
	int ret = socket (AF_INET, SOCK_STREAM, 0);
	if (ret == -1) {
		throw std::runtime_error ("Can't create socket");
	}
	
	addrinfo hints, *results;
	
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */

	ret = getaddrinfo(addr.c_str(), port.c_str(), &hints, &results);
	if (ret) {
			throw std::runtime error (("Can't getaddrinfo(); of " + addr + ":" + port).c_str());
	}
	int sfd = -1;	
	for (addrinfo * rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;
		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;                  /* Success */
	}
	if (sfd == -1) {
		throw std::runtime_error (("Can't connect to " + addr + ":" + port).c_str());
	} else {
		m_sock = sfd;
	}	
}

void NetClnt::disconnect () {
	if (m_sock != -1) {
		close (m_sock);
		m_sock == -1;
	} 
}

