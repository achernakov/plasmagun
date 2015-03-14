#include "../stdafx.h"
#include "infiniium_proto.h"

namespace Plasma {
	
	
	InfiniiumProtocol::InfiniiumProtocol () {
	}
	
	InfiniiumProtocol::~InfiniiumProtocol () {
	}
	
	InfiniiumProtocol::CommType InfiniiumProtocol::getCommType (const std::string & comm) {
		if (comm[comm.size() - 1] == '?') {
			return STR_REQUEST;
		} else {
			return COMMAND;
		}
	}
};
