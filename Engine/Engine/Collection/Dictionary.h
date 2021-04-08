#pragma once
#include "Engine/System/ObjectUtil.h"
#include "Engine/Collection/HashHelper.h"
#include "Engine/System/Memory.h"
#include "Engine/System/Debug.h"

namespace Engine {
	// A hashmap.
	// TKey needs to implement `int32 GetHashCode() const` and `bool operator==(const T&) const`
	// TValue needs to be: default-constructable, copy-constructable, move-contstructable.
	template<typename TKey,typename TValue>
	class Dictionary {
	public:
		using KeyType = TKey;
		using ValueType = TValue;

		Dictionary(int32 capacity=0) {
			SetCapacity(capacity);
		}
		~Dictionary() {
			Clear();

			MEMDELARR(buckets);
			Memory::Deallocate(entries);
		}

		bool SetCapacity(int32 capacity) {
			ERR_ASSERT(capacity >= count, "capacity cannot be smaller than element count.", return false);
			if (capacity == count) {
				return true;
			}

			// Stop immediately if buckets and entries are not in sync (this should never happen, but just in case)
			ERR_ASSERT((buckets == nullptr && entries == nullptr) || (buckets != nullptr && entries != nullptr),
				"buckets and entries are out of sync!",
				FATAL_CRASH("Cannot proceed as Dictionary is in a dangerous state and may have caused memory leak!")
			);

			// Get a closest prime to help mod the hash code.
			int32 desired = HashHelper::GetPrime(capacity);
			ERR_ASSERT(desired >= capacity, "Failed to find a prime number for capacity!", return false);

			if (buckets == nullptr && entries == nullptr) {
				this->buckets = MEMNEWARR(int32, desired);
				std::memset(this->buckets, -1, desired * sizeof(int32));
				this->entries = (Entry*)Memory::Allocate(desired * sizeof(Entry));
				this->capacity = desired;
			} else {
				int32 oldCapacity = this->capacity;
				int32* oldBuckets = this->buckets;
				Entry* oldEntries = this->entries;

				this->capacity = desired;
				this->count = 0;
				this->buckets = MEMNEWARR(int32, desired);
				std::memset(this->buckets, -1, desired * sizeof(int32));
				this->entries = (Entry*)Memory::Allocate(desired * sizeof(Entry));

				// Re-index the elements in the old container into the new one and destroy the old element.
				for (int32 i = 0; i < oldCapacity; i += 1) {
					if (oldBuckets[i] < 0) {
						continue;
					}
					for (int32 j = oldBuckets[i]; j >= 0; j = oldEntries[j].next) {
						Insert(oldEntries[j].key, oldEntries[j].value, oldEntries[j].hashCode, InsertMode::Set);
						Memory::Destruct(oldEntries + j);
					}
				}

				MEMDELARR(oldBuckets);
				Memory::Deallocate(oldEntries);
			}
			return true;
		}
		int32 GetCapacity() const {
			return capacity;
		}
		int32 GetCount() const {
			return count;
		}

		bool Add(const TKey& key, const TValue& value) {
			bool result = Insert(key, value, GetKeyHash(key),InsertMode::Add);
			ERR_ASSERT(result, "Failed to add an entry, the key already exists.", return false);
			return true;
		}
		void Set(const TKey& key, const TValue& value) {
			Insert(key, value, GetKeyHash(key), InsertMode::Set);
		}

		bool ContainsKey(const TKey& key) const {
			if (buckets == nullptr && entries == nullptr) {
				return false;
			}

			uint32 hash = GetKeyHash(key);
			int bucket = GetBucketIndex(hash);
			if (buckets[bucket] == -1) {
				return false;
			}
			for (int32 i = buckets[bucket]; i >= 0; i = entries[i].next) {
				if (entries[i].hashCode == hash && entries[i].key == key) {
					return true;
				}
			}
			return false;
		}
		void Clear() {
			if (buckets == nullptr && entries == nullptr) {
				return;
			}

			// Do destruction
			for (int32 i = 0; i < capacity; i += 1) {
				if (buckets[i] < 0) {
					continue;
				}
				for (int32 j = buckets[i]; j >= 0; j = entries[j].next) {
					Memory::Destruct(entries + j);
				}
				buckets[i] = -1;
			}

			count = 0;
			freeIndex = -1;
		}
		bool TryGet(const TKey& key, TValue& result) const {
			if (buckets == nullptr && entries == nullptr) {
				return false;
			}

			uint32 hash = GetKeyHash(key);
			int bucket = GetBucketIndex(hash);
			if (buckets[bucket] == -1) {
				return false;
			}
			for (int32 i = buckets[bucket]; i >= 0; i = entries[i].next) {
				if (entries[i].hashCode == hash && entries[i].key == key) {
					result = entries[i].value;
					return true;
				}
			}
			return false;
		}
		TValue Get(const TKey& key) const {
			TValue result{};
			bool succeed = TryGet(key, result);
			FATAL_ASSERT(succeed, "Entry with key does not exists! Cannot return a value.");
			return result;
		}
		bool Remove(const TKey& key) {
			if (buckets == nullptr && entries == nullptr) {
				return false;
			}

			uint32 hash = GetKeyHash(key);
			int bucket = GetBucketIndex(hash);
			if (buckets[bucket] == -1) {
				return false;
			}
			for (int32 i = buckets[bucket],prev=-1; i >= 0; prev=i,i = entries[i].next) {
				if (entries[i].hashCode == hash && entries[i].key == key) {
					if (prev != -1) {
						// Connect prev and next if it has a prev linked entry.
						entries[prev].next = entries[i].next;
					} else {
						// Set bucket entry target to -1 if it is the first entry of the bucket.
						buckets[bucket] = -1;
					}
					// Destruct this entry
					Memory::Destruct(entries + i);
					
					// Add i into free list
					entries[i].next = freeIndex;
					freeIndex = i;

					count -= 1;

					return true;
				}
			}
			return false;
		}

		static const inline int32 CapacityMultiplier = 2;

	private:
		struct Entry {
			Entry(const TKey& key, const TValue& value,uint32 hashCode,int32 next) :key(key), value(value),hashCode(hashCode),next(next) {}
			TKey key;
			TValue value;
			uint32 hashCode;
			int32 next;
		};
		enum class InsertMode { Add, Set };
		static uint32 GetKeyHash(const TKey& key) {
			int32 s_hash = ObjectUtil::GetHashCode(key);
			return *((uint32*)(&s_hash));
		}
		
		bool Insert(const TKey& key, const TValue& value, uint32 hash, InsertMode mode) {
			RequireCapacity(count + 1);
			int32 bucket = GetBucketIndex(hash);

			// If bucket is empty.
			if (buckets[bucket] == -1) {
				buckets[bucket] = AddEntry(key, value, hash, -1);
				return true;
			}

			for (int32 i = buckets[bucket]; i >= 0; i = entries[i].next) {
				// the key already exists.
				if (entries[i].hashCode == hash && entries[i].key == key) {
					if (mode == InsertMode::Add) {
						// In add mode, fails.
						ERR_MSG("Key is already exists.");
						return false;
					} else {
						// In set mode, overwrite the value.
						entries[i].value = value;
						return true;
					}
				}
			}
			// the key doesn't exist, add entry.
			buckets[bucket] = AddEntry(key, value, hash, buckets[bucket]);

			return true;
		}

		int32 AddEntry(const TKey& key, const TValue& value, uint32 hash, int32 next) {
			int32 index = -1;
			if (freeIndex == -1) {
				index = count;
			} else {
				index = freeIndex;
				freeIndex = entries[freeIndex].next;
			}
			Memory::Construct(entries + index, key, value, hash, next);
			count += 1;
			return index;
		}

		void RequireCapacity(int32 capacity) {
			if (capacity <= this->capacity) {
				return;
			}
			int result = (this->capacity == 0 ? capacity : this->capacity);
			while (result < capacity) {
				result *= CapacityMultiplier;
			}
			SetCapacity(result);
		}
		int32 GetBucketIndex(uint32 hash) const {
			return (int32)(hash % capacity);
		}

		int32 capacity = 0;
		int32 count = 0;
		int32* buckets = nullptr;
		Entry* entries = nullptr;
		int freeIndex = -1;
	};
}
