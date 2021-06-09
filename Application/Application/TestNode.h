#pragma once
#include "Engine/Application/Node/Node.h"

namespace Application {
	class TestNode :public ::Engine::Node {
		REFLECTION_CLASS(::Engine::TestNode, ::Engine::Node) {
			REFLECTION_METHOD(STRL("OnKeyDown"), TestNode::OnKeyDown, { STRL("keyCode") }, {});
		}

	public:
		TestNode();
		void OnEnteredTree() override;
		void OnReady() override;
		void OnUpdate(float delta) override;
		//void OnPhysicsUpdate(float delta) override;
		void OnExitingTree() override;

		void OnKeyDown(::Engine::int32 keyCode);
	private:
		double next = 1;
		bool border = true;
	};
}
