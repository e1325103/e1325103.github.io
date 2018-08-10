#version 430 core

uniform vec3 targetColor;

out vec4 color;

void main() {
	color = vec4(targetColor, 1);	
}