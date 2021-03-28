#include "Engine/System/InstanceId.h"
#include "Engine/System/ObjectUtil.h"

namespace Engine {
	AtomicValue<uint64> InstanceId::lastManual{};
	AtomicValue<uint64> InstanceId::lastReferenced{};

	InstanceId InstanceId::Generate(bool referenced) {
		InstanceId r{};
		if (!referenced) {
			r.value = lastManual.Add(1);
		} else {
			r.value = lastReferenced.Add(1) | (uint64)1 << 63;
		}
		return r;
	}

	InstanceId::InstanceId(uint64 id) :value(id) {}
	uint64 InstanceId::Get() const {
		return value;
	}
	bool InstanceId::IsValid() const {
		return value != 0;
	}
	bool InstanceId::IsReferenced() const {
		return value >> 63 == 1;
	}
	bool InstanceId::operator==(const InstanceId& obj) const {
		return value == obj.value;
	}
	bool InstanceId::operator!=(const InstanceId& obj) const {
		return value != obj.value;
	}

	int32 InstanceId::GetHashCode() const {
		return ObjectUtil::GetHashCode(value);
	}
}
