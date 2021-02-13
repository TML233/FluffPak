#include "Engine/System/Memory.h"
#include <memory>
#include "Engine/System/Exception.h"

namespace Engine {
	void* Memory::Allocate(sizeint size) {
		if (size == 0) {
			throw ArgumentOutOfRangeException("size", "must be larger than 0.");
		}
		
		void* ptr = std::malloc(size);
		if (ptr == nullptr) {
			throw OutOfMemoryException(String::Format("malloc failed to allocate a memory of {0} bytes!", size));
		}
		return ptr;
	}
	void* Memory::Reallocate(void* ptr, sizeint newSize) {
		if (ptr == nullptr) {
			throw ArgumentException("ptr", "must not be nullptr!");
		}
		if (newSize == 0) {
			throw ArgumentOutOfRangeException("newSize", "must be larger than 0.");
		}

		void* result = realloc(ptr, newSize);
		if (result == NULL) {
			throw OutOfMemoryException(String::Format("realloc failed to allocate a new memory of {0} bytes!", newSize));
		}
		return result;
	}
	void Memory::Deallocate(void* ptr) {
		std::free(ptr);
	}
}

void* operator new(size_t size, bool reserved) {
	return Engine::Memory::Allocate(size);
}