#version 150

uniform mat4 ciModelViewProjection;

in vec4 ciPosition;
in vec2 ciTexCoord0;

noperspective out vec2 TexCoord0;

void main()
{
	TexCoord0 = ciTexCoord0;
    gl_Position = ciModelViewProjection * ciPosition;
}