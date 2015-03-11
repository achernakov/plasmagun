#include "../stdafx.h"
#include "connection.h"


namespace Plasma {

	Connection::Connection () {
		m_stat = Plasma::Connection::DISCONNECTED;
	}
	
	Connection::~Connection () {
		m_sock.disconnect();
	}
	
	void Connection::connect (const std::string & addr, const std::string & port) {
		bool success = true;
		try {
			m_sock.connect(addr, port);
		} catch (const Plasma::Error & ex) {
			success = false;
		}
		if (success) {
			m_stat = Plasma::Connection::CONNECTED;
			onConnect(addr, port);
		} else {
			m_stat = Plasma::Connection::DISCONNECTED;
			onFailedConnect(addr, port);
		}
	}
	
	void Connection::disconnect () {
		m_sock.disconnect();
		m_stat = Plasma::Connection::DISCONNECTED;
		onDisconnect();
	}
	
	
	Connection::Status Connection::status () {
		return m_stat;
	}	
	
	size_t Connection::send (const char * data, size_t size) {
		if (m_stat == DISCONNECTED) {
			throw Error("Failed to send: Connection not established");
		}
		size_t ret = m_sock.send(data, size);
		onSend();
		return ret;
	}
	
	size_t Connection::recv (char * data, size_t size) {
		if (m_stat == DISCONNECTED) {
			throw Error("Failed to recv: Connection not established");
		}
		size_t ret = m_sock.recv(data, size);
		onRecv();
		return ret;
	}
	
	void Connection::sendString (const std::string & str) {
		if (m_stat == DISCONNECTED) {
			throw Error("Failed to sendString: Connection not established");
		}
		m_sock.sendString(str);
		onSend();
	}
	
	void Connection::recvString (std::string  & str) {
		if (m_stat == DISCONNECTED) {
			throw Error("Failed to recv: Connection not established");
		}
		m_sock.recvString(str);
		onRecv();
	}
}
