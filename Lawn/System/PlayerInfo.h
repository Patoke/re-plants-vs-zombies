#ifndef __PLAYERINFO_H__
#define __PLAYERINFO_H__

#define MAX_NUM_ZOMBATARS 100
#define MAX_POTTED_PLANTS 200
#define PURCHASE_COUNT_OFFSET 1000

#include <ctime>
#include "../../ConstEnums.h"
#include "../../SexyAppFramework/Common.h"

class PottedPlant
{
public:
    enum FacingDirection
    {
        FACING_RIGHT,
        FACING_LEFT
    };

public:
    SeedType            mSeedType;                  //+0x0
    GardenType          mWhichZenGarden;            //+0x4
    int                 mX;                         //+0x8
    int                 mY;                         //+0xC
    FacingDirection     mFacing;                    //+0x10

    __time64_t          mLastWateredTime;           //+0x18
    DrawVariation       mDrawVariation;             //+0x20
    PottedPlantAge      mPlantAge;                  //+0x24
    int                 mTimesFed;                  //+0x28
    int                 mFeedingsPerGrow;           //+0x2C
    PottedPlantNeed     mPlantNeed;                 //+0x30

    __time64_t          mLastNeedFulfilledTime;     //+0x38
    __time64_t          mLastFertilizedTime;        //+0x40
    __time64_t          mLastChocolateTime;         //+0x48
    __time64_t          mFutureAttribute[1];        //+0x50

public:
    void                InitializePottedPlant(SeedType theSeedType);
};

class Zombatar
{
public:

    int mSkin;
    int mSkinColor;
    int mClothes;
    int mClothesColor;
    int mTidbits;
    int mTidbitsColor;
    int mAccessories;
    int mAccessoriesColor;
    int mFacialHair;
    int mFacialHairColor;
    int mHair;
    int mHairColor;
    int mEyewear;
    int mEyewearColor;
    int mHat;
    int mHatColor;
    int mBackdrop;
    int mBackdropColor;
};

class DataSync;
class PlayerInfo
{
public:
    SexyString          mName;                              //+GOTY @Patoke: 0x0
    ulong               mUseSeq;                            //+GOTY @Patoke: 0x1C
    ulong               mId;                                //+GOTY @Patoke: 0x20
	bool                mEarnedAchievements[20];            //+GOTY @Patoke: 0x24
	bool                mShownAchievements[20];             //+GOTY @Patoke: 0x38
	int                 mLevel;                             //+GOTY @Patoke: 0x4C
	int                 mCoins;                             //+GOTY @Patoke: 0x50
	int                 mFinishedAdventure;                 //+GOTY @Patoke: 0x54
	int                 mChallengeRecords[100];             //+GOTY @Patoke: 0x58
	long                mPurchases[80];                     //+GOTY @Patoke: 0x1E8
	int                 mPlayTimeActivePlayer;              //+GOTY @Patoke: 0x328
	int                 mPlayTimeInactivePlayer;            //+GOTY @Patoke: 0x32C
	int                 mHasUsedCheatKeys;                  //+GOTY @Patoke: 0x330
	int                 mHasWokenStinky;                    //+GOTY @Patoke: 0x334
	int                 mDidntPurchasePacketUpgrade;        //+GOTY @Patoke: 0x338
	long                mLastStinkyChocolateTime;           //+GOTY @Patoke: 0x33C
	int                 mStinkyPosX;                        //+GOTY @Patoke: 0x340
	int                 mStinkyPosY;                        //+GOTY @Patoke: 0x344
	int                 mHasUnlockedMinigames;              //+GOTY @Patoke: 0x348
	int                 mHasUnlockedPuzzleMode;             //+GOTY @Patoke: 0x34C
	int                 mHasNewMiniGame;                    //+GOTY @Patoke: 0x350
	int                 mHasNewScaryPotter;                 //+GOTY @Patoke: 0x354
	int                 mHasNewIZombie;                     //+GOTY @Patoke: 0x358
	int                 mHasNewSurvival;                    //+GOTY @Patoke: 0x35C
	int                 mHasUnlockedSurvivalMode;           //+GOTY @Patoke: 0x360
	int                 mNeedsMessageOnGameSelector;        //+GOTY @Patoke: 0x364
	int                 mNeedsMagicTacoReward;              //+GOTY @Patoke: 0x368
	int                 mHasSeenStinky;                     //+GOTY @Patoke: 0x36C
	int                 mHasSeenUpsell;                     //+GOTY @Patoke: 0x370
	int                 mPlaceHolderPlayerStats;            //+GOTY @Patoke: 0x374
	int                 mNumPottedPlants;                   //+GOTY @Patoke: 0x378
	bool                mShownZombatarDesktopMessage;       //+GOTY @Patoke: 0x37C
	bool                mAcceptedZombatarULA;               //+GOTY @Patoke: 0x37D
    PottedPlant         mPottedPlant[MAX_POTTED_PLANTS];    //+GOTY @Patoke: 0x380
    int                 mNumZombatars;                      //+GOTY @Patoke: 0x4840
    Zombatar            mZombatars[MAX_NUM_ZOMBATARS];      //+GOTY @Patoke: 0x4844
    bool                mMiniGamesCompleted[20];            //+GOTY @Patoke: 0x6464

public:
    PlayerInfo();

    void                Reset();
    /*inline*/ void     AddCoins(int theAmount);
    void                SyncSummary(DataSync& theSync);
    void                SyncDetails(DataSync& theSync);
    void                DeleteUserFiles();
    void                LoadDetails();
    void                SaveDetails();
    inline int          GetLevel() const { return mLevel; }
    inline void         SetLevel(int theLevel) { mLevel = theLevel; }
    /*inline*/ void     ResetChallengeRecord(GameMode theGameMode);
};

#endif
