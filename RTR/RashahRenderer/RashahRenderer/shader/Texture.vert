#version 430 core

layout(location = 0) in vec3 position;
layout(location = 3) in vec2 uv;

uniform mat4 MVP;

out vec2 fragUV;

void main() {
	//fragUV = vec2((position.x + 1.0) / 2.0, (position.y + 1.0) / 2.0);
	fragUV = uv;
	gl_Position = vec4(position, 1);
}