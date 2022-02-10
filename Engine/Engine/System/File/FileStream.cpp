#include "Engine/System/File/FileStream.h"

namespace Engine{
	bool FileStream::CanRandomAccess() const {
		return true;
	}
}
