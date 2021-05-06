#pragma once

#include "Engine/System/Definition.h"

namespace Engine {
	template<typename T>
	class ReadonlyIterator {
	public:
		ReadonlyIterator(const T* ptr) :target(ptr) {}

		/// @brief Getter
		operator const T() const {
			return *target;
		}

		/// @brief Iterator getter/dereference
		const T& operator*() const {
			return *target;
		}
		/// @brief Iterator !=
		bool operator!=(const ReadonlyIterator<T>& obj) const {
			return target != obj.target;
		}
		/// @brief Iterator next
		ReadonlyIterator<T>& operator++() {
			target += 1;
			return *this;
		}

		const T* GetPointer() const {
			return target;
		}

	private:
		const T* target;
	};
}