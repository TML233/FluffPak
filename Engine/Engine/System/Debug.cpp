#include "Engine/System/Debug.h"
#include "Engine/System/String.h"

void DebugPrintInfo(const char* message) {
	std::printf("%s\n", message);
}
void DebugPrintWarn(const char* func, const char* file, int line, const char* message) {
	std::printf("\033[93m[WARNING] %s:\033[39m %s\n          \033[33mAt:\033[90m %s\033[39m::\033[90m%d\033[0m\n", func, message, file, line);
}
void DebugPrintError(const char* func, const char* file, int line, const char* message) {
	std::printf("\033[91m[ ERROR ] %s:\033[39m %s\n          \033[31mAt:\033[90m %s\033[39m::\033[90m%d\033[0m\n", func, message, file, line);
}
void DebugPrintErrorAssert(const char* func, const char* file, int line, const char* expr,const char* message,const char* action) {
	std::printf("\033[91m[ ERROR ] %s:\033[39m Assertion \"%s\" failed: %s Executing \"%s\".\n          \033[31mAt:\033[90m %s\033[39m::\033[90m%d\033[0m\n", func, expr,message,action, file, line);
}
void DebugPrintFatal(const char* func, const char* file, int line, const char* message) {
	std::printf("\033[97;101m[ FATAL ] %s:\033[39;49m %s\n          \033[37;41mAt:\033[90;49m %s\033[39m::\033[90m%d\033[0m\n", func, message, file, line);
}
void DebugPrintFatalAssert(const char* func, const char* file, int line, const char* expr, const char* message) {
	std::printf("\033[97;101m[ FATAL ] %s:\033[39;49m Assertion \"%s\" failed: %s Crashing.\n          \033[37;41mAt:\033[90;49m %s\033[39m::\033[90m%d\033[0m\n", func, expr, message, file, line);
}