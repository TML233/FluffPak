#include "Engine/Application/Node/TestNode.h"
#include "Engine/Application/Engine.h"
#include "Engine/Math/Math.h"

namespace Engine {
	TestNode::TestNode(int32 id) {
		this->id = id;
		if (this->id < 2) {
			TestNode* node = MEMNEW(TestNode(this->id + 1));
			AddChild(node);
			INFO_MSG(String::Format("TestNode {0}: Added {1} as child.", this->id, node->id).GetRawArray());
		}
	}
	void TestNode::OnEnteredTree() {
		INFO_MSG(String::Format("TestNode {0}: Entered tree.",id).GetRawArray());
	}
	void TestNode::OnReady() {
		INFO_MSG(String::Format("TestNode {0}: Ready.", id).GetRawArray());

		lastTime = ::Engine::Engine::GetInstance()->GetTime().GetTotal();

		if (id == 0) {
			INFO_MSG(GetTreeStructureFormated().GetRawArray());
		}
	}
	void TestNode::OnUpdate(float delta) {
		double now = ::Engine::Engine::GetInstance()->GetTime().GetTotal();

		int elaspedSeconds = Math::Floor(now) - Math::Floor(lastTime);
		if (elaspedSeconds >= 1) {
			//INFO_MSG(String::Format("TestNode {0}: {1} seconds elapsed from last output!", id,elaspedSeconds).GetRawArray());
		}

		lastTime = now;
	}
	void TestNode::OnExitingTree() {
		INFO_MSG(String::Format("TestNode {0}: Exiting tree.", id).GetRawArray());
	}

}
