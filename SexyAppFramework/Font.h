#ifndef __FONT_H__
#define __FONT_H__

#include "Common.h"
#include "Rect.h"
#include "Color.h"

namespace Sexy
{

class Graphics;

class Font
{
public:
	int						mAscent;
	int						mAscentPadding; // How much space is above the avg uppercase char
	int						mHeight;
	int						mLineSpacingOffset; // This plus height should get added between lines
	
public:
	Font();
	Font(const Font& theFont);
	virtual ~Font();

	virtual int				GetAscent();
	virtual int				GetAscentPadding();
	virtual int				GetDescent();
	virtual int				GetHeight();
	virtual int				GetLineSpacingOffset();
	virtual int				GetLineSpacing();
	virtual int				StringWidth(const SexyString& theString);
	virtual int				CharWidth(SexyChar theChar);
	virtual int				CharWidthKern(SexyChar theChar, SexyChar thePrevChar);

	virtual void			DrawString(Graphics* g, int theX, int theY, const SexyString& theString, const Color& theColor, const Rect& theClipRect);

	virtual Font*			Duplicate() = NULL;
};

}

#endif //__FONT_H__