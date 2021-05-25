#include "Engine/System/File/Protocol/Native.h"
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

	bool FileProtocolNative::IsFileExists(const String& path) const {
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

	bool FileProtocolNative::IsDirectoryExists(const String& path) const {
		std::error_code err;
		auto st = fs::status(fs::u8path(path.GetStringView()), err);
		if (!fs::status_known(st)) {
			return false;
		}

		return st.type() == fs::file_type::directory;
	}

	FileSystem::Result FileProtocolNative::CreateFile(const String& path) {
		if (IsFileExists(path)) {
			return FileSystem::Result::AlreadyExists;
		}
		FILE* f = fopen((char*)path.ToIndividual().GetRawArray(), "wb");
		ERR_ASSERT(f != nullptr, u8"Failed to create file!", return FileSystem::Result::UnknownError);

		fclose(f);
		return FileSystem::Result::OK;
	}

	FileSystem::Result FileProtocolNative::CreateDirectory(const String& path) {
		std::error_code err;
		fs::create_directories(fs::u8path(path.GetStringView()), err);
		return GetResult(err);
	}

	FileSystem::ResultFile FileProtocolNative::OpenFile(const String& path, FileSystem::OpenMode mode) {
		ERR_ASSERT(FileSystem::IsOpenModeValid(mode), u8"mode is invalid!", return FileSystem::ResultFile(FileSystem::Result::InvalidOpenMode, IntrusivePtr<FileStream>(nullptr)));
		// make sure the file exists when read-only.
		if (FileSystem::IsOpenModeReadOnly(mode)) {
			ERR_ASSERT(
				IsFileExists(path), u8"Attemped to open a non-existing file in read-only mode!",
				return FileSystem::ResultFile(FileSystem::Result::NotFound, IntrusivePtr<FileStream>(nullptr));
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
			return FileSystem::ResultFile(FileSystem::Result::UnknownError, IntrusivePtr<FileStream>(nullptr))
		);

		auto fileStream = IntrusivePtr<FileStreamNative>::Create(file, mode);
		return ResultPair<FileSystem::Result, IntrusivePtr<FileStream>>(FileSystem::Result::OK, fileStream);
	}

	FileSystem::Result FileProtocolNative::RemoveFile(const String& path) {
		std::error_code err;
		fs::remove(fs::u8path(path.GetStringView()), err);
		return GetResult(err);
	}

	FileSystem::Result FileProtocolNative::RemoveDirectory(const String& path) {
		std::error_code err;
		fs::remove_all(fs::u8path(path.GetStringView()), err);
		return GetResult(err);
	}

	FileSystem::Result FileProtocolNative::GetAllFiles(const String& path, List<String>& result) const {
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

	FileSystem::Result FileProtocolNative::GetAllDirectories(const String& path, List<String>& result) const {
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
	FileStreamNative::FileStreamNative(std::FILE* file, FileSystem::OpenMode openMode) :file(file),
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

	FileSystem::Result FileStreamNative::SetPosition(int64 position) {
		ERR_ASSERT(IsValid(), u8"Attempted to operate an invalid FileStream!", return FileSystem::Result::InvalidStream);

		int result = std::fseek(file, position, SEEK_SET);
		ERR_ASSERT(result == 0, u8"Failed to set current pointer position.", return FileSystem::Result::UnknownError);
		return FileSystem::Result::OK;
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

		int64 length = (int64)std::ftell(file) + 1;
		std::fseek(file, origPos, SEEK_SET);

		return length;
	}

	FileSystem::Result FileStreamNative::WriteBytes(const byte* valuePtr, int64 length) {
		ERR_ASSERT(IsValid(), u8"Attempted to operate an invalid FileStream!", return FileSystem::Result::InvalidStream);
		ERR_ASSERT(CanWrite(), u8"This FileStream cannot write.", return FileSystem::Result::NoPermission);

		fwrite(valuePtr, sizeof(byte), length, file);
		return FileSystem::Result::OK;
	}
	FileSystem::Result FileStreamNative::WriteBytesEndian(const byte* valuePtr, int64 length) {
		ERR_ASSERT(IsValid(), u8"Attempted to operate an invalid FileStream!", return FileSystem::Result::InvalidStream);
		ERR_ASSERT(CanWrite(), u8"This FileStream cannot write.", return FileSystem::Result::NoPermission);

		if (FileSystem::LocalEndianness == GetCurrentEndianness()) {
			return WriteBytes(valuePtr, length);
		}

		for (int64 i = length - 1; i >= 0; i += 1) {
			fwrite(valuePtr + i, sizeof(byte), 1, file);
		}
		return FileSystem::Result::OK;
	}

	FileSystem::Result FileStreamNative::WriteByte(byte value) {
		return WriteBytes(&value, sizeof(byte));
	}
	FileSystem::Result FileStreamNative::WriteBytes(const List<byte>& value) {
		return WriteBytes(value.begin().GetPointer(), value.GetCount());
	}
	FileSystem::Result FileStreamNative::WriteSByte(sbyte value) {
		return WriteBytes((byte*)&value, sizeof(sbyte));
	}
	FileSystem::Result FileStreamNative::WriteInt16(int16 value) {
		return WriteBytesEndian((byte*)&value, sizeof(int16));
	}
	FileSystem::Result FileStreamNative::WriteUInt16(uint16 value) {
		return WriteBytesEndian((byte*)&value, sizeof(uint16));
	}
	FileSystem::Result FileStreamNative::WriteInt32(int32 value) {
		return WriteBytesEndian((byte*)&value, sizeof(int32));
	}
	FileSystem::Result FileStreamNative::WriteUInt32(uint32 value) {
		return WriteBytesEndian((byte*)&value, sizeof(uint32));
	}
	FileSystem::Result FileStreamNative::WriteInt64(int64 value) {
		return WriteBytesEndian((byte*)&value, sizeof(int64));
	}
	FileSystem::Result FileStreamNative::WriteUInt64(uint64 value) {
		return WriteBytesEndian((byte*)&value, sizeof(uint64));
	}
	FileSystem::Result FileStreamNative::WriteFloat(float value) {
		return WriteBytesEndian((byte*)&value, sizeof(float));
	}
	FileSystem::Result FileStreamNative::WriteDouble(double value) {
		return WriteBytesEndian((byte*)&value, sizeof(double));
	}
	FileSystem::Result FileStreamNative::WriteString(const String& value) {
		return WriteBytes((byte*)value.GetStartPtr(), value.GetCount());
	}
	FileSystem::Result FileStreamNative::WriteStringLine(const String& value) {
		auto result = WriteString(value);
		WriteByte((byte)'\n');
		return result;
	}

	
#pragma endregion

}