#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/Object.h"
#include "Engine/Collection/List.h"

namespace Engine {
	class Node :public ManualObject {
	public:
		int32 GetChildrenCount() const;
		Node* GetChild(int32 index) const;

		String GetName() const;
		void SetNameUnchecked(String name);

		static bool IsNameValid(String name);
		static String ValidateName(String name);
	private:
		String name;
		List<Node*> children{};
		Node* parent = nullptr;
		bool inTree = false;

		static List<String> invalidChars;
	};
}