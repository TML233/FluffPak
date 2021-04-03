#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Atomic.h"
#include "Engine/System/InstanceId.h"
#include "Engine/Collection/Dictionary.h"
#include "Engine/System/UniquePtr.h"
#include "Engine/System/SharedPtr.h"
#include "Engine/System/Variant.h"

#pragma region Reflection macros

#pragma region Common name getter.
#define _REFLECTION_CLASS_NAME_GETTERS(name,parent)									\
static ::Engine::String GetClassNameStatic(){										\
	return STRING_LITERAL(name);													\
}																					\
virtual ::Engine::String GetClassName() const{										\
	return STRING_LITERAL(name);													\
}																					\
static ::Engine::String GetParentClassNameStatic(){									\
	return STRING_LITERAL(parent);													\
}																					\
virtual ::Engine::String GetParentClassName() const{								\
	return STRING_LITERAL(parent);													\
}
#pragma endregion

#pragma region Auto register
#define _REFLECTION_CLASS_AUTO_REGISTER()											\
	class _ReflectionInitializerCaller{												\
	public:																			\
		_ReflectionInitializerCaller(){												\
			_InitializeReflection();												\
		}																			\
	};																				\
	inline static _ReflectionInitializerCaller _reflectionInitializerCaller{}
#pragma endregion

#pragma region Initializer components

#pragma region Initializer common head
#define _REFLECTION_CLASS_INITIALIZER_HEAD()										\
	static void _InitializeReflection(){											\
		static bool inited=false;													\
		if(inited){																	\
			return;																	\
		}
#pragma endregion

#define _REFLECTION_CLASS_INITIALIZER_CALL_PARENT(parent) parent::_InitializeReflection()

#pragma region Initializer common tail
#define _REFLECTION_CLASS_INITIALIZER_TAIL(name)									\
		::Engine::ReflectionClass* ptr=::Engine::Reflection::AddClass<name>();		\
		FATAL_ASSERT_CRASH(ptr!=nullptr,"Failed to register class.");				\
		_InitializeCustomReflection(ptr);											\
																					\
		inited=true;																\
	}
#pragma endregion

#define _REFLECTION_CLASS_INITIALIZER_CUSTOM() static void _InitializeCustomReflection(::Engine::ReflectionClass* c)

#pragma endregion

#pragma region Root class register, not calling parent
#define REFLECTION_ROOTCLASS(name)													\
public:																				\
	_REFLECTION_CLASS_NAME_GETTERS(#name,"")										\
																					\
protected:																			\
	_REFLECTION_CLASS_INITIALIZER_HEAD()											\
	_REFLECTION_CLASS_INITIALIZER_TAIL(name)										\
																					\
private:																			\
	_REFLECTION_CLASS_AUTO_REGISTER();												\
	_REFLECTION_CLASS_INITIALIZER_CUSTOM()

#pragma endregion

#pragma region Normal class register, calling parent
// Name and ParentName must be located from the root namespace!
// e.g. Object should be Engine::Object
#define REFLECTION_CLASS(name,parent)												\
public:																				\
	_REFLECTION_CLASS_NAME_GETTERS(#name,#parent)									\
																					\
protected:																			\
	_REFLECTION_CLASS_INITIALIZER_HEAD()											\
	_REFLECTION_CLASS_INITIALIZER_CALL_PARENT(parent);								\
	_REFLECTION_CLASS_INITIALIZER_TAIL(name)										\
																					\
private:																			\
	_REFLECTION_CLASS_AUTO_REGISTER();												\
	_REFLECTION_CLASS_INITIALIZER_CUSTOM()
#pragma endregion

#define REFLECTION_CLASS_INSTANTIABLE(instantiable) c->SetInstantiable(instantiable)

#pragma endregion

namespace Engine{
	class Object;
	class ManualObject;
	class ReferencedObject;

	class Reflection;
	class ReflectionClass;
	class ReflectionMethod;
	class ReflectionProperty;
	class ReflectionSignal;

#pragma region Reflection
	class Reflection final {
	public:
		STATIC_CLASS(Reflection);

		static bool IsClassExists(const String& name);

		static ReflectionClass* GetClass(const String& name);

		template<typename T>
		static ReflectionClass* AddClass() {
			SharedPtr<ReflectionClass> data = SharedPtr<ReflectionClass>::Create();

			data->name = T::GetClassNameStatic();
			data->parentName = T::GetParentClassNameStatic();

			return (GetData().Add(T::GetClassNameStatic(), data) ? data.GetRaw() : nullptr);
		}

	private:
		using ClassData = Dictionary<String, SharedPtr<ReflectionClass>>;
		static ClassData& GetData();
	};

	class ReflectionClass final {
	public:
		String GetName() const;
		String GetParentName() const;

		bool IsInstantiatable() const;
		void SetInstantiable(bool instantiable);

		UniquePtr<Object> Instantiate() const;
	private:
		friend class Reflection;

		String name;
		String parentName;
		bool instantiable = true;
		UniquePtr<Object>(*instantiator)() = nullptr;

		using MethodData = Dictionary<String, SharedPtr<ReflectionMethod>>;
	};

	class ReflectionMethod {
	public:
		virtual ~ReflectionMethod();

		enum class InvokeResult {
			OK,
			ArgumentCountNotMatching,

		};
		virtual InvokeResult Invoke(Object* target, Variant* arguments, int32 argumentCount, Variant& returnValue);

	private:
		friend class ReflectionClass;

		String name;
		Variant::Type returnType;
		int32 argumentCount;
	};
#pragma endregion

#pragma region Object
	// Inherit ManualManagement or AutoManagement instead.
	class Object {
		REFLECTION_ROOTCLASS(::Engine::Object) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		static bool IsInstanceValid(InstanceId id);
		virtual ~Object() = 0;

		// Indicates if current object is a ReferencedObject
		virtual bool IsReferenced() const = 0;

		virtual String ToString() const;

		// Get hash code. Hash code is not used to identify objects!
		// Override this if the object indicates value.
		virtual int32 GetHashCode() const;

		InstanceId GetInstanceId() const;

	protected:
		static Dictionary<InstanceId, bool> objectLookup;
		InstanceId instanceId;
	};

	// Represents a Object which its memory management is done by the user.
	// Raw pointers of ManualObject can be passed around.
	class ManualObject :public Object {
		REFLECTION_CLASS(::Engine::ManualObject,::Engine::Object) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		ManualObject();
		virtual ~ManualObject() = 0;
		bool IsReferenced() const override;
	};

	// Represents a Object which its memory management is done automatically via smart pointers.
	// No raw pointer of ReferenceObject should be passed around!
	class ReferencedObject :public Object {
		REFLECTION_CLASS(::Engine::ReferencedObject, ::Engine::Object) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		ReferencedObject();
		virtual ~ReferencedObject() = 0;

		bool IsReferenced() const override;
		uint32 Reference();
		uint32 Dereference();
		uint32 GetReferenceCount() const;
	private:
		ReferenceCount referenceCount;
	};
#pragma endregion
}