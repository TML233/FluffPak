#ifndef HEADER_CORE_SYSTEM_TIMESPAN
#define HEADER_CORE_SYSTEM_TIMESPAN

#include "Engine/System/Definition.h"

namespace Engine {
	struct TimeSpan {
		TimeSpan(int64 ticks);

		static constexpr int64 TicksPerMillisecond = 10000;
		static constexpr int64 TicksPerSecond = TicksPerMillisecond * 1000;
		static constexpr int64 TicksPerMinute = TicksPerSecond * 60;
		static constexpr int64 TicksPerHour = TicksPerMinute * 60;
		static constexpr int64 TicksPerDay = TicksPerHour * 24;

		int64 GetTicks() const;
		void SetTicks(int64 ticks);

		int32 GetMilliseconds() const;
		int32 GetSeconds() const;
		int32 GetMinutes() const;
		int32 GetHours() const;
		int32 GetDays() const;
		
		double GetTotalMilliseconds() const;
		double GetTotalSeconds() const;
		double GetTotalMinutes() const;
		double GetTotalHours() const;
		double GetTotalDays() const;

		TimeSpan operator+(const TimeSpan& obj) const;
		TimeSpan operator-(const TimeSpan& obj) const;
	private:
		int64 ticks = 0;
	};
}

#endif