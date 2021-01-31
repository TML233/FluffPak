#ifndef HEADER_CORE_SYSTEM_TIMESPAN
#define HEADER_CORE_SYSTEM_TIMESPAN

#include "Engine/System/Definition.h"

namespace Engine {
	struct TimeSpan {
		TimeSpan(Long ticks);

		static constexpr Long TicksPerMillisecond = 10000;
		static constexpr Long TicksPerSecond = TicksPerMillisecond * 1000;
		static constexpr Long TicksPerMinute = TicksPerSecond * 60;
		static constexpr Long TicksPerHour = TicksPerMinute * 60;
		static constexpr Long TicksPerDay = TicksPerHour * 24;

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