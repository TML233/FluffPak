#pragma once

#include "Platform/Windows/Include.h"
#include "Engine/System/String.h"
#include "Engine/System/Exception.h"
#include "Engine/System/UniquePtr.h"

using namespace Engine;

namespace Platform {
	class UnicodeHelper final{
	public:
		static UniquePtr<WCHAR[]> UTF8ToUnicode(const String& string);
		static String UnicodeToUTF8(WCHAR* unicode);
	};
}