#ifndef __GAMESELECTOR_H__
#define __GAMESELECTOR_H__

#include "../../ConstEnums.h"
#include "../../SexyAppFramework/Widget.h"
#include "../../SexyAppFramework/ButtonListener.h"

class LawnApp;
class ToolTipWidget;
namespace Sexy
{
    class DialogButton;
}

using namespace Sexy;

enum SelectorAnimState
{
    SELECTOR_OPEN,
    SELECTOR_NEW_USER,
    SELECTOR_SHOW_SIGN,
    SELECTOR_IDLE
};

class GameSelector : public Widget, public ButtonListener
{
private:
    enum
    {
        GameSelector_Adventure = 100,
        GameSelector_Minigame = 101,
        GameSelector_Puzzle = 102,
        GameSelector_Options = 103,
        GameSelector_Help = 104,
        GameSelector_Quit = 105,
        GameSelector_ChangeUser = 106,
        GameSelector_Store = 107,
        GameSelector_Almanac = 108,
        GameSelector_ZenGarden = 109,
        GameSelector_Survival = 110
    };

public:
    LawnApp*                    mApp;                       //+0x8C
    NewLawnButton*              mAdventureButton;           //+0x90
    NewLawnButton*              mMinigameButton;            //+0x94
    NewLawnButton*              mPuzzleButton;              //+0x98
    NewLawnButton*              mOptionsButton;             //+0x9C
    NewLawnButton*              mQuitButton;                //+0xA0
    NewLawnButton*              mHelpButton;                //+0xA4
    NewLawnButton*              mStoreButton;               //+0xA8
    NewLawnButton*              mAlmanacButton;             //+0xAC
    NewLawnButton*              mZenGardenButton;           //+0xB0
    NewLawnButton*              mSurvivalButton;            //+0xB4
    NewLawnButton*              mChangeUserButton;          //+0xB8
    Widget*                     mOverlayWidget;             //+0xBC
    bool                        mStartingGame;              //+0xC0
    int                         mStartingGameCounter;       //+0xC4
    bool                        mMinigamesLocked;           //+0xC8
    bool                        mPuzzleLocked;              //+0xC9
    bool                        mSurvivalLocked;            //+0xCA
    bool                        mShowStartButton;           //+0xCB
    ParticleSystemID            mTrophyParticleID;          //+0xCC
    ReanimationID               mSelectorReanimID;          //+0xD0
    ReanimationID               mCloudReanimID[6];          //+0xD4
    int                         mCloudCounter[6];           //+0xEC
    ReanimationID               mFlowerReanimID[3];         //+0x104
    ReanimationID               mLeafReanimID;              //+0x110
    ReanimationID               mHandReanimID;              //+0x114
    int                         mLeafCounter;               //+0x118
    SelectorAnimState           mSelectorState;             //+0x11C
    int                         mLevel;                     //+0x120
    bool                        mLoading;                   //+0x124
    ToolTipWidget*              mToolTip;                   //+0x128
    bool                        mHasTrophy;                 //+0x12C
    bool                        mUnlockSelectorCheat;       //+0x12D

public:
    GameSelector(LawnApp* theApp);
    virtual ~GameSelector();

    void                        SyncProfile(bool theShowLoading);
    virtual void                Draw(Graphics* g);
    virtual void                DrawOverlay(Graphics* g);
    virtual void                Update();
    virtual void                AddedToManager(WidgetManager* theWidgetManager);
    virtual void                RemovedFromManager(WidgetManager* theWidgetManager);
    virtual void                OrderInManagerChanged();
    virtual void                ButtonMouseEnter(int theId);
    virtual void                ButtonPress(int theId, int theClickCount);
    virtual void                ButtonDepress(int theId);
    virtual void                KeyDown(KeyCode theKey);
    virtual void                KeyChar(char theChar);
    virtual void                MouseDown(int x, int y, int theClickCount);
    void                        TrackButton(DialogButton* theButton, const char* theTrackName, float theOffsetX, float theOffsetY);
    void                        SyncButtons();
    void                        AddTrophySparkle();
    void                        ClickedAdventure();
    void                        UpdateTooltip();
    /*inline*/ bool             ShouldDoZenTuturialBeforeAdventure();
    void                        AddPreviewProfiles();
};

class GameSelectorOverlay : public Widget
{
public:
    GameSelector*               mParent;                    //+0x88

public:
    GameSelectorOverlay(GameSelector* theGameSelector);
    virtual ~GameSelectorOverlay() { }

    virtual void Draw(Graphics* g);
};

#endif
