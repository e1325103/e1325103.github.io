#version 410

#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)

uniform sampler2D tex;
uniform vec2 center;
uniform float scale;
uniform int iter;

in vec2	TexCoord0x;
in vec2	TexCoord0y;
in vec2	TexCoord0;
out vec4 oColor;

vec2 ds_set(float a) {
    vec2 z;
    z.x = a;
    z.y = 0.0;
    return z;
}

vec2 ds_add(vec2 dsa, vec2 dsb) {
	vec2 dsc;
	float t1, t2, e;

	t1 = dsa.x + dsb.x;
	e = t1 - dsa.x;
	t2 = ((dsb.x - e) + (dsa.x - (t1 - e))) + dsa.y + dsb.y;

	dsc.x = t1 + t2;
	dsc.y = t2 - (dsc.x - t1);
	return dsc;
}

bool ds_greaterThan(vec2 dsa, vec2 dsb) {
	if (dsa.x > dsb.x) {
		return true;
	} else if(dsa.x == dsb.x) {
		if (dsa.y > dsb.y) {
			return true;
		}
	}
	return false;
}

vec2 ds_mul(vec2 dsa, vec2 dsb)
{
	vec2 dsc;
	float c11, c21, c2, e, t1, t2;
	float a1, a2, b1, b2, cona, conb, split = 8193.;

	cona = dsa.x * split;
	conb = dsb.x * split;
	a1 = cona - (cona - dsa.x);
	b1 = conb - (conb - dsb.x);
	a2 = dsa.x - a1;
	b2 = dsb.x - b1;

	c11 = dsa.x * dsb.x;
	c21 = a2 * b2 + (a2 * b1 + (a1 * b2 + (a1 * b1 - c11)));

	c2 = dsa.x * dsb.y + dsa.y * dsb.x;

	t1 = c11 + c2;
	e = t1 - c11;
	t2 = dsa.y * dsb.y + ((c2 - e) + (c11 - (t1 - e))) + c21;

	dsc.x = t1 + t2;
	dsc.y = t2 - (dsc.x - t1);

	return dsc;
}

void main() {
    vec2 zx, zy;

    vec2 cx = ds_add(ds_mul(ds_mul(ds_set(1.3333), ds_add(TexCoord0x, -ds_set(0.5))), ds_set(scale)), -ds_set(center.x));
    vec2 cy = ds_add(ds_mul(ds_add(TexCoord0y, -ds_set(0.5)), ds_set(scale)), -ds_set(center.y));

	//vec2 cx = ds_set(1.3333 * (TexCoord0.x - 0.5) * scale - center.x);
    //vec2 cy = ds_set((TexCoord0.y - 0.5) * scale - center.y);

    int i;
    zx = cx;
	zy = cy;
    for(i=0; i<iter; i++) {
        vec2 x = ds_add(ds_add(ds_mul(zx, zx), - ds_mul(zy, zy)), cx);
        vec2 y = ds_add(ds_add(ds_mul(zy, zx), ds_mul(zx, zy)), cy);

        if(ds_greaterThan(ds_add(ds_mul(x, x), ds_mul(y, y)), ds_set(4.0))) break;
        zx = x;
        zy = y;
    }

	oColor = texture2D(tex, vec2((i >= iter ? 0.0 : float(i)) / iter, 0.5));
}