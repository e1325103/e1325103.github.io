#include "../src/Frame.h"

Frame Frame::interpolate(Frame f1, Frame f2, float f) {
	vec2 center = (1 - f) * f1.center + f * f2.center;
	float scale = (1 - f) * f1.scale + f * f2.scale;
	int iter = (int)round((1 - f) * f1.iter + f * f2.iter);
	return Frame(center, scale, iter);
}

Frame::Frame(vec2 _center, float _scale, int _iter) {
	center = _center;
	scale = _scale;
	iter = _iter;
	steps = 60;
}