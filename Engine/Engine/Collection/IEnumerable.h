#pragma once

#include "Engine/Collection/Iterator.h"

namespace Engine {
	template<typename T>
	class IEnumerable {
		virtual ReadonlyIterator<T> begin() const = 0;
		virtual ReadonlyIterator<T> end() const = 0;
	};
}