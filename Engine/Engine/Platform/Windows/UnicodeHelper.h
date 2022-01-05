#pragma once

#include "Engine/Platform/Windows/BetterWindows.h"
#include "Engine/System/String.h"
#include "Engine/System/Memory/UniquePtr.h"

namespace Engine::PlatformSpecific {
	class UnicodeHelper final {
	public:
		static bool UTF8ToUnicode(const String& string, UniquePtr<WCHAR[]>& result);
		static bool UnicodeToUTF8(WCHAR* unicode, String& result);
	};
}