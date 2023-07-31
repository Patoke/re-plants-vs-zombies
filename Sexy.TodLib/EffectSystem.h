#ifndef __EFFECTSYSTEM_H__
#define __EFFECTSYSTEM_H__

#include "DataArray.h"
#include "../ConstEnums.h"
#include "../SexyAppFramework/SWTri.h"
#include "../SexyAppFramework/Graphics.h"
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

extern void TodDrawTriangle_8888_TEX1_TALPHA0_MOD0_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA0_MOD0_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA0_MOD0_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA0_MOD0_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA0_MOD1_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA0_MOD1_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA0_MOD1_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA0_MOD1_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA1_MOD0_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA1_MOD0_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA1_MOD0_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA1_MOD0_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA1_MOD1_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA1_MOD1_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA1_MOD1_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_8888_TEX1_TALPHA1_MOD1_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD0_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD0_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD0_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD0_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD1_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD1_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD1_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD1_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD0_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD0_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD0_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD0_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD1_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD1_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD1_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD1_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD0_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD0_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD0_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD0_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD1_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD1_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD1_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD1_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD0_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD0_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD0_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD0_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD1_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD1_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD1_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD1_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA0_MOD0_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA0_MOD0_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA0_MOD0_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA0_MOD0_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA0_MOD1_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA0_MOD1_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA0_MOD1_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA0_MOD1_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA1_MOD0_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA1_MOD0_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA1_MOD0_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA1_MOD0_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA1_MOD1_GLOB0_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA1_MOD1_GLOB0_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA1_MOD1_GLOB1_BLEND0(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0555_TEX1_TALPHA1_MOD1_GLOB1_BLEND1(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD0_GLOB1_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD0_GLOB1_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD1_GLOB0_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD1_GLOB0_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD1_GLOB1_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA0_MOD1_GLOB1_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD0_GLOB0_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD0_GLOB0_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD0_GLOB1_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD0_GLOB1_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD1_GLOB0_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD1_GLOB0_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD1_GLOB1_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0888_TEX1_TALPHA1_MOD1_GLOB1_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD0_GLOB1_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD0_GLOB1_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD1_GLOB0_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD1_GLOB0_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD1_GLOB1_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA0_MOD1_GLOB1_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD0_GLOB0_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD0_GLOB0_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD0_GLOB1_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD0_GLOB1_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD1_GLOB0_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD1_GLOB0_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD1_GLOB1_BLEND0_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);
extern void TodDrawTriangle_0565_TEX1_TALPHA1_MOD1_GLOB1_BLEND1_ADDITIVE(SWHelper::SWVertex* pVerts, void* pFrameBuffer, const unsigned int bytepitch, const SWHelper::SWTextureInfo* textureInfo, SWHelper::SWDiffuse& globalDiffuse);

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
