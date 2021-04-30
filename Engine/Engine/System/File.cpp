#include "Engine/System/File.h"

namespace Engine{
	bool FileSystem::IsProtocolValid(Protocol protocol) {
		return protocol > Protocol::Null && protocol < Protocol::End;
	}
	FileSystem::FileSystem() {
		using Handler = UniquePtr<FileSystemProtocolHandler>;
		AddProtocol(STRING_LITERAL("file"), Protocol::File);
		//AddProtocolHandler(Protocol::File, Handler(MEMNEW(...));
		AddProtocol(STRING_LITERAL("res"), Protocol::Resource);
		//AddProtocolHandler(Protocol::Resource, Handler(MEMNEW(...));
		AddProtocol(STRING_LITERAL("user"), Protocol::User);
		//AddProtocolHandler(Protocol::User, Handler(MEMNEW(...));
	}

	ResultPair<String, int32> FileSystem::SplitProtocolString(const String& path) {
		const String prefix = STRING_LITERAL("://");

		int32 index = path.IndexOf(prefix);
		
		ResultPair<String, int32> result{ String::GetEmpty(),0 };
		
		if (index >= 0) {
			result.result = path.Substring(0, index);
			result.value = index + prefix.GetCount();
		}

		return result;
	}

	FileSystem::Protocol FileSystem::GetProtocolFriendly(const String& name) const {
		if (name.GetCount() == 0) {
			return Protocol::File;
		}
		return GetProtocol(name);
	}

	FileSystem::Protocol FileSystem::GetProtocol(const String& name) const {
		Protocol r = Protocol::Null;
		protocols.TryGet(name, r);
		return r;
	}
	FileSystemProtocolHandler* FileSystem::GetProtocolHandler(Protocol protocol) const {
		SharedPtr<FileSystemProtocolHandler> p{};
		if (protocolHandlers.TryGet(protocol, p)) {
			return p.GetRaw();
		} else {
			return nullptr;
		}
	}

	void FileSystem::AddProtocol(const String& name, FileSystem::Protocol protocol) {
		protocols.Set(name, protocol);
	}
	void FileSystem::AddProtocolHandler(Protocol protocol, UniquePtr<FileSystemProtocolHandler>&& handler) {
		protocolHandlers.Set(protocol, SharedPtr<FileSystemProtocolHandler>(handler.Release()));
	}
}
