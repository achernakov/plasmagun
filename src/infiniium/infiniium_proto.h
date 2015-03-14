#pragma once

namespace Plasma {

	
	class InfiniiumProtocol {
		public:
			enum CommType {COMMAND, STR_REQUEST, UNKNOWN};
			InfiniiumProtocol ();
			virtual ~InfiniiumProtocol ();
			
			static CommType getCommType (const std::string & comm);
			
		protected:
		private:
	};


}
