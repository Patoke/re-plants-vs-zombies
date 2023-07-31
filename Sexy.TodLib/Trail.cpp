#include "Trail.h"
#include "Definition.h"

int gTrailDefCount;  //[0x6A9F20]
TrailDefinition* gTrailDefArray;  //[0x6A9F24]
int gTrailParamArraySize;  //[0x6A9F28]
TrailParams* gTrailParamArray;  //[0x6A9F2C]

TrailParams gLawnTrailArray[(int)TrailType::NUM_TRAILS] = { //0x6A19F4
	{ TrailType::TRAIL_ICE, "particles\\IceTrail.trail" }
};

TrailDefinition::TrailDefinition()
{
	memset(this, 0, sizeof(TrailDefinition));
	mMinPointDistance = 1.0f;
	mMaxPoints = 2;
	mTrailFlags = 0U;
	mImage = nullptr;
}

TrailDefinition::~TrailDefinition()
{
}

TrailPoint::TrailPoint()
{
}

//0x51B830
bool TrailLoadADef(TrailDefinition* theTrailDef, const char* theTrailFileName)
{
	TodHesitationBracket aHesitation("Load Trail '%s'", theTrailFileName);

	if (!DefinitionLoadXML(StringToSexyString(theTrailFileName), &gTrailDefMap, theTrailDef))
		return false;

	FloatTrackSetDefault(theTrailDef->mWidthOverLength, 1.0f);
	FloatTrackSetDefault(theTrailDef->mWidthOverTime, 1.0f);
	FloatTrackSetDefault(theTrailDef->mTrailDuration, 100.0f);
	FloatTrackSetDefault(theTrailDef->mAlphaOverLength, 1.0f);
	FloatTrackSetDefault(theTrailDef->mAlphaOverTime, 1.0f);
	return true;
}

//0x51B960
void TrailLoadDefinitions(TrailParams* theTrailParamArray, int theTrailParamArraySize)
{
	TodHesitationBracket aHesitation("TrailLoadDefinitions");
	TOD_ASSERT(!gTrailParamArray && !gTrailDefArray);

	gTrailParamArraySize = theTrailParamArraySize;
	gTrailParamArray = theTrailParamArray;
	gTrailDefCount = theTrailParamArraySize;
	gTrailDefArray = new TrailDefinition[theTrailParamArraySize];

	for (int i = 0; i < gTrailParamArraySize; i++)
	{
		TrailParams* aTrailParams = &theTrailParamArray[i];
		TOD_ASSERT(aTrailParams->mTrailType == (TrailType)i);
		if (!TrailLoadADef(&gTrailDefArray[i], aTrailParams->mTrailFileName))
		{
			char aBuf[512];
			sprintf(aBuf, "Failed to load trail '%s'", aTrailParams->mTrailFileName);
			TodErrorMessageBox(aBuf, "Error");
		}
	}
}

//0x51BAB0
void TrailFreeDefinitions()
{
	for (int i = 0; i < gTrailDefCount; i++)
		DefinitionFreeMap(&gTrailDefMap, &gTrailDefArray[i]);
	delete[] gTrailDefArray;
	gTrailDefArray = nullptr;
	gTrailDefCount = 0;
	gTrailParamArray = nullptr;
	gTrailParamArraySize = 0;
}

Trail::Trail()
{
	mNumTrailPoints = 0;
	mDead = false;
	mIsAttachment = false;
	mRenderOrder = 0;
	mTrailAge = 0;
	mDefinition = nullptr;
	mTrailDuration = 0;
	mColorOverride = Color::White;
	for (int i = 0; i < 4; i++)
	{
		mTrailInterp[i] = RandRangeFloat(0.0f, 1.0f);
	}
}

//0x51BB30
void Trail::AddPoint(float x, float y)
{
	int aMaxPoints = ClampInt(mDefinition->mMaxPoints, 2, 20);

	if (mNumTrailPoints > 0)
	{
		TrailPoint& aPoint = mTrailPoints[mNumTrailPoints - 1];
		float aDistance = Distance2D(x, y, aPoint.aPos.x, aPoint.aPos.y);
		if (aDistance < mDefinition->mMinPointDistance)
		{
			return;  // 距离上次记录的轨迹点的距离不能小于规定的最小值
		}
	}
	
	// 当已有轨迹点数量达到上限时，舍弃最早的一个轨迹点
	if (mNumTrailPoints == aMaxPoints)
	{
		memmove(mTrailPoints, mTrailPoints + 1, (mNumTrailPoints - 1) * sizeof(TrailPoint));
		mNumTrailPoints--;
	}

	TrailPoint& aNewPoint = mTrailPoints[mNumTrailPoints];
	aNewPoint.aPos.x = x;
	aNewPoint.aPos.y = y;
	mNumTrailPoints++;
}

void Trail::Update()
{
	mTrailAge++;
	if (mTrailAge >= mTrailDuration)
	{
		if (TestBit(mDefinition->mTrailFlags, (int)TrailFlags::TRAIL_FLAG_LOOPS))
		{
			mTrailAge = 0;
		}
		else
		{
			mDead = true;
		}
	}
}

//0x41BC40
bool Trail::GetNormalAtPoint(int nIndex, SexyVector2& theNormal)
{
	SexyVector2 aDirection;
	if (nIndex == 0)
	{
		SexyVector2 aToNext = mTrailPoints[nIndex + 1].aPos - mTrailPoints[nIndex].aPos;
		aDirection = aToNext.Perp();
	}
	else if (nIndex == mNumTrailPoints - 1)
	{
		SexyVector2 aFromPrev = mTrailPoints[nIndex].aPos - mTrailPoints[nIndex - 1].aPos;
		aDirection = aFromPrev.Perp();
	}
	else
	{
		SexyVector2 aToNext = mTrailPoints[nIndex + 1].aPos - mTrailPoints[nIndex].aPos;
		SexyVector2 aToPrev = mTrailPoints[nIndex - 1].aPos - mTrailPoints[nIndex].aPos;
		aDirection = aToNext.Normalize() + aToPrev.Normalize();
	}

	float aMag = aDirection.Magnitude();
	if (FloatApproxEqual(aMag, 0.0f))
		return false;

	theNormal.x = aDirection.x / aMag;
	theNormal.y = aDirection.y / aMag;
	return true;
}

//0x51BD00
void Trail::Draw(Graphics* g)
{
	if (mDead || mNumTrailPoints < 2)
		return;

	float aTimeValue = mTrailAge / (float)(mTrailDuration - 1);
	int aTriangleCount = (mNumTrailPoints - 1) * 2;
	TOD_ASSERT(aTriangleCount < MAX_TRAIL_TRIANGLES);

	TriVertex aVertArray[MAX_TRAIL_TRIANGLES][3];

	bool aHavePrev = false;
	SexyVector2 aNormalPrev;
	for (int i = 0; i < mNumTrailPoints - 1; i++)
	{
		if (!aHavePrev)
		{
			if (!GetNormalAtPoint(i, aNormalPrev))
			{
				continue;
			}
			aHavePrev = true;
		}

		SexyVector2 aNormalNext;
		SexyVector2 aNormalCur = aNormalPrev;
		if (!GetNormalAtPoint(i + 1, aNormalNext))
		{
			aNormalNext = aNormalPrev;
		}
		else
		{
			aNormalPrev = aNormalNext;
		}

		TrailPoint& aPointCur = mTrailPoints[i];
		TrailPoint& aPointNext = mTrailPoints[i + 1];
		float aUCur = 1.0f - i / (float)(mNumTrailPoints - 1);
		float aUNext = 1.0f - (i + 1) / (float)(mNumTrailPoints - 1);
		float aWidthOverLengthCur = FloatTrackEvaluate(mDefinition->mWidthOverLength, aUCur, mTrailInterp[(int)TrailTracks::TRACK_WIDTH_OVER_LENGTH]);
		float aWidthOverLengthNext = FloatTrackEvaluate(mDefinition->mWidthOverLength, aUNext, mTrailInterp[(int)TrailTracks::TRACK_WIDTH_OVER_LENGTH]);
		float aWidthOverTimeCur = FloatTrackEvaluate(mDefinition->mWidthOverTime, aTimeValue, mTrailInterp[(int)TrailTracks::TRACK_WIDTH_OVER_TIME]);
		float aWidthOverTimeNext = FloatTrackEvaluate(mDefinition->mWidthOverTime, aTimeValue, mTrailInterp[(int)TrailTracks::TRACK_WIDTH_OVER_TIME]);
		float aAlphaOverLengthCur = FloatTrackEvaluate(mDefinition->mAlphaOverLength, aUCur, mTrailInterp[(int)TrailTracks::TRACK_ALPHA_OVER_LENGTH]);
		float aAlphaOverLengthNext = FloatTrackEvaluate(mDefinition->mAlphaOverLength, aUNext, mTrailInterp[(int)TrailTracks::TRACK_ALPHA_OVER_LENGTH]);
		float aAlphaOverTimeCur = FloatTrackEvaluate(mDefinition->mAlphaOverTime, aTimeValue, mTrailInterp[(int)TrailTracks::TRACK_ALPHA_OVER_TIME]);
		float aAlphaOverTimeNext = FloatTrackEvaluate(mDefinition->mAlphaOverTime, aTimeValue, mTrailInterp[(int)TrailTracks::TRACK_ALPHA_OVER_TIME]);
		int anAlphaCur = ClampInt(FloatRoundToInt(aAlphaOverLengthCur * aAlphaOverTimeCur * mColorOverride.mAlpha), 0, 255);
		int anAlphaNext = ClampInt(FloatRoundToInt(aAlphaOverLengthNext * aAlphaOverTimeNext * mColorOverride.mAlpha), 0, 255);
		Sexy::Color aColorCur = mColorOverride;
		Sexy::Color aColorNext = mColorOverride;
		aColorCur.mAlpha = anAlphaCur;
		aColorNext.mAlpha = anAlphaNext;

		SexyVector2 aPosition[4];
		aPosition[0].x = mTrailCenter.x + aPointCur.aPos.x + aNormalCur.x * aWidthOverLengthCur * aWidthOverTimeCur;
		aPosition[0].y = mTrailCenter.y + aPointCur.aPos.y + aNormalCur.y * aWidthOverLengthCur * aWidthOverTimeCur;
		aPosition[1].x = mTrailCenter.x + aPointCur.aPos.x + -aNormalCur.x * aWidthOverLengthCur * aWidthOverTimeCur;
		aPosition[1].y = mTrailCenter.y + aPointCur.aPos.y + -aNormalCur.y * aWidthOverLengthCur * aWidthOverTimeCur;
		aPosition[2].x = mTrailCenter.x + aPointNext.aPos.x + aNormalNext.x * aWidthOverLengthNext * aWidthOverTimeNext;
		aPosition[2].y = mTrailCenter.y + aPointNext.aPos.y + aNormalNext.y * aWidthOverLengthNext * aWidthOverTimeNext;
		aPosition[3].x = mTrailCenter.x + aPointNext.aPos.x + -aNormalNext.x * aWidthOverLengthNext * aWidthOverTimeNext;
		aPosition[3].y = mTrailCenter.y + aPointNext.aPos.y + -aNormalNext.y * aWidthOverLengthNext * aWidthOverTimeNext;

		int aVertCur = i * 2;
		int aVertNext = aVertCur + 1;
		aVertArray[aVertCur][0].x = aPosition[0].x;
		aVertArray[aVertCur][0].y = aPosition[0].y;
		aVertArray[aVertCur][0].u = aUCur;
		aVertArray[aVertCur][0].v = 1.0f;
		aVertArray[aVertCur][0].color = aColorCur.ToInt();
		aVertArray[aVertCur][1].x = aPosition[1].x;
		aVertArray[aVertCur][1].y = aPosition[1].y;
		aVertArray[aVertCur][1].u = aUCur;
		aVertArray[aVertCur][1].v = 0.0f;
		aVertArray[aVertCur][1].color = aColorCur.ToInt();
		aVertArray[aVertCur][2].x = aPosition[2].x;
		aVertArray[aVertCur][2].y = aPosition[2].y;
		aVertArray[aVertCur][2].u = aUNext;
		aVertArray[aVertCur][2].v = 1.0f;
		aVertArray[aVertCur][2].color = aColorNext.ToInt();
		aVertArray[aVertNext][0].x = aPosition[2].x;
		aVertArray[aVertNext][0].y = aPosition[2].y;
		aVertArray[aVertNext][0].u = aUNext;
		aVertArray[aVertNext][0].v = 1.0f;
		aVertArray[aVertNext][0].color = aColorNext.ToInt();
		aVertArray[aVertNext][1].x = aPosition[1].x;
		aVertArray[aVertNext][1].y = aPosition[1].y;
		aVertArray[aVertNext][1].u = aUCur;
		aVertArray[aVertNext][1].v = 0.0f;
		aVertArray[aVertNext][1].color = aColorCur.ToInt();
		aVertArray[aVertNext][2].x = aPosition[3].x;
		aVertArray[aVertNext][2].y = aPosition[3].y;
		aVertArray[aVertNext][2].u = aUNext;
		aVertArray[aVertNext][2].v = 0.0f;
		aVertArray[aVertNext][2].color = aColorNext.ToInt();
	}

	g->DrawTrianglesTex(mDefinition->mImage, aVertArray, aTriangleCount);
}

void TrailHolder::InitializeHolder()
{
	mTrails.DataArrayInitialize(1024U, "trails");
}

//0x4455A0
void TrailHolder::DisposeHolder()
{
	mTrails.DataArrayDispose();
}

Trail* TrailHolder::AllocTrail(int theRenderOrder, TrailType theTrailType)
{
	TOD_ASSERT(theTrailType >= 0 && theTrailType < gTrailDefCount);
	return AllocTrailFromDef(theRenderOrder, &gTrailDefArray[(int)theTrailType]);
}

Trail* TrailHolder::AllocTrailFromDef(int theRenderOrder, TrailDefinition* theDefinition)
{
	if (mTrails.mSize == mTrails.mMaxSize)
		return nullptr;

	Trail* aTrail = mTrails.DataArrayAlloc();
	aTrail->mTrailHolder = this;
	aTrail->mDefinition = theDefinition;

	float aDurationInterp = RandRangeFloat(0.0f, 1.0f);
	aTrail->mTrailDuration = (int)FloatTrackEvaluate(aTrail->mDefinition->mTrailDuration, 0.0f, aDurationInterp);
	return aTrail;
}
