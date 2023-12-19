#ifndef __GLINTERFACE_H__
#define __GLINTERFACE_H__

#include "GLUtil.h"
#include "NativeDisplay.h"
#include "MemoryImage.h"
#include "misc/CritSect.h"
#include "misc/Rect.h"
#include "misc/Ratio.h"
#include "misc/SexyMatrix.h"

namespace Sexy
{

	class SexyAppBase;
	class GLImage;
	class Image;
	class MemoryImage;

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	struct TextureData
	{
	public:
		typedef std::vector<TextureDataPiece> TextureVector;

		TextureVector mTextures;

		int mWidth, mHeight;
		int mTexVecWidth, mTexVecHeight;
		int mTexPieceWidth, mTexPieceHeight;
		int mBitsChangedCount;
		int mTexMemSize;
		float mMaxTotalU, mMaxTotalV;
		GLImageFlags mImageFlags;

		TextureData();
		~TextureData();

		void ReleaseTextures();

		// OpenGL pointers are loaded globally, no need to pass individual interfaces
		void CreateTextureDimensions(MemoryImage* theImage);
		void CreateTextures(MemoryImage* theImage);
		void CheckCreateTextures(MemoryImage* theImage);
		GLTexture GetTexture(int x, int y, int& width, int& height, float& u1, float& v1, float& u2, float& v2);
		GLTexture GetTextureF(float x, float y, float& width, float& height, float& u1, float& v1, float& u2, float& v2);

		void Blt(float theX, float theY, const Rect& theSrcRect, const Color& theColor);
		void BltTransformed(const SexyMatrix3& theTrans, const Rect& theSrcRect, const Color& theColor, const Rect* theClipRect = NULL, float theX = 0, float theY = 0, bool center = false);
		void BltTriangles(const TriVertex theVertices[][3], int theNumTriangles, DWORD theColor, float tx = 0, float ty = 0);
	};

	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	class GLInterface : public NativeDisplay
	{
	public:
		SexyAppBase* mApp;

	public:
		GLInterface(SexyAppBase* theApp);
		virtual ~GLInterface();

		int						Init(HWND theWindow, bool IsWindowed);
	};

}

#endif //__GLINTERFACE_H__