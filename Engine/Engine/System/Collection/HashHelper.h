#pragma once

#include "Engine/System/Definition.h"

namespace Engine {
	// Referenced .NET 5 standard library: https://source.dot.net
	class HashHelper final {
	public:
		STATIC_CLASS(HashHelper);

		static bool IsPrime(int32 value);
		static int32 GetPrime(int32 min);
	private:
		static const int32 hashPrime;
		static const int32 primes[];
	};
}