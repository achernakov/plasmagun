#include "../stdafx.h"
#include "infiniium_connection.h"

namespace Plasma {

	InfiniiumConnection::InfiniiumConnection () {
	}
	
	InfiniiumConnection::~InfiniiumConnection () {
	}


	void InfiniiumConnection::connect (const std::string & addr, 
			const std::string & port) {
		Connection::connect (addr, port);
	}
	
	void InfiniiumConnection::disconnect () {
		Connection::disconnect ();
	}
	
	void InfiniiumConnection::initialize () {
	}
	
	void InfiniiumConnection::command (const std::string & cmd) {
		sendString (cmd + "\n");
	}
	
	void InfiniiumConnection::stringRequest (const std::string & req, 
			std::string & resp) {
		sendString (req + "\n");
		getStrData (resp);
	};
	
	void InfiniiumConnection::getStrData (std::string & data) {
		char buf[PLASMA_INFINIIUM_BUF_SIZE];
		
		bool gotAllData = false;
		
		data.clear();
		
		do {
			size_t size = recv(buf, PLASMA_INFINIIUM_BUF_SIZE-1);
			for (size_t i = 0; i < size; i++) {
				if (buf[i] == '\n')
					gotAllData = true; 
			}
			//m_respData.insert(m_respData.end(), buf, buf + size);
			data.append(buf, size);
		} while (!gotAllData);
	}
	
	/*std::string && InfiniiumConnection::formatWordData (std::vector<int16_t> & data, int timeIncr_m, int timeIncr_e) {
	}*/
	
	void InfiniiumConnection::getWordData (std::vector<int16_t> & data) {
		char buf[PLASMA_INFINIIUM_BUF_SIZE];
		
		bool gotAllData = false;
		size_t dataStart;
		
		size_t recieved = recv(buf, PLASMA_INFINIIUM_BUF_SIZE);
		size_t dataLength = parseWordRespHeader(buf, dataStart);
		size_t fullLength = dataStart + dataLength + 1;

		data.clear();
		int16_t * startPtr = (int16_t*)(buf + dataStart);
		size_t gotWords = 0;
		
		std::vector<char> dataBuf(dataLength + 1);
		dataBuf.assign(buf + dataStart, buf + recieved);

		while (recieved < fullLength) {
			size_t ret = recv (dataBuf.data() + recieved - dataStart, fullLength - recieved); 
			recieved += ret;
		}
		
		parseWordsFromBuf (dataBuf.data(), dataLength, data);
	
		if (dataBuf[dataLength] != '\n') {
			throw Plasma::Error("InfiniiumConnection::getWordData(): wrong message ending");
		}
	}
	
	void InfiniiumConnection::parseWordsFromBuf (const char * buf, size_t size, 
			std::vector<int16_t> & data) {
		size_t i;
		size_t reminder = size % 2;
		size_t count = size / 2;
		const int16_t * ptr = reinterpret_cast<const int16_t*>(buf);
		data.assign(ptr, ptr + count);
	}
	
	size_t InfiniiumConnection::parseWordRespHeader (const char * buf, 
			size_t & dataStart) {
		if (buf[0] != '#') {
			throw Plasma::Error ("InfiniiumConnection::parseWordRespHeader: \
					invalid header");
		}
		size_t digitsCount = (size_t) (buf[1] - '0');
		if (digitsCount < 0 || digitsCount > 9) {
			throw Plasma::Error ("InfiniiumConnection::parseWordRespHeader: \
					wrong digitsCount");
		}
		
		size_t dataSize = 0;
		
		for (size_t i = 0; i < digitsCount; i++) {
			size_t d = (size_t) (buf[2 + i] - '0');
			if (d < 0 || d > 9) {
				throw Plasma::Error ("InfiniiumConnection::parseWordRespHeader: \
						wrong digit");
			}
			dataSize = dataSize * 10 + d;
		}
		
		dataStart = 2 + digitsCount;
		
		return dataSize;
		
	}
	
	void InfiniiumConnection::translateData (std::string & data, char f, char r) {
		size_t addedLen = 0;
		for (size_t i = 0; i < data.size() + addedLen; i++) {
			if (f == data[i]) {
				data[i] = r;
			}
		}
	}
	
	void InfiniiumConnection::splitData (const std::string & data, std::string & copy, 
			char sp) {
		size_t start = 0;
		copy.clear();
		for (size_t i = 0; i < data.size(); i++) {
			if (data[i] == sp) {
				copy.insert(copy.end(), data.c_str() + start, data.c_str() + (i + 1));
				copy.insert(copy.end(), '\n');
				start = i + 1;
			}
		}
	}
	
	void InfiniiumConnection::getIntParameter (const std::string & cmd, int & response) {
		std::string strResp;
		stringRequest (cmd, strResp);
		std::stringstream stream(strResp);
		stream >> response;
	}
	
	void InfiniiumConnection::getFloatParameter (const std::string & cmd, double & response) {
		std::string strResp;
		stringRequest (cmd, strResp);
		std::stringstream stream(strResp);
		stream >> response;
	}
	
	/*unsigned int InfiniiumConnection::parseUInt (const char * buf) {
		if (buf[0] < '0' || buf[i] > '9') {
			throw Plasma::Error("Wrong number format");
		} 
		unsigned int num = 0;
		for (size_t i = 0; buf[i] >= '0' && buf[i] <= '9'; i++) {
			int d = buf[i] - '0';
			if (d < 0 || d > 9) {
				break;
			}
			num = num * 10 + d;	
		}		
		
		return num;
	}
	
	int InfiniiumConnection::parseInt (const char * buf) {
		if (strResp[0] == '-') {
			return -(int)parseUInt(buf + 1);
		} else {
			return (int)parseUInt(buf);
		}
	}
	
	double InfiniiumConnection::parseFloat (const char * buf) {
		bool minus = false;
		bool point = false; size_t pointPos;
		bool exp = false; size_t expPos;
		bool esign = false; int emul = 1;
		double ret;
		size_t i = 0;

		if (buf[0] == '-') {
			minus = true;
			i++;
		}
		for (; buf[i] >='0' && buf[i] <= '9'; i++);
		if (buf[i] == '.') {
			point = true;
			pointPos = i;
			i++;
		}
		for (; buf[i] >='0' && buf[i] <= '9'; i++);
		if (buf[i] == 'E' || buf[i] == 'e') {
			exp = true;
			expPos = i;
			i++;
			if (buf[i] == '-') {
				emul = -1;
				esign = true;
			} else if (buf[i] == '+' {
				esign = true;
			} 
		}
		unsigned int m, p, e;
		m = parseUInt(buf + (minus)?1:0);
		if (point) {
			p = parseUInt(buf + pointPos + 1);
		}
		if (exp) {
			e = parseUInt(buf + expPos + (esign)?1:0);
		}
		
		return m + 
	}
	
	void InfiniiumConnection::getFloatParameter (const std::string & cmd, double & resp) {
		std::string strResp;
		stringRequest (cmd, strResp);
	}*/

}
