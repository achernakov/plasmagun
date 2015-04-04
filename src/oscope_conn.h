#pragma once

#include "infiniium/infiniium_connection.h"

namespace Plasma {

	class MainWindow;

	class OscopeConn : public InfiniiumConnection {
		public:
			OscopeConn ();

			virtual ~OscopeConn ();
			
			void operator () ();
			
			virtual void onConnect (const std::string & addr, 
					const std::string & port) override;
			virtual void onFailedConnect (const std::string & addr,
					const std::string & port) override;
			virtual void onSend () override;
			virtual void onRecv () override;
			virtual void onDisconnect () override;
			virtual void onRemoteClosed () override;

		protected:

		private:
	};


}
