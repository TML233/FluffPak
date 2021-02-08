#pragma once

#include "Engine/System/String.h"

namespace Engine {
	class Console final{
	public:
		// FUCK YOU WINDOWS
		static void Initialize();

		static void Print(const String& text);
		static void PrintLine(const String& text);
		static void PrintLine();
	private:
		// Actual printing happens here.
		static void PrintRaw(const char* text);
	};
}