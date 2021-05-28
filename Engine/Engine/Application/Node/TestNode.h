#pragma once
#include "Engine/Application/Node/Node.h"

namespace Engine {
	class TestNode :public Node {
		REFLECTION_CLASS(::Engine::TestNode, ::Engine::Node) {
			REFLECTION_METHOD(STRL("OnKeyDown"), TestNode::OnKeyDown, { STRL("keyCode") }, {});
			REFLECTION_METHOD(STRL("OnPrepareRender"), TestNode::OnPrepareRender, {}, {});
			REFLECTION_METHOD(STRL("OnRender"), TestNode::OnRender, {}, {});
			REFLECTION_METHOD(STRL("OnCleanupRender"), TestNode::OnCleanupRender, {}, {});
		}

	public:
		TestNode();
		void OnEnteredTree() override;
		void OnReady() override;
		void OnUpdate(float delta) override;
		//void OnPhysicsUpdate(float delta) override;
		void OnExitingTree() override;

		void OnKeyDown(int32 keyCode);
		void OnPrepareRender();
		void OnRender();
		void OnCleanupRender();
	private:
		double next = 1;
		bool border = true;
	};
}
