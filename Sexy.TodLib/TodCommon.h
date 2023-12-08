#pragma once
#include <stdlib.h>
#include <cmath>
#include <cfloat>
#include "../Lawn/LawnCommon.h"
#include "../SexyAppFramework/Common.h"
#include "misc/ResourceManager.h"

struct TodAllocator;
namespace Sexy
{
	class Graphics;
	class SexyMatrix;
	class SexyVector2;
};
//using namespace std;
using namespace Sexy;

#define D3DIMAGEFLAG_SANDING 0x1000
#define DEG_TO_RAD(deg) ((deg) * 0.017453292f)
#define RAD_TO_DEG(rad) ((rad) * 57.29578f)

// #################################################################################################### //

struct TodWeightedArray
{
    intptr_t mItem;
	int mWeight;
};

struct TodWeightedGridArray
{
	int mX;
	int mY;
	int mWeight;
};

class TodSmoothArray
{
public:
	int			mItem;
	float		mWeight;
	float		mLastPicked;
	float		mSecondLastPicked;
};

/*inline*/ intptr_t		TodPickFromArray(const intptr_t* theArray, int theCount);
intptr_t				TodPickFromWeightedArray(const TodWeightedArray* theArray, int theCount);
TodWeightedArray*		TodPickArrayItemFromWeightedArray(const TodWeightedArray* theArray, int theCount);
TodWeightedGridArray*	TodPickFromWeightedGridArray(const TodWeightedGridArray* theArray, int theCount);
float					TodCalcSmoothWeight(float aWeight, float aLastPicked, float aSecondLastPicked);
void					TodUpdateSmoothArrayPick(TodSmoothArray* theArray, int theCount, int thePickIndex);
int						TodPickFromSmoothArray(TodSmoothArray* theArray, int theCount);

// #################################################################################################### //

class TodResourceManager : public ResourceManager
{
public:
	bool				FindImagePath(Image* theImage, std::string* thePath);
	void				AddImageToMap(SharedImageRef* theImage, const std::string& thePath);
	bool				TodLoadNextResource();
	bool				TodLoadResources(const std::string& theGroup);
};

/*inline*/ bool			TodLoadResources(const std::string& theGroup);
/*inline*/ bool			TodLoadNextResource();
void					TodAddImageToMap(SharedImageRef* theImage, const std::string& thePath);
bool					TodFindImagePath(Image* theImage, std::string* thePath);
bool					TodFindFontPath(_Font* theFont, std::string* thePath);

// #################################################################################################### //

/*inline*/ float		TodCurveQuad(float theTime);
/*inline*/ float		TodCurveInvQuad(float theTime);
/*inline*/ float		TodCurveS(float theTime);
/*inline*/ float		TodCurveInvQuadS(float theTime);
/*inline*/ float		TodCurveBounce(float theTime);
float					TodCurveQuadS(float theTime);
float					TodCurveCubic(float theTime);
float					TodCurveInvCubic(float theTime);
float					TodCurveCubicS(float theTime);
float					TodCurvePoly(float theTime, float thePoly);
float					TodCurveInvPoly(float theTime, float thePoly);
float					TodCurvePolyS(float theTime, float thePoly);
float					TodCurveCircle(float theTime);
float					TodCurveInvCircle(float theTime);
float					TodCurveEvaluate(float theTime, float thePositionStart, float thePositionEnd, TodCurves theCurve);
float					TodCurveEvaluateClamped(float theTime, float thePositionStart, float thePositionEnd, TodCurves theCurve);
float					TodAnimateCurveFloatTime(float theTimeStart, float theTimeEnd, float theTimeAge, float thePositionStart, float thePositionEnd, TodCurves theCurve);
float					TodAnimateCurveFloat(int theTimeStart, int theTimeEnd, int theTimeAge, float thePositionStart, float thePositionEnd, TodCurves theCurve);
int						TodAnimateCurve(int theTimeStart, int theTimeEnd, int theTimeAge, int thePositionStart, int thePositionEnd, TodCurves theCurve);

/*inline*/ void			TodScaleTransformMatrix(SexyMatrix3& m, float x, float y, float theScaleX, float theScaleY);
void					TodScaleRotateTransformMatrix(SexyMatrix3& m, float x, float y, float rad, float theScaleX, float theScaleY);
void					SexyMatrix3ExtractScale(const SexyMatrix3& m, float& theScaleX, float& theScaleY);
/*inline*/ void			SexyMatrix3Translation(SexyMatrix3& m, float x, float y);
void					SexyMatrix3Transpose(const SexyMatrix3& m, SexyMatrix3& r);  // r = m ^ T
void					SexyMatrix3Inverse(const SexyMatrix3& m, SexyMatrix3& r);  // r = m ^ -1
void					SexyMatrix3Multiply(SexyMatrix3& m, const SexyMatrix3& l, const SexyMatrix3& r);  // m = l × r
bool					TodIsPointInPolygon(const SexyVector2* thePolygonPoint, int theNumberPolygonPoints, const SexyVector2& theCheckPoint);

void					TodDrawString(Graphics* g, const SexyString& theText, int thePosX, int thePosY, _Font* theFont, const Color& theColor, DrawStringJustification theJustification);
void					TodDrawStringMatrix(Graphics* g, const _Font* theFont, const SexyMatrix3& theMatrix, const SexyString& theString, const Color& theColor);
void					TodDrawImageScaledF(Graphics* g, Image* theImage, float thePosX, float thePosY, float theScaleX, float theScaleY);
void					TodDrawImageCenterScaledF(Graphics* g, Image* theImage, float thePosX, float thePosY, float theScaleX, float theScaleY);
void					TodDrawImageCelF(Graphics* g, Image* theImageStrip, float thePosX, float thePosY, int theCelCol, int theCelRow);
void					TodDrawImageCelScaled(Graphics* g, Image* theImageStrip, int thePosX, int thePosY, int theCelCol, int theCelRow, float theScaleX, float theScaleY);
void					TodDrawImageCelScaledF(Graphics* g, Image* theImageStrip, float thePosX, float thePosY, int theCelCol, int theCelRow, float theScaleX, float theScaleY);
void					TodDrawImageCelCenterScaledF(Graphics* g, Image* theImageStrip, float thePosX, float thePosY, int theCelCol, float theScaleX, float theScaleY);
void					TodBltMatrix(Graphics* g, Image* theImage, const SexyMatrix3& theTransform, const Rect& theClipRect, const Color& theColor, int theDrawMode, const Rect& theSrcRect);
void					TodMarkImageForSanding(Image* theImage);
void					TodSandImageIfNeeded(Image* theImage);
void					FixPixelsOnAlphaEdgeForBlending(Image* theImage);
unsigned long			AverageNearByPixels(MemoryImage* theImage, unsigned long* thePixel, int x, int y);
void					Tod_SWTri_AddAllDrawTriFuncs();

SexyString				TodReplaceString(const SexyString& theText, const SexyChar* theStringToFind, const SexyString& theStringToSubstitute);
SexyString				TodReplaceNumberString(const SexyString& theText, const SexyChar* theStringToFind, int theNumber);
int						TodSnprintf(char* theBuffer, int theSize, const char* theFormat, ...);
int						TodVsnprintf(char* theBuffer, int theSize, const char* theFormat, va_list theArgList);

TodAllocator*			FindGlobalAllocator(int theSize);
void                    FreeGlobalAllocators();

SexyString				TodGetCurrentLevelName();
bool					TodHasUsedCheatKeys();
bool					TodAppCloseRequest();

//====================================================================================================//
/*inline*/ int			RandRangeInt(int theMin, int theMax);
/*inline*/ float		RandRangeFloat(float theMin, float theMax);
inline char				ClampByte(char theNum, char theMin, char theMax)			{ return theNum <= theMin ? theMin : theNum >= theMax ? theMax : theNum; }
inline int				ClampInt(int theNum, int theMin, int theMax)				{ return theNum <= theMin ? theMin : theNum >= theMax ? theMax : theNum; }
inline float			ClampFloat(float theNum, float theMin, float theMax)		{ return theNum <= theMin ? theMin : theNum >= theMax ? theMax : theNum; }
inline float			Distance2D(float x1, float y1, float x2, float y2)			{ return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); }
inline float			FloatLerp(float theStart, float theEnd, float theFactor)	{ return theStart + theFactor * (theEnd - theStart); }
inline int				FloatRoundToInt(float theFloatValue)						{ return theFloatValue > 0 ? theFloatValue + 0.5f : theFloatValue - 0.5f; }
inline bool				FloatApproxEqual(float theFloatVal1, float theFloatVal2)	{ return fabs(theFloatVal1 - theFloatVal2) < FLT_EPSILON; }

Color					GetFlashingColor(int theCounter, int theFlashTime);
/*inline*/ int			ColorComponentMultiply(int theColor1, int theColor2);
Color					ColorsMultiply(const Color& theColor1, const Color& theColor2);
Color					ColorAdd(const Color& theColor1, const Color& theColor2);

inline void				SetBit(uint& theNum, int theIdx, bool theValue = true)		{ if (theValue) theNum |= 1 << theIdx; else theNum &= ~(1 << theIdx); }
inline bool				TestBit(uint theNum, int theIdx)							{ return theNum & (1 << theIdx); }
//#define SetBit(num, idx, val) { if (val) (num) |= 1 << (idx); else (num) &= ~(1 << (idx)); }
//#define TestBit(num, idx) ((num) & (1 - (idx)))