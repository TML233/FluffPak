#pragma once

#include "Engine/System/Definition.h"

namespace Engine {
	class StringSearcherSunday {
	public:
		int32 Search(const char* target, int32 lenTarget, const char* pattern, int32 lenPattern);
	private:
		int32 charPos[256] = { -1 };
	};
}