#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include <chrono>

using namespace ci;
using namespace ci::app;
using namespace std;

class MandelbrotJuliaFractalsApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	gl::TextureRef	colorTexture;
	gl::BatchRef planeBatch;

	CameraPersp mCam;
	gl::BatchRef mRectM;
	gl::BatchRef mRectJ;

	float f = 70;
};

void MandelbrotJuliaFractalsApp::setup()
{
	mCam.lookAt(vec3(0, 0, 1), vec3(0));

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

	auto rect = geom::Rect().texCoords(vec2(0, 0),
		vec2(1, 0),
		vec2(1, 1),
		vec2(0, 1));

	mRectM = gl::Batch::create(rect, rectMGlsl);
	mRectJ = gl::Batch::create(rect, rectJGlsl);

	float t = (float)10 / 1000.0;
	float cx = (sin(cos(t / 10) * 10) + cos(t * 2.0) / 4.0 + sin(t * 3.0) / 6.0) * 0.8;
	float cy = (cos(sin(t / 10) * 10) + sin(t * 2.0) / 4.0 + cos(t * 3.0) / 6.0) * 0.8;

	mRectM->getGlslProg()->uniform("tex", 0);
	mRectM->getGlslProg()->uniform("center", glm::vec2(0.7f, 0.0f));
	mRectM->getGlslProg()->uniform("scale", 2.2f);
	mRectM->getGlslProg()->uniform("iter", 100);


	mRectJ->getGlslProg()->uniform("tex", 0);
	mRectJ->getGlslProg()->uniform("c", glm::vec2(cx, cy));
	mRectJ->getGlslProg()->uniform("iter", 100);

	gl::enableDepthWrite();
	gl::enableDepthRead();
}

void MandelbrotJuliaFractalsApp::draw()
{
	gl::clear(Color(0, 0, 0));
	gl::setMatrices(mCam);
	gl::pushMatrices();
	gl::scale(vec2(0.35, 0.35));
	gl::translate(vec3(-1, 0, 0));
	mRectM->draw();
	gl::popModelMatrix();
	gl::pushMatrices();
	gl::scale(vec2(0.35, 0.35));
	gl::translate(vec3(1, 0, 0));
	mRectJ->draw();
	gl::popModelMatrix();
}

void MandelbrotJuliaFractalsApp::mouseDown( MouseEvent event )
{
	float cx = (float)event.getX() / (float)getWindowWidth();
	float cy = (float)event.getY() / (float)getWindowHeight();

	console() << "Click: " << event.getX() << " - " << event.getY() << std::endl << "Pos:  " << cx << " - " << cy << std::endl;

	mRectJ->getGlslProg()->uniform("c", glm::vec2(cx, cy));
	mRectM->getGlslProg()->uniform("center", glm::vec2(cx, cy));
}

void MandelbrotJuliaFractalsApp::update()
{
	/*f += 1;
	mRectM->getGlslProg()->uniform("iter", (int)f);*/
	//mRectM->getGlslProg()->uniform("scale", f);
	//f = f * 0.99f;
	/*if (f > 80){
		f = 0;
		float t = (float)rand() * 100 / 1000.0;
		float cx = (sin(cos(t / 10) * 10) + cos(t * 2.0) / 4.0 + sin(t * 3.0) / 6.0) * 0.8;
		float cy = (cos(sin(t / 10) * 10) + sin(t * 2.0) / 4.0 + cos(t * 3.0) / 6.0) * 0.8;
		mRectM->getGlslProg()->uniform("c", glm::vec2(cx, cy));
	}
	f = f + 1;*/
}

CINDER_APP( MandelbrotJuliaFractalsApp, RendererGl(RendererGl::Options().msaa(16)) )
