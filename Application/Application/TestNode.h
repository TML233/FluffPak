#pragma once
#include "Engine/Application/Node/Node.h"
#include "GLShader.h"
#include "Engine/System/Math/TransformMatrix.h"

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
		::Engine::int32 uniformFactor = 0;
		::Engine::int32 uniformTransformModel = 0;
		::Engine::int32 uniformTransformView = 0;
		::Engine::int32 uniformTransformProjection = 0;
		float factor = 0;
		::Engine::uint32 vao = 0;
		::Engine::uint32 texContainer = 0;
		::Engine::uint32 texFace = 0;

		::Engine::TransformMatrix transformModel;
		::Engine::TransformMatrix transformView;
		::Engine::TransformMatrix transformProjection;

		::Engine::Vector3 positions[10] = {
			::Engine::Vector3(0.0f,  0.0f,  0.0f),
			::Engine::Vector3(2.0f,  5.0f, -15.0f),
			::Engine::Vector3(-1.5f, -2.2f, -2.5f),
			::Engine::Vector3(-3.8f, -2.0f, -12.3f),
			::Engine::Vector3(2.4f, -0.4f, -3.5f),
			::Engine::Vector3(-1.7f,  3.0f, -7.5f),
			::Engine::Vector3(1.3f, -2.0f, -2.5f),
			::Engine::Vector3(1.5f,  2.0f, -2.5f),
			::Engine::Vector3(1.5f,  0.2f, -1.5f),
			::Engine::Vector3(-1.3f,  1.0f, -1.5f)
		};
	};
}
