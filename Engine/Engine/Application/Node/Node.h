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
		/// @brief Destroys the node and all its children.
		virtual ~Node();

		/// @brief Get the parent of this node.
		/// @return The parent of this node.\n
		/// nullptr when it has no parent.
		Node* GetParent() const;
		/// @brief Check if the current node has a parent.
		bool HasParent() const;
		/// @brief Get the children count.
		int32 GetChildrenCount() const;
		/// @brief Get the child by the given index.
		Node* GetChildByIndex(int32 index) const;
		/// @brief Get the child by the given name.
		Node* GetChildByName(const String& name) const;

		bool MoveChild(int32 from, int32 to);

		/// @brief Check if the given node is a child of the current node.
		bool IsChild(Node* node) const;

		/// @brief Add a node as a child of current node.
		/// @param node The node to add.
		/// @param index The position to insert at. -1 for the last position.
		bool AddChild(Node* node, int index = -1);

		/// @brief Remove a child from the current node.
		/// @param The node to remove.
		bool RemoveChild(Node* child);

		/// @brief Check if other nodes can add or remove child for the current node.\n
		/// When preparing node tree, the parent node is locked to prevent the data from out of sync.  
		bool CanAddOrRemoveChildren() const;

		/// @brief Get the index of this node inside its parent.
		/// @return The index inside its parent.\n
		/// -1 when it has no parent.
		int GetIndex() const;
 
		/// @brief Get the name of the node.
		String GetName() const;
		
		/// @brief Set the name of the node directly, without the name check.\n
		/// This might cause some problems in certain situation. 
		void SetNameUnchecked(const String& name);

		/// @brief Set the name of the node directly, with the name check.\n
		/// The invalid characters in the name will be removed.\n
		/// A suffix number will be added if a node in the parent has the same name.
		void SetName(const String& name);

		/// @brief Find the node with the given path. Produces error messages when not found.
		/// @return The found node.\n
		/// nullptr when not found.
		Node* GetNode(const NodePath& path) const;

		/// @brief Find the node with the given path. Will not produce error messages when not found.
		/// @return The found node.\n
		/// nullptr when not found.
		Node* GetNodeOrNull(const NodePath& path) const;
		
		/// @brief Get the NodeTree the current node is in.
		/// @return The NodeTree the current node is in.\n
		/// nullptr when not in the NodeTree.
		NodeTree* GetTree() const;

		/// @brief Check if the current node is in the NodeTree.
		bool IsInTree() const;

		/// @brief Called right after the current node entered the NodeTree.
		virtual void OnEnteredTree();
		/// @brief Called when all children of the current node is ready.
		virtual void OnReady();
		/// @brief Called when logic update occurs.
		/// @param delta Elapsed seconds since last Update.
		virtual void OnUpdate(float delta);
		/// @brief Called when physics update occurs.
		/// @param delta Elapsed seconds since last PhysicsUpdate.
		virtual void OnPhysicsUpdate(float delta);
		/// @brief Called right before the current node exits the NodeTree.
		virtual void OnExitingTree();

		/// @brief Validate a node name, removing invalid chars in it.
		static String ValidateName(const String& name);

		enum class ChildNameValidation {
			/// @brief System will automatically choose a validation method.
			NotSpecified,
			/// @brief Human-readable name style.\n
			/// e.g., "CollidedName" -> "CollidedName1"
			Ordinal,
			/// @brief Use auto generated names, less human-readable.
			/// e.g., "CollidedName" -> "@@Auto@700"
			Fast
		};

		/// @brief Validate a node name, makes it suitable for inserting into the given parent.\n
		/// Given name must be validated by ValidateName method first!\n
		/// @param originalName The original name.
		/// @param orginalParent The original parent.
		/// @param targetName The name to validate. Must be grammerly validated by ValidateName(...) first.
		/// @param targetParent The parent for "inserting".
		/// @param method The method to validate the name. Choose automatically when not specified.
		/// @return The validated name.
		static String ValidateChildName(
			const String& originalName, Node* orginalParent,
			const String& targetName,Node* targetParent,
			ChildNameValidation method = ChildNameValidation::NotSpecified
		);

		/// @brief Generates an auto node name.\n
		/// Guaranteed not collided with other node names.
		static String GenerateAutoName();
	private:
		String name;
		List<Node*> children{};
		Node* parent = nullptr;
		int index = -1;

		NodeTree* tree = nullptr;

		bool preparingTree = false;
		bool exitingTree = false;

		static List<String> invalidChars;
		static AtomicValue<uint32> autoNameCounter;

		friend class NodeTree;

		void SystemUpdate(float delta);
		void SystemPhysicsUpdate(float delta);
		void SystemAssignTree(NodeTree* tree);
	};
}