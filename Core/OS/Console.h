#ifndef HEADER_CORE_SYSTEM_CONSOLE
#define HEADER_CORE_SYSTEM_CONSOLE

#include <cstdio>
#include "Core/Definition.h"


namespace Core {
	class Console final{
	public:
		static Console& GetInstance();

		virtual ~Console();
		void Print(const String& text) const;
		void PrintLine() const;
		void PrintLine(const String& text) const;

		// Actual printing
		virtual void Print(Char* text) const;
	};
}

#endif