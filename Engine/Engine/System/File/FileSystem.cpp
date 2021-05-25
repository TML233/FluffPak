#include "Engine/System/File/FileSystem.h"
#include "Engine/System/File/FileProtocol.h"
#include "Engine/System/File/Protocol/Native.h"

namespace Engine {
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
		using Handler = UniquePtr<FileProtocol>;
		AddProtocol(STRING_LITERAL("file"), Protocol::Native);
		AddProtocolHandler(Protocol::Native, Handler(MEMNEW(FileProtocolNative)));
		AddProtocol(STRING_LITERAL("res"), Protocol::Resource);
		//AddProtocolHandler(Protocol::Resource, Handler(MEMNEW(...));
		AddProtocol(STRING_LITERAL("user"), Protocol::Persistent);
		//AddProtocolHandler(Protocol::User, Handler(MEMNEW(...));
	}

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

	ResultPair<IntrusivePtr<FileStream>> FileSystem::OpenFile(const String& path, OpenMode mode) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.protocol), u8"Invalid path protocol!", return ResultPair<IntrusivePtr<FileStream>>(ResultCode::InvalidArgument, IntrusivePtr<FileStream>(nullptr)));

		FileProtocol* handler = GetProtocolHandler(data.protocol);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.index, path.GetCount() - data.index);
		return handler->OpenFile(handlerPath, mode);
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
