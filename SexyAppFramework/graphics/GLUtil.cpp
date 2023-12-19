#include "GLUtil.h"

using namespace Sexy;

static inline float GetCoord(const GLVertex& theVertex, int theCoord)
{
	switch (theCoord)
	{
	case 0: return theVertex.sx;
	case 1: return theVertex.sy;
	case 2: return theVertex.sz;
	case 3: return theVertex.tu;
	case 4: return theVertex.tv;
	default: return 0;
	}
}

template<class Pred>
void PointClipper<Pred>::ClipPoint(int n, float clipVal, const GLVertex& v1, const GLVertex& v2, VertexList& out)
{
	if (!mPred(GetCoord(v1, n), clipVal))
	{
		if (!mPred(GetCoord(v2, n), clipVal)) // both inside
			out.push_back(v2);
		else // inside -> outside
		{
			float t = (clipVal - GetCoord(v1, n)) / (GetCoord(v2, n) - GetCoord(v1, n));
			out.push_back(Interpolate(v1, v2, t));
		}
	}
	else
	{
		if (!mPred(GetCoord(v2, n), clipVal)) // outside -> inside
		{
			float t = (clipVal - GetCoord(v1, n)) / (GetCoord(v2, n) - GetCoord(v1, n));
			out.push_back(Interpolate(v1, v2, t));
			out.push_back(v2);
		}
		//			else // outside -> outside
	}
}

template<class Pred>
void PointClipper<Pred>::ClipPoints(int n, float clipVal, VertexList& in, VertexList& out)
{
	if (in.size() < 2)
		return;

	ClipPoint(n, clipVal, in[in.size() - 1], in[0], out);
	for (VertexList::size_type i = 0; i < in.size() - 1; i++)
		ClipPoint(n, clipVal, in[i], in[i + 1], out);
}

void Sexy::GetBestTextureDimensions(int& theWidth, int& theHeight, bool isEdge, bool usePow2, GLImageFlags theImageFlags)
{
	if (theImageFlags & GLImageFlag_Use64By64Subdivisions)
	{
		theWidth = theHeight = 64;
		return;
	}

	static int aGoodTextureSize[MAX_TEXTURE_SIZE];
	static bool haveInited = false;
	if (!haveInited)
	{
		haveInited = true;
		int i;
		int aPow2 = 1;
		for (i = 0; i < MAX_TEXTURE_SIZE; i++)
		{
			if (i > aPow2)
				aPow2 <<= 1;

			int aGoodValue = aPow2;
			if ((aGoodValue - i) > 64)
			{
				aGoodValue >>= 1;
				while (true)
				{
					int aLeftOver = i % aGoodValue;
					if (aLeftOver < 64 || IsPowerOf2(aLeftOver))
						break;

					aGoodValue >>= 1;
				}
			}
			aGoodTextureSize[i] = aGoodValue;
		}
	}

	int aWidth = theWidth;
	int aHeight = theHeight;

	if (usePow2)
	{
		if (isEdge || (theImageFlags & GLImageFlag_MinimizeNumSubdivisions))
		{
			aWidth = GetClosestPowerOf2Above(aWidth);
			aHeight = GetClosestPowerOf2Above(aHeight);
		}
		else
		{
			aWidth = aGoodTextureSize[aWidth];
			aHeight = aGoodTextureSize[aHeight];
		}
	}

	theWidth = aWidth;
	theHeight = aHeight;
}