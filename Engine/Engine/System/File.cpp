#include "Engine/System/File.h"

namespace Engine{
	bool FileSystem::IsProtocolValid(Protocol protocol) {
		return protocol > Protocol::Null && protocol < Protocol::End;
	}
	template<>
	static int32 ObjectUtil::GetHashCode<FileSystem::Protocol>(const FileSystem::Protocol& obj) {
		return (int32)obj;
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
		handler->owner = this;
		protocolHandlers.Set(protocol, SharedPtr<FileSystemProtocolHandler>(handler.Release()));
	}

	ResultPair<File::Result, bool> FileSystem::IsFileExists(const String& path) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), "Invalid path protocol!", return ResultBool(File::Result::InvalidProtocol, false));

		FileSystemProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, "Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->IsFileExists(handlerPath);
	}
	ResultPair<File::Result, bool> FileSystem::IsDirectoryExists(const String& path) const {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), "Invalid path protocol!", return ResultBool(File::Result::InvalidProtocol, false));

		FileSystemProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, "Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->IsDirectoryExists(handlerPath);
	}

	File::Result FileSystem::CreateFile(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), "Invalid path protocol!", return File::Result::InvalidProtocol);

		FileSystemProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, "Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->CreateFile(handlerPath);
	}
	File::Result FileSystem::CreateDirectory(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), "Invalid path protocol!", return File::Result::InvalidProtocol);

		FileSystemProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, "Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->CreateDirectory(handlerPath);
	}

	ResultPair<File::Result, ReferencePtr<File>> FileSystem::OpenFile(const String& path, File::OpenMode mode) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), "Invalid path protocol!", return ResultFile(File::Result::InvalidProtocol, ReferencePtr<File>(nullptr)));

		FileSystemProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, "Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->OpenFile(handlerPath, mode);
	}

	File::Result FileSystem::RemoveFile(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), "Invalid path protocol!", return File::Result::InvalidProtocol);

		FileSystemProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, "Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->RemoveFile(handlerPath);
	}
	File::Result FileSystem::RemoveDirectory(const String& path) {
		auto data = GetSplitData(path);
		ERR_ASSERT(IsProtocolValid(data.result), "Invalid path protocol!", return File::Result::InvalidProtocol);

		FileSystemProtocolHandler* handler = GetProtocolHandler(data.result);
		FATAL_ASSERT(handler != nullptr, "Protocol handler not found!");

		String handlerPath = path.Substring(data.value, path.GetCount() - data.value);
		return handler->RemoveDirectory(handlerPath);
	}
}
