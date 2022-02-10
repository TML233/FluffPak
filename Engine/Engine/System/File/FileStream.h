#pragma once
#include "Engine/System/Stream.h"

namespace Engine {
	template<typename T>
	class List;

	class FileStream :public Stream {
		REFLECTION_CLASS(::Engine::FileStream, ::Engine::Stream) {
			REFLECTION_CLASS_INSTANTIABLE(false);
		}

	public:
		bool CanRandomAccess() const override;
	};

}