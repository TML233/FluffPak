#include "Engine/Application/Node/TestNode.h"
#include "Engine/Application/Engine.h"
#include "Engine/System/Math/Math.h"
#include "Engine/Application/Node/NodeTree.h"
#include "Engine/Application/Window.h"
#include "Engine/System/Thread/JobSystem.h"
#include "glad/glad.h"

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

		auto wnd = ::Engine::Engine::GetInstance()->GetWindowManager()->Get(1);

		wnd->ConnectSignal(STRL("KeyDown"), Invokable(this, STRL("OnKeyDown")));
		wnd->ConnectSignal(STRL("PrepareRender"), Invokable(this, STRL("OnPrepareRender")));
		wnd->ConnectSignal(STRL("Render"), Invokable(this, STRL("OnRender")));
		wnd->ConnectSignal(STRL("CleanupRender"), Invokable(this, STRL("OnCleanupRender")));
	}
	void TestNode::OnUpdate(float delta) {
		double elapsed = ::Engine::Engine::GetInstance()->GetTime().GetTotal();
		if (elapsed > next) {
			//INFO_MSG(String::Format(u8"{0}: {1} seconds elapsed!.", GetName(),elapsed).GetRawArray());
			next += 1;
			border = !border;

			//ENGINEINST->GetWindowManager()->Get(1)->SetBorder(border);
		}
	}
	void TestNode::OnExitingTree() {
		INFO_MSG(String::Format(u8"{0}: Exiting tree.", GetName()).GetRawArray());
	}

	void TestNode::OnKeyDown(int32 keyCode) {
		INFO_MSG(String::Format(u8"Key {0} down!", keyCode).GetRawArray());
	}
	void TestNode::OnPrepareRender() {

	}
	void TestNode::OnRender() {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void TestNode::OnCleanupRender() {

	}
}
