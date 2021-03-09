#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Exception.h"
#include "Engine/System/Memory.h"
#include "Engine/System/String.h"

namespace Engine{
	template<typename T>
	class List:Object {
	public:
		using ValueType = T;
		~List() {
			if (elements == nullptr) {
				return;
			}
			for (int32 i = 0; i < size; i += 1) {
				Memory::Destruct(elements + i);
			}
			Memory::Deallocate(elements);
		}

		int32 GetCapacity() const {
			return capacity;
		}
		void SetCapacity(int32 capacity) {
			if (capacity < 0 || capacity < size) {
				throw ArgumentOutOfRangeException("capacity", "Cannot be less than 0 or the current size.");
			}
			if (capacity == size) {
				return;
			}

			if (elements == nullptr) {
				elements = Memory::Allocate(capacity);
			} else {
				elements = Memory::Reallocate(elements, capacity);
			}
			this->capacity = capacity;
		}
		int32 GetSize() const {
			return size;
		}
		T GetValue(int32 index) const {
			if (index < 0 || index >= size) {
				throw ArgumentOutOfRangeException("index", "Out of bounds.");
			}
			return elements[index];
		}
		void SetValue(int32 index, const T& value) {
			if (index < 0 || index >= size) {
				throw ArgumentOutOfRangeException("index", "Out of bounds.");
			}
			*(elements + index) = value;
		}
		void AddValue(const T& value) {
			RequireCapacity(size + 1);
			Memory::Construct(elements + size, value);
			size += 1;
		}
		void InsertValue(int32 index, const T& value) {
			if (index<0 || index>size) {
				throw ArgumentOutOfRangeException("index", "Out of bounds.");
			}
			if (index == size) {
				AddValue(value);
				return;
			}

			RequireCapacity(size + 1);
			
			// Move objects.
			for (int32 i = size; i > index; i -= 1) {
				*(elements + i) = *(elements + i - 1);
			}

			// Assign
			*(elements + index) = value;
			
			size += 1;
		}
		void RemoveValue(int32 index) {
			if (index < 0 || index >= size) {
				throw ArgumentOutOfRangeException("index", "Out of bounds.");
			}
			// Destruct.
			Memory::Destruct(elements + index);

			// Move object routine.
			if (index < size - 1) {
				// Move objects.
				for (int32 i = index; i < size; i += 1) {
					*(elements + i) = *(elements + i + 1);
				}

				// Destruct the last element.
				Memory.Destruct(elements + size - 1);
			}

			size -= 1;
		}

		static const inline int32 DefaultCapacity = 4;
		static const inline int32 CapacityMultiplier = 2;
		
	private:
		void RequireCapacity(int32 capacity) {
			int32 result = (capacity == 0 ? DefaultCapacity : capacity);
			while (result < capacity) {
				result *= CapacityMultiplier;
			}
			SetCapacity(result);
		}
		T* elements = nullptr;
		int32 capacity = 0;
		int32 size = 0;
	};
}