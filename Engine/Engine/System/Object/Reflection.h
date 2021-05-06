#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Memory/UniquePtr.h"
#include "Engine/System/Memory/SharedPtr.h"
#include "Engine/Collection/Dictionary.h"
#include "Engine/Collection/List.h"
#include "Engine/System/Object/Variant.h"

#pragma region Macros

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
#define REFLECTION_ROOTCLASS(name)																		\
public:																									\
	static ::Engine::String GetReflectionClassNameStatic(){												\
		return STRING_LITERAL(u8## #name);																\
	}																									\
	virtual ::Engine::String GetReflectionClassName() const{											\
		return STRING_LITERAL(u8## #name);																\
	}																									\
	static ::Engine::String GetReflectionParentClassNameStatic(){										\
		return ::Engine::String::GetEmpty();															\
	}																									\
	virtual ::Engine::String GetReflectionParentClassName() const{										\
		return ::Engine::String::GetEmpty();															\
	}																									\
																										\
protected:																								\
	static void _InitializeReflection(){																\
		static bool inited=false;																		\
		if(inited){																						\
			return;																						\
		}																								\
																										\
		::Engine::ReflectionClass* ptr=::Engine::Reflection::AddClass(									\
			STRING_LITERAL(u8## #name),																	\
			::Engine::String::GetEmpty()																\
		);																								\
		FATAL_ASSERT(ptr!=nullptr,u8"Failed to register class.");										\
		_InitializeCustomReflection(ptr);																\
																										\
		inited=true;																					\
	}																									\
																										\
private:																								\
	_REFLECTION_CLASS_AUTO_REGISTER();																	\
	static void _InitializeCustomReflection(::Engine::ReflectionClass* c)

#pragma endregion

#pragma region Normal class register, calling parent
// Name and ParentName must be located from the root namespace!
// e.g. Object should be Engine::Object
#define REFLECTION_CLASS(name,parent)																	\
public:																									\
	static ::Engine::String GetReflectionClassNameStatic(){												\
		return STRING_LITERAL(u8## #name);																\
	}																									\
	virtual ::Engine::String GetReflectionClassName() const override{									\
		return STRING_LITERAL(u8## #name);																\
	}																									\
	static ::Engine::String GetReflectionParentClassNameStatic(){										\
		return STRING_LITERAL(u8## #parent);															\
	}																									\
	virtual ::Engine::String GetReflectionParentClassName() const override{								\
		return STRING_LITERAL(u8## #parent);															\
	}																									\
																										\
protected:																								\
	static void _InitializeReflection(){																\
		static bool inited=false;																		\
		if(inited){																						\
			return;																						\
		}																								\
																										\
		parent::_InitializeReflection();																\
																										\
		::Engine::ReflectionClass* ptr=::Engine::Reflection::AddClass(									\
			STRING_LITERAL(u8## #name),																	\
			STRING_LITERAL(u8## #parent)																\
		);																								\
		FATAL_ASSERT(ptr!=nullptr,u8"Failed to register class.");										\
		_InitializeCustomReflection(ptr);																\
																										\
		inited=true;																					\
	}																									\
																										\
private:																								\
	_REFLECTION_CLASS_AUTO_REGISTER();																	\
	static void _InitializeCustomReflection(::Engine::ReflectionClass* c)

#pragma endregion

#define REFLECTION_CLASS_INSTANTIABLE(instantiable) c->SetInstantiable(instantiable)
//#define REFLECTION_CLASS_METHOD(...) ::Reflection::RegisterMethod(__VA_ARGS__)
//#define REFLECTION_CLASS_STATIC_METHOD(...) ::Reflection::RegisterStaticMethod(__VA_ARGS__)

#pragma endregion

namespace Engine {
	class Object;
	class ManualObject;
	class ReferencedObject;

	class Reflection;
	class ReflectionClass;
	class ReflectionMethod;
	class ReflectionMethodBind;
	class ReflectionProperty;
	class ReflectionSignal;

	class Variant;


	class Reflection final {
	public:
		STATIC_CLASS(Reflection);

		static bool IsClassExists(const String& name);

		static const ReflectionClass* GetClass(const String& name);

		static ReflectionClass* AddClass(const String& name, const String& parent);

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

	class ReflectionMethodBind {
	public:
		virtual ~ReflectionMethodBind() = default;
		virtual bool IsConst() const = 0;
		virtual bool IsStatic() const = 0;
		virtual Variant::Type GetReturnType() const = 0;
		virtual int32 GetArgumentCount() const = 0;
		virtual Variant::Type GetArgumentType(int32 index) const = 0;

		virtual Variant Invoke(Object* target, const Variant** arguments, int32 argumentCount, const List<Variant>& defaultArguments) const = 0;
	};

	class ReflectionMethod final {
	public:
		enum class InvokeResult {
			OK,
			InvalidObject,
			TooFewArguments,
			TooManyArguments,

		};
		bool IsConst() const;
		bool IsStatic() const;
		Variant::Type GetReturnType() const;
		int32 GetArgumentCount() const;
		
		InvokeResult Invoke(Object* target, const Variant** arguments, int32 argumentCount, Variant& returnValue) const;

	private:
		friend class ReflectionClass;

		String name;
		List<String> argumentNames{};
		List<Variant> defaultArguments{};

		SharedPtr<ReflectionMethodBind> bind;
	};

	template<typename TReturn,typename ... TArgs>
	class ReflectionMethodBindStaticReturn final:public ReflectionMethodBind {
	public:
		ReflectionMethodBindStaticReturn(TReturn(*method)(TArgs...)) :method(method) {}

		bool IsConst() const override {
			return false;
		}
		bool IsStatic() const override {
			return true;
		}
		Variant::Type GetReturnType() const override {
			return Variant::GetTypeFromNative<TReturn>::type;
		}
		int32 GetArgumentCount() const override {
			return (int32)sizeof...(TArgs);
		}
		Variant::Type GetArgumentType(int32 index) const override {
			ERR_ASSERT(index >= 0 && index < GetArgumentCount(), u8"index out of bounds.", return Variant::Type::Null);
			return Variant::Type::Null;
		}
		Variant Invoke(Object* target, const Variant** arguments, int32 argumentCount, const List<Variant>& defaultArguments) const {
			const Variant* args[sizeof...(TArgs) == 0 ? 1 : sizeof...(TArgs)] = { nullptr };
			for (int32 i = 0; i < GetArgumentCount(); i += 1) {
				if (i < argumentCount) {
					args[i] = arguments[i];
				} else {
					args[i] = defaultArguments.begin().GetPointer() + (i - (argumentCount - 1) - (defaultArguments.GetCount() - 1));
				}
			}
			return InternalInvoke(target, arguments, std::make_index_sequence<sizeof...(TArgs)>());
		}
		template<sizeint...Index>
		Variant InternalInvoke(Object* target, const Variant** arguments, std::index_sequence<Index...>) const {
			return (*method)(Variant::CastToNative<TArgs>::Cast(*(arguments[Index]))...);
		}

		TReturn(*method)(TArgs...);
	};
}