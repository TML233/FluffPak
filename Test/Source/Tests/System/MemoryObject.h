#pragma once
#include "Engine/System/Memory/Memory.h"

class MemoryObject {
public:
	MemoryObject(::Engine::int32 v = 0) {
		heap = MEMNEW(::Engine::int32);
		*heap = v;
	}
	~MemoryObject() {
		MEMDEL(heap);
	}
	MemoryObject(const MemoryObject& obj) {
		heap = MEMNEW(::Engine::int32);
		*heap = *(obj.heap);
	}
	MemoryObject& operator=(const MemoryObject& obj) {
		MEMDEL(heap);
		heap = MEMNEW(::Engine::int32);
		*heap = *(obj.heap);
		
		return *this;
	}
	::Engine::int32 Get() const {
		return *heap;
	}
private:
	::Engine::int32* heap = nullptr;
};