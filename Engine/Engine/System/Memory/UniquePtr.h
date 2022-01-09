#pragma once

#include "Engine/System/Memory/Memory.h"

namespace Engine {
	/// @brief Smart pointer for managing exclusive heap pointers.\n
	/// Cannot be copied, but can be moved.
	template<typename T>
	class UniquePtr final {
	public:
		template<typename ... Args>
		static UniquePtr Create(Args&& ... args) {
			return UniquePtr(MEMNEW(T(Memory::Forward<Args>(args)...)));
		}

		UniquePtr(std::nullptr_t) :ptr(nullptr) {}
		explicit UniquePtr(T* ptr = nullptr) :ptr(ptr) {}
		~UniquePtr() {
			Reset();
		}
		
		// No copying
		UniquePtr(const UniquePtr& obj) = delete;
		UniquePtr& operator=(const UniquePtr& obj) = delete;

		// Moving
		UniquePtr(UniquePtr&& obj) {
			ptr = obj.ptr;
			obj.ptr = nullptr;
		}
		UniquePtr& operator=(UniquePtr&& obj) {
			if (this == &obj) {
				return *this;
			}
			Reset();
			ptr = obj.ptr;
			obj.ptr = nullptr;
			return *this;
		}

		T* GetRaw() const {
			return ptr;
		}
		T* Release() {
			T* ptr = this->ptr;
			this->ptr = nullptr;
			return ptr;
		}
		void Reset(T* ptr = nullptr) {
			if (this->ptr != nullptr) {
				MEMDEL(this->ptr);
			}
			this->ptr = ptr;
		}
		T& operator*() const {
			return *ptr;
		}
		T* operator->() const {
			return ptr;
		}
		bool operator==(T* ptr) const {
			return this->ptr == ptr;
		}
		bool operator!=(T* ptr) const {
			return this->ptr != ptr;
		}
		operator bool() {
			return ptr != nullptr;
		}
	private:
		T* ptr = nullptr;
	};

	/// @brief Smart pointer for managing exclusive heap array pointers.\n
	/// Cannot be copied, but can be moved.
	template<typename T>
	class UniquePtr<T[]> {
	public:
		static UniquePtr Create(sizeint length) {
			return UniquePtr(MEMNEWARR(T, length));
		}

		UniquePtr(std::nullptr_t) :ptr(nullptr) {}
		explicit UniquePtr(T* ptr = nullptr) :ptr(ptr) {}
		~UniquePtr() {
			Reset();
		}

		// No copying
		UniquePtr(const UniquePtr& obj) = delete;
		UniquePtr& operator=(const UniquePtr& obj) = delete;

		// Moving
		UniquePtr(UniquePtr&& obj) {
			ptr = obj.ptr;
			obj.ptr = nullptr;
		}
		UniquePtr& operator=(UniquePtr&& obj) {
			if (this == &obj) {
				return *this;
			}
			Reset();
			ptr = obj.ptr;
			obj.ptr = nullptr;
			return *this;
		}

		T* GetRaw() const {
			return ptr;
		}
		T* Release() {
			T* ptr = this->ptr;
			this->ptr = nullptr;
			return ptr;
		}
		void Reset(T* ptr = nullptr) {
			if (this->ptr != nullptr) {
				MEMDELARR(this->ptr);
			}
			this->ptr = ptr;
		}
		bool operator==(T* ptr) const {
			return this->ptr == ptr;
		}
		bool operator!=(T* ptr) const {
			return this->ptr != ptr;
		}
		operator bool() {
			return ptr != nullptr;
		}
	private:
		T* ptr = nullptr;
	};
}