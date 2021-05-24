#pragma once
#include "Engine/System/File.h"
#include <fstream>

namespace Engine {
	class std::error_code;

	class FileProtocolNative final :public FileProtocol {
		REFLECTION_CLASS(::Engine::FileProtocolNative, ::Engine::FileProtocol) {}

	public:
		bool IsFileExists(const String& path) const override;
		bool IsDirectoryExists(const String& path) const override;

		FileSystem::Result CreateFile(const String& path) override;
		FileSystem::Result CreateDirectory(const String& path) override;

		ResultPair<FileSystem::Result, IntrusivePtr<File>> OpenFile(const String& path, FileSystem::OpenMode mode) override;

		FileSystem::Result RemoveFile(const String& path) override;
		FileSystem::Result RemoveDirectory(const String& path) override;

		FileSystem::Result GetAllFiles(const String& path, List<String>& result) const override;
		FileSystem::Result GetAllDirectories(const String& path, List<String>& result) const override;

	private:
		static FileSystem::Result GetResult(const std::error_code& err);
	};

	class FileNative final : public File {
		REFLECTION_CLASS(::Engine::FileNative, ::Engine::File) {}

	public:
		FileNative(std::fstream&& fstream, FileSystem::OpenMode openMode);
		~FileNative();

		void Close() override;
		bool IsValid() const override;
		bool CanRead() const override;
		bool CanWrite() const override;

		FileSystem::Result SetPosition(int64 position) override;
		ResultPair<FileSystem::Result, int64> GetPosition() const override;
		ResultPair<FileSystem::Result, int64> GetLength() const override;

		FileSystem::Result WriteByte(byte value) override;
		FileSystem::Result WriteBytes(const List<byte>& value) override;
		FileSystem::Result WriteBytes(byte* value, int32 length) override;
		FileSystem::Result WriteSByte(sbyte value) override;
		FileSystem::Result WriteInt16(int16 value) override;
		FileSystem::Result WriteUInt16(uint16 value) override;
		FileSystem::Result WriteInt32(int32 value) override;
		FileSystem::Result WriteUInt32(uint32 value) override;
		FileSystem::Result WriteInt64(int64 value) override;
		FileSystem::Result WriteUInt64(uint64 value) override;
		FileSystem::Result WriteFloat(int16 value) override;
		FileSystem::Result WriteDouble(uint16 value) override;
		FileSystem::Result WriteString(const String& value) override;

	private:
		std::fstream fstream;
		bool canRead;
		bool canWrite;
	};
}
