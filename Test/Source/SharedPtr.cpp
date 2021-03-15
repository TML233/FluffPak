#include "doctest.h"
#include "Engine/System/SharedPtr.h"

class Base {
public:
	virtual ~Base() {
		MESSAGE("BASE Dtor");
	}
	Base() {
		MESSAGE("BASE Ctor");
	}
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

TEST_CASE("SharedPtr"){
	SharedPtr<Derived> ptr(new Derived());
	SharedPtr<Base> base = ptr.StaticCastTo<Base>();
	SharedPtr<Derived> casted = base.DynamicCastTo<Derived>();
	casted->Print();
}
