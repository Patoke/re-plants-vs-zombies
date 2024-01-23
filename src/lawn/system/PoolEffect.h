#ifndef __POOLEFFECT_H__
#define __POOLEFFECT_H__

#include "framework/graphics/Image.h"
#include <memory>
#include <array>

constexpr const int CAUSTIC_IMAGE_WIDTH = 128;
constexpr const int CAUSTIC_IMAGE_HEIGHT = 64;

class LawnApp;
class PoolEffect
{
public:
	std::array<uint8_t, 256 * 256> mCausticGrayscaleImage;
	std::unique_ptr<Sexy::Image> mCausticImage;
	LawnApp*			mApp;
	int					mPoolCounter;

public:
	void				PoolEffectInitialize();
	void				PoolEffectDispose();
	void				PoolEffectDraw(Sexy::Graphics* g, bool theIsNight);
	void				UpdateWaterEffect();
	unsigned int		BilinearLookupFixedPoint(unsigned int u, unsigned int v);
	//unsigned int		BilinearLookup(float u, float v);
	void				PoolEffectUpdate();
};

#endif
