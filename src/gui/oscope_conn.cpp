#include "../stdafx.h"
#include "oscope_conn.h"
#include "main_window.h"

namespace Plasma {

	OscopeConn::OscopeConn (MainWindow & mainWnd) :
		m_wnd (mainWnd)
	{
	}
	
	OscopeConn::~OscopeConn () {
	}

	void OscopeConn::onConnect (const std::string & addr, 
			const std::string & port) {	
		g_print ("[OSCOPE] Connected\n");
		m_wnd.onConnect ();
	}


	void OscopeConn::onFailedConnect (const std::string & addr,
			const std::string & port) {
		g_print ("[OSCOPE] Connection attempt failed\n");
	}


	void OscopeConn::onSend () {
		g_print ("[OSCOPE] Sent\n");
	}


	void OscopeConn::onRecv () {
		g_print ("[OSCOPE] Recieved\n");
	}


	void OscopeConn::onDisconnect () {
		g_print ("[OSCOPE] Disconnected\n");
		m_wnd.onDisconnect();
	}


	void OscopeConn::onRemoteClosed () {
		g_print ("[OSCOPE] Remote host closed the connection\n");
	}


	void OscopeConn::operator () () {
		g_print ("[OSCOPE] Called separate thread\n");
	}


}
