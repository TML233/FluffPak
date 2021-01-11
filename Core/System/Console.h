#ifndef HEADER_CORE_SYSTEM_CONSOLE
#define HEADER_CORE_SYSTEM_CONSOLE

#include <iostream>
#include "Core/Definition.h"


namespace Core {
	class Console final{
	public:
		static void Initialize();
		static void Print(const String& text);
		static void PrintLine();
		static void PrintLine(const String& text);
	};
}

#endif