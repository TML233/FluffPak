#include "Engine/System/Math/Random.h"
#include "Engine/System/Debug.h"
#include "Engine/System/String.h"
#include <chrono>

namespace Engine {
	Random::Random() {
		// TODO: Change this to DateTime calls.
		int32 time = (int32)std::chrono::high_resolution_clock::now().time_since_epoch().count();
		uint32 seed = 0;
		std::memcpy(&seed, &time, sizeof(int32));
		SetSeed(seed);
	}
	Random::Random(uint32 seed) {
		SetSeed(seed);
	}
	int32 Random::Next(int32 min, int32 max) {
		ERR_ASSERT(min <= max, u8"min cannot be greater than max.", return max);

		if (min == max) {
			return min;
		}
		return std::uniform_int_distribution<int32>(min, max - 1)(engine);
	}
	float Random::NextFloat(float min, float max) {
		ERR_ASSERT(min <= max, u8"min cannot be greater than max.", return max);

		return std::uniform_real_distribution<float>(min, max)(engine);
	}
	double Random::NextDouble(double min, double max) {
		ERR_ASSERT(min <= max, u8"min cannot be greater than max.", return max);

		return std::uniform_real_distribution<double>(min, max)(engine);
	}
	void Random::SetSeed(uint32 seed) {
		engine.seed(seed);
	}
}