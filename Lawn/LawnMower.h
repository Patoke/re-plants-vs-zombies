#ifndef __LAWNMOWER_H__
#define __LAWNMOWER_H__

#include "../ConstEnums.h"
#include "../SexyAppFramework/Rect.h"

class LawnApp;
class Board;
class Zombie;
namespace Sexy
{
    class Graphics;
};
using namespace Sexy;

class LawnMower
{
public:
    LawnApp*            mApp;                   //+0x0
    Board*              mBoard;                 //+0x4
    float               mPosX;                  //+0x8
    float               mPosY;                  //+0xC
    int                 mRenderOrder;           //+0x10
    int                 mRow;                   //+0x14
    int                 mAnimTicksPerFrame;     //+0x18
    ReanimationID       mReanimID;              //+0x1C
    int                 mChompCounter;          //+0x20
    int                 mRollingInCounter;      //+0x24
    int                 mSquishedCounter;       //+0x28
    LawnMowerState      mMowerState;            //+0x2C
    bool                mDead;                  //+0x30
    bool                mVisible;               //+0x31
    LawnMowerType       mMowerType;             //+0x34
    float               mAltitude;              //+0x38
    MowerHeight         mMowerHeight;           //+0x3C
    int                 mLastPortalX;           //+0x40

public:
    void                LawnMowerInitialize(int theRow);
    void                StartMower();
    void                Update();
    void                Draw(Graphics* g);
    void                Die();
    Rect                GetLawnMowerAttackRect();
    void                UpdatePool();
    void                MowZombie(Zombie* theZombie);
    void                SquishMower();
    /*inline*/ void     EnableSuperMower(bool theEnable);
};

#endif
