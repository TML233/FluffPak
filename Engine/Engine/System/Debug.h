#pragma once
#include "Engine/System/Definition.h"
#include <source_location>

#define DEBUG_ENABLED true

#define INFO_MSG(msg) ::Engine::DebugPrintInfo(msg)

#define WARN_MSG(msg) ::Engine::DebugPrintWarn(std::source_location::current().function_name(),std::source_location::current().file_name(),std::source_location::current().line(),msg)

#define ERR_MSG(msg) ::Engine::DebugPrintError(std::source_location::current().function_name(),std::source_location::current().file_name(),std::source_location::current().line(),msg)
#define ERR_ASSERT(expr,msg,action) if (!(expr)) { ::Engine::DebugPrintErrorAssert(std::source_location::current().function_name(),std::source_location::current().file_name(),std::source_location::current().line(),#expr,msg,#action);action;}

#ifdef _MSC_VER
#define FATAL_CRASH_IMMEDIATELY() __debugbreak()
#else
#define FATAL_CRASH_IMMEDIATELY() __builtin_trap()
#endif
#define FATAL_MSG(msg) ::Engine::DebugPrintFatal(std::source_location::current().function_name(),std::source_location::current().file_name(),std::source_location::current().line(),msg)
#define FATAL_CRASH(msg) FATAL_MSG(msg);FATAL_CRASH_IMMEDIATELY()
#define FATAL_ASSERT(expr,msg) if (!(expr)) { ::Engine::DebugPrintFatalAssert(std::source_location::current().function_name(),std::source_location::current().file_name(),std::source_location::current().line(),#expr,msg);FATAL_CRASH_IMMEDIATELY();}

namespace Engine {
	void DebugPrintInfo(const u8char* message);
	void DebugPrintWarn(const char* func, const char* file, int line, const u8char* message);
	void DebugPrintError(const char* func, const char* file, int line, const u8char* message);
	void DebugPrintErrorAssert(const char* func, const char* file, int line, const char* expr, const u8char* message, const char* action);
	void DebugPrintFatal(const char* func, const char* file, int line, const u8char* message);
	void DebugPrintFatalAssert(const char* func, const char* file, int line, const char* expr, const u8char* message);
}