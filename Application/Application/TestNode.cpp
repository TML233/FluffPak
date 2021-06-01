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
		transform = glm::mat4(1);
		transform = glm::translate(transform, glm::vec3(0.4f, -0.4f, 0));
		transform = glm::rotate(transform, (float)elapsed, glm::vec3(0, 0, 1));

		transform2 = glm::mat4(1);
		transform2 = glm::translate(transform2, glm::vec3(-0.4f, 0.4f, 0));
		transform2 = glm::scale(transform2, glm::vec3(1.5f * Math::Sin(elapsed), 1.2f, 1));

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
			//Pos               Color             TexCoord
			-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f + zoom, 1.0f - zoom,//¨I
			 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f - zoom, 1.0f - zoom,//¨J
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f + zoom, 0.0f + zoom,//¨L
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f - zoom, 0.0f + zoom//¨K
		};

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#pragma endregion

#pragma region Element buffer object
		unsigned int order[] = {
			0,1,3,
			0,2,3
		};
		unsigned int ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(order), order, GL_STATIC_DRAW);
#pragma endregion

#pragma region Vertex attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
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
		facLocation = glGetUniformLocation(shader.GetId(), "factor");
		transformLocation = glGetUniformLocation(shader.GetId(), "transform");

		transform = glm::mat4(1);
	}
	void TestNode::OnRender() {
		//Clear screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader.GetId());
		glUniform1f(facLocation, factor);

		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texContainer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texFace);

		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform2));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	void TestNode::OnCleanupRender() {

	}
}