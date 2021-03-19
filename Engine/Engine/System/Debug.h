#pragma once

#define DEBUG_ENABLED true

#if(DEBUG_ENABLED)
#define DEBUG_TEXT(text) text
#else
#define DEBUG_TEXT(text) ""
#endif

#define INFO_MSG(msg) DebugPrintInfo(msg)

#define WARN_MSG(msg) DebugPrintWarn(__func__,__FILE__,__LINE__,DEBUG_TEXT(msg))

#define ERR_MSG(msg) DebugPrintError(__func__,__FILE__,__LINE__,DEBUG_TEXT(msg))
#define ERR_ASSERT(expr,msg) if (!(expr)) { ERR_MSG("Assertion \"" #expr "\" failed: " msg);}
#define ERR_ASSERT_ACTION(expr,msg,action) if (!(expr)) { ERR_MSG("Assertion \"" #expr "\" failed: " msg " Executing \"" #action "\".");action;}

#ifdef _MSC_VER
#define FATAL_CRASH_IMMEDIATELY() __debugbreak()
#else
#define FATAL_CRASH_IMMEDIATELY() __builtin_trap()
#endif
#define FATAL_MSG(msg) DebugPrintFatal(__func__,__FILE__,__LINE__,DEBUG_TEXT(msg))
#define FATAL_CRASH(msg) FATAL_MSG(msg);FATAL_CRASH_IMMEDIATELY()


void DebugPrintInfo(const char* message);
void DebugPrintWarn(const char* func, const char* file, int line, const char* message);
void DebugPrintError(const char* func, const char* file, int line, const char* message);
void DebugPrintFatal(const char* func, const char* file, int line, const char* message);
