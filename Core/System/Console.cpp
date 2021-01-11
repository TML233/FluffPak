#include "Core/System/Console.h"
#include "Platform/Platform.h"
#include "Core/System/String.h"

namespace Core {
	void Console::Initialize(){
#ifdef OS_WINDOWS
		// Set code page to CP_UTF8 (65001)
		SetConsoleOutputCP(65001);
		setvbuf(stdout, nullptr, _IOFBF, 1000);
#endif
	}
	void Console::Print(const String& text) {
		std::cout << text.GetRawArray();
	}
	void Console::PrintLine() {
		std::cout << std::endl;
	}
	void Console::PrintLine(const String& text){
		Print(text);
		PrintLine();
	}
}