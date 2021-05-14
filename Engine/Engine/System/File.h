#pragma once
#include "Engine/System/String.h"
#include "Engine/System/Object/Object.h"
#include "Engine/System/Collection/Dictionary.h"
#include <bit>

namespace Engine {
	class FileSystem;
	class FileProtocolHandler;
	class File;

	class FileSystem final {
	public:
		FileSystem();

		enum class Endianness {
			Little,
			Big
		};
		static constexpr Endianness LocalEndianness = (std::endian::native == std::endian::little ? Endianness::Little : Endianness::Big);

		enum class Result {
			/// @brief Succeeded.
			OK,
			/// @brief Error that cannot be categoried.
			UnknownError,
			/// @brief Protocol doesn't exist.
			InvalidProtocol,
			/// @brief File or directory is not found.
			NotFound,
			/// @brief Not enough permission for the operation.
			NoPermission,
			/// @brief File or directory is already exists.
			AlreadyExists
		};

#pragma region Protocols
		enum class Protocol {
			/// @brief Invalid protocol
			Null,
			/// @brief Files on the local machine filesystem.
			File,
			/// @brief Files in the resource directory.
			Resource,
			/// @brief Files in the user presistent data directory.
			User,
			/// @brief End of the protocol enum.
			End,
		};
		static bool IsProtocolValid(Protocol protocol);
		/// @brief Get the protocol enum item by its name.
		/// @return Protocol::Null when not found. Use IsProtocolValid to check.
		Protocol GetProtocol(const String& name) const;
		/// @brief Get a protocol handler.
		/// @return nullptr when not found.
		FileProtocolHandler* GetProtocolHandler(Protocol protocol) const;
#pragma endregion

#pragma region Open modes
		enum class OpenMode {
			/// @brief Read-only.
			ReadOnly,
			/// @brief Write-only, clears the file.
			WriteTruncate,
			/// @brief Write-only, append to the file.
			WriteAppend,
			/// @brief Read-write, clears the file.
			ReadWriteTruncate,
			/// @brief Read-write, append to the file.
			ReadWriteAppend
		};

		static bool IsOpenModeValid(OpenMode mode);
		static bool IsOpenModeReadOnly(OpenMode mode);
		static bool IsOpenModeRead(OpenMode mode);
		static bool IsOpenModeWrite(OpenMode mode);
		static bool IsOpenModeTruncate(OpenMode mode);
		static bool IsOpenModeAppend(OpenMode mode);
#pragma endregion

#pragma region File operations
		/// @brief Check if the file exists.
		ResultPair<Result, bool> IsFileExists(const String& path) const;
		/// @brief Check if the directory exists.
		ResultPair<Result, bool> IsDirectoryExists(const String& path) const;

		/// @brief Create a empty file at the given path.
		Result CreateFile(const String& path);
		/// @brief Create a empty directory at the given path.
		Result CreateDirectory(const String& path);

		/// @brief Open the file at the given path. Will fail if the file doesn't exist.
		ResultPair<Result, IntrusivePtr<File>> OpenFile(const String& path, OpenMode mode);

		/// @brief Delete a file.
		Result RemoveFile(const String& path);
		/// @brief Delete a directory.
		Result RemoveDirectory(const String& path);

		/// @brief Enumerate all files only in the given path.
		FileSystem::Result GetAllFiles(const String& path, List<String>& result) const;
		/// @brief Enumerate all directories only in the given path.
		FileSystem::Result GetAllDirectories(const String& path, List<String>& result) const;
#pragma endregion

	private:
		using ResultBool = ResultPair<Result, bool>;
		using ResultFile = ResultPair<Result, IntrusivePtr<File>>;

		void AddProtocol(const String& name, Protocol protocol);
		void AddProtocolHandler(Protocol protocol, UniquePtr<FileProtocolHandler>&& handler);

		ResultPair<Protocol, int32> GetSplitData(const String& path) const;

		Dictionary<String, Protocol> protocols{};
		Dictionary<Protocol, SharedPtr<FileProtocolHandler>> protocolHandlers{};
	};

	/// @brief Protocol handler abstract layer for FileSystem.
	class FileProtocolHandler:public ManualObject {
		REFLECTION_CLASS(::Engine::FileProtocolHandler, ::Engine::ManualObject) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}
	public:
		virtual ~FileProtocolHandler() = default;
		virtual ResultPair<FileSystem::Result, bool> IsFileExists(const String& path) const = 0;
		virtual ResultPair<FileSystem::Result, bool> IsDirectoryExists(const String& path) const = 0;

		virtual FileSystem::Result CreateFile(const String& path)=0;
		virtual FileSystem::Result CreateDirectory(const String& path)=0;

		virtual ResultPair<FileSystem::Result, IntrusivePtr<File>> OpenFile(const String& path, FileSystem::OpenMode mode)=0;

		virtual FileSystem::Result RemoveFile(const String& path) = 0;
		virtual FileSystem::Result RemoveDirectory(const String& path) = 0;

		virtual FileSystem::Result GetAllFiles(const String& path, List<String>& result) const = 0;
		virtual FileSystem::Result GetAllDirectories(const String& path,List<String>& result) const = 0;

	private:
		friend class FileSystem;
		FileSystem* owner = nullptr;
	};

	class File :public ReferencedObject {
		REFLECTION_CLASS(::Engine::File, ::Engine::ReferencedObject) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		virtual ~File() = default;

		/// @brief Get current endianness.\n
		/// This only indicates that how this File instance deal with data.\n
		/// Does not check whether the original file is small-endian or big-endian. (And there's no way.)
		/// Default endianness will always be small-endian,
		FileSystem::Endianness GetCurrentEndianness();
		/// @brief Set current endianness.\n
		/// This affects how this File instance deal with data.
		/// Default endianness will always be small-endian.
		void SetCurrentEndianness(FileSystem::Endianness endian);

		virtual void Close() = 0;
		virtual bool IsValid() const = 0;
		virtual bool CanRead() const = 0;
		virtual bool CanWrite() const = 0;

		virtual FileSystem::Result SetPosition(int64 position) = 0;
		virtual ResultPair<FileSystem::Result, int64> GetPosition() const = 0;
		virtual ResultPair<FileSystem::Result, int64> GetLength() const = 0;

		virtual FileSystem::Result WriteByte(byte value) = 0;
		virtual FileSystem::Result WriteBytes(const List<byte>& value) = 0;
		virtual FileSystem::Result WriteBytes(byte* value, int32 length) = 0;
		virtual FileSystem::Result WriteSByte(sbyte value) = 0;
		virtual FileSystem::Result WriteInt16(int16 value) = 0;
		virtual FileSystem::Result WriteUInt16(uint16 value) = 0;
		virtual FileSystem::Result WriteInt32(int32 value) = 0;
		virtual FileSystem::Result WriteUInt32(uint32 value) = 0;
		virtual FileSystem::Result WriteInt64(int64 value) = 0;
		virtual FileSystem::Result WriteUInt64(uint64 value) = 0;
		virtual FileSystem::Result WriteFloat(int16 value) = 0;
		virtual FileSystem::Result WriteDouble(uint16 value) = 0;
		virtual FileSystem::Result WriteString(const String& value) = 0;

	private:
		FileSystem::Endianness currentEndianness = FileSystem::Endianness::Little;
	};

}