#version 330 core

in vec3 vertexColor;
in vec2 vertexTexCoord;
out vec4 FragColor;

uniform sampler2D mTex1;
uniform sampler2D mTex2;
uniform float factor;

void main() {
	FragColor = mix(texture(mTex1, vertexTexCoord),texture(mTex2, vertexTexCoord),factor);
	//FragColor = texture(mTex2, texCoord);
}
