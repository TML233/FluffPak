#include "Engine/Application/Node/TestNode.h"
#include "Engine/Application/Engine.h"
#include "Engine/System/Math/Math.h"
#include "Engine/Application/Node/NodeTree.h"
#include "Engine/Application/NativeWindow.h"
#include "Engine/System/Thread/JobSystem.h"

namespace Engine {
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
			GetNativeWindowManager()->
			Get(1)->
			ConnectSignal(STRL("KeyDown"), Invokable(this, STRL("OnKeyDown")));
	}
	void TestNode::OnUpdate(float delta) {
		double elapsed = ::Engine::Engine::GetInstance()->GetTime().GetTotal();
		if (elapsed > next) {
			INFO_MSG(String::Format(u8"{0}: {1} seconds elapsed!.", GetName(),elapsed).GetRawArray());
			next += 1;
		}
	}
	void TestNode::OnExitingTree() {
		INFO_MSG(String::Format(u8"{0}: Exiting tree.", GetName()).GetRawArray());
	}

	void TestNode::OnKeyDown(int32 keyCode) {
		INFO_MSG(String::Format(u8"Key {0} down!", keyCode).GetRawArray());
		if (keyCode == 65) {
			for (int32 i = 0; i < 100; i += 1) {
				::Engine::Engine::GetInstance()->GetJobSystem()->AddJob([](auto job, auto data) {INFO_MSG(u8"Job ran!"); });
			}
		}
	}
}
