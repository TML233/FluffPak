#include "TestNode.h"
#include "Engine/Application/Engine.h"
#include "Engine/System/Math/Math.h"
#include "Engine/Application/Node/NodeTree.h"
#include "Engine/Application/Window.h"
#include "Engine/System/Thread/JobSystem.h"

using namespace Engine;
namespace Application {
	TestNode::TestNode() {
		Node* node = MEMNEW(Node());
		AddChild(node);
	}
	void TestNode::OnEnteredTree() {
		INFO_MSG(String::Format(u8"{0}: Entered tree.",GetName()).GetRawArray());
	}
	void TestNode::OnReady() {
		INFO_MSG(String::Format(u8"{0}: Ready.", GetName()).GetRawArray());

		ENGINEINST->GetWindowSystem()->GetWindow(0)->
			ConnectSignal(STRL("KeyDown"), Invokable(this, STRL("OnKeyDown")));
		ENGINEINST->SetFpsUpdateFrequency(0.5f);
	}
	void TestNode::OnUpdate(float delta) {
		double elapsed = ENGINEINST->GetTime().GetTotal();
		if (elapsed > next) {
			//INFO_MSG(String::Format(u8"{0}: {1} seconds elapsed!.", GetName(),elapsed).GetRawArray());
			next += ENGINEINST->GetFpsUpdateFrequency();
			border = !border;

			ENGINEINST->GetWindowSystem()->GetWindow(0)->SetTitle(String::Format(
				STRING_LITERAL("Rabbik Engine - FPS: {0}/{1}"),
				ENGINEINST->GetFps(), ENGINEINST->GetTargetFps()
			));
		}
	}
	void TestNode::OnExitingTree() {
		INFO_MSG(String::Format(u8"{0}: Exiting tree.", GetName()).GetRawArray());
	}

	void TestNode::OnKeyDown(int32 keyCode) {
		INFO_MSG(String::Format(u8"Key {0} down!", keyCode).GetRawArray());
	}
}
