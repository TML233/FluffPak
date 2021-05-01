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
		enum class OpenMode {
			/// @brief Read-only.
			Read,
			/// @brief Write-only, clears the file.
			WriteTruncate,
			/// @brief Write-only, append to the file.
			WriteAppend,
			/// @brief Read-write, clears the file.
			ReadWriteTruncate,
			/// @brief Read-write, append to the file.
			ReadWriteAppend
		};
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

		virtual ~File() = default;

	};

	/// @brief Protocol handler abstract layer for FileSystem.
	class FileSystemProtocolHandler {
	public:
		virtual ~FileSystemProtocolHandler() = default;
		virtual ResultPair<File::Result, bool> IsFileExists(const String& path) const = 0;
		virtual ResultPair<File::Result, bool> IsDirectoryExists(const String& path) const = 0;

		virtual File::Result CreateFile(const String& path)=0;
		virtual File::Result CreateDirectory(const String& path)=0;

		virtual ResultPair<File::Result, ReferencePtr<File>> OpenFile(const String& path, File::OpenMode mode)=0;

		virtual File::Result RemoveFile(const String& path) = 0;
		virtual File::Result RemoveDirectory(const String& path) = 0;

		virtual File::Result GetAllFiles(const String& path, List<String>& result) = 0;
		virtual File::Result GetAllDirectories(const String& path, List<String>& result) = 0;

	private:
		friend class FileSystem;
		FileSystem* owner = nullptr;
	};

	class FileSystem final {
	public:
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

		FileSystem();

		/// @brief Check if the file exists.
		ResultPair<File::Result, bool> IsFileExists(const String& path) const;
		/// @brief Check if the directory exists.
		ResultPair<File::Result, bool> IsDirectoryExists(const String& path) const;
		
		/// @brief Create a empty file at the given path.
		File::Result CreateFile(const String& path);
		/// @brief Create a empty directory at the given path.
		File::Result CreateDirectory(const String& path);
		
		/// @brief Open the file at the given path. Will fail if the file doesn't exist.
		ResultPair<File::Result, ReferencePtr<File>> OpenFile(const String& path, File::OpenMode mode);

		/// @brief Delete a file.
		File::Result RemoveFile(const String& path);
		/// @brief Delete a directory.
		File::Result RemoveDirectory(const String& path);

		Protocol GetProtocol(const String& name) const;
		FileSystemProtocolHandler* GetProtocolHandler(Protocol protocol) const;

	private:
		using ResultBool = ResultPair<File::Result, bool>;
		using ResultFile = ResultPair<File::Result, ReferencePtr<File>>;

		void AddProtocol(const String& name, Protocol protocol);
		void AddProtocolHandler(Protocol protocol, UniquePtr<FileSystemProtocolHandler>&& handler);

		ResultPair<Protocol, int32> GetSplitData(const String& path) const;
		
		Dictionary<String, Protocol> protocols{};
		Dictionary<Protocol, SharedPtr<FileSystemProtocolHandler>> protocolHandlers{};
	};
}