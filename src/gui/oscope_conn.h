#pragma once

#include "../infiniium/infiniium_connection.h"

namespace Plasma {

	class MainWindow;

	class OscopeConn : public InfiniiumConnection {
		public:
			OscopeConn (); //Never implements;
			OscopeConn (MainWindow & mainWnd);
			virtual ~OscopeConn ();
			
			virtual void onConnect (const std::string & addr, 
					const std::string & port);
			virtual void onFailedConnect (const std::string & addr,
					const std::string & port);
			virtual void onSend ();
			virtual void onRecv ();
			virtual void onDisconnect ();
			virtual void onRemoteClosed ();

		protected:
			MainWindow & m_wnd;
		private:
	};


}
