#include "Engine/System/ObjectId.h"

namespace Engine {
	AtomicValue<uint64> ObjectId::lastManual{};
	AtomicValue<uint64> ObjectId::lastReferenced{};

	ObjectId ObjectId::Generate(bool referenced) {
		ObjectId r{};
		if (!referenced) {
			r.value = lastManual.Add(1);
		} else {
			r.value = lastReferenced.Add(1) | (uint64)1 << 63;
		}
		return r;
	}

	ObjectId::ObjectId(uint64 id = 0) :value(id) {}
	uint64 ObjectId::Get() const {
		return value;
	}
	bool ObjectId::IsValid() const {
		return value != 0;
	}
	bool ObjectId::IsReferenced() const {
		return value >> 63 == 1;
	}
	bool ObjectId::operator==(const ObjectId& obj) const {
		return value == obj.value;
	}
	bool ObjectId::operator!=(const ObjectId& obj) const {
		return value != obj.value;
	}
}
