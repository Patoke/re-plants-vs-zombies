#ifndef __EFFECTSYSTEM_H__
#define __EFFECTSYSTEM_H__

#include "DataArray.h"
#include "../ConstEnums.h"
#include "graphics/Graphics.h"
using namespace Sexy;

#define MAX_TRIANGLES 256

class TodTriVertex
{
public:
    float                       x;
    float                       y;
    float                       u;
    float                       v;
    unsigned long               color;
};

class TodTriangleGroup
{
public:
    Image*                      mImage;
    TriVertex                   mVertArray[MAX_TRIANGLES][3];
    int                         mTriangleCount;
    int                         mDrawMode;

    TodTriangleGroup();
    void                        DrawGroup(Graphics* g);
    void                        AddTriangle(Graphics* g, Image* theImage, const SexyMatrix3& theMatrix, const Rect& theClipRect, const Color& theColor, int theDrawMode, const Rect& theSrcRect);
};

extern bool gTodTriangleDrawAdditive;  //0x6A9EF5

class Reanimation;
class TodParticleHolder;
class TrailHolder;
class ReanimationHolder;
class AttachmentHolder;
class EffectSystem
{
public:
    TodParticleHolder*          mParticleHolder;
    TrailHolder*                mTrailHolder;
    ReanimationHolder*          mReanimationHolder;
    AttachmentHolder*           mAttachmentHolder;

public:
    EffectSystem() : mParticleHolder(nullptr), mTrailHolder(nullptr), mReanimationHolder(nullptr), mAttachmentHolder(nullptr) { }
    ~EffectSystem() { }

    void                        EffectSystemInitialize();
    void                        EffectSystemDispose();
    void                        EffectSystemFreeAll();
    void                        ProcessDeleteQueue();
    void                        Update();
};
extern EffectSystem* gEffectSystem;  //[0x6A9EB8]

#endif
