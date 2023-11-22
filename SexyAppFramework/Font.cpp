#include "Font.h"
#include "Image.h"

using namespace Sexy;

_Font::_Font()
{	
	mAscent = 0;
	mAscentPadding = 0;
	mHeight = 0;
	mLineSpacingOffset = 0;
}

_Font::_Font(const _Font& theFont) :
	mAscent(theFont.mAscent),
	mAscentPadding(theFont.mAscentPadding),
	mHeight(theFont.mHeight),
	mLineSpacingOffset(theFont.mLineSpacingOffset)
{
}

_Font::~_Font()
{
}

int	_Font::GetAscent()
{
	return mAscent;
}

int	_Font::GetAscentPadding()
{
	return mAscentPadding;
}

int	_Font::GetDescent()
{
	return mHeight - mAscent;
}

int	_Font::GetHeight()
{
	return mHeight;
}

int _Font::GetLineSpacingOffset()
{
	return mLineSpacingOffset;
}

int _Font::GetLineSpacing()
{
	return mHeight + mLineSpacingOffset;
}

int _Font::StringWidth(const SexyString& theString)
{
	return 0;
}

int _Font::CharWidth(SexyChar theChar)
{
	SexyString aString(1, theChar);
	return StringWidth(aString);
}

int _Font::CharWidthKern(SexyChar theChar, SexyChar thePrevChar)
{
	return CharWidth(theChar);
}

void _Font::DrawString(Graphics* g, int theX, int theY, const SexyString& theString, const Color& theColor, const Rect& theClipRect)
{
}
