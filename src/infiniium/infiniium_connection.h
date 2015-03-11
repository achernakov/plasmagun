#pragma once

#include "../net/connection.h"

namespace Plasma {

	class InfiniiumConnection : public Connection {
		public:
			InfiniiumConnection ();
			virtual ~InfiniiumConnection ();
			
			void connect (const std::string & addr, const std::string & port);
			void disconnect ();
			
			void initialize ();
			
			void command (const std::string & cmd);
			void stringRequest (const std::string & req, std::string & resp);
			
			
		protected:
		private:
	};


}
