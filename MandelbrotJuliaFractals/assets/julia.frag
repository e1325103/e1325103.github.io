#version 150

uniform sampler2D tex;
uniform vec2 c;
uniform int iter;

noperspective in vec2	TexCoord0;
out vec4 oColor;

void main() {
    vec2 z;
    z.x = 3.0 * (TexCoord0.x - 0.5);
    z.y = 2.0 * (TexCoord0.y - 0.5);

    int i;
    for(i=0; i<iter; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }

	oColor = texture2D(tex, vec2((i == iter ? 0.0 : float(i)) / iter, 0.5));
}