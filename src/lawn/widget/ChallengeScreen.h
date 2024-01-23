#ifndef __CHALLENGESCREEN_H__
#define __CHALLENGESCREEN_H__

#include "ConstEnums.h"
#include "framework/widget/Dialog.h"
using namespace Sexy;

#define NUM_CHALLENGE_MODES ((int)GameMode::NUM_GAME_MODES - 1)

class LawnApp;
class ToolTipWidget;
class NewLawnButton;
class ChallengeScreen : public Widget, public ButtonListener
{
private:
    enum
    {
        ChallengeScreen_Back = 100,
        ChallengeScreen_Mode = 200,
        ChallengeScreen_Page = 300
    };

public:
    NewLawnButton*              mBackButton;                                //+0x8C
    ButtonWidget*               mPageButton[MAX_CHALLANGE_PAGES];           //+0x90
    ButtonWidget*               mChallengeButtons[NUM_CHALLENGE_MODES];     //+0xA0
    LawnApp*                    mApp;                                       //+0x1C0
    ToolTipWidget*              mToolTip;                                   //+0x1C4
    ChallengePage               mPageIndex;                                 //+0x1C8
    bool                        mCheatEnableChallenges;                     //+0x1CC
    UnlockingState              mUnlockState;                               //+0x1D0
    int                         mUnlockStateCounter;                        //+0x1D4
    int                         mUnlockChallengeIndex;                      //+0x1D8
    float                       mLockShakeX;                                //+0x1DC
    float                       mLockShakeY;                                //+0x1E0

public:
    ChallengeScreen(LawnApp* theApp, ChallengePage thePage);
    virtual ~ChallengeScreen();
    void                        SetUnlockChallengeIndex(ChallengePage thePage, bool theIsIZombie = false);
    int                         MoreTrophiesNeeded(int theChallengeIndex);
    /*inline*/ bool             ShowPageButtons();
    void                        UpdateButtons();
    int                         AccomplishmentsNeeded(int theChallengeIndex);
    void                        DrawButton(Graphics* g, int theChallengeIndex);
    virtual void                Draw(Graphics* g);
    virtual void                Update();
    virtual void                AddedToManager(WidgetManager* theWidgetManager);
    virtual void                RemovedFromManager(WidgetManager* theWidgetManager);
    virtual void                ButtonPress(int theId);
    virtual void                ButtonDownTick(int){}
    virtual void                ButtonMouseEnter(int){}
    virtual void                ButtonMouseLeave(int){}
    virtual void                ButtonMouseMove(int, int, int){}
    virtual void                ButtonDepress(int theId);
    void                        UpdateToolTip();
//  virtual void                KeyChar(char theChar);

    /*inline*/ bool             IsScaryPotterLevel(GameMode theGameMode);
    /*inline*/ bool             IsIZombieLevel(GameMode theGameMode);
};

class ChallengeDefinition
{
public:
    GameMode                    mChallengeMode;
    int                         mChallengeIconIndex;
    ChallengePage               mPage;
    int                         mRow;
    int                         mCol;
    const SexyChar*             mChallengeName;
};
extern ChallengeDefinition gChallengeDefs[NUM_CHALLENGE_MODES];

ChallengeDefinition& GetChallengeDefinition(int theChallengeMode);

#endif