#pragma once

#include "Engine/System/Definition.h"

namespace Engine {
	class Time final {
		// Normal updates

		void SetScale(float scale);
		float GetScale() const;
		float GetUnscaledDelta() const;
		float GetDelta() const;


		// Physics updates

		void SetPhysicsScale(float scale);
		float GetPhysicsScale() const;
		
		void SetPhysicsUnscaledDelta(float unscaledDelta);
		float GetPhysicsUnscaledDelta() const;
		
		float GetPhysicsDelta() const;


		// Summed up

		float GetTotalFrames() const;
		float GetTotal() const;
		float GetUnscaledTotal() const;

	public:
		float scale = 1;
		float unscaledDelta = 0;

		float physicsScale = 1;
		float unscaledPhysicsDelta = 60;
		
		uint32 totalFrames = 0;
		float total = 0;
		float unscaledTotal = 0;


		friend class Engine;
	};
}
