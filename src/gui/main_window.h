#pragma once

#include "window.h"

namespace Plasma {


	class MainWindow : public Window {
		public:
			MainWindow ();
			MainWindow (const std::string & uiXmlPath);
			virtual ~MainWindow ();
		protected:
			virtual void connectSignals ();

		private:
			gboolean on_delete_event (GtkWidget *widget,
				GdkEvent  *event,
				gpointer   data)  ;

		gboolean on_destroy (GtkWidget *widget,
				GdkEvent  *event,
				gpointer   data);
	
		gboolean on_button1_clicked (GtkWidget *widget, 
				GdkEvent  *event, gpointer   data);
	};
	
	
	
}
