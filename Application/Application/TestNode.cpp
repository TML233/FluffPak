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

		::Engine::Engine::
			GetInstance()->
			GetWindowSystem()->
			GetWindow(0)->
			ConnectSignal(STRL("KeyDown"), Invokable(this, STRL("OnKeyDown")));
	}
	void TestNode::OnUpdate(float delta) {
		double elapsed = ::Engine::Engine::GetInstance()->GetTime().GetTotal();
		if (elapsed > next) {
			INFO_MSG(String::Format(u8"{0}: {1} seconds elapsed!.", GetName(),elapsed).GetRawArray());
			next += 1;
			border = !border;

			//ENGINEINST->GetWindowSystem()->Get(1)->SetBorder(border);
		}
	}
	void TestNode::OnExitingTree() {
		INFO_MSG(String::Format(u8"{0}: Exiting tree.", GetName()).GetRawArray());
	}

	void TestNode::OnKeyDown(int32 keyCode) {
		INFO_MSG(String::Format(u8"Key {0} down!", keyCode).GetRawArray());
	}
}
