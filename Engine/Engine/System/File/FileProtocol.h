#pragma once
#include "Engine/System/File/FileSystem.h"

namespace Engine {
	/// @brief Protocol handler abstract layer for FileSystem.
	class FileProtocol :public ManualObject {
		REFLECTION_CLASS(::Engine::FileProtocol, ::Engine::ManualObject) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}
	public:
		virtual ~FileProtocol() = default;
		virtual bool IsFileExists(const String& path) const = 0;
		virtual bool IsDirectoryExists(const String& path) const = 0;

		virtual ResultCode CreateFile(const String& path) = 0;
		virtual ResultCode CreateDirectory(const String& path) = 0;

		virtual ResultPair<IntrusivePtr<FileStream>> OpenFile(const String& path, FileSystem::OpenMode mode) = 0;

		virtual ResultCode RemoveFile(const String& path) = 0;
		virtual ResultCode RemoveDirectory(const String& path) = 0;

		virtual ResultCode GetAllFiles(const String& path, List<String>& result) const = 0;
		virtual ResultCode GetAllDirectories(const String& path, List<String>& result) const = 0;

	private:
		friend class FileSystem;
		FileSystem* owner = nullptr;
	};
}
