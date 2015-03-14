#include "../stdafx.h"
#include "../net/socket.h"
#include "../infiniium/infiniium_proto.h"
#include "main_window.h"

namespace Plasma {

	MainWindow::MainWindow (const std::string & uiXmlPath) :
			Window(uiXmlPath, true), m_oscopeConn(*this) {
		connectSignals();
		//assignTabs();
	}
	
	MainWindow::~MainWindow () {
		if (m_oscopeConn.status() == Plasma::Connection::CONNECTED) {
			m_oscopeConn.disconnect();
		}
	}

	void MainWindow::connectSignal (const std::string & wdg, 
			const std::string & signal, Handler fun) {
		registerEventHandler(GTK_WIDGET(this->operator[](wdg.c_str())), signal.c_str(),
				WINDOW_CALLBACK(fun), NULL);
	}
	
	void MainWindow::connectSignals () {
		connectSignal ("main_window",  "delete-event", &MainWindow::on_delete_event);
		connectSignal ("request_button", "clicked", &MainWindow::on_connect_clicked);
	}
	
	//HELPERS

	void MainWindow::appendText (GtkTextBuffer * buf, const std::string & text) {
		GtkTextIter iter;
		gtk_text_buffer_get_end_iter (buf, &iter);
		gtk_text_buffer_insert (buf, &iter, text.c_str(), -1);
	}
	
	
	//==========================================
	//============EVENT HANDLERS================
	//==========================================

	gboolean MainWindow::on_delete_event (GtkWidget *widget,
			GdkEvent  *event,
			gpointer   data)  {
		g_print ("delete event occurred\n");
		return FALSE;
	}

	gboolean MainWindow::on_connect_clicked (GtkWidget *widget, 
			GdkEvent  *event, gpointer   data) {
		
		if (m_oscopeConn.status() == Plasma::Connection::DISCONNECTED) {
			m_oscopeConn.connect(gtk_entry_get_text(GTK_ENTRY(operator[]("addr_entry"))), 
					gtk_entry_get_text(GTK_ENTRY(operator[]("port_entry"))));
		}
		
		std::string cmd = std::string( gtk_entry_get_text (GTK_ENTRY( operator[]("command_entry" ))));
		Plasma::InfiniiumProtocol::CommType type = Plasma::InfiniiumProtocol::getCommType (cmd);
		
		
		//cmd += "\r\n\r\n";
		//gtk_entry_set_text (GTK_ENTRY(operator[]("command_entry")), "");
		
		
		//m_oscopeConn.sendString(cmd.c_str());
		appendText(GTK_TEXT_BUFFER(operator[]("log_buffer")), cmd+"\n\n");

		if (type == Plasma::InfiniiumProtocol::STR_REQUEST) {
			std::string resp;
			//m_oscopeConn.recvString(resp);
			m_oscopeConn.stringRequest(cmd, resp);
			g_print(resp.c_str());
			appendText(GTK_TEXT_BUFFER(operator[]("log_buffer")), resp);
		} else {
			m_oscopeConn.command(cmd);
		}
	}
	
	void MainWindow::onConnect () {
		gtk_switch_set_state (GTK_SWITCH(operator[]("connect_switch")), TRUE);
	}
	
	void MainWindow::onDisconnect () {
		gtk_switch_set_state (GTK_SWITCH(operator[]("connect_switch")), FALSE);
	}



	
}


