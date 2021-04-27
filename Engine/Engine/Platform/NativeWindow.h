#pragma once
#include "Engine/Platform/Definition.h"

#if CURRENT_PLATFORM_WINDOWS
#	include "Engine/Platform/Windows/NativeWindow.h"
#elif CURRENT_PLATFORM_LINUX
#   include "Engine/Platform/Linux/X11/NativeWindow.h"
#endif