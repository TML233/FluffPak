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

	void Time::SetPhysicsScale(float scale) {
		this->physicsScale = scale;
	}
	float Time::GetPhysicsScale() const {
		return physicsScale;
	}
	float Time::GetUnscaledPhysicsDelta() const {
		return unscaledPhysicsDelta;
	}
	void Time::SetUnscaledPhysicsDelta(float unscaledDelta) {
		unscaledPhysicsDelta = unscaledDelta;
	}
	float Time::GetPhysicsDelta() const {
		return unscaledPhysicsDelta * physicsScale;
	}

	int32 Time::GetTotalFrames() const {
		return totalFrames;
	}
	double Time::GetTotal() const {
		return total;
	}
	double Time::GetUnscaledTotal() const {
		return unscaledTotal;
	}
}
