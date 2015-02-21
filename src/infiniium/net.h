#pragma once

class NetClnt {
	public:
		NetClnt ();
		NetClnt (int sock);
		NetClnt (const NetClnt & oth);
		virtual ~NetClnt ();

		void connect (const std::string & addr, const std::string & port);
		void disconnect ();
		void sendString (const std::string & str);
		void recvString (std::string & str);
		void performCommand (const std::string & cmd, std::string & resp);
	private:
		int m_sock;
};
