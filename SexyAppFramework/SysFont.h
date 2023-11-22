#ifndef __SYSFONT_H__
#define __SYSFONT_H__

#include "Font.h"

namespace Sexy
{

class ImageFont;
class SexyAppBase;

class SysFont : public _Font
{
public:	
	HFONT					mHFont;
	SexyAppBase*			mApp;
	bool					mDrawShadow;
	bool					mSimulateBold;
	
	void Init(SexyAppBase* theApp, const std::string& theFace, int thePointSize, int theScript, bool bold, bool italics, bool underline, bool useDevCaps);

public:
	SysFont(const std::string& theFace, int thePointSize, bool bold = false, bool italics = false, bool underline = false);
	SysFont(SexyAppBase* theApp, const std::string& theFace, int thePointSize, int theScript = ANSI_CHARSET, bool bold = false, bool italics = false, bool underline = false);
	SysFont(const SysFont& theSysFont);

	virtual ~SysFont();

	ImageFont*				CreateImageFont();
	virtual int				StringWidth(const SexyString& theString);
	virtual void			DrawString(Graphics* g, int theX, int theY, const SexyString& theString, const Color& theColor, const Rect& theClipRect);

	virtual _Font*			Duplicate();
};

}

#endif //__SYSFONT_H__