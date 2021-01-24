#ifndef HEADER_CORE_SYSTEM_IENUMERABLE
#define HEADER_CORE_SYSTEM_IENUMERABLE

#include "Engine/Collection/Iterator.h"

namespace Engine {
	template<typename T>
	class IEnumerable {
		virtual Iterator<T> begin() const = 0;
		virtual Iterator<T> end() const = 0;
	};
}

#endif