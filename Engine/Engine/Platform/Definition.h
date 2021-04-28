#pragma once
#include "Engine/System/Definition.h"

namespace Engine {
	enum class Platform :byte {
		NotSpecified,
		Windows,
		Linux,
		Android,
		MacOS,
		IOS,
	};
}

#define PLATFORM(x) ::Engine::Platform::x

#if defined(WIN32)
#	define CURRENT_PLATFORM_WINDOWS true
#	define CURRENT_PLATFORM PLATFORM(Windows)
#elif defined(__linux__)
#	define CURRENT_PLATFORM_LINUX true
#	define CURRENT_PLATFORM PLATFORM(Linux)
/*
#elif defined(__ANDROID__)
#	define CURRENT_PLATFORM_ANDROID true
#	define CURRENT_PLATFORM PLATFORM(Android)
*/
/*
#elif defined(__APPLE__)
#	include <TargetConditionals.h>
#	if TARGET_OS_MAC == 1
#		define CURRENT_PLATFORM_MACOS true
#		define CURRENT_PLATFORM PLATFORM(MacOS)
#	elif TARGET_OS_IPHONE == 1
#		define CURRENT_PLATFORM_IOS true
#		define CURRENT_PLATFORM PLATFORM(IOS)
#	elif
#		error The current unknown Apple platform is not supported!
*/
#else
#	error The current platform is not supported!
#endif

#undef PLATFORM
