
#include "stdafx.h"
#include "infiniium/net.h"
#include "gui/main_window.h"


int main (int argc, char ** argv) {
	gtk_init (&argc, &argv);
	
	Plasma::MainWindow wnd ("res/main_window.ui");
	gtk_widget_show (wnd["main_window"]);
	
	gtk_main ();
	return 0;
}

