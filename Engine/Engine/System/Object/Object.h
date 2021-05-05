#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Atomic.h"
#include "Engine/System/Object/InstanceId.h"
#include "Engine/System/Memory/UniquePtr.h"
#include "Engine/System/Memory/SharedPtr.h"
#include "Engine/System/Object/Variant.h"
#include "Engine/Collection/Dictionary.h"
#include "Engine/Collection/List.h"

#pragma region Reflection macros

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


#pragma region Root class register, not calling parent
#define REFLECTION_ROOTCLASS(name)													\
public:																				\
	static ::Engine::String GetReflectionClassNameStatic(){							\
		return STRING_LITERAL(u8## #name);											\
	}																				\
	virtual ::Engine::String GetReflectionClassName() const{						\
		return STRING_LITERAL(u8## #name);											\
	}																				\
	static ::Engine::String GetReflectionParentClassNameStatic(){					\
		return String::GetEmpty();													\
	}																				\
	virtual ::Engine::String GetReflectionParentClassName() const{					\
		return String::GetEmpty();													\
	}																				\
																					\
protected:																			\
	static void _InitializeReflection(){											\
		static bool inited=false;													\
		if(inited){																	\
			return;																	\
		}																			\
																					\
		::Engine::ReflectionClass* ptr=::Engine::Reflection::AddClass<name>();		\
		FATAL_ASSERT(ptr!=nullptr,u8"Failed to register class.");					\
		_InitializeCustomReflection(ptr);											\
																					\
		inited=true;																\
	}																				\
																					\
private:																			\
	_REFLECTION_CLASS_AUTO_REGISTER();												\
	static void _InitializeCustomReflection(::Engine::ReflectionClass* c)

#pragma endregion

#pragma region Normal class register, calling parent
// Name and ParentName must be located from the root namespace!
// e.g. Object should be Engine::Object
#define REFLECTION_CLASS(name,parent)												\
public:																				\
	static ::Engine::String GetReflectionClassNameStatic(){							\
		return STRING_LITERAL(u8## #name);											\
	}																				\
	virtual ::Engine::String GetReflectionClassName() const override{				\
		return STRING_LITERAL(u8## #name);											\
	}																				\
	static ::Engine::String GetReflectionParentClassNameStatic(){					\
		return STRING_LITERAL(u8## #parent);										\
	}																				\
	virtual ::Engine::String GetReflectionParentClassName() const override{			\
		return STRING_LITERAL(u8## #parent);										\
	}																				\
																					\
protected:																			\
	static void _InitializeReflection(){											\
		static bool inited=false;													\
		if(inited){																	\
			return;																	\
		}																			\
																					\
		parent::_InitializeReflection();											\
																					\
		::Engine::ReflectionClass* ptr=::Engine::Reflection::AddClass<name>();		\
		FATAL_ASSERT(ptr!=nullptr,u8"Failed to register class.");					\
		_InitializeCustomReflection(ptr);											\
																					\
		inited=true;																\
	}																				\
																					\
private:																			\
	_REFLECTION_CLASS_AUTO_REGISTER();												\
	static void _InitializeCustomReflection(::Engine::ReflectionClass* c)

#pragma endregion

#define REFLECTION_CLASS_INSTANTIABLE(instantiable) c->SetInstantiable(instantiable)
//#define REFLECTION_CLASS_METHOD(...) ::Reflection::RegisterMethod(__VA_ARGS__)
//#define REFLECTION_CLASS_STATIC_METHOD(...) ::Reflection::RegisterStaticMethod(__VA_ARGS__)

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

		Variant::Type returnType;
		String name;

		List<Variant::Type> argumentTypes;
		List<String> argumentNames;
		List<Variant> defaultArguments;
	};

	class ReflectionClass final {
	public:
		String GetName() const;
		String GetParentName() const;

		bool IsInstantiatable() const;
		void SetInstantiable(bool instantiable);

		bool IsParentOf(const ReflectionClass* target) const;
		bool IsChildOf(const ReflectionClass* target) const;

		UniquePtr<Object> Instantiate() const;
	private:
		friend class Reflection;

		String name;
		String parentName;
		bool instantiable = true;
		UniquePtr<Object>(*instantiator)() = nullptr;

		using MethodData = Dictionary<String, SharedPtr<ReflectionMethod>>;
	};

	class Reflection final {
	public:
		STATIC_CLASS(Reflection);

		static bool IsClassExists(const String& name);

		static const ReflectionClass* GetClass(const String& name);

		template<typename T>
		static ReflectionClass* AddClass() {
			SharedPtr<ReflectionClass> data = SharedPtr<ReflectionClass>::Create();

			data->name = T::GetReflectionClassNameStatic();
			data->parentName = T::GetReflectionParentClassNameStatic();

			return (GetData().Add(T::GetReflectionClassNameStatic(), data) ? data.GetRaw() : nullptr);
		}

	private:
		using ClassData = Dictionary<String, SharedPtr<ReflectionClass>>;
		static ClassData& GetData();
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
	class ReferencedObject :public Object {
		REFLECTION_CLASS(::Engine::ReferencedObject, ::Engine::Object) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		ReferencedObject();
		virtual ~ReferencedObject() = 0;

		bool IsReferenced() const override;
		uint32 Reference() const;
		uint32 Dereference() const;
		uint32 GetReferenceCount() const;
	private:
		mutable ReferenceCount referenceCount;
	};
#pragma endregion
}