#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Atomic.h"

namespace Engine{
	struct ObjectId{
	public:
		static ObjectId Generate(bool referenced);

		explicit ObjectId(uint64 id = 0);
		uint64 Get() const;
		bool IsValid() const;
		bool IsReferenced() const;
		bool operator==(const ObjectId& obj) const;
		bool operator!=(const ObjectId& obj) const;

	private:
		// The highest bit == 1 == ReferencedObject
		uint64 value;

		static AtomicValue<uint64> lastManual;
		static AtomicValue<uint64> lastReferenced;
	};
}
