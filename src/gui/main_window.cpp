#include "../stdafx.h"
#include "main_window.h"

namespace Plasma {

	MainWindow::MainWindow (const std::string & uiXmlPath) :
			Window(uiXmlPath) {
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
		//connectSignal ("main_window", "destroy", &MainWindow::on_destroy);
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

	/*gboolean MainWindow::on_destroy (GtkWidget *widget,
			GdkEvent  *event,
			gpointer   data)  {
		g_print ("destroy event occurred\n");
		return FALSE;
	}*/
	
	gboolean MainWindow::on_button1_clicked (GtkWidget *widget, 
			GdkEvent  *event, gpointer   data) {
		gtk_window_set_title(GTK_WINDOW(this->operator[]("main_window")), 
				gtk_entry_get_text(GTK_ENTRY(operator[]("entry1"))));
	}
	
}
