#include "Core/OS/Console.h"
#include "Platform/Platform.h"
#include "Core/System/String.h"

namespace Core {
	Console::Console(){
#ifdef OS_WINDOWS
		// Set code page to CP_UTF8 (65001)
		SetConsoleOutputCP(65001);
		setvbuf(stdout, nullptr, _IOFBF, 1000);
#endif
	}

	void Console::Print(Char* text)const {
		std::printf(text);
	}
	void Console::Print(const String& text) const {
		Print(text.GetRawArray());
	}
	void Console::PrintLine() const {
		Print("\n");
	}
	void Console::PrintLine(const String& text) const {
		Print(text);
		PrintLine();
	}
}