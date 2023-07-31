#include <time.h>
#include "../Coin.h"
#include "../Board.h"
#include "../Plant.h"
#include "LawnDialog.h"
#include "GameButton.h"
#include "StoreScreen.h"
#include "../ZenGarden.h"
#include "../SeedPacket.h"
#include "../../LawnApp.h"
#include "../../Resources.h"
#include "../System/Music.h"
#include "SeedChooserScreen.h"
#include "../../GameConstants.h"
#include "../System/PopDRMComm.h"
#include "../../Sexy.TodLib/TodFoley.h"
#include "../../Sexy.TodLib/TodCommon.h"
#include "../../Sexy.TodLib/Reanimator.h"
#include "../../SexyAppFramework/Debug.h"
#include "../../Sexy.TodLib/TodStringFile.h"
#include "../../SexyAppFramework/ImageFont.h"
#include "../../SexyAppFramework/WidgetManager.h"

static StoreItem gStoreItemSpots[NUM_STORE_PAGES][MAX_PAGE_SPOTS] =
{
    { STORE_ITEM_PACKET_UPGRADE,    STORE_ITEM_POOL_CLEANER,        STORE_ITEM_RAKE,                STORE_ITEM_ROOF_CLEANER,
      STORE_ITEM_PLANT_GATLINGPEA,  STORE_ITEM_PLANT_TWINSUNFLOWER, STORE_ITEM_PLANT_GLOOMSHROOM,   STORE_ITEM_PLANT_CATTAIL },
    { STORE_ITEM_PLANT_SPIKEROCK,   STORE_ITEM_PLANT_GOLD_MAGNET,   STORE_ITEM_PLANT_WINTERMELON,   STORE_ITEM_PLANT_COBCANNON,
      STORE_ITEM_PLANT_IMITATER,    STORE_ITEM_FIRSTAID,            STORE_ITEM_INVALID,             STORE_ITEM_INVALID },
    { STORE_ITEM_POTTED_MARIGOLD_1, STORE_ITEM_POTTED_MARIGOLD_2,   STORE_ITEM_POTTED_MARIGOLD_3,   STORE_ITEM_GOLD_WATERINGCAN,
      STORE_ITEM_FERTILIZER,        STORE_ITEM_BUG_SPRAY,           STORE_ITEM_PHONOGRAPH,          STORE_ITEM_GARDENING_GLOVE },
    { STORE_ITEM_MUSHROOM_GARDEN,   STORE_ITEM_AQUARIUM_GARDEN,     STORE_ITEM_WHEEL_BARROW,        STORE_ITEM_STINKY_THE_SNAIL,
      STORE_ITEM_TREE_OF_WISDOM,    STORE_ITEM_TREE_FOOD,           STORE_ITEM_INVALID,             STORE_ITEM_INVALID }
};

StoreScreenOverlay::StoreScreenOverlay(StoreScreen* theParent)
{
    mParent = theParent;
    mMouseVisible = false;
    mHasAlpha = true;
}

void StoreScreenOverlay::Draw(Graphics* g)
{
    mParent->DrawOverlay(g);
}

//0x489DA0
StoreScreen::StoreScreen(LawnApp* theApp) : Dialog(nullptr, nullptr, DIALOG_STORE, true, _S("Store"), _S(""), _S(""), BUTTONS_NONE)
{
	mApp = theApp;
    mClip = false;
    mStoreTime = 0;
    mBubbleCountDown = 0;
    mBubbleClickToContinue = false;
    mAmbientSpeechCountDown = 200;
    mPreviousAmbientSpeechIndex = -1;
    mPage = STORE_PAGE_SLOT_UPGRADES;
    mMouseOverItem = STORE_ITEM_INVALID;
    mHatchTimer = 0;
    mShakeX = 0;
    mShakeY = 0;
    mStartDialog = -1;
    mHatchOpen = true;
    mEasyBuyingCheat = false;
    mWaitForDialog = false;
    mCoins.DataArrayInitialize(1024U, "coins");
    TodLoadResources("DelayLoad_Store");
    Resize(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
    mPottedPlantSpecs.InitializePottedPlant(SEED_MARIGOLD);
    mPottedPlantSpecs.mDrawVariation = (DrawVariation)RandRangeInt(VARIATION_MARIGOLD_WHITE, VARIATION_MARIGOLD_LIGHT_GREEN);

    mBackButton = new NewLawnButton(nullptr, StoreScreen::StoreScreen_Back, this);
    mBackButton->mDoFinger = true;
    mBackButton->SetLabel(_S("[STORE_MAIN_MENU_BUTTON]"));
    Image* aMenuImage = Sexy::IMAGE_STORE_MAINMENUBUTTON;
    mBackButton->mButtonImage = aMenuImage;
    mBackButton->mOverImage = Sexy::IMAGE_STORE_MAINMENUBUTTONHIGHLIGHT;
    mBackButton->mDownImage = Sexy::IMAGE_STORE_MAINMENUBUTTONDOWN;
    mBackButton->SetFont(Sexy::FONT_HOUSEOFTERROR20);
    mBackButton->mColors[ButtonWidget::COLOR_LABEL] = Color(98, 153, 235);
    mBackButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(167, 192, 235);
    mBackButton->Resize(366, 512, aMenuImage->mWidth, aMenuImage->mHeight);
    mBackButton->mTextOffsetX = -7;
    mBackButton->mTextOffsetY = 1;
    mBackButton->mTextDownOffsetX = 2;
    mBackButton->mTextDownOffsetY = 1;

    mPrevButton = new NewLawnButton(nullptr, StoreScreen::StoreScreen_Prev, this);
    mPrevButton->mDoFinger = true;
    mPrevButton->SetLabel(_S(""));
    Image* aPrevImage = Sexy::IMAGE_STORE_PREVBUTTON;
    mPrevButton->mButtonImage = aPrevImage;
    mPrevButton->mOverImage = Sexy::IMAGE_STORE_PREVBUTTONHIGHLIGHT;
    mPrevButton->mDownImage = Sexy::IMAGE_STORE_PREVBUTTONHIGHLIGHT;
    mPrevButton->mColors[ButtonWidget::COLOR_LABEL] = Color(255, 240, 0);
    mPrevButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(200, 200, 255);
    mPrevButton->Resize(252, 402, aPrevImage->mWidth, aPrevImage->mHeight);

    mNextButton = new NewLawnButton(nullptr, StoreScreen::StoreScreen_Next, this);
    mNextButton->mDoFinger = true;
    mNextButton->SetLabel(_S(""));
    Image* aNextImage = Sexy::IMAGE_STORE_NEXTBUTTON;
    mNextButton->mButtonImage = aNextImage;
    mNextButton->mOverImage = Sexy::IMAGE_STORE_NEXTBUTTONHIGHLIGHT;
    mNextButton->mDownImage = Sexy::IMAGE_STORE_NEXTBUTTONHIGHLIGHT;
    mNextButton->mColors[ButtonWidget::COLOR_LABEL] = Color(255, 240, 0);
    mNextButton->mColors[ButtonWidget::COLOR_LABEL_HILITE] = Color(200, 200, 255);
    mNextButton->Resize(596, 402, aNextImage->mWidth, aNextImage->mHeight);

    mOverlayWidget = new StoreScreenOverlay(this);
    mOverlayWidget->Resize(0, 0, BOARD_WIDTH, BOARD_HEIGHT);

    if (!IsPageShown(STORE_PAGE_PLANT_UPGRADES))
    {
        mPrevButton->mDisabledImage = Sexy::IMAGE_STORE_PREVBUTTONDISABLED;
        mPrevButton->SetDisabled(true);
        mNextButton->mDisabledImage = Sexy::IMAGE_STORE_NEXTBUTTONDISABLED;
        mNextButton->SetDisabled(true);
    }
    mDrawnOnce = false;
    mGoToTreeNow = false;
    mPurchasedFullVersion = false;
    mTrialLockedWhenStoreOpened = mApp->IsTrialStageLocked();
}

//0x48A610��0x48A630
StoreScreen::~StoreScreen()
{
    mCoins.DataArrayDispose();
    if (mBackButton) delete mBackButton;
    if (mPrevButton) delete mPrevButton;
    if (mNextButton) delete mNextButton;
    if (mOverlayWidget) delete mOverlayWidget;
}

//0x48A760
StoreItem StoreScreen::GetStoreItemType(int theSpotIndex)
{
    // �������ԭ��������жϵģ������Ż�һ�¡���

    if (mPage < NUM_STORE_PAGES && theSpotIndex < MAX_PAGE_SPOTS)
    {
        if (mPage == STORE_PAGE_SLOT_UPGRADES && theSpotIndex == 6 && mApp->IsTrialStageLocked())
        {
            return STORE_ITEM_PVZ;
        }
        return gStoreItemSpots[mPage][theSpotIndex];
    }

    TOD_ASSERT();
    return STORE_ITEM_INVALID;
}

//0x48A8D0
bool StoreScreen::IsFullVersionOnly(StoreItem theStoreItem)
{
    if (!mApp->IsTrialStageLocked())
        return false;

    if (theStoreItem == STORE_ITEM_PACKET_UPGRADE && mApp->mPlayerInfo->mPurchases[STORE_ITEM_PACKET_UPGRADE] >= 2)
        return true;
    
    return theStoreItem == STORE_ITEM_PLANT_TWINSUNFLOWER;
}

bool StoreScreen::IsPottedPlant(StoreItem theStoreItem)
{
    return theStoreItem == STORE_ITEM_POTTED_MARIGOLD_1 || theStoreItem == STORE_ITEM_POTTED_MARIGOLD_2 || theStoreItem == STORE_ITEM_POTTED_MARIGOLD_3;
}

//0x48A940
bool StoreScreen::IsComingSoon(StoreItem theStoreItem)
{
    if (IsFullVersionOnly(theStoreItem))
        return true;
    else if (theStoreItem == STORE_ITEM_WHEEL_BARROW)
        return !mApp->mPlayerInfo->mPurchases[STORE_ITEM_MUSHROOM_GARDEN] && !mApp->mPlayerInfo->mPurchases[STORE_ITEM_AQUARIUM_GARDEN];
    else if (IsPottedPlant(theStoreItem))
        return !mApp->HasFinishedAdventure();
    else if (theStoreItem == STORE_ITEM_TREE_FOOD)
        return !mApp->mPlayerInfo->mPurchases[STORE_ITEM_TREE_OF_WISDOM] || mApp->mPlayerInfo->mPurchases[STORE_ITEM_TREE_FOOD] < PURCHASE_COUNT_OFFSET;
    return false;
}

//0x48A9D0
bool StoreScreen::IsItemSoldOut(StoreItem theStoreItem)
{
    PlayerInfo* aPlayer = mApp->mPlayerInfo;
    if (theStoreItem == STORE_ITEM_INVALID)
        return false;
    else if (theStoreItem == STORE_ITEM_PACKET_UPGRADE)
        return aPlayer->mPurchases[STORE_ITEM_PACKET_UPGRADE] >= 4;
    else if (theStoreItem == STORE_ITEM_FERTILIZER || theStoreItem == STORE_ITEM_BUG_SPRAY)
        return aPlayer->mPurchases[theStoreItem] - PURCHASE_COUNT_OFFSET > 15;
    else if (theStoreItem == STORE_ITEM_TREE_FOOD)
        return aPlayer->mPurchases[STORE_ITEM_TREE_FOOD] - PURCHASE_COUNT_OFFSET > 10;
    else if (theStoreItem == STORE_ITEM_BONUS_LAWN_MOWER)
        return aPlayer->mPurchases[STORE_ITEM_BONUS_LAWN_MOWER] >= 2;
    else if (IsPottedPlant(theStoreItem))
        return mApp->mZenGarden->IsZenGardenFull(true) || aPlayer->mPurchases[theStoreItem] == GetCurrentDaysSince2000();
    else return aPlayer->mPurchases[theStoreItem];
}

//0x48AAD0
bool StoreScreen::IsItemUnavailable(StoreItem theStoreItem)
{
    if (mEasyBuyingCheat)
        return false;

    /*
    if (mApp->HasFinishedAdventure())
        return true;

    bool aTrialStageLocked = mApp->IsTrialStageLocked();
    int aCurrentLevel = mApp->mPlayerInfo->mLevel;
    if (theStoreItem == STORE_ITEM_ROOF_CLEANER)
    {
        return aTrialStageLocked || aCurrentLevel < 42;
    }
    else if (theStoreItem == STORE_ITEM_PLANT_GLOOMSHROOM || theStoreItem == STORE_ITEM_PLANT_CATTAIL)
    {
        return aTrialStageLocked || aCurrentLevel < 35;
    }
    else if (theStoreItem == STORE_ITEM_PLANT_SPIKEROCK || theStoreItem == STORE_ITEM_PLANT_GOLD_MAGNET)
    {
        return aCurrentLevel < 41;
    }

    return 
        theStoreItem != STORE_ITEM_PLANT_WINTERMELON && 
        theStoreItem != STORE_ITEM_PLANT_COBCANNON &&
        theStoreItem != STORE_ITEM_PLANT_IMITATER && 
        theStoreItem != STORE_ITEM_FIRSTAID;
    */

    if (theStoreItem == STORE_ITEM_ROOF_CLEANER)
    {
        return mApp->IsTrialStageLocked() || (!mApp->HasFinishedAdventure() && mApp->mPlayerInfo->GetLevel() < 42);
    }
    if (theStoreItem == STORE_ITEM_PLANT_GLOOMSHROOM)
    {
        return mApp->IsTrialStageLocked() || (!mApp->HasFinishedAdventure() && mApp->mPlayerInfo->GetLevel() < 35);
    }
    if (theStoreItem == STORE_ITEM_PLANT_CATTAIL)
    {
        return mApp->IsTrialStageLocked() || (!mApp->HasFinishedAdventure() && mApp->mPlayerInfo->GetLevel() < 35);
    }
    if (theStoreItem == STORE_ITEM_PLANT_SPIKEROCK)
    {
        return !mApp->HasFinishedAdventure() && mApp->mPlayerInfo->GetLevel() < 41;
    }
    if (theStoreItem == STORE_ITEM_PLANT_GOLD_MAGNET)
    {
        return !mApp->HasFinishedAdventure() && mApp->mPlayerInfo->GetLevel() < 41;
    }
    if (theStoreItem == STORE_ITEM_PLANT_WINTERMELON || theStoreItem == STORE_ITEM_PLANT_COBCANNON ||
        theStoreItem == STORE_ITEM_PLANT_IMITATER || theStoreItem == STORE_ITEM_FIRSTAID)
    {
        return !mApp->HasFinishedAdventure();
    }
    return false;
}

void StoreScreen::GetStorePosition(int theSpotIndex, int& thePosX, int& thePosY)
{
    if (theSpotIndex <= 3)
    {
        thePosX = STORESCREEN_ITEMOFFSET_1_X + STORESCREEN_ITEMSIZE * theSpotIndex;
        thePosY = STORESCREEN_ITEMOFFSET_1_Y;
    }
    else
    {
        thePosX = STORESCREEN_ITEMOFFSET_2_X + STORESCREEN_ITEMSIZE * (theSpotIndex - 4);
        thePosY = STORESCREEN_ITEMOFFSET_2_Y;
    }
}

//0x48AC50
void StoreScreen::DrawItemIcon(Graphics* g, int theItemPosition, StoreItem theItemType, bool theIsForHighlight)
{
    if (theIsForHighlight)
    {
        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        g->SetColor(Color(255, 255, 255, 96));
        g->SetColorizeImages(true);
    }

    int aPosX, aPosY;
    GetStorePosition(theItemPosition, aPosX, aPosY);
    if (theItemType == STORE_ITEM_PACKET_UPGRADE)
    {
        g->SetColor(Color(255, 255, 255, 32));
        g->DrawImage(Sexy::IMAGE_STORE_PACKETUPGRADE, aPosX - 7, aPosY + 7);
        if (theIsForHighlight)
        {
            g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
            g->SetColorizeImages(false);
        }

        SexyString aSlotText = TodReplaceNumberString(_S("[STORE_UPGRADE_SLOTS]"), _S("{SLOTS}"), mApp->mPlayerInfo->mPurchases[STORE_ITEM_PACKET_UPGRADE] + 7);
        Rect aRect(aPosX, aPosY + 6, 55, 70);
        TodDrawStringWrapped(g, aSlotText, aRect, Sexy::FONT_HOUSEOFTERROR16, Color::White, DS_ALIGN_CENTER_VERTICAL_MIDDLE);
    }
    else if (theItemType == STORE_ITEM_POOL_CLEANER)
    {
        g->DrawImage(Sexy::IMAGE_ICON_POOLCLEANER, aPosX + 1, aPosY + 7);
    }
    else if (theItemType == STORE_ITEM_RAKE)
    {
        g->DrawImage(Sexy::IMAGE_ICON_RAKE, aPosX - 5, aPosY + 10);
    }
    else if (theItemType == STORE_ITEM_ROOF_CLEANER)
    {
        g->DrawImage(Sexy::IMAGE_ICON_ROOFCLEANER, aPosX, aPosY + 28);
    }
    else if (theItemType == STORE_ITEM_PLANT_IMITATER)
    {
        g->DrawImage(Sexy::IMAGE_IMITATERSEED, aPosX, aPosY);
    }
    else if (theItemType == STORE_ITEM_MUSHROOM_GARDEN)
    {
        g->DrawImage(Sexy::IMAGE_STORE_MUSHROOMGARDENICON, aPosX - 8, aPosY + 2);
    }
    else if (theItemType == STORE_ITEM_AQUARIUM_GARDEN)
    {
        g->DrawImage(Sexy::IMAGE_STORE_AQUARIUMGARDENICON, aPosX - 8, aPosY + 2);
    }
    else if (theItemType == STORE_ITEM_TREE_OF_WISDOM)
    {
        g->DrawImage(Sexy::IMAGE_STORE_TREEOFWISDOMICON, aPosX - 8, aPosY + 2);
    }
    else if (theItemType == STORE_ITEM_FIRSTAID)
    {
        g->DrawImage(Sexy::IMAGE_STORE_FIRSTAIDWALLNUTICON, aPosX - 1, aPosY + 13);
    }
    else if (theItemType == STORE_ITEM_PVZ)
    {
        g->DrawImage(Sexy::IMAGE_STORE_PVZICON, aPosX, aPosY - 9);
    }
    else if (theItemType == STORE_ITEM_TREE_FOOD)
    {
        g->DrawImage(Sexy::IMAGE_TREEFOOD, aPosX - 8, aPosY - 2);
    }
    else if (theItemType == STORE_ITEM_STINKY_THE_SNAIL)
    {
        g->DrawImage(Sexy::IMAGE_REANIM_STINKY_TURN3, aPosX - 24, aPosY + 14);
    }
    else if (theItemType == STORE_ITEM_GOLD_WATERINGCAN)
    {
        g->DrawImage(Sexy::IMAGE_WATERINGCANGOLD, aPosX - 14, aPosY - 4);
    }
    else if (theItemType == STORE_ITEM_FERTILIZER)
    {
        g->DrawImage(Sexy::IMAGE_FERTILIZER, aPosX - 11, aPosY - 2);
        TodDrawString(g, _S("x5"), aPosX + 56, aPosY + 62, Sexy::FONT_HOUSEOFTERROR16, Color::White, DS_ALIGN_RIGHT);
    }
    else if (theItemType == STORE_ITEM_PHONOGRAPH)
    {
        g->DrawImage(Sexy::IMAGE_PHONOGRAPH, aPosX - 12, aPosY + 3);
    }
    else if (theItemType == STORE_ITEM_BUG_SPRAY)
    {
        g->DrawImage(Sexy::IMAGE_BUG_SPRAY, aPosX - 12, aPosY + 3);
        TodDrawString(g, _S("x5"), aPosX + 56, aPosY + 62, Sexy::FONT_HOUSEOFTERROR16, Color::White, DS_ALIGN_RIGHT);
    }
    else if (theItemType == STORE_ITEM_GARDENING_GLOVE)
    {
        g->DrawImage(Sexy::IMAGE_ZEN_GARDENGLOVE, aPosX - 12, aPosY + 3);
    }
    else if (theItemType == STORE_ITEM_WHEEL_BARROW)
    {
        g->DrawImage(Sexy::IMAGE_ZEN_WHEELBARROW, aPosX - 12, aPosY + 3);
    }
    else if (IsPottedPlant(theItemType))
    {
        mApp->mZenGarden->DrawPottedPlantIcon(g, aPosX, aPosY, &mPottedPlantSpecs);
    }
    else
    {
        DrawSeedPacket(g, aPosX, aPosY, (SeedType)(theItemType + 40), SEED_NONE, 0, 255, false, false);
    }

    g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    g->SetColorizeImages(false);
}

//0x48B170
void StoreScreen::DrawItem(Graphics* g, int theItemPosition, StoreItem theItemType)
{
    if (IsItemUnavailable(theItemType))
        return;

    DrawItemIcon(g, theItemPosition, theItemType, false);

    int aPosX, aPosY;
    GetStorePosition(theItemPosition, aPosX, aPosY);
    if (theItemType != STORE_ITEM_PVZ)
    {
        g->DrawImage(Sexy::IMAGE_STORE_PRICETAG, aPosX - 3, aPosY + 70);
        SexyString aCostString = LawnApp::GetMoneyString(GetItemCost(theItemType));
        TodDrawString(g, aCostString, aPosX + 23, aPosY + 85, Sexy::FONT_BRIANNETOD12, Color::Black, DS_ALIGN_CENTER);
    }
    if (IsComingSoon(theItemType))
    {
        Rect aRect(aPosX, aPosY, 60, 70);
        if (theItemType == STORE_ITEM_PLANT_TWINSUNFLOWER || theItemType == STORE_ITEM_PACKET_UPGRADE)
        {
            aRect.mX -= 4;
        }
        TodDrawStringWrapped(g, _S("[COMING_SOON]"), aRect, Sexy::FONT_HOUSEOFTERROR16, Color(255, 0, 0), DS_ALIGN_CENTER_VERTICAL_MIDDLE);
    }
    else if (IsItemSoldOut(theItemType))
    {
        Rect aRect(aPosX, aPosY, 50, 70);
        TodDrawStringWrapped(g, _S("[SOLD_OUT]"), aRect, Sexy::FONT_HOUSEOFTERROR16, Color(255, 0, 0), DS_ALIGN_CENTER_VERTICAL_MIDDLE);
    }
    else if (mMouseOverItem == theItemType)
    {
        if (theItemType >= 0 && theItemType <= 8)
        {
            g->DrawImage(Sexy::IMAGE_SEEDPACKETFLASH, aPosX, aPosY);
        }
        else
        {
            DrawItemIcon(g, theItemPosition, theItemType, true);
        }
    }
}

//0x48B4C0
void StoreScreen::Draw(Graphics* g)
{
    g->SetLinearBlend(true);
    mDrawnOnce = true;

    int aStoreSignPosY = TodAnimateCurve(50, 110, mStoreTime, -150, 0, CURVE_EASE_IN_OUT);
    if (mApp->IsNight())
    {
        g->DrawImage(Sexy::IMAGE_STORE_BACKGROUNDNIGHT, 0, 0);
    }
    else
    {
        g->DrawImage(Sexy::IMAGE_STORE_BACKGROUND, 0, 0);
    }

    if (!mHatchTimer && mHatchOpen)
    {
        g->DrawImage(Sexy::IMAGE_STORE_CAR, mShakeX + 196, mShakeY + 138);
        g->DrawImage(Sexy::IMAGE_STORE_HATCHBACKOPEN, mShakeX + 299, mShakeY);
        if (mApp->IsNight())
        {
            g->DrawImage(Sexy::IMAGE_STORE_CAR_NIGHT, mShakeX + 688, mShakeY + 193);
        }
    }
    else
    {
        g->DrawImage(Sexy::IMAGE_STORE_CARCLOSED, mShakeX + 196, mShakeY + 138);
        if (mApp->IsNight())
        {
            g->DrawImage(Sexy::IMAGE_STORE_CAR_NIGHT, mShakeX + 688, mShakeY + 193);
            g->DrawImage(Sexy::IMAGE_STORE_CARCLOSED_NIGHT, mShakeX + 337, mShakeY + 187);
        }
    }
    g->DrawImage(Sexy::IMAGE_STORE_SIGN, 285, aStoreSignPosY);

    Graphics gCrazyDave = Graphics(*g);
    gCrazyDave.mTransX -= 42.0f;
    gCrazyDave.mTransY += 68.0f;
    mApp->DrawCrazyDave(&gCrazyDave);

    if (!mHatchTimer && mHatchOpen)
    {
        for (int i = 0; i < MAX_PAGE_SPOTS; i++)
        {
            StoreItem aStoreItem = GetStoreItemType(i);
            if (aStoreItem != STORE_ITEM_INVALID)
            {
                DrawItem(g, i, aStoreItem);
            }
        }
    }

    g->DrawImage(Sexy::IMAGE_COINBANK, STORESCREEN_COINBANK_X, STORESCREEN_COINBANK_Y);
    g->SetColor(Color(180, 255, 90));
    g->SetFont(Sexy::FONT_CONTINUUMBOLD14);
    SexyString aCoinLabel = mApp->GetMoneyString(mApp->mPlayerInfo->mCoins);
    g->DrawString(aCoinLabel, STORESCREEN_COINBANK_X + 111 - Sexy::FONT_CONTINUUMBOLD14->StringWidth(aCoinLabel), STORESCREEN_COINBANK_Y + 24);

    if (!mPrevButton->mDisabled)
    {
        int aNumPages = 0;
        for (StorePages aPage = STORE_PAGE_SLOT_UPGRADES; aPage < NUM_STORE_PAGES; aPage = (StorePages)(aPage + 1))
        {
            if (IsPageShown(aPage))
            {
                aNumPages++;
            }
        }

        SexyString aPageString = TodReplaceNumberString(TodReplaceNumberString(_S("[STORE_PAGE]"), _S("{PAGE}"), mPage), _S("{NUM_PAGES}"), aNumPages);
        TodDrawString(g, aPageString, STORESCREEN_PAGESTRING_X, STORESCREEN_COINBANK_Y, Sexy::FONT_BRIANNETOD12, Color(80, 80, 80), DS_ALIGN_CENTER);
    }
}

//0x48BA30
void StoreScreen::DrawOverlay(Graphics* g)
{
    Coin* aCoin = nullptr;
    while (mCoins.IterateNext(aCoin))
    {
        if (!aCoin->mDead)
        {
            aCoin->Draw(g);
        }
    }
}

//0x48BAA0
void StoreScreen::SetBubbleText(int theCrazyDaveMessage, int theTime, bool theClickToContinue)
{
    mApp->CrazyDaveTalkIndex(theCrazyDaveMessage);
    mBubbleCountDown = theTime;
    mBubbleClickToContinue = theClickToContinue;
}

//0x48BAD0
void StoreScreen::UpdateMouse()
{
    mMouseOverItem = STORE_ITEM_INVALID;
    if (mStoreTime < 120 || mBubbleClickToContinue || mHatchTimer > 0 || mWaitForDialog) return;
    int aMouseX = mApp->mWidgetManager->mLastMouseX - mX, aMouseY = mApp->mWidgetManager->mLastMouseY - mY;
    bool aShowFinger = false;
    for (int aItemPos = 0; aItemPos < MAX_PAGE_SPOTS; aItemPos++)
    {
        StoreItem aItemType = GetStoreItemType(aItemPos);
        if (aItemType != STORE_ITEM_INVALID && !IsItemUnavailable(aItemType))
        {
            int aItemX, aItemY;
            GetStorePosition(aItemPos, aItemX, aItemY);
            if (Rect(aItemX, aItemY, 50, 87).Contains(aMouseX, aMouseY))
            {
                mMouseOverItem = aItemType;
                int aMessageIndex = -1;
                switch (aItemType)
                {
                case STORE_ITEM_PLANT_GATLINGPEA:       aMessageIndex = 2000;                           break;
                case STORE_ITEM_PLANT_TWINSUNFLOWER:    aMessageIndex = 2001;                           break;
                case STORE_ITEM_PLANT_GLOOMSHROOM:      aMessageIndex = 2002;                           break;
                case STORE_ITEM_PLANT_CATTAIL:          aMessageIndex = 2003;                           break;
                case STORE_ITEM_PLANT_WINTERMELON:      aMessageIndex = 2004;                           break;
                case STORE_ITEM_PLANT_GOLD_MAGNET:      aMessageIndex = 2005;                           break;
                case STORE_ITEM_PLANT_SPIKEROCK:        aMessageIndex = 2006;                           break;
                case STORE_ITEM_PLANT_COBCANNON:        aMessageIndex = 2007;                           break;
                case STORE_ITEM_PLANT_IMITATER:         aMessageIndex = 2008;                           break;
                case STORE_ITEM_BONUS_LAWN_MOWER:       aMessageIndex = 2009;                           break;
                case STORE_ITEM_POTTED_MARIGOLD_1:
                case STORE_ITEM_POTTED_MARIGOLD_2:
                case STORE_ITEM_POTTED_MARIGOLD_3:      aMessageIndex = 2010;                           break;
                case STORE_ITEM_GOLD_WATERINGCAN:       aMessageIndex = 2019;                           break;
                case STORE_ITEM_FERTILIZER:             aMessageIndex = 2020;                           break;
                case STORE_ITEM_BUG_SPRAY:              aMessageIndex = 2022;                           break;
                case STORE_ITEM_PHONOGRAPH:             aMessageIndex = 2021;                           break;
                case STORE_ITEM_GARDENING_GLOVE:        aMessageIndex = 2023;                           break;
                case STORE_ITEM_MUSHROOM_GARDEN:        aMessageIndex = 2032;                           break;
                case STORE_ITEM_WHEEL_BARROW:           aMessageIndex = 2024;                           break;
                case STORE_ITEM_STINKY_THE_SNAIL:       aMessageIndex = 2025;                           break;
                case STORE_ITEM_PACKET_UPGRADE:
                    aMessageIndex = mApp->mPlayerInfo->mPurchases[STORE_ITEM_PACKET_UPGRADE] + 2011;    break;
                case STORE_ITEM_POOL_CLEANER:           aMessageIndex = 2026;                           break;
                case STORE_ITEM_ROOF_CLEANER:           aMessageIndex = 2027;                           break;
                case STORE_ITEM_RAKE:                   aMessageIndex = 2028;                           break;
                case STORE_ITEM_AQUARIUM_GARDEN:        aMessageIndex = 2029;                           break;
                case STORE_ITEM_CHOCOLATE:                                                              break;
                case STORE_ITEM_TREE_OF_WISDOM:         aMessageIndex = 2030;                           break;
                case STORE_ITEM_TREE_FOOD:              aMessageIndex = 2031;                           break;
                case STORE_ITEM_FIRSTAID:               aMessageIndex = 2033;                           break;
                case STORE_ITEM_PVZ:                    aMessageIndex = 2034;                           break;
                default:                                TOD_ASSERT();                                   break;
                }
                if (mApp->mCrazyDaveMessageIndex != aMessageIndex)
                    SetBubbleText(aMessageIndex, 100, false);
                else mBubbleCountDown = 100;
                if (IsFullVersionOnly(aItemType) || (!IsItemSoldOut(aItemType) && !IsItemUnavailable(aItemType) && !IsComingSoon(aItemType)))
                    aShowFinger = true;
                break;
            }
        }
    }

    mApp->SetCursor(mBackButton->mIsOver || mPrevButton->mIsOver || mNextButton->mIsOver || aShowFinger ? CURSOR_HAND : CURSOR_POINTER);
}

//0x48BE30
void StoreScreen::StorePreload()
{
    ReanimatorEnsureDefinitionLoaded(REANIM_CRAZY_DAVE, true);
    ReanimatorEnsureDefinitionLoaded(REANIM_ZENGARDEN_FERTILIZER, true);
    mApp->CrazyDaveEnter();

    Plant::PreloadPlantResources(SeedType::SEED_GARLIC);
    Plant::PreloadPlantResources(SeedType::SEED_TWINSUNFLOWER);

    if (mApp->HasFinishedAdventure())
    {
        Plant::PreloadPlantResources(SeedType::SEED_GLOOMSHROOM);
        Plant::PreloadPlantResources(SeedType::SEED_CATTAIL);
        Plant::PreloadPlantResources(SeedType::SEED_WINTERMELON);
        Plant::PreloadPlantResources(SeedType::SEED_GOLD_MAGNET);
        Plant::PreloadPlantResources(SeedType::SEED_SPIKEROCK);
        Plant::PreloadPlantResources(SeedType::SEED_COBCANNON);
        Plant::PreloadPlantResources(SeedType::SEED_IMITATER);
    }
}

bool StoreScreen::CanInteractWithButtons()
{
    return mStoreTime >= 120 && !mBubbleClickToContinue && mHatchTimer <= 0 && !mWaitForDialog;
}

//0x48BF60
void StoreScreen::Update()
{
    mApp->mMusic->MakeSureMusicIsPlaying(MUSIC_TUNE_TITLE_CRAZY_DAVE_MAIN_THEME);
    mApp->UpdateCrazyDave();

    // ���� DataArray<Coin> �е����� Coin
    Coin* aCoin = nullptr;
    while (mCoins.IterateNext(aCoin))
    {
        if (!aCoin->mDead)
        {
            aCoin->Update();
        }
    }

    if (mWaitForDialog)
        return;

    if (mApp->mCrazyDaveState == CRAZY_DAVE_OFF)
    {
        if (mDrawnOnce)
        {
            StorePreload();
        }
        return;
    }

    mStoreTime++;
    if (mApp->mCrazyDaveState != CRAZY_DAVE_OFF && mApp->mCrazyDaveState != CRAZY_DAVE_ENTERING)
    {
        if (mHatchTimer > 0)
        {
            mHatchTimer--;
            mBackButton->mX -= mShakeX;
            mBackButton->mY -= mShakeY;
            mPrevButton->mX -= mShakeX;
            mPrevButton->mY -= mShakeY;
            mNextButton->mX -= mShakeX;
            mNextButton->mY -= mShakeY;

            /*
            if (mHatchTimer <= 35)
            {
                if (mHatchTimer == 0)
                {
                    EnableButtons(true);
                }
                mShakeY = 0;
            }
            else
            {
                mShakeY = RandRangeInt(1, 3);
            }
            mShakeX = 0;
            */

            if (mHatchTimer == 0)
            {
                EnableButtons(true);
                mShakeX = 0;
                mShakeY = 0;
            }
            else
            {
                mShakeX = 0;
                if (mHatchTimer > 35)
                {
                    mShakeY = RandRangeInt(1, 3);
                }
                else
                {
                    mShakeY = 0;
                }
            }
            
            mBackButton->mX += mShakeX;
            mBackButton->mY += mShakeY;
            mPrevButton->mX += mShakeX;
            mPrevButton->mY += mShakeY;
            mNextButton->mX += mShakeX;
            mNextButton->mY += mShakeY;
        }
        else if (mStartDialog != -1)
        {
            SetBubbleText(mStartDialog, 0, true);
            mStartDialog = -1;
        }
        else if (!mBubbleClickToContinue)
        {
            if (mBubbleCountDown > 0)
            {
                mBubbleCountDown--;
                if (mBubbleCountDown == 0)
                {
                    if (mApp->mSoundSystem->IsFoleyPlaying(FOLEY_CRAZY_DAVE_SHORT) || 
                        mApp->mSoundSystem->IsFoleyPlaying(FOLEY_CRAZY_DAVE_LONG) ||
                        mApp->mSoundSystem->IsFoleyPlaying(FOLEY_CRAZY_DAVE_EXTRA_LONG))
                    {
                        mBubbleCountDown = 1;
                    }
                    else
                    {
                        mApp->CrazyDaveStopTalking();
                    }
                }
            }
            else
            {
                mAmbientSpeechCountDown--;
                if (mAmbientSpeechCountDown <= 0)
                {
                    TodWeightedArray aPickArray[4];
                    for (int i = 0; i < 4; i++)
                    {
                        int aMessage = 2015 + i;
                        aPickArray[i].mItem = aMessage;
                        if (mPreviousAmbientSpeechIndex == aMessage)
                        {
                            aPickArray[i].mWeight = 0;
                        }
                        else if (i == 3)
                        {
                            aPickArray[i].mWeight = mApp->HasFinishedAdventure() ? 20 : 0;
                        }
                        else
                        {
                            aPickArray[i].mWeight = 100;
                        }
                    }

                    int aDaveMessage = TodPickFromWeightedArray(aPickArray, 4);
                    mPreviousAmbientSpeechIndex = aDaveMessage;
                    SetBubbleText(aDaveMessage, 800, false);
                    mAmbientSpeechCountDown = RandRangeInt(500, 1000);
                }
            }
        }
    }

    UpdateMouse();
    // ��������̵�ʱΪ����棬����ǰΪ�����棬�ҿ����밴ť���н�����������ж�����ѹ���������
    if (CanInteractWithButtons() && mTrialLockedWhenStoreOpened && !mApp->IsTrialStageLocked())
    {
        mPurchasedFullVersion = true;
        mResult = Dialog::ID_OK;
    }
    else
    {
        Widget::Update();
        MarkDirty();
    }
}

//0x48C350
void StoreScreen::AddedToManager(WidgetManager* theWidgetManager)
{
    WidgetContainer::AddedToManager(theWidgetManager);
    AddWidget(mBackButton);
    AddWidget(mPrevButton);
    AddWidget(mNextButton);
    AddWidget(mOverlayWidget);
}

//0x48C3B0
void StoreScreen::RemovedFromManager(WidgetManager* theWidgetManager)
{
    WidgetContainer::RemovedFromManager(theWidgetManager);
    RemoveWidget(mBackButton);
    RemoveWidget(mPrevButton);
    RemoveWidget(mNextButton);
    RemoveWidget(mOverlayWidget);
    mApp->CrazyDaveDie();
}

//0x48C410
void StoreScreen::ButtonPress(int theId)
{
    if (theId != StoreScreen::StoreScreen_Prev && theId != StoreScreen::StoreScreen_Next)
        mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
}

//0x48C440
bool StoreScreen::IsPageShown(StorePages thePage)
{
    // ����ģʽ�£�����ʾĬ��ҳ
    if (mApp->IsTrialStageLocked()) return thePage == STORE_PAGE_SLOT_UPGRADES;
    // һ��Ŀ��ɺ�����ҳȫ����
    if (mApp->HasFinishedAdventure()) return true;
    // �������ͨ��ð��ģʽ 5-2 �ؿ�ʱ����ʾ�Ͽ�ҳ
    if (thePage == STORE_PAGE_PLANT_UPGRADES) return mApp->mPlayerInfo->mLevel >= 42;
    // �������ͨ��ð��ģʽ 5-5 �ؿ�ʱ����ʾ��԰����ҳ
    if (thePage == STORE_PAGE_ZEN1) return mApp->mPlayerInfo->mLevel >= 45;
    // ð��ģʽδ���ʱ������ʾ�ǻ�������ҳ
    return thePage != STORE_PAGE_ZEN2;
}

//0x48C4D0
void StoreScreen::ButtonDepress(int theId)
{
    if (theId == StoreScreen::StoreScreen_Back)
        mResult = 1000;
    else if (theId == StoreScreen::StoreScreen_Prev || theId == StoreScreen::StoreScreen_Next)
    {
        mHatchTimer = 50;
        mApp->PlaySample(Sexy::SOUND_HATCHBACK_CLOSE);
        mBubbleCountDown = 0;
        mApp->CrazyDaveStopTalking();
        EnableButtons(false);
        do
        {
            if (theId == StoreScreen::StoreScreen_Prev)
            {
                mPage = (StorePages)(mPage - 1);
                if (mPage < STORE_PAGE_SLOT_UPGRADES)
                {
                    mPage = STORE_PAGE_ZEN2;
                }
            }
            else
            {
                mPage = (StorePages)(mPage + 1);
                if (mPage >= NUM_STORE_PAGES)
                {
                    mPage = STORE_PAGE_SLOT_UPGRADES;
                }
            }
        } while (!IsPageShown(mPage));
    }
}

//0x48C5F0
void StoreScreen::KeyChar(char theChar)
{
    if (mBubbleClickToContinue && (theChar == ' ' || theChar == '\r')) AdvanceCrazyDaveDialog();
}

//0x48C620
int StoreScreen::GetItemCost(StoreItem theStoreItem)
{
    if (theStoreItem == STORE_ITEM_BONUS_LAWN_MOWER)    return gLawnApp->mPlayerInfo->mPurchases[STORE_ITEM_BONUS_LAWN_MOWER] ? 500 : 200;
    switch (theStoreItem)
    {
    case STORE_ITEM_PLANT_GATLINGPEA:                   return 500;
    case STORE_ITEM_PLANT_TWINSUNFLOWER:                return 500;
    case STORE_ITEM_PLANT_GLOOMSHROOM:                  return 750;
    case STORE_ITEM_PLANT_CATTAIL:                      return 1000;
    case STORE_ITEM_PLANT_WINTERMELON:                  return 1000;
    case STORE_ITEM_PLANT_GOLD_MAGNET:                  return 300;
    case STORE_ITEM_PLANT_SPIKEROCK:                    return 750;
    case STORE_ITEM_PLANT_COBCANNON:                    return 2000;
    case STORE_ITEM_PLANT_IMITATER:                     return 3000;
    case STORE_ITEM_POTTED_MARIGOLD_1:                  return 250;
    case STORE_ITEM_POTTED_MARIGOLD_2:                  return 250;
    case STORE_ITEM_POTTED_MARIGOLD_3:                  return 250;
    case STORE_ITEM_GOLD_WATERINGCAN:                   return 1000;
    case STORE_ITEM_FERTILIZER:                         return 75;
    case STORE_ITEM_BUG_SPRAY:                          return 100;
    case STORE_ITEM_PHONOGRAPH:                         return 1500;
    case STORE_ITEM_GARDENING_GLOVE:                    return 100;
    case STORE_ITEM_MUSHROOM_GARDEN:                    return 3000;
    case STORE_ITEM_WHEEL_BARROW:                       return 20;
    case STORE_ITEM_STINKY_THE_SNAIL:                   return 300;
    case STORE_ITEM_PACKET_UPGRADE:
    {
        int aPurchase = gLawnApp->mPlayerInfo->mPurchases[STORE_ITEM_PACKET_UPGRADE];
        return aPurchase == 0 ? 75 : aPurchase == 1 ? 500 : aPurchase == 2 ? 2000 : 8000;
    }
    case STORE_ITEM_POOL_CLEANER:                       return 100;
    case STORE_ITEM_ROOF_CLEANER:                       return 300;
    case STORE_ITEM_RAKE:                               return 20;
    case STORE_ITEM_AQUARIUM_GARDEN:                    return 3000;
    case STORE_ITEM_TREE_OF_WISDOM:                     return 1000;
    case STORE_ITEM_TREE_FOOD:                          return 250;
    case STORE_ITEM_FIRSTAID:                           return 200;
    default: TOD_ASSERT();                              return 0;
    }
}

bool StoreScreen::CanAffordItem(StoreItem theStoreItem)
{
    return mApp->mPlayerInfo->mCoins >= GetItemCost(theStoreItem);
}

//0x48C740
void StoreScreen::PurchaseItem(StoreItem theStoreItem)
{
    mApp->SetCursor(CURSOR_POINTER);
    mBubbleCountDown = 0;
    mApp->CrazyDaveStopTalking();
    if (!CanAffordItem(theStoreItem))
    {
        Dialog* aDialog = mApp->DoDialog(DIALOG_NOT_ENOUGH_MONEY, true, _S("[NOT_ENOUGH_MONEY]"), _S("[CANNOT_AFFORD_ITEM]"), _S("[DIALOG_BUTTON_OK]"), BUTTONS_FOOTER);
        mWaitForDialog = true;
        aDialog->WaitForResult(true);
        mWaitForDialog = false;
    }
    else
    {
        LawnDialog* aComfirmDialog = (LawnDialog*)mApp->DoDialog(
            DIALOG_STORE_PURCHASE, 
            true, 
            _S("���������Ʒ��"), 
            _S("�����Ҫ���������Ʒ��"), 
            _S(""), 
            BUTTONS_YES_NO
        );
        aComfirmDialog->mLawnYesButton->SetLabel(_S("[DIALOG_BUTTON_YES]"));
        aComfirmDialog->mLawnNoButton->SetLabel(_S("[DIALOG_BUTTON_NO]"));

        mWaitForDialog = true;
        int aComfirmResult = aComfirmDialog->WaitForResult(true);
        mWaitForDialog = false;

        if (aComfirmResult == ID_OK)
        {
            mApp->mPlayerInfo->AddCoins(-GetItemCost(theStoreItem));
            if (theStoreItem == STORE_ITEM_PACKET_UPGRADE)
            {
                ++mApp->mPlayerInfo->mPurchases[theStoreItem];
                SexyString aDialogLines = StrFormat(_S("�����ڿ�����ÿ���ؿ���ѡ��%d�������ˣ�"), 6 + mApp->mPlayerInfo->mPurchases[theStoreItem]);
                Dialog* aDialog = mApp->DoDialog(DIALOG_UPGRADED, true, _S("[MORE_SLOTS]"), aDialogLines, _S("[DIALOG_BUTTON_OK]"), BUTTONS_FOOTER);

                mWaitForDialog = true;
                aDialog->WaitForResult(true);
                mWaitForDialog = false;

                if (mApp->mBoard)
                {
                    mApp->mBoard->mSeedBank->UpdateWidth();
                }
            }
            else if (theStoreItem == STORE_ITEM_BONUS_LAWN_MOWER)
            {
                mApp->mPlayerInfo->mPurchases[theStoreItem]++;
            }
            else if (theStoreItem == STORE_ITEM_RAKE)
            {
                mApp->mPlayerInfo->mPurchases[theStoreItem] = 3;
            }
            else if (theStoreItem == STORE_ITEM_STINKY_THE_SNAIL)
            {
                mApp->mPlayerInfo->mPurchases[theStoreItem] = _time32(nullptr);
            }
            else if (theStoreItem == STORE_ITEM_FERTILIZER || theStoreItem == STORE_ITEM_BUG_SPRAY)
            {
                if (mApp->mPlayerInfo->mPurchases[theStoreItem] < PURCHASE_COUNT_OFFSET)
                {
                    mApp->mPlayerInfo->mPurchases[theStoreItem] = PURCHASE_COUNT_OFFSET;
                }
                mApp->mPlayerInfo->mPurchases[theStoreItem] += 5;
            }
            else if (theStoreItem == STORE_ITEM_TREE_FOOD)
            {
                if (mApp->mPlayerInfo->mPurchases[theStoreItem] < PURCHASE_COUNT_OFFSET)
                {
                    mApp->mPlayerInfo->mPurchases[theStoreItem] = PURCHASE_COUNT_OFFSET;
                }
                mApp->mPlayerInfo->mPurchases[theStoreItem]++;
            }
            else if (theStoreItem == STORE_ITEM_TREE_OF_WISDOM)
            {
                mApp->mPlayerInfo->mPurchases[theStoreItem] = 1;
                mApp->mPlayerInfo->mChallengeRecords[GAMEMODE_TREE_OF_WISDOM] = 1;

                LawnDialog* aDialog = (LawnDialog*)mApp->DoDialog(
                    DIALOG_STORE_PURCHASE, 
                    true, 
                    _S("[VISIT_TREE_HEADER]"), 
                    _S("[VISIT_TREE_BODY]"), 
                    _S(""), 
                    BUTTONS_YES_NO
                );
                aDialog->mLawnYesButton->SetLabel(_S("[DIALOG_BUTTON_YES]"));
                aDialog->mLawnNoButton->SetLabel(_S("[DIALOG_BUTTON_NO]"));

                mWaitForDialog = true;
                int aResult = aDialog->WaitForResult(true);
                mWaitForDialog = false;

                if (aResult == ID_OK)
                {
                    mGoToTreeNow = true;
                    mResult = aResult;
                }
            }
            else if (IsPottedPlant(theStoreItem))
            {
                mApp->mZenGarden->AddPottedPlant(&mPottedPlantSpecs);
                mPottedPlantSpecs.InitializePottedPlant(SEED_MARIGOLD);
                mPottedPlantSpecs.mDrawVariation = (DrawVariation)RandRangeInt(VARIATION_MARIGOLD_WHITE, VARIATION_MARIGOLD_LIGHT_GREEN);
                mApp->mPlayerInfo->mPurchases[theStoreItem] = GetCurrentDaysSince2000();
            }
            else
            {
                TOD_ASSERT(theStoreItem >= STORE_ITEM_PLANT_GATLINGPEA && theStoreItem < (StoreItem)MAX_PURCHASES);
                mApp->mPlayerInfo->mPurchases[theStoreItem] = 1;
            }

            if (theStoreItem == STORE_ITEM_FIRSTAID)
            {
                SetBubbleText(3400, 800, false);
            }

            if (mApp->mSeedChooserScreen)
            {
                mApp->mSeedChooserScreen->UpdateAfterPurchase();
            }

            mApp->WriteCurrentUserConfig();
        }
    }
}

//0x48CF50
void StoreScreen::AdvanceCrazyDaveDialog()
{
    if (!mBubbleClickToContinue)
        return;

    // ���٣��ҵ��ھӣ�����һЩ�¶�������������
    if (mApp->mCrazyDaveMessageIndex == 3100)
    {
        mHatchTimer = 150;
        mHatchOpen = true;
        mApp->PlaySample(Sexy::SOUND_HATCHBACK_OPEN);
    }
    if (!mApp->AdvanceCrazyDaveText())
    {
        mApp->CrazyDaveStopTalking();
        mBubbleClickToContinue = false;
        mBubbleCountDown = 500;
        if (mHatchTimer == 0)
        {
            EnableButtons(true);
        }
    }
    else
    {
        SetBubbleText(mApp->mCrazyDaveMessageIndex, 0, true);
    }

    int aMessage = mApp->mCrazyDaveMessageIndex;
    if (aMessage == 303 || aMessage == 606 || aMessage == 2601)
    {
        mHatchTimer = 150;
        mHatchOpen = true;
        mApp->PlaySample(Sexy::SOUND_HATCHBACK_OPEN);
    }
    else if (aMessage == 603)
    {
        mApp->mPlayerInfo->mNeedsMagicTacoReward = false;
        mApp->WriteCurrentUserConfig();
        mApp->PlaySample(Sexy::SOUND_DIAMOND);
        Coin* aCoin = mCoins.DataArrayAlloc();
        aCoin->CoinInitialize(80, 520, COIN_DIAMOND, COIN_MOTION_FROM_PRESENT);
        aCoin->mVelX = 0;
        aCoin->mVelY = -5;
    }
    else if (aMessage == 902 || aMessage == 1002)
    {
        mApp->mPlayerInfo->AddCoins(100);
    }
}

//0x48D130
void StoreScreen::MouseDown(int x, int y, int theClickCount)
{
    if (mBubbleClickToContinue)
    {
        AdvanceCrazyDaveDialog();
        return;
    }
    if (!CanInteractWithButtons()) return;
    for (int aItemPos = 0; aItemPos < MAX_PAGE_SPOTS; aItemPos++)
    {
        StoreItem aItemType = GetStoreItemType(aItemPos);
        if (aItemType == STORE_ITEM_INVALID) continue;
        int aItemX, aItemY;
        GetStorePosition(aItemPos, aItemX, aItemY);
        if (Rect(aItemX, aItemY, 50, 87).Contains(x, y))
        {
            if (IsFullVersionOnly(aItemType))
            {
                mWaitForDialog = true;
                mApp->LawnMessageBox(DIALOG_MESSAGE, "[GET_FULL_VERSION_TITLE]", "[FULL_VERSION_TO_BUY]", "[DIALOG_BUTTON_OK]", "", BUTTONS_FOOTER);
                mWaitForDialog = false;
            }
            else if (aItemType == STORE_ITEM_PVZ)
            {
                mWaitForDialog = true;
                int aResult = mApp->LawnMessageBox(
                    DIALOG_MESSAGE, "[BUY_PVZ_TITLE]", "[BUY_PVZ_BODY]", "[GET_FULL_VERSION_YES_BUTTON]", "[GET_FULL_VERSION_NO_BUTTON]", BUTTONS_YES_NO);
                mWaitForDialog = false;
                if (aResult == ID_OK)
                {
                    if (mApp->mDRM) mApp->mDRM->BuyGame();
                }
            }
            else if(!IsItemSoldOut(aItemType) && !IsItemUnavailable(aItemType) && !IsComingSoon(aItemType))
                PurchaseItem(aItemType);
            break;
        }
    }
}

//0x48D2E0
void StoreScreen::EnableButtons(bool theEnable)
{
    if (mEasyBuyingCheat || IsPageShown(STORE_PAGE_PLANT_UPGRADES) || !theEnable)
    {
        mNextButton->mMouseVisible = theEnable;
        mNextButton->SetDisabled(!theEnable);
        mPrevButton->mMouseVisible = theEnable;
        mPrevButton->SetDisabled(!theEnable);
    }
    mBackButton->mMouseVisible = theEnable;
    mBackButton->SetDisabled(!theEnable);
}

//0x48D3A0
void StoreScreen::SetupForIntro(int theDialogIndex)
{
    mStartDialog = theDialogIndex;
    mHatchOpen = false;
    mBackButton->mLabel = TodStringTranslate(_S("[STORE_NEXT_LEVEL_BUTTON]"));
    EnableButtons(false);
}
