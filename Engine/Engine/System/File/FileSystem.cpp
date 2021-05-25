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
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return false);

		FileProtocol* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->IsFileExists(handlerPath);
	}
	bool FileSystem::IsDirectoryExists(const String& path) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return false);

		FileProtocol* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->IsDirectoryExists(handlerPath);
	}

	FileSystem::Result FileSystem::CreateFile(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return Result::InvalidProtocol);

		FileProtocol* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->CreateFile(handlerPath);
	}
	FileSystem::Result FileSystem::CreateDirectory(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return Result::InvalidProtocol);

		FileProtocol* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->CreateDirectory(handlerPath);
	}

	FileSystem::ResultFile FileSystem::OpenFile(const String& path, OpenMode mode) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return ResultFile(Result::InvalidProtocol, IntrusivePtr<FileStream>(nullptr)));

		FileProtocol* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->OpenFile(handlerPath, mode);
	}

	FileSystem::Result FileSystem::RemoveFile(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return Result::InvalidProtocol);

		FileProtocol* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->RemoveFile(handlerPath);
	}
	FileSystem::Result FileSystem::RemoveDirectory(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return Result::InvalidProtocol);

		FileProtocol* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->RemoveDirectory(handlerPath);
	}

	FileSystem::Result FileSystem::GetAllFiles(const String& path, List<String>& result) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return Result::InvalidProtocol);

		FileProtocol* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->GetAllFiles(handlerPath, result);
	}
	FileSystem::Result FileSystem::GetAllDirectories(const String& path, List<String>& result) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), u8"Invalid path protocol!", return Result::InvalidProtocol);

		FileProtocol* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, u8"Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->GetAllDirectories(handlerPath, result);
	}
#pragma endregion
}
