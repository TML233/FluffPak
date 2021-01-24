#ifndef HEADER_CORE_SYSTEM_ALLOCATOR
#define HEADER_CORE_SYSTEM_ALLOCATOR
#include "Core/System/Definition.h"
#include <memory>

namespace Core {
	template<typename T>
	class Allocator final {
	public:
		T* Allocate(int count);
		void Deallocate(T* ptr, int count);
		template<typename ... Args>
		void Construct(T* ptr, const Args& ... args);
		void Deconstruct(T* ptr);
	private:
		std::allocator<T> allocator;
	};
	
	template<typename T>
	T* Allocator<T>::Allocate(int count) {
		return allocator.allocate(count);
	}
	template<typename T>
	void Allocator<T>::Deallocate(T* ptr,int count) {
		allocator.deallocate(ptr, count);
	}
	template<typename T>
	template<typename ... Args>
	void Allocator<T>::Construct(T* ptr, const Args& ... args) {
		allocator.construct(ptr, args...);
	}
	template<typename T>
	void Allocator<T>::Deconstruct(T* ptr) {
		allocator.destroy(ptr);
	}
}

#endif