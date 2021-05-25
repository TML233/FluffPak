#pragma once
#include "Engine/System/File/FileStream.h"
#include "Engine/System/File/FileProtocol.h"
#include <cstdio>

namespace Engine {
	class std::error_code;

	class FileProtocolNative final :public FileProtocol {
		REFLECTION_CLASS(::Engine::FileProtocolNative, ::Engine::FileProtocol) {}

	public:
		bool IsFileExists(const String& path) const override;
		bool IsDirectoryExists(const String& path) const override;

		FileSystem::Result CreateFile(const String& path) override;
		FileSystem::Result CreateDirectory(const String& path) override;

		ResultPair<FileSystem::Result, IntrusivePtr<FileStream>> OpenFile(const String& path, FileSystem::OpenMode mode) override;

		FileSystem::Result RemoveFile(const String& path) override;
		FileSystem::Result RemoveDirectory(const String& path) override;

		FileSystem::Result GetAllFiles(const String& path, List<String>& result) const override;
		FileSystem::Result GetAllDirectories(const String& path, List<String>& result) const override;

	private:
		static FileSystem::Result GetResult(const std::error_code& err);
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

		FileSystem::Result SetPosition(int64 position) override;
		int64 GetPosition() const override;
		int64 GetLength() const override;

		FileSystem::Result WriteBytes(const byte* valuePtr, int64 length) override;
		FileSystem::Result WriteBytesEndian(const byte* valuePtr, int64 length);

		FileSystem::Result WriteByte(byte value) override;
		FileSystem::Result WriteBytes(const List<byte>& value) override;
		FileSystem::Result WriteSByte(sbyte value) override;
		FileSystem::Result WriteInt16(int16 value) override;
		FileSystem::Result WriteUInt16(uint16 value) override;
		FileSystem::Result WriteInt32(int32 value) override;
		FileSystem::Result WriteUInt32(uint32 value) override;
		FileSystem::Result WriteInt64(int64 value) override;
		FileSystem::Result WriteUInt64(uint64 value) override;
		FileSystem::Result WriteFloat(float value) override;
		FileSystem::Result WriteDouble(double value) override;
		FileSystem::Result WriteString(const String& value) override;
		FileSystem::Result WriteStringLine(const String& value) override;

		//Actual reading things.
		int64 ReadBytes(int64 length, List<byte>& result) override;

		byte ReadByte() override;
		sbyte ReadSByte() override;
		int16 ReadInt16() override;
		uint16 ReadUInt16() override;
		int32 ReadInt32() override;
		uint32 ReadUInt32() override;
		int64 ReadInt64() override;
		uint64 ReadUInt64() override;
		float ReadFloat() override;
		double ReadDouble() override;
		String ReadStringLine() override;
		String ReadAllString() override;

	private:
		std::FILE* file;
		bool valid = true;
		bool canRead;
		bool canWrite;
	};
}
