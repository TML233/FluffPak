#include "Engine/Application/Node/Node.h"
#include "Engine/System/Debug.h"

namespace Engine {
	Node* Node::GetParent() const {
		return parent;
	}
	bool Node::HasParent() const {
		return parent != nullptr;
	}
	int32 Node::GetChildrenCount() const {
		return children.GetCount();
	}
	Node* Node::GetChildByIndex(int32 index) const {
		ERR_ASSERT(index >= 0 && index < children.GetCount(), "index out of bounds.", return nullptr);
		return children.Get(index);
	}
	Node* Node::GetChildByName(const String& name) const {
		if (name.GetCount() <= 0) {
			return nullptr;
		}
		for (Node* child : children) {
			if (child->GetName() == name) {
				return child;
			}
		}
		return nullptr;
	}
	
	bool Node::IsChild(Node* node) const {
		if (node == nullptr) {
			return false;
		}
		for (Node* child : children) {
			if (child == node) {
				return true;
			}
		}
		return false;
	}

	int Node::GetIndex() const {
		return index;
	}
	String Node::GetName() const {
		return name;
	}
	void Node::SetNameUnchecked(const String& name) {
		this->name = name;
	}
	void Node::SetName(const String& name) {
		if (name == GetName()) {
			return;
		}
		// Node names that start with @@ is auto names.
		if (name.StartsWith(STRING_LITERAL("@@"))) {
			return;
		}

		SetNameUnchecked(ValidateChildName(ValidateName(name), GetName(), parent));
	}

	Node* Node::GetNode(const NodePath& path) const {
		Node* node = GetNodeOrNull(path);
		ERR_ASSERT(node != nullptr, "Cannot find node with the given path.", return nullptr);
		return node;
	}

	NodeTree* Node::GetTree() const {
		return tree;
	}
	bool Node::IsInTree() const {
		return tree != nullptr;
	}

	List<String> Node::invalidChars = { ".","/",":","\r","\n" };
	String Node::ValidateName(const String& name) {
		if (name.GetCount() <= 0) {
			return STRING_LITERAL("@");
		}

		String result = name;
		for (const String& c : invalidChars) {
			result.Replace(c, String::GetEmpty());
		}

		if (result.GetCount() <= 0) {
			return STRING_LITERAL("@Unnamed_0");
		}
		return result;
	}
	String Node::ValidateChildName(const String& name, const String& original,Node* parent,ChildNameValidation method) {
		switch (method) {
			case ChildNameValidation::HumanReadable:
				return ValidateChildNameHumanReadable(name, original, parent);
			case ChildNameValidation::Fast:
				return ValidateChildNameFast(name, parent);
			default:
				return ValidateChildNameFast(name, parent);
		}
	}

	String Node::ValidateChildNameFast(const String& name, Node* parent) {
		if (parent->GetChildByName(name) == nullptr) {
			return name;
		}
		return GenerateAutoName();
	}
	String Node::ValidateChildNameHumanReadable(const String& name, const String& original,Node* parent) {
		if (parent->GetChildByName(name) == nullptr) {
			return name;
		}
		// Get the name before the digits.
		int split;
		for (split = name.GetCount() - 1; split >= 0; split -= 1) {
			char c = name[split];
			if (c < '0' || c>'9') {
				break;
			}
		}
		split += 1;
		String part = name.Substring(0, name.GetCount() - split);
		for(uint32 i=1;i<2147483647;i+=1){
			String candidate = String::Format(STRING_LITERAL("{0}{1}"), part, i);
			// Stop if the candidate name is the same as the original.
			if (candidate == original) {
				return name;
			}
			Node* node = parent->GetChildByName(candidate);
			// No node is using this name, push the candidate.
			if (node == nullptr) {
				return candidate;
			}
		}
		// If no name is available(unlikely), fallback to the auto name.
		return GenerateAutoName();
	}

	String Node::GenerateAutoName() {
		return String::Format(STRING_LITERAL("@@Auto@{0}"), autoNameCounter.FetchAdd(1));
	}
}
