
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


/* This is a callback function. The data arguments are ignored
 * in this example. More on callbacks below.
 */
/*static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  NetClnt net;
  net.connect("google.com", "http");
  net.sendString("GET / HTTP/1.0\r\n\r\n");
  std::string resp;
  net.recvString(resp);
  g_print(resp.c_str());
  net.disconnect();
}

static gboolean
on_delete_event (GtkWidget *widget,
                 GdkEvent  *event,
                 gpointer   data)
{
  g_print ("delete event occurred\n");

  return TRUE;
}

int main (int   argc,
      char *argv[])
{
  GtkWidget *window;
  GtkWidget *button;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Hello");
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  button = gtk_button_new_with_label ("Hello World");
  gtk_container_add (GTK_CONTAINER (window), button);

  g_signal_connect (window, "delete-event", G_CALLBACK (on_delete_event), NULL);
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);

  gtk_widget_show (button);
  gtk_widget_show (window);

  gtk_main ();

  return 0;
}*/
