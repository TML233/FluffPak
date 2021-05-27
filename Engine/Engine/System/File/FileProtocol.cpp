#include "Engine/System/File/FileProtocol.h"
#include "Engine/System/Object/ObjectUtil.h"

namespace Engine {
	bool FileProtocol::IsProtocolValid(Protocol protocol) {
		return protocol > Protocol::Null && protocol < Protocol::End;
	}
}
