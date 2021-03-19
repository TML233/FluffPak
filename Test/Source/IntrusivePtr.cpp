#include "doctest.h"
#include "Engine/System/IntrusivePtr.h"
#include <atomic>

using namespace Engine;

class Base {
public:
	Base() {
		MESSAGE("BASE Ctor");
	}
	virtual ~Base() {
		MESSAGE("BASE Dtor");
	}

	void Reference() {
		referenceCount += 1;
	}
	void Dereference() {
		referenceCount -= 1;
	}
	int32 GetReferenceCount() const {
		return referenceCount;
	}

private:
	std::atomic<int32> referenceCount;
};
class Derived :public Base {
public:
	Derived() {
		MESSAGE("DERIVED Ctor");
	}
	~Derived() {
		MESSAGE("DERIVED Dtor");
	}
	void Print() const {
		MESSAGE("HI!");
	}
};

TEST_CASE("IntrusivePtr") {
	Derived* raw = MEMNEW(Derived);
	IntrusivePtr<Derived> ptr(raw);
	IntrusivePtr<Derived> ptr2(raw);
	ptr->Print();
}
