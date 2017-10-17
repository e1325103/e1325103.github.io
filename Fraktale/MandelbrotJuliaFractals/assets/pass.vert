#version 410

#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)

uniform mat4 ciModelViewProjection;

in vec4 ciPosition;
in vec2 ciTexCoord0;

uniform vec2 centerX;
uniform vec2 centerY;
uniform float scale;

out vec2 TexCoord0;
out vec2	TexCoord0x;
out vec2	TexCoord0y;

vec2 splitDouble(double d) {
	float f1 = float(d);
	float f2 = float(d - double(f1));
	return vec2(f1, f2);
}

double joinDouble(vec2 v) {
	double d1 = double(v.x);
	double d2 = double(v.y);
	return d1 + d2;
}

void main()
{
	TexCoord0 = ciTexCoord0;
	double width = double(scale) / 1.0;
	double d_centerX = joinDouble(centerX);
	double d_centerY = joinDouble(centerY);
	double d_TexCoord0x = 0;
	double d_TexCoord0y = 0;
	if ((ciTexCoord0.x == 0) && (ciTexCoord0.y == 0)) {
		d_TexCoord0x = d_centerX - width;
		d_TexCoord0y = d_centerY + width;
	} else if ((ciTexCoord0.x == 1) && (ciTexCoord0.y == 0)) {
		d_TexCoord0x = d_centerX + width;
		d_TexCoord0y = d_centerY + width;
	} else if ((ciTexCoord0.x == 1) && (ciTexCoord0.y == 1)) {
		d_TexCoord0x = d_centerX + width;
		d_TexCoord0y = d_centerY - width;
	} else if ((ciTexCoord0.x == 0) && (ciTexCoord0.y == 1)) {
		d_TexCoord0x = d_centerX - width;
		d_TexCoord0y = d_centerY - width;
	}
	TexCoord0x = splitDouble(d_TexCoord0x);
	TexCoord0y = splitDouble(d_TexCoord0y);
    gl_Position = ciModelViewProjection * ciPosition;
}