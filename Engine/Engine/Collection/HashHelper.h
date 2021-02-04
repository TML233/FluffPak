#ifndef HEADER_CORE_COLLECTION_HASHHELPER
#define HEADER_CORE_COLLECTION_HASHHELPER

#include "Engine/System/Object.h"
#include "Engine/System/String.h"

namespace Engine {
	// Referenced .NET 5 standard library: https://source.dot.net
	class HashHelper final {
	public:
		static Bool IsPrime(Int32 value);
		static Int32 GetPrime(Int32 min);
	private:
		static const Int32 hashPrime;
		static const Int32 primes[];
	};
}


#endif