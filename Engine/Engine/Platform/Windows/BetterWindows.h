#pragma once

#define NOMINMAX
#define WINDOWS_LEAN_AND_MEAN
//#define NOGDI

#define UNICODE
#define _UNICODE
#include <Windows.h>

#undef SendMessage
#undef GetObject
#undef CreateWindow
#undef AddJob
#undef GetJob
