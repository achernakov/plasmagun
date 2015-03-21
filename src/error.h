#pragma once

namespace Plasma {

	enum ErrorType {PL_ERR_NET, PL_ERR_PROTO, PL_ERR_DATA, PL_ERR_UNKNOWN};
	enum ErrorLevel {PL_ERROR, PL_WARNING};

	class Error : public std::exception {
		public:
			Error();
			Error (const std::string & err, ErrorLevel level = PL_ERROR, 
					ErrorType type = PL_ERR_UNKNOWN) throw() :
					m_type(type), m_level(level), m_what(err) {
			};
			virtual ~Error() throw() {
			};

			const char * what () const throw () {
				return m_what.c_str();
			};
			ErrorLevel level () const {
				return m_level;
			};
			ErrorType type () const {
				return m_type;
			};

		protected:
			std::string m_what;
			ErrorLevel m_level;
			ErrorType m_type;
	};

}
