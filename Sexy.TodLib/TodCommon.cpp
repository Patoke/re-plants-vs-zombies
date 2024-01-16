#include <bits/chrono.h>
#include <chrono>
#include <cstdarg>

#include "Common.h"
#include "SexyAppBase.h"
#include "TodList.h"
#include "TodDebug.h"
#include "TodCommon.h"
#include "../LawnApp.h"
#include "EffectSystem.h"
#include "../Resources.h"
#include "TodStringFile.h"
#include "../GameConstants.h"
#include "graphics/Font.h"
//#include "graphics/DDImage.h"
#include "graphics/Graphics.h"
#include "graphics/ImageFont.h"
//#include "misc/PerfTimer.h"
#include "misc/SexyMatrix.h"
//#include "graphics/DDInterface.h"
//#include "graphics/D3DInterface.h"

//0x510BC0
/*
void Tod_SWTri_AddAllDrawTriFuncs()
{
	SWTri_AddDrawTriFunc(true, false, false, false, 0x8888, false, TodDrawTriangle_8888_TEX1_TALPHA0_MOD0_GLOB0_BLEND0);	//0x4A59B0
	SWTri_AddDrawTriFunc(true, false, false, false, 0x8888, true, TodDrawTriangle_8888_TEX1_TALPHA0_MOD0_GLOB0_BLEND1);		//0x48E6F0
	SWTri_AddDrawTriFunc(true, false, false, true, 0x8888, false, TodDrawTriangle_8888_TEX1_TALPHA0_MOD0_GLOB1_BLEND0);		//0x48F9C0
	SWTri_AddDrawTriFunc(true, false, false, true, 0x8888, true, TodDrawTriangle_8888_TEX1_TALPHA0_MOD0_GLOB1_BLEND1);		//0x490920
	SWTri_AddDrawTriFunc(true, false, true, false, 0x8888, false, TodDrawTriangle_8888_TEX1_TALPHA0_MOD1_GLOB0_BLEND0);		//0x4921A0
	SWTri_AddDrawTriFunc(true, false, true, false, 0x8888, true, TodDrawTriangle_8888_TEX1_TALPHA0_MOD1_GLOB0_BLEND1);		//0x493970
	SWTri_AddDrawTriFunc(true, false, true, true, 0x8888, false, TodDrawTriangle_8888_TEX1_TALPHA0_MOD1_GLOB1_BLEND0);		//0x495BB0
	SWTri_AddDrawTriFunc(true, false, true, true, 0x8888, true, TodDrawTriangle_8888_TEX1_TALPHA0_MOD1_GLOB1_BLEND1);		//0x497480

	SWTri_AddDrawTriFunc(true, true, false, false, 0x8888, false, TodDrawTriangle_8888_TEX1_TALPHA1_MOD0_GLOB0_BLEND0);		//0x4997D0
	SWTri_AddDrawTriFunc(true, true, false, false, 0x8888, true, TodDrawTriangle_8888_TEX1_TALPHA1_MOD0_GLOB0_BLEND1);		//0x49A610
	SWTri_AddDrawTriFunc(true, true, false, true, 0x8888, false, TodDrawTriangle_8888_TEX1_TALPHA1_MOD0_GLOB1_BLEND0);		//0x49BC90
	SWTri_AddDrawTriFunc(true, true, false, true, 0x8888, true, TodDrawTriangle_8888_TEX1_TALPHA1_MOD0_GLOB1_BLEND1);		//0x49CBB0
	SWTri_AddDrawTriFunc(true, true, true, false, 0x8888, false, TodDrawTriangle_8888_TEX1_TALPHA1_MOD1_GLOB0_BLEND0);		//0x49E400
	SWTri_AddDrawTriFunc(true, true, true, false, 0x8888, true, TodDrawTriangle_8888_TEX1_TALPHA1_MOD1_GLOB0_BLEND1);		//0x49FC10
	SWTri_AddDrawTriFunc(true, true, true, true, 0x8888, false, TodDrawTriangle_8888_TEX1_TALPHA1_MOD1_GLOB1_BLEND0);		//0x4A1DF0
	SWTri_AddDrawTriFunc(true, true, true, true, 0x8888, true, TodDrawTriangle_8888_TEX1_TALPHA1_MOD1_GLOB1_BLEND1);		//0x4A3710

	SWTri_AddDrawTriFunc(true, false, false, false, 0x0888, false, TodDrawTriangle_0888_TEX1_TALPHA0_MOD0_GLOB0_BLEND0);	//0x4A59B0
	SWTri_AddDrawTriFunc(true, false, false, false, 0x0888, true, TodDrawTriangle_0888_TEX1_TALPHA0_MOD0_GLOB0_BLEND1);		//0x48E6F0
	SWTri_AddDrawTriFunc(true, false, false, true, 0x0888, false, TodDrawTriangle_0888_TEX1_TALPHA0_MOD0_GLOB1_BLEND0);		//0x4A63C0
	SWTri_AddDrawTriFunc(true, false, false, true, 0x0888, true, TodDrawTriangle_0888_TEX1_TALPHA0_MOD0_GLOB1_BLEND1);		//0x4A7D70
	SWTri_AddDrawTriFunc(true, false, true, false, 0x0888, false, TodDrawTriangle_0888_TEX1_TALPHA0_MOD1_GLOB0_BLEND0);		//0x4AAAA0
	SWTri_AddDrawTriFunc(true, false, true, false, 0x0888, true, TodDrawTriangle_0888_TEX1_TALPHA0_MOD1_GLOB0_BLEND1);		//0x4AD590
	SWTri_AddDrawTriFunc(true, false, true, true, 0x0888, false, TodDrawTriangle_0888_TEX1_TALPHA0_MOD1_GLOB1_BLEND0);		//0x4B1580
	SWTri_AddDrawTriFunc(true, false, true, true, 0x0888, true, TodDrawTriangle_0888_TEX1_TALPHA0_MOD1_GLOB1_BLEND1);		//0x4B42D0

	SWTri_AddDrawTriFunc(true, true, false, false, 0x0888, false, TodDrawTriangle_0888_TEX1_TALPHA1_MOD0_GLOB0_BLEND0);		//0x4B84F0
	SWTri_AddDrawTriFunc(true, true, false, false, 0x0888, true, TodDrawTriangle_0888_TEX1_TALPHA1_MOD0_GLOB0_BLEND1);		//0x4B9D30
	SWTri_AddDrawTriFunc(true, true, false, true, 0x0888, false, TodDrawTriangle_0888_TEX1_TALPHA1_MOD0_GLOB1_BLEND0);		//0x4BC670
	SWTri_AddDrawTriFunc(true, true, false, true, 0x0888, true, TodDrawTriangle_0888_TEX1_TALPHA1_MOD0_GLOB1_BLEND1);		//0x4BE040
	SWTri_AddDrawTriFunc(true, true, true, false, 0x0888, false, TodDrawTriangle_0888_TEX1_TALPHA1_MOD1_GLOB0_BLEND0);		//0x4C0D60
	SWTri_AddDrawTriFunc(true, true, true, false, 0x0888, true, TodDrawTriangle_0888_TEX1_TALPHA1_MOD1_GLOB0_BLEND1);		//0x4C38C0
	SWTri_AddDrawTriFunc(true, true, true, true, 0x0888, false, TodDrawTriangle_0888_TEX1_TALPHA1_MOD1_GLOB1_BLEND0);		//0x4C77C0
	SWTri_AddDrawTriFunc(true, true, true, true, 0x0888, true, TodDrawTriangle_0888_TEX1_TALPHA1_MOD1_GLOB1_BLEND1);		//0x4CA580

	SWTri_AddDrawTriFunc(true, false, false, false, 0x0565, false, TodDrawTriangle_0565_TEX1_TALPHA0_MOD0_GLOB0_BLEND0);	//0x4CE700
	SWTri_AddDrawTriFunc(true, false, false, false, 0x0565, true, TodDrawTriangle_0565_TEX1_TALPHA0_MOD0_GLOB0_BLEND1);		//0x4CF170
	SWTri_AddDrawTriFunc(true, false, false, true, 0x0565, false, TodDrawTriangle_0565_TEX1_TALPHA0_MOD0_GLOB1_BLEND0);		//0x4D0500
	SWTri_AddDrawTriFunc(true, false, false, true, 0x0565, true, TodDrawTriangle_0565_TEX1_TALPHA0_MOD0_GLOB1_BLEND1);		//0x4D20B0
	SWTri_AddDrawTriFunc(true, false, true, false, 0x0565, false, TodDrawTriangle_0565_TEX1_TALPHA0_MOD1_GLOB0_BLEND0);		//0x4D4F40
	SWTri_AddDrawTriFunc(true, false, true, false, 0x0565, true, TodDrawTriangle_0565_TEX1_TALPHA0_MOD1_GLOB0_BLEND1);		//0x4D7C20
	SWTri_AddDrawTriFunc(true, false, true, true, 0x0565, false, TodDrawTriangle_0565_TEX1_TALPHA0_MOD1_GLOB1_BLEND0);		//0x4DBDF0
	SWTri_AddDrawTriFunc(true, false, true, true, 0x0565, true, TodDrawTriangle_0565_TEX1_TALPHA0_MOD1_GLOB1_BLEND1);		//0x4DED40

	SWTri_AddDrawTriFunc(true, true, false, false, 0x0565, false, TodDrawTriangle_0565_TEX1_TALPHA1_MOD0_GLOB0_BLEND0);		//0x4E3150
	SWTri_AddDrawTriFunc(true, true, false, false, 0x0565, true, TodDrawTriangle_0565_TEX1_TALPHA1_MOD0_GLOB0_BLEND1);		//0x4E4B90
	SWTri_AddDrawTriFunc(true, true, false, true, 0x0565, false, TodDrawTriangle_0565_TEX1_TALPHA1_MOD0_GLOB1_BLEND0);		//0x4E76C0
	SWTri_AddDrawTriFunc(true, true, false, true, 0x0565, true, TodDrawTriangle_0565_TEX1_TALPHA1_MOD0_GLOB1_BLEND1);		//0x4E92B0
	SWTri_AddDrawTriFunc(true, true, true, false, 0x0565, false, TodDrawTriangle_0565_TEX1_TALPHA1_MOD1_GLOB0_BLEND0);		//0x4EC170
	SWTri_AddDrawTriFunc(true, true, true, false, 0x0565, true, TodDrawTriangle_0565_TEX1_TALPHA1_MOD1_GLOB0_BLEND1);		//0x4EEE70
	SWTri_AddDrawTriFunc(true, true, true, true, 0x0565, false, TodDrawTriangle_0565_TEX1_TALPHA1_MOD1_GLOB1_BLEND0);		//0x4F2FB0
	SWTri_AddDrawTriFunc(true, true, true, true, 0x0565, true, TodDrawTriangle_0565_TEX1_TALPHA1_MOD1_GLOB1_BLEND1);		//0x4F5F90

	SWTri_AddDrawTriFunc(true, false, false, false, 0x0555, false, TodDrawTriangle_0555_TEX1_TALPHA0_MOD0_GLOB0_BLEND0);	//0x4FA280
	SWTri_AddDrawTriFunc(true, false, false, false, 0x0555, true, TodDrawTriangle_0555_TEX1_TALPHA0_MOD0_GLOB0_BLEND1);		//0x4FACF0
	SWTri_AddDrawTriFunc(true, false, false, true, 0x0555, false, TodDrawTriangle_0555_TEX1_TALPHA0_MOD0_GLOB1_BLEND0);		//0x4FC080
	SWTri_AddDrawTriFunc(true, false, false, true, 0x0555, true, TodDrawTriangle_0555_TEX1_TALPHA0_MOD0_GLOB1_BLEND1);		//0x4FCE50
	SWTri_AddDrawTriFunc(true, false, true, false, 0x0555, false, TodDrawTriangle_0555_TEX1_TALPHA0_MOD1_GLOB0_BLEND0);		//0x4FE560
	SWTri_AddDrawTriFunc(true, false, true, false, 0x0555, true, TodDrawTriangle_0555_TEX1_TALPHA0_MOD1_GLOB0_BLEND1);		//0x4FFB80
	SWTri_AddDrawTriFunc(true, false, true, true, 0x0555, false, TodDrawTriangle_0555_TEX1_TALPHA0_MOD1_GLOB1_BLEND0);		//0x501C80
	SWTri_AddDrawTriFunc(true, false, true, true, 0x0555, true, TodDrawTriangle_0555_TEX1_TALPHA0_MOD1_GLOB1_BLEND1);		//0x503390

	SWTri_AddDrawTriFunc(true, true, false, false, 0x0555, false, TodDrawTriangle_0555_TEX1_TALPHA1_MOD0_GLOB0_BLEND0);		//0x5054D0
	SWTri_AddDrawTriFunc(true, true, false, false, 0x0555, true, TodDrawTriangle_0555_TEX1_TALPHA1_MOD0_GLOB0_BLEND1);		//0x5061E0
	SWTri_AddDrawTriFunc(true, true, false, true, 0x0555, false, TodDrawTriangle_0555_TEX1_TALPHA1_MOD0_GLOB1_BLEND0);		//0x507710
	SWTri_AddDrawTriFunc(true, true, false, true, 0x0555, true, TodDrawTriangle_0555_TEX1_TALPHA1_MOD0_GLOB1_BLEND1);		//0x5084B0
	SWTri_AddDrawTriFunc(true, true, true, false, 0x0555, false, TodDrawTriangle_0555_TEX1_TALPHA1_MOD1_GLOB0_BLEND0);		//0x509BD0
	SWTri_AddDrawTriFunc(true, true, true, false, 0x0555, true, TodDrawTriangle_0555_TEX1_TALPHA1_MOD1_GLOB0_BLEND1);		//0x50B230
	SWTri_AddDrawTriFunc(true, true, true, true, 0x0555, false, TodDrawTriangle_0555_TEX1_TALPHA1_MOD1_GLOB1_BLEND0);		//0x50D2F0
	SWTri_AddDrawTriFunc(true, true, true, true, 0x0555, true, TodDrawTriangle_0555_TEX1_TALPHA1_MOD1_GLOB1_BLEND1);		//0x50EA10
}*/

//0x5114E0
SexyString TodGetCurrentLevelName()
{
	return _S("Unknown level");
}

//0x511510
bool TodHasUsedCheatKeys()
{
	return false;
}

bool TodAppCloseRequest()
{
	return false;
}

intptr_t TodPickFromArray(const intptr_t* theArray, int theCount)
{
	TOD_ASSERT(theCount > 0);
	return theCount > 0 ? theArray[Sexy::Rand(theCount)] : 0;
}

intptr_t TodPickFromWeightedArray(const TodWeightedArray* theArray, int theCount)
{
	return TodPickArrayItemFromWeightedArray(theArray, theCount)->mItem;
}

//0x511520
TodWeightedArray* TodPickArrayItemFromWeightedArray(const TodWeightedArray* theArray, int theCount)
{
	if (theCount <= 0)
		return nullptr;

	int aTotalWeight = 0;
	for (int i = 0; i < theCount; i++)
	{
		aTotalWeight += theArray[i].mWeight;
	}
	TOD_ASSERT(aTotalWeight > 0);

	aTotalWeight = Sexy::Rand(aTotalWeight);

	for (int i = 0; i < theCount; i++)
	{
		aTotalWeight -= theArray[i].mWeight;
		if (aTotalWeight < 0)
		{
			return (TodWeightedArray*)&theArray[i];
		}
	}

	TOD_ASSERT();
	return nullptr;
}

//0x511570
TodWeightedGridArray* TodPickFromWeightedGridArray(const TodWeightedGridArray* theArray, int theCount)
{
	if (theCount <= 0)
		return nullptr;

	int aTotalWeight = 0;
	for (int i = 0; i < theCount; i++)
	{
		aTotalWeight += theArray[i].mWeight;
	}
	TOD_ASSERT(aTotalWeight > 0);

	aTotalWeight = Sexy::Rand(aTotalWeight);

	for (int i = 0; i < theCount; i++)
	{
		aTotalWeight -= theArray[i].mWeight;
		if (aTotalWeight < 0)
		{
			return (TodWeightedGridArray*)&theArray[i];
		}
	}

	TOD_ASSERT();
	return nullptr;
}

//0x5115C0
float TodCalcSmoothWeight(float aWeight, float aLastPicked, float aSecondLastPicked)
{
	if (aWeight < 1E-6f)
	{
		return 0.0f;
	}

	float aExpectedLength1 = 1.0f / aWeight;								// theLastPicked 的期望值
	float aExpectedLength2 = aExpectedLength1 * 2.0f;						// theSecondLastPicked 的期望值
	float aAdvancedLength1 = aLastPicked + 1.0f - aExpectedLength1;			// 相较于 theLastPicked 的期望值，提前的轮数
	float aAdvancedLength2 = aSecondLastPicked + 1.0f - aExpectedLength2;	// 相较于 theSecondLastPicked 的期望值，提前的轮数
	float aFactor1 = 1.0f + aAdvancedLength1 / aExpectedLength1 * 2.0f;		// = aWeight * aLastPicked * 2 + aWeight * 2 - 1
	float aFactor2 = 1.0f + aAdvancedLength2 / aExpectedLength2 * 2.0f;		// = aSecondLastPicked * aWeight + aWeight - 1
	float aFactorFinal = ClampFloat(aFactor1 * 0.75f + aFactor2 * 0.25f, 0.01f, 100.0f);
	return aWeight * aFactorFinal;
}

int TodPickFromSmoothArray(TodSmoothArray* theArray, int theCount)
{
	float aTotalWeight = 0.0f;
	for (int i = 0; i < theCount; i++)
	{
		aTotalWeight += theArray[i].mWeight;
	}
	TOD_ASSERT(aTotalWeight > 0.0f);

	float aNormalizeFactor = 1.0f / aTotalWeight;
	float aTotalAdjustedWeight = 0.0f;
	for (int j = 0; j < theCount; j++)
	{
		aTotalAdjustedWeight += TodCalcSmoothWeight(theArray[j].mWeight * aNormalizeFactor, theArray[j].mLastPicked, theArray[j].mSecondLastPicked);
	}
	TOD_ASSERT(aTotalAdjustedWeight > 0.0f);

	float aRandWeight = Rand(aTotalAdjustedWeight);
	float aAccumulatedWeight = 0.0f;
	int k;
	for (k = 0; k < theCount - 1; k++)
	{
		aAccumulatedWeight += TodCalcSmoothWeight(theArray[k].mWeight * aNormalizeFactor, theArray[k].mLastPicked, theArray[k].mSecondLastPicked);
		if (aRandWeight <= aAccumulatedWeight)
		{
			break;
		}
	}

	TodUpdateSmoothArrayPick(theArray, theCount, k);
	return theArray[k].mItem;
}

//0x5117F0
void TodUpdateSmoothArrayPick(TodSmoothArray* theArray, int theCount, int thePickIndex)
{
	for (int i = 0; i < theCount; i++)
	{
		if (theArray[i].mWeight > 0.0f)
		{
			theArray[i].mLastPicked += 1.0f;
			theArray[i].mSecondLastPicked += 1.0f;
		}
	}

	theArray[thePickIndex].mSecondLastPicked = theArray[thePickIndex].mLastPicked;
	theArray[thePickIndex].mLastPicked = 0.0f;
}

float TodCurveQuad(float theTime)
{
	return theTime * theTime;
}

float TodCurveInvQuad(float theTime)
{
	return 2 * theTime - theTime * theTime;
}

//0x5118C0
float TodCurveS(float theTime)
{
	return 3 * theTime * theTime - 2 * theTime * theTime * theTime;
}

//0x5118F0
float TodCurveInvQuadS(float theTime)
{
	//float aVal = 2 * (theTime - theTime * theTime);
	//return theTime <= 0.5 ? aVal : 1 - aVal;
	if (theTime <= 0.5f)
	{
		return TodCurveInvQuad(theTime * 2.0f) * 0.5f;
	}
	return TodCurveQuad((theTime - 0.5f) * 2.0f) * 0.5f + 0.5f;
}

//0x511970
float TodCurveBounce(float theTime)
{
	return 1 - fabs(2 * theTime - 1);
}

float TodCurveQuadS(float theTime)
{
	if (theTime <= 0.5f)
	{
		return TodCurveQuad(theTime * 2.0f) * 0.5f;
	}
	return TodCurveInvQuad((theTime - 0.5f) * 2.0f) * 0.5f + 0.5f;
}

float TodCurveCubic(float theTime)
{
	return theTime * theTime * theTime;
}

float TodCurveInvCubic(float theTime)
{
	return (theTime - 1.0f) * (theTime - 1.0f) * (theTime - 1.0f) + 1.0f;
}

float TodCurveCubicS(float theTime)
{
	if (theTime <= 0.5f)
	{
		return TodCurveCubic(theTime * 2.0f) * 0.5f;
	}
	return TodCurveInvCubic((theTime - 0.5f) * 2.0f) * 0.5f + 0.5f;
}

float TodCurvePoly(float theTime, float thePoly)
{
	return (float)pow(theTime, thePoly);
}

float TodCurveInvPoly(float theTime, float thePoly)
{
	return (float)pow(theTime - 1.0f, thePoly) + 1.0f;
}

float TodCurvePolyS(float theTime, float thePoly)
{
	if (theTime <= 0.5f)
	{
		return TodCurvePoly(theTime * 2.0f, thePoly) * 0.5f;
	}
	return TodCurveInvPoly((theTime - 0.5f) * 2.0f, thePoly) * 0.5f + 0.5f;
}

float TodCurveCircle(float theTime)
{
	if (theTime > 1 - 1E-6f)
	{
		return 1.0f;
	}
	return 1.0f - (float)sqrt(1.0f - theTime * theTime);
}

float TodCurveInvCircle(float theTime)
{
	if (theTime < 1E-06f)
	{
		return 0.0f;
	}
	return (float)sqrt(1.0f - (theTime - 1.0f) * (theTime - 1.0f));
}

//0x5119B0
float TodCurveEvaluate(float theTime, float thePositionStart, float thePositionEnd, TodCurves theCurve)
{
	float aWarpedTime = 0;
	switch (theCurve)
	{
	case TodCurves::CURVE_CONSTANT:				aWarpedTime = 0;													break;
	case TodCurves::CURVE_LINEAR:				aWarpedTime = theTime;												break;
	case TodCurves::CURVE_EASE_IN:				aWarpedTime = TodCurveQuad(theTime);								break;
	case TodCurves::CURVE_EASE_OUT:				aWarpedTime = TodCurveInvQuad(theTime);								break;
	case TodCurves::CURVE_EASE_IN_OUT:			aWarpedTime = TodCurveS(TodCurveS(theTime));						break;
	case TodCurves::CURVE_EASE_IN_OUT_WEAK:		aWarpedTime = TodCurveS(theTime);									break;
	case TodCurves::CURVE_FAST_IN_OUT:			aWarpedTime = TodCurveInvQuadS(TodCurveInvQuadS(theTime));			break;
	case TodCurves::CURVE_FAST_IN_OUT_WEAK:		aWarpedTime = TodCurveInvQuadS(theTime);							break;
	case TodCurves::CURVE_BOUNCE:				aWarpedTime = TodCurveBounce(theTime);								break;
	case TodCurves::CURVE_BOUNCE_FAST_MIDDLE:	aWarpedTime = TodCurveQuad(TodCurveBounce(theTime));				break;
	case TodCurves::CURVE_BOUNCE_SLOW_MIDDLE:	aWarpedTime = TodCurveInvQuad(TodCurveBounce(theTime));				break;
	case TodCurves::CURVE_SIN_WAVE:				aWarpedTime = sinf(2 * PI * theTime);								break;
	case TodCurves::CURVE_EASE_SIN_WAVE:		aWarpedTime = sinf(2 * PI * TodCurveS(theTime));					break;
	default:									TOD_ASSERT();														break;
	}
	return (thePositionEnd - thePositionStart) * aWarpedTime + thePositionStart;
}

//0x511B30
float TodCurveEvaluateClamped(float theTime, float thePositionStart, float thePositionEnd, TodCurves theCurve)
{
	if (theTime <= 0.0f)
	{
		return thePositionStart;
	}

	if (theTime >= 1.0f)
	{
		if (theCurve == TodCurves::CURVE_BOUNCE ||
			theCurve == TodCurves::CURVE_BOUNCE_FAST_MIDDLE ||
			theCurve == TodCurves::CURVE_BOUNCE_SLOW_MIDDLE ||
			theCurve == TodCurves::CURVE_SIN_WAVE ||
			theCurve == TodCurves::CURVE_EASE_SIN_WAVE)
		{
			return thePositionStart;
		}
		else
		{
			return thePositionEnd;
		}
	}

	return TodCurveEvaluate(theTime, thePositionStart, thePositionEnd, theCurve);
}

//0x511BA0
float TodAnimateCurveFloatTime(float theTimeStart, float theTimeEnd, float theTimeAge, float thePositionStart, float thePositionEnd, TodCurves theCurve)
{
	float aWarpedAge = (theTimeAge - theTimeStart) / (theTimeEnd - theTimeStart);
	return TodCurveEvaluateClamped(aWarpedAge, thePositionStart, thePositionEnd, theCurve);
}

//0x511BF0
float TodAnimateCurveFloat(int theTimeStart, int theTimeEnd, int theTimeAge, float thePositionStart, float thePositionEnd, TodCurves theCurve)
{
	//return TodAnimateCurveFloatTime(theTimeStart, theTimeEnd, theTimeAge, thePositionStart, thePositionEnd, theCurve);

	float aWarpedAge = (theTimeAge - theTimeStart) / (float)(theTimeEnd - theTimeStart);
	return TodCurveEvaluateClamped(aWarpedAge, thePositionStart, thePositionEnd, theCurve);
}

//0x511C40
// GOTY @Patoke: 0x51BEA0
int TodAnimateCurve(int theTimeStart, int theTimeEnd, int theTimeAge, int thePositionStart, int thePositionEnd, TodCurves theCurve)
{
	return FloatRoundToInt(TodAnimateCurveFloat(theTimeStart, theTimeEnd, theTimeAge, thePositionStart, thePositionEnd, theCurve));
}

int RandRangeInt(int theMin, int theMax)
{
	TOD_ASSERT(theMin <= theMax);
	return Rand(theMax - theMin + 1) + theMin;
}

//0x511CB0
float RandRangeFloat(float theMin, float theMax)
{
	TOD_ASSERT(theMin <= theMax);
	return Rand(theMax - theMin) + theMin;
}

//0x511CE0
void TodDrawString(Graphics* g, const SexyString& theText, int thePosX, int thePosY, _Font* theFont, const Color& theColor, DrawStringJustification theJustification)
{
	SexyString aFinalString = TodStringTranslate(theText);

	int aPosX = thePosX;
	if (theJustification == DrawStringJustification::DS_ALIGN_RIGHT || theJustification == DrawStringJustification::DS_ALIGN_RIGHT_VERTICAL_MIDDLE)
	{
		aPosX -= theFont->StringWidth(aFinalString);
	}
	else if (theJustification == DrawStringJustification::DS_ALIGN_CENTER || theJustification == DrawStringJustification::DS_ALIGN_CENTER_VERTICAL_MIDDLE)
	{
		aPosX -= theFont->StringWidth(aFinalString) / 2;
	}

	theFont->DrawString(g, aPosX, thePosY, aFinalString, theColor, g->mClipRect);
}

//0x511D90
void TodDrawImageCelScaled(Graphics* g, Image* theImageStrip, int thePosX, int thePosY, int theCelCol, int theCelRow, float theScaleX, float theScaleY)
{
	TOD_ASSERT(theCelCol >= 0 && theCelCol < theImageStrip->mNumCols);
	TOD_ASSERT(theCelRow >= 0 && theCelRow < theImageStrip->mNumRows);

	int aCelWidth = theImageStrip->GetCelWidth();
	int aCelHeight = theImageStrip->GetCelHeight();
	Rect aSrcRect(aCelWidth * theCelCol, aCelHeight * theCelRow, aCelWidth, aCelHeight);
	Rect aDestRect(thePosX, thePosY, FloatRoundToInt(aCelWidth * theScaleX), FloatRoundToInt(aCelHeight * theScaleY));
	g->DrawImage(theImageStrip, aSrcRect, aDestRect);
}

/*
static const int POOL_SIZE = 4096;
static RenderCommand gRenderCommandPool[POOL_SIZE];
static RenderCommand* gRenderTail[256];
static RenderCommand* gRenderHead[256];
*/
//0x511E50
void TodDrawStringMatrix(Graphics* g, const _Font* theFont, const SexyMatrix3& theMatrix, const SexyString& theString, const Color& theColor)
{
	std::multimap<int, RenderCommand>aRenderCommandPool;
	SexyString aFinalString = TodStringTranslate(theString);

	/*
	memset(gRenderTail, 0, sizeof(gRenderTail));
	memset(gRenderHead, 0, sizeof(gRenderHead));*/
	ImageFont* aFont = (ImageFont*)theFont;
	if (!aFont->mFontData->mInitialized)
		return;

	aFont->Prepare();
	int aCurXPos = 0;
	for (int aCharNum = 0; aCharNum < (int)aFinalString.size(); aCharNum++)
	{
		SexyChar aChar = aFont->GetMappedChar(aFinalString[aCharNum]);
		SexyChar aNextChar = '\0';
		if (aCharNum < (int)aFinalString.size() - 1)
		{
			aNextChar = aFont->GetMappedChar(aFinalString[aCharNum + 1]);
		}

		int aMaxXPos = aCurXPos;
		for (auto aKernItr = aFont->mActiveLayerList.begin(); aKernItr != aFont->mActiveLayerList.end(); aKernItr++)
		{
			FontLayer* aLayer = aKernItr->mBaseFontLayer;
			CharData* aCharData = aLayer->GetCharData(aChar);
			double aScale = aFont->mScale;
			int aLayerPointSize = aLayer->mPointSize;
			if (aLayerPointSize)
			{
				aScale *= (float)aFont->mPointSize / (float)aLayerPointSize;
			}

			int anImageX, anImageY, aCharWidth, aSpacing;
			if (aScale == 1.0f)
			{
				anImageX = aCharData->mOffset.mX + aLayer->mOffset.mX + aCurXPos;
				anImageY = aCharData->mOffset.mY + aLayer->mOffset.mY - aLayer->mAscent;
				aCharWidth = aCharData->mWidth;

				if (aNextChar == '\0')
				{
					aSpacing = 0;
				}
				else
				{
					aSpacing = aLayer->mSpacing;

					//aSpacing += aCharData->mKerningOffsets[aNextChar];
					auto anItr = aCharData->mKerningOffsets.find(aNextChar);
					if (anItr != aCharData->mKerningOffsets.end())
					{
						aSpacing += anItr->second;
					}
				}
			}
			else
			{
				anImageX = aCurXPos + floor((aCharData->mOffset.mX + aLayer->mOffset.mX) * aScale);
				anImageY = -floor((aLayer->mAscent - aLayer->mOffset.mY - aCharData->mOffset.mY) * aScale);
				aCharWidth = aCharData->mWidth * aScale;

				if (aNextChar == '\0')
				{
					aSpacing = 0;
				}
				else
				{
					aSpacing = aLayer->mSpacing;

					//aSpacing += aCharData->mKerningOffsets[aNextChar] * aScale;
					auto anItr = aCharData->mKerningOffsets.find(aNextChar);
					if (anItr != aCharData->mKerningOffsets.end())
					{
						aSpacing += anItr->second * aScale;
					}
				}
			}

			Color aColor;
			aColor.mRed = std::min(aLayer->mColorAdd.mRed + theColor.mRed * aLayer->mColorMult.mRed / 255, 255);
			aColor.mGreen = std::min(aLayer->mColorAdd.mGreen + theColor.mGreen * aLayer->mColorMult.mGreen / 255, 255);
			aColor.mBlue = std::min(aLayer->mColorAdd.mBlue + theColor.mBlue * aLayer->mColorMult.mBlue / 255, 255);
			aColor.mAlpha = std::min(aLayer->mColorAdd.mAlpha + theColor.mAlpha * aLayer->mColorMult.mAlpha / 255, 255);
			int anOrder = aCharData->mOrder + aLayer->mBaseOrder;

			int anOrderIdx = std::min(std::max(anOrder + 128, 0), 255);
			aRenderCommandPool.insert(std::pair<int, RenderCommand>(anOrderIdx, {
				.mImage = aKernItr->mScaledImage,
				.mDest = {anImageX, anImageY},
				.mSrc = aKernItr->mScaledCharImageRects.find(aChar)->second,
				.mMode = aLayer->mDrawMode,
				.mColor = aColor,
				.mUseAlphaCorrection = aLayer->mUseAlphaCorrection,
				.mNext = nullptr
			}));

			if (aMaxXPos < aCurXPos + aSpacing + aCharWidth)
			{
				aMaxXPos = aCurXPos + aSpacing + aCharWidth;
			}
		}

		aCurXPos = aMaxXPos;
	}

	for (auto aRenderCommand : aRenderCommandPool) {
		RenderCommand &cmd = aRenderCommand.second;

		int aDrawMode = cmd.mMode != -1 ? cmd.mMode : g->GetDrawMode();
		if (cmd.mImage) {
			SexyTransform2D aTransform;
			float aPosX = cmd.mSrc.mWidth * 0.5f + cmd.mDest[0];
			float aPosY = cmd.mSrc.mHeight * 0.5f + cmd.mDest[1];
			SexyMatrix3Translation(aTransform, aPosX, aPosY);
			SexyMatrix3Multiply(aTransform, theMatrix, aTransform);
			TodBltMatrix(g, cmd.mImage, aTransform, g->mClipRect, cmd.mColor, aDrawMode, cmd.mSrc);
		}
	}
}

//0x512570
// GOTY @Patoke: 0x51C863
void TodDrawImageCelF(Graphics* g, Image* theImageStrip, float thePosX, float thePosY, int theCelCol, int theCelRow)
{
	TOD_ASSERT(theCelCol >= 0 && theCelCol < theImageStrip->mNumCols);
	TOD_ASSERT(theCelRow >= 0 && theCelRow < theImageStrip->mNumRows);

	int aCelWidth = theImageStrip->GetCelWidth();
	int aCelHeight = theImageStrip->GetCelHeight();
	Rect theSrcRect(aCelWidth * theCelCol, aCelHeight * theCelRow, aCelWidth, aCelHeight);
	g->DrawImageF(theImageStrip, thePosX, thePosY, theSrcRect);
}

void SexyMatrix3Translation(SexyMatrix3& m, float x, float y)
{
	m.m02 += x;
	m.m12 += y;
}

void TodScaleTransformMatrix(SexyMatrix3& m, float x, float y, float theScaleX, float theScaleY)
{
	m.m00 = theScaleX;
	m.m10 = 0.0f;
	m.m20 = 0.0f;
	m.m01 = 0.0f;
	m.m11 = theScaleY;
	m.m21 = 0.0f;
	m.m02 = x;
	m.m12 = y;
	m.m22 = 1.0f;
}

//0x5125D0
void TodScaleRotateTransformMatrix(SexyMatrix3& m, float x, float y, float rad, float theScaleX, float theScaleY)
{
	m.m00 = cos(rad) * theScaleX;
	m.m10 = -sin(rad) * theScaleX;
	m.m20 = 0.0f;
	m.m01 = sin(rad) * theScaleY;
	m.m11 = cos(rad) * theScaleY;
	m.m21 = 0.0f;
	m.m02 = x;
	m.m12 = y;
	m.m22 = 1.0f;
}

void SexyMatrix3ExtractScale(const SexyMatrix3& m, float& theScaleX, float& theScaleY)
{
	float kx = atan2(m.m00, m.m10);
	if (abs(kx) < PI / 4 || abs(kx) > 4 * PI / 3)
	{
		theScaleX = m.m10 / cos(kx);
	}
	else
	{
		theScaleX = m.m00 / sin(kx);
	}

	float ky = atan2(m.m11, m.m01);
	if (abs(ky) < PI / 4 || abs(ky) > 4 * PI / 3)
	{
		theScaleY = m.m01 / cos(ky);
	}
	else
	{
		theScaleY = m.m11 / sin(ky);
	}
}

void TodMarkImageForSanding(Image* /*theImage*/)
{
	static bool madeWarning = false;
	if(!madeWarning) printf("warning:  Image Sanding is Skipped\n");
	madeWarning = true;
	//unreachable();
	//((MemoryImage*)theImage)->mD3DFlags |= D3DIMAGEFLAG_SANDING;
}

void TodSandImageIfNeeded(Image* /*theImage*/)
{
	static bool madeWarning = false;
	if(!madeWarning) printf("warning:  Tried to sand Image but it didn't exist!\n");
	madeWarning = true;
	/* TODO
	MemoryImage* aImage = (MemoryImage*)theImage;
	if (aImage->mD3DFlags & D3DIMAGEFLAG_SANDING)
	{
		FixPixelsOnAlphaEdgeForBlending(theImage);
		((MemoryImage*)theImage)->mD3DFlags &= ~D3DIMAGEFLAG_SANDING; // Unset the sanding flag
	}*/
}

//0x512650
void TodBltMatrix(Graphics* g, Image* theImage, const SexyMatrix3& theTransform, const Rect& theClipRect, const Color& theColor, int theDrawMode, const Rect& theSrcRect)
{
	//float aOffsetX = 0.0f;
	//float aOffsetY = 0.0f;
	//if (gSexyAppBase->Is3DAccelerated())
	//{
	//aOffsetX -= 0.5f;
	//aOffsetY -= 0.5f;
	//}
	/*
	else if (theDrawMode == Graphics::DRAWMODE_ADDITIVE)
	{
		gTodTriangleDrawAdditive = true;
	}*/

	TodSandImageIfNeeded(theImage);

	g->mDestImage->BltMatrix(theImage, 0.0f, 0.0f, theTransform, theClipRect, theColor, theDrawMode, theSrcRect, g->mLinearBlend);

	/*if (theClipRect.mX != 0 || theClipRect.mY != 0 || theClipRect.mWidth != BOARD_WIDTH || theClipRect.mHeight != BOARD_HEIGHT)
	{
		
	}
	else if (DDImage::Check3D(g->mDestImage))
	{
		theImage->mDrawn = true;
		D3DInterface* aInterface = ((DDImage*)g->mDestImage)->mDDInterface->mD3DInterface;
		aInterface->BltTransformed(theImage, nullptr, theColor, theDrawMode, theSrcRect, theTransform, g->mLinearBlend, aOffsetX, aOffsetY, true);
	}
	else
	{
		Rect aBufFixClipRect(0, 0, BOARD_WIDTH + 1, BOARD_HEIGHT + 1);
		g->mDestImage->BltMatrix(theImage, aOffsetX, aOffsetY, theTransform, aBufFixClipRect, theColor, theDrawMode, theSrcRect, g->mLinearBlend);
	}*/

	//gTodTriangleDrawAdditive = false;
}

//0x5127C0
void TodDrawImageCelCenterScaledF(Graphics* g, Image* theImageStrip, float thePosX, float thePosY, int theCelCol, float theScaleX, float theScaleY)
{
	TOD_ASSERT(theCelCol >= 0 && theCelCol < theImageStrip->mNumCols);

	int aCelWidth = theImageStrip->GetCelWidth();
	int aCelHeight = theImageStrip->GetCelHeight();
	Rect aSrcRect(aCelWidth * theCelCol, 0, aCelWidth, aCelHeight);
	if (theScaleX == 1.0f && theScaleY == 1.0f)
	{
		g->DrawImageF(theImageStrip, thePosX, thePosY, aSrcRect);
		return;
	}

	float aTransX = aCelWidth * 0.5f + thePosX + g->mTransX;
	float aTransY = aCelHeight * 0.5f + thePosY + g->mTransY;

	SexyMatrix3 aTransform;
	aTransform.m00 = theScaleX;
	aTransform.m10 = 0.0f;
	aTransform.m20 = 0.0f;
	aTransform.m01 = 0.0f;
	aTransform.m11 = theScaleY;
	aTransform.m21 = 0.0f;
	aTransform.m02 = aTransX;
	aTransform.m12 = aTransY;
	aTransform.m22 = 1.0f;

	const Color& aColor = g->mColorizeImages ? g->mColor : Color::White;
	TodBltMatrix(g, theImageStrip, aTransform, g->mClipRect, aColor, g->mDrawMode, aSrcRect);
}

//0x512880
void TodDrawImageCelScaledF(Graphics* g, Image* theImageStrip, float thePosX, float thePosY, int theCelCol, int theCelRow, float theScaleX, float theScaleY)
{
	(void)theCelRow;
	TOD_ASSERT(theCelCol >= 0 && theCelCol < theImageStrip->mNumCols);

	int aCelWidth = theImageStrip->GetCelWidth();
	int aCelHeight = theImageStrip->GetCelHeight();
	Rect aSrcRect(aCelWidth * theCelCol, 0, aCelWidth, aCelHeight);
	if (theScaleX == 1.0f && theScaleY == 1.0f)
	{
		g->DrawImageF(theImageStrip, thePosX, thePosY, aSrcRect);
		return;
	}

	float aTransX = aCelWidth * 0.5f * theScaleX + thePosX + g->mTransX;
	float aTransY = aCelHeight * 0.5f * theScaleY + thePosY + g->mTransY;

	SexyMatrix3 aTransform;
	aTransform.m00 = theScaleX;
	aTransform.m10 = 0.0f;
	aTransform.m20 = 0.0f;
	aTransform.m01 = 0.0f;
	aTransform.m11 = theScaleY;
	aTransform.m21 = 0.0f;
	aTransform.m02 = aTransX;
	aTransform.m12 = aTransY;
	aTransform.m22 = 1.0f;

	const Color& aColor = g->mColorizeImages ? g->mColor : Color::White;
	TodBltMatrix(g, theImageStrip, aTransform, g->mClipRect, aColor, g->mDrawMode, aSrcRect);
}

//0x512950
// GOTY @Patoke: 0x51CC90
void TodDrawImageScaledF(Graphics* g, Image* theImage, float thePosX, float thePosY, float theScaleX, float theScaleY)
{
	if (theScaleX == 1.0f && theScaleY == 1.0f)
	{
		g->DrawImageF(theImage, thePosX, thePosY);
		return;
	}

	Rect aSrcRect(0, 0, theImage->mWidth, theImage->mHeight);
	float aTransX = theImage->mWidth * 0.5f * theScaleX + thePosX + g->mTransX;
	float aTransY = theImage->mHeight * 0.5f * theScaleY + thePosY + g->mTransY;

	SexyMatrix3 aTransform;
	aTransform.m00 = theScaleX;
	aTransform.m10 = 0.0f;
	aTransform.m20 = 0.0f;
	aTransform.m01 = 0.0f;
	aTransform.m11 = theScaleY;
	aTransform.m21 = 0.0f;
	aTransform.m02 = aTransX;
	aTransform.m12 = aTransY;
	aTransform.m22 = 1.0f;

	const Color& aColor = g->mColorizeImages ? g->mColor : Color::White;
	TodBltMatrix(g, theImage, aTransform, g->mClipRect, aColor, g->mDrawMode, aSrcRect);
}

//0x512A10
void TodDrawImageCenterScaledF(Graphics* g, Image* theImage, float thePosX, float thePosY, float theScaleX, float theScaleY)
{
	if (theScaleX == 1.0f && theScaleY == 1.0f)
	{
		g->DrawImageF(theImage, thePosX, thePosY);
		return;
	}

	Rect aSrcRect(0, 0, theImage->mWidth, theImage->mHeight);
	float aTransX = theImage->mWidth * 0.5f + thePosX + g->mTransX;
	float aTransY = theImage->mHeight * 0.5f + thePosY + g->mTransY;

	SexyMatrix3 aTransform;
	aTransform.m00 = theScaleX;
	aTransform.m10 = 0.0f;
	aTransform.m20 = 0.0f;
	aTransform.m01 = 0.0f;
	aTransform.m11 = theScaleY;
	aTransform.m21 = 0.0f;
	aTransform.m02 = aTransX;
	aTransform.m12 = aTransY;
	aTransform.m22 = 1.0f;

	const Color& aColor = g->mColorizeImages ? g->mColor : Color::White;
	TodBltMatrix(g, theImage, aTransform, g->mClipRect, aColor, g->mDrawMode, aSrcRect);
}

//0x512AC0
/*
uint32_t AverageNearByPixels(MemoryImage* theImage, uint32_t* thePixel, int x, int y)
{
	int aRed = 0;
	int aGreen = 0;
	int aBlue = 0;
	int aBitsCount = 0;

	for (int i = -1; i <= 1; i++)  // 依次循环上方、当前、下方的一行
	{
		if (i == 0)  // 排除当前行
		{
			continue;
		}

		for (int j = -1; j <= 1; j++)  // 依次循环左方、当前、右方的一列
		{
			if ((x != 0 || j != -1) && (x != theImage->mWidth - 1 || j != 1) && (y != 0 || i != -1) && (y != theImage->mHeight - 1 || i != 1))
			{
				unsigned long aPixel = *(thePixel + i * theImage->mWidth + j);
				if (aPixel & 0xFF000000UL)  // 如果不是透明像素
				{
					aRed += (aPixel >> 16) & 0x000000FFUL;
					aGreen += (aPixel >> 8) & 0x000000FFUL;
					aBlue += aPixel & 0x000000FFUL;
					aBitsCount++;
				}
			}
		}
	}

	if (aBitsCount == 0)
		return 0;

	aRed /= aBitsCount;
	aRed = std::min(aRed, 255);
	aGreen /= aBitsCount;
	aGreen = std::min(aGreen, 255);
	aBlue /= aBitsCount;
	aBlue = std::min(aBlue, 255);
	return (aRed << 16) | (aGreen << 8) | (aBlue);
}*/

//0x512C60

void FixPixelsOnAlphaEdgeForBlending(Image* /*theImage*/)
{
	// TODO
	/*
	MemoryImage* aImage = (MemoryImage*)theImage;
	if (aImage->mBits == nullptr)
		return;

	aImage->CommitBits();  // 分析 mHasTrans 和 mHasAlpha
	if (!aImage->mHasTrans)
		return;

	auto aTimer = std::chrono::high_resolution_clock::now();


	uint32_t* aBitsPtr = aImage->mBits;
	for (int y = 0; y < theImage->mHeight; y++)
	{
		for (int x = 0; x < theImage->mWidth; x++)
		{
			if ((*aBitsPtr & 0xFF000000UL) == 0)  // 如果像素的不透明度为 0
			{
				*aBitsPtr = AverageNearByPixels(aImage, aBitsPtr, x, y);  // 计算该点周围非透明像素的平均颜色
			}

			aBitsPtr++;
		}
	}
	aImage->mBitsChangedCount++;

	int aDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - aTimer).count();
	if (aDuration > 20)
	{
		TodTraceAndLog("LOADING:Long sanding '%s' %d ms on %s", theImage->mFilePath.c_str(), aDuration, gGetCurrentLevelName().c_str());
	}*/
}

void SexyMatrix3Transpose(const SexyMatrix3& m, SexyMatrix3 &r)
{
	SexyMatrix3 temp;
	temp.m00 = m.m00;
	temp.m01 = m.m10;
	temp.m02 = m.m20;
	temp.m10 = m.m01;
	temp.m11 = m.m11;
	temp.m12 = m.m21;
	temp.m20 = m.m02;
	temp.m21 = m.m12;
	temp.m22 = m.m22;
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			r.m[i][j] = temp.m[i][j];
		}
	}
}

//0x512D00
void SexyMatrix3Inverse(const SexyMatrix3& m, SexyMatrix3 &r)
{
	float aDet = (m.m22 * m.m11 - m.m21 * m.m12) * m.m00- (m.m22 * m.m10 - m.m20 * m.m12) * m.m01 + (m.m21 * m.m10 - m.m20 * m.m11) * m.m02;
	float aInvDet = 1.0f / aDet;

	SexyMatrix3 temp;
	temp.m00 = (m.m22 * m.m11 - m.m21 * m.m12) * aInvDet;
	temp.m01 = (m.m02 * m.m21 - m.m22 * m.m01) * aInvDet;
	temp.m02 = (m.m12 * m.m01 - m.m02 * m.m11) * aInvDet;
	temp.m10 = (m.m20 * m.m12 - m.m22 * m.m10) * aInvDet;
	temp.m11 = (m.m00 * m.m22 - m.m02 * m.m20) * aInvDet;
	temp.m12 = (m.m02 * m.m10 - m.m12 * m.m00) * aInvDet;
	temp.m20 = (m.m21 * m.m10 - m.m20 * m.m11) * aInvDet;
	temp.m21 = (m.m20 * m.m01 - m.m21 * m.m00) * aInvDet;
	temp.m22 = (m.m00 * m.m11 - m.m10 * m.m01) * aInvDet;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			r.m[i][j] = temp.m[i][j];
		}
	}
}

//0x512E20
void SexyMatrix3Multiply(SexyMatrix3& m, const SexyMatrix3& l, const SexyMatrix3& r)
{
	//SexyMatrix3 temp = l * r;
	SexyMatrix3 temp;
	temp.m00 = l.m00 * r.m00 + l.m01 * r.m10 + l.m02 * r.m20;
	temp.m01 = l.m00 * r.m01 + l.m01 * r.m11 + l.m02 * r.m21;
	temp.m02 = l.m00 * r.m02 + l.m01 * r.m12 + l.m02 * r.m22;
	temp.m10 = l.m10 * r.m00 + l.m11 * r.m10 + l.m12 * r.m20;
	temp.m11 = l.m10 * r.m01 + l.m11 * r.m11 + l.m12 * r.m21;
	temp.m12 = l.m10 * r.m02 + l.m11 * r.m12 + l.m12 * r.m22;
	temp.m20 = l.m20 * r.m00 + l.m21 * r.m10 + l.m22 * r.m20;
	temp.m21 = l.m20 * r.m01 + l.m21 * r.m11 + l.m22 * r.m21;
	temp.m22 = l.m20 * r.m02 + l.m21 * r.m12 + l.m22 * r.m22;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m.m[i][j] = temp.m[i][j];
		}
	}
}

//0x512F20
// GOTY @Patoke: 0x51D2C0
Color GetFlashingColor(int theCounter, int theFlashTime)
{
	int aTimeAge = theCounter % theFlashTime;
	int aTimeInf = theFlashTime / 2;
	//int aTimeDel = abs(aTimeInf - aTimeAge) / aTimeInf;
	// @Patoke: order wasn't like in binaries
	int aGrayness = ClampInt(200 * abs(aTimeInf - aTimeAge) / aTimeInf + 55, 0, 255);
	//int aGrayness = ClampInt(55 + 200 * abs(aTimeInf - aTimeAge)/ aTimeInf, 0, 255);
	return Color(aGrayness, aGrayness, aGrayness, 255);
}

//0x512F80
Color ColorAdd(const Color& theColor1, const Color& theColor2)
{
	int r = theColor1.mRed + theColor2.mRed;
	int g = theColor1.mGreen + theColor2.mGreen;
	int b = theColor1.mBlue + theColor2.mBlue;
	int a = theColor1.mAlpha + theColor2.mAlpha;

	return Color(ClampInt(r, 0, 255), ClampInt(g, 0, 255), ClampInt(b, 0, 255), ClampInt(a, 0, 255));  // 线性减淡
}

//0x513020
// GOTY @Patoke: 0x51D3C0
int ColorComponentMultiply(int theColor1, int theColor2)
{
	return ClampInt(theColor1 * theColor2 / 255, 0, 255);  // 正片叠底
}

//0x513050
Color ColorsMultiply(const Color& theColor1, const Color& theColor2)
{
	return Color(
		ColorComponentMultiply(theColor1.mRed, theColor2.mRed), 
		ColorComponentMultiply(theColor1.mGreen, theColor2.mGreen),
		ColorComponentMultiply(theColor1.mBlue, theColor2.mBlue),
		ColorComponentMultiply(theColor1.mAlpha, theColor2.mAlpha)
	);  // 正片叠底
}

//0x513120
// GOTY @Patoke: inlined 0x51D4C0
bool TodLoadResources(const std::string& theGroup)
{
	// It's fixed now.. but still, wtf
	/* For everything that is mighty, this is so not okay. This code promotes the base class
	 * ResourceManager to the derived class TodResourceManager and just yolo raw dogs it
	 * and hopes for the best. It does work but Address sanitizer is having an aneurism.
	 * Honestly I don't even know what the best way to do this is. Probably implement a
	 * constructor in TodResourceManager and use that instead?
	 */
	return ((TodResourceManager*)gSexyAppBase->mResourceManager)->TodLoadResources(theGroup);
}

//0x513140
// GOTY @Patoke: 0x51D4C0
bool TodResourceManager::TodLoadResources(const std::string& theGroup)
{
	if (IsGroupLoaded(theGroup))
		return true;

	auto aTimer = std::chrono::high_resolution_clock::now();

	StartLoadResources(theGroup);
	while (!gSexyAppBase->mShutdown && TodLoadNextResource());
	if (gSexyAppBase->mShutdown)
		return false;

	if (HadError())
	{
		gSexyAppBase->ShowResourceError(true);
		return false;
	}

	if (gExtractResourcesByName && !gExtractResourcesByName(this, theGroup.c_str()))
	{
		gSexyAppBase->ShowResourceError(true);
		return false;
	}

	mLoadedGroups.insert(theGroup);

	int aDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - aTimer).count();
	if (aDuration > 20)
	{
		TodTraceAndLog("LOADED: '%s' %d ms on %s", theGroup.c_str(), aDuration, gGetCurrentLevelName().c_str());
	}

	return true;
}

void TodAddImageToMap(Image* theImage, const std::string& thePath)
{
	((TodResourceManager*)gSexyAppBase->mResourceManager)->AddImageToMap(theImage, thePath);
}

//0x513230
void TodResourceManager::AddImageToMap(Image* theImage, const std::string& thePath)
{
	TOD_ASSERT(mImageMap.find(thePath) == mImageMap.end());

	ImageRes* aImageRes = new ImageRes();
	aImageRes->mImage = theImage;
	aImageRes->mPath = thePath;
	mImageMap.insert(ResMap::value_type(thePath, aImageRes));
}

bool TodLoadNextResource()
{
	return ((TodResourceManager*)gSexyAppBase->mResourceManager)->TodLoadNextResource();
}

//0x513330
bool TodResourceManager::TodLoadNextResource()
{
	//GetTickCount();
	TodHesitationTrace("preres");

	while (mCurResGroupListItr != mCurResGroupList->end())
	{
		BaseRes* aRes = *mCurResGroupListItr;
		if (aRes->mFromProgram)
			continue;

		switch (aRes->mType)
		{
		case ResType_Image:
		{
			ImageRes* anImageRes = (ImageRes*)aRes;
			if (anImageRes->mImage != nullptr)
			{
				mCurResGroupListItr++;
				continue;
			}

			break;
		}

		case ResType_Sound:
		{
			SoundRes* aSoundRes = (SoundRes*)aRes;
			if (aSoundRes->mSoundId != -1)
			{
				mCurResGroupListItr++;
				continue;
			}

			break;
		}

		case ResType_Font:
		{
			FontRes* aFontRes = (FontRes*)aRes;
			if (aFontRes->mFont != nullptr)
			{
				mCurResGroupListItr++;
				continue;
			}

			break;
		}
		}

		if (!LoadNextResource())
			break;

		if (aRes->mType == ResType::ResType_Image)
		{
			ImageRes* anImageRes = (ImageRes*)aRes;
			Image* aImage = (Image*)anImageRes->mImage;
			if (aImage != nullptr)
			{
				TodMarkImageForSanding(aImage);
			}
		}

		//GetTickCount();
		TodHesitationTrace("Loading: '%s'", aRes->mPath.c_str());
		TodHesitationTrace("resource '%s'", aRes->mPath.c_str());
		return true;
	}

	return false;
}

bool TodFindImagePath(Image* theImage, std::string* thePath)
{
	return ((TodResourceManager*)gSexyAppBase->mResourceManager)->FindImagePath(theImage, thePath);
}

// @Patoke implemented
bool TodFindFontPath(_Font* theFont, std::string* thePath) {
	return ((TodResourceManager*)gSexyAppBase->mResourceManager)->FindFontPath(theFont, thePath);
}

bool TodResourceManager::FindFontPath(_Font* theFont, std::string* thePath)
{
	for (auto anItr = mFontMap.begin(); anItr != mFontMap.end(); anItr++)
	{
		FontRes* aFontRes = (FontRes*)anItr->second;
		_Font* aFont = aFontRes->mFont;
		if (aFont == theFont)
		{
			*thePath = anItr->first;
			return true;
		}
	}
	return false;
}

bool TodResourceManager::FindImagePath(Image* theImage, std::string* thePath)
{
	for (auto anItr = mImageMap.begin(); anItr != mImageMap.end(); anItr++)
	{
		ImageRes* aImageRes = (ImageRes*)anItr->second;
		Image* aImage = (Image*)aImageRes->mImage;
		if (aImage == theImage)
		{
			*thePath = anItr->first;
			return true;
		}
	}
	return false;
}

TodAllocator gGlobalAllocators[MAX_GLOBAL_ALLOCATORS];  //0x6A7B68
int gNumGlobalAllocators = 0;  //[0x6A9EFC]

//0x513570
TodAllocator* FindGlobalAllocator(int theSize)
{
	for (int i = 0; i < gNumGlobalAllocators; i++)
	{
		if (gGlobalAllocators[i].mItemSize == theSize)
		{
			return &gGlobalAllocators[i];
		}
	}

	TOD_ASSERT(gNumGlobalAllocators < MAX_GLOBAL_ALLOCATORS - 1);

	TodAllocator* pAllocator = &gGlobalAllocators[gNumGlobalAllocators++];
	pAllocator->Initialize(16, theSize);
	return pAllocator;
}

//0x513600
void FreeGlobalAllocators()
{
	for (int i = 0; i < gNumGlobalAllocators; i++)
	{
		gGlobalAllocators[i].FreeAll();
	}

	gNumGlobalAllocators = 0;
}

//0x513660
SexyString TodReplaceString(const SexyString& theText, const SexyChar* theStringToFind, const SexyString& theStringToSubstitute)
{
	SexyString aFinalString = TodStringTranslate(theText);
	size_t aPos = aFinalString.find(theStringToFind);
	if (aPos != SexyString::npos)
	{
		SexyString aFinalStringToSubstitute = TodStringTranslate(theStringToSubstitute);
		aFinalString.replace(aPos, strlen(theStringToFind), aFinalStringToSubstitute);
	}

	return aFinalString;
}

//0x513720
SexyString TodReplaceNumberString(const SexyString& theText, const SexyChar* theStringToFind, int theNumber)
{
	SexyString aFinalString = TodStringTranslate(theText);
	size_t aPos = aFinalString.find(theStringToFind);
	if (aPos != SexyString::npos)
	{
		SexyString aNumberString = StrFormat(_S("%d"), theNumber);
		aFinalString.replace(aPos, strlen(theStringToFind), aNumberString);
	}

	return aFinalString;
}

//0x5137F0
// GOTY @Patoke: 0x51DB00
bool TodIsPointInPolygon(const SexyVector2* thePolygonPoint, int theNumberPolygonPoints, const SexyVector2& theCheckPoint)
{
	TOD_ASSERT(theNumberPolygonPoints >= 3);

	for (int i = 0; i < theNumberPolygonPoints; i++)
	{
		const SexyVector2& cur = thePolygonPoint[i];
		const SexyVector2& nex = thePolygonPoint[i == theNumberPolygonPoints - 1 ? 0 : i + 1];

		SexyVector2 u = (nex - cur).Perp();
		SexyVector2 v = theCheckPoint - cur;
		if (u.Dot(v) < 0)
			return false;
	}
	return true;
}

int TodVsnprintf(char* theBuffer, int theSize, const char* theFormat, va_list theArgList)
{
	try
	{
		int aCount = vsnprintf(theBuffer, theSize, theFormat, theArgList);
		if (aCount == -1)
		{
			theBuffer[theSize - 1] = '\0';
			aCount = theSize - 1;
		}
		return aCount;
	}
	catch (std::exception&)
	{
		TOD_ASSERT(, "bad format string");
		return 1;
	}
}

int TodSnprintf(char* theBuffer, int theSize, const char* theFormat, ...)
{
	va_list argList;
	va_start(argList, theFormat);
	int aCount = TodVsnprintf(theBuffer, theSize, theFormat, argList);
	va_end(argList);

	return aCount;
}
