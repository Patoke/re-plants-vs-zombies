#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "framework/Common.h"
#include "framework/misc/Rect.h"
#include "Color.h"
#include "Image.h"
#include "TriVertex.h"

namespace Sexy
{

class _Font;
class SexyMatrix3;
class Transform;

const int MAX_TEMP_SPANS = 8192;

struct Edge
{
    double mX;
    double mDX;
    int i;
	double b;
};

class Graphics;

class GraphicsState
{
public:
	static DummyImage		mStaticImage;
	Image*					mDestImage;
	float					mTransX;
	float					mTransY;
	float					mScaleX;
	float					mScaleY;
	float					mScaleOrigX;
	float					mScaleOrigY;
	Rect					mClipRect;
	Color					mColor;
	_Font*					mFont;
	int						mDrawMode;
	bool					mColorizeImages;
	bool					mFastStretch;
	bool					mWriteColoredString;
	bool					mLinearBlend;
//	bool					mIs3D;

public:
	void					CopyStateFrom(const GraphicsState* theState);
};

typedef std::list<GraphicsState> GraphicsStateList;

class Graphics : public GraphicsState
{
public:	
	enum
	{
		DRAWMODE_NORMAL,
		DRAWMODE_ADDITIVE
	};
	
	Edge*					mPFActiveEdgeList;
	int						mPFNumActiveEdges;
	static const Point*		mPFPoints;
	int						mPFNumVertices;

	GraphicsStateList		mStateStack;

protected:	
	static int				PFCompareInd(const void* u, const void* v);
	static int				PFCompareActive(const void* u, const void* v);
	void					PFDelete(int i); 
	void					PFInsert(int i, int y);

	void					DrawImageTransformHelper(Image* theImage, const Transform &theTransform, const Rect &theSrcRect, float x, float y, bool useFloat);

public:
	Graphics(const Graphics& theGraphics);
	Graphics(Image* theDestImage = NULL);
	virtual ~Graphics();	

	void					PushState();
	void					PopState();

	Graphics*				Create();
	
	void					SetFont(_Font* theFont);
	_Font*					GetFont();

	void					SetColor(const Color& theColor);
	const Color&			GetColor();
	
	void					SetDrawMode(int theDrawMode);
	int						GetDrawMode();
	
	void					SetColorizeImages(bool colorizeImages);
	bool					GetColorizeImages();

	void					SetFastStretch(bool fastStretch);
	bool					GetFastStretch();

	void					SetLinearBlend(bool linear); // for DrawImageMatrix, DrawImageTransform, etc...
	bool					GetLinearBlend();

	void					FillRect(int theX, int theY, int theWidth, int theHeight);
	void					FillRect(const Rect& theRect);
	void					DrawRect(int theX, int theY, int theWidth, int theHeight);	
	void					DrawRect(const Rect& theRect);
	void					ClearRect(int theX, int theY, int theWidth, int theHeight);	
	void					ClearRect(const Rect& theRect);
	void					DrawString(const SexyString& theString, int theX, int theY);
	
private:
	bool					DrawLineClipHelper(double* theStartX, double* theStartY, double *theEndX, double* theEndY);
public:
	void					DrawLine(int theStartX, int theStartY, int theEndX, int theEndY);
	void					DrawLineAA(int theStartX, int theStartY, int theEndX, int theEndY);
	void					PolyFill(const Point *theVertexList, int theNumVertices, bool convex = false);
	void					PolyFillAA(const Point *theVertexList, int theNumVertices, bool convex = false);

	void					DrawImage(Image* theImage, int theX, int theY);
	void					DrawImage(Image* theImage, int theX, int theY, const Rect& theSrcRect);
	void					DrawImage(Image* theImage, const Rect& theDestRect, const Rect& theSrcRect);
	void					DrawImage(Image* theImage, int theX, int theY, int theStretchedWidth, int theStretchedHeight);
	void					DrawImageF(Image* theImage, float theX, float theY);
	void					DrawImageF(Image* theImage, float theX, float theY, const Rect& theSrcRect);

	void					DrawImageMirror(Image* theImage, int theX, int theY, bool mirror = true);
	void					DrawImageMirror(Image* theImage, int theX, int theY, const Rect& theSrcRect, bool mirror = true);
	void					DrawImageMirror(Image* theImage, const Rect& theDestRect, const Rect& theSrcRect, bool mirror = true);

	void					DrawImageRotated(Image* theImage, int theX, int theY, double theRot, const Rect *theSrcRect = NULL);
	void					DrawImageRotated(Image* theImage, int theX, int theY, double theRot, int theRotCenterX, int theRotCenterY, const Rect *theSrcRect = NULL);
	void					DrawImageRotatedF(Image* theImage, float theX, float theY, double theRot, const Rect *theSrcRect = NULL);
	void					DrawImageRotatedF(Image* theImage, float theX, float theY, double theRot, float theRotCenterX, float theRotCenterY, const Rect *theSrcRect = NULL);

	void					DrawImageMatrix(Image* theImage, const SexyMatrix3 &theMatrix, float x = 0, float y = 0);
	void					DrawImageMatrix(Image* theImage, const SexyMatrix3 &theMatrix, const Rect &theSrcRect, float x = 0, float y = 0);
	void					DrawImageTransform(Image* theImage, const Transform &theTransform, float x = 0, float y = 0);
	void					DrawImageTransform(Image* theImage, const Transform &theTransform, const Rect &theSrcRect, float x = 0, float y = 0);
	void					DrawImageTransformF(Image* theImage, const Transform &theTransform, float x = 0, float y = 0);
	void					DrawImageTransformF(Image* theImage, const Transform &theTransform, const Rect &theSrcRect, float x = 0, float y = 0);
	void					DrawTriangleTex(Image *theTexture, const TriVertex &v1, const TriVertex &v2, const TriVertex &v3);
	void					DrawTrianglesTex(Image *theTexture, const TriVertex theVertices[][3], int theNumTriangles);

	void					DrawImageCel(Image* theImageStrip, int theX, int theY, int theCel);
	void					DrawImageCel(Image* theImageStrip, const Rect& theDestRect, int theCel);
	void					DrawImageCel(Image* theImageStrip, int theX, int theY, int theCelCol, int theCelRow);
	void					DrawImageCel(Image* theImageStrip, const Rect& theDestRect, int theCelCol, int theCelRow);

	void					DrawImageAnim(Image* theImageAnim, int theX, int theY, int theTime);

	void					ClearClipRect();
	void					SetClipRect(int theX, int theY, int theWidth, int theHeight);
	void					SetClipRect(const Rect& theRect);
	void					ClipRect(int theX, int theY, int theWidth, int theHeight);
	void					ClipRect(const Rect& theRect);
	void					Translate(int theTransX, int theTransY);
	void					TranslateF(float theTransX, float theTransY);

	// In progress: Only affects DrawImage
	void					SetScale(float theScaleX, float theScaleY, float theOrigX, float theOrigY);

	int						StringWidth(const SexyString& theString);
	void					DrawImageBox(const Rect& theDest, Image* theComponentImage);
	void					DrawImageBox(const Rect& theSrc, const Rect& theDest, Image* theComponentImage);

	int						WriteString(const SexyString& theString, int theX, int theY, int theWidth = -1, int theJustification = 0, bool drawString = true, int theOffset = 0, int theLength = -1, int theOldColor = -1);
	int						WriteWordWrapped(const Rect& theRect, const SexyString& theLine, int theLineSpacing = -1, int theJustification = -1, int *theMaxWidth = NULL, int theMaxChars = -1, int* theLastWidth = NULL);
	int						DrawStringColor(const SexyString& theString, int theX, int theY, int theOldColor = -1); //works like DrawString but can have color tags like ^ff0000^.
	int						DrawStringWordWrapped(const SexyString& theLine, int theX, int theY, int theWrapWidth = 10000000, int theLineSpacing = -1, int theJustification = -1, int *theMaxWidth = NULL); //works like DrawString but also word wraps
	int						GetWordWrappedHeight(int theWidth, const SexyString& theLine, int theLineSpacing = -1, int *theMaxWidth = NULL);

//	bool					Is3D() { return mIs3D; }
};

class GraphicsAutoState
{
public:
	Graphics*				mG;

public:
	
	GraphicsAutoState(Graphics* theG) : mG(theG)
	{
		mG->PushState();
	}

	~GraphicsAutoState()
	{
		mG->PopState();
	}
};

}

#endif //__GRAPHICS_H__
