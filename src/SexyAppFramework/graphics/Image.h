#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "Common.h"
#include "Color.h"
#include "misc/Rect.h"
#include "misc/Point.h"

namespace Sexy
{

struct Span
{
	int						mY;
	int						mX;
	int						mWidth;
};

enum AnimType
{
	AnimType_None,
	AnimType_Once,
	AnimType_PingPong,
	AnimType_Loop
};

struct AnimInfo
{
	AnimType				mAnimType;
	int						mFrameDelay; // 1/100s
	int						mNumCels;
	std::vector<int>		mPerFrameDelay;
	std::vector<int>		mFrameMap;
	int						mTotalAnimTime;

	AnimInfo();
	void SetPerFrameDelay(int theFrame, int theTime);
	void Compute(int theNumCels, int theBeginFrameTime = 0, int theEndFrameTime = 0);

	int GetPerFrameCel(int theTime);
	int GetCel(int theTime);
};

class Graphics;
class SexyMatrix3;
class SysFont;
class TriVertex;

class Image
{
	friend class			Sexy::SysFont;

public:
	bool					mDrawn;
	std::string				mFilePath;
	int						mWidth;
	int						mHeight;

	// for image strips
	int						mNumRows; 
	int						mNumCols;

	// for animations
	AnimInfo				*mAnimInfo;

public:
	Image();
	Image(const Image& theImage) = delete;
	Image& operator= (const Image&) = delete;
	virtual ~Image();

	int			 GetWidth();
	int			 GetHeight();
	int			 GetCelWidth();		// returns the width of just 1 cel in a strip of images
	int			 GetCelHeight();	// like above but for vertical strips
	int			 GetAnimCel(int theTime); // use animinfo to return appropriate cel to draw at the time
	Rect		 GetAnimCelRect(int theTime);
	Rect		 GetCelRect(int theCel);				// Gets the rectangle for the given cel at the specified row/col 
	Rect		 GetCelRect(int theCol, int theRow);	// Same as above, but for an image with both multiple rows and cols
	void		 CopyAttributes(Image *from);
	Graphics*	 GetGraphics();

	virtual void FillScanLines(Span* theSpans, int theSpanCount, const Color& theColor, int theDrawMode);
	virtual void DrawRect(const Rect& theRect, const Color& theColor, int theDrawMode);

	virtual bool PolyFill3D(const Point theVertices[], int theNumVertices, const Rect *theClipRect, const Color &theColor, int theDrawMode, int tx, int ty) = 0;

	virtual void FillRect(const Rect& theRect, const Color& theColor, int theDrawMode) = 0;
	virtual void ClearRect(const Rect& theRect) = 0;
	virtual void DrawLine(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor, int theDrawMode) = 0;
	virtual void DrawLineAA(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor, int theDrawMode) = 0;
	virtual void FillScanLinesWithCoverage(Span* theSpans, int theSpanCount, const Color& theColor, int theDrawMode, const BYTE* theCoverage, int theCoverX, int theCoverY, int theCoverWidth, int theCoverHeight) = 0;
	virtual void Blt(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode) = 0;
	virtual void BltF(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Rect &theClipRect, const Color& theColor, int theDrawMode) = 0;
	virtual void BltRotated(Image* theImage, float theX, float theY, const Rect &theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, double theRot, float theRotCenterX, float theRotCenterY) = 0;
	virtual void StretchBlt(Image* theImage, const Rect& theDestRect, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, bool fastStretch) = 0;
	virtual void BltMatrix(Image* theImage, float x, float y, const SexyMatrix3 &theMatrix, const Rect& theClipRect, const Color& theColor, int theDrawMode, const Rect &theSrcRect, bool blend) = 0;
	virtual void BltTrianglesTex(Image *theTexture, const TriVertex theVertices[][3], int theNumTriangles, const Rect& theClipRect, const Color &theColor, int theDrawMode, float tx, float ty, bool blend) = 0;
	virtual void BltMirror(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode) = 0;
	virtual void StretchBltMirror(Image* theImage, const Rect& theDestRect, const Rect& theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, bool fastStretch) = 0;
};

class DummyImage : public Image {
	bool PolyFill3D(const Point*, int, const Rect*, const Color&, int, int, int) { return false; }

	void FillRect(const Rect&, const Color&, int){}
	void ClearRect(const Rect&){}
	void DrawLine(double, double, double, double, const Color&, int){}
	void DrawLineAA(double, double, double, double, const Color&, int){}
	void FillScanLinesWithCoverage(Span*, int, const Color&, int, const BYTE*, int, int, int, int){}
	void Blt(Image*, int, int, const Rect&, const Color&, int){}
	void BltF(Image*, float, float, const Rect&, const Rect&, const Color&, int){}
	void BltRotated(Image*, float, float, const Rect &, const Rect&, const Color&, int, double, float, float){}
	void StretchBlt(Image*, const Rect&, const Rect&, const Rect&, const Color&, int, bool){}
	void BltMatrix(Image*, float, float, const SexyMatrix3&, const Rect&, const Color&, int, const Rect&, bool){}
	void BltTrianglesTex(Image*, const TriVertex (*)[3], int, const Rect&, const Color &, int, float, float, bool){}
	void BltMirror(Image*, int, int, const Rect&, const Color&, int){}
	void StretchBltMirror(Image*, const Rect&, const Rect&, const Rect&, const Color&, int, bool){}
};

}

#endif //__IMAGE_H__
