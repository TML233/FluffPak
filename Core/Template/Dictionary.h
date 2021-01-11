#ifndef HEADER_CORE_TEMPLATE_DICTIONARY
#define HEADER_CORE_TEMPLATE_DICTIONARY

#include "Core/System/Object.h"
#include "Core/System/String.h"

namespace Core {
	template<typename TKey, typename TValue>
	struct KeyValuePair final:public Object {
		TKey key;
		TValue value;
		const String ToString() const override;
	};
	template<typename TKey, typename TValue>
	const String KeyValuePair<TKey, TValue>::ToString() const {
		return String::Format("(Key: {0}, Value: {1})", Object::ToString(key), Objecte::ToString(value));
	}

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