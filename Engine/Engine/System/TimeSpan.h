#ifndef HEADER_CORE_SYSTEM_TIMESPAN
#define HEADER_CORE_SYSTEM_TIMESPAN

#include "Engine/System/Definition.h"

namespace Engine {
	struct TimeSpan {
		TimeSpan(Long ticks);

		static const Long TicksPerMillisecond;
		static const Long TicksPerSecond;
		static const Long TicksPerMinute;
		static const Long TicksPerHour;
		static const Long TicksPerDay;

		Long GetTicks() const;
		void SetTicks(Long ticks);

		Int GetMilliseconds() const;
		Int GetSeconds() const;
		Int GetMinutes() const;
		Int GetHours() const;
		Int GetDays() const;
		
		Double GetTotalMilliseconds() const;
		Double GetTotalSeconds() const;
		Double GetTotalMinutes() const;
		Double GetTotalHours() const;
		Double GetTotalDays() const;

		TimeSpan operator+(const TimeSpan& obj) const;
		TimeSpan operator-(const TimeSpan& obj) const;
	private:
		Long ticks = 0;
	};
}

#endif