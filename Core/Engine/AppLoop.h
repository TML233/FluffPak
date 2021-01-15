#ifndef HEADER_CORE_ENGINE_APPLOOP
#define HEADER_CORE_ENGINE_APPLOOP

#include "Core/Definition.h"
#include "Core/System/UniquePtr.h"
namespace Core {
	class AppLoop :public Object {
	public:
		virtual ~AppLoop();
		virtual Bool IsWorking() const;
		virtual void Update(Float deltatime) = 0;
	private:
		Bool working = true;
	};
}

#endif