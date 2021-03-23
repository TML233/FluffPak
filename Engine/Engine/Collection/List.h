#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Memory.h"
#include "Engine/System/Debug.h"
#include "Engine/Collection/Iterator.h"
#include <initializer_list>

namespace Engine{
	template<typename T>
	class List {
	public:
		using ValueType = T;

		List(std::initializer_list<T> values) {
			if (values.size() <= 0) {
				return;
			}
			SetCapacity(static_cast<int32>(values.size()));
			for (auto value : values) {
				Add(value);
			}
		}

		~List() {
			if (elements == nullptr) {
				return;
			}
			for (int32 i = 0; i < count; i += 1) {
				Memory::Destruct(elements + i);
			}
			Memory::Deallocate(elements);
		}

		int32 GetCapacity() const {
			return capacity;
		}
		void SetCapacity(int32 capacity) {
			ERR_ASSERT(capacity >= 0 && capacity >= count, "capacity cannot be less than 0 or the current size.", return);

			if (capacity == count || capacity == this->capacity) {
				return;
			}

			if (elements == nullptr) {
				elements = (T*)Memory::Allocate(capacity * sizeof(T));
			} else {
				elements = (T*)Memory::Reallocate(elements, capacity * sizeof(T));
			}
			this->capacity = capacity;
		}
		int32 GetCount() const {
			return count;
		}
		T Get(int32 index) const {
			ERR_ASSERT(index >= 0 && index < count, "index out of bounds.", FATAL_CRASH("List<T>::Get cannot return a value."));
			return elements[index];
		}
		void Set(int32 index, const T& value) {
			ERR_ASSERT(index >= 0 && index < count, "index out of bounds.", return);
			*(elements + index) = value;
		}
		void Add(const T& value) {
			RequireCapacity(count + 1);
			Memory::Construct(elements + count, value);
			count += 1;
		}
		void Insert(int32 index, const T& value) {
			ERR_ASSERT(index >= 0 && index <= count, "index out of bounds.", return);
			
			if (index == count) {
				Add(value);
				return;
			}

			RequireCapacity(count + 1);
			
			// Move objects.
			for (int32 i = count; i > index; i -= 1) {
				*(elements + i) = *(elements + i - 1);
			}

			// Assign
			*(elements + index) = value;
			
			count += 1;
		}
		void RemoveAt(int32 index) {
			ERR_ASSERT(index >= 0 && index < count, "index out of bounds", return);

			// Destruct.
			Memory::Destruct(elements + index);

			// Move object routine.
			if (index < count - 1) {
				// Move objects.
				for (int32 i = index; i < count; i += 1) {
					*(elements + i) = *(elements + i + 1);
				}

				// Destruct the last element.
				Memory::Destruct(elements + count - 1);
			}

			count -= 1;
		}
		void Clear() {
			for (int32 i = 0; i < count; i += 1) {
				Memory::Destruct(elements + i);
			}
			count = 0;
		}
		static const inline int32 DefaultCapacity = 4;
		static const inline int32 CapacityMultiplier = 2;
		
		ReadonlyIterator<T> begin() const {
			return ReadonlyIterator<T>(elements);
		}
		ReadonlyIterator<T> end() const {
			return ReadonlyIterator<T>(elements + count);
		}
	private:
		void RequireCapacity(int32 capacity) {
			if (capacity <= this->capacity) {
				return;
			}
			int32 result = (this->capacity == 0 ? DefaultCapacity : this->capacity);
			while (result < capacity) {
				result *= CapacityMultiplier;
			}
			SetCapacity(result);
		}
		T* elements = nullptr;
		int32 capacity = 0;
		int32 count = 0;
	};
}