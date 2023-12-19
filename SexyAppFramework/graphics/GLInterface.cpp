#include "GLInterface.h"

using namespace Sexy;

static DWORD gSupportedPixelFormats;
static bool gTextureSizeMustBePow2;
static bool gLinearFilter = false;

///////////////////////////////////////////////////////////////////////////////
// GLInterface start
///////////////////////////////////////////////////////////////////////////////

GLInterface::GLInterface(SexyAppBase* theApp)
{
	mApp = theApp;

}

GLInterface::~GLInterface()
{

}

///////////////////////////////////////////////////////////////////////////////
// GLInterface end
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// TextureData start
///////////////////////////////////////////////////////////////////////////////

TextureData::TextureData()
{
	mWidth = 0;
	mHeight = 0;
	mTexVecWidth = 0;
	mTexVecHeight = 0;
	mBitsChangedCount = 0;
	mTexMemSize = 0;
	mTexPieceWidth = 64;
	mTexPieceHeight = 64;

	mImageFlags = GLImageFlag_None;
}

TextureData::~TextureData()
{
	ReleaseTextures();
}

void TextureData::ReleaseTextures()
{
	for (int i = 0; i < (int)mTextures.size(); i++)
	{
		GLTexture aTexture = mTextures[i].mTexture;
		if (aTexture != NULL)
			glDeleteTextures(1, &aTexture);
	}

	mTextures.clear();

	mTexMemSize = 0;
}

void TextureData::CreateTextureDimensions(MemoryImage* theImage)
{
	int aWidth = theImage->GetWidth();
	int aHeight = theImage->GetHeight();
	int i;

	// Calculate inner piece sizes
	mTexPieceWidth = aWidth;
	mTexPieceHeight = aHeight;
	bool usePow2 = true; //gTextureSizeMustBePow2 || mPixelFormat==PixelFormat_Palette8;
	GetBestTextureDimensions(mTexPieceWidth, mTexPieceHeight, false, usePow2, mImageFlags);

	// Calculate right boundary piece sizes
	int aRightWidth = aWidth % mTexPieceWidth;
	int aRightHeight = mTexPieceHeight;
	if (aRightWidth > 0)
		GetBestTextureDimensions(aRightWidth, aRightHeight, true, usePow2, mImageFlags);
	else
		aRightWidth = mTexPieceWidth;

	// Calculate bottom boundary piece sizes
	int aBottomWidth = mTexPieceWidth;
	int aBottomHeight = aHeight % mTexPieceHeight;
	if (aBottomHeight > 0)
		GetBestTextureDimensions(aBottomWidth, aBottomHeight, true, usePow2, mImageFlags);
	else
		aBottomHeight = mTexPieceHeight;

	// Calculate corner piece size
	int aCornerWidth = aRightWidth;
	int aCornerHeight = aBottomHeight;
	GetBestTextureDimensions(aCornerWidth, aCornerHeight, true, usePow2, mImageFlags);

	// Allocate texture array
	mTexVecWidth = (aWidth + mTexPieceWidth - 1) / mTexPieceWidth;
	mTexVecHeight = (aHeight + mTexPieceHeight - 1) / mTexPieceHeight;
	mTextures.resize(mTexVecWidth * mTexVecHeight);

	// Assign inner pieces
	for (i = 0; i < (int)mTextures.size(); i++)
	{
		TextureDataPiece& aPiece = mTextures[i];
		aPiece.mTexture = NULL;
		aPiece.mWidth = mTexPieceWidth;
		aPiece.mHeight = mTexPieceHeight;
	}

	// Assign right pieces
	for (i = mTexVecWidth - 1; i < (int)mTextures.size(); i += mTexVecWidth)
	{
		TextureDataPiece& aPiece = mTextures[i];
		aPiece.mWidth = aRightWidth;
		aPiece.mHeight = aRightHeight;
	}

	// Assign bottom pieces
	for (i = mTexVecWidth * (mTexVecHeight - 1); i < (int)mTextures.size(); i++)
	{
		TextureDataPiece& aPiece = mTextures[i];
		aPiece.mWidth = aBottomWidth;
		aPiece.mHeight = aBottomHeight;
	}

	// Assign corner piece
	mTextures.back().mWidth = aCornerWidth;
	mTextures.back().mHeight = aCornerHeight;
	/**/

	mMaxTotalU = aWidth / (float)mTexPieceWidth;
	mMaxTotalV = aHeight / (float)mTexPieceHeight;
}

void TextureData::CreateTextures(MemoryImage* theImage)
{
	theImage->DeleteSWBuffers(); // don't need these buffers for 3d drawing

	theImage->CommitBits();

	// Release texture if image size has changed
	bool createTextures = false;
	if (mWidth != theImage->mWidth || mHeight != theImage->mHeight || theImage->mTexFlags != mImageFlags)
	{
		ReleaseTextures();

		mImageFlags = (GLImageFlags)theImage->mTexFlags;
		CreateTextureDimensions(theImage);
		createTextures = true;
	}

	int aHeight = theImage->GetHeight();
	int aWidth = theImage->GetWidth();

	int i = 0;
	for (int y = 0; y < aHeight; y += mTexPieceHeight)
	{
		for (int x = 0; x < aWidth; x += mTexPieceWidth, i++)
		{
			TextureDataPiece& aPiece = mTextures[i];
			if (createTextures)
			{
				glGenTextures(1, &aPiece.mTexture);
				glBindTexture(GL_TEXTURE_2D, aPiece.mTexture);
				if (aPiece.mTexture == NULL) // create texture failure
					return;

				// We times 4 because we're using the A8R8G8B
				mTexMemSize += aPiece.mWidth * aPiece.mHeight * 4;
			}
			else if (aPiece.mTexture)
				glBindTexture(GL_TEXTURE_2D, aPiece.mTexture);
			else
				return; // strange problem happened

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, aPiece.mWidth, aPiece.mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, theImage->mTexData);
		}
	}

	mWidth = theImage->mWidth;
	mHeight = theImage->mHeight;
	mBitsChangedCount = theImage->mBitsChangedCount;
}

void TextureData::CheckCreateTextures(MemoryImage* theImage)
{
	if (theImage->mWidth != mWidth || theImage->mHeight != mHeight || theImage->mBitsChangedCount != mBitsChangedCount || theImage->mTexFlags != mImageFlags)
		CreateTextures(theImage);
}

GLTexture TextureData::GetTexture(int x, int y, int& width, int& height, float& u1, float& v1, float& u2, float& v2)
{
	int tx = x / mTexPieceWidth;
	int ty = y / mTexPieceHeight;

	TextureDataPiece& aPiece = mTextures[ty * mTexVecWidth + tx];

	int left = x % mTexPieceWidth;
	int top = y % mTexPieceHeight;
	int right = left + width;
	int bottom = top + height;

	if (right > aPiece.mWidth)
		right = aPiece.mWidth;

	if (bottom > aPiece.mHeight)
		bottom = aPiece.mHeight;

	width = right - left;
	height = bottom - top;

	u1 = (float)left / aPiece.mWidth;
	v1 = (float)top / aPiece.mHeight;
	u2 = (float)right / aPiece.mWidth;
	v2 = (float)bottom / aPiece.mHeight;

	return aPiece.mTexture;
}

GLTexture TextureData::GetTextureF(float x, float y, float& width, float& height, float& u1, float& v1, float& u2, float& v2)
{
	int tx = x / mTexPieceWidth;
	int ty = y / mTexPieceHeight;

	TextureDataPiece& aPiece = mTextures[ty * mTexVecWidth + tx];

	float left = x - tx * mTexPieceWidth;
	float top = y - ty * mTexPieceHeight;
	float right = left + width;
	float bottom = top + height;

	if (right > aPiece.mWidth)
		right = aPiece.mWidth;

	if (bottom > aPiece.mHeight)
		bottom = aPiece.mHeight;

	width = right - left;
	height = bottom - top;

	u1 = (float)left / aPiece.mWidth;
	v1 = (float)top / aPiece.mHeight;
	u2 = (float)right / aPiece.mWidth;
	v2 = (float)bottom / aPiece.mHeight;

	return aPiece.mTexture;
}

void TextureData::BltTransformed(const SexyMatrix3& theTrans, const Rect& theSrcRect, const Color& theColor, const Rect* theClipRect, float theX, float theY, bool center)
{
	int srcLeft = theSrcRect.mX;
	int srcTop = theSrcRect.mY;
	int srcRight = srcLeft + theSrcRect.mWidth;
	int srcBottom = srcTop + theSrcRect.mHeight;
	int srcX, srcY;
	float dstX, dstY;
	int aWidth;
	int aHeight;
	float u1, v1, u2, v2;
	float startx = 0, starty = 0;
	float pixelcorrect = 0.5f;

	if (center)
	{
		startx = -theSrcRect.mWidth / 2.0f;
		starty = -theSrcRect.mHeight / 2.0f;
		pixelcorrect = 0.0f;
	}

	srcY = srcTop;
	dstY = starty;

	uint32_t aColor = RGBA_MAKE(theColor.mRed, theColor.mGreen, theColor.mBlue, theColor.mAlpha);

	if ((srcLeft >= srcRight) || (srcTop >= srcBottom))
		return;

	while (srcY < srcBottom)
	{
		srcX = srcLeft;
		dstX = startx;
		while (srcX < srcRight)
		{
			aWidth = srcRight - srcX;
			aHeight = srcBottom - srcY;
			GLTexture aTexture = GetTexture(srcX, srcY, aWidth, aHeight, u1, v1, u2, v2);

			float x = dstX; // - 0.5f;
			float y = dstY; // - 0.5f;

			SexyVector2 p[4] = { SexyVector2(x, y), SexyVector2(x,y + aHeight), SexyVector2(x + aWidth, y) , SexyVector2(x + aWidth, y + aHeight) };
			SexyVector2 tp[4];

			int i;
			for (i = 0; i < 4; i++)
			{
				tp[i] = theTrans * p[i];
				tp[i].x -= pixelcorrect - theX;
				tp[i].y -= pixelcorrect - theY;
			}

			bool clipped = false;
			if (theClipRect != NULL)
			{
				int left = theClipRect->mX;
				int right = left + theClipRect->mWidth;
				int top = theClipRect->mY;
				int bottom = top + theClipRect->mHeight;
				for (i = 0; i < 4; i++)
				{
					if (tp[i].x < left || tp[i].x >= right || tp[i].y < top || tp[i].y >= bottom)
					{
						clipped = true;
						break;
					}
				}
			}

			GLVertex aVertex[4] =
			{
				{ {tp[0].x},{tp[0].y},{0}, {1},{aColor},{0}, {u1},{v1} },
				{ {tp[1].x},{tp[1].y},{0}, {1},{aColor},{0}, {u1},{v2} },
				{ {tp[2].x},{tp[2].y},{0}, {1},{aColor},{0}, {u2},{v1} },
				{ {tp[3].x},{tp[3].y},{0}, {1},{aColor},{0}, {u2},{v2} }
			};

			// @Patoke render: properly implement
			glBindTexture(GL_TEXTURE_2D, aTexture);

			if (!clipped) {
				//glBindVertexArray(aVertex);
				glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, 0);

				//theDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, gVertexType, aVertex, 4, 0);
			}
			else
			{
				// @Patoke todo: should this even be done at runtime?
				// modify DrawPolyClipped so it accepts a fixed size
				VertexList aList;
				aList.push_back(aVertex[0]);
				aList.push_back(aVertex[1]);
				aList.push_back(aVertex[3]);
				aList.push_back(aVertex[2]);

				DrawPolyClipped(theClipRect, aList);
			}

			srcX += aWidth;
			dstX += aWidth;
		}

		srcY += aHeight;
		dstY += aHeight;
	}
}

///////////////////////////////////////////////////////////////////////////////
// TextureData end
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Render Helpers start
///////////////////////////////////////////////////////////////////////////////

static void DrawPolyClipped(const Rect* theClipRect, const VertexList& theList)
{
	VertexList l1, l2;
	l1 = theList;

	int left = theClipRect->mX;
	int right = left + theClipRect->mWidth;
	int top = theClipRect->mY;
	int bottom = top + theClipRect->mHeight;

	VertexList* in = &l1, * out = &l2;
	PointClipper<std::less<float> > aLessClipper;
	PointClipper<std::greater_equal<float> > aGreaterClipper;

	aLessClipper.ClipPoints(0, left, *in, *out); std::swap(in, out); out->clear();
	aLessClipper.ClipPoints(1, top, *in, *out); std::swap(in, out); out->clear();
	aGreaterClipper.ClipPoints(0, right, *in, *out); std::swap(in, out); out->clear();
	aGreaterClipper.ClipPoints(1, bottom, *in, *out);

	VertexList& aList = *out;

	if (aList.size() >= 3)
		// @Patoke render: properly implement
		glDrawElements(GL_TRIANGLES, aList.size(), GL_UNSIGNED_INT, 0);
		//theDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, gVertexType, &aList[0], aList.size(), 0);
}

///////////////////////////////////////////////////////////////////////////////
// Render Helpers end
///////////////////////////////////////////////////////////////////////////////