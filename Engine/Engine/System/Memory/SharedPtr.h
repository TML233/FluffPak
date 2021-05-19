#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Memory/Memory.h"
#include "Engine/System/Thread/Atomic.h"

namespace Engine {
	struct SharedPtrCounter {
		ReferenceCount refCount;
		ReferenceCount weakCount;
	};

	template<typename T>
	class SharedPtr {
	public:
		template<typename ... Args>
		static SharedPtr Create(Args&& ... args) {
			return SharedPtr(MEMNEW(T(Memory::Forward<Args>(args)...)));
		}

		SharedPtr(T* ptr, SharedPtrCounter* data) :ptr(ptr), data(data) {
			Reference();
		}

		SharedPtr() {}
		explicit SharedPtr(T* ptr) :ptr(ptr) {
			if (ptr == nullptr) {
				return;
			}
			data = MEMNEW(SharedPtrCounter);
			Reference();
		}

		SharedPtr(const SharedPtr& shared) :ptr(shared.ptr), data(shared.data) {
			Reference();
		}

		template<typename U>
		friend class SharedPtr;

		template<typename U,typename = std::is_convertible<U,T>>
		SharedPtr(const SharedPtr<U>& shared):ptr(shared.ptr),data(shared.data) {
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

		SharedPtr(SharedPtr&& obj) :ptr(obj.ptr), data(obj.data) {
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

		uint32 GetReferenceCount() const {
			return data->refCount.Get();
		}

		bool operator==(const T* ptr) const {
			return this->ptr == ptr;
		}
		bool operator!=(const T* ptr) const {
			return this->ptr != ptr;
		}
		operator bool() {
			return ptr != nullptr;
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
			data->refCount.Reference();
		}
		void Dereference() {
			if (data == nullptr) {
				return;
			}
			if (data->refCount.Dereference() == 0) {
				MEMDEL(ptr);

				if (data->weakCount.Get() == 0) {
					MEMDEL(data);
				}
			}
		}

		T* ptr = nullptr;
		SharedPtrCounter* data = nullptr;
	};
}