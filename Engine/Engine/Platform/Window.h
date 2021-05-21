#pragma once
#include "Engine/Platform/Definition.h"

#if CURRENT_PLATFORM_WINDOWS
#	include "Engine/Platform/Windows/Window.h"
#elif CURRENT_PLATFORM_LINUX
#   include "Engine/Platform/Linux/Window.h"
#endif