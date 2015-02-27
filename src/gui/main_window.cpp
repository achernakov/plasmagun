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
	
	void MainWindow::assignTab (const std::string & box, const std::string & tabLabel) {
		GtkNotebook * notebook = GTK_NOTEBOOK(operator[]("main_notebook"));
		if (!notebook) {
			throw Plasma::Error("Can't find main_notebook");
		}
		gint res = gtk_notebook_append_page (notebook, 
			GTK_WIDGET(operator[](box.c_str())),
			GTK_WIDGET(operator[](tabLabel.c_str())));
		if (res == -1) {
			throw Plasma::Error (("Failed to attach box " + box + 
					" or tabLabel " + tabLabel).c_str());
		}
	}
	
	void MainWindow::assignTabs () {
		assignTab("command_box", "tab_label_log");
	}
	
	//HELPERS

	void appendText (GtkTextBuffer * buf, const std::string & text) {
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
		NetClnt net;
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


