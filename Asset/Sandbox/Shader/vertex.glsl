#version 330 core

layout(location = 0) in vec3 rawPos;
//layout(location = 1) in vec3 rawColor;
layout(location = 1) in vec2 rawTexCoord;

//out vec3 vertexColor;
out vec2 vertexTexCoord;

uniform mat4 transformModel;
uniform mat4 transformView;
uniform mat4 transformProjection;

void main() {
	gl_Position = transformProjection * transformView * transformModel * vec4(rawPos, 1.0f);
	//vertexColor = rawColor;
	vertexTexCoord = rawTexCoord;
}
