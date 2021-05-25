#include "Engine/System/File/FileStream.h"

namespace Engine{
	FileSystem::Endianness FileStream::GetCurrentEndianness() const {
		return currentEndianness;
	}
	void FileStream::SetCurrentEndianness(FileSystem::Endianness endianness) {
		currentEndianness = endianness;
	}
}
