#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Memory.h"

namespace Engine {
	template<typename T>
	class ReferencePtr {
	public:
		// Create a SharedPtr from the given arguments.
		template<typename ... Args>
		static ReferencePtr Create(Args&& ... args) {
			return ReferencePtr(MEMNEW(T(Memory::Forward<Args>(args)...)));
		}

		ReferencePtr() {}
		explicit ReferencePtr(T* ptr) :ptr(ptr) {
			if (ptr == nullptr) {
				return;
			}
			Reference();
		}

		ReferencePtr(const ReferencePtr& shared) :ptr(shared.ptr) {
			Reference();
		}
		ReferencePtr& operator=(const ReferencePtr& obj) {
			if (&obj == this) {
				return *this;
			}

			Dereference();

			ptr = obj.ptr;
			Reference();

			return *this;
		}
		~ReferencePtr() {
			Dereference();
		}

		ReferencePtr(ReferencePtr&& obj) :ptr(obj.ptr) {
			obj.ptr = nullptr;
		}
		ReferencePtr& operator=(ReferencePtr&& obj) {
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
		ReferencePtr<U> StaticCastTo() {
			return ReferencePtr<U>(static_cast<U*>(ptr));
		}
		template<typename U>
		ReferencePtr<U> DynamicCastTo() {
			return ReferencePtr<U>(dynamic_cast<U*>(ptr));
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
			if (ptr->Dereference() == 0) {
				MEMDEL(ptr);
			}
		}

		T* ptr = nullptr;
	};
}