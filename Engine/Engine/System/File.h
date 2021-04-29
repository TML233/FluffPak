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
			Read,
			WriteTruncate,
			WriteAppend,
			ReadWriteTruncate,
			ReadWriteAppend
		};
		enum class Result {
			/// @brief Succeeded.
			OK,
			/// @brief File is not found.
			NotFound,
			/// @brief Not enough permission for the operation.
			NoPermission,
			/// @brief File is already exists.
			AlreadyExists
		};

		virtual ~File() = default;


	private:
		friend class FileSystem;
	};

	class FileSystemProtocolHandler {
	public:
		virtual ~FileSystemProtocolHandler() = default;
		virtual ResultPair<File::Result, bool> IsExists(const String& path) const = 0;
		virtual File::Result Create(const String& path) = 0;
		virtual ResultPair<File::Result, ReferencePtr<File>> Open(const String& path, File::OpenMode mode);
		virtual File::Result Delete(const String& path) = 0;
	};

	class FileSystem final {
	public:
		enum class Protocol {
			/// @brief Files on the local machine filesystem.
			File,
			/// @brief Files in the resource directory.
			Resource,
			/// @brief Files in the user presistent data directory.
			User
		};

		FileSystem();

		/// @brief Check if the file exists.
		ResultPair<File::Result, bool> IsExists(const String& path) const;
		
		/// @brief Create a empty file at the given path.
		File::Result Create(const String& path);
		
		/// @brief Open the file at the given path. Will fail if the file doesn't exist.
		ResultPair<File::Result, ReferencePtr<File>> Open(const String& path, File::OpenMode mode);

		/// @brief Delete a file or a whole directory.
		File::Result Delete(const String& path);

		ResultPair<bool, Protocol> GetProtocol(const String& name) const;
		FileSystemProtocolHandler* GetProtocolHandler(Protocol protocol) const;
	private:
		void AddProtocol(const String& name, Protocol protocol);
		void AddProtocolHandler(Protocol protocol, UniquePtr<FileSystemProtocolHandler>&& handler);

		Dictionary<String, Protocol> protocols{};
		Dictionary<Protocol, SharedPtr<FileSystemProtocolHandler>> protocolHandlers{};
	};
}