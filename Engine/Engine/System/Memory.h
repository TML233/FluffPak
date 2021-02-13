#pragma once

#include "Engine/System/Definition.h"
#include <memory>
#include <new>
#include <type_traits>

// Partly referenced Godot Engine 3.2.3 source code.
// https://www.github.com/godotengine/godot

namespace Engine {
	class Memory final {
		STATIC_CLASS(Memory);
	public:
		// Allocate a memory of the specific size.
		static void* Allocate(sizeint size);
		// Resize a memory block.
		static void* Reallocate(void* ptr, sizeint newSize);
		// Free a memory block.
		static void Deallocate(void* ptr);

		// Constructs a object on an existing memory.
		template<typename T,typename ... Args>
		static void Construct(T* ptr, const Args& ... args) {
			// Placement new
			new (ptr) T(args...);
		}
		// Deconstructs a object but doesn't deallocate the memory.
		template<typename T>
		static void Destruct(T* ptr) {
			// Only do necessary destructor calls
			if (!__has_trivial_destructor(T)) {
				ptr->~T();
			}
		}

		// Internal implementation.
		// Use MEMDEL(ptr) instead.
		template<typename T>
		static void _Delete(T* ptr) {
			if (ptr == nullptr) {
				return;
			}
			// Call destructor if necessary.
			Destruct(ptr);
			// Free memory.
			Deallocate(ptr);
		}
	};
}

#define MEMNEW(x) new (true) x
#define MEMDEL(ptr) ::Engine::Memory::_Delete(ptr)
//#define MEMNEWARR(type,count) ::Engine::Memory::_NewArray<type>(count)
//#define MEMDELARR(ptr) ::Engine::Memory::_DeleteArray(ptr)

void* operator new(size_t size, bool reserved);