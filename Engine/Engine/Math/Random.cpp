#include "Engine/Math/Random.h"
#include "Engine/System/Exception.h"
#include "Engine/System/String.h"

namespace Engine {
	Random::Random() {
		// TODO: Uncomment after DateTime system is done.
		// SetSeed(DateTime::GetCurrent());
	}
	Random::Random(UInt seed) {
		SetSeed(seed);
	}
	Int Random::Next(Int min, Int max) {
		if (min > max) {
			throw ArgumentOutOfRangeException("min", "min cannot be greater than max.");
		}
		if (min == max) {
			return min;
		}
		return std::uniform_int_distribution<Int>(min, max - 1)(engine);
	}
	Float Random::NextFloat(Float min, Float max) {
		if (min > max) {
			throw ArgumentOutOfRangeException("min", "min cannot be greater than max.");
		}
		return std::uniform_real_distribution<Float>(min, max)(engine);
	}
	Double Random::NextDouble(Double min, Double max) {
		if (min > max) {
			throw ArgumentOutOfRangeException("min", "min cannot be greater than max.");
		}
		return std::uniform_real_distribution<Double>(min, max)(engine);
	}
	void Random::SetSeed(UInt seed) {
		engine.seed(seed);
	}
}