#pragma once
#include "Engine/System/String.h"
#include "Engine/System/Object/Object.h"
#include "Engine/System/Collection/Dictionary.h"
#include <bit>


namespace Engine {
	class FileProtocol;
	class FileStream;

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
			/// @brief Open mode is invalid.
			InvalidOpenMode,
			/// @brief File or directory is not found.
			NotFound,
			/// @brief Not enough permission for the operation.
			NoPermission,
			/// @brief File or directory is already exists.
			AlreadyExists,
			/// @brief FileStream is invalid, e.g. FileStream is already closed.
			InvalidStream,
		};

#pragma region Protocols
		enum class Protocol {
			/// @brief Invalid protocol
			Null,
			/// @brief Files on the local machine filesystem.
			Native,
			/// @brief Files in the resource directory.
			Resource,
			/// @brief Files in the user presistent data directory.
			Persistent,
			/// @brief End of the protocol enum.
			End,
		};
		static bool IsProtocolValid(Protocol protocol);
		/// @brief Get the protocol enum item by its name.
		/// @return Protocol::Null when not found. Use IsProtocolValid to check.
		Protocol GetProtocol(const String& name) const;
		/// @brief Get a protocol handler.
		/// @return nullptr when not found.
		FileProtocol* GetProtocolHandler(Protocol protocol) const;
#pragma endregion

#pragma region Open modes
		enum class OpenMode :byte {
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

		using ResultFile = ResultPair<Result, IntrusivePtr<FileStream>>;
#pragma region File operations
		/// @brief Check if the file exists.
		bool IsFileExists(const String& path) const;
		/// @brief Check if the directory exists.
		bool IsDirectoryExists(const String& path) const;

		/// @brief Create a empty file at the given path.
		Result CreateFile(const String& path);
		/// @brief Create a empty directory at the given path.
		Result CreateDirectory(const String& path);

		/// @brief Open the file at the given path with the given mode.\n
		/// If the mode is read-only, and the file doesn't exists, the operation will fail.
		ResultFile OpenFile(const String& path, OpenMode mode);

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

		void AddProtocol(const String& name, Protocol protocol);
		void AddProtocolHandler(Protocol protocol, UniquePtr<FileProtocol>&& handler);

		ResultPair<Protocol, int32> GetSplitData(const String& path) const;

		Dictionary<String, Protocol> protocols{};
		Dictionary<Protocol, SharedPtr<FileProtocol>> protocolHandlers{};
	};
}
