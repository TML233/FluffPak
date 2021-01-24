#ifndef HEADER_CORE_COLLECTION_HASHHELPER
#define HEADER_CORE_COLLECTION_HASHHELPER

#include "Engine/System/Object.h"
#include "Engine/System/String.h"

namespace Engine {
	// Referenced .NET 5 standard library: https://source.dot.net
	class HashHelper final {
	public:
		static Bool IsPrime(Int value);
		static Int GetPrime(Int min);
	private:
		static const Int hashPrime;
		static const Int primes[];
	};
}


#endif