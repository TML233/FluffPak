#include "Engine/Application/Node.h"

namespace Engine {
	int32 Node::GetChildrenCount() const {
		return children.GetCount();
	}
	Node* Node::GetChild(int32 index) const {
		if (index < 0 || index>children.GetCount()) {
			throw ArgumentOutOfRangeException("index", "out of bounds.");
		}
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
