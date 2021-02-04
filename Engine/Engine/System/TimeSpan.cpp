#include "Engine/System/TimeSpan.h"
#include "Engine/System/Exception.h"
#include "Engine/System/String.h"

namespace Engine {
	TimeSpan::TimeSpan(int64 ticks) :ticks(ticks) {}

	int64 TimeSpan::GetTicks() const {
		return ticks;
	}
	void TimeSpan::SetTicks(int64 ticks) {
		this->ticks = ticks;
	}

	int32 TimeSpan::GetMilliseconds() const {
		return (int32)((ticks / TicksPerMillisecond) % 1000);
	}
	int32 TimeSpan::GetSeconds() const {
		return (int32)((ticks / TicksPerSecond) % 60);
	}
	int32 TimeSpan::GetMinutes() const {
		return (int32)((ticks / TicksPerMinute) % 60);
	}
	int32 TimeSpan::GetHours() const {
		return (int32)((ticks / TicksPerHour) % 24);
	}
	int32 TimeSpan::GetDays() const {
		return (int32)(ticks / TicksPerDay);
	}

	double TimeSpan::GetTotalMilliseconds() const {
		return (double)ticks / TicksPerMillisecond;
	}
	double TimeSpan::GetTotalSeconds() const {
		return (double)ticks / TicksPerSecond;
	}
	double TimeSpan::GetTotalMinutes() const {
		return (double)ticks / TicksPerMinute;
	}
	double TimeSpan::GetTotalHours() const {
		return (double)ticks / TicksPerHour;
	}
	double TimeSpan::GetTotalDays() const {
		return (double)ticks / TicksPerDay;
	}

	TimeSpan TimeSpan::operator+(const TimeSpan& obj) const {
		int64 result = ticks + obj.ticks;
		// Check overflow: the sign bit flipped when doing the same operation
		if ((ticks >> 63 == obj.ticks >> 63) && (ticks >> 63 != result >> 63)) {
			throw OverflowException("Ticks operation: int64 overflowed.");
		}
		return TimeSpan(result);
	}
	TimeSpan TimeSpan::operator-(const TimeSpan& obj) const {
		int64 result = ticks - obj.ticks;
		// Check overflow: the sign bit flipped when doing the same operation
		if ((ticks >> 63 == obj.ticks >> 63) && (ticks >> 63 != result >> 63)) {
			throw OverflowException("Ticks operation: int64 overflowed.");
		}
		return TimeSpan(result);
	}
}