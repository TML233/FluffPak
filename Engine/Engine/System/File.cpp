#include "Engine/System/File.h"

namespace Engine{
#pragma region Protocols
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

		String protocolName = STRING_LITERAL("file");
		int32 rIndex = 0;

		if (index >= 0) {
			protocolName = path.Substring(0, index);
			rIndex = index + prefix.GetCount();
		}

		Protocol rProtocol = GetProtocol(protocolName);

		return ResultPair<Protocol, int32>(rProtocol, rIndex);
	}

	FileSystem::Protocol FileSystem::GetProtocol(const String& name) const {
		Protocol r = Protocol::Null;
		protocols.TryGet(name, r);
		return r;
	}
	FileProtocolHandler* FileSystem::GetProtocolHandler(Protocol protocol) const {
		SharedPtr<FileProtocolHandler> p{};
		if (protocolHandlers.TryGet(protocol, p)) {
			return p.GetRaw();
		} else {
			return nullptr;
		}
	}

	void FileSystem::AddProtocol(const String& name, FileSystem::Protocol protocol) {
		protocols.Set(name, protocol);
	}
	void FileSystem::AddProtocolHandler(Protocol protocol, UniquePtr<FileProtocolHandler>&& handler) {
		handler->owner = this;
		protocolHandlers.Set(protocol, SharedPtr<FileProtocolHandler>(handler.Release()));
	}

#pragma endregion

#pragma region Open modes
	bool FileSystem::IsOpenModeValid(OpenMode mode) {
		return (mode >= OpenMode::ReadOnly && mode <= OpenMode::ReadWriteAppend);
	}
	bool FileSystem::IsOpenModeReadOnly(OpenMode mode) {
		return mode == OpenMode::ReadOnly;
	}
	bool FileSystem::IsOpenModeRead(OpenMode mode) {
		switch (mode) {
			case OpenMode::ReadOnly:
			case OpenMode::ReadWriteTruncate:
			case OpenMode::ReadWriteAppend:
				return true;
			default:
				return false;
		}
	}
	bool FileSystem::IsOpenModeWrite(OpenMode mode) {
		switch (mode) {
			case OpenMode::WriteTruncate:
			case OpenMode::WriteAppend:
			case OpenMode::ReadWriteTruncate:
			case OpenMode::ReadWriteAppend:
				return true;
			default:
				return false;
		}
	}
	bool FileSystem::IsOpenModeTruncate(OpenMode mode) {
		switch (mode) {
			case OpenMode::WriteTruncate:
			case OpenMode::ReadWriteTruncate:
				return true;
			default:
				return false;
		}
	}
	bool FileSystem::IsOpenModeAppend(OpenMode mode) {
		switch (mode) {
			case OpenMode::WriteAppend:
			case OpenMode::ReadWriteAppend:
				return true;
			default:
				return false;
		}
	}
#pragma endregion

	FileSystem::FileSystem() {
		using Handler = UniquePtr<FileProtocolHandler>;
		AddProtocol(STRING_LITERAL("file"), Protocol::File);
		//AddProtocolHandler(Protocol::File, Handler(MEMNEW(...));
		AddProtocol(STRING_LITERAL("res"), Protocol::Resource);
		//AddProtocolHandler(Protocol::Resource, Handler(MEMNEW(...));
		AddProtocol(STRING_LITERAL("user"), Protocol::User);
		//AddProtocolHandler(Protocol::User, Handler(MEMNEW(...));
	}

#pragma region File operations
	ResultPair<FileSystem::Result, bool> FileSystem::IsFileExists(const String& path) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return ResultBool(Result::InvalidProtocol, false));

		FileProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->IsFileExists(handlerPath);
	}
	ResultPair<FileSystem::Result, bool> FileSystem::IsDirectoryExists(const String& path) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return ResultBool(Result::InvalidProtocol, false));

		FileProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->IsDirectoryExists(handlerPath);
	}

	FileSystem::Result FileSystem::CreateFile(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return Result::InvalidProtocol);

		FileProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->CreateFile(handlerPath);
	}
	FileSystem::Result FileSystem::CreateDirectory(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return Result::InvalidProtocol);

		FileProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->CreateDirectory(handlerPath);
	}

	ResultPair<FileSystem::Result, IntrusivePtr<File>> FileSystem::OpenFile(const String& path, OpenMode mode) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return ResultFile(Result::InvalidProtocol, IntrusivePtr<File>(nullptr)));

		FileProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->OpenFile(handlerPath, mode);
	}

	FileSystem::Result FileSystem::RemoveFile(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return Result::InvalidProtocol);

		FileProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->RemoveFile(handlerPath);
	}
	FileSystem::Result FileSystem::RemoveDirectory(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return Result::InvalidProtocol);

		FileProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->RemoveDirectory(handlerPath);
	}

	FileSystem::Result FileSystem::GetAllFiles(const String& path, List<String>& result) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return Result::InvalidProtocol);

		FileProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->GetAllFiles(handlerPath, result);
	}
	FileSystem::Result FileSystem::GetAllDirectories(const String& path, List<String>& result) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return Result::InvalidProtocol);

		FileProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->GetAllDirectories(handlerPath, result);
	}
#pragma endregion
}
