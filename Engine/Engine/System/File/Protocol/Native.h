#pragma once
#include "Engine/System/File/FileStream.h"
#include "Engine/System/File/FileProtocol.h"
#include <cstdio>
#include <system_error>

namespace Engine {
	class FileProtocolNative final :public FileProtocol {
		REFLECTION_CLASS(::Engine::FileProtocolNative, ::Engine::FileProtocol) {}

	public:
		bool IsFileExists(const String& path) const override;
		bool IsDirectoryExists(const String& path) const override;

		ResultCode CreateFile(const String& path) override;
		ResultCode CreateDirectory(const String& path) override;

		ResultPair<IntrusivePtr<FileStream>> OpenFile(const String& path, FileSystem::OpenMode mode) override;

		ResultCode RemoveFile(const String& path) override;
		ResultCode RemoveDirectory(const String& path) override;

		ResultCode GetAllFiles(const String& path, List<String>& result) const override;
		ResultCode GetAllDirectories(const String& path, List<String>& result) const override;

	private:
		static ResultCode GetResult(const std::error_code& err);
	};

	class FileStreamNative final : public FileStream {
		REFLECTION_CLASS(::Engine::FileStreamNative, ::Engine::FileStream) {}

	public:
		FileStreamNative(std::FILE* file, FileSystem::OpenMode openMode);
		~FileStreamNative();

		void Close() override;
		bool IsValid() const override;
		bool CanRead() const override;
		bool CanWrite() const override;

		ResultCode SetPosition(int64 position) override;
		int64 GetPosition() const override;
		int64 GetLength() const override;

		ResultCode WriteBytes(const byte* valuePtr, int64 length) override;
		ResultCode WriteBytesEndian(const byte* valuePtr, int64 length) override;
		int64 ReadBytes(int64 length, List<byte>& result) override;
		int64 ReadBytesEndian(int64 length, List<byte>& result) override;


	private:
		std::FILE* file;
		bool valid = true;
		bool canRead;
		bool canWrite;
	};
}
