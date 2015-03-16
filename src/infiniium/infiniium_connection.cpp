#include "../stdafx.h"
#include "infiniium_connection.h"

namespace Plasma {

	InfiniiumConnection::InfiniiumConnection () {
	}
	
	InfiniiumConnection::~InfiniiumConnection () {
	}


	void InfiniiumConnection::connect (const std::string & addr, 
			const std::string & port) {
		Connection::connect (addr, port);
	}
	
	void InfiniiumConnection::disconnect () {
		Connection::disconnect ();
	}
	
	void InfiniiumConnection::initialize () {
	}
	
	void InfiniiumConnection::command (const std::string & cmd) {
		sendString (cmd + "\n");
	}
	
	void InfiniiumConnection::stringRequest (const std::string & req, 
			std::string & resp) {
		sendString (req + "\n");
		recvString (resp);
	};
	
	void InfiniiumConnection::getWfData (std::string & data) {
		char buf[PLASMA_INFINIIUM_BUF_SIZE];
		
		bool gotAllData = false;
		
		data.clear();
		
		do {
			size_t size = recv(buf, PLASMA_INFINIIUM_BUF_SIZE-1);
			for (size_t i = 0; i < size; i++) {
				if (buf[i] == '\n')
					gotAllData = true; 
			}
			//m_respData.insert(m_respData.end(), buf, buf + size);
			data.append(buf, size);
		} while (!gotAllData);
	} 

}
