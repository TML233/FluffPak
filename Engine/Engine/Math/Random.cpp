#include "Engine/Math/Random.h"
#include "Engine/System/Exception.h"
#include "Engine/System/String.h"
#include <chrono>

namespace Engine {
	Random::Random() {
		// TODO: Change this to DateTime calls.
		Int time = (Int)std::chrono::high_resolution_clock::now().time_since_epoch().count();
		UInt seed = 0;
		std::memcpy(&seed, &time, sizeof(Int));
		SetSeed(seed);
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