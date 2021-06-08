#include "Application/TestNode.h"
#include "Engine/Application/Engine.h"
#include "Engine/System/Math/Math.h"
#include "Engine/Application/Node/NodeTree.h"
#include "Engine/Application/Window.h"
#include "Engine/System/File/FileSystem.h"
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Engine;

namespace Sandbox {
	TestNode::TestNode() {
		Node* node = MEMNEW(Node());
		AddChild(node);
	}
	void TestNode::OnEnteredTree() {
		INFO_MSG(String::Format(u8"{0}: Entered tree.", GetName()).GetRawArray());
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

		transformView = TransformMatrix::LookAt(Vector3(Math::Sin(elapsed)*5, 0, Math::Cos(elapsed)*5), Vector3(0, 0, 0));

		transformProjection = TransformMatrix::Perspective((45+ Math::Sin(elapsed+Math::PI)*10) * Math::Deg2Rad, 640 / 480.0f, 0.1f, 100.0f);

		/*if (elapsed > next) {
			//INFO_MSG(String::Format(u8"{0}: {1} seconds elapsed!.", GetName(),elapsed).GetRawArray());
			next += 1;
			border = !border;

			//ENGINEINST->GetWindowManager()->Get(1)->SetBorder(border);
		}*/
	}
	void TestNode::OnExitingTree() {
		INFO_MSG(String::Format(u8"{0}: Exiting tree.", GetName()).GetRawArray());
	}

	void TestNode::OnKeyDown(int32 keyCode) {
		INFO_MSG(String::Format(u8"Key {0} down!", keyCode).GetRawArray());

		if (keyCode == 38) {
			factor += 0.1;
			if (factor > 1) {
				factor = 1;
			}
		}
		if (keyCode == 40) {
			factor -= 0.1;
			if (factor < 0) {
				factor = 0;
			}
		}
	}

	void TestNode::OnPrepareRender() {
#pragma region Vertex array object
		//Templates that records your following operatings.
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
#pragma endregion

#pragma region Vertex buffer object
		float zoom = 0.f;
		float vertices[] = {
			//Pos                 TexCoord
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion

#pragma region Element buffer object
		unsigned int order[] = {
			0,1,3,	0,2,3,
			4,5,7,	4,6,7,
			
			4,0,2,	4,6,2,
			5,1,3,	5,7,3,

			4,5,1,	4,0,1,
			6,7,3,	6,2,3
		};
		unsigned int ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(order), order, GL_STATIC_DRAW);
#pragma endregion

#pragma region Vertex attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
#pragma endregion

#pragma region Texture
		auto fs = ENGINEINST->GetFileSystem();
		stbi_set_flip_vertically_on_load(true);
		//Container
		int widthContainer;
		int heightContainer;
		int channelsContainer;
		unsigned char* imgContainer = stbi_load("Texture/container.png", &widthContainer, &heightContainer, &channelsContainer, 0);

		glGenTextures(1, &texContainer);
		glBindTexture(GL_TEXTURE_2D, texContainer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthContainer, heightContainer, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgContainer);
		stbi_image_free(imgContainer);

		glGenerateMipmap(GL_TEXTURE_2D);
		//Set wrap mode -- S=X, T=Y, R=Z
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		//Set filter & mipmap mode -- DO NOT USE MIPMAP FOR MAGNIFYING
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		INFO_MSG(u8"Container image loaded!");

		//Face
		int widthFace;
		int heightFace;
		int channelsFace;
		unsigned char* imgFace = stbi_load("Texture/face.png", &widthFace, &heightFace, &channelsFace, 0);

		glGenTextures(1, &texFace);
		glBindTexture(GL_TEXTURE_2D, texFace);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthFace, heightFace, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgFace);
		stbi_image_free(imgFace);

		glGenerateMipmap(GL_TEXTURE_2D);
		//Set wrap mode -- S=X, T=Y, R=Z
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		//Set filter & mipmap mode -- DO NOT USE MIPMAP FOR MAGNIFYING
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		INFO_MSG(u8"Face image loaded!");

#pragma endregion

		shader.Open(STRL("file://Shader/vertex.glsl"), STRL("file://Shader/fragment.glsl"));

		glBindVertexArray(0);

		glUseProgram(shader.GetId());
		glUniform1i(glGetUniformLocation(shader.GetId(), "mTex1"), 0);
		glUniform1i(glGetUniformLocation(shader.GetId(), "mTex2"), 1);
		uniformFactor = glGetUniformLocation(shader.GetId(), "factor");
		uniformTransformModel = glGetUniformLocation(shader.GetId(), "transformModel");
		uniformTransformView = glGetUniformLocation(shader.GetId(), "transformView");
		uniformTransformProjection = glGetUniformLocation(shader.GetId(), "transformProjection");

		glEnable(GL_DEPTH_TEST);
	}
	void TestNode::OnRender() {
		//Clear screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader.GetId());
		glUniform1f(uniformFactor, factor);

		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texContainer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texFace);

		glUniformMatrix4fv(uniformTransformView, 1, GL_FALSE, transformView.GetRaw());
		glUniformMatrix4fv(uniformTransformProjection, 1, GL_FALSE, transformProjection.GetRaw());

		double elapsed = ::Engine::Engine::GetInstance()->GetTime().GetTotal();
		for (int32 i = 0; i < 10; i += 1) {
			transformModel = TransformMatrix();
			transformModel = TransformMatrix::Rotate(Vector3(1.0f, 0.8f, 0.6f), ((float)elapsed * 30 + i * 62.6516f) * Math::Deg2Rad);
			transformModel = TransformMatrix::Translate(positions[i]) * transformModel;
			glUniformMatrix4fv(uniformTransformModel, 1, GL_FALSE, transformModel.GetRaw());
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
	}
	void TestNode::OnCleanupRender() {

	}
}