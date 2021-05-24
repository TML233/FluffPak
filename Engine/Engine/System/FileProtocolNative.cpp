#include "Engine/System/FileProtocolNative.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace Engine {
#pragma region Protocol
	FileSystem::Result FileProtocolNative::GetResult(const std::error_code& err) {
		if (err == std::errc::permission_denied) {
			return FileSystem::Result::NoPermission;
		} else if (err == std::errc::no_such_file_or_directory) {
			return FileSystem::Result::NotFound;
		} else if (err == std::errc::file_exists) {
			return FileSystem::Result::AlreadyExists;
		} else {
			return FileSystem::Result::OK;
		}
	}

	bool Engine::FileProtocolNative::IsFileExists(const String& path) const {
		std::error_code err;
		auto st = fs::status(fs::u8path(path.GetStringView()), err);
		if (!fs::status_known(st)) {
			return false;
		}

		auto type = st.type();
		if (type == fs::file_type::not_found) {
			return false;
		}
		if (type == fs::file_type::directory) {
			return false;
		}
		return true;
	}

	bool Engine::FileProtocolNative::IsDirectoryExists(const String& path) const {
		std::error_code err;
		auto st = fs::status(fs::u8path(path.GetStringView()), err);
		if (!fs::status_known(st)) {
			return false;
		}

		return st.type() == fs::file_type::directory;
	}

	FileSystem::Result Engine::FileProtocolNative::CreateFile(const String& path) {
		return FileSystem::Result();
	}

	FileSystem::Result Engine::FileProtocolNative::CreateDirectory(const String& path) {
		std::error_code err;
		fs::create_directories(fs::u8path(path.GetStringView()), err);
		return GetResult(err);
	}

	ResultPair<FileSystem::Result, IntrusivePtr<File>> Engine::FileProtocolNative::OpenFile(const String& path, FileSystem::OpenMode mode) {
		return ResultPair<FileSystem::Result, IntrusivePtr<File>>(FileSystem::Result::OK, IntrusivePtr<File>());
	}

	FileSystem::Result Engine::FileProtocolNative::RemoveFile(const String& path) {
		std::error_code err;
		fs::remove(fs::u8path(path.GetStringView()), err);
		return GetResult(err);
	}

	FileSystem::Result Engine::FileProtocolNative::RemoveDirectory(const String& path) {
		std::error_code err;
		fs::remove_all(fs::u8path(path.GetStringView()), err);
		return GetResult(err);
	}

	FileSystem::Result Engine::FileProtocolNative::GetAllFiles(const String& path, List<String>& result) const {
		if (!IsDirectoryExists(path)) {
			return FileSystem::Result::NotFound;
		}

		auto iter = fs::directory_iterator(fs::u8path(path.GetStringView()));
		for (const auto& one : iter) {
			if (!one.is_directory()) {
				result.Add(one.path().u8string());
			}
		}
		return FileSystem::Result::OK;
	}

	FileSystem::Result Engine::FileProtocolNative::GetAllDirectories(const String& path, List<String>& result) const {
		if (!IsDirectoryExists(path)) {
			return FileSystem::Result::NotFound;
		}

		auto iter = fs::directory_iterator(fs::u8path(path.GetStringView()));
		for (const auto& one : iter) {
			if (one.is_directory()) {
				result.Add(one.path().u8string());
			}
		}
		return FileSystem::Result::OK;
	}
#pragma endregion

#pragma region File
	FileNative::FileNative(std::fstream&& fstream, FileSystem::OpenMode openMode) :fstream(std::move(fstream)),
		canRead(FileSystem::IsOpenModeRead(openMode)),
		canWrite(FileSystem::IsOpenModeWrite(openMode)) {}

	FileNative::~FileNative() {
		Close();
	}

	void FileNative::Close(){
		if (!IsValid()) {
			return;
		}
		fstream.close();
	}
	bool FileNative::IsValid() const {
		return fstream.is_open();
	}
	bool FileNative::CanRead() const {
		return canRead;
	}
	bool FileNative::CanWrite() const {
		return canWrite;
	}

	FileSystem::Result FileNative::SetPosition(int64 position) {
		
	}
	ResultPair<FileSystem::Result, int64> FileNative::GetPosition() const {

	}
	ResultPair<FileSystem::Result, int64> FileNative::GetLength() const {

	}

	FileSystem::Result FileNative::WriteByte(byte value) {

	}
	FileSystem::Result FileNative::WriteBytes(const List<byte>& value) {

	}
	FileSystem::Result FileNative::WriteBytes(byte* value, int32 length) {

	}
	FileSystem::Result FileNative::WriteSByte(sbyte value) {

	}
	FileSystem::Result FileNative::WriteInt16(int16 value) {

	}
	FileSystem::Result FileNative::WriteUInt16(uint16 value) {

	}
	FileSystem::Result FileNative::WriteInt32(int32 value) {

	}
	FileSystem::Result FileNative::WriteUInt32(uint32 value) {

	}
	FileSystem::Result FileNative::WriteInt64(int64 value) {

	}
	FileSystem::Result FileNative::WriteUInt64(uint64 value) {

	}
	FileSystem::Result FileNative::WriteFloat(int16 value) {

	}
	FileSystem::Result FileNative::WriteDouble(uint16 value) {

	}
	FileSystem::Result FileNative::WriteString(const String& value) {

	}
#pragma endregion

}