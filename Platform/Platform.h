#ifndef HEADER_PLATFORM_PLATFORM
#define HEADER_PLATFORM_PLATFORM

//Platforms
#ifdef WIN32
#define OS_WINDOWS
#include "Platform/Windows/Windows.h"
#endif

#ifdef __linux__
#define OS_LINUX

#endif

#endif