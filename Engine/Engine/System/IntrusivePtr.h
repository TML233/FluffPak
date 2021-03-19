#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Memory.h"

namespace Engine {
	template<typename T>
	class IntrusivePtr {
	public:
		// Create a SharedPtr from the given arguments.
		template<typename ... Args>
		static IntrusivePtr Create(Args&& ... args) {
			return IntrusivePtr(MEMNEW(T(Object::Forward<Args>(args)...)));
		}

		IntrusivePtr() {}
		explicit IntrusivePtr(T* ptr) :ptr(ptr) {
			if (ptr == nullptr) {
				return;
			}
			Reference();
		}

		IntrusivePtr(const IntrusivePtr& shared) :ptr(shared.ptr) {
			Reference();
		}
		IntrusivePtr& operator=(const IntrusivePtr& obj) {
			if (&obj == this) {
				return *this;
			}

			Dereference();

			ptr = obj.ptr;
			Reference();

			return *this;
		}
		~IntrusivePtr() {
			Dereference();
		}

		IntrusivePtr(IntrusivePtr&& obj) :ptr(obj.ptr) {
			obj.ptr = nullptr;
		}
		IntrusivePtr& operator=(IntrusivePtr&& obj) {
			if (&obj == this) {
				return *this;
			}

			Dereference();

			ptr = obj.ptr;
			obj.ptr = nullptr;

			return *this;
		}

		int32 GetReferenceCount() const {
			if (ptr == nullptr) {
				return 0;
			}
			return ptr->GetReferenceCount();
		}
		T* GetRaw() const {
			return ptr;
		}
		T* operator->() const {
			return ptr;
		}

		template<typename U>
		IntrusivePtr<U> StaticCastTo() {
			return IntrusivePtr<U>(static_cast<U*>(ptr));
		}
		template<typename U>
		IntrusivePtr<U> DynamicCastTo() {
			return IntrusivePtr<U>(dynamic_cast<U*>(ptr));
		}
	private:
		void Reference() {
			if (ptr == nullptr) {
				return;
			}
			ptr->Reference();
		}
		void Dereference() {
			if (ptr == nullptr) {
				return;
			}
			ptr->Dereference();
			if (ptr->GetReferenceCount() == 0) {
				MEMDEL(ptr);
			}
		}

		T* ptr = nullptr;
	};
}