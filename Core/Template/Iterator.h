#ifndef HEADER_CORE_TEMPLATES_ITERATOR
#define HEADER_CORE_TEMPLATES_ITERATOR

#include "Core/Definition.h"

namespace Core {
	template<typename T>
	class ReadonlyIterator {
	public:
		ReadonlyIterator(T* ptr);

		// Getter
		operator T() const;

		// Iterator getter/dereference
		T& operator*() const;
		// Iterator !=
		Bool operator!=(const ReadonlyIterator<T>& obj) const;
		// Iterator next
		ReadonlyIterator<T>& operator++();

	private:
		T* target;
	};

	template<typename T>
	ReadonlyIterator<T>::ReadonlyIterator(T* ptr) :target(ptr) {}

	template<typename T>
	ReadonlyIterator<T>::operator T() const {
		return *target;
	}

	template<typename T>
	T& ReadonlyIterator<T>::operator*() const {
		return *target;
	}

	template<typename T>
	Bool ReadonlyIterator<T>::operator!=(const ReadonlyIterator<T>& obj) const {
		return target != obj.target;
	}

	template<typename T>
	ReadonlyIterator<T>& ReadonlyIterator<T>::operator++() {
		target += 1;
		return *this;
	}

	template<typename T>
	class Iterator:public ReadonlyIterator<T> {
	public:
		Iterator(T* ptr);

		Iterator<T>& operator=(const T& value);
	};

	template<typename T>
	Iterator<T>::Iterator(T* ptr) :ReadonlyIterator(ptr) {}

	template<typename T>
	Iterator<T>& Iterator<T>::operator=(const T& value) {
		*target = value;
		return *this;
	}
}

#endif