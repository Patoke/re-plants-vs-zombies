#ifndef __GLUTIL_H__
#define __GLUTIL_H__

#include "Common.h"

#define RGBA_MAKE(r, g, b, a)   ((uint32_t) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))
#define MAX_TEXTURE_SIZE 1024

namespace Sexy
{
	class GLImage;
	class Image;
	class MemoryImage;

	typedef std::set<GLImage*> GLImageSet;
	typedef GLuint GLTexture;

	enum GLImageFlags : uint32_t
	{
		GLImageFlag_None = 0x0000,
		GLImageFlag_MinimizeNumSubdivisions = 0x0001,		// subdivide image into fewest possible textures (may use more memory)
		GLImageFlag_Use64By64Subdivisions = 0x0002,		// good to use with image strips so the entire texture isn't pulled in when drawing just a piece
	};

	struct TextureDataPiece
	{
		GLTexture mTexture; // pointer to texture data
		int mWidth, mHeight;
	};

	struct GLVertex {
		float sx;
		float sy;
		float sz;
		float rhw;
		uint32_t color;
		uint32_t specular;
		float tu;
		float tv;
	};

	struct VertexList
	{
		enum { MAX_STACK_VERTS = 100 };
		GLVertex mStackVerts[MAX_STACK_VERTS];
		GLVertex* mVerts;
		int mSize;
		int mCapacity;

		typedef int size_type;

		VertexList() : mVerts(mStackVerts), mSize(0), mCapacity(MAX_STACK_VERTS) { }
		VertexList(const VertexList& theList) : mVerts(mStackVerts), mSize(theList.mSize), mCapacity(MAX_STACK_VERTS)
		{
			reserve(mSize);
			memcpy(mVerts, theList.mVerts, mSize * sizeof(mVerts[0]));
		}

		~VertexList()
		{
			if (mVerts != mStackVerts)
				delete mVerts;
		}

		void reserve(int theCapacity)
		{
			if (mCapacity < theCapacity)
			{
				mCapacity = theCapacity;
				GLVertex* aNewList = new GLVertex[theCapacity];
				memcpy(aNewList, mVerts, mSize * sizeof(mVerts[0]));
				if (mVerts != mStackVerts)
					delete mVerts;

				mVerts = aNewList;
			}
		}

		void push_back(const GLVertex& theVert)
		{
			if (mSize == mCapacity)
				reserve(mCapacity * 2);

			mVerts[mSize++] = theVert;
		}

		void operator=(const VertexList& theList)
		{
			reserve(theList.mSize);
			mSize = theList.mSize;
			memcpy(mVerts, theList.mVerts, mSize * sizeof(mVerts[0]));
		}


		GLVertex& operator[](int thePos)
		{
			return mVerts[thePos];
		}

		int size() { return mSize; }
		void clear() { mSize = 0; }
	};

	template<class Pred>
	struct PointClipper
	{
		Pred mPred;

		void ClipPoint(int n, float clipVal, const GLVertex& v1, const GLVertex& v2, VertexList& out);
		void ClipPoints(int n, float clipVal, VertexList& in, VertexList& out);
	};

	static int GetClosestPowerOf2Above(int theNum)
	{
		// https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2Float
		// A copy of theNum is created for this function so we don't care about modifying it
		theNum--;
		theNum |= theNum >> 1;
		theNum |= theNum >> 2;
		theNum |= theNum >> 4;
		theNum |= theNum >> 8;
		theNum |= theNum >> 16;
		// unlikely to happen
		if constexpr (sizeof(theNum) == 8)
			theNum |= theNum >> 32;
		theNum++;

		return theNum;
	}

	static bool IsPowerOf2(int theNum)
	{
		// For powers of 2 there's only 1 bit set
		return theNum > 0 && !(theNum & (theNum - 1));
	}

	extern void GetBestTextureDimensions(int& theWidth, int& theHeight, bool isEdge, bool usePow2, GLImageFlags theImageFlags);
}

#endif //__GLUTIL_H__