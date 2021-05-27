#include "Engine/System/File/FileSystem.h"
#include "Engine/System/File/Protocol/Native.h"

namespace Engine {
	FileSystem::FileSystem() {
		using Handler = UniquePtr<FileProtocol>;
		AddProtocol(STRING_LITERAL("file"), FileProtocol::Protocol::Native);
		AddProtocolHandler(FileProtocol::Protocol::Native, Handler(MEMNEW(FileProtocolNative)));
		AddProtocol(STRING_LITERAL("res"), FileProtocol::Protocol::Resource);
		//AddProtocolHandler(Protocol::Resource, Handler(MEMNEW(...));
		AddProtocol(STRING_LITERAL("user"), FileProtocol::Protocol::Persistent);
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

		FileProtocol::Protocol rProtocol = protocolName.GetCount() > 0 ? GetProtocol(protocolName) : FileProtocol::Protocol::Native;

		return SplitData(rProtocol, rIndex);
	}

	FileProtocol::Protocol FileSystem::GetProtocol(const String& name) const {
		FileProtocol::Protocol r = FileProtocol::Protocol::Null;
		protocols.TryGet(name, r);
		return r;
	}
	FileProtocol* FileSystem::GetProtocolHandler(FileProtocol::Protocol protocol) const {
		SharedPtr<FileProtocol> p{};
		if (protocolHandlers.TryGet(protocol, p)) {
			return p.GetRaw();
		} else {
			return nullptr;
		}
	}

	void FileSystem::AddProtocol(const String& name, FileProtocol::Protocol protocol) {
		protocols.Set(name, protocol);
	}
	void FileSystem::AddProtocolHandler(FileProtocol::Protocol protocol, UniquePtr<FileProtocol>&& handler) {
		handler->owner = this;
		protocolHandlers.Set(protocol, SharedPtr<FileProtocol>(handler.Release()));
	}
#pragma endregion

#pragma region File operations
	bool FileSystem::IsFileExists(const String& path) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(FileProtocol::IsProtocolValid(data.protocol), u8"Invalid path protocol!", return false);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->IsFileExists(handlerPath);
	}
	bool FileSystem::IsDirectoryExists(const String& path) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(FileProtocol::IsProtocolValid(data.protocol), u8"Invalid path protocol!", return false);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->IsDirectoryExists(handlerPath);
	}

	ResultCode FileSystem::CreateFile(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(FileProtocol::IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->CreateFile(handlerPath);
	}
	ResultCode FileSystem::CreateDirectory(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(FileProtocol::IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->CreateDirectory(handlerPath);
	}

	ResultPair<IntrusivePtr<FileStream>> FileSystem::OpenFile(const String& path, FileStream::OpenMode mode) {
		auto data = GetSplitData(path);
		ERR_ASSERT(FileProtocol::IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultPair<IntrusivePtr<FileStream>>(ResultCode::InvalidArgument, IntrusivePtr<FileStream>(nullptr)));

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->OpenFile(handlerPath, mode);
	}

	ResultCode FileSystem::RemoveFile(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(FileProtocol::IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->RemoveFile(handlerPath);
	}
	ResultCode FileSystem::RemoveDirectory(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(FileProtocol::IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->RemoveDirectory(handlerPath);
	}

	ResultCode FileSystem::GetAllFiles(const String& path, List<String>& result) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(FileProtocol::IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->GetAllFiles(handlerPath, result);
	}
	ResultCode FileSystem::GetAllDirectories(const String& path, List<String>& result) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(FileProtocol::IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultCode::InvalidArgument);

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->GetAllDirectories(handlerPath, result);
	}
#pragma endregion
}
