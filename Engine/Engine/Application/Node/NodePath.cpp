#include "Engine/Application/Node/NodePath.h"
#include "Engine/System/Debug.h"

namespace Engine {
	NodePath::NodePath(String path) {

	}

	int32 NodePath::GetNameCount() const {
		if (data == nullptr) {
			return 0;
		}
		return data->names.GetCount();
	}
	String NodePath::GetName(int32 index) const {
		ERR_ASSERT(index >= 0 && index < GetNameCount(), "index out of bounds.", return String());

		return data->names.Get(index);
	}
	int32 NodePath::GetSubnameCount() const {
		if (data == nullptr) {
			return 0;
		}
		return data->subnames.GetCount();
	}
	String NodePath::GetSubname(int32 index) const {
		ERR_ASSERT(index >= 0 && index < GetSubnameCount(), "index out of bounds.", return String());

		return data->subnames.Get(index);
	}
}
