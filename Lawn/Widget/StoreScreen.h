#ifndef __STORESCREEN_H__
#define __STORESCREEN_H__

#include "../../ConstEnums.h"
#include "../System/PlayerInfo.h"
#include "../../Sexy.TodLib/DataArray.h"
#include "widget/Dialog.h"
//using namespace std;
using namespace Sexy;

#define MAX_PAGE_SPOTS 8
#define MAX_PURCHASES 80

class Coin;
class LawnApp;
class NewLawnButton;

class StoreScreen : public Dialog
{
private:
    enum
    {
        StoreScreen_Back = 100,
        StoreScreen_Prev = 101,
        StoreScreen_Next = 102
    };

public:
	LawnApp*					mApp;                           //+0x150
	NewLawnButton*				mBackButton;                    //+0x154
	NewLawnButton*				mPrevButton;                    //+0x158
	NewLawnButton*				mNextButton;                    //+0x15C
    Widget*                     mOverlayWidget;                 //+0x160
	int                         mStoreTime;                     //+0x164
	std::string                      mBubbleText;                    //+0x168
    int                         mBubbleCountDown;               //+0x184
    bool                        mBubbleClickToContinue;         //+0x188
    int                         mAmbientSpeechCountDown;        //+0x18C
    int                         mPreviousAmbientSpeechIndex;    //+0x190
    StorePages                  mPage;                          //+0x194
    StoreItem                   mMouseOverItem;                 //+0x198
    int                         mHatchTimer;                    //+0x19C
    bool                        mHatchOpen;                     //+0x1A0
    int                         mShakeX;                        //+0x1A4
    int                         mShakeY;                        //+0x1A8
    int                         mStartDialog;                   //+0x1AC
    bool                        mEasyBuyingCheat;               //+0x1B0
    bool                        mWaitForDialog;                 //+0x1B1
    PottedPlant                 mPottedPlantSpecs;              //+0x1B8
    DataArray<Coin>             mCoins;                         //+0x210
    bool                        mDrawnOnce;                     //+0x22C
    bool                        mGoToTreeNow;                   //+0x22D
    bool                        mPurchasedFullVersion;          //+0x22E
    bool                        mTrialLockedWhenStoreOpened;    //+0x22F

public:
    StoreScreen(LawnApp* theApp);
    virtual ~StoreScreen();

    /*inline*/ StoreItem        GetStoreItemType(int theSpotIndex);
    bool                        IsFullVersionOnly(StoreItem theStoreItem);
    static /*inline*/ bool      IsPottedPlant(StoreItem theStoreItem);
    bool                        IsComingSoon(StoreItem theStoreItem);
    bool                        IsItemSoldOut(StoreItem theStoreItem);
    bool                        IsItemUnavailable(StoreItem theStoreItem);
    static /*inline*/ void      GetStorePosition(int theSpotIndex, int& thePosX, int& thePosY);
    void                        DrawItemIcon(Graphics* g, int theItemPosition, StoreItem theItemType, bool theIsForHighlight);
    void                        DrawItem(Graphics* g, int theItemPosition, StoreItem theItemType);
    virtual void                Draw(Graphics* g);
    virtual void                DrawOverlay(Graphics* g);
    /*inline*/ void             SetBubbleText(int theCrazyDaveMessage, int theTime, bool theClickToContinue);
    void                        UpdateMouse();
    void                        StorePreload();
    /*inline*/ bool             CanInteractWithButtons();
    virtual void                Update();
    virtual void                AddedToManager(WidgetManager* theWidgetManager);
    virtual void                RemovedFromManager(WidgetManager* theWidgetManager);
    virtual void                ButtonPress(int theId);
    /*inline*/ bool             IsPageShown(StorePages thePage);
    virtual void                ButtonDepress(int theId);
    virtual void                KeyChar(char theChar);
    static /*inline*/ int		GetItemCost(StoreItem theStoreItem);
    /*inline*/ bool             CanAffordItem(StoreItem theStoreItem);
    void                        PurchaseItem(StoreItem theStoreItem);
    void                        AdvanceCrazyDaveDialog();
    virtual void                MouseDown(int x, int y, int theClickCount);
    /*inline*/ void             EnableButtons(bool theEnable);
    void                        SetupForIntro(int theDialogIndex);
};

class StoreScreenOverlay : public Widget
{
public:
    StoreScreen*                mParent;

public:
    StoreScreenOverlay(StoreScreen* theParent);
    virtual void                Draw(Graphics* g);
};


#endif
