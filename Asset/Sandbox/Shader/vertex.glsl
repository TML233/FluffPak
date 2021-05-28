#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inCoord;

out vec3 ourColor;
out vec2 texCoord;

void main() {
	gl_Position = vec4(inPos, 1.0f);
	ourColor=inColor;
	texCoord=inCoord;
}
