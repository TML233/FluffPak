#include "Engine/System/File/FileSystem.h"
#include "Engine/System/File/Protocol/Native.h"

namespace Engine {
	bool FileSystem::IsProtocolValid(Protocol protocol) {
		return protocol > Protocol::Null && protocol < Protocol::End;
	}
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

	FileSystem::FileSystem() {
		using Handler = UniquePtr<FileProtocol>;
		AddProtocol(STRING_LITERAL("file"), Protocol::Native);
		AddProtocolHandler(Protocol::Native, Handler(MEMNEW(FileProtocolNative)));
		AddProtocol(STRING_LITERAL("res"), Protocol::Resource);
		//AddProtocolHandler(Protocol::Resource, Handler(MEMNEW(...));
		AddProtocol(STRING_LITERAL("user"), Protocol::Persistent);
		//AddProtocolHandler(Protocol::User, Handler(MEMNEW(...));
	}

#pragma region Protocols
	FileSystem::SplitData FileSystem::GetSplitData(const String& path) const {
		const String prefix = STRING_LITERAL("://");

		int32 index = path.IndexOf(prefix);

		String protocolName;
		int32 rIndex = 0;

		if (index >= 0) {
			protocolName = path.Substring(0, index);
			rIndex = index + prefix.GetCount();
		}

		Protocol rProtocol = protocolName.GetCount() > 0 ? GetProtocol(protocolName) : Protocol::Native;

		return SplitData(rProtocol, rIndex);
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

	void FileSystem::AddProtocol(const String& name, Protocol protocol) {
		protocols.Set(name, protocol);
	}
	void FileSystem::AddProtocolHandler(Protocol protocol, UniquePtr<FileProtocol>&& handler) {
		handler->owner = this;
		protocolHandlers.Set(protocol, SharedPtr<FileProtocol>(handler.Release()));
	}
#pragma endregion

#pragma region File operations
	bool FileSystem::IsFileExists(const String& path) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.protocol), u8"Invalid path protocol!", return false);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->IsFileExists(handlerPath);
	}
	bool FileSystem::IsDirectoryExists(const String& path) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.protocol), u8"Invalid path protocol!", return false);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->IsDirectoryExists(handlerPath);
	}

	ResultCode FileSystem::CreateFile(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->CreateFile(handlerPath);
	}
	ResultCode FileSystem::CreateDirectory(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->CreateDirectory(handlerPath);
	}

	ResultCode FileSystem::TryOpenFile(const String& path, OpenMode mode, IntrusivePtr<FileStream>& result) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->TryOpenFile(handlerPath, mode, result);
	}

	ResultCode FileSystem::RemoveFile(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->RemoveFile(handlerPath);
	}
	ResultCode FileSystem::RemoveDirectory(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->RemoveDirectory(handlerPath);
	}

	ResultCode FileSystem::GetAllFiles(const String& path, List<String>& result) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->GetAllFiles(handlerPath, result);
	}
	ResultCode FileSystem::GetAllDirectories(const String& path, List<String>& result) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->GetAllDirectories(handlerPath, result);
	}
#pragma endregion
}
