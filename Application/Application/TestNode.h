#pragma once
#include "Engine/Application/Node/Node.h"
#include "GLShader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Sandbox {
	class TestNode :public ::Engine::Node {
		REFLECTION_CLASS(::Sandbox::TestNode, ::Engine::Node) {
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

		void OnKeyDown(::Engine::int32 keyCode);
		void OnPrepareRender();
		void OnRender();
		void OnCleanupRender();

	private:
		double next = 1;
		bool border = true;
		GLShader shader;
		::Engine::int32 facLocation = 0;
		::Engine::int32 transformLocation = 0;
		float factor = 0;
		::Engine::uint32 vao = 0;
		::Engine::uint32 texContainer = 0;
		::Engine::uint32 texFace = 0;
		glm::mat4 transform;
		glm::mat4 transform2;
	};
}
