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
		SharedPtr(T* ptr, SharedPtrCounter* data) :ptr(ptr), data(data) {
			Reference();
		}

		SharedPtr(T* ptr):ptr(ptr) {
			data = MEMNEW(SharedPtrCounter);
			Reference();
		}
		SharedPtr(const SharedPtr& shared) :ptr(shared.ptr), data(shared.data) {
			Reference();
		}
		SharedPtr& operator=(const SharedPtr& obj) {
			Dereference();

			ptr = obj.ptr;
			data = obj.data;
			Reference();
		}
		~SharedPtr() {
			Dereference();
		}
		// No moving.
		SharedPtr(SharedPtr&&) = delete;
		SharedPtr& operator=(SharedPtr&&) = delete;

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
			data->refCount += 1;
		}
		void Dereference() {
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