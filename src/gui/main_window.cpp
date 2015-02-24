#include "../stdafx.h"
#include "../infiniium/net.h"
#include "main_window.h"

namespace Plasma {

	MainWindow::MainWindow (const std::string & uiXmlPath) :
			Window(uiXmlPath, true) {
		connectSignals();
	}
	
	MainWindow::~MainWindow () {
	}

	void MainWindow::connectSignal (const std::string & wdg, 
			const std::string & signal, Handler fun) {
		registerEventHandler(this->operator[](wdg.c_str()), signal.c_str(),
				WINDOW_CALLBACK(fun), NULL);
	}
	
	void MainWindow::connectSignals () {
		connectSignal ("main_window",  "delete-event", &MainWindow::on_delete_event);
		connectSignal ("request_button", "clicked", &MainWindow::on_button1_clicked);
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

	gboolean MainWindow::on_button1_clicked (GtkWidget *widget, 
			GdkEvent  *event, gpointer   data) {
		NetClnt net;
		net.connect(gtk_entry_get_text(GTK_ENTRY(operator[]("addr_entry"))), 
				gtk_entry_get_text(GTK_ENTRY(operator[]("port_entry"))));
		//net.sendString("GET / HTTP/1.0\r\n\r\n");
		net.sendString((std::string(gtk_entry_get_text(GTK_ENTRY(operator[]("command_entry")))) + "\n").c_str());
		std::string resp;
		net.recvString(resp);
		g_print(resp.c_str());
		net.disconnect();
	}
	
}
