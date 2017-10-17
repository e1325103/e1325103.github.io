#version 410

#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)

uniform sampler2D tex;
uniform vec2 centerX;
uniform vec2 centerY;
uniform float scale;
uniform int iter;

in vec2	TexCoord0x;
in vec2	TexCoord0y;
in vec2	TexCoord0;
out vec4 oColor;

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

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    vec2 zx, zy;

	double d_zx, d_zy;
	double h1 = 1 + (1 / 3);
	double d_TexCoord0x = joinDouble(TexCoord0x);
	double d_TexCoord0y = joinDouble(TexCoord0y);
	double d_centerX = joinDouble(centerX);
	double d_centerY = joinDouble(centerY);
	double d_scale = double(scale);
	
	double d_cx = h1 * (d_TexCoord0x - 0.5) * d_scale - d_centerX;
	double d_cy = (d_TexCoord0y - 0.5) * d_scale - d_centerY;

    int i;
	d_zx = d_cx;
	d_zy = d_cy;
    for(i=0; i<iter; i++) {

		double d_x = ((d_zx * d_zx) - (d_zy * d_zy)) + d_cx;
		double d_y = ((d_zy * d_zx) + (d_zx * d_zy)) + d_cy;

		if (((d_x * d_x) + (d_y * d_y)) > 4.0) break;
        d_zx = d_x;
        d_zy = d_y;
    }
	
	float f_zx = float(d_zx);
	float f_zy = float(d_zy);

	oColor = texture2D(tex, vec2((i >= iter ? 0.0 : float(i)) / iter, 0.5));

	float r = splitDouble(d_cx).y;
	if (r > 0) {
		while (r < 1) {
			r = r * 10;
		} 
	}
	float g = splitDouble(d_cy).y;
	if (g > 0) {
		while (g < 1) {
			g = g * 10;
		} 
	}
	//oColor = vec4(r/2, g/2, 0, 1);
	//oColor = vec4((-cos(0.025*i)+1.0)/2.0,
    //                                      (-cos(0.08*i)+1.0)/2.0,
     //                                     (-cos(0.12*i)+1.0)/2.0,
     //                                  1.0);
}