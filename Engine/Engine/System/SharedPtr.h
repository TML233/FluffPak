#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Memory.h"
#include <atomic>

namespace Engine{
	struct SharedPtrCounter {
		std::atomic<int32> refCount = 0;
		std::atomic<int32> weakCount = 0;
	};

	template<typename T>
	class SharedPtr {
	public:
		// Create a SharedPtr from the given arguments.
		template<typename ... Args>
		static SharedPtr Create(Args&& ... args) {
			return SharedPtr(MEMNEW(T(Object::Forward<Args>(args)...)));
		}

		SharedPtr(T* ptr, SharedPtrCounter* data) :ptr(ptr), data(data) {
			Reference();
		}

		SharedPtr() {}
		explicit SharedPtr(T* ptr):ptr(ptr) {
			if (ptr == nullptr) {
				return;
			}
			data = MEMNEW(SharedPtrCounter);
			Reference();
		}

		SharedPtr(const SharedPtr& shared) :ptr(shared.ptr), data(shared.data) {
			Reference();
		}
		SharedPtr& operator=(const SharedPtr& obj) {
			if (&obj == this) {
				return *this;
			}

			Dereference();

			ptr = obj.ptr;
			data = obj.data;
			Reference();

			return *this;
		}
		~SharedPtr() {
			Dereference();
		}
		
		SharedPtr(SharedPtr&& obj):ptr(obj.ptr),data(obj.data) {
			obj.ptr = nullptr;
			obj.data = nullptr;
		}
		SharedPtr& operator=(SharedPtr&& obj) {
			if (&obj == this) {
				return *this;
			}

			Dereference();

			ptr = obj.ptr;
			data = obj.data;
			obj.ptr = nullptr;
			obj.data = nullptr;

			return *this;
		}

		int32 GetReferenceCount() const {
			return data->refCount;
		}
		T* GetRaw() const {
			return ptr;
		}
		T* operator->() const {
			return ptr;
		}

		template<typename U>
		SharedPtr<U> StaticCastTo() {
			return SharedPtr<U>(static_cast<U*>(ptr), data);
		}
		template<typename U>
		SharedPtr<U> DynamicCastTo() {
			return SharedPtr<U>(dynamic_cast<U*>(ptr), data);
		}
	private:
		void Reference() {
			if (data == nullptr) {
				return;
			}
			data->refCount += 1;
		}
		void Dereference() {
			if (data == nullptr) {
				return;
			}
			data->refCount -= 1;
			if (data->refCount <= 0) {
				MEMDEL(ptr);

				if (data->weakCount <= 0) {
					MEMDEL(data);
				}
			}
		}

		T* ptr = nullptr;
		SharedPtrCounter* data = nullptr;
	};
}