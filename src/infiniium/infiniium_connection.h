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
			
			void getStrData (std::string & data);
			void getWordData (std::vector<int16_t> & data);
			
			
			void getIntParameter (const std::string & cmd, int & response);
			void getFloatParameter (const std::string & cmd, double & response);
			
			void translateData (std::string & string, char f, char r);
			void splitData (const std::string & data, std::string & copy, char sp);
			size_t parseWordRespHeader (const char * data, size_t & dataStart);
			void parseWordsFromBuf (const char * buf, size_t size, 
					std::vector<int16_t> & data);
					
			int parseIntFromString (const std::string & data);
			float parseFloatFromString (const std::string & data);
			//void parsedFizedPoint (const std::string & data, int & m, int & e);
				
			template<typename T> void getTypedParam(const std::string & cmd, T & response) {
				std::string strResp;
				stringRequest (cmd, strResp);
				std::stringstream buff(strResp);
				g_print(strResp.c_str());
				buff >> response;
			}

		protected:
			std::vector<char> m_respData;
		private:
	};


}
