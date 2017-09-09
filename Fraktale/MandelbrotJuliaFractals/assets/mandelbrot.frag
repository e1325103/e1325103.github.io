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


// Emulation based on Fortran-90 double-single package. See http://crd.lbl.gov/~dhbailey/mpdist/
// Substract: res = ds_add(a, b) => res = a + b
vec2 ds_add (vec2 dsa, vec2 dsb)
{
vec2 dsc;
float t1, t2, e;

 t1 = dsa.x + dsb.x;
 e = t1 - dsa.x;
 t2 = ((dsb.x - e) + (dsa.x - (t1 - e))) + dsa.y + dsb.y;

 dsc.x = t1 + t2;
 dsc.y = t2 - (dsc.x - t1);
 return dsc;
}

// Substract: res = ds_sub(a, b) => res = a - b
vec2 ds_sub (vec2 dsa, vec2 dsb)
{
vec2 dsc;
float e, t1, t2;

 t1 = dsa.x - dsb.x;
 e = t1 - dsa.x;
 t2 = ((-dsb.x - e) + (dsa.x - (t1 - e))) + dsa.y - dsb.y;

 dsc.x = t1 + t2;
 dsc.y = t2 - (dsc.x - t1);
 return dsc;
}

// Compare: res = -1 if a < b
//              = 0 if a == b
//              = 1 if a > b
float ds_compare(vec2 dsa, vec2 dsb)
{
 if (dsa.x < dsb.x) return -1.;
 else if (dsa.x == dsb.x) 
	{
	if (dsa.y < dsb.y) return -1.;
	else if (dsa.y == dsb.y) return 0.;
	else return 1.;
	}
 else return 1.;
}

// Multiply: res = ds_mul(a, b) => res = a * b
vec2 ds_mul (vec2 dsa, vec2 dsb)
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

// create double-single number from float
vec2 ds_set(float a)
{
 vec2 z;
 z.x = a;
 z.y = 0.0;
 return z;
}

void main() {
    vec2 zx, zy;

    vec2 cx = ds_sub(ds_mul(ds_mul(ds_set(1.3333), ds_sub(TexCoord0x, ds_set(0.5))), ds_set(scale)), ds_set(center.x));
    vec2 cy = ds_sub(ds_mul(ds_sub(TexCoord0y, ds_set(0.5)), ds_set(scale)), ds_set(center.y));

	//vec2 cx = ds_set(1.3333 * (TexCoord0.x - 0.5) * scale - center.x);
    //vec2 cy = ds_set((TexCoord0.y - 0.5) * scale - center.y);

    int i;
    zx = cx;
	zy = cy;
    for(i=0; i<iter; i++) {
        vec2 x = ds_add(ds_add(ds_mul(zx, zx), - ds_mul(zy, zy)), cx);
        vec2 y = ds_add(ds_add(ds_mul(zy, zx), ds_mul(zx, zy)), cy);

        if(ds_compare(ds_add(ds_mul(x, x), ds_mul(y, y)), ds_set(4.0)) > 0) break;
        zx = x;
        zy = y;
    }

	oColor = texture2D(tex, vec2((i >= iter ? 0.0 : float(i)) / iter, 0.5));
	//oColor = vec4((-cos(0.025*i)+1.0)/2.0,
    //                                      (-cos(0.08*i)+1.0)/2.0,
     //                                     (-cos(0.12*i)+1.0)/2.0,
     //                                  1.0);
}