#include "Engine/System/Debug.h"
#include "Engine/System/Object.h"
#include "Engine/System/Atomic.h"
#include "Engine/System/ReferencePtr.h"
#include "Engine/System/SharedPtr.h"

using namespace Engine;

class Base {
public:
	Base() {
		INFO_MSG("INTRUSIVE Ctor");
	}
	~Base() {
		INFO_MSG("INTRUSIVE Dtor");
	}

	uint32 Reference() {
		return referenceCount.Reference();
	}
	uint32 Dereference() {
		return referenceCount.Dereference();
	}
	uint32 GetReferenceCount() const {
		return referenceCount.Get();
	}

private:
	ReferenceCount referenceCount;
};

class Bar {
public:
	Bar() {
		INFO_MSG("SHARED Ctor");
	}
	~Bar() {
		INFO_MSG("SHARED Dtor");
	}
};

int main() {
	SharedPtr<Bar> sptr = SharedPtr<Bar>::Create();

	Base* raw = MEMNEW(Base);
	ReferencePtr<Base> ptr(raw);
	ReferencePtr<Base> ptr2(raw);
}
