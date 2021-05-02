#pragma once
#include "Engine/System/Definition.h"

#define DEBUG_ENABLED true

#if(DEBUG_ENABLED)
#define DEBUG_TEXT(text) text
#else
#define DEBUG_TEXT(text) ""
#endif

#define INFO_MSG(msg) ::Engine::DebugPrintInfo(msg)

#define WARN_MSG(msg) ::Engine::DebugPrintWarn(__func__,__FILE__,__LINE__,DEBUG_TEXT(msg))

#define ERR_MSG(msg) ::Engine::DebugPrintError(__func__,__FILE__,__LINE__,DEBUG_TEXT(msg))
#define ERR_ASSERT(expr,msg,action) if (!(expr)) { ::Engine::DebugPrintErrorAssert(__func__,__FILE__,__LINE__,#expr,msg,#action);action;}

#ifdef _MSC_VER
#define FATAL_CRASH_IMMEDIATELY() __debugbreak()
#else
#define FATAL_CRASH_IMMEDIATELY() __builtin_trap()
#endif
#define FATAL_MSG(msg) ::Engine::DebugPrintFatal(__func__,__FILE__,__LINE__,DEBUG_TEXT(msg))
#define FATAL_CRASH(msg) FATAL_MSG(msg);FATAL_CRASH_IMMEDIATELY()
#define FATAL_ASSERT(expr,msg) if (!(expr)) { ::Engine::DebugPrintFatalAssert(__func__,__FILE__,__LINE__,#expr,msg);FATAL_CRASH_IMMEDIATELY();}

namespace Engine {
	void DebugPrintInfo(const u8char* message);
	void DebugPrintWarn(const char* func, const char* file, int line, const u8char* message);
	void DebugPrintError(const char* func, const char* file, int line, const u8char* message);
	void DebugPrintErrorAssert(const char* func, const char* file, int line, const char* expr, const u8char* message, const char* action);
	void DebugPrintFatal(const char* func, const char* file, int line, const u8char* message);
	void DebugPrintFatalAssert(const char* func, const char* file, int line, const char* expr, const u8char* message);
}