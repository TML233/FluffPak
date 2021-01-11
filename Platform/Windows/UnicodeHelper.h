#ifndef HEADER_PLATFORM_WINDOWS_UNICODEHELPER
#define HEADER_PLATFORM_WINDOWS_UNICODEHELPER

#include "Platform/Windows/Include.h"
#include "Core/System/String.h"
#include "Core/System/Exception.h"

using namespace Core;

namespace Platform {
	class UnicodeHelper final{
	public:
		static std::unique_ptr<WCHAR[]> UTF8ToUnicode(const String& string);
		static String UnicodeToUTF8(WCHAR* unicode);
	};
}

#endif