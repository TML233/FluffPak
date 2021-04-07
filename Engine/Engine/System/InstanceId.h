#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Atomic.h"

namespace Engine{
	struct InstanceId{
	public:
		static InstanceId Generate(bool referenced);

		explicit InstanceId(uint64 id = 0);
		uint64 Get() const;
		bool IsValid() const;
		bool IsReferenced() const;
		bool operator==(const InstanceId& obj) const;
		bool operator!=(const InstanceId& obj) const;

		int32 GetHashCode() const;

	private:
		// The highest bit == 1 == ReferencedObject
		uint64 value;

		static AtomicValue<uint64> lastManual;
		static AtomicValue<uint64> lastReferenced;
	};
}
