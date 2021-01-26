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
		Random(UInt seed);
		// Generates a integer which is [min, max).
		Int Next(Int min, Int max);
		// Generates a float which is [min, max).
		Float NextFloat(Float min=0, Float max=1);
		// Generates a double which is [min, max).
		Double NextDouble(Double min=0, Double max=1);
		// Set the seed of the random number generator.
		void SetSeed(UInt seed);
	private:
		std::default_random_engine engine;
	};
}

#endif