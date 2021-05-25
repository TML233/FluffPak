#pragma once
#include "Engine/System/String.h"
#include "Engine/System/Object/Object.h"
#include "Engine/System/Collection/Dictionary.h"


namespace Engine {
	class FileProtocol;
	class FileStream;

	class FileSystem final {
	public:
		FileSystem();

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

#pragma region File operations
		/// @brief Check if the file exists.
		bool IsFileExists(const String& path) const;
		/// @brief Check if the directory exists.
		bool IsDirectoryExists(const String& path) const;

		/// @brief Create a empty file at the given path.
		ResultCode CreateFile(const String& path);
		/// @brief Create a empty directory at the given path.
		ResultCode CreateDirectory(const String& path);

		/// @brief Open the file at the given path with the given mode.\n
		/// If the mode is read-only, and the file doesn't exists, the operation will fail.
		ResultPair<IntrusivePtr<FileStream>> OpenFile(const String& path, OpenMode mode);

		/// @brief Delete a file.
		ResultCode RemoveFile(const String& path);
		/// @brief Delete a directory.
		ResultCode RemoveDirectory(const String& path);

		/// @brief Enumerate all files only in the given path.
		ResultCode GetAllFiles(const String& path, List<String>& result) const;
		/// @brief Enumerate all directories only in the given path.
		ResultCode GetAllDirectories(const String& path, List<String>& result) const;
#pragma endregion

	private:
		void AddProtocol(const String& name, Protocol protocol);
		void AddProtocolHandler(Protocol protocol, UniquePtr<FileProtocol>&& handler);

		struct SplitData {
			SplitData(Protocol protocol, int32 index) :protocol(protocol), index(index) {}
			Protocol protocol;
			int32 index;
		};
		SplitData GetSplitData(const String& path) const;

		Dictionary<String, Protocol> protocols{};
		Dictionary<Protocol, SharedPtr<FileProtocol>> protocolHandlers{};
	};
}
