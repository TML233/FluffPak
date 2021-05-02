#include "Engine/Platform/Windows/UnicodeHelper.h"

using namespace Engine;
namespace Engine::PlatformSpecific::Windows {
	bool UnicodeHelper::UTF8ToUnicode(const String& string,UniquePtr<WCHAR[]>& result) {
		int32 len = MultiByteToWideChar(CP_UTF8, NULL, reinterpret_cast<const char*>(string.GetRawArray()), -1, NULL, 0);
		ERR_ASSERT(len > 0, u8"MultiByteToWideChar failed to calculate required WCHAR[] length!", return false);
		
		UniquePtr<WCHAR[]> wchar = UniquePtr<WCHAR[]>::Create(len);

		int32 converted = MultiByteToWideChar(CP_UTF8, NULL, reinterpret_cast<const char*>(string.GetRawArray()), string.GetCount() + 1, wchar.GetRaw(), len);
		ERR_ASSERT(converted > 0, u8"MultiByteToWideChar failed to convert!", return false);

		result.Reset(wchar.Release());
		return true;
	}

	bool UnicodeHelper::UnicodeToUTF8(WCHAR* unicode,String& result) {
		int32 len = WideCharToMultiByte(CP_UTF8, NULL, unicode, -1, NULL, 0, NULL, NULL);
		ERR_ASSERT(len > 0, u8"WideCharToMultiByte failed to calculate required char[] length!", return false);
		
		UniquePtr<strchar[]> chars = UniquePtr<strchar[]>::Create(len);

		int32 converted = WideCharToMultiByte(CP_UTF8, NULL, unicode, -1, reinterpret_cast<char*>(chars.GetRaw()), len, NULL, NULL);
		ERR_ASSERT(converted > 0, u8"WideCharToMultiByte failed to convert!", return false);

		result = String(ReferencePtr<String::ContentData>::Create(Memory::Move(chars), len));
		return true;
	}
}