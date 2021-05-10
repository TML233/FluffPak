#include "Engine/System/Memory/SharedPtr.h"

namespace Engine {
	template<typename T>
	class CopyOnWrite {
	public:
		CopyOnWrite() {}
		CopyOnWrite(T* ptr) :ptr(SharedPtr<T>(ptr)) {}

		template<typename ... TArgs>
		static CopyOnWrite Create(TArgs&& ... args) {
			return CopyOnWrite(MEMNEW(T(Memory::Forward<TArgs>(args)...)));
		}

		const T* DoRead() const {
			return ptr.GetRaw();
		}
		T* DoWrite() {
			if (ptr == nullptr) {
				return nullptr;
			}

			// Not exclusive, then copy.
			if (!IsExclusive()) {
				SharedPtr<T> copied = SharedPtr<T>::Create(*(ptr.GetRaw()));
				ptr = copied;
			}
			return ptr.GetRaw();
		}
		bool IsExclusive() const {
			return ptr == nullptr || ptr.GetReferenceCount() <= 1;
		}
	private:
		SharedPtr<T> ptr;
	};
}
