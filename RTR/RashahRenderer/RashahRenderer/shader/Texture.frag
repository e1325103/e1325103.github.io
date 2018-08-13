#version 430 core

uniform vec3 targetColor;
uniform sampler2D imgTexture;

in vec2 fragUV;

out vec4 color;

void main() {
	color = vec4(texture(imgTexture, fragUV).rgb, 1);	
}