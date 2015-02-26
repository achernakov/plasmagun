#pragma once

#include "window.h"
#include "settings_window.h"

namespace Plasma {


	class MainWindow : public Window {
		public:
		
			typedef gboolean (MainWindow::*Handler)\
				(GtkWidget *widget, GdkEvent *event, gpointer data);
			MainWindow ();
			MainWindow (const std::string & uiXmlPath);
			virtual ~MainWindow ();
		protected:
			void connectSignal(const std::string & wdg, 
				const std::string & signal, Handler fun);
			void assignTab (const std::string & box, const std::string & tabLabel);
			void assignTabs ();
			virtual void connectSignals ();
			//SettingsWindow settingsWnd;
		private:
			gboolean on_delete_event (GtkWidget *widget,
				GdkEvent  *event,
				gpointer   data)  ;

			gboolean on_destroy (GtkWidget *widget,
					GdkEvent  *event,
					gpointer   data);
	
			gboolean on_connect_clicked (GtkWidget *widget, 
					GdkEvent  *event, gpointer   data);

	};
	
	
	
}
