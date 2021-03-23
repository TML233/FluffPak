#include "Engine/Application/Time.h"

namespace Engine {
	void Time::SetScale(float scale) {
		this->scale = scale;
	}
	float Time::GetScale() const {
		return scale;
	}
	float Time::GetUnscaledDelta() const {
		return unscaledDelta;
	}
	float Time::GetDelta() const {
		return unscaledDelta * scale;
	}

}
