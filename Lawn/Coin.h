#ifndef __COIN_H__
#define __COIN_H__

#include "GameObject.h"
#include "System/PlayerInfo.h"

class HitResult;
namespace Sexy
{
    class Graphics;
}
using namespace Sexy;

class Coin : public GameObject
{
public:
    float                   mPosX;                  //+0x24
    float                   mPosY;                  //+0x28
    float                   mVelX;                  //+0x2C
    float                   mVelY;                  //+0x30
    float                   mScale;                 //+0x34
    bool                    mDead;                  //+0x38
    int                     mFadeCount;             //+0x3C
    float                   mCollectX;              //+0x40
    float                   mCollectY;              //+0x44
    int                     mGroundY;               //+0x48
    int                     mCoinAge;               //+0x4C
    bool                    mIsBeingCollected;      //+0x50
    int                     mDisappearCounter;      //+0x54
    CoinType                mType;                  //+0x58
    CoinMotion              mCoinMotion;            //+0x5C
    AttachmentID            mAttachmentID;          //+0x60
    float                   mCollectionDistance;    //+0x64
    SeedType                mUsableSeedType;        //+0x68
    PottedPlant             mPottedPlantSpec;       //+0x70
    bool                    mNeedsBouncyArrow;      //+0xC8
    bool                    mHasBouncyArrow;        //+0xC9
    bool                    mHitGround;             //+0xCA
    int                     mTimesDropped;          //+0xCC

public:
    Coin();
    ~Coin();

    void                    CoinInitialize(int theX, int theY, CoinType theCoinType, CoinMotion theCoinMotion);
    void                    MouseDown(int x, int y, int theClickCount);
    bool                    MouseHitTest(int theX, int theY, HitResult* theHitResult);
    void                    Die();
    void                    StartFade();
    void                    Update();
    void                    Draw(Graphics* g);
    void                    Collect();
    /*inline*/ int          GetSunValue();
    static /*inline*/ int   GetCoinValue(CoinType theCoinType);
    void                    UpdateFade();
    void                    UpdateFall();
    void                    ScoreCoin();
    void                    UpdateCollected();
    Color                   GetColor();
    /*inline*/ bool         IsMoney();
    /*inline*/ bool         IsSun();
    float                   GetSunScale();
    inline bool             IsOnGround() { return false; }
    SeedType                GetFinalSeedPacketType();
    bool                    IsLevelAward();
    bool                    CoinGetsBouncyArrow();
    void                    FanOutCoins(CoinType theCoinType, int theNumCoins);
    int                     GetDisappearTime();
    void                    DroppedUsableSeed();
    void                    PlayCollectSound();
    void                    TryAutoCollectAfterLevelAward();
    bool                    IsPresentWithAdvice();
    void                    PlayLaunchSound();
    void                    PlayGroundSound();

    static /*inline*/ bool  IsMoney(CoinType theType);
};

#endif
