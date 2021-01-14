#ifndef HEADER_CORE_TEMPLATE_UNIQUEPTR
#define HEADER_CORE_TEMPLATE_UNIQUEPTR

#include "Core/System/Exception.h"

namespace Core {
	// Default deleter which calls delete
	template<typename T>
	class Deleter {
	public:
		virtual void operator()(T* ptr);
	};
	template<typename T>
	void Deleter<T>::operator()(T* ptr) {
		delete ptr;
	}

	// Default deleter for arrays which calls delete[]
	template<typename T>
	class Deleter<T[]> {
	public:
		virtual void operator()(T* ptr);
	};
	template<typename T>
	void Deleter<T[]>::operator()(T* ptr) {
		delete[] ptr;
	}

	// UniquePtr
	template<typename T>
	class UniquePtr final {
	public:
		explicit UniquePtr(T* ptr = nullptr, const Deleter<T>& deleter = Deleter<T>());
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
		Deleter<T> deleter;
	};

	template<typename T>
	UniquePtr<T>::UniquePtr(T* ptr, const Deleter<T>& deleter) :ptr(ptr), deleter(deleter) {}
	template<typename T>
	T* UniquePtr<T>::GetRaw() const {
		return ptr;
	}
	template<typename T>
	UniquePtr<T>::~UniquePtr() {
		Reset();
	}
	template<typename T>
	T* UniquePtr<T>::Release() {
		T* ptr = this->ptr;
		this->ptr = nullptr;
		return ptr;
	}
	template<typename T>
	void UniquePtr<T>::Reset(T* ptr) {
		if (this->ptr != nullptr) {
			deleter(this->ptr);
		}
		this->ptr = ptr;
	}
	template<typename T>
	T& UniquePtr<T>::operator*() const {
		return *ptr;
	}
	template<typename T>
	T* UniquePtr<T>::operator->() const {
		return ptr;
	}
	template<typename T>
	UniquePtr<T>::operator bool() {
		return ptr != nullptr;
	}

	// UniquePtr for arrays
	template<typename T>
	class UniquePtr<T[]> {
	public:
		explicit UniquePtr(T* ptr = nullptr, const Deleter<T[]>& deleter = Deleter<T[]>());
		~UniquePtr();
		UniquePtr(const UniquePtr<T[]>& obj) = delete;
		UniquePtr& operator=(const UniquePtr<T[]> obj) = delete;
		T* GetRaw() const;
		T* Release();
		void Reset(T* ptr = nullptr);
		operator bool();
	private:
		T* ptr = nullptr;
		Deleter<T[]> deleter;
	};
	template<typename T>
	UniquePtr<T[]>::UniquePtr(T* ptr, const Deleter<T[]>& deleter) :ptr(ptr), deleter(deleter) {}
	template<typename T>
	T* UniquePtr<T[]>::GetRaw() const {
		return ptr;
	}
	template<typename T>
	UniquePtr<T[]>::~UniquePtr() {
		deleter(ptr);
	}
	template<typename T>
	T* UniquePtr<T[]>::Release() {
		T* ptr = this->ptr;
		this->ptr = nullptr;
		return ptr;
	}
	template<typename T>
	void UniquePtr<T[]>::Reset(T* ptr) {
		if (this->ptr != nullptr) {
			deleter(this->ptr);
		}
		this->ptr = ptr;
	}
	template<typename T>
	UniquePtr<T[]>::operator bool() {
		return ptr != nullptr;
	}
}

#endif