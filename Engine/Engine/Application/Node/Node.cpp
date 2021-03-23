#include "Engine/Application/Node/Node.h"
#include "Engine/System/Debug.h"

namespace Engine {
	int32 Node::GetChildrenCount() const {
		return children.GetCount();
	}
	Node* Node::GetChild(int32 index) const {
		ERR_ASSERT(index >= 0 && index < children.GetCount(), "index out of bounds.", return nullptr);
		return children.Get(index);
	}
	String Node::GetName() const {
		return name;
	}
	void Node::SetNameUnchecked(String name) {
		this->name = name;
	}

	List<String> Node::invalidChars = { ".","/",":" };
	bool Node::IsNameValid(String name) {
		for (String c : invalidChars) {
			if (name.Contains(c)) {
				return false;
			}
		}
		return true;
	}
}
