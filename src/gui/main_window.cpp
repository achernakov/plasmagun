#include "../stdafx.h"
#include "../infiniium/net.h"
#include "main_window.h"

namespace Plasma {

	MainWindow::MainWindow (const std::string & uiXmlPath) :
			Window(uiXmlPath, true) {
		connectSignals();
		//assignTabs();
	}
	
	MainWindow::~MainWindow () {
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
		Socket net;
		net.connect(gtk_entry_get_text(GTK_ENTRY(operator[]("addr_entry"))), 
				gtk_entry_get_text(GTK_ENTRY(operator[]("port_entry"))));
		//net.sendString("GET / HTTP/1.0\r\n\r\n");
		std::string cmd = std::string(gtk_entry_get_text(GTK_ENTRY(operator[]("command_entry")))) + "\r\n\r\n";
		//gtk_entry_set_text (GTK_ENTRY(operator[]("command_entry")), "");
		net.sendString(cmd.c_str());
		appendText(GTK_TEXT_BUFFER(operator[]("log_buffer")), cmd);
		std::string resp;
		net.recvString(resp);
		g_print(resp.c_str());
		appendText(GTK_TEXT_BUFFER(operator[]("log_buffer")), resp);
		net.disconnect();
	}



	
}


