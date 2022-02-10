#include "Engine/System/File/Protocol/Native.h"
#include <filesystem>
#include "Engine/System/Collection/List.h"

namespace fs = std::filesystem;

namespace Engine {
#pragma region Protocol
	ResultCode FileProtocolNative::GetResult(const std::error_code& err) {
		if (err == std::errc::permission_denied) {
			return ResultCode::NoPermission;
		} else if (err == std::errc::no_such_file_or_directory) {
			return ResultCode::NotFound;
		} else if (err == std::errc::file_exists) {
			return ResultCode::AlreadyExists;
		} else {
			return ResultCode::OK;
		}
	}

	bool FileProtocolNative::IsFileExists(const String& path) const {
		std::error_code err;
		auto st = fs::status(fs::path(path.GetU8StringView()), err);
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

	bool FileProtocolNative::IsDirectoryExists(const String& path) const {
		std::error_code err;
		auto st = fs::status(fs::path(path.GetU8StringView()), err);
		if (!fs::status_known(st)) {
			return false;
		}

		return st.type() == fs::file_type::directory;
	}

	ResultCode FileProtocolNative::CreateFile(const String& path) {
		if (IsFileExists(path)) {
			return ResultCode::AlreadyExists;
		}
		FILE* f = std::fopen((char*)path.ToIndividual().GetRawArray(), "wb");
		ERR_ASSERT(f != nullptr, u8"Failed to create file!", return ResultCode::UnknownError);

		fclose(f);
		return ResultCode::OK;
	}

	ResultCode FileProtocolNative::CreateDirectory(const String& path) {
		std::error_code err;
		fs::create_directories(fs::path(path.GetU8StringView()), err);
		return GetResult(err);
	}

	ResultCode FileProtocolNative::TryOpenFile(const String& path, FileSystem::OpenMode mode, IntrusivePtr<FileStream>& result) {
		ERR_ASSERT(FileSystem::IsOpenModeValid(mode), u8"mode is invalid!", return ResultCode::InvalidArgument);
		// make sure the file exists when read-only.
		if (FileSystem::IsOpenModeReadOnly(mode)) {
			ERR_ASSERT(
				IsFileExists(path), u8"Attemped to open a non-existing file in read-only mode!",
				return ResultCode::NotFound
			);
		}
		static const char* modes[] = { // See FileSystem::OpenMode, must be matched
			"rb",	//ReadOnly
			"wb",	//WriteTruncate
			"wab",	//WriteAppend
			"w+b",	//ReadWriteTruncate
			"w+ab"	//ReadWriteAppend
		};
		FILE* file = std::fopen((char*)path.ToIndividual().GetRawArray(), modes[(byte)mode]);
		ERR_ASSERT(file != nullptr, u8"Failed to open the file!",
			return ResultCode::UnknownError
		);

		result = IntrusivePtr<FileStreamNative>::Create(file, mode);
		return ResultCode::OK;
	}

	ResultCode FileProtocolNative::RemoveFile(const String& path) {
		std::error_code err;
		fs::remove(fs::path(path.GetU8StringView()), err);
		return GetResult(err);
	}

	ResultCode FileProtocolNative::RemoveDirectory(const String& path) {
		std::error_code err;
		fs::remove_all(fs::path(path.GetU8StringView()), err);
		return GetResult(err);
	}

	ResultCode FileProtocolNative::GetAllFiles(const String& path, List<String>& result) const {
		if (!IsDirectoryExists(path)) {
			return ResultCode::NotFound;
		}

		auto iter = fs::directory_iterator(fs::path(path.GetU8StringView()));
		for (const auto& one : iter) {
			if (!one.is_directory()) {
				result.Add(one.path().u8string());
			}
		}
		return ResultCode::OK;
	}

	ResultCode FileProtocolNative::GetAllDirectories(const String& path, List<String>& result) const {
		if (!IsDirectoryExists(path)) {
			return ResultCode::NotFound;
		}

		auto iter = fs::directory_iterator(fs::path(path.GetU8StringView()));
		for (const auto& one : iter) {
			if (one.is_directory()) {
				result.Add(one.path().u8string());
			}
		}
		return ResultCode::OK;
	}
#pragma endregion

#pragma region File
	FileStreamNative::FileStreamNative(std::FILE* file, FileSystem::OpenMode openMode) :
		file(file),
		canRead(FileSystem::IsOpenModeRead(openMode)),
		canWrite(FileSystem::IsOpenModeWrite(openMode)) {}

	FileStreamNative::~FileStreamNative() {
		Close();
	}

	void FileStreamNative::Close(){
		if (!IsValid()) {
			return;
		}
		std::fclose(file);
		valid = false;
	}
	bool FileStreamNative::IsValid() const {
		return valid;
	}
	bool FileStreamNative::CanRead() const {
		return canRead;
	}
	bool FileStreamNative::CanWrite() const {
		return canWrite;
	}

	ResultCode FileStreamNative::SetPosition(int64 position) {
		ERR_ASSERT(IsValid(), u8"Attempted to operate an invalid FileStream!", return ResultCode::InvalidStream);

		int result = std::fseek(file, position, SEEK_SET);
		ERR_ASSERT(result == 0, u8"Failed to set current pointer position.", return ResultCode::UnknownError);
		return ResultCode::OK;
	}
	int64 FileStreamNative::GetPosition() const {
		ERR_ASSERT(IsValid(), u8"Attempted to operate an invalid FileStream!", return -1);

		int64 pos = std::ftell(file);
		ERR_ASSERT(pos>=0, u8"Failed to get current pointer position.", return -1);
		return pos;
	}
	int64 FileStreamNative::GetLength() const {
		ERR_ASSERT(IsValid(), u8"Attempted to operate an invalid FileStream!", return -1);

		int64 origPos = std::ftell(file);

		int seeked = std::fseek(file, 0, SEEK_END);
		ERR_ASSERT(seeked == 0, u8"Failed to seek to the end.", return -1);

		int64 length = (int64)std::ftell(file);
		std::fseek(file, origPos, SEEK_SET);

		return length;
	}

	ResultCode FileStreamNative::WriteBytesUnchecked(const byte* valuePtr, int32 length) {
		fwrite(valuePtr, sizeof(byte), length, file);
		return ResultCode::OK;
	}

	ResultCode FileStreamNative::TryReadBytesUnchecked(int32 length, int32& readCount, List<byte>& result) {
		readCount = (int32)fread(result.GetRawElementPtr() + result.GetCount() - length, sizeof(byte), length, file);
		return ResultCode::OK;
	}
#pragma endregion

}