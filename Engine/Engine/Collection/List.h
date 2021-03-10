#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Exception.h"
#include "Engine/System/Memory.h"
#include "Engine/System/String.h"
#include "Engine/Collection/IEnumerable.h"

namespace Engine{
	template<typename T>
	class List:Object,IEnumerable<T> {
	public:
		using ValueType = T;
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
			if (capacity < 0 || capacity < count) {
				throw ArgumentOutOfRangeException("capacity", "Cannot be less than 0 or the current size.");
			}
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
			if (index < 0 || index >= count) {
				throw ArgumentOutOfRangeException("index", "Out of bounds.");
			}
			return elements[index];
		}
		void Set(int32 index, const T& value) {
			if (index < 0 || index >= count) {
				throw ArgumentOutOfRangeException("index", "Out of bounds.");
			}
			*(elements + index) = value;
		}
		void Add(const T& value) {
			RequireCapacity(count + 1);
			Memory::Construct(elements + count, value);
			count += 1;
		}
		void Insert(int32 index, const T& value) {
			if (index<0 || index>count) {
				throw ArgumentOutOfRangeException("index", "Out of bounds.");
			}
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
			if (index < 0 || index >= count) {
				throw ArgumentOutOfRangeException("index", "Out of bounds.");
			}
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
		
		ReadonlyIterator<T> begin() const override {
			return ReadonlyIterator<T>(elements);
		}
		ReadonlyIterator<T> end() const override {
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