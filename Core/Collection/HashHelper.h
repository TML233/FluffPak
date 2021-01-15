#ifndef HEADER_CORE_COLLECTION_HASHHELPER
#define HEADER_CORE_COLLECTION_HASHHELPER

#include "Core/System/Object.h"
#include "Core/System/String.h"

namespace Core {
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