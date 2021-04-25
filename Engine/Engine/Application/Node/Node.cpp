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

	bool Node::AddChild(Node* node,int index) {
		ERR_ASSERT(
			CanAddOrRemoveChildren(),
			"The node is busy preparing its children. Try not to add or remove nodes of the parent in OnEnteredTree() or OnReady().",
			return false
		);

		ERR_ASSERT(node != nullptr, "node is nullptr.", return false);
		ERR_ASSERT(node != this, "node can't be a child of itself.", return false);
		ERR_ASSERT(index <= GetChildrenCount(), "index out of bounds", return false);


		ERR_ASSERT(!node->HasParent(), "node already has a parent.", return false);

		if (index < 0) {
			index = GetChildrenCount();
		}

		children.Insert(index, node);

		String name = ValidateChildName(node->GetName(), nullptr, node->GetName(), this);
		node->SetNameUnchecked(name);
		node->parent = this;
		node->index = index;
		// Not assigning tree when preparing the tree because it is assigned later by the preparation procedure.
		// And not assigning the tree when exiting tree because the parent is leaving the tree.
		if (!preparingTree&&!exitingTree) {
			node->SystemAssignTree(tree);
		}
		return true;
	}
	bool Node::RemoveChild(Node* child) {
		ERR_ASSERT(
			CanAddOrRemoveChildren(),
			"The node is busy preparing its children. Try not to add or remove nodes of the parent in OnEnteredTree() or OnReady().",
			return false
		);

		ERR_ASSERT(child != nullptr, "child is nullptr.", return false);
		ERR_ASSERT(child != this, "child can't be itself.", return false);

		if (!child->HasParent()) {
			return false;
		}
		if (GetChildByIndex(child->GetIndex()) != child) {
			return false;
		}
		
		child->SystemAssignTree(nullptr);
		child->parent = nullptr;
		child->index = -1;
		children.RemoveAt(child->GetIndex());
	}
	
	bool Node::CanAddOrRemoveChildren() const {
		return !preparingTree;
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

		String validated = ValidateChildName(GetName(), GetParent(), ValidateName(name), GetParent());
		SetNameUnchecked(validated);
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
	String Node::ValidateChildName(const String& originalName,Node* originalParent, const String& targetName,Node* targetParent, ChildNameValidation method) {
		// Has no parent.
		if (targetParent == nullptr) {
			return targetName;
		}
		// The same parent, the same name.
		if (originalParent == targetParent && originalName == targetName) {
			return targetName;
		}
		// Not collided with other nodes in parent.
		if (targetParent->GetChildByName(targetName) == nullptr) {
			return targetName;
		}
		
		// If method not specified, automatically choose one.
		if (method == ChildNameValidation::NotSpecified) {
			// TODO: Choose a validation method based on environment.
			method = ChildNameValidation::Fast;
		}
		
		// Ordinal validation method.
		if (method == ChildNameValidation::Ordinal) {
			// Get the name before the digits.
			int split;
			for (split = targetName.GetCount() - 1; split >= 0; split -= 1) {
				char c = targetName[split];
				if (c < '0' || c>'9') {
					break;
				}
			}
			split += 1;
			String part = targetName.Substring(0, targetName.GetCount() - split);
			for (uint32 i = 1; i <= 25565; i += 1) {
				String candidate = String::Format(STRING_LITERAL("{0}{1}"), part, i);
				// Stop if the candidate name is the same as the original.
				if (originalParent == targetParent && candidate == originalName) {
					return originalName;
				}
				// Check if any node in targetParent is using the candidate name.
				Node* node = targetParent->GetChildByName(candidate);
				if (node == nullptr) {
					return candidate;
				}
			}
		}
		// If the specified validation failed(unlikely), fallback to the auto name method.
		return GenerateAutoName();
	}

	String Node::GenerateAutoName() {
		return String::Format(STRING_LITERAL("@@Auto@{0}"), autoNameCounter.FetchAdd(1));
	}

	void Node::SystemAssignTree(NodeTree* tree) {
		if (this->tree == tree) {
			return;
		}
		if (this->tree != nullptr && tree != nullptr) {
			FATAL_CRASH("Cannot assign the tree when the node is already in another tree! Check the code!!");
		}
		
		if (this->tree == nullptr && tree != nullptr) {
			// Current has no tree, assigning into a tree.
			this->tree = tree;
			OnEnteredTree();

			// Lock the current node to prevent the child from adding or removing nodes into the current node.
			preparingTree = true;
			for (int32 i = 0; i < children.GetCount(); i += 1) {
				children.Get(i)->SystemAssignTree(tree);
			}
			preparingTree = false;

			OnReady();
		} else if (this->tree != nullptr && tree == nullptr) {
			// Current has tree, removing the tree assignment.
			preparingTree = true;
			for (int32 i = 0; i < children.GetCount(); i += 1) {
				children.Get(i)->SystemAssignTree(nullptr);
			}
			preparingTree = false;

			// Make a exiting tree flag to prevent AddChild(...) prepares the child tree.
			// But why the F someone add nodes when exiting tree??
			exitingTree = true;
			OnExitingTree();
			exitingTree = false;

			this->tree = nullptr;
		}
	}
}
