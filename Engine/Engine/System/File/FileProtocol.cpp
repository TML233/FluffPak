#include "Engine/System/File/FileProtocol.h"

namespace Engine {
	bool FileSystem::IsProtocolValid(Protocol protocol) {
		return protocol > Protocol::Null && protocol < Protocol::End;
	}
	template<>
	int32 ObjectUtil::GetHashCode<FileSystem::Protocol>(const FileSystem::Protocol& obj) {
		return (int32)obj;
	}
	ResultPair<FileSystem::Protocol, int32> FileSystem::GetSplitData(const String& path) const {
		const String prefix = STRING_LITERAL("://");

		int32 index = path.IndexOf(prefix);

		String protocolName;
		int32 rIndex = 0;

		if (index >= 0) {
			protocolName = path.Substring(0, index);
			rIndex = index + prefix.GetCount();
		}

		Protocol rProtocol = protocolName.GetCount() > 0 ? GetProtocol(protocolName) : Protocol::Native;

		return ResultPair<Protocol, int32>(rProtocol, rIndex);
	}

	FileSystem::Protocol FileSystem::GetProtocol(const String& name) const {
		Protocol r = Protocol::Null;
		protocols.TryGet(name, r);
		return r;
	}
	FileProtocol* FileSystem::GetProtocolHandler(Protocol protocol) const {
		SharedPtr<FileProtocol> p{};
		if (protocolHandlers.TryGet(protocol, p)) {
			return p.GetRaw();
		} else {
			return nullptr;
		}
	}

	void FileSystem::AddProtocol(const String& name, FileSystem::Protocol protocol) {
		protocols.Set(name, protocol);
	}
	void FileSystem::AddProtocolHandler(Protocol protocol, UniquePtr<FileProtocol>&& handler) {
		handler->owner = this;
		protocolHandlers.Set(protocol, SharedPtr<FileProtocol>(handler.Release()));
	}
}
