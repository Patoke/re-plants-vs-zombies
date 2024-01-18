#ifndef __FILTEREFFECT_H__
#define __FILTEREFFECT_H__

#include <map>

namespace Sexy
{
    class Image;
//    class MemoryImage;
}
using namespace Sexy;

enum FilterEffect
{
    FILTER_EFFECT_NONE = -1,
    FILTER_EFFECT_WASHED_OUT,
    FILTER_EFFECT_LESS_WASHED_OUT,
    FILTER_EFFECT_WHITE,
    NUM_FILTER_EFFECTS
};

typedef std::map<Image*, Image*> ImageFilterMap;
extern ImageFilterMap gFilterMap[FilterEffect::NUM_FILTER_EFFECTS];

void                FilterEffectInitForApp();
void                FilterEffectDisposeForApp();
//void                FilterEffectDoLumSat(MemoryImage* theImage, float theLum, float theSat);
///*inline*/ void     FilterEffectDoWashedOut(MemoryImage* theImage);
///*inline*/ void     FilterEffectDoLessWashedOut(MemoryImage* theImage);
//void                FilterEffectDoWhite(MemoryImage* theImage);
//MemoryImage*        FilterEffectCreateImage(Image* theImage, FilterEffect theFilterEffect);
Image*              FilterEffectGetImage(Image* theImage, FilterEffect theFilterEffect);

#endif
