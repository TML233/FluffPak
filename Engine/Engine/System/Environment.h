#include "Engine/System/Definition.h"

#if defined(WIN32)
#	define CURRENT_PLATFORM_TYPE_WINDOWS true
#	define CURRENT_PLATFORM_TYPE Windows
#else
#	error The current platform is not supported!
#endif

#undef PLATFORM

namespace Engine {
	enum class PlatformType {
		NotSpecified,
		Windows,
		Linux,
		Android,
		MacOS,
		IOS,
	};
	class Environment final {
	public:
		STATIC_CLASS(Environment);

		static constexpr PlatformType GetPlatformType() {
			return PlatformType::CURRENT_PLATFORM_TYPE;
		}
	};
}