#version 410

#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)

uniform mat4 ciModelViewProjection;

in vec4 ciPosition;
in vec2 ciTexCoord0;

out vec2 TexCoord0;
out vec2	TexCoord0x;
out vec2	TexCoord0y;

vec2 ds_set(float a) {
    vec2 z;
    z.x = a;
    z.y = 0.0;
    return z;
}

void main()
{
	TexCoord0 = ciTexCoord0;
	TexCoord0x = ds_set(TexCoord0.x);
	TexCoord0y = ds_set(TexCoord0.y);
    gl_Position = ciModelViewProjection * ciPosition;
}