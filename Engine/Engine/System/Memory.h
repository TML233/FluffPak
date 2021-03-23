#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Debug.h"
#include <memory>
#include <new>

// Partly referenced Godot Engine 3.2.3 source code.
// https://www.github.com/godotengine/godot

namespace Engine {
	class Memory final {
		STATIC_CLASS(Memory);
	public:
#pragma region Move & Forward support
	private:
		template<typename T>
		class ReferenceRemover {
		public:
			using Type = T;
		};

		template<typename T>
		class ReferenceRemover<T&> {
		public:
			using Type = T;
		};

		template<typename T>
		class ReferenceRemover<T&&> {
		public:
			using Type = T;
		};

	public:
		// Turn everything to Right Value Reference.
		// Convenient tool for moving objects.
		template<typename T>
		static typename ReferenceRemover<T>::Type Move(T&& obj) {
			return static_cast<typename ReferenceRemover<T>::Type&&>(obj);
		}

		// Keep the Right Value Reference still as a Right Value.
		template<typename T>
		static T&& Forward(typename ReferenceRemover<T>::Type& obj) {
			return static_cast<T&&>(obj);
		}
		// Keep the Right Value Reference still as a Right Value.
		template<typename T>
		static T&& Forward(typename ReferenceRemover<T>::Type&& obj) {
			return static_cast<T&&>(obj);
		}
#pragma endregion

		// Allocate a memory of the specific size.
		static void* Allocate(sizeint size);
		// Resize a memory block.
		static void* Reallocate(void* ptr, sizeint newSize);
		// Free a memory block.
		static void Deallocate(void* ptr);

		template<typename T>
		static constexpr bool IsDestructionNeeded() {
			return !__has_trivial_destructor(T);
		}

		// Constructs a object on an existing memory.
		template<typename T,typename ... Args>
		static void Construct(T* ptr, Args&& ... args) {
			// Placement new
			new (ptr) T(Forward<Args>(args)...);
		}
		// Deconstructs a object but doesn't deallocate the memory.
		template<typename T>
		static void Destruct(T* ptr) {
			if (ptr == nullptr) {
				return;
			}
			// Only do necessary destructor calls
			if (IsDestructionNeeded<T>()) {
				ptr->~T();
			}
		}

		// Use MEMDEL(ptr) instead.
		template<typename T>
		static void Delete(T* ptr) {
			if (ptr == nullptr) {
				return;
			}
			// Call destructor if necessary.
			Destruct(ptr);
			// Free memory.
			Deallocate(ptr);
		}

		template<typename T,typename ... Args>
		static T* NewArray(sizeint count, Args&& ... args) {
			ERR_ASSERT(count > 0, "count must be larger than 0.", return nullptr);

			// Reserve a few bytes of size_t for saving the count data.
			sizeint size = sizeof(T) * count + sizeof(sizeint);

			// Allocate memory and write count data.
			sizeint* rawPtr = (sizeint*)Allocate(size);
			rawPtr[0] = count;

			// Do constructions.
			T* ptr = (T*)(rawPtr + 1);
			for (sizeint i = 0; i < count; i += 1) {
				Construct(ptr + i, Forward<Args>(args)...);
			}

			return ptr;
		}

		template<typename T>
		static void DeleteArray(T* ptr) {
			if (ptr == nullptr) {
				return;
			}

			// Get count data.
			sizeint* rawPtr = ((sizeint*)ptr) - 1;
			sizeint count = rawPtr[0];

			// Do destructions if necessary.
			if (IsDestructionNeeded<T>()) {
				for (sizeint i = 0; i < count; i += 1) {
					Destruct(ptr + i);
				}
			}

			// Free memory.
			Deallocate(rawPtr);
		}

		static sizeint GetHeapArrayElementCount(void* ptr);
	};
}

#define MEMNEW(type) new (true) type
#define MEMDEL(ptr) ::Engine::Memory::Delete(ptr)
#define MEMNEWARR(type,count) ::Engine::Memory::NewArray<type>(count)
#define MEMDELARR(ptr) ::Engine::Memory::DeleteArray(ptr)

void* operator new(size_t size, bool reserved);
// Paired operator delete for freeing memory when exception is thrown in ctor.
void operator delete(void* ptr, bool reserved);

