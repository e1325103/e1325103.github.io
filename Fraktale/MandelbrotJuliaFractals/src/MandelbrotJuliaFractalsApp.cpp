#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/params/Params.h"
#include "cinder/gl/Fbo.h"
#include "cinder/ImageIo.h"
#include <chrono>

#include "Frame.h"

using namespace ci;
using namespace ci::app;
using namespace ci::params;
using namespace std;

class MandelbrotJuliaFractalsApp : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void mouseDrag(MouseEvent event) override;
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;
	void fillOptions();
	void addFrame();
	void startAnimation();

	gl::TextureRef	colorTexture;
	gl::BatchRef planeBatch;

	CameraOrtho mCam;
	gl::BatchRef mRectM;
	gl::BatchRef mRectJ;

	vec3 v = vec3(1, 0, 0);
	int iter = 100;
	float scale = 2.2f;
	vec2 center = vec2(0.7f, 0.0f);

	InterfaceGlRef mParams;
	bool showJulia = true;
	bool showAnimation = false;
	int animationStep = 0;
	int animationFrame = 0;
	std::vector<string> frameTextList = std::vector<string>({ "Frame 1" });
	std::vector<Frame> frameList = std::vector<Frame>({ Frame(center, scale, iter) });
	int frameSelected = 0;
	gl::FboRef fbo;

};

void MandelbrotJuliaFractalsApp::setup()
{
	mCam.lookAt(vec3(1, 0, 0), vec3(0));

	DataSourceRef vert;
	DataSourceRef fragM;
	DataSourceRef fragJ;
	try {
		colorTexture = gl::Texture::create(loadImage(loadAsset("colorbar4.png")));
		vert = loadAsset("pass.vert");
		fragM = loadAsset("mandelbrot.frag");
		fragJ = loadAsset("julia.frag");
	}
	catch (const std::exception& e) {
		console() << "loadImage error: " << e.what() << std::endl;
	}
	colorTexture->bind(0);



	auto rectMGlsl = gl::GlslProg::create(vert, fragM);
	auto rectJGlsl = gl::GlslProg::create(vert, fragJ);

	/*auto rect = geom::Rect().texCoords(
		vec2(0, 0),
		vec2(1, 0),
		vec2(1, 1),
		vec2(0, 1));*/
	auto rect = geom::Rect().texCoords(
		vec2(0.5, 0),
		vec2(1, 0),
		vec2(1, 0.5),
		vec2(0.5, 0.5));

	mRectM = gl::Batch::create(rect, rectMGlsl);
	mRectJ = gl::Batch::create(rect, rectJGlsl);

	float t = (float)10 / 1000.0;
	float cx = (sin(cos(t / 10) * 10) + cos(t * 2.0) / 4.0 + sin(t * 3.0) / 6.0) * 0.8;
	float cy = (cos(sin(t / 10) * 10) + sin(t * 2.0) / 4.0 + cos(t * 3.0) / 6.0) * 0.8;

	mRectM->getGlslProg()->uniform("tex", 0);
	mRectM->getGlslProg()->uniform("center", center);
	mRectM->getGlslProg()->uniform("scale", scale);
	mRectM->getGlslProg()->uniform("iter", iter);


	mRectJ->getGlslProg()->uniform("tex", 0);
	mRectJ->getGlslProg()->uniform("c", glm::vec2(cx, cy));
	mRectJ->getGlslProg()->uniform("iter", iter);

	gl::enableDepthWrite();
	gl::enableDepthRead();

	mParams = mParams = params::InterfaceGl::create("Fractal Options", ivec2(250, 250));
	fillOptions();

	gl::Fbo::Format format;
	format.getDefaultColorInternalFormat(GL_RGBA32F);
	fbo = gl::Fbo::create(640, 480, format);
}

void MandelbrotJuliaFractalsApp::fillOptions() {
	mParams->clear();
	mParams->addText("Update Params");
	mParams->addParam("Show Julia Fractal", &showJulia);
	mParams->addSeparator();
	mParams->addParam("Frame", frameTextList, &frameSelected).updateFn([&] { cout << frameSelected; });
	//mParams->addParam("Center", &center);
	//mParams->addParam("Scale", &scale);
	mParams->addParam("Iterations", &iter).min(0).keyIncr("i").keyDecr("o").step(100);
	//mParams->addParam("Center", &center);
	mParams->addButton("Add Frame", bind(&MandelbrotJuliaFractalsApp::addFrame, this));
	mParams->addButton("Start Animation", bind(&MandelbrotJuliaFractalsApp::startAnimation, this));
}

void MandelbrotJuliaFractalsApp::draw()
{
	gl::clear(Color(0.2f, 0.2f, 0.2f));

	gl::pushModelMatrix();
	gl::translate(getWindowCenter());
	gl::scale(vec2(getWindowWidth(), getWindowHeight()));
	mRectM->draw();
	gl::popModelMatrix();
	if (showJulia) {
		gl::pushModelMatrix();
		gl::translate(vec3(getWindowWidth() / 4, getWindowHeight() / 4, 1));
		gl::scale(vec2(getWindowWidth() / 2, getWindowHeight() / 2));
		mRectJ->draw();
		gl::popModelMatrix();
	}
	mParams->draw();
}

void MandelbrotJuliaFractalsApp::mouseDown(MouseEvent event)
{
	float cx = (float)event.getX() / (float)getWindowWidth();
	float cy = (float)event.getY() / (float)getWindowHeight();

	cx = 1.3333 * (cx - 0.5) * scale - center.x;
	cy = (cy - 0.5) * scale - center.y;
	mRectJ->getGlslProg()->uniform("c", glm::vec2(cx, cy));

}

void MandelbrotJuliaFractalsApp::mouseDrag(MouseEvent event) {
	mouseDown(event);
}

void MandelbrotJuliaFractalsApp::keyDown(KeyEvent event) {
	if (event.getChar() == 'c') {
		scale /= 2.0f;
	}
	if (event.getChar() == 'v') {
		scale *= 2.0f;
	}
	if (event.getChar() == 'a') {
		center += vec2(0.1 * scale, 0);
	}
	else if (event.getChar() == 'd') {
		center += vec2(-0.1 * scale, 0);
	}
	else if (event.getChar() == 'w') {
		center += vec2(0, 0.1 * scale);
	}
	else if (event.getChar() == 's') {
		center += vec2(0, -0.1 * scale);
	}
	mRectM->getGlslProg()->uniform("iter", iter);
	mRectM->getGlslProg()->uniform("scale", scale);
	mRectM->getGlslProg()->uniform("center", center);
}

void MandelbrotJuliaFractalsApp::update()
{
	v -= vec3(-0.01, 0, 0);
	mCam.lookAt(v, vec3(0));

	if (showAnimation) {
		Frame f1 = frameList.at(animationFrame);
		Frame f2 = frameList.at(animationFrame + 1);
		float d = ((float)animationStep) / f1.steps;
		Frame fi = Frame::interpolate(f1, f2, d);
		iter = fi.iter;
		scale = fi.scale;
		center = fi.center;
		animationStep++;
		if (animationStep > f1.steps) {
			animationFrame++;
			animationStep = 0;
			if (animationFrame >= (frameList.size() - 1)) {
				showAnimation = false;
			}
		}
		fbo.get()->bindFramebuffer();
		this->draw();
		fbo.get()->unbindFramebuffer();
		writeImage( "C:\\Users\\nicol\\Desktop\\Fractals\\i.png", fbo.get()->getColorTexture().get()->createSource());
	}
	mRectM->getGlslProg()->uniform("iter", iter);
	mRectM->getGlslProg()->uniform("scale", scale);
	mRectM->getGlslProg()->uniform("center", center);
}


void MandelbrotJuliaFractalsApp::addFrame()
{
	string s = frameTextList.back();
	s = s.substr(6, s.length());
	int i = std::stoi(s);
	i++;
	frameTextList.push_back("Frame " + to_string(i));
	frameList.push_back(Frame(center, scale, iter));
	fillOptions();
}

void MandelbrotJuliaFractalsApp::startAnimation()
{
	showAnimation = true;
	animationFrame = 0;
	animationStep = 0;
}

CINDER_APP(MandelbrotJuliaFractalsApp, RendererGl(RendererGl::Options().msaa(16)))
