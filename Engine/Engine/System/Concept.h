#pragma once
#include "Engine/System/Definition.h"

namespace Engine {
	class Object;
	class ReferencedObject;

	namespace Concept {
		template<typename T>
		concept IsObject = std::is_base_of_v<Object, T>;

		template<typename T>
		concept IsReferencedObject = std::is_base_of_v<ReferencedObject, T>;

		template<typename T>
		concept IsEnum = std::is_enum_v<T>;
	}
}