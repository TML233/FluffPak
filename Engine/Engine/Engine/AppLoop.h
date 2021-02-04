#ifndef HEADER_CORE_ENGINE_APPLOOP
#define HEADER_CORE_ENGINE_APPLOOP

#include "Engine/System/Definition.h"
#include "Engine/System/UniquePtr.h"
namespace Engine {
	class AppLoop :public Object {
	public:
		virtual ~AppLoop();
		virtual bool IsWorking() const;
		virtual void Update(float deltatime) = 0;
	private:
		bool working = true;
	};
}

#endif