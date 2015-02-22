#include "../stdafx.h"
#include "main_window.h"

namespace Plasma {

	MainWindow::MainWindow (const std::string & uiXmlPath) :
			Window(uiXmlPath) {
		connectSignals();
	}
	
	MainWindow::~MainWindow () {
	}

	void MainWindow::connectSignals () {
		registerEventHandler(this->operator[]("main_window"), "delete-event",
				WINDOW_CALLBACK(&MainWindow::on_delete_event), NULL);
		registerEventHandler(this->operator[]("main_window"), "destroy", 
				WINDOW_CALLBACK(&MainWindow::on_destroy), NULL);
		registerEventHandler(this->operator[]("button1"), "clicked",
				WINDOW_CALLBACK(&MainWindow::on_button1_clicked), NULL);
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

	gboolean MainWindow::on_destroy (GtkWidget *widget,
			GdkEvent  *event,
			gpointer   data)  {
		gtk_main_quit();
		return FALSE;
	}
	
	gboolean MainWindow::on_button1_clicked (GtkWidget *widget, 
			GdkEvent  *event, gpointer   data) {
		gtk_window_set_title(GTK_WINDOW(this->operator[]("main_window")), 
				gtk_entry_get_text(GTK_ENTRY(operator[]("entry1"))));
	}
	
}
