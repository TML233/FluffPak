#ifndef HEADER_CORE_SYSTEM_DATETIME
#define HEADER_CORE_SYSTEM_DATETIME

#include "Engine/System/Definition.h"
#include "Engine/System/TimeSpan.h"

namespace Engine {
	struct DateTime {
	public:
		static DateTime GetNow();
		static DateTime GetNowUtc();
		
	private:
		uint64 time;
		int16 offsetMinutes;
	};
}

#endif