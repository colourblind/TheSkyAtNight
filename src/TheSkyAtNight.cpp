#include "cinder/app/AppBasic.h"
#include "cinder/Camera.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"

#include <string>
#include <sstream>
#include <vector>

#include "Resources.h"

using namespace std;
using namespace cinder;
using namespace cinder::app;

class TheSkyAtNight : public AppBasic
{
public:
    virtual void prepareSettings(Settings *settings);
    virtual void setup();
    virtual void draw();

private:
    void RenderStars();
    void RenderNebulae();
    void RenderClouds();

    void DrawStar(float size);
    void DrawPoint(int numPoints);
    void DrawBillboard(Vec3f objectPos, Vec2f size, Camera *camera);

    gl::Texture smallStarTexture_;
    gl::Texture mediumStarTexture_;
    gl::Texture largeStarTexture_;
    gl::Texture hugeStarTexture_;
    gl::Texture cloudTexture0_;
    gl::Texture cloudTexture1_;
    gl::Fbo side_;
    gl::Fbo final_;
    Color tint_;
    CameraPersp camera_;
    int seed_;
    int textureSize_;
    int frame_;
};

void TheSkyAtNight::prepareSettings(Settings *settings)
{
    settings->setWindowSize(128, 128);
}

void TheSkyAtNight::setup()
{
    vector<string> args = getArgs();
    if (args.size() > 1)
        textureSize_ = atoi(args[1].c_str());
    else
        textureSize_ = 512;

    if (args.size() > 2)
        seed_ = atoi(args[2].c_str());
    else
    {
        Rand::randomize();
        seed_ = Rand::randInt(65536);
    }

    frame_ = 0;

    gl::disableDepthRead();
    gl::disableDepthWrite();

    final_ = gl::Fbo(textureSize_ * 3, textureSize_ * 2, false, true, false);
    side_ = gl::Fbo(textureSize_, textureSize_, false, true, false);

    // Load textures
    gl::Texture::Format format;
    format.enableMipmapping(true);
    format.setMinFilter(GL_LINEAR_MIPMAP_NEAREST);
    format.setMagFilter(GL_LINEAR);
    
    smallStarTexture_ = gl::Texture(loadImage(loadResource(RES_STAR_TEXTURE2)), format);
    mediumStarTexture_ = gl::Texture(loadImage(loadResource(RES_STAR_TEXTURE0)), format);
    largeStarTexture_ = gl::Texture(loadImage(loadResource(RES_STAR_TEXTURE1)), format);
    hugeStarTexture_ = gl::Texture(loadImage(loadResource(RES_STAR_TEXTURE1)), format);
    cloudTexture0_ = gl::Texture(loadImage(loadResource(RES_CLOUD_TEXTURE0)), format);
    cloudTexture1_ = gl::Texture(loadImage(loadResource(RES_CLOUD_TEXTURE1)), format);

    Rand::randSeed(seed_);

    int r = Rand::randInt(0, 2);
    int g = Rand::randInt(0, 2);
    int b = Rand::randInt(0, 2);
    tint_ = ColorA(r, g, b);

    side_.bindFramebuffer();

    camera_ = CameraPersp(textureSize_, textureSize_, 90, 0.1f, 20);
    camera_.setWorldUp(Vec3f(0, -1, 0));
}

void TheSkyAtNight::draw()
{
    if (frame_ < 6)
    {
        Rand::randSeed(seed_);
        Area destination;

        string filename;
        switch(frame_)
        {
        case 0:
            camera_.lookAt(Vec3f(0, 0, 0), Vec3f(0, 0, -1));
            filename = "f.png";
            destination = Area(0, 0, textureSize_, textureSize_);
            break;
        case 1:
            camera_.lookAt(Vec3f(0, 0, 0), Vec3f(0, 0, 1));
            filename = "b.png";
            destination = Area(textureSize_, 0, textureSize_ * 2, textureSize_);
            break;
        case 2:
            camera_.lookAt(Vec3f(0, 0, 0), Vec3f(-1, 0, 0));
            filename = "l.png";
            destination = Area(textureSize_ * 2, 0, textureSize_ * 3, textureSize_);
            break;
        case 3:
            camera_.lookAt(Vec3f(0, 0, 0), Vec3f(1, 0, 0));
            filename = "r.png";
            destination = Area(0, textureSize_, textureSize_, textureSize_ * 2);
            break;
        case 4:
            camera_.lookAt(Vec3f(0, 0, 0), Vec3f(0, -1, 0));
            filename = "u.png";
            destination = Area(textureSize_, textureSize_, textureSize_ * 2, textureSize_ * 2);
            break;
        case 5:
            camera_.lookAt(Vec3f(0, 0, 0), Vec3f(0, 1, 0));
            filename = "d.png";
            destination = Area(textureSize_ * 2, textureSize_, textureSize_ * 3, textureSize_ * 2);
            break;
        }

        gl::clear();
        gl::setMatrices(camera_);
        gl::setViewport(Area(0, 0, textureSize_, textureSize_));

        RenderClouds();
        RenderNebulae();
        RenderStars();

        side_.blitTo(final_, Area(0, 0, textureSize_, textureSize_), destination);

        stringstream fullFilename;
        fullFilename << getAppPath() << "out\\" << filename;
        writeImage(fullFilename.str(), side_.getTexture());

        frame_ ++;
    }
    else
    {
        stringstream fullFilename;
        fullFilename << getAppPath() << "out\\final_" << seed_ << ".png";
        writeImage(fullFilename.str(), final_.getTexture());
        quit();
    }
}

void TheSkyAtNight::RenderStars()
{
    gl::enableAdditiveBlending();
    smallStarTexture_.enableAndBind();
    int numSmallStars = Rand::randInt(500, 10000);
    for (int i = 0; i < numSmallStars; i ++)
        DrawStar(0.005f);

    mediumStarTexture_.enableAndBind();
    int numMediumStars = Rand::randInt(200, 5000);
    for (int i = 0; i < numMediumStars; i ++)
        DrawStar(0.02f);

    largeStarTexture_.enableAndBind();
    int numLargeStars = Rand::randInt(20, 200);
    for (int i = 0; i < numLargeStars; i ++)
        DrawStar(0.06f);

    hugeStarTexture_.enableAndBind();
    int numHugeStars = Rand::randInt(0, 4);
    for (int i = 0; i < numHugeStars; i ++)
        DrawStar(0.2f);

    hugeStarTexture_.disable();
    DrawPoint(Rand::randInt(10000, 100000));
}

void TheSkyAtNight::RenderNebulae()
{
    Vec3f right, up;
    camera_.getBillboardVectors(&right, &up);

    int numNebula = Rand::randInt(0, 4);
    for (int i = 0; i < numNebula; i ++)
    {
        float x = Rand::randFloat(-3, 3);
        float y = Rand::randFloat(-3, 3);
        float z = Rand::randFloat(-3, 3);
        Vec3f pos = Vec3f(x, y, z);

        float size = Rand::randFloat(0.5, 4);

        int numClouds = Rand::randInt(10, 50);
        for (int i = 0; i < numClouds; i ++)
        {
            float x1 = Rand::randFloat(0, 1);
            float y1 = Rand::randFloat(0, 1);
            float z1 = Rand::randFloat(0, 1);
            int cloudTexture = Rand::randInt(6);
            x1 = x1 * x1 - 0.5f; // Square the offsets to make them tend towards the centre
            y1 = y1 * y1 - 0.5f;
            z1 = z1 * z1 - 0.5f;
            Vec3f offset = Vec3f(x1 * size, y1 * size, z1 * size);
            float scaleX = Rand::randFloat(0.5f, 1);
            float scaleY = Rand::randFloat(0.5f, 1);
            Vec2f scale = Vec2f(scaleX * size, scaleY * size);
            float rotate = Rand::randFloat(0, 360);

            float r = Rand::randFloat(0, 1);
            float b = Rand::randFloat(0, 1);
            float g = Rand::randFloat(0, 1);
            float a = Rand::randFloat(0, 1);

            if (cloudTexture == 0)
            {
                cloudTexture1_.enableAndBind();
                gl::enableAlphaBlending();
                gl::color(Color::white());
            }
            else
            {
                cloudTexture0_.enableAndBind();
                gl::enableAdditiveBlending();
                gl::color(ColorA(tint_.r + r, tint_.g + g, tint_.b + b, a));
            }
            DrawBillboard(pos + offset, Vec2f(scaleX, scaleY), &camera_); // TODO: rotate
        }
    }
}

void TheSkyAtNight::RenderClouds()
{
    Vec3f right, up;
    camera_.getBillboardVectors(&right, &up);

    gl::enableAdditiveBlending();
    cloudTexture0_.enableAndBind();
    int numClouds = Rand::randInt(3, 15);
    for (int i = 0; i < numClouds; i ++)
    {
        float x = Rand::randFloat(-5, 5);
        float y = Rand::randFloat(-5, 5);
        float z = Rand::randFloat(-5, 5);
        float size = Rand::randFloat(4, 10);
        // float rotation = Rand::randFloat(0, 360);
        gl::color(tint_ * 0.25f);
        DrawBillboard(Vec3f(x, y, z), Vec2f(size, size), &camera_);
    }
}

void TheSkyAtNight::DrawStar(float size)
{
    float x = Rand::randFloat(-5, 5);
    float y = Rand::randFloat(-5, 5);
    float z = Rand::randFloat(-5, 5);
    float r = Rand::randFloat(0.85f, 1);
    float g = Rand::randFloat(0.85f, 1);
    float b = Rand::randFloat(0.85f, 1);

    Vec3f position = Vec3f(x, y, z);
    float distSq = position.lengthSquared();
    if (distSq < 1)
        position *= 1.0f / distSq;

    gl::color(Color(r, g, b));
    DrawBillboard(position, Vec2f(size, size), &camera_);
}

void TheSkyAtNight::DrawPoint(int numPoints)
{
    vector<Vec3f> verts;
    vector<Vec3f> colours;

    gl::enableAdditiveBlending();
    for (int i = 0; i < numPoints; i ++)
    {
        float x = Rand::randFloat(-5, 5);
        float y = Rand::randFloat(-5, 5);
        float z = Rand::randFloat(-5, 5);
        float colour = Rand::randFloat(0, 0.5f);
        verts.push_back(Vec3f(x, y, z));
        colours.push_back(Vec3f(colour, colour, colour));
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &(verts[0].x));
    glColorPointer(3, GL_FLOAT, 0, &(colours[0].x));

	glDrawArrays(GL_POINTS, 0, verts.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void TheSkyAtNight::DrawBillboard(Vec3f objectPos, Vec2f scale, Camera *camera)
{
    // Yoinked from the SlimDX implementation (thanks Promit and friends!)
    Matrix44f result;
    Vec3f difference = objectPos - camera->getEyePoint();
    Vec3f crossed;
    Vec3f final;

    float lengthSq = difference.lengthSquared();
    if (lengthSq < 0.0001f)
        difference = camera->getViewDirection();
    else
        difference *= static_cast<float>(1.0f / math<float>::sqrt(lengthSq));

    crossed = camera->getWorldUp().cross(difference);
    crossed.normalize();
    final = difference.cross(crossed);

    result.m[0] = crossed.x;
    result.m[1] = crossed.y;
    result.m[2] = crossed.z;
    result.m[3] = 0;

    result.m[4] = final.x;
    result.m[5] = final.y;
    result.m[6] = final.z;
    result.m[7] = 0;
    
    result.m[8] = difference.x;
    result.m[9] = difference.y;
    result.m[10] = difference.z;
    result.m[11] = 0;

    result.m[12] = objectPos.x;
    result.m[13] = objectPos.y;
    result.m[14] = objectPos.z;
    result.m[15] = 1;

	glEnableClientState(GL_VERTEX_ARRAY);
	Vec3f verts[4];
	glVertexPointer(3, GL_FLOAT, 0, &verts[0].x);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	GLfloat texCoords[8] = { 0, 0, 0, 1, 1, 0, 1, 1 };
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    gl::pushModelView();
    gl::multModelView(result);

    Vec2f halfScale = scale * 0.5;

	verts[0] = Vec3f(-halfScale.x, -halfScale.y, 0);
	verts[1] = Vec3f(-halfScale.x, halfScale.y, 0);
	verts[2] = Vec3f(halfScale.x, -halfScale.y, 0);
	verts[3] = Vec3f(halfScale.x, halfScale.y, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    gl::popModelView();
}

CINDER_APP_BASIC(TheSkyAtNight, RendererGl)
