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
		void SetNameUnchecked(String name);
		void SetName(String name);
		Node* GetNode(NodePath path) const;
		Node* GetNodeOrNull(NodePath path) const;
		
		NodeTree* GetTree() const;
		bool IsInTree() const;

		static bool IsNameValid(String name);
		static String ValidateName(String name);
	private:
		String name;
		List<Node*> children{};
		Node* parent = nullptr;

		NodeTree* tree = nullptr;

		static List<String> invalidChars;
	};
}