#pragma once

#include "Engine/Collection/List.h"
#include "Engine/System/SharedPtr.h"
#include "Engine/System/String.h"

namespace Engine {
	class NodePath {
	public:
		NodePath(String path = "");


		int32 GetNameCount() const;
		String GetName(int32 index) const;
		int32 GetSubnameCount() const;
		String GetSubname(int32 index) const;

	private:
		struct Data {
			bool absolute = false;
			List<String> names{};
			List<String> subnames{};
		};
		SharedPtr<Data> data;
	};
}