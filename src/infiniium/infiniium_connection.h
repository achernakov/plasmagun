#pragma once

#include "../net/connection.h"

#define PLASMA_INFINIIUM_BUF_SIZE ((size_t)(1024 * 10))

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
			
			void getWfData (std::string & data);
			
		protected:
			std::vector<char> m_respData;
		private:
	};


}
