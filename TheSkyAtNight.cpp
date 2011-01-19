#include "cinder/app/AppBasic.h"
#include "cinder/Camera.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"
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
    void RenderNebulae();
    void RenderClouds();

    void DrawStar(float size);

    gl::Texture smallStarTexture_;
    gl::Texture mediumStarTexture_;
    gl::Texture largeStarTexture_;
    gl::Texture hugeStarTexture_;
    gl::Texture cloudTexture_;
    Color tint_;
    bool done_;
};

void TheSkyAtNight::setup()
{
    vector<string> args = getArgs();
    if (args.size() > 1)
        Rand::randSeed(atoi(args[1].c_str()));
    else
        Rand::randomize();

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
    if (!done_)
    {
        int r = Rand::randInt(0, 2);
        int g = Rand::randInt(0, 2);
        int b = Rand::randInt(0, 2);
        tint_ = ColorA(r, g, b);

        // gl::clear(Color(r * tintStrength * 0.1f, g * tintStrength * 0.1f, b * tintStrength * 0.1f));
        gl::clear();

        CameraPersp camera = CameraPersp(getWindowWidth(), getWindowHeight(), 60, 0.5f, 10);
        camera.lookAt(Vec3f(0, 0, 0), Vec3f(0, 0, -1));
        gl::setMatrices(camera);

        RenderClouds();
        RenderNebulae();
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
    gl::enableAdditiveBlending();
    smallStarTexture_.enableAndBind();
    int numSmallStars = Rand::randInt(10000, 100000);
    for (int i = 0; i < numSmallStars; i ++)
        DrawStar(0.01f);

    mediumStarTexture_.enableAndBind();
    int numMediumStars = Rand::randInt(200, 5000);
    for (int i = 0; i < numMediumStars; i ++)
        DrawStar(0.04f);

    largeStarTexture_.enableAndBind();
    int numLargeStars = Rand::randInt(20, 200);
    for (int i = 0; i < numLargeStars; i ++)
        DrawStar(0.08f);

    hugeStarTexture_.enableAndBind();
    int numHugeStars = Rand::randInt(0, 4);
    for (int i = 0; i < numHugeStars; i ++)
        DrawStar(0.3f);
}

void TheSkyAtNight::DrawStar(float size)
{
    float x = Rand::randFloat(-3, 3);
    float y = Rand::randFloat(-3, 3);
    float z = Rand::randFloat(-3, 3);
    float r = Rand::randFloat(0.85f, 1);
    float g = Rand::randFloat(0.85f, 1);
    float b = Rand::randFloat(0.85f, 1);
    gl::color(Color(r, g, b));
    gl::drawBillboard(Vec3f(x, y, z), Vec2f(size, size), 0, Vec3f(1, 0, 0), Vec3f(0, 1, 0));
}

void TheSkyAtNight::RenderNebulae()
{
    gl::enableAlphaBlending();
    gl::enableAdditiveBlending(); // temp
    int numNebula = Rand::randInt(0, 10);
    numNebula = 1;
    for (int i = 0; i < numNebula; i ++)
    {
        float x = Rand::randFloat(-3, 3);
        float y = Rand::randFloat(-3, 3);
        float z = Rand::randFloat(-3, 3);
//        Vec3f pos = Vec3f(x, y, z);
        Vec3f pos = Vec3f(0, 0, -2);

        int numClouds = Rand::randInt(10, 50);
        for (int i = 0; i < numClouds; i ++)
        {
            float x1 = Rand::randFloat(0, 1);
            float y1 = Rand::randFloat(0, 1);
            float z1 = Rand::randFloat(0, 1);
            x1 = x1 * x1 - 0.5f; // Square the offsets to make them tend towards the centre
            y1 = y1 * y1 - 0.5f;
            z1 = z1 * z1 - 0.5f;
            Vec3f offset = Vec3f(x1, y1, z1);
            float scaleX = Rand::randFloat(0.1f, 0.5f);
            float scaleY = Rand::randFloat(0.1f, 0.5f);
            Vec2f scale = Vec2f(scaleX, scaleY);
            float rotate = Rand::randFloat(0, 360);

            float r = Rand::randFloat(0, 1);
            float b = Rand::randFloat(0, 1);
            float g = Rand::randFloat(0, 1);
            float a = Rand::randFloat(0, 1);

            gl::color(ColorA(tint_.r + r, tint_.g + g, tint_.b + b, a));
            gl::drawBillboard(pos + offset, scale, rotate, Vec3f(1, 0, 0), Vec3f(0, 1, 0));
        }
    }
}

void TheSkyAtNight::RenderClouds()
{
    gl::enableAdditiveBlending();
    cloudTexture_.enableAndBind();
    int numClouds = Rand::randInt(3, 15);
    for (int i = 0; i < numClouds; i ++)
    {
        float x = Rand::randFloat(-5, 5);
        float y = Rand::randFloat(-5, 5);
        float z = Rand::randFloat(-5, 5);
        float size = Rand::randFloat(4, 10);
        float rotation = Rand::randFloat(0, 360);
        gl::color(tint_ * 0.25f);
        gl::drawBillboard(Vec3f(x, y, z), Vec2f(size, size), rotation, Vec3f(1, 0, 0), Vec3f(0, 1, 0));
    }
}

CINDER_APP_BASIC(TheSkyAtNight, RendererGl)
