#pragma once

#define NOMINMAX
#define WINDOWS_LEAN_AND_MEAN
//#define NOGDI

#define UNICODE
#define _UNICODE
#include <Windows.h>

#undef NOMINMAX
#undef WINDOWS_LEAN_AND_MEAN
#undef NOGDI

#undef SendMessage
#undef GetObject
#undef CreateWindow