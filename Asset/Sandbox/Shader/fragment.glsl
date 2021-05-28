#version 330 core

in vec3 ourColor;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D mTex1;
uniform sampler2D mTex2;
uniform float factor;

void main() {
	FragColor = mix(texture(mTex1, texCoord),texture(mTex2, texCoord),factor);
	//FragColor = texture(mTex2, texCoord);
}
