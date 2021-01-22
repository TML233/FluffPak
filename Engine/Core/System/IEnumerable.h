#ifndef HEADER_CORE_SYSTEM_IENUMERABLE
#define HEADER_CORE_SYSTEM_IENUMERABLE

#include "Core/Collection/Iterator.h"

namespace Core {
	template<typename T>
	class IEnumerable {
		virtual Iterator<T> begin() const = 0;
		virtual Iterator<T> end() const = 0;
	};
}

#endif