#ifndef HEADER_CORE_COLLECTION_DICTIONARY
#define HEADER_CORE_COLLECTION_DICTIONARY

#include "Core/System/Object.h"
#include "Core/System/String.h"
#include "Core/Collection/HashHelper.h"

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
		Bool ContainsKey(const TKey& key) const;
		TValue operator[](const TKey& key) const;

	private:
		void SetCapacity(int min);
		int* buckets = nullptr;
		Entry* entries = nullptr;
		int count = 0;
		int capacity = 0;
		int freeIndex = -1;
		int freeCount = -1;
	};

	template<typename TKey, typename TValue>
	void Dictionary<TKey, TValue>::SetCapacity(int min) {
		capacity = HashHelper::GetPrime(min);
		int* newBuckets = new int[capacity];
		Entry* newEntries = new Entry[capacity];
		
		if (buckets != nullptr) {
			// TODO: Copy old buckets to the new one.
			delete[] buckets;
		}
		buckets = newBuckets;
		
		if (entries != nullptr) {
			// TODO: Copy old entries to the new one.
			delete[] entries;
		}
		entries = newEntries;
	}
}


#endif