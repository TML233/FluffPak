#include "doctest.h"
#include "Engine/System/Memory.h"
#include "Engine/System/SharedPtr.h"

using namespace Engine;

class ComplexClass {
public:
	ComplexClass(int32 v=0) {
		heap = new int32;
		*heap = v;
	}
	~ComplexClass() {
		delete heap;
	}
	int32 Get() const {
		return *heap;
	}
private:
	int32* heap = nullptr;
};

class Base {

};
class Derived :public Base {

};

TEST_SUITE("Memory"){
	TEST_CASE("Memory - Necessary destruction check") {
		CHECK(!Memory::IsDestructionNeeded<int>());
		CHECK(Memory::IsDestructionNeeded<ComplexClass>());
	}

	TEST_CASE("Memory - C Style Management") {
		int32* ptr = (int32*)Memory::Allocate(sizeof(int32));
		*ptr = 114514;
		Memory::Deallocate(ptr);

		ComplexClass* c = (ComplexClass*)Memory::Allocate(sizeof(ComplexClass));
		Memory::Construct(c, 1234);
		CHECK(c->Get() == 1234);
		Memory::Destruct(c);
		Memory::Deallocate(c);
	}

	TEST_CASE("Memory - CPP Style Management") {
		// Value type
		int32* ptr = MEMNEW(int32(1919810));
		CHECK(*ptr == 1919810);
		MEMDEL(ptr);

		// Class type
		ComplexClass* classptr = MEMNEW(ComplexClass(893));
		CHECK(classptr->Get() == 893);
		MEMDEL(classptr);

		// Value array
		int32* ptrarr = MEMNEWARR(int32, 8);
		CHECK(Memory::GetHeapArrayElementCount(ptrarr) == 8);
		MEMDELARR(ptrarr);
	}

	TEST_CASE("SharedPtr") {
		SharedPtr<Derived> d = SharedPtr<Derived>::Create();
		SharedPtr<Base> b{ d };
	}
}