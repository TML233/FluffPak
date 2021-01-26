#include "Engine/System/TimeSpan.h"
#include "Engine/System/Exception.h"
#include "Engine/System/String.h"

namespace Engine {
	TimeSpan::TimeSpan(Long ticks) :ticks(ticks) {}

	const Long TimeSpan::TicksPerMillisecond = 10000;
	const Long TimeSpan::TicksPerSecond = TicksPerMillisecond * 1000;
	const Long TimeSpan::TicksPerMinute = TicksPerSecond * 60;
	const Long TimeSpan::TicksPerHour = TicksPerMinute * 60;
	const Long TimeSpan::TicksPerDay = TicksPerHour * 24;

	Long TimeSpan::GetTicks() const {
		return ticks;
	}
	void TimeSpan::SetTicks(Long ticks) {
		this->ticks = ticks;
	}

	Int TimeSpan::GetMilliseconds() const {
		return (Int)((ticks / TicksPerMillisecond) % 1000);
	}
	Int TimeSpan::GetSeconds() const {
		return (Int)((ticks / TicksPerSecond) % 60);
	}
	Int TimeSpan::GetMinutes() const {
		return (Int)((ticks / TicksPerMinute) % 60);
	}
	Int TimeSpan::GetHours() const {
		return (Int)((ticks / TicksPerHour) % 24);
	}
	Int TimeSpan::GetDays() const {
		return (Int)(ticks / TicksPerDay);
	}

	Double TimeSpan::GetTotalMilliseconds() const {
		return (Double)ticks / TicksPerMillisecond;
	}
	Double TimeSpan::GetTotalSeconds() const {
		return (Double)ticks / TicksPerSecond;
	}
	Double TimeSpan::GetTotalMinutes() const {
		return (Double)ticks / TicksPerMinute;
	}
	Double TimeSpan::GetTotalHours() const {
		return (Double)ticks / TicksPerHour;
	}
	Double TimeSpan::GetTotalDays() const {
		return (Double)ticks / TicksPerDay;
	}

	TimeSpan TimeSpan::operator+(const TimeSpan& obj) const {
		Long result = ticks + obj.ticks;
		// Check overflow: the sign bit flipped when doing the same operation
		if ((ticks >> 63 == obj.ticks >> 63) && (ticks >> 63 != result >> 63)) {
			throw OverflowException("Ticks operation: Int64 overflowed.");
		}
		return TimeSpan(result);
	}
	TimeSpan TimeSpan::operator-(const TimeSpan& obj) const {
		Long result = ticks - obj.ticks;
		// Check overflow: the sign bit flipped when doing the same operation
		if ((ticks >> 63 == obj.ticks >> 63) && (ticks >> 63 != result >> 63)) {
			throw OverflowException("Ticks operation: Int64 overflowed.");
		}
		return TimeSpan(result);
	}
}