#include "Core/System/Console.h"
#include "Platform/Platform.h"
#include <cstdio>

namespace Core {
	void Console::Initialize() {
#ifdef OS_WINDOWS
		// Make fucking Windows display UTF-8 properly.
		SetConsoleOutputCP(65001);
		setvbuf(stdout, nullptr, _IOFBF, 1000);
#endif
	}
	void Console::Print(const String& text) {
		//TODO: Richtext.
		PrintRaw(text.GetRawArray());
	}
	void Console::PrintLine(const String& text) {
		Print(text);
		PrintLine();
	}
	void Console::PrintLine() {
		PrintRaw("\n");
	}
	void Console::PrintRaw(const char* text) {
		printf(text);
	}
}