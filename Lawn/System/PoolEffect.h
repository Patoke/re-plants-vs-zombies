#ifndef __POOLEFFECT_H__
#define __POOLEFFECT_H__

#include "../../ConstEnums.h"

constexpr const int CAUSTIC_IMAGE_WIDTH = 128;
constexpr const int CAUSTIC_IMAGE_HEIGHT = 64;

namespace Sexy
{
	class MemoryImage;
	class Graphics;
};

class LawnApp;
class PoolEffect
{
public:
	unsigned char*		mCausticGrayscaleImage;
	Sexy::MemoryImage*	mCausticImage;
	LawnApp*			mApp;
	int					mPoolCounter;

public:
	void				PoolEffectInitialize();
	void				PoolEffectDispose();
	void				PoolEffectDraw(Sexy::Graphics* g, bool theIsNight);
	void				UpdateWaterEffect(Sexy::Graphics* g);
	unsigned int		BilinearLookupFixedPoint(unsigned int u, unsigned int v);
	//unsigned int		BilinearLookup(float u, float v);
	void				PoolEffectUpdate();
};

#endif
