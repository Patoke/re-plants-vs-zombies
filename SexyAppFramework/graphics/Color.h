#ifndef __COLOR_H__
#define __COLOR_H__

#include "Common.h"

namespace Sexy
{

#pragma pack(push,1)
struct SexyRGBA {uint8_t b,g,r,a;};
#pragma pack(pop)

class Color
{
public:
	int32_t mRed;
	int32_t mGreen;
	int32_t mBlue;
	int32_t mAlpha;

	static Color Black;
	static Color White;

public:
	Color();
	Color(uint32_t theColor);
	Color(uint32_t theColor, int32_t theAlpha);
	Color(int32_t theRed, int32_t theGreen, int32_t theBlue);
	Color(int32_t theRed, int32_t theGreen, int32_t theBlue, int32_t theAlpha);
	Color(const SexyRGBA &theColor);
	Color(const uchar* theElements);	
	Color(const int* theElements);

	int32_t					GetRed() const;
	int32_t					GetGreen() const;
	int32_t					GetBlue() const;
	int32_t					GetAlpha() const;
	uint32_t				ToInt() const;
	SexyRGBA				ToRGBA() const;

	int32_t&				operator[](int theIdx);
	int32_t					operator[](int theIdx) const;
};

bool operator==(const Color& theColor1, const Color& theColor2);
bool operator!=(const Color& theColor1, const Color& theColor2);

}

#endif //__COLOR_H__