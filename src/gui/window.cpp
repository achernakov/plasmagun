#include "../stdafx.h"
#include "window.h"

namespace Plasma {

	Window::Window (const std::string & uiXmlPath, bool isMain) {
		loadUiXml(uiXmlPath);
		if (isMain) {
			g_signal_connect (this->operator[]("main_window"), "destroy",
				G_CALLBACK(gtk_main_quit), NULL);
		}
	}

	void Window::loadUiXml (const std::string & uiXmlPath) {
		m_builder = gtk_builder_new ();
		if (!m_builder) {
			throw Plasma::Error("Can't create XML loader");
		}
		guint err = gtk_builder_add_from_file (m_builder, uiXmlPath.c_str(), NULL);
		if (!err) {
			throw Plasma::Error("Can't load widgets XML");
		}
	}
	
	void Window::registerEventHandler(GtkWidget *widget, const std::string &event,
			EventHandlerMethod method, gpointer userData) {
		EventHandler * tmp = new EventHandler(this, method, userData);
		m_handlers.push_back(tmp);
		g_signal_connect(widget, event.c_str(), G_CALLBACK(staticEventHandler), tmp);
	}
	
	Window::~Window () {
		for (std::vector<EventHandler*>::iterator iter = m_handlers.begin(); 
				iter != m_handlers.end(); iter++) {
			delete *iter;
		}
	}
	
	Window::EventHandler::EventHandler(Window * thisPtr, EventHandlerMethod method,
			gpointer ptr) {
		m_this = thisPtr;
		m_handler = method;
		m_data = ptr;
	}
	
	GObject * Window::operator [] (const std::string & wdgName) {
		if (!m_builder) {
			throw Plasma::Error("No UI builder instance created");
		}
		GObject * ret = gtk_builder_get_object(m_builder, wdgName.c_str());
		if (!ret) {
			throw Plasma::Error(("Can't get required widget: " + wdgName).c_str());
		}
		return ret;
	}
	
	gboolean Window::staticEventHandler(GtkWidget *widget, GdkEvent *event, 
			gpointer data) {
		EventHandler * handler = reinterpret_cast<EventHandler*>(data);
		EventHandlerMethod method = handler->m_handler;
		(handler->m_this->*method)(widget, event, handler->m_data);
	}

}
