#pragma once
#include "Engine/Application/Node/Node.h"

namespace Engine {
	class TestNode :public Node {
		REFLECTION_CLASS(TestNode, Node) {}

	public:
		TestNode(int32 id = 0);
		void OnEnteredTree() override;
		void OnReady() override;
		void OnUpdate(float delta) override;
		//void OnPhysicsUpdate(float delta) override;
		void OnExitingTree() override;

		int id = 0;
	private:
		double lastTime = 0;
	};
}
