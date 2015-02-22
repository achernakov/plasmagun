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
		connectSignal ("button1", "clicked", &MainWindow::on_button1_clicked);
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
		gtk_window_set_title(GTK_WINDOW(this->operator[]("main_window")), 
				gtk_entry_get_text(GTK_ENTRY(operator[]("entry1"))));
				
		NetClnt net;
		net.connect(gtk_entry_get_text(GTK_ENTRY(operator[]("entry1"))), "http");
		net.sendString("GET / HTTP/1.0\r\n\r\n");
		std::string resp;
		net.recvString(resp);
		g_print(resp.c_str());
		net.disconnect();
	}
	
}
