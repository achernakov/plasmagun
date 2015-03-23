
#include "stdafx.h"
#include "net/socket.h"
#include "gui/main_window.h"


int main (int argc, char ** argv) {
	gtk_init (&argc, &argv);
	
#ifdef _WIN32
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        g_print("WSAStartup failed with error: %d\n", err);
        return 1;
    }
#endif
	
	Plasma::MainWindow wnd ("res/main_window.ui");
	gtk_widget_show (GTK_WIDGET(wnd["main_window"]));
	
	gtk_main ();
	
#ifdef _WIN32
	WSACleanup();
#endif
	return 0;
}

