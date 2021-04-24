#pragma once

#include "Engine/System/Definition.h"
#include <random>

namespace Engine {
	/// @brief A pseudo random number generator.
	class Random final {
	public:
		
		/// @brief Initializes a random number generator with the current timestamp as seed.
		Random();
		/// @brief Initializes a random number generator with the given seed.
		Random(uint32 seed);
		/// @brief Generates a integer which is between [min, max).
		int32 Next(int32 min, int32 max);
		/// @brief Generates a float which is [min, max).
		float NextFloat(float min=0, float max=1);
		/// @brief Generates a double which is [min, max).
		double NextDouble(double min=0, double max=1);
		/// @brief Set the seed of the random number generator.
		void SetSeed(uint32 seed);
	private:
		std::default_random_engine engine;
	};
}