#ifndef HEADER_PLATFORM_WINDOWS_UNICODEHELPER
#define HEADER_PLATFORM_WINDOWS_UNICODEHELPER

#include "Platform/Windows/Include.h"
#include "Core/System/String.h"
#include "Core/System/Exception.h"
#include "Core/System/UniquePtr.h"

using namespace Core;

namespace Platform {
	class UnicodeHelper final{
	public:
		static UniquePtr<WCHAR[]> UTF8ToUnicode(const String& string);
		static String UnicodeToUTF8(WCHAR* unicode);
	};
}

#endif