#pragma once

#include "Engine/System/Collection/List.h"
#include "Engine/System/Memory/SharedPtr.h"
#include "Engine/System/String.h"

namespace Engine {
	/// @brief Stores a parsed node path.
	class NodePath {
	public:
		NodePath(String path = u8"");


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