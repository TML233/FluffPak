#pragma once

#include "Engine/System/Definition.h"

namespace Engine {
	template<typename T>
	class ReadonlyIterator {
	public:
		ReadonlyIterator(const T* ptr) :target(ptr) {}

		// Getter
		operator const T() const {
			return *target;
		}

		// Iterator getter/dereference
		const T& operator*() const {
			return *target;
		}
		// Iterator !=
		bool operator!=(const ReadonlyIterator<T>& obj) const {
			return target != obj.target;
		}
		// Iterator next
		ReadonlyIterator<T>& operator++() {
			target += 1;
			return *this;
		}

	private:
		const T* target;
	};
}