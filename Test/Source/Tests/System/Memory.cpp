#include "doctest.h"
#include "Engine/System/Memory/Memory.h"
#include "Engine/System/Memory/SharedPtr.h"
#include "Engine/System/Memory/CopyOnWrite.h"

using namespace Engine;

class ComplexClass {
public:
	ComplexClass(int32 v=0) {
		heap = MEMNEW(int32);
		*heap = v;
	}
	~ComplexClass() {
		MEMDEL(heap);
	}
	ComplexClass(const ComplexClass& obj) {
		heap = MEMNEW(int32);
		*heap = *(obj.heap);
	}
	ComplexClass& operator=(const ComplexClass& obj) {
		MEMDEL(heap);
		heap = MEMNEW(int32);
		*heap = *(obj.heap);
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
	TEST_CASE("Necessary destruction check") {
		CHECK(!Memory::IsDestructionNeeded<int>());
		CHECK(Memory::IsDestructionNeeded<ComplexClass>());
	}

	TEST_CASE("C Style Management") {
		int32* ptr = (int32*)Memory::Allocate(sizeof(int32));
		*ptr = 114514;
		Memory::Deallocate(ptr);

		ComplexClass* c = (ComplexClass*)Memory::Allocate(sizeof(ComplexClass));
		Memory::Construct(c, 1234);
		CHECK(c->Get() == 1234);
		Memory::Destruct(c);
		Memory::Deallocate(c);
	}

	TEST_CASE("CPP Style Management") {
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

	TEST_CASE("CopyOnWrite") {
		using Type = CopyOnWrite<ComplexClass>;
		Type a = Type::Create();
		CHECK(a.IsExclusive());
		
		Type b = a;
		CHECK(!a.IsExclusive());
		CHECK(!b.IsExclusive());

		Type c = b;
		CHECK(!c.IsExclusive());

		c.DoRead();
		CHECK(!a.IsExclusive());
		CHECK(!b.IsExclusive());
		CHECK(!c.IsExclusive());

		c.DoWrite();
		CHECK(!a.IsExclusive());
		CHECK(!b.IsExclusive());
		CHECK(c.IsExclusive());
	}
}