#include "Engine/System/File/FileStream.h"

namespace Engine{
	bool FileStream::IsOpenModeValid(OpenMode mode) {
		return (mode >= OpenMode::ReadOnly && mode <= OpenMode::ReadWriteAppend);
	}
	bool FileStream::IsOpenModeReadOnly(OpenMode mode) {
		return mode == OpenMode::ReadOnly;
	}
	bool FileStream::IsOpenModeRead(OpenMode mode) {
		switch (mode) {
			case OpenMode::ReadOnly:
			case OpenMode::ReadWriteTruncate:
			case OpenMode::ReadWriteAppend:
				return true;
			default:
				return false;
		}
	}
	bool FileStream::IsOpenModeWrite(OpenMode mode) {
		switch (mode) {
			case OpenMode::WriteTruncate:
			case OpenMode::WriteAppend:
			case OpenMode::ReadWriteTruncate:
			case OpenMode::ReadWriteAppend:
				return true;
			default:
				return false;
		}
	}
	bool FileStream::IsOpenModeTruncate(OpenMode mode) {
		switch (mode) {
			case OpenMode::WriteTruncate:
			case OpenMode::ReadWriteTruncate:
				return true;
			default:
				return false;
		}
	}
	bool FileStream::IsOpenModeAppend(OpenMode mode) {
		switch (mode) {
			case OpenMode::WriteAppend:
			case OpenMode::ReadWriteAppend:
				return true;
			default:
				return false;
		}
	}

	bool FileStream::CanRandomAccess() const {
		return true;
	}
}
