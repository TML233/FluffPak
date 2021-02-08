#pragma once

#include "Engine/System/Exception.h"

namespace Engine {
	// Default deleter which calls delete
	template<typename T>
	class DefaultDeleter {
	public:
		void operator()(T* ptr);
	};
	template<typename T>
	void DefaultDeleter<T>::operator()(T* ptr) {
		delete ptr;
	}

	// Default deleter for arrays which calls delete[]
	template<typename T>
	class DefaultDeleter<T[]> {
	public:
		void operator()(T* ptr);
	};
	template<typename T>
	void DefaultDeleter<T[]>::operator()(T* ptr) {
		delete[] ptr;
	}

	// UniquePtr
	template<typename T,typename TDeleter=DefaultDeleter<T>>
	class UniquePtr final {
	public:
		explicit UniquePtr(T* ptr = nullptr);
		~UniquePtr();
		UniquePtr(const UniquePtr<T>& obj) = delete;
		UniquePtr& operator=(const UniquePtr<T> obj) = delete;
		T* GetRaw() const;
		T* Release();
		void Reset(T* ptr = nullptr);
		T& operator*() const;
		T* operator->() const;
		operator bool();
	private:
		T* ptr = nullptr;
		TDeleter deleter;
	};

	template<typename T, typename TDeleter>
	UniquePtr<T,TDeleter>::UniquePtr(T* ptr) :ptr(ptr) {}
	template<typename T, typename TDeleter>
	T* UniquePtr<T, TDeleter>::GetRaw() const {
		return ptr;
	}
	template<typename T, typename TDeleter>
	UniquePtr<T, TDeleter>::~UniquePtr() {
		Reset();
	}
	template<typename T, typename TDeleter>
	T* UniquePtr<T, TDeleter>::Release() {
		T* ptr = this->ptr;
		this->ptr = nullptr;
		return ptr;
	}
	template<typename T, typename TDeleter>
	void UniquePtr<T, TDeleter>::Reset(T* ptr) {
		if (this->ptr != nullptr) {
			deleter(this->ptr);
		}
		this->ptr = ptr;
	}
	template<typename T, typename TDeleter>
	T& UniquePtr<T, TDeleter>::operator*() const {
		return *ptr;
	}
	template<typename T, typename TDeleter>
	T* UniquePtr<T, TDeleter>::operator->() const {
		return ptr;
	}
	template<typename T, typename TDeleter>
	UniquePtr<T, TDeleter>::operator bool() {
		return ptr != nullptr;
	}

	// UniquePtr for arrays
	template<typename T, typename TDeleter>
	class UniquePtr<T[],TDeleter> {
	public:
		explicit UniquePtr(T* ptr = nullptr);
		~UniquePtr();
		UniquePtr(const UniquePtr<T[]>& obj) = delete;
		UniquePtr& operator=(const UniquePtr<T[]> obj) = delete;
		T* GetRaw() const;
		T* Release();
		void Reset(T* ptr = nullptr);
		operator bool();
	private:
		T* ptr = nullptr;
		TDeleter deleter;
	};
	template<typename T, typename TDeleter>
	UniquePtr<T[], TDeleter>::UniquePtr(T* ptr) :ptr(ptr) {}
	template<typename T, typename TDeleter>
	T* UniquePtr<T[], TDeleter>::GetRaw() const {
		return ptr;
	}
	template<typename T, typename TDeleter>
	UniquePtr<T[], TDeleter>::~UniquePtr() {
		deleter(ptr);
	}
	template<typename T, typename TDeleter>
	T* UniquePtr<T[], TDeleter>::Release() {
		T* ptr = this->ptr;
		this->ptr = nullptr;
		return ptr;
	}
	template<typename T, typename TDeleter>
	void UniquePtr<T[], TDeleter>::Reset(T* ptr) {
		if (this->ptr != nullptr) {
			deleter(this->ptr);
		}
		this->ptr = ptr;
	}
	template<typename T, typename TDeleter>
	UniquePtr<T[], TDeleter>::operator bool() {
		return ptr != nullptr;
	}
}