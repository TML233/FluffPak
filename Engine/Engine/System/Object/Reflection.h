#pragma once

#include "Engine/System/Definition.h"
#include "Engine/System/String.h"
#include "Engine/System/Memory/UniquePtr.h"
#include "Engine/System/Memory/SharedPtr.h"
#include "Engine/System/Collection/Dictionary.h"
#include "Engine/System/Collection/List.h"
#include "Engine/System/Object/Variant.h"
#include "Engine/System/Object/InstanceId.h"

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
		return STRING_LITERAL(#name);																	\
	}																									\
	virtual ::Engine::String GetReflectionClassName() const{											\
		return STRING_LITERAL(#name);																	\
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
			STRING_LITERAL(#name),																		\
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
		return STRING_LITERAL(#name);																	\
	}																									\
	virtual ::Engine::String GetReflectionClassName() const override{									\
		return STRING_LITERAL(#name);																	\
	}																									\
	static ::Engine::String GetReflectionParentClassNameStatic(){										\
		return STRING_LITERAL(#parent);																	\
	}																									\
	virtual ::Engine::String GetReflectionParentClassName() const override{								\
		return STRING_LITERAL(#parent);																	\
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
			STRING_LITERAL(#name),																		\
			STRING_LITERAL(#parent)																		\
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

#define ARGLIST(...) {__VA_ARGS__}

#define REFLECTION_METHOD(name,func,argNames,defaultArgs)							\
c->AddMethod(::Engine::SharedPtr<::Engine::ReflectionMethod>::Create(				\
	name,																			\
	::Engine::ReflectionMethodBindHelper::Create(&func),							\
	std::initializer_list<::Engine::String> argNames,								\
	std::initializer_list<::Engine::Variant> defaultArgs							\
))

#define REFLECTION_STATIC_METHOD(name,func,argNames,defaultArgs)					\
c->AddMethod(::Engine::SharedPtr<::Engine::ReflectionMethod>::Create(				\
	name,																			\
	::Engine::ReflectionMethodBindHelper::Create(func),								\
	std::initializer_list<::Engine::String> argNames,								\
	std::initializer_list<::Engine::Variant> defaultArgs							\
))

#define REFLECTION_PROPERTY(name,getterName,setterName)								\
c->AddProperty(::Engine::SharedPtr<::Engine::ReflectionProperty>::Create(			\
	name,c->GetMethodOrNull(getterName),c->GetMethodOrNull(setterName)				\
))

#define REFLECTION_PROPERTY_HINT(name,getterName,setterName,hint,hintText)			\
c->AddProperty(::Engine::SharedPtr<::Engine::ReflectionProperty>::Create(			\
	name,c->GetMethodOrNull(getterName),c->GetMethodOrNull(setterName),hint,hintText\
))

#define SIGARG(name,type) ::Engine::ReflectionSignal::ArgumentInfo(name,type)
#define SIGARGD(name,type,detailedType) ::Engine::ReflectionSignal::ArgumentInfo(name,type,detailedType)

#define REFLECTION_SIGNAL(name,arguments)											\
c->AddSignal(::Engine::SharedPtr<::Engine::ReflectionSignal>::Create(				\
	name,																			\
	std::initializer_list<::Engine::ReflectionSignal::ArgumentInfo> arguments		\
))

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

		static ReflectionClass* AddClass(const String& name, const String& parent);
		static bool IsClassExists(const String& name);
		static bool TryGetClass(const String& name, ReflectionClass*& result);
		static bool TryGetClass(const String& name, const ReflectionClass*& result);

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

		bool HasMethod(const String& name) const;
		bool HasMethodInTree(const String& name) const;
		bool TryGetMethod(const String& name, ReflectionMethod*& result) const;
		ReflectionMethod* GetMethodOrNull(const String& name) const;
		bool TryGetMethodInTree(const String& name, ReflectionMethod*& result) const;
		ReflectionMethod* AddMethod(SharedPtr<ReflectionMethod> method);
		bool RemoveMethod(const String& name);

		bool HasProperty(const String& name) const;
		bool HasPropertyInTree(const String& name) const;
		bool TryGetProperty(const String& name, ReflectionProperty*& result) const;
		bool TryGetPropertyInTree(const String& name, ReflectionProperty*& result) const;
		ReflectionProperty* AddProperty(SharedPtr<ReflectionProperty> prop);
		bool RemoveProperty(const String& name);

		bool HasSignal(const String& name) const;
		bool HasSignalInTree(const String& name) const;
		bool TryGetSignal(const String& name, ReflectionSignal*& result) const;
		bool TryGetSignalInTree(const String& name, ReflectionSignal*& result) const;
		ReflectionSignal* AddSignal(SharedPtr<ReflectionSignal> signal);
		bool RemoveSignal(const String& name);
	private:
		friend class Reflection;

		String name;
		String parentName;
		bool instantiable = true;

		using MethodData = Dictionary<String, SharedPtr<ReflectionMethod>>;
		MethodData methods{};

		using PropertyData = Dictionary<String, SharedPtr<ReflectionProperty>>;
		PropertyData properties{};

		using SignalData = Dictionary<String, SharedPtr<ReflectionSignal>>;
		SignalData signals{};
	};

	class ReflectionMethod final {
	public:
		ReflectionMethod(const String& name, SharedPtr<ReflectionMethodBind> bind);
		ReflectionMethod(
			const String& name, SharedPtr<ReflectionMethodBind> bind,
			std::initializer_list<String> argumentNames, std::initializer_list<Variant> defaultArguments
		);

		String GetName() const;
		bool IsConst() const;
		bool IsStatic() const;
		Variant::Type GetReturnType() const;
		int32 GetArgumentCount() const;

		void SetBind(SharedPtr<ReflectionMethodBind> bind);
		SharedPtr<ReflectionMethodBind> GetBind() const;

		List<String>& GetArgumentNameList();
		List<Variant>& GetDefaultArgumentList();

		ResultCode Invoke(Object* target, const Variant** arguments, int32 argumentCount, Variant& returnValue) const;

	private:
		friend class ReflectionClass;

		String name;
		List<String> argumentNames;
		List<Variant> defaultArguments;

		SharedPtr<ReflectionMethodBind> bind;
	};

#pragma region MethodBind
	class ReflectionMethodBind {
	public:
		virtual ~ReflectionMethodBind() = default;
		virtual bool IsConst() const = 0;
		virtual bool IsStatic() const = 0;
		virtual Variant::Type GetReturnType() const = 0;
		virtual int32 GetArgumentCount() const = 0;
		//virtual Variant::Type GetArgumentType(int32 index) const = 0;

		virtual ResultCode Invoke(Object* target, const Variant** arguments, int32 argumentCount, const List<Variant>& defaultArguments,Variant& result) const = 0;
	};

	/// @internal

	// Static, return a value.
	template<typename TReturn,typename ... TArgs>
	class _ReflectionMethodBindStaticReturn final:public ReflectionMethodBind {
	public:
		_ReflectionMethodBindStaticReturn(TReturn(*method)(TArgs...)) :method(method) {}

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

		template<sizeint...Index>
		ResultCode InternalInvoke(Object* target, const Variant** arguments, Variant& result, std::index_sequence<Index...>) const {
			result = (*method)(Variant::CastToNative<TArgs>::Cast(*(arguments[Index]))...);
			return ResultCode::OK;
		}
		ResultCode Invoke(Object* target, const Variant** arguments, int32 argumentCount, const List<Variant>& defaultArguments,Variant& result) const override {
			const Variant* args[sizeof...(TArgs) == 0 ? 1 : sizeof...(TArgs)] = { nullptr };
			for (int32 i = 0; i < GetArgumentCount(); i += 1) {
				if (i < argumentCount) {
					args[i] = arguments[i];
				} else {
					args[i] = defaultArguments.begin().GetPointer() + (i - argumentCount);
				}
			}
			return InternalInvoke(target, args, result,std::make_index_sequence<sizeof...(TArgs)>());
		}

		TReturn(*method)(TArgs...);
	};

	// Static, No return.
	template<typename ... TArgs>
	class _ReflectionMethodBindStaticVoid final :public ReflectionMethodBind {
	public:
		_ReflectionMethodBindStaticVoid(void (*method)(TArgs...)) :method(method) {}

		bool IsConst() const override {
			return false;
		}
		bool IsStatic() const override {
			return true;
		}
		Variant::Type GetReturnType() const override {
			return Variant::Type::Null;
		}
		int32 GetArgumentCount() const override {
			return (int32)sizeof...(TArgs);
		}

		template<sizeint...Index>
		ResultCode InternalInvoke(Object* target, const Variant** arguments, Variant& result,std::index_sequence<Index...>) const {
			(*method)(Variant::CastToNative<TArgs>::Cast(*(arguments[Index]))...);
			result = Variant();
			return ResultCode::OK;
		}
		ResultCode Invoke(Object* target, const Variant** arguments, int32 argumentCount, const List<Variant>& defaultArguments,Variant& result) const override {
			const Variant* args[sizeof...(TArgs) == 0 ? 1 : sizeof...(TArgs)] = { nullptr };
			for (int32 i = 0; i < GetArgumentCount(); i += 1) {
				if (i < argumentCount) {
					args[i] = arguments[i];
				} else {
					args[i] = defaultArguments.begin().GetPointer() + (i - argumentCount);
				}
			}
			return InternalInvoke(target, args, result, std::make_index_sequence<sizeof...(TArgs)>());
		}

		void (*method)(TArgs...);
	};

	// Object, return a value.
	template<typename TClass,typename TReturn, typename ... TArgs>
	class _ReflectionMethodBindReturn final :public ReflectionMethodBind {
	public:
		_ReflectionMethodBindReturn(TReturn(TClass::*method)(TArgs...)) :method(method) {}

		bool IsConst() const override {
			return false;
		}
		bool IsStatic() const override {
			return false;
		}
		Variant::Type GetReturnType() const override {
			return Variant::GetTypeFromNative<TReturn>::type;
		}
		int32 GetArgumentCount() const override {
			return (int32)sizeof...(TArgs);
		}

		template<sizeint...Index>
		ResultCode InternalInvoke(Object* target, const Variant** arguments, Variant& result, std::index_sequence<Index...>) const {
			result = (((TClass*)target)->*method)(Variant::CastToNative<TArgs>::Cast(*(arguments[Index]))...);
			return ResultCode::OK;
		}
		ResultCode Invoke(Object* target, const Variant** arguments, int32 argumentCount, const List<Variant>& defaultArguments, Variant& result) const override {
			const Variant* args[sizeof...(TArgs) == 0 ? 1 : sizeof...(TArgs)] = { nullptr };
			for (int32 i = 0; i < GetArgumentCount(); i += 1) {
				if (i < argumentCount) {
					args[i] = arguments[i];
				} else {
					args[i] = defaultArguments.begin().GetPointer() + (i - argumentCount);
				}
			}
			return InternalInvoke(target, args, result, std::make_index_sequence<sizeof...(TArgs)>());
		}

		TReturn(TClass::*method)(TArgs...);
	};

	// Object, const, return a value.
	template<typename TClass, typename TReturn, typename ... TArgs>
	class _ReflectionMethodBindReturnConst final :public ReflectionMethodBind {
	public:
		_ReflectionMethodBindReturnConst(TReturn(TClass::*method)(TArgs...) const) :method(method) {}

		bool IsConst() const override {
			return true;
		}
		bool IsStatic() const override {
			return false;
		}
		Variant::Type GetReturnType() const override {
			return Variant::GetTypeFromNative<TReturn>::type;
		}
		int32 GetArgumentCount() const override {
			return (int32)sizeof...(TArgs);
		}

		template<sizeint...Index>
		ResultCode InternalInvoke(Object* target, const Variant** arguments, Variant& result, std::index_sequence<Index...>) const {
			result = (((TClass*)target)->*method)(Variant::CastToNative<TArgs>::Cast(*(arguments[Index]))...);
			return ResultCode::OK;
		}
		ResultCode Invoke(Object* target, const Variant** arguments, int32 argumentCount, const List<Variant>& defaultArguments, Variant& result) const override {
			const Variant* args[sizeof...(TArgs) == 0 ? 1 : sizeof...(TArgs)] = { nullptr };
			for (int32 i = 0; i < GetArgumentCount(); i += 1) {
				if (i < argumentCount) {
					args[i] = arguments[i];
				} else {
					args[i] = defaultArguments.begin().GetPointer() + (i - argumentCount);
				}
			}
			return InternalInvoke(target, args, result, std::make_index_sequence<sizeof...(TArgs)>());
		}

		TReturn(TClass::* method)(TArgs...) const;
	};

	// Object, no return.
	template<typename TClass, typename ... TArgs>
	class _ReflectionMethodBindVoid final :public ReflectionMethodBind {
	public:
		_ReflectionMethodBindVoid(void(TClass::* method)(TArgs...)) :method(method) {}

		bool IsConst() const override {
			return false;
		}
		bool IsStatic() const override {
			return false;
		}
		Variant::Type GetReturnType() const override {
			return Variant::Type::Null;
		}
		int32 GetArgumentCount() const override {
			return (int32)sizeof...(TArgs);
		}

		template<sizeint...Index>
		ResultCode InternalInvoke(Object* target, const Variant** arguments, Variant& result, std::index_sequence<Index...>) const {
			(((TClass*)target)->*method)(Variant::CastToNative<TArgs>::Cast(*(arguments[Index]))...);
			result = Variant();
			return ResultCode::OK;
		}
		ResultCode Invoke(Object* target, const Variant** arguments, int32 argumentCount, const List<Variant>& defaultArguments, Variant& result) const override {
			const Variant* args[sizeof...(TArgs) == 0 ? 1 : sizeof...(TArgs)] = { nullptr };
			for (int32 i = 0; i < GetArgumentCount(); i += 1) {
				if (i < argumentCount) {
					args[i] = arguments[i];
				} else {
					args[i] = defaultArguments.begin().GetPointer() + (i - argumentCount);
				}
			}
			return InternalInvoke(target, args, result, std::make_index_sequence<sizeof...(TArgs)>());
		}

		void(TClass::* method)(TArgs...);
	};

	// Object, const, no return.
	template<typename TClass, typename ... TArgs>
	class _ReflectionMethodBindVoidConst final :public ReflectionMethodBind {
	public:
		_ReflectionMethodBindVoidConst(void(TClass::* method)(TArgs...) const) :method(method) {}

		bool IsConst() const override {
			return true;
		}
		bool IsStatic() const override {
			return false;
		}
		Variant::Type GetReturnType() const override {
			return Variant::Type::Null;
		}
		int32 GetArgumentCount() const override {
			return (int32)sizeof...(TArgs);
		}

		template<sizeint...Index>
		ResultCode InternalInvoke(Object* target, const Variant** arguments, Variant& result, std::index_sequence<Index...>) const {
			(((TClass*)target)->*method)(Variant::CastToNative<TArgs>::Cast(*(arguments[Index]))...);
			result = Variant();
			return ResultCode::OK;
		}
		ResultCode Invoke(Object* target, const Variant** arguments, int32 argumentCount, const List<Variant>& defaultArguments, Variant& result) const override {
			const Variant* args[sizeof...(TArgs) == 0 ? 1 : sizeof...(TArgs)] = { nullptr };
			for (int32 i = 0; i < GetArgumentCount(); i += 1) {
				if (i < argumentCount) {
					args[i] = arguments[i];
				} else {
					args[i] = defaultArguments.begin().GetPointer() + (i - argumentCount);
				}
			}
			return InternalInvoke(target, args, result, std::make_index_sequence<sizeof...(TArgs)>());
		}

		void(TClass::* method)(TArgs...) const;
	};

	/// @endinternal

	class ReflectionMethodBindHelper final{
	public:
		// Static, no return.
		template<typename ... TArgs>
		static SharedPtr<ReflectionMethodBind> Create(void (*method)(TArgs...)) {
			return SharedPtr<_ReflectionMethodBindStaticVoid<TArgs...>>::Create(method);
		}
		// Static, return a value.
		template<typename TReturn, typename ... TArgs>
		static SharedPtr<ReflectionMethodBind> Create(TReturn(*method)(TArgs...)) {
			return SharedPtr<_ReflectionMethodBindStaticReturn<TReturn, TArgs...>>::Create(method);
		}

		// Object, no return.
		template<typename TClass, typename ... TArgs>
		static SharedPtr<ReflectionMethodBind> Create(void(TClass::* method)(TArgs...)) {
			return SharedPtr<_ReflectionMethodBindVoid<TClass, TArgs...>>::Create(method);
		}
		// Object, return a value.
		template<typename TClass, typename TReturn, typename ... TArgs>
		static SharedPtr<ReflectionMethodBind> Create(TReturn(TClass::* method)(TArgs...)) {
			return SharedPtr<_ReflectionMethodBindReturn<TClass, TReturn, TArgs...>>::Create(method);
		}
		// Object, const, no return.
		template<typename TClass, typename ... TArgs>
		static SharedPtr<ReflectionMethodBind> Create(void(TClass::* method)(TArgs...) const) {
			return SharedPtr<_ReflectionMethodBindVoidConst<TClass, TArgs...>>::Create(method);
		}
		// Object, const, return a value;
		template<typename TClass, typename TReturn, typename ... TArgs>
		static SharedPtr<ReflectionMethodBind> Create(TReturn(TClass::* method)(TArgs...) const) {
			return SharedPtr<_ReflectionMethodBindReturnConst<TClass, TReturn, TArgs...>>::Create(method);
		}
	};
#pragma endregion

	class ReflectionProperty final {
	public:
		/// @brief Describes the style of this property shown in the editor.
		enum class Hint:byte {
			/// @brief Default.
			Null,
			/// @brief Limit the number value between min and max and. Also shows a slider.\n
			/// HintText format: "Min,Max,Step"
			NumberRange,
			/// @brief Show a dropdown list for enum.\n
			/// HintText format: "EnumName0,EnumName1,EnumName2..."
			NumberEnum,
			/// @brief Show a group of checkboxes for flags.\n
			/// HintText format: "FlagName0,FlagName1,FlagName2..."
			NumberFlag,
			/// @brief Show a big input box for strings.
			StringMultiline,
			/// @brief Show a file picker for file path strings.\n
			/// HintText format: "Protocol|*.filter0,*.filter1..."
			StringFile,
			/// @brief Show a directory picker for directory path strings.\n
			/// HintText format: "Protocol"
			StringDirectory,
			/// @brief Show a resource box for creating internal resource or picking existsing resource.\n
			/// HintText format: "ResourceClassName"
			ObjectResource,
			/// @brief End of the enum. Do not use.
			End
		};

		ReflectionProperty(
			const String& name,
			ReflectionMethod* getter, ReflectionMethod* setter,
			Hint hint = Hint::Null, const String& hintText = String::GetEmpty()
		);

		String GetName() const;
		Variant::Type GetType() const;

		bool CanGet() const;
		bool CanSet() const;
		ResultCode Get(const Object* obj, Variant& result) const;
		ResultCode Set(Object* obj, const Variant& value);

		ReflectionMethod* GetGetter() const;
		void SetGetter(ReflectionMethod* method);
		ReflectionMethod* GetSetter() const;
		void SetSetter(ReflectionMethod* method);

		Hint GetHint() const;
		void SetHint(Hint hint);
		String GetHintText() const;
		void SetHintText(const String& hintText);

	private:
		String name;
		Hint hint;
		String hintText;
		ReflectionMethod* getter;
		ReflectionMethod* setter;
	};

	class ReflectionSignal final {
	public:
		struct ArgumentInfo final {
			ArgumentInfo();
			ArgumentInfo(const String& name, Variant::Type type, const String& detailedClass = String::GetEmpty());

			String name;
			Variant::Type type;
			String detailedClass;
		};
		enum class ConnectFlag {
			Null=0b0000,
			Deferred = 0b0001,
			Once = 0b0010
		};

		String GetName() const;
		int32 GetArgumentCount() const;
		ArgumentInfo GetArgument(int32 index) const;

		ReflectionSignal(String name, std::initializer_list<ArgumentInfo> arguments);

	private:
		String name;
		List<ArgumentInfo> arguments;
	};
}