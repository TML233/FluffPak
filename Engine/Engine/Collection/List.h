#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Memory/Memory.h"
#include "Engine/System/Debug.h"
#include "Engine/Collection/Iterator.h"
#include <initializer_list>

namespace Engine{
	/// @brief A random-access list.
	/// @tparam T The value type. Needs to be default-constructable, copy-constructable and move-contstructable.
	template<typename T>
	class List {
	public:
		using Iterator = ReadonlyIterator<T>;

		List(int32 capacity = 0) {
			SetCapacity(capacity);
		}

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
			Destroy();
		}

		List(const List& obj) {
			CopyFromOther(obj);
		}
		List& operator=(const List& obj) {
			if (this == &obj) {
				return *this;
			}

			Destroy();

			CopyFromOther(obj);

			return *this;
		}

		List(List&& obj) :elements(obj.elements), capacity(obj.capacity), count(obj.count) {
			obj.elements = nullptr;
			obj.capacity = 0;
			obj.count = 0;
		}
		List& operator=(List&& obj) {
			Destroy();

			elements = obj.elements;
			obj.elements = nullptr;
			capacity = obj.capacity;
			obj.capacity = 0;
			count = obj.count;
			obj.count = 0;

			return *this;
		}

		int32 GetCapacity() const {
			return capacity;
		}
		void SetCapacity(int32 capacity) {
			ERR_ASSERT(capacity >= 0 && capacity >= count, u8"capacity cannot be less than 0 or the current size.", return);

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
			ERR_ASSERT(index >= 0 && index < count, u8"index out of bounds.", return T());
			return elements[index];
		}
		void Set(int32 index, const T& value) {
			ERR_ASSERT(index >= 0 && index < count, u8"index out of bounds.", return);
			*(elements + index) = value;
		}
		void Add(const T& value) {
			RequireCapacity(count + 1);
			Memory::Construct(elements + count, value);
			count += 1;
		}
		void Insert(int32 index, const T& value) {
			ERR_ASSERT(index >= 0 && index <= count, u8"index out of bounds.", return);
			
			if (index == count) {
				Add(value);
				return;
			}

			RequireCapacity(count + 1);
			
			// Move objects.
			for (int32 i = count; i > index; i -= 1) {
				if (i == count) {
					Memory::Construct(elements + i, *(elements + i - 1));
				} else {
					*(elements + i) = *(elements + i - 1);
				}
			}

			// Assign
			*(elements + index) = value;
			
			count += 1;
		}
		void RemoveAt(int32 index) {
			ERR_ASSERT(index >= 0 && index < count, u8"index out of bounds", return);

			// Move object routine.
			if (index < count - 1) {
				for (int32 i = index; i < count - 1; i += 1) {
					*(elements + i) = *(elements + i + 1);
				}
			}

			// Destruct the last element.
			Memory::Destruct(elements + count - 1);

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
		
		Iterator begin() const {
			return Iterator(elements);
		}
		Iterator end() const {
			return Iterator(elements + count);
		}
	private:
		void CopyFromOther(const List& obj) {
			capacity = obj.capacity;
			count = obj.count;
			elements = (T*)Memory::Allocate(sizeof(T) * count);
			for (sizeint i = 0; i < count; i += 1) {
				Memory::Construct(elements + i, *(obj.elements + i));
			}
		}
		void Destroy() {
			if (elements == nullptr) {
				return;
			}
			for (int32 i = 0; i < count; i += 1) {
				Memory::Destruct(elements + i);
			}
			Memory::Deallocate(elements);
		}
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