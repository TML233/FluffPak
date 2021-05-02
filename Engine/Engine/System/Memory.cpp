#include "Engine/System/Memory.h"
#include <memory>
#include "Engine/System/Debug.h"

namespace Engine {
	void* Memory::Allocate(sizeint size) {
		ERR_ASSERT(size > 0, u8"size must be larger than 0.", return nullptr);

		void* ptr = std::malloc(size);
		return ptr;
	}
	void* Memory::Reallocate(void* ptr, sizeint newSize) {
		ERR_ASSERT(ptr != nullptr, u8"ptr must not be nullptr!", return nullptr);
		ERR_ASSERT(newSize > 0, u8"newSize must be larger than 0.", return nullptr);

		void* result = std::realloc(ptr, newSize);
		return result;
	}
	void Memory::Deallocate(void* ptr) {
		std::free(ptr);
	}
	sizeint Memory::GetHeapArrayElementCount(void* ptr) {
		return *(((sizeint*)ptr) - 1);
	}
}

void* operator new(size_t size, bool reserved) {
	return Engine::Memory::Allocate(size);
}
void operator delete(void* ptr, bool reserved){
	Engine::Memory::Deallocate(ptr);
}