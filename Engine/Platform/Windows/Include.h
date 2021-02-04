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

#endif