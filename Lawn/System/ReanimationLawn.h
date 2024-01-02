#ifndef __REANIMATORCACHE_H__
#define __REANIMATORCACHE_H__

#include "../../ConstEnums.h"
#include "graphics/Image.h"

#include <memory>
#include <sys/types.h>

namespace Sexy
{
    class Graphics;
};
using namespace Sexy;

class LawnApp;

class Reanimation;
class ReanimatorCache
{
public:
    std::map<uint64_t, std::unique_ptr<Image>> mImageVariationMap;
    std::unique_ptr<Image>              mPlantImages[SeedType::NUM_SEED_TYPES] = {};
    std::unique_ptr<Image>              mLawnMowers[LawnMowerType::NUM_MOWER_TYPES] = {};
    std::unique_ptr<Image>              mZombieImages[ZombieType::NUM_CACHED_ZOMBIE_TYPES] = {};

public:
    //ReanimatorCache() { ReanimatorCacheInitialize(); }
    //~ReanimatorCache() { ReanimatorCacheDispose(); }

    void                    DrawCachedPlant(Graphics* g, float thePosX, float thePosY, SeedType theSeedType, DrawVariation theDrawVariation);
    void                    DrawCachedMower(Graphics* g, float thePosX, float thePosY, LawnMowerType theMowerType);
    void                    DrawCachedZombie(Graphics* g, float thePosX, float thePosY, ZombieType theZombieType);
    std::unique_ptr<Image>  MakeBlankImage(int theWidth, int theHeight);
    std::unique_ptr<Image>  MakeCachedPlantFrame(SeedType theSeedType, DrawVariation theDrawVariation);
    std::unique_ptr<Image>  MakeCachedMowerFrame(LawnMowerType theMowerType);
    std::unique_ptr<Image>  MakeCachedZombieFrame(ZombieType theZombieType);
    /*inline*/ void         GetPlantImageSize(SeedType theSeedType, int& theOffsetX, int& theOffsetY, int& theWidth, int& theHeight);
    void                    DrawReanimatorFrame(Graphics* g, float thePosX, float thePosY, ReanimationType theReanimationType, const char* theTrackName, DrawVariation theDrawVariation);
    void                    UpdateReanimationForVariation(Reanimation* theReanim, DrawVariation theDrawVariation);
};

#endif
