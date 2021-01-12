#ifndef HEADER_PLATFORM_WINDOWS_INCLUDE
#define HEADER_PLATFORM_WINDOWS_INCLUDE

#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#undef WINDOWS_LEAN_AND_MEAN
#undef NOMINMAX

// Undefine Windows's shit
#undef GetMessage
#undef CreateWindow

// Define module dll export/import
// Export/import mode
#define PLATFORM_WINDOWS_DLL_EXPORT
//#define PLATFORM_WINDOWS_DLL_IMPORT

#ifdef PLATFORM_WINDOWS_DLL_EXPORT
#undef ENGINE_API
#define ENGINE_API __declspec(dllexport)
#endif
#ifdef PLATFORM_WINDOWS_DLL_IMPORT
#undef ENGINE_API
#define ENGINE_API __declspec(dllimport)
#endif

#endif