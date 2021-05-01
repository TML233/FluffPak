#pragma once
#include "Engine/System/String.h"
#include "Engine/System/Object.h"
#include "Engine/Collection/Dictionary.h"

namespace Engine {
	class FileSystem;

	class File:public ReferencedObject {
		REFLECTION_CLASS(File, ReferencedObject) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		virtual ~File() = default;

	};

	/// @brief Protocol handler abstract layer for FileSystem.
	class FileSystemProtocolHandler {
	public:
		virtual ~FileSystemProtocolHandler() = default;
		virtual ResultPair<FileSystem::Result, bool> IsFileExists(const String& path) const = 0;
		virtual ResultPair<FileSystem::Result, bool> IsDirectoryExists(const String& path) const = 0;

		virtual FileSystem::Result CreateFile(const String& path)=0;
		virtual FileSystem::Result CreateDirectory(const String& path)=0;

		virtual ResultPair<FileSystem::Result, ReferencePtr<File>> OpenFile(const String& path, FileSystem::OpenMode mode)=0;

		virtual FileSystem::Result RemoveFile(const String& path) = 0;
		virtual FileSystem::Result RemoveDirectory(const String& path) = 0;

		virtual FileSystem::Result GetAllFiles(const String& path, List<String>& result) const = 0;
		virtual FileSystem::Result GetAllDirectories(const String& path,List<String>& result) const = 0;

	private:
		friend class FileSystem;
		FileSystem* owner = nullptr;
	};

	class FileSystem final {
	public:
		FileSystem();

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
			Null=-1,
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
		FileSystemProtocolHandler* GetProtocolHandler(Protocol protocol) const;
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
		ResultPair<Result, ReferencePtr<File>> OpenFile(const String& path, OpenMode mode);

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
		using ResultFile = ResultPair<Result, ReferencePtr<File>>;

		void AddProtocol(const String& name, Protocol protocol);
		void AddProtocolHandler(Protocol protocol, UniquePtr<FileSystemProtocolHandler>&& handler);

		ResultPair<Protocol, int32> GetSplitData(const String& path) const;
		
		Dictionary<String, Protocol> protocols{};
		Dictionary<Protocol, SharedPtr<FileSystemProtocolHandler>> protocolHandlers{};
	};
}