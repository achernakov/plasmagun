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
		sendString (cmd);
	}
	
	void InfiniiumConnection::stringRequest (const std::string & req, 
			std::string & resp) {
		sendString (req);
		recvString (resp);
	};

}
