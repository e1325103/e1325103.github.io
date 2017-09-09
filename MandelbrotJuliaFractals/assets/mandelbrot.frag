#version 150

uniform sampler2D tex;
uniform vec2 center;
uniform float scale;
uniform int iter;

noperspective in vec2	TexCoord0;
out vec4 oColor;

void main() {
    vec2 z, c;

    c.x = 1.3333 * (TexCoord0.x - 0.5) * scale - center.x;
    c.y = (TexCoord0.y - 0.5) * scale - center.y;

    int i;
    z = c;
    for(i=0; i<iter; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }

	oColor = texture2D(tex, vec2((i == iter ? 0.0 : float(i)) / iter, 0.5));
}