#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Object.h"
#include "Engine/Collection/List.h"
#include "Engine/Application/Node/NodePath.h"

namespace Engine {
	// Avoid circular dependency.
	class NodeTree;

	class Node :public ManualObject {
	public:
		Node* GetParent() const;
		int32 GetChildrenCount() const;
		Node* GetChild(int32 index) const;

		String GetName() const;
		void SetNameUnchecked(const String& name);
		void SetName(const String& name);
		Node* GetNode(const NodePath& path) const;
		Node* GetNodeOrNull(const NodePath& path) const;
		
		NodeTree* GetTree() const;
		bool IsInTree() const;

		// Validate a node name, removing invalid chars in it.
		static String ValidateNameBasically(const String& name);
		// Recieves a name, makes it validated for inserting the given parent.
		static String ValidateNameInParent(const String& validatedName, Node* parent);
	private:
		String name;
		List<Node*> children{};
		Node* parent = nullptr;

		NodeTree* tree = nullptr;

		static List<String> invalidChars;
	};
}