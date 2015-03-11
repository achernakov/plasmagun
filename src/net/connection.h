#pragma once

#include "socket.h"

namespace Plasma {

	class Connection {
		public:
			enum Status {DISCONNECTED, CONNECTED};
//			enum Event {EV_CONNECT, EV_DISCONNECT, EV_RECV, EV_SEND, EV_CONNECT_FAILED}

			Connection ();
			virtual ~Connection ();
			
			void connect (const std::string & addr, const std::string & port);
			void disconnect ();
			Status status ();
			
			size_t send (const char * data, size_t size);
			size_t recv (char * data, size_t size);
			void sendString (const std::string & str);
			void recvString (std::string & str);

			virtual void onConnect (const std::string & addr, 
					const std::string & port) = 0;
			virtual void onFailedConnect (const std::string & addr,
					const std::string & port) = 0;
			virtual void onSend () = 0;
			virtual void onRecv () = 0;
			virtual void onDisconnect () = 0;
			virtual void onRemoteClosed () = 0;
		protected:
			Socket m_sock;
			Status m_stat;
		private:
	};


}
