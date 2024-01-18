#include "SysFont.h"
#include "SexyAppBase.h"
#include "Graphics.h"
#include "ImageFont.h"
//#include "MemoryImage.h"
#include "widget/WidgetManager.h"
#include <stdlib.h>

using namespace Sexy;

SysFont::SysFont(const std::string& theFace, int thePointSize, bool bold, bool italics, bool underline)
{
	Init(gSexyAppBase,theFace,thePointSize,ANSI_CHARSET,bold,italics,underline,false);
}

SysFont::SysFont(SexyAppBase* theApp, const std::string& theFace, int thePointSize, int theScript, bool bold, bool italics, bool underline)
{
	Init(theApp,theFace,thePointSize,theScript,bold,italics,underline,true);
}

void SysFont::Init(SexyAppBase* theApp, const std::string& theFace, int thePointSize, int theScript, bool bold, bool italics, bool underline, bool useDevCaps)
{
	mApp = theApp;

	HDC aDC = ::GetDC(mApp->mHWnd);

	int aHeight = -MulDiv(thePointSize, useDevCaps?GetDeviceCaps(aDC, LOGPIXELSY):96, 72);

	mHFont = CreateFontA(aHeight, 0, 0, 0, bold ? FW_BOLD : FW_NORMAL, italics, underline,
			false, theScript, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, theFace.c_str());
	
	TEXTMETRIC aTextMetric;	
	HFONT anOldFont = (HFONT) SelectObject(aDC, mHFont);
	GetTextMetrics(aDC, &aTextMetric);
	SelectObject(aDC, anOldFont);
	ReleaseDC(mApp->mHWnd, aDC);

	mHeight = aTextMetric.tmHeight;
	mAscent = aTextMetric.tmAscent;

	mDrawShadow = false;
	mSimulateBold = false;
}

SysFont::SysFont(const SysFont& theSysFont)
{
	LOGFONT aLogFont;

	GetObject(theSysFont.mHFont, sizeof(LOGFONT), &aLogFont);
	mHFont = CreateFontIndirect(&aLogFont);
	mApp = theSysFont.mApp;
	mHeight = theSysFont.mHeight;
	mAscent = theSysFont.mAscent;

	mDrawShadow = false;
	mSimulateBold = false;
}

SysFont::~SysFont()
{
	DeleteObject(mHFont);
}

ImageFont* SysFont::CreateImageFont()
{
	int i;
	MemoryImage*			anImage;
	int anImageCharWidth, anImageXOff, anImageYOff;

	////////////////////////////////////////////////////
	// Step 1: Create image 
	anImageCharWidth = CharWidth('W')*2;
	anImageXOff = anImageCharWidth/4;
	anImageYOff = mHeight/2;
	int aWidth = 257*anImageCharWidth;
	int aHeight = mHeight*2;

	BITMAPINFO aBitmapInfo;
	memset(&aBitmapInfo,0,sizeof(aBitmapInfo));
	BITMAPINFOHEADER &aHeader = aBitmapInfo.bmiHeader;
	aHeader.biSize = sizeof(aHeader);
	aHeader.biWidth = aWidth;
	aHeader.biHeight = -aHeight;
	aHeader.biPlanes = 1;
	aHeader.biBitCount = 32;
	aHeader.biCompression = BI_RGB;

	HDC aDC = CreateCompatibleDC(NULL);

	DWORD *aBits = NULL;
	HBITMAP aBitmap = CreateDIBSection(aDC,&aBitmapInfo,DIB_RGB_COLORS,(void**)&aBits,NULL,0);

	HBITMAP anOldBitmap = (HBITMAP)SelectObject(aDC,aBitmap);
	HFONT anOldFont = (HFONT)SelectObject(aDC,mHFont);

	HBRUSH anOldBrush = (HBRUSH)SelectObject(aDC,GetStockObject(BLACK_BRUSH));
	Rectangle(aDC,0,0,aWidth,aHeight);

	SetBkMode(aDC, TRANSPARENT); 	
	SetTextColor(aDC, RGB(255,255,255));
				
	int xpos = anImageXOff;
	int ypos = anImageYOff;
	for (i=0; i<256; i++)
	{
		char aChar = i;
		TextOutA(aDC,xpos,ypos,&aChar,1);
		xpos += anImageCharWidth;
	}
	GdiFlush();


	SelectObject(aDC,anOldBrush);
	SelectObject(aDC,anOldBitmap);
	SelectObject(aDC,anOldFont);

	int aSize = aWidth*aHeight;
	anImage = new MemoryImage(mApp);
	anImage->Create(aWidth,aHeight);
	DWORD *src = aBits;
	DWORD *dst = anImage->GetBits();
	for (i=0; i<aSize; i++)
	{
		DWORD anAlpha = ((*src++)&0xff)<<24;
		*dst++ = anAlpha | 0xFFFFFF;
	}
	anImage->BitsChanged();
	DeleteObject(aBitmap);


	////////////////////////////////////////////////////
	// Step 2: Create image font

	ImageFont *aFont = new ImageFont(anImage);
	FontLayer *aFontLayer = &aFont->mFontData->mFontLayerList.back();

	aFontLayer->mAscent = mAscent;
	aFontLayer->mHeight = mHeight;

	for (i=0; i<256; i++)
	{
		char aChar = i;

		//aFontLayer->mCharData[(uchar) aChar].mImageRect = Rect(aChar*anImageCharWidth,0,anImageCharWidth,anImage->mHeight);
		//aFontLayer->mCharData[(uchar) aChar].mWidth = CharWidth(aChar);
		//aFontLayer->mCharData[(uchar) aChar].mOffset = Point(-anImageXOff,-anImageYOff);
		aFontLayer->GetCharData(aChar)->mImageRect = Rect(aChar*anImageCharWidth,0,anImageCharWidth,anImage->mHeight);
		aFontLayer->GetCharData(aChar)->mWidth = CharWidth(aChar);
		aFontLayer->GetCharData(aChar)->mOffset = Point(-anImageXOff,-anImageYOff);
	}

	aFont->GenerateActiveFontLayers();
	aFont->mActiveListValid = true;

	return aFont;
}

int	SysFont::StringWidth(const SexyString& theString)
{
	HDC aDC = ::GetDC(mApp->mHWnd);
	HFONT anOldFont = (HFONT)::SelectObject(aDC, mHFont);
	int aWidth = 0;

#ifdef _USE_WIDE_STRING
	if (CheckFor98Mill())
	{
		SIZE aSize = { 0, 0 };
		GetTextExtentPoint32W(aDC, theString.c_str(), theString.length(), &aSize);
		aWidth = int(aSize.cx);
	}
	else
#endif
	{
		RECT aRect = {0, 0, 0, 0};	
		DrawTextEx(aDC, (SexyChar*)theString.c_str(), theString.length(), &aRect, DT_CALCRECT | DT_NOPREFIX, NULL);
		aWidth = aRect.right;
	}

	::SelectObject(aDC, anOldFont);
	::ReleaseDC(mApp->mHWnd, aDC);

	return aWidth;
}

void SysFont::DrawString(Graphics* g, int theX, int theY, const SexyString& theString, const Color& theColor, const Rect& theClipRect)
{
	DDImage* aDDImage = dynamic_cast<DDImage*>(g->mDestImage);

	if (aDDImage != NULL)
	{
		LPDIRECTDRAWSURFACE aSurface = aDDImage->GetSurface();
		if (aSurface != NULL)
		{
			HDC aDC;

			if (aDDImage->mLockCount > 0)
				aDDImage->mSurface->Unlock(NULL);

			if ((g->mDestImage == gSexyAppBase->mWidgetManager->mImage) && (gSexyAppBase->Is3DAccelerated()))
				gSexyAppBase->mDDInterface->mD3DInterface->Flush();				
			
			if (aSurface->GetDC(&aDC) == DD_OK)
			{				
				HFONT anOldFont = (HFONT) SelectObject(aDC, mHFont);
				SetBkMode(aDC, TRANSPARENT); 
				IntersectClipRect(aDC, theClipRect.mX, theClipRect.mY, theClipRect.mX + theClipRect.mWidth, theClipRect.mY + theClipRect.mHeight);

				if (mDrawShadow)
				{
					SetTextColor(aDC, RGB(0,0,0));
					TextOut(aDC, theX + g->mTransX+1, theY - mAscent + 1 + g->mTransY+1, theString.c_str(), theString.length());
					if (mSimulateBold)
						TextOut(aDC, theX + g->mTransX+2, theY - mAscent + 1 + g->mTransY+1, theString.c_str(), theString.length());
				}
				SetTextColor(aDC, RGB(theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue()));
				TextOut(aDC, theX + g->mTransX, theY - mAscent + 1 + g->mTransY, theString.c_str(), theString.length());
				if (mSimulateBold)
					TextOut(aDC, theX + g->mTransX + 1, theY - mAscent + 1 + g->mTransY, theString.c_str(), theString.length());

				::SelectObject(aDC, anOldFont);
				aSurface->ReleaseDC(aDC);
				aDDImage->DeleteAllNonSurfaceData();				
			}			

			if (aDDImage->mLockCount > 0)
				aDDImage->mSurface->Lock(NULL, &aDDImage->mLockedSurfaceDesc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);			
		}
	}
	else if (g->mDestImage != &Graphics::mStaticImage) // DrawString can be called when not drawing onto an image.
	{
		HDC aDC = CreateCompatibleDC(NULL);
		HFONT anOldFont = (HFONT) SelectObject(aDC, mHFont);

		int aWidth = StringWidth(theString);
		int aHeight = mHeight;

		BITMAPINFOHEADER bih;
		memset(&bih, 0, sizeof(bih));

		bih.biPlanes = 1;
		bih.biWidth = aWidth;
		bih.biHeight = -aHeight;
		bih.biCompression = BI_RGB;
		bih.biBitCount = 32;
		bih.biSize = sizeof(BITMAPINFOHEADER);

		ulong* whiteBits, * blackBits;
		HBITMAP whiteBitmap = (HBITMAP)CreateDIBSection(aDC, (BITMAPINFO*)&bih, DIB_RGB_COLORS, (void**)&whiteBits, NULL, 0);
		HBITMAP blackBitmap = (HBITMAP)CreateDIBSection(aDC, (BITMAPINFO*)&bih, DIB_RGB_COLORS, (void**)&blackBits, NULL, 0);

		RECT rc = { 0, 0, aWidth, aHeight };

#define DRAW_BITMAP(bmp, brush) \
		{																							\
			HBITMAP oldBmp = (HBITMAP) SelectObject(aDC, bmp);										\
			::FillRect(aDC, &rc, brush);															\
			SetBkMode(aDC, TRANSPARENT);															\
																									\
			if (mDrawShadow)																		\
			{																						\
				SetTextColor(aDC, RGB(0,0,0));														\
				TextOut(aDC, 1, 1, theString.c_str(), theString.length());							\
				if (mSimulateBold)																	\
					TextOut(aDC, 2, 1, theString.c_str(), theString.length());						\
			}																						\
			SetTextColor(aDC, RGB(theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue()));		\
			TextOut(aDC, 0, 0, theString.c_str(), theString.length());								\
			if (mSimulateBold)																		\
				TextOut(aDC, 1, 0, theString.c_str(), theString.length());							\
																									\
			SelectObject(aDC, oldBmp);																\
		}

		DRAW_BITMAP(whiteBitmap, (HBRUSH)GetStockObject(WHITE_BRUSH));
		DRAW_BITMAP(blackBitmap, (HBRUSH)GetStockObject(BLACK_BRUSH));

		SelectObject(aDC, anOldFont);

		MemoryImage aTempImage;
		aTempImage.Create(aWidth, aHeight);

		int aCount = aHeight*aWidth;
		ulong* ptr1 = whiteBits, *ptr2 = blackBits;
		while (aCount > 0)
		{
			if (*ptr1 == *ptr2)
				*ptr1 |= 0xFF000000;
			else if ((*ptr1 & 0xFFFFFF) != 0xFFFFFF || (*ptr2 & 0xFFFFFF) != 0x000000) // if not the background of either, it's a 'blend'
			{
				int ba = 255 + (*ptr2 & 0xFF) - (*ptr1 & 0xFF);
				int ga = 255 + ((*ptr2 >> 8) & 0xFF) - ((*ptr1 >> 8) & 0xFF);
				int ra = 255 + ((*ptr2 >> 16) & 0xFF) - ((*ptr1 >> 16) & 0xFF);
				int aBlue = 255 * (*ptr2 & 0xFF) / ba;
				int aGreen = 255 * ((*ptr2 >> 8) & 0xFF) / ga;
				int aRed = 255 * ((*ptr2 >> 16) & 0xFF) / ra;
				int anAlpha = std::min(ra, std::min(ga, ba));
				*ptr1 = (aBlue) | (aGreen << 8) | (aRed << 16) | (anAlpha << 24);
			}
			else *ptr1 &= 0;

			ptr1++;
			ptr2++;
			--aCount;
		}

		memcpy(aTempImage.GetBits(), whiteBits, aWidth*aHeight*sizeof(ulong));
		g->DrawImage(&aTempImage, theX, theY - mAscent);

		DeleteObject(whiteBitmap);
		DeleteObject(blackBitmap);
		DeleteDC(aDC);
	}
}

_Font* SysFont::Duplicate()
{
	return new SysFont(*this);
}
