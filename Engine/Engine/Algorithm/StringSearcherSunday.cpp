#include "Engine/Algorithm/StringSearcherSunday.h"
#include <memory>

namespace Engine {
	int32 StringSearcherSunday::Search(const char* target, int32 lenTarget, const char* pattern, int32 lenPattern) {
		//Prepare charPos
		std::memset(charPos, -1, 256*sizeof(int32));
		for (int32 i = 0; i < lenPattern; i += 1) {
			byte index = 0;
			memcpy(&index, pattern + i, sizeof(byte));
			charPos[index] = i;
		}

		int32 pos = 0;
		for (int32 i = 0; i < lenPattern; i += 1) {
			if (pos > lenTarget-1) {
				// Not found
				return -1;
			}
			if (target[pos + i] != pattern[i]) {
				int32 checkpos = pos + lenPattern;
				if (checkpos > lenTarget-1) {
					// Not found
					return -1;
				} else {
					byte checking = 0;
					std::memcpy(&checking, target+checkpos, sizeof(byte));
					int32 cpos = charPos[checking];
					if (cpos < 0) {
						pos += lenPattern+1;
					} else {
						pos += checkpos - (pos + cpos);
					}
					i = -1;
				}
			}
		}
		return pos;
	}
}
