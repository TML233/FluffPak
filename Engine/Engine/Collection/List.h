#ifndef HEADER_CORE_COLLECTION_LIST
#define HEADER_CORE_COLLECTION_LIST

#include <initializer_list>
#include "Engine/System/Definition.h"
#include "Engine/System/Exception.h"
#include "Engine/System/Object.h"
#include "Engine/System/IEnumerable.h"
#include "Engine/Collection/Iterator.h"

namespace Engine {
	// A list that expands the internal array when exceeding capacity.
	// Slow inserting in the middle. Fast random accessing.
	template<typename T>
	class List final :public Object, public IEnumerable<T> {
	public:
		List(Int capacity = 0);
		List(const std::initializer_list<T>& values);
		List(const List<T>& list);
		List<T>& operator=(const List<T>& list);
		~List();

		// Get item count.
		Int GetCount() const;

		// Get capacity, the number of item the list can hold before expanding.
		Int GetCapacity() const;

		// Set capacity.
		// Throws ArgumentOutOfRangeException when the capacity is smaller than item count;
		void SetCapacity(Int capacity);

		// Set capacity to item count to save up some memory.
		void TrimExcess();

		// Get item by index.
		// Throws ArgumentOutOfRangeException when the index is out of bounds.
		Iterator<T> operator[](int index) const;

		// Add an item to the end of the list.
		void Add(const T& item);

		// Insert an item to the specified position.
		// Warning: might be slow if adding item to the position but the last one!
		void Insert(Int index, const T& item);

		// Remove the item at the specified position.
		// Warning: might be slow if removing the item but the last one!
		Bool RemoveAt(Int index);

		// Remove all the item in the list.
		void Clear();

		// Get the original C-style array.
		// This array is NOT persistent!
		T* GetRawArray() const;

		// Prepare the capacity.
		// The capacity will be doubled each time the capacity needs to be increased.
		void PrepareCapacity(Int target);

		Iterator<T> begin() const override;
		Iterator<T> end() const override;

	private:
		static const Int DefaultCapacity;
		void CopyFrom(const List<T>& list);
		void MoveItems(Int from, Int to, Int dest);

		T* items = nullptr;
		Int capacity = 0;
		Int count = 0;
	};

	template<typename T>
	List<T>::List(const std::initializer_list<T>& values) {
		SetCapacity(values.size());
		for (T& value : values) {
			Add(value);
		}
	}

	template<typename T>
	const Int List<T>::DefaultCapacity = 4;

	template<typename T>
	Iterator<T> List<T>::begin() const {
		return Iterator<T>(count > 0 ? &items[0] : nullptr);
	}
	
	template<typename T>
	Iterator<T> List<T>::end() const {
		return Iterator<T>(count > 0 ? &items[count] : nullptr);
	}

	template<typename T>
	T* List<T>::GetRawArray() const {
		return items;
	}

	template<typename T>
	List<T>::List(Int capacity) {
		SetCapacity(capacity);
	}
	template<typename T>
	List<T>::List(const List<T>& list) {
		CopyFrom(list);
	}
	template<typename T>
	List<T>& List<T>::operator=(const List<T>& list) {
		CopyFrom(list);
		return *this;
	}
	template<typename T>
	List<T>::~List() {
		delete[] items;
	}

	template<typename T>
	Int List<T>::GetCount() const {
		return count;
	}

	template<typename T>
	Int List<T>::GetCapacity() const {
		return capacity;
	}
	template<typename T>
	void List<T>::SetCapacity(Int capacity) {
		if (capacity < count) {
			throw ArgumentOutOfRangeException(u8"capacity", u8"Specified capacity is lower than item count.");
		}
		if (capacity == 0) {
			delete[] items;
			items = nullptr;
			capacity = 0;
			return;
		}

		T* newArr = new T[capacity];
		if (items != nullptr && count > 0) {
			for (Int i = 0; i < count; i += 1) {
				newArr[i] = items[i];
			}
			delete[] items;
		}
		items = newArr;
		this->capacity = capacity;
	}
	template<typename T>
	void List<T>::TrimExcess() {
		SetCapacity(count);
	}

	template<typename T>
	Iterator<T> List<T>::operator[](int index) const {
		if (index < 0 || index >= count) {
			throw ArgumentOutOfRangeException(u8"index", u8"Index out of bounds.");
		}
		return Iterator<T>(&(items[index]));
	}

	template<typename T>
	void List<T>::Add(const T& item) {
		Insert(count, item);
	}

	template<typename T>
	void List<T>::Insert(Int index, const T& item) {
		if (index < 0 || index > count) {
			throw ArgumentOutOfRangeException(u8"index", u8"Specified index is out of bounds.");
		}
		PrepareCapacity(count + 1);
		if (index != count) {
			MoveItems(index, count - 1, index + 1);
		}
		items[index] = item;
		count += 1;
	}

	template<typename T>
	Bool List<T>::RemoveAt(Int index) {
		if (index < 0 || index >= count) {
			return false;
		}
		MoveItems(index + 1, count - 1, index);
		count -= 1;
		return true;
	}

	template<typename T>
	void List<T>::Clear() {
		count = 0;
	}

	// Private
	template<typename T>
	void List<T>::CopyFrom(const List<T>& list) {
		if (capacity < list.capacity) {
			SetCapacity(list.capacity);
		}
		count = list.count;
		for (Int i = 0; i < count; i += 1) {
			items[i] = list.items[i];
		}
	}

	template<typename T>
	void List<T>::PrepareCapacity(Int target) {
		if (target <= capacity) {
			return;
		}

		// Capacity exceeded, expanding.
		if (capacity == 0) {
			SetCapacity(DefaultCapacity);
			return;
		}

		Int ideal = capacity;
		while (target > ideal) {
			ideal *= 2;
		}
		SetCapacity(ideal);
	}

	template<typename T>
	void List<T>::MoveItems(Int from, Int to, Int dest) {
		if (from < 0 || to < 0 || from >= capacity || to >= capacity || dest < 0 || dest >= capacity) {
			return;
		}
		if (from == dest) {
			return;
		}
		if (dest < from) {
			for (int i = 0; i < to - from + 1; i += 1) {
				items[dest + i] = std::move(items[from + i]);
			}
		} else {
			for (int i = 0; i < to - from + 1; i += 1) {
				items[dest + (to - from) - i] = std::move(items[to - i]);
			}
		}
	}
}

#endif