#pragma once

#include "Engine/System/Definition.h"
#include "Engine/Collection/Dictionary.h"
#include "Engine/System/SharedPtr.h"
#include "Engine/System/Variant.h"
#include "Engine/System/String.h"

namespace Engine{
	class ReflectionClass;
	class ReflectionMethod;
	class ReflectionProperty;
	class ReflectionSignal;

	class Reflection final {
	public:
		STATIC_CLASS(Reflection);

		static bool IsClassExists(const String& name);

		static ReflectionClass* GetClass(const String& name);
		
		template<typename T>
		static void AddClass() {
			SharedPtr<ReflectionClass> data = SharedPtr<ReflectionClass>::Create();

			data->name = T::GetClassNameStatic();
			data->parentName = T::GetParentClassNameStatic();

			GetData().Add(T::GetClassNameStatic(), data);
		}

	private:
		using ClassData = Dictionary<String, SharedPtr<ReflectionClass>>;
		static ClassData& GetData();
	};
	class ReflectionClass final {
	public:
		String GetName() const;
		String GetParentName() const;
	private:
		friend class Reflection;
		String name;
		String parentName;
	};
}

#pragma region Common name getter.
#define _REFLECTION_CLASS_NAME_GETTERS(name,parent)				\
static ::Engine::String GetClassNameStatic(){					\
	return STRING_LITERAL(name);								\
}																\
virtual ::Engine::String GetClassName() const{					\
	return STRING_LITERAL(name);								\
}																\
static ::Engine::String GetParentClassNameStatic(){				\
	return STRING_LITERAL(parent);								\
}																\
virtual ::Engine::String GetParentClassName() const{			\
	return STRING_LITERAL(parent);								\
}
#pragma endregion

#pragma region Auto initializer.
#define _REFLECTION_CLASS_AUTO_INITIALIZER()					\
	class _ReflectionInitializer{								\
	public:														\
		_ReflectionInitializer(){								\
			/*WARN_MSG("Reflection Initializer!");*/			\
			InitializeReflectionClass();						\
		}														\
	};															\
	inline static _ReflectionInitializer _reflectionInitializer{}
#pragma endregion

// Name and ParentName must be located from the root namespace!
// e.g. Object should be Engine::Object
#define REFLECTION_CLASS(name,parent)							\
public:															\
	_REFLECTION_CLASS_NAME_GETTERS(#name,#parent);				\
																\
protected:														\
	static void InitializeReflectionClass(){					\
		static bool inited=false;								\
		if(inited){												\
			return;												\
		}														\
		parent::InitializeReflectionClass();					\
		::Engine::Reflection::AddClass<name>();					\
																\
		inited=true;											\
	}															\
private:														\
	_REFLECTION_CLASS_AUTO_INITIALIZER();


#define REFLECTION_CLASS_ROOT(name)								\
public:															\
	_REFLECTION_CLASS_NAME_GETTERS(#name,"");					\
																\
protected:														\
	static void InitializeReflectionClass(){					\
		static bool inited=false;								\
		if(inited){												\
			return;												\
		}														\
		::Engine::Reflection::AddClass<name>();					\
																\
		inited=true;											\
	}															\
private:														\
	_REFLECTION_CLASS_AUTO_INITIALIZER();