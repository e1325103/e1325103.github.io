#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

class Frame {
public:
	vec2 center;
	float scale;
	int iter;
	int steps;

	Frame(vec2 _center, float _scale, int _iter);
	static Frame interpolate(Frame f1, Frame f2, float f);
};