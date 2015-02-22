#pragma once


namespace Plasma {


	class Window {
		public:
	
			typedef gboolean (Window::*EventHandlerMethod)\
				(GtkWidget *widget, GdkEvent *event, gpointer data);
		
			struct EventHandler {
				EventHandler (Window * thisPtr,
						EventHandlerMethod method,
						gpointer ptr);
				Window * m_this;
				EventHandlerMethod m_handler;
				gpointer m_data;
			};
		
			Window ();
			Window (const std::string & uiXmlPath);
			virtual ~Window ();
			GtkWidget * operator [] (const std::string & widget);
			void registerEventHandler (GtkWidget *widget, const std::string &event,
					EventHandlerMethod method, gpointer userData = NULL);
			static gboolean staticEventHandler (GtkWidget *widget, GdkEvent *event, 
					gpointer data);
		protected:
			virtual void connectSignals () = 0;
			void loadUiXml (const std::string & uiXmlPath);

			GtkBuilder * m_builder;
			std::vector<EventHandler*> m_handlers;
	};
	
	
}
