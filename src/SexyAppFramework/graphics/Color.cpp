#include "Color.h"

using namespace Sexy;

Color Color::Black(0, 0, 0);
Color Color::White(255, 255, 255);

Color::Color() :
	mRed(0),
	mGreen(0),
	mBlue(0),
	mAlpha(255)
{
}

Color::Color(int theColor) :
	mRed((theColor   >> 16) & 0xFF),
	mGreen((theColor >> 8 ) & 0xFF),
	mBlue((theColor       ) & 0xFF),
	mAlpha((theColor >> 24) & 0xFF)
{
	if(mAlpha==0)
		mAlpha = 0xff;
}

Color::Color(int theColor, int theAlpha) :
	mRed((theColor   >> 16) & 0xFF),
	mGreen((theColor >> 8 ) & 0xFF),
	mBlue((theColor       ) & 0xFF),
	mAlpha(theAlpha)
{
}

Color::Color(int theRed, int theGreen, int theBlue) :
	mRed(theRed),
	mGreen(theGreen),
	mBlue(theBlue),
	mAlpha(0xFF)
{
}

Color::Color(int theRed, int theGreen, int theBlue, int theAlpha) :
	mRed(theRed),
	mGreen(theGreen),
	mBlue(theBlue),
	mAlpha(theAlpha)
{
}

Color::Color(const SexyRGBA &theColor) :
	mRed(theColor.r),
	mGreen(theColor.g),
	mBlue(theColor.b),
	mAlpha(theColor.a)
{
}

Color::Color(const uchar* theElements) :
	mRed(theElements[0]),
	mGreen(theElements[1]),
	mBlue(theElements[2]),
	mAlpha(0xFF)
{
}

Color::Color(const int* theElements) :
	mRed(theElements[0]),
	mGreen(theElements[1]),
	mBlue(theElements[2]),
	mAlpha(0xFF)
{
}

int	Color::GetRed() const
{
	return mRed;	
}

int Color::GetGreen() const
{
	return mGreen;
}

int	Color::GetBlue() const
{
	return mBlue;
}

int	Color::GetAlpha() const
{
	return mAlpha;
}

int& Color::operator[](int theIdx)
{
	static int aJunk = 0;

	switch (theIdx)
	{
	case 0:
		return mRed;
	case 1:
		return mGreen;
	case 2:
		return mBlue;
	case 3:
		return mAlpha;
	default:
		return aJunk;
	}
}

int Color::operator[](int theIdx) const
{
	switch (theIdx)
	{
	case 0:
		return mRed;
	case 1:
		return mGreen;
	case 2:
		return mBlue;
	case 3:
		return mAlpha;
	default:
		return 0;
	}
}

ulong Color::ToInt() const
{
	return (mAlpha << 24) | (mRed << 16) | (mGreen << 8) | (mBlue);
}

SexyRGBA Color::ToRGBA() const
{
	SexyRGBA anRGBA;
	anRGBA.r = mRed;
	anRGBA.g = mGreen;
	anRGBA.b = mBlue;
	anRGBA.a = mAlpha;

	return anRGBA;
}

bool Sexy::operator==(const Color& theColor1, const Color& theColor2)
{
	return 
		(theColor1.mRed == theColor2.mRed) &&
		(theColor1.mGreen == theColor2.mGreen) &&
		(theColor1.mBlue == theColor2.mBlue) && 
		(theColor1.mAlpha == theColor2.mAlpha);
}

bool Sexy::operator!=(const Color& theColor1, const Color& theColor2)
{
	return 
		(theColor1.mRed != theColor2.mRed) ||
		(theColor1.mGreen != theColor2.mGreen) ||
		(theColor1.mBlue != theColor2.mBlue) ||
		(theColor1.mAlpha != theColor2.mAlpha);
}
