#include "cinder/app/AppBasic.h"
#include "cinder/Camera.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"

#include <string>
#include <vector>

#include "Resources.h"

using namespace std;
using namespace cinder;
using namespace cinder::app;

class TheSkyAtNight : public AppBasic
{
public:
    virtual void setup();
    virtual void draw();

private:
    void RenderStars();
    void RenderClouds();

    gl::Texture smallStarTexture_;
    gl::Texture mediumStarTexture_;
    gl::Texture largeStarTexture_;
    gl::Texture hugeStarTexture_;
    gl::Texture cloudTexture_;
    gl::Fbo fbo_;
    bool done_;
};

void TheSkyAtNight::setup()
{
    vector<string> args = getArgs();
    if (args.size() > 1)
        Rand::randSeed(atoi(args[1].c_str()));
    else
        Rand::randomize();

    gl::enableAdditiveBlending();
    gl::disableDepthRead();
    gl::disableDepthWrite();

    // Load textures
    gl::Texture::Format format;
    format.enableMipmapping(true);
    format.setMinFilter(GL_LINEAR_MIPMAP_NEAREST);
    format.setMagFilter(GL_LINEAR);
    
    smallStarTexture_ = gl::Texture(loadImage(loadResource(RES_STAR_TEXTURE0)), format);
    mediumStarTexture_ = gl::Texture(loadImage(loadResource(RES_STAR_TEXTURE1)), format);
    largeStarTexture_ = gl::Texture(loadImage(loadResource(RES_STAR_TEXTURE1)), format);
    hugeStarTexture_ = gl::Texture(loadImage(loadResource(RES_STAR_TEXTURE1)), format);
    cloudTexture_ = gl::Texture(loadImage(loadResource(RES_CLOUD_TEXTURE0)), format);

    done_ = false;
}

void TheSkyAtNight::draw()
{
//    CameraPersp camera = CameraPersp(512, 512, 90, 1, 10);

    if (!done_)
    {
        int r = Rand::randInt(0, 2);
        int g = Rand::randInt(0, 2);
        int b = Rand::randInt(0, 2);
        float tintStrength = Rand::randFloat(0, 1);

        // gl::clear(Color(r * tintStrength * 0.1f, g * tintStrength * 0.1f, b * tintStrength * 0.1f));
        gl::clear();

        CameraPersp camera = CameraPersp(getWindowWidth(), getWindowHeight(), 60, 0.5f, 10);
        camera.lookAt(Vec3f(0, 0, 0), Vec3f(0, 0, -1));
        gl::setMatrices(camera);

        gl::color(Color(r * tintStrength, g * tintStrength, b * tintStrength));
        // gl::color(ColorA(255, 255, 255, 32));
        RenderClouds();

        RenderStars();

        done_ = true;
    }

/*
    // render up
    camera.lookAt(Vec3f(0, 0, 0), Vec3f(0, 1, 0));
    writeImage(getHomeDirectory() + "u.png", copyWindowSurface());
    // render down
    camera.lookAt(Vec3f(0, 0, 0), Vec3f(0, -1, 0));
    writeImage(getHomeDirectory() + "d.png", copyWindowSurface());
    // render left
    camera.lookAt(Vec3f(0, 0, 0), Vec3f(-1, 0, 0));
    writeImage(getHomeDirectory() + "l.png", copyWindowSurface());
    // render right
    camera.lookAt(Vec3f(0, 0, 0), Vec3f(1, 0, 0));
    writeImage(getHomeDirectory() + "r.png", copyWindowSurface());
    // render front
    camera.lookAt(Vec3f(0, 0, 0), Vec3f(0, 0, 1));
    writeImage(getHomeDirectory() + "f.png", copyWindowSurface());
    // render back
    camera.lookAt(Vec3f(0, 0, 0), Vec3f(0, 0, -1));
    //writeImage(getHomeDirectory() + "b.png", copyWindowSurface());

    //quit();
    */
}

void TheSkyAtNight::RenderStars()
{
    smallStarTexture_.enableAndBind();
    int numSmallStars = Rand::randInt(10000, 100000);
    for (int i = 0; i < numSmallStars; i ++)
    {
        float x = Rand::randFloat(-3, 3);
        float y = Rand::randFloat(-3, 3);
        float z = Rand::randFloat(-3, 3);
        float r = Rand::randFloat(0.85f, 1);
        float g = Rand::randFloat(0.85f, 1);
        float b = Rand::randFloat(0.85f, 1);
        gl::color(Color(r, g, b));
        gl::drawBillboard(Vec3f(x, y, z), Vec2f(0.01f, 0.01f), 0, Vec3f(1, 0, 0), Vec3f(0, 1, 0));
    }

    mediumStarTexture_.enableAndBind();
    int numMediumStars = Rand::randInt(200, 5000);
    for (int i = 0; i < numMediumStars; i ++)
    {
        float x = Rand::randFloat(-3, 3);
        float y = Rand::randFloat(-3, 3);
        float z = Rand::randFloat(-3, 3);
        float r = Rand::randFloat(0.85f, 1);
        float g = Rand::randFloat(0.85f, 1);
        float b = Rand::randFloat(0.85f, 1);
        gl::color(Color(r, g, b));
        gl::drawBillboard(Vec3f(x, y, z), Vec2f(0.04f, 0.04f), 0, Vec3f(1, 0, 0), Vec3f(0, 1, 0));
    }

    largeStarTexture_.enableAndBind();
    int numLargeStars = Rand::randInt(20, 200);
    for (int i = 0; i < numLargeStars; i ++)
    {
        float x = Rand::randFloat(-3, 3);
        float y = Rand::randFloat(-3, 3);
        float z = Rand::randFloat(-3, 3);
        float r = Rand::randFloat(0.85f, 1);
        float g = Rand::randFloat(0.85f, 1);
        float b = Rand::randFloat(0.85f, 1);
        gl::color(Color(r, g, b));
        gl::drawBillboard(Vec3f(x, y, z), Vec2f(0.08f, 0.08f), 0, Vec3f(1, 0, 0), Vec3f(0, 1, 0));
    }

    hugeStarTexture_.enableAndBind();
    int numHugeStars = Rand::randInt(0, 4);
    for (int i = 0; i < numHugeStars; i ++)
    {
        float x = Rand::randFloat(-3, 3);
        float y = Rand::randFloat(-3, 3);
        float z = Rand::randFloat(-3, 3);
        float r = Rand::randFloat(0.85f, 1);
        float g = Rand::randFloat(0.85f, 1);
        float b = Rand::randFloat(0.85f, 1);
        gl::color(Color(r, g, b));
        gl::drawBillboard(Vec3f(x, y, z), Vec2f(0.3f, 0.3f), 0, Vec3f(1, 0, 0), Vec3f(0, 1, 0));
    }
}

void TheSkyAtNight::RenderClouds()
{
    cloudTexture_.enableAndBind();
    int numClouds = Rand::randInt(3, 15);
    for (int i = 0; i < numClouds; i ++)
    {
        float x = Rand::randFloat(-5, 5);
        float y = Rand::randFloat(-5, 5);
        float z = Rand::randFloat(-5, 5);
        float size = Rand::randFloat(4, 10);
        float rotation = Rand::randFloat(0, 360);
        gl::drawBillboard(Vec3f(x, y, z), Vec2f(size, size), rotation, Vec3f(1, 0, 0), Vec3f(0, 1, 0));
    }
}

CINDER_APP_BASIC(TheSkyAtNight, RendererGl)
