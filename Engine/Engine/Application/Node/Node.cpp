#include "Engine/Application/Node/Node.h"
#include "Engine/System/Debug.h"

namespace Engine {
	Node* Node::GetParent() const {
		return parent;
	}
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
	void Node::SetNameUnchecked(const String& name) {
		this->name = name;
	}
	void Node::SetName(const String& name) {
		SetNameUnchecked(ValidateNameInParent(ValidateNameBasically(name), parent));
	}

	List<String> Node::invalidChars = { ".","/",":","\r","\n" };
	String Node::ValidateNameBasically(const String& name) {
		if (name.GetCount() <= 0) {
			return STRING_LITERAL("@");
		}

		String result = name;
		for (const String& c : invalidChars) {
			result.Replace(c, String::GetEmpty());
		}

		if (result.GetCount() <= 0) {
			return STRING_LITERAL("@");
		}
		return result;
	}
}
