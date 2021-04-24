#pragma once

#include "Engine/System/Definition.h"

namespace Engine {
	class Time final {
	public:
		// Normal updates

		void SetScale(float scale);
		float GetScale() const;
		float GetUnscaledDelta() const;
		float GetDelta() const;


		// Physics updates

		void SetPhysicsScale(float scale);
		float GetPhysicsScale() const;
		
		void SetUnscaledPhysicsDelta(float unscaledDelta);
		float GetUnscaledPhysicsDelta() const;
		
		float GetPhysicsDelta() const;


		// Summed up

		int32 GetTotalFrames() const;

		/// @brief Get total seconds elapsed from the beginning.
		/// @note Use double for total time to avoid catastrophic cancellation.
		/// @see https://randomascii.wordpress.com/2012/02/13/dont-store-that-in-a-float/
		double GetTotal() const;
		double GetUnscaledTotal() const;

	private:
		float scale = 1;
		float unscaledDelta = 0;

		float physicsScale = 1;
		float unscaledPhysicsDelta = 60;
		
		int32 totalFrames = 0;

		double total = 0;
		double unscaledTotal = 0;


		friend class Engine;
	};
}
