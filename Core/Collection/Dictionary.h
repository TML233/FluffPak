#ifndef HEADER_CORE_COLLECTION_DICTIONARY
#define HEADER_CORE_COLLECTION_DICTIONARY

#include "Core/System/Object.h"
#include "Core/System/String.h"

namespace Core {
	template<typename TKey, typename TValue>
	struct KeyValuePair final {
		TKey key;
		TValue value;
	};

	// Referenced .NET 5 standard library: https://source.dot.net
	template<typename TKey, typename TValue>
	class Dictionary final:public Object {
	public:
		struct Entry {
			int hashCode;
			int next;
			TKey key;
			TValue value;
		};
		void Add(const TKey& key, const TValue& value);
		bool Remove(const TKey& key);

	private:
		int* buckets = nullptr;
		Entry* entries = nullptr;
		int count = 0;
		int freeIndex = -1;
		int freeCount = -1;
	};
}


#endif