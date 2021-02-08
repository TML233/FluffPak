#pragma once

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