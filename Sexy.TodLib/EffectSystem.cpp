#include "Common.h"
#include "Trail.h"
#include "TodDebug.h"
#include "Attachment.h"
#include "Reanimator.h"
#include "TodParticle.h"
#include "EffectSystem.h"
#include "../GameConstants.h"
//#include "graphics/DDImage.h"
//#include "graphics/DDInterface.h"
//#include "graphics/D3DInterface.h"

EffectSystem* gEffectSystem = nullptr;  //[0x6A9EB8]

//0x445330
void EffectSystem::EffectSystemInitialize()
{
	TOD_ASSERT(!gEffectSystem);
	TOD_ASSERT(!mParticleHolder && !mTrailHolder && !mReanimationHolder && !mAttachmentHolder);

	gEffectSystem = this;
	mParticleHolder = new TodParticleHolder();
	mTrailHolder = new TrailHolder();
	mReanimationHolder = new ReanimationHolder();
	mAttachmentHolder = new AttachmentHolder();

	mParticleHolder->InitializeHolder();
	mTrailHolder->InitializeHolder();
	mReanimationHolder->InitializeHolder();
	mAttachmentHolder->InitializeHolder();
}

//0x445490
void EffectSystem::EffectSystemDispose()
{
	if (mParticleHolder)
	{
		mParticleHolder->DisposeHolder();
		delete mParticleHolder;
		mParticleHolder = nullptr;
	}
	if (mTrailHolder)
	{
		mTrailHolder->DisposeHolder(); // DisposeHolder is called from the destructor!
		delete mTrailHolder;
		mTrailHolder = nullptr;
	}
	if (mReanimationHolder)
	{
		mReanimationHolder->DisposeHolder();
		delete mReanimationHolder;
		mReanimationHolder = nullptr;
	}
	if (mAttachmentHolder)
	{
		mAttachmentHolder->DisposeHolder();
		delete mAttachmentHolder;
		mAttachmentHolder = nullptr;
	}

	gEffectSystem = nullptr;
}

//0x4455E0
void EffectSystem::EffectSystemFreeAll()
{
	mParticleHolder->mParticleSystems.DataArrayFreeAll();
	mParticleHolder->mEmitters.DataArrayFreeAll();
	mParticleHolder->mParticles.DataArrayFreeAll();
	mParticleHolder->mEmitterListNodeAllocator.FreeAll();
	mParticleHolder->mParticleListNodeAllocator.FreeAll();

	mTrailHolder->mTrails.DataArrayFreeAll();
	mReanimationHolder->mReanimations.DataArrayFreeAll();
	mAttachmentHolder->mAttachments.DataArrayFreeAll();
}

//0x445680
void EffectSystem::ProcessDeleteQueue()
{
	TodParticleSystem* aParticle = nullptr;
	while (mParticleHolder->mParticleSystems.IterateNext(aParticle))
		if (aParticle->mDead)
			mParticleHolder->mParticleSystems.DataArrayFree(aParticle);

	Trail* aTrail = nullptr;
	while (mTrailHolder->mTrails.IterateNext(aTrail))
		if (aTrail->mDead)
			mTrailHolder->mTrails.DataArrayFree(aTrail);

	Reanimation* aReanim = nullptr;
	while (mReanimationHolder->mReanimations.IterateNext(aReanim))
		if (aReanim->mDead)
			mReanimationHolder->mReanimations.DataArrayFree(aReanim);

	Attachment* aAttachment = nullptr;
	while (mAttachmentHolder->mAttachments.IterateNext(aAttachment))
		if (aAttachment->mDead)
			mAttachmentHolder->mAttachments.DataArrayFree(aAttachment);
}

//0x445890
void EffectSystem::Update()
{
	TodParticleSystem* aParticle = nullptr;
	while (mParticleHolder->mParticleSystems.IterateNext(aParticle))
		if (!aParticle->mIsAttachment)
			aParticle->Update();

	Trail* aTrail = nullptr;
	while (mTrailHolder->mTrails.IterateNext(aTrail))
		if (!aTrail->mIsAttachment)
			aTrail->Update();

	Reanimation* aReanim = nullptr;
	while (mReanimationHolder->mReanimations.IterateNext(aReanim))
		if (!aReanim->mIsAttachment)
			aReanim->Update();
}

// #################################################################################################### //
// #################################################################################################### //
// #################################################################################################### //

static TriVertex		gTodVertexReservoir[64];
static unsigned int		gTodVertexReservoirUsed = 0;

static int FixedFloor(int x)
{
	if (x > 0)
		return x & 0xFFFF0000;
	else
		return (x & 0xFFFF0000) - 0x10000;
}

//0x4459B0
static inline void Tod_Tod_lClip(TriVertex& dst, const TriVertex& on, const TriVertex& off, const float edge)
{
	float delta = (edge - off.x) / (on.x - off.x);
	dst.x = off.x + (on.x - off.x) * delta;
	dst.y = off.y + (on.y - off.y) * delta;
	dst.u = off.u + (on.u - off.u) * delta;
	dst.v = off.v + (on.v - off.v) * delta;
	dst.color =
		((int)((off.color >> 24 & 0xff) + ((on.color >> 24 & 0xff) - (off.color >> 24 & 0xff)) * delta) << 24) |
		((int)((off.color >> 16 & 0xff) + ((on.color >> 16 & 0xff) - (off.color >> 16 & 0xff)) * delta) << 16) |
		((int)((off.color >>  8 & 0xff) + ((on.color >>  8 & 0xff) - (off.color >>  8 & 0xff)) * delta) << 8) |
		((int)((off.color       & 0xff) + ((on.color       & 0xff) - (off.color       & 0xff)) * delta));
}
static inline void rClip(TriVertex& dst, const TriVertex& on, const TriVertex& off, const float edge)
{
	float delta = (edge - off.x) / (on.x - off.x);
	dst.x = off.x + (on.x - off.x) * delta;
	dst.y = off.y + (on.y - off.y) * delta;
	dst.u = off.u + (on.u - off.u) * delta;
	dst.v = off.v + (on.v - off.v) * delta;
	dst.color =
		((int)((off.color >> 24 & 0xff) + ((on.color >> 24 & 0xff) - (off.color >> 24 & 0xff)) * delta) << 24) |
		((int)((off.color >> 16 & 0xff) + ((on.color >> 16 & 0xff) - (off.color >> 16 & 0xff)) * delta) << 16) |
		((int)((off.color >>  8 & 0xff) + ((on.color >>  8 & 0xff) - (off.color >>  8 & 0xff)) * delta) << 8) |
		((int)((off.color       & 0xff) + ((on.color       & 0xff) - (off.color       & 0xff)) * delta));
}

//0x445B50
static inline void Tod_tClip(TriVertex& dst, const TriVertex& on, const TriVertex& off, const float edge)
{
	float delta = (edge - off.y) / (on.y - off.y);
	dst.x = off.x + (on.x - off.x) * delta;
	dst.y = off.y + (on.y - off.y) * delta;
	dst.u = off.u + (on.u - off.u) * delta;
	dst.v = off.v + (on.v - off.v) * delta;
	dst.color = 
		((int)((off.color >> 24 & 0xff) + ((on.color >> 24 & 0xff) - (off.color >> 24  & 0xff)) * delta) << 24) |
		((int)((off.color >> 16 & 0xff) + ((on.color >> 16 & 0xff) - (off.color >> 16  & 0xff)) * delta) << 16) |
		((int)((off.color >>  8 & 0xff) + ((on.color >>  8 & 0xff) - (off.color >>  8  & 0xff)) * delta) <<  8) |
		((int)((off.color       & 0xff) + ((on.color       & 0xff) - (off.color        & 0xff)) * delta));
}
static inline void Tod_bClip(TriVertex& dst, const TriVertex& on, const TriVertex& off, const float edge)
{
	float delta = (edge - off.y) / (on.y - off.y);
	dst.x = off.x + (on.x - off.x) * delta;
	dst.y = off.y + (on.y - off.y) * delta;
	dst.u = off.u + (on.u - off.u) * delta;
	dst.v = off.v + (on.v - off.v) * delta;
	dst.color = 
		((int)((off.color >> 24 & 0xff) + ((on.color >> 24 & 0xff) - (off.color >> 24  & 0xff)) * delta) << 24) |
		((int)((off.color >> 16 & 0xff) + ((on.color >> 16 & 0xff) - (off.color >> 16  & 0xff)) * delta) << 16) |
		((int)((off.color >>  8 & 0xff) + ((on.color >>  8 & 0xff) - (off.color >>  8  & 0xff)) * delta) <<  8) |
		((int)((off.color       & 0xff) + ((on.color       & 0xff) - (off.color        & 0xff)) * delta));
}

//0x445D00
static inline unsigned int Tod_leClip(TriVertex** src, TriVertex** dst, const float edge)
{
	TriVertex** _dst = dst;

	for (TriVertex** v = src; *v; ++v)
	{
		TriVertex* cur = *v;
		TriVertex* nex = *(v + 1) ? *(v + 1) : *src;

		switch ((cur->x < edge ? 1 : 0) | (nex->x < edge ? 2 : 0))
		{
		case 0:
			*dst = *v;
			++dst;
			break;
		case 1:
		{
			TriVertex& tmp = gTodVertexReservoir[gTodVertexReservoirUsed++];
			Tod_Tod_lClip(tmp, *nex, *cur, edge);
			*dst = &tmp;
			++dst;
			break;
		}
		case 2:
		{
			*dst = *v;
			++dst;
			TriVertex& tmp = gTodVertexReservoir[gTodVertexReservoirUsed++];
			Tod_Tod_lClip(tmp, *cur, *nex, edge);
			*dst = &tmp;
			++dst;
			break;
		}
		}
	}
	*dst = 0;
	return static_cast<int>(dst - _dst);
}

//0x445E00
static inline unsigned int Tod_reClip(TriVertex** src, TriVertex** dst, const float edge)
{
	TriVertex** _dst = dst;

	for (TriVertex** v = src; *v; ++v)
	{
		TriVertex* cur = *v;
		TriVertex* nex = *(v + 1) ? *(v + 1) : *src;

		switch ((cur->x > edge ? 1 : 0) | (nex->x > edge ? 2 : 0))
		{
		case 0:
			*dst = *v;
			++dst;
			break;
		case 1:
		{
			TriVertex& tmp = gTodVertexReservoir[gTodVertexReservoirUsed++];
			rClip(tmp, *nex, *cur, edge);
			*dst = &tmp;
			++dst;
			break;
		}
		case 2:
		{
			*dst = *v;
			++dst;
			TriVertex& tmp = gTodVertexReservoir[gTodVertexReservoirUsed++];
			rClip(tmp, *cur, *nex, edge);
			*dst = &tmp;
			++dst;
			break;
		}
		}
	}
	*dst = 0;
	return static_cast<int>(dst - _dst);
}

//0x445F00
static inline unsigned int Tod_teClip(TriVertex** src, TriVertex** dst, const float edge)
{
	TriVertex** _dst = dst;

	for (TriVertex** v = src; *v; ++v)
	{
		TriVertex* cur = *v;
		TriVertex* nex = *(v + 1) ? *(v + 1) : *src;

		switch ((cur->y < edge ? 1 : 0) | (nex->y < edge ? 2 : 0))
		{
		case 0:
			*dst = *v;
			++dst;
			break;
		case 1:
		{
			TriVertex& tmp = gTodVertexReservoir[gTodVertexReservoirUsed++];
			Tod_tClip(tmp, *nex, *cur, edge);
			*dst = &tmp;
			++dst;
			break;
		}
		case 2:
		{
			*dst = *v;
			++dst;
			TriVertex& tmp = gTodVertexReservoir[gTodVertexReservoirUsed++];
			Tod_tClip(tmp, *cur, *nex, edge);
			*dst = &tmp;
			++dst;
			break;
		}
		}
	}
	*dst = 0;
	return static_cast<int>(dst - _dst);
}

//0x446000
static inline unsigned int Tod_beClip(TriVertex** src, TriVertex** dst, const float edge)
{
	TriVertex** _dst = dst;

	for (TriVertex** v = src; *v; ++v)
	{
		TriVertex* cur = *v;
		TriVertex* nex = *(v + 1) ? *(v + 1) : *src;

		switch ((cur->y > edge ? 1 : 0) | (nex->y > edge ? 2 : 0))
		{
		case 0:
			*dst = *v;
			++dst;
			break;
		case 1:
		{
			TriVertex& tmp = gTodVertexReservoir[gTodVertexReservoirUsed++];
			Tod_bClip(tmp, *nex, *cur, edge);
			*dst = &tmp;
			++dst;
			break;
		}
		case 2:
		{
			*dst = *v;
			++dst;
			TriVertex& tmp = gTodVertexReservoir[gTodVertexReservoirUsed++];
			Tod_bClip(tmp, *cur, *nex, edge);
			*dst = &tmp;
			++dst;
			break;
		}
		}
	}
	*dst = 0;
	return static_cast<int>(dst - _dst);
}

//0x446100
static inline int Tod_clipShape(TriVertex** dst, TriVertex* src, const float left, const float right, const float top, const float bottom)
{
	gTodVertexReservoirUsed = 0;

	TriVertex* buf[64];
	TriVertex* ptr[4];
	ptr[0] = src;
	ptr[1] = src + 1;
	ptr[2] = src + 2;
	ptr[3] = 0;

	if (Tod_leClip(ptr, buf, left) < 3)
		return 0;
	if (Tod_reClip(buf, dst, right) < 3)
		return 0;
	if (Tod_teClip(dst, buf, top) < 3)
		return 0;
	return Tod_beClip(buf, dst, bottom);
}

bool gTodTriangleDrawAdditive = false;

#include "TodDrawTriangleInc.cpp"

//0x4461B0
TodTriangleGroup::TodTriangleGroup()
{
	for (int i = 0; i < 256; i++)
		for (int j = 0; j < 3; j++)
			mVertArray[i][j].color = 0UL;

	mImage = nullptr;
	mTriangleCount = 0;
	mDrawMode = Graphics::DRAWMODE_NORMAL;
}

//0x4461F0
void TodTriangleGroup::DrawGroup(Graphics* /*g*/)
{
	if (mImage && mTriangleCount)
	{
		/*
		// @Patoke: do we want this? if 3D acceleration is off then blending is messed up
		if (!gSexyAppBase->Is3DAccelerated() && mDrawMode == Graphics::DRAWMODE_ADDITIVE)
			gTodTriangleDrawAdditive = true;*/
		TodSandImageIfNeeded(mImage);

		unreachable();
		/* FIXME
		if (DDImage::Check3D(g->mDestImage))
		{
			DDImage* anImage = (DDImage*)g->mDestImage;
			mImage->mDrawn = true;
			anImage->mDDInterface->mD3DInterface->DrawTrianglesTex(mVertArray, mTriangleCount, Color::White, mDrawMode, mImage, 0.0f, 0.0f, g->mLinearBlend);
		}
		else
		{
			g->mDestImage->BltTrianglesTex(mImage, mVertArray, mTriangleCount, Rect(0, 0, BOARD_WIDTH, BOARD_HEIGHT), Color::White, mDrawMode, 0.0f, 0.0f, g->mLinearBlend);
		}*/

		mTriangleCount = 0;
		gTodTriangleDrawAdditive = false;
	}
}

//0x446300
void TodTriangleGroup::AddTriangle(Graphics* g, Image* theImage, const SexyMatrix3& theMatrix, const Rect& theClipRect, const Color& theColor, int theDrawMode, const Rect& theSrcRect)
{
	TOD_ASSERT(theImage != nullptr);

	if (mTriangleCount > 0 && (mDrawMode != theDrawMode || mImage != theImage))
		DrawGroup(g);
	mImage = theImage;
	mDrawMode = theDrawMode;

	SexyVector2 p[4];
	float x = -theSrcRect.mWidth * 0.5f;
	float y = -theSrcRect.mHeight * 0.5f;
	p[0].x = x;
	p[0].y = y;
	p[1].x = x;
	p[1].y = y + theSrcRect.mHeight;
	p[2].x = x + theSrcRect.mWidth;
	p[2].y = y;
	p[3].x = x + theSrcRect.mWidth;
	p[3].y = y + theSrcRect.mHeight;
	SexyVector2 tp[4];
	for (int i = 0; i < 4; i++)
	{
		tp[i] = theMatrix * p[i];
		tp[i].x -= 0.5f;
		tp[i].y -= 0.5f;
	}

	TOD_ASSERT(theSrcRect.mX >= 0 && theSrcRect.mWidth >= 0 && theSrcRect.mX + theSrcRect.mWidth <= mImage->mWidth);
	TOD_ASSERT(theSrcRect.mY >= 0 && theSrcRect.mHeight >= 0 && theSrcRect.mY + theSrcRect.mHeight <= mImage->mHeight);

	float aOneOverWidth = 1.0f / mImage->mWidth;
	float aOneOverHeight = 1.0f / mImage->mHeight;
	float aTexX1 = aOneOverWidth * theSrcRect.mX;
	float aTexY1 = aOneOverHeight * theSrcRect.mY;
	float aTexX2 = aOneOverWidth * (theSrcRect.mX + theSrcRect.mWidth);
	float aTexY2 = aOneOverHeight * (theSrcRect.mY + theSrcRect.mHeight);
	unsigned int aTexColor = theColor.ToInt();

	bool aNoClipping = false;
	TriVertex aVertBuffer[2][3];
	TriVertex (*aTriRef)[3] = aVertBuffer;
	if (mTriangleCount + 2 <= MAX_TRIANGLES)
	{
		if ((
				theClipRect.mX == 0 && theClipRect.mY == 0 && theClipRect.mWidth == BOARD_WIDTH && theClipRect.mHeight == BOARD_HEIGHT
			) || (
				theClipRect.mX <= tp[0].x && theClipRect.mX + theClipRect.mWidth >= tp[0].x && 
				theClipRect.mX <= tp[1].x && theClipRect.mX + theClipRect.mWidth >= tp[1].x &&
				theClipRect.mX <= tp[2].x && theClipRect.mX + theClipRect.mWidth >= tp[2].x && 
				theClipRect.mX <= tp[3].x && theClipRect.mX + theClipRect.mWidth >= tp[3].x &&
				theClipRect.mY <= tp[0].y && theClipRect.mY + theClipRect.mHeight >= tp[0].y && 
				theClipRect.mY <= tp[1].y && theClipRect.mY + theClipRect.mHeight >= tp[1].y &&
				theClipRect.mY <= tp[2].y && theClipRect.mY + theClipRect.mHeight >= tp[2].y && 
				theClipRect.mY <= tp[3].y && theClipRect.mY + theClipRect.mHeight >= tp[3].y
			))
		{
			aNoClipping = true;
			aTriRef = &mVertArray[mTriangleCount];
			mTriangleCount += 2;
		}
	}

	aTriRef[0][0].x = tp[0].x;
	aTriRef[0][0].y = tp[0].y;
	aTriRef[0][0].u = aTexX1;
	aTriRef[0][0].v = aTexY1;
	aTriRef[0][0].color = aTexColor;
	aTriRef[0][1].x = tp[1].x;
	aTriRef[0][1].y = tp[1].y;
	aTriRef[0][1].u = aTexX1;
	aTriRef[0][1].v = aTexY2;
	aTriRef[0][1].color = aTexColor;
	aTriRef[0][2].x = tp[2].x;
	aTriRef[0][2].y = tp[2].y;
	aTriRef[0][2].u = aTexX2;
	aTriRef[0][2].v = aTexY1;
	aTriRef[0][2].color = aTexColor;
	aTriRef[1][0].x = tp[2].x;
	aTriRef[1][0].y = tp[2].y;
	aTriRef[1][0].u = aTexX2;
	aTriRef[1][0].v = aTexY1;
	aTriRef[1][0].color = aTexColor;
	aTriRef[1][1].x = tp[1].x;
	aTriRef[1][1].y = tp[1].y;
	aTriRef[1][1].u = aTexX1;
	aTriRef[1][1].v = aTexY2;
	aTriRef[1][1].color = aTexColor;
	aTriRef[1][2].x = tp[3].x;
	aTriRef[1][2].y = tp[3].y;
	aTriRef[1][2].u = aTexX2;
	aTriRef[1][2].v = aTexY2;
	aTriRef[1][2].color = aTexColor;

	if (aNoClipping)
	{
		if (mTriangleCount == MAX_TRIANGLES)
			DrawGroup(g);
	}
	else
	{
		TriVertex* clipped[64];
		float clipX0 = theClipRect.mX;
		float clipY0 = theClipRect.mY;
		float clipX1 = theClipRect.mX + theClipRect.mWidth;
		float clipY1 = theClipRect.mY + theClipRect.mHeight;
		
		for (int i = 0; i < 2; i++)
		{
			int vCount = Tod_clipShape(clipped, aTriRef[i], clipX0, clipX1, clipY0, clipY1);
			for (int j = 0; j < vCount - 2; j++)
			{
				TriVertex* pVert = mVertArray[mTriangleCount];
				pVert[0].x = clipped[0]->x;
				pVert[0].y = clipped[0]->y;
				pVert[0].u = clipped[0]->u;
				pVert[0].v = clipped[0]->v;
				pVert[0].color = clipped[0]->color;
				pVert[1].x = clipped[j + 1]->x;
				pVert[1].y = clipped[j + 1]->y;
				pVert[1].u = clipped[j + 1]->u;
				pVert[1].v = clipped[j + 1]->v;
				pVert[1].color = clipped[j + 1]->color;
				pVert[2].x = clipped[j + 2]->x;
				pVert[2].y = clipped[j + 2]->y;
				pVert[2].u = clipped[j + 2]->u;
				pVert[2].v = clipped[j + 2]->v;
				pVert[2].color = clipped[j + 2]->color;
				if (++mTriangleCount == MAX_TRIANGLES)
					DrawGroup(g);
			}
		}
	}
}

