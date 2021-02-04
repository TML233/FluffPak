#ifndef HEADER_CORE_MATH_RANDOM
#define HEADER_CORE_MATH_RANDOM

#include "Engine/System/Definition.h"
#include <random>

namespace Engine {
	class Random final {
	public:
		// Initializes a random number generator with the current timestamp as seed.
		Random();
		// Initializes a random number generator with the given seed.
		Random(uint32 seed);
		// Generates a integer which is [min, max).
		int32 Next(int32 min, int32 max);
		// Generates a float which is [min, max).
		float NextFloat(float min=0, float max=1);
		// Generates a double which is [min, max).
		double NextDouble(double min=0, double max=1);
		// Set the seed of the random number generator.
		void SetSeed(uint32 seed);
	private:
		std::default_random_engine engine;
	};
}

#endif