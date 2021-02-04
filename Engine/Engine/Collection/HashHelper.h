#ifndef HEADER_CORE_COLLECTION_HASHHELPER
#define HEADER_CORE_COLLECTION_HASHHELPER

#include "Engine/System/Object.h"
#include "Engine/System/String.h"

namespace Engine {
	// Referenced .NET 5 standard library: https://source.dot.net
	class HashHelper final {
	public:
		static bool IsPrime(int32 value);
		static int32 GetPrime(int32 min);
	private:
		static const int32 hashPrime;
		static const int32 primes[];
	};
}


#endif