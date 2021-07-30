#pragma once
#include "Engine/System/Object/Object.h"

namespace Engine {
	class Resource:public ReferencedObject{
		REFLECTION_CLASS(::Engine::Resource, ::Engine::ReferencedObject) {

		}
	public:
		virtual ~Resource() = default;
	private:

	};

	class ResourceFileHandler:public ManualObject {
		REFLECTION_CLASS(::Engine::ResourceFileHandler, ::Engine::ManualObject) {
			REFLECTION_CLASS_INSTANTIABLE(false);
			//REFLECTION_METHOD("Load", ResourceFileHandler::Load, ARGLIST(STRL("path"), STRL("resource")), {});
			REFLECTION_METHOD(STRL("Save"), ResourceFileHandler::Save, ARGLIST(STRL("path"),STRL("resource")), {});
		}
	public:
		virtual ~ResourceFileHandler() = default;
		bool CanHandle(const String& extension) const;
		int32 GetSupportedExtensions(List<String>& result) const;

		virtual ResultCode Load(const String& path, IntrusivePtr<Resource>& result) const = 0;
		virtual ResultCode Save(const String& path, const IntrusivePtr<Resource>& resource) = 0;

	protected:
		List<String> extensions{};
	};

	class ResourceSystem {
	public:
		ResultCode Load(const String& path, IntrusivePtr<Resource>& result) const;
		ResultCode Save(const String& path, const IntrusivePtr<Resource>& resource);

	private:
		Dictionary<String, SharedPtr<ResourceFileHandler>> fileHandlers{ 10 };
		//using DefaultFileHandlerType = ResourceFileHandlerRouter;
		UniquePtr<ResourceFileHandler> defaultFileHandler;

		//Dictionary<String, SharedPtr<ResourceTypeHandler>> typeHandlers{ 50 };
	};
}