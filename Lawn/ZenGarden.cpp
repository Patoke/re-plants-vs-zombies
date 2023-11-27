#include <time.h>
#include "Plant.h"
#include "Board.h"
#include "Challenge.h"
#include "ZenGarden.h"
#include "../LawnApp.h"
#include "CursorObject.h"
#include "System/Music.h"
#include "Widget/GameButton.h"
#include "System/PlayerInfo.h"
#include "Widget/LawnDialog.h"
#include "Widget/StoreScreen.h"
#include "System/ReanimationLawn.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/Attachment.h"
#include "../Sexy.TodLib/TodParticle.h"
#include "../Sexy.TodLib/EffectSystem.h"
#include "../SexyAppFramework/Graphics.h"
#include "../Sexy.TodLib/TodStringFile.h"

static SpecialGridPlacement gGreenhouseGridPlacement[] =  //0x69DE50
{
    {  73,  73, 0, 0 },
    { 155,  71, 1, 0 },
    { 239,  68, 2, 0 },
    { 321,  73, 3, 0 },
    { 406,  71, 4, 0 },
    { 484,  67, 5, 0 },
    { 566,  70, 6, 0 },
    { 648,  72, 7, 0 },
    {  67, 168, 0, 1 },
    { 150, 165, 1, 1 },
    { 232, 170, 2, 1 },
    { 314, 175, 3, 1 },
    { 416, 173, 4, 1 },
    { 497, 170, 5, 1 },
    { 578, 164, 6, 1 },
    { 660, 168, 7, 1 },
    {  41, 268, 0, 2 },
    { 130, 266, 1, 2 },
    { 219, 260, 2, 2 },
    { 310, 266, 3, 2 },
    { 416, 267, 4, 2 },
    { 504, 261, 5, 2 },
    { 594, 265, 6, 2 },
    { 684, 269, 7, 2 },
    {  37, 371, 0, 3 },
    { 124, 369, 1, 3 },
    { 211, 368, 2, 3 },
    { 302, 369, 3, 3 },
    { 425, 375, 4, 3 },
    { 512, 368, 5, 3 },
    { 602, 365, 6, 3 },
    { 691, 368, 7, 3 }
};

static SpecialGridPlacement gMushroomGridPlacement[] = {  //0x69E050
    { 110, 441, 0, 0 },
    { 237, 360, 1, 0 },
    { 298, 458, 2, 0 },
    { 355, 296, 3, 0 },
    { 387, 203, 4, 0 },
    { 460, 385, 5, 0 },
    { 486, 478, 6, 0 },
    { 552, 283, 7, 0 }
};

static SpecialGridPlacement gAquariumGridPlacement[] = {  //0x69E0D0
    { 113, 185, 0, 0 },
    { 306, 120, 1, 0 },
    { 356, 270, 2, 0 },
    { 622, 120, 3, 0 },
    { 669, 270, 4, 0 },
    { 122, 355, 5, 0 },
    { 365, 458, 6, 0 },
    { 504, 417, 7, 0 }
};

ZenGarden::ZenGarden()
{
    mApp = (LawnApp*)gSexyAppBase;
    mBoard = nullptr;
    mGardenType = GardenType::GARDEN_MAIN;
}

//0x51D0E0
void ZenGarden::DrawPottedPlantIcon(Graphics* g, float x, float y, PottedPlant* thePottedPlant)
{
    DrawPottedPlant(g, x, y, thePottedPlant, 0.7f, true);
}

//0x51D110
void ZenGarden::DrawPottedPlant(Graphics* g, float x, float y, PottedPlant* thePottedPlant, float theScale, bool theDrawPot)
{
    Graphics aPottedPlantG(*g);
    aPottedPlantG.mScaleX = theScale;
    aPottedPlantG.mScaleY = theScale;

    DrawVariation aPlantVariation = DrawVariation::VARIATION_NORMAL;
    SeedType aSeedType = thePottedPlant->mSeedType;
    if (thePottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SPROUT)
    {
        aSeedType = SeedType::SEED_SPROUT;
        if (thePottedPlant->mSeedType != SeedType::SEED_MARIGOLD)
        {
            aPlantVariation = DrawVariation::VARIATION_SPROUT_NO_FLOWER;
        }
    }
    else if ((aSeedType == SeedType::SEED_TANGLEKELP || aSeedType == SeedType::SEED_SEASHROOM) && thePottedPlant->mWhichZenGarden == GardenType::GARDEN_AQUARIUM)
    {
        aPlantVariation = DrawVariation::VARIATION_AQUARIUM;
    }
    else
    {
        aPlantVariation = thePottedPlant->mDrawVariation;
    }

    float aOffsetX = 0.0f;
    float aOffsetY = PlantDrawHeightOffset(mBoard, nullptr, aSeedType, -1, -1);
    if (theDrawPot)
    {
        float aPotOffsetY = theScale * 0.0f - 0.0f + 0.0f;
        aPotOffsetY += PlantDrawHeightOffset(mBoard, nullptr, SeedType::SEED_FLOWERPOT, -1, -1);

        DrawVariation aPotVariation2 = DrawVariation::VARIATION_ZEN_GARDEN;
        if (Plant::IsAquatic(aSeedType))
        {
            aPotVariation2 = DrawVariation::VARIATION_ZEN_GARDEN_WATER;
        }

        Plant::DrawSeedType(&aPottedPlantG, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE, aPotVariation2, x, y + aPotOffsetY * theScale);
    }

    if (thePottedPlant->mFacing == PottedPlant::FacingDirection::FACING_LEFT)
    {
        aPottedPlantG.mScaleX = -theScale;
        aOffsetX += 80.0f * theScale;
    }

    if (thePottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SMALL)
    {
        aOffsetX += 20.0f * aPottedPlantG.mScaleX;
        aOffsetY += 40.0f * aPottedPlantG.mScaleY;
        aPottedPlantG.mScaleX *= 0.5f;
        aPottedPlantG.mScaleY *= 0.5f;
    }
    else if (thePottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_MEDIUM)
    {
        aOffsetX += 10.0f * aPottedPlantG.mScaleX;
        aOffsetY += 20.0f * aPottedPlantG.mScaleY;
        aPottedPlantG.mScaleX *= 0.75f;
        aPottedPlantG.mScaleY *= 0.75f;
    }

    if (theDrawPot)
    {
        aOffsetY += PlantFlowerPotHeightOffset(aSeedType, theScale);
    }
    aOffsetY += PlantPottedDrawHeightOffset(aSeedType, aPottedPlantG.mScaleY);

    Plant::DrawSeedType(&aPottedPlantG, aSeedType, SeedType::SEED_NONE, aPlantVariation, x + aOffsetX, y + aOffsetY);
}

void ZenGarden::PlantSetLaunchCounter(Plant* thePlant)
{
    int aTime = PlantGetMinutesSinceHappy(thePlant);
    int aCounterMax = TodAnimateCurve(5, 30, aTime, 3000, 15000, TodCurves::CURVE_LINEAR);
    thePlant->mLaunchCounter = RandRangeInt(1800, aCounterMax);
}

//0x51D3A0
Plant* ZenGarden::PlacePottedPlant(intptr_t thePottedPlantIndex)
{
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePottedPlantIndex);
    SeedType aSeedType = aPottedPlant->mSeedType;
    if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SPROUT)
    {
        aSeedType = SeedType::SEED_SPROUT;
    }

    bool needPot = true;
    if (mGardenType == GardenType::GARDEN_MUSHROOM && !Plant::IsAquatic(aSeedType))
    {
        needPot = false;
    }
    else if (mGardenType == GardenType::GARDEN_AQUARIUM)
    {
        needPot = false;
    }

    if (needPot)
    {
        Plant* aPot = mBoard->NewPlant(aPottedPlant->mX, aPottedPlant->mY, SeedType::SEED_FLOWERPOT, SeedType::SEED_NONE);
        aPot->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PLANT, 0, aPot->mY);
        aPot->mStateCountdown = 0;

        Reanimation* aPotReanim = mApp->ReanimationGet(aPot->mBodyReanimID);
        if (Plant::IsAquatic(aSeedType))
        {
            aPotReanim->SetFramesForLayer("anim_waterplants");
        }
        else
        {
            aPotReanim->SetFramesForLayer("anim_zengarden");
        }
    }

    Plant* aPlant = mBoard->NewPlant(aPottedPlant->mX, aPottedPlant->mY, aSeedType, SeedType::SEED_NONE);
    aPlant->mPottedPlantIndex = thePottedPlantIndex;
    aPlant->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PLANT, 0, aPlant->mY + 1);
    aPlant->mStateCountdown = 0;

    Reanimation* aPlantReanim = mApp->ReanimationTryToGet(aPlant->mBodyReanimID);
    if (aPlantReanim)
    {
        if (aSeedType == SeedType::SEED_SPROUT)
        {
            if (aPottedPlant->mSeedType != SeedType::SEED_MARIGOLD)
            {
                aPlantReanim->SetFramesForLayer("anim_idle_noflower");
            }
        }
        else if ((aSeedType == SeedType::SEED_TANGLEKELP || aSeedType == SeedType::SEED_SEASHROOM) && mGardenType == GardenType::GARDEN_AQUARIUM)
        {
            aPlantReanim->SetFramesForLayer("anim_idle_aquarium");
        }
        else if (aPottedPlant->mDrawVariation != DrawVariation::VARIATION_NORMAL)
        {
            mApp->mReanimatorCache->UpdateReanimationForVariation(aPlantReanim, aPottedPlant->mDrawVariation);
        }

        aPlant->UpdateReanim();
        aPlantReanim->Update();
    }

    PlantSetLaunchCounter(aPlant);
    UpdatePlantEffectState(aPlant);
    return aPlant;
}

//0x51D5C0
void ZenGarden::RemovePottedPlant(Plant* thePlant)
{
    thePlant->Die();
    Plant* aPot = mBoard->GetTopPlantAt(thePlant->mPlantCol, thePlant->mRow, PlantPriority::TOPPLANT_ONLY_UNDER_PLANT);
    if (aPot)
    {
        aPot->Die();
    }
}

PottedPlant* ZenGarden::PottedPlantFromIndex(intptr_t thePottedPlantIndex)
{
    TOD_ASSERT(thePottedPlantIndex >= 0 && thePottedPlantIndex < mApp->mPlayerInfo->mNumPottedPlants);
    return &mApp->mPlayerInfo->mPottedPlant[thePottedPlantIndex];
}

//0x51D630
void ZenGarden::ZenGardenInitLevel()
{
    mBoard = mApp->mBoard;
    for (int i = 0; i < mApp->mPlayerInfo->mNumPottedPlants; i++)
    {
        PottedPlant* aPottedPlant = PottedPlantFromIndex(i);
        if (aPottedPlant->mWhichZenGarden == mGardenType)
        {
            PlacePottedPlant(i);
        }
    }

    mBoard->mChallenge->mChallengeStateCounter = 3000;
    AddStinky();
    mApp->mMusic->StartGameMusic();
}

void ZenGarden::ZenGardenStart()
{
}

//0x51D6B0
bool ZenGarden::PlantCanHaveChocolate(Plant* thePlant)
{
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    return aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_FULL && WasPlantNeedFulfilledToday(aPottedPlant) && !PlantHighOnChocolate(aPottedPlant);
}

//0x51D710
bool ZenGarden::CanDropChocolate()
{
    return HasPurchasedStinky() && mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_CHOCOLATE] - PURCHASE_COUNT_OFFSET < 10;
}

//0x51D740
bool ZenGarden::IsZenGardenFull(bool theIncludeDroppedPresents)
{
    int aNumDroppedPresents = 0;
    if (mBoard && theIncludeDroppedPresents)
    {
        aNumDroppedPresents += mBoard->CountCoinByType(CoinType::COIN_AWARD_PRESENT);
        aNumDroppedPresents += mBoard->CountCoinByType(CoinType::COIN_PRESENT_PLANT);
    }

    int aNumPottedPlantsInGarden = 0;
    for (int i = 0; i < mApp->mPlayerInfo->mNumPottedPlants; i++)
    {
        PottedPlant* aPottedPlant = PottedPlantFromIndex(i);
        if (aPottedPlant->mWhichZenGarden == GardenType::GARDEN_MAIN)
        {
            aNumPottedPlantsInGarden++;
        }
    }

    return aNumDroppedPresents + aNumPottedPlantsInGarden >= ZEN_MAX_GRIDSIZE_X * ZEN_MAX_GRIDSIZE_Y;
}

bool ZenGarden::CanDropPottedPlantLoot()
{
    return mApp->HasFinishedAdventure() && !IsZenGardenFull(true);
}

//0x51D7B0
void ZenGarden::FindOpenZenGardenSpot(int& theSpotX, int& theSpotY)
{
    TodWeightedGridArray aPicks[ZEN_MAX_GRIDSIZE_X * ZEN_MAX_GRIDSIZE_Y];
    int aPickCount = 0;

    for (int x = 0; x < ZEN_MAX_GRIDSIZE_X; x++)
    {
        for (int y = 0; y < ZEN_MAX_GRIDSIZE_Y; y++)
        {
            if (mApp->mCrazyDaveMessageIndex != -1 && (x < 2 || y < 1))
            {
                continue;  // 忽略被戴夫遮挡住的部分
            }

            for (int i = 0; i < mApp->mPlayerInfo->mNumPottedPlants; i++)
            {
                PottedPlant* aPottedPlant = PottedPlantFromIndex(i);
                if (aPottedPlant->mWhichZenGarden == GardenType::GARDEN_MAIN && aPottedPlant->mX == x && aPottedPlant->mY == y)
                {
                    continue;  // 格子内已有盆栽植物则不可选择
                }
            }

            aPicks[aPickCount].mX = x;
            aPicks[aPickCount].mY = y;
            aPicks[aPickCount].mWeight = 1;
            aPickCount++;
        }
    }

    TodWeightedGridArray* aSpot = TodPickFromWeightedGridArray(aPicks, aPickCount);
    theSpotX = aSpot->mX;
    theSpotY = aSpot->mY;
}

//0x51D8C0
void ZenGarden::AddPottedPlant(PottedPlant* thePottedPlant)
{
    TOD_ASSERT(mApp->mPlayerInfo->mNumPottedPlants < MAX_POTTED_PLANTS);

    int aPottedPlantIndex = mApp->mPlayerInfo->mNumPottedPlants;
    PottedPlant* aPottedPlant = &mApp->mPlayerInfo->mPottedPlant[aPottedPlantIndex];
    *aPottedPlant = *thePottedPlant;
    aPottedPlant->mWhichZenGarden = GardenType::GARDEN_MAIN;
    aPottedPlant->mLastWateredTime = 0;
    FindOpenZenGardenSpot(aPottedPlant->mX, aPottedPlant->mY);
    mApp->mPlayerInfo->mNumPottedPlants++;

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN && mBoard && aPottedPlant->mWhichZenGarden == mGardenType)
    {
        Plant* aPlant = PlacePottedPlant(aPottedPlantIndex);
        if (mApp->GetDialog(Dialogs::DIALOG_STORE) == nullptr)
        {
            mBoard->DoPlantingEffects(aPottedPlant->mX, aPottedPlant->mY, aPlant);
        }
    }
}

//0x51D970
int ZenGarden::GetPlantSellPrice(Plant* thePlant)
{
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    if (aPottedPlant->mSeedType == SeedType::SEED_MARIGOLD)
    {
        if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SPROUT)
        {
            return 150;
        }
        if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SMALL)
        {
            return 200;
        }
        if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_MEDIUM)
        {
            return 250;
        }
        if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_FULL)
        {
            return 300;
        }
        TOD_ASSERT();
    }
    if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SPROUT)
    {
        return 150;
    }
    if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SMALL)
    {
        return 300;
    }
    if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_MEDIUM)
    {
        return 500;
    }
    if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_FULL)
    {
        if (Plant::IsNocturnal(aPottedPlant->mSeedType) || Plant::IsAquatic(aPottedPlant->mSeedType))
        {
            return 1000;
        }
        return 800;
    }
    TOD_ASSERT();

    __builtin_unreachable();
}

//0x51DA00
void ZenGarden::MouseDownWithMoneySign(Plant* thePlant)
{
    mBoard->ClearCursor();

    SexyString aHeader = TodStringTranslate(_S("[ZEN_SELL_HEADER]"));
    SexyString aLines = TodStringTranslate(_S("[ZEN_SELL_LINES]"));
    int aPrice = GetPlantSellPrice(thePlant);
    if (mApp->mCrazyDaveState == CrazyDaveState::CRAZY_DAVE_OFF)
    {
        mApp->CrazyDaveEnter();
    }

    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    SexyString aMessageText = mApp->GetCrazyDaveText(1700);
    aMessageText = TodReplaceNumberString(aMessageText, _S("{SELL_PRICE}"), aPrice);

    SexyString aPlantName;
    if (thePlant->mSeedType == SeedType::SEED_SPROUT && aPottedPlant->mSeedType == SeedType::SEED_MARIGOLD)
    {
        aPlantName = TodStringTranslate(_S("[MARIGOLD_SPROUT]"));
    }
    else
    {
        aPlantName = Plant::GetNameString(thePlant->mSeedType, thePlant->mImitaterType);
    }
    aMessageText = TodReplaceString(aMessageText, _S("{PLANT_TYPE}"), aPlantName);

    mApp->CrazyDaveTalkMessage(aMessageText);
    Reanimation* aCrazyDaveReanim = mApp->ReanimationGet(mApp->mCrazyDaveReanimID);
    aCrazyDaveReanim->PlayReanim("anim_blahblah", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 12.0f);

    Dialog* aDialog = mApp->DoDialog(Dialogs::DIALOG_ZEN_SELL, true, aHeader, aLines, _S(""), Dialog::BUTTONS_YES_NO);
    aDialog->mX += 120;
    aDialog->mY += 60;
    mBoard->ShowCoinBank();
    int aResult = aDialog->WaitForResult(true);
    mApp->CrazyDaveLeave();

    if (aResult == Dialog::ID_YES)
    {
        mApp->mPlayerInfo->AddCoins(aPrice);
        mBoard->mCoinsCollected += aPrice;

        int aNumPlantsAfterThis = mApp->mPlayerInfo->mNumPottedPlants - thePlant->mPottedPlantIndex - 1;
        if (aNumPlantsAfterThis > 0)
        {
            memcpy(aPottedPlant, aPottedPlant + 1, aNumPlantsAfterThis * sizeof(PottedPlant));

            Plant* aUpdatePlant = nullptr;
            while (mBoard->IteratePlants(aUpdatePlant))
            {
                if (aUpdatePlant->mPottedPlantIndex > thePlant->mPottedPlantIndex)
                {
                    aUpdatePlant->mPottedPlantIndex--;
                }
            }
        }

        mApp->mPlayerInfo->mNumPottedPlants--;
        mApp->PlayFoley(FoleyType::FOLEY_USE_SHOVEL);
        //mApp->PlaySample(SOUND_PLANT2);
        RemovePottedPlant(thePlant);
    }
}

//0x51DF40
void ZenGarden::PlantFertilized(Plant* thePlant)
{
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    aPottedPlant->mLastFertilizedTime = _time64(nullptr);
    aPottedPlant->mPlantAge = (PottedPlantAge)((int)aPottedPlant->mPlantAge + 1);
    aPottedPlant->mPlantNeed = PottedPlantNeed::PLANTNEED_NONE;
    aPottedPlant->mTimesFed = 0;

    if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SMALL)
    {
        RemovePottedPlant(thePlant);
        PlacePottedPlant(thePlant->mPottedPlantIndex);
        mApp->PlaySample(SOUND_LOADINGBAR_FLOWER);
    }
    else
    {
        thePlant->mStateCountdown = 100;
        mApp->PlayFoley(FoleyType::FOLEY_PLANTGROW);
    }

    mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
    if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SMALL)
    {
        mBoard->AddCoin(thePlant->mX + 40, thePlant->mY, CoinType::COIN_GOLD, CoinMotion::COIN_MOTION_COIN);
    }
    else if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_MEDIUM)
    {
        mBoard->AddCoin(thePlant->mX + 30, thePlant->mY, CoinType::COIN_GOLD, CoinMotion::COIN_MOTION_COIN);
        mBoard->AddCoin(thePlant->mX + 50, thePlant->mY, CoinType::COIN_GOLD, CoinMotion::COIN_MOTION_COIN);
    }
    else if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_FULL)
    {
        if (aPottedPlant->mSeedType == SeedType::SEED_MARIGOLD)
        {
            mBoard->AddCoin(thePlant->mX + 40, thePlant->mY, CoinType::COIN_DIAMOND, CoinMotion::COIN_MOTION_COIN);
        }
        else
        {
            mBoard->AddCoin(thePlant->mX + 10, thePlant->mY, CoinType::COIN_DIAMOND, CoinMotion::COIN_MOTION_COIN);
            mBoard->AddCoin(thePlant->mX + 70, thePlant->mY, CoinType::COIN_DIAMOND, CoinMotion::COIN_MOTION_COIN);
        }
    }
}

//0x51E110
void ZenGarden::PlantFulfillNeed(Plant* thePlant)
{
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    aPottedPlant->mLastNeedFulfilledTime = _time64(nullptr);
    aPottedPlant->mPlantNeed = PottedPlantNeed::PLANTNEED_NONE;
    aPottedPlant->mTimesFed = 0;

    mApp->PlayFoley(FoleyType::FOLEY_PRIZE);
    mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
    mBoard->AddCoin(thePlant->mX + 40, thePlant->mY, CoinType::COIN_GOLD, CoinMotion::COIN_MOTION_COIN);
    if (Plant::IsNocturnal(thePlant->mSeedType) || Plant::IsAquatic(thePlant->mSeedType))
    {
        mBoard->AddCoin(thePlant->mX + 10, thePlant->mY, CoinType::COIN_GOLD, CoinMotion::COIN_MOTION_COIN);
        mBoard->AddCoin(thePlant->mX + 70, thePlant->mY, CoinType::COIN_GOLD, CoinMotion::COIN_MOTION_COIN);
    }
}

//0x51E290
bool ZenGarden::PlantsNeedWater()
{
    for (int i = 0; i < mApp->mPlayerInfo->mNumPottedPlants; i++)
    {
        PottedPlant* aPottedPlant = PottedPlantFromIndex(i);
        if (mApp->mZenGarden->GetPlantsNeed(aPottedPlant) == PottedPlantNeed::PLANTNEED_WATER)
        {
            return true;
        }
    }
    return false;
}

//0x51E2F0
bool ZenGarden::PlantCanBeWatered(Plant* thePlant)
{
    if (thePlant->mPottedPlantIndex == -1)
    {
        return false;
    }

    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    return mApp->mZenGarden->GetPlantsNeed(aPottedPlant) == PottedPlantNeed::PLANTNEED_WATER;
}

//0x51E320
int ZenGarden::CountPlantsNeedingFertilizer()
{
    int aCount = 0;
    for (int i = 0; i < mApp->mPlayerInfo->mNumPottedPlants; i++)
    {
        PottedPlant* aPottedPlant = PottedPlantFromIndex(i);
        if (mApp->mZenGarden->GetPlantsNeed(aPottedPlant) == PottedPlantNeed::PLANTNEED_FERTILIZER)
        {
            aCount++;
        }
    }
    return aCount;
}

//0x51E390
bool ZenGarden::AllPlantsHaveBeenFertilized()
{
    for (int i = 0; i < mApp->mPlayerInfo->mNumPottedPlants; i++)
    {
        PottedPlant* aPottedPlant = PottedPlantFromIndex(i);
        if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SPROUT)
        {
            return false;
        }
    }
    return true;
}

//0x51E3D0
void ZenGarden::PlantWatered(Plant* thePlant)
{
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    aPottedPlant->mTimesFed++;
    int aTimeSpan = RandRangeInt(0, 8);
    if (mBoard->mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_WATER_PLANT || mBoard->mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_KEEP_WATERING)
    {
        aTimeSpan = 9;
    }
    aPottedPlant->mLastWateredTime = _time64(nullptr) - (__time64_t)aTimeSpan;

    mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);
    mBoard->AddCoin(thePlant->mX + 40, thePlant->mY, CoinType::COIN_SILVER, CoinMotion::COIN_MOTION_COIN);
    if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_FULL && aPottedPlant->mPlantNeed == PottedPlantNeed::PLANTNEED_NONE)
    {
        aPottedPlant->mPlantNeed = (PottedPlantNeed)RandRangeInt((int)PottedPlantNeed::PLANTNEED_BUGSPRAY, (int)PottedPlantNeed::PLANTNEED_PHONOGRAPH);
    }

    if (mBoard->mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_WATER_PLANT)
    {
        mBoard->mTutorialState = TutorialState::TUTORIAL_ZEN_GARDEN_KEEP_WATERING;
        mBoard->DisplayAdvice(_S("[ADVICE_ZEN_GARDEN_KEEP_WATERING]"), MessageStyle::MESSAGE_STYLE_ZEN_GARDEN_LONG, AdviceType::ADVICE_NONE);
    }
}

//0x51E560
void ZenGarden::UpdatePlantEffectState(Plant* thePlant)
{
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    PlantState aOriginalState = thePlant->mState;
    PottedPlantNeed aPlantNeed = GetPlantsNeed(aPottedPlant);
    if (aPlantNeed == PottedPlantNeed::PLANTNEED_WATER)
    {
        thePlant->mState = PlantState::STATE_NOTREADY;
    }
    else if (aPlantNeed == PottedPlantNeed::PLANTNEED_NONE)
    {
        if (WasPlantNeedFulfilledToday(aPottedPlant))
        {
            thePlant->mState = PlantState::STATE_ZEN_GARDEN_HAPPY;
        }
        else if (thePlant->mIsAsleep)
        {
            thePlant->mState = PlantState::STATE_NOTREADY;
        }
        else
        {
            thePlant->mState = PlantState::STATE_ZEN_GARDEN_WATERED;
        }
    }
    else
    {
        thePlant->mState = PlantState::STATE_ZEN_GARDEN_NEEDY;
    }
    if (aOriginalState == thePlant->mState)
    {
        return;
    }

    Plant* aFlowerPot = mBoard->GetTopPlantAt(thePlant->mPlantCol, thePlant->mRow, PlantPriority::TOPPLANT_ONLY_UNDER_PLANT);
    if (aFlowerPot && !Plant::IsAquatic(thePlant->mSeedType))
    {
        Reanimation* aPotReanim = mApp->ReanimationGet(aFlowerPot->mBodyReanimID);
        if (thePlant->mState == PlantState::STATE_ZEN_GARDEN_WATERED || 
            thePlant->mState == PlantState::STATE_ZEN_GARDEN_NEEDY || 
            thePlant->mState == PlantState::STATE_ZEN_GARDEN_HAPPY)
        {
            aPotReanim->SetImageOverride("Pot_top", IMAGE_REANIM_POT_TOP_DARK);
        }
        else
        {
            aPotReanim->SetImageOverride("Pot_top", nullptr);
        }
    }
    if (aOriginalState == PlantState::STATE_ZEN_GARDEN_HAPPY)
    {
        RemoveHappyEffect(thePlant);
    }

    if (thePlant->mState == PlantState::STATE_ZEN_GARDEN_HAPPY)
    {
        thePlant->SetSleeping(false);
        AddHappyEffect(thePlant);
    }
    else if (Plant::IsNocturnal(thePlant->mSeedType) && !mBoard->StageIsNight())
    {
        thePlant->SetSleeping(true);
    }
}

//0x51E730
void ZenGarden::AddHappyEffect(Plant* thePlant)
{
    Plant* aFlowerPot = mBoard->GetTopPlantAt(thePlant->mPlantCol, thePlant->mRow, PlantPriority::TOPPLANT_ONLY_UNDER_PLANT);
    if (aFlowerPot == nullptr)
    {
        thePlant->AddAttachedParticle(thePlant->mX + 40, thePlant->mY + 60, thePlant->mRenderOrder - 1, ParticleEffect::PARTICLE_POTTED_ZEN_GLOW);
    }
    else if (Plant::IsAquatic(thePlant->mSeedType))
    {
        aFlowerPot->AddAttachedParticle(aFlowerPot->mX + 40, aFlowerPot->mY + 61, aFlowerPot->mRenderOrder - 1, ParticleEffect::PARTICLE_POTTED_WATER_PLANT_GLOW);
    }
    else
    {
        aFlowerPot->AddAttachedParticle(aFlowerPot->mX + 40, aFlowerPot->mY + 63, aFlowerPot->mRenderOrder - 1, ParticleEffect::PARTICLE_POTTED_ZEN_GLOW);
    }
}

//0x45E7E0
void ZenGarden::RemoveHappyEffect(Plant* thePlant)
{
    Plant* aFlowerPot = mBoard->GetTopPlantAt(thePlant->mPlantCol, thePlant->mRow, PlantPriority::TOPPLANT_ONLY_UNDER_PLANT);
    TodParticleSystem* aParticleSystem;
    if (aFlowerPot)
    {
        aParticleSystem = mApp->ParticleTryToGet(aFlowerPot->mParticleID);
    }
    else
    {
        aParticleSystem = mApp->ParticleTryToGet(thePlant->mParticleID);
    }
    if (aParticleSystem)
    {
        aParticleSystem->ParticleSystemDie();
    }
}

//0x51E890
bool ZenGarden::WasPlantNeedFulfilledToday(PottedPlant* thePottedPlant)
{
    __time64_t aNow = _time64(nullptr);
    if (aNow - thePottedPlant->mLastNeedFulfilledTime < 3600)
    {
        return true;
    }

    tm aNowTM, aLastNeedFulfilledTM;
    _localtime64_s(&aNowTM, &aNow);
    _localtime64_s(&aLastNeedFulfilledTM, &thePottedPlant->mLastNeedFulfilledTime);
    return aNowTM.tm_year <= aLastNeedFulfilledTM.tm_year && aNowTM.tm_yday <= aLastNeedFulfilledTM.tm_yday;
}

//0x51E910
bool ZenGarden::PlantShouldRefreshNeed(PottedPlant* thePottedPlant)
{
    __time64_t aNow = _time64(nullptr);
    if (aNow - thePottedPlant->mLastWateredTime < 3600)
    {
        return false;
    }
    
    tm aNowTM, aLastWateredTM;
    _localtime64_s(&aNowTM, &aNow);
    _localtime64_s(&aLastWateredTM, &thePottedPlant->mLastWateredTime);
    return aNowTM.tm_year > aLastWateredTM.tm_year || aNowTM.tm_yday > aLastWateredTM.tm_yday;
}

// GOTY @Patoke: 0x5292A0
void ZenGarden::RefreshPlantNeeds(PottedPlant* thePottedPlant)
{
    if (thePottedPlant->mPlantAge != PottedPlantAge::PLANTAGE_FULL || !PlantShouldRefreshNeed(thePottedPlant))
    {
        return;
    }

    if (Plant::IsAquatic(thePottedPlant->mSeedType))
    {
        thePottedPlant->mLastWateredTime = _time64(nullptr);
        thePottedPlant->mPlantNeed = (PottedPlantNeed)RandRangeInt((int)PottedPlantNeed::PLANTNEED_BUGSPRAY, (int)PottedPlantNeed::PLANTNEED_PHONOGRAPH);
    }
    else
    {
        thePottedPlant->mTimesFed = 0;
        thePottedPlant->mPlantNeed = PottedPlantNeed::PLANTNEED_NONE;
    }
}

//0x51E990
void ZenGarden::UpdatePlantNeeds()
{
    if (mApp->mPlayerInfo == nullptr)
    {
        return;
    }

    for (int i = 0; i < mApp->mPlayerInfo->mNumPottedPlants; i++)
    {
        PottedPlant* aPottedPlant = PottedPlantFromIndex(i);
        RefreshPlantNeeds(aPottedPlant);
    }
}

bool ZenGarden::WasPlantFertilizedInLastHour(PottedPlant* thePottedPlant)
{
    return _time64(nullptr) - thePottedPlant->mLastFertilizedTime < 3600;
}

//0x51EA30
PottedPlantNeed ZenGarden::GetPlantsNeed(PottedPlant* thePottedPlant)
{
    if (thePottedPlant->mPlantAge != PottedPlantAge::PLANTAGE_SPROUT && 
        Plant::IsNocturnal(thePottedPlant->mSeedType) && 
        thePottedPlant->mWhichZenGarden == GardenType::GARDEN_MAIN)
    {
        return PottedPlantNeed::PLANTNEED_NONE;
    }
    if (thePottedPlant->mWhichZenGarden == GardenType::GARDEN_WHEELBARROW)
    {
        return PottedPlantNeed::PLANTNEED_NONE;
    }

    __time64_t aNow = _time64(nullptr);
    bool aTooLongSinceWatering = aNow - thePottedPlant->mLastWateredTime > 15;
    bool aTooShortSinceWatering = aNow - thePottedPlant->mLastWateredTime < 3;

    if (WasPlantFertilizedInLastHour(thePottedPlant) || WasPlantNeedFulfilledToday(thePottedPlant))
    {
        return PottedPlantNeed::PLANTNEED_NONE;
    }
    if (Plant::IsAquatic(thePottedPlant->mSeedType) && thePottedPlant->mPlantAge != PottedPlantAge::PLANTAGE_SPROUT)
    {
        if (thePottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_FULL)
        {
            if (PlantShouldRefreshNeed(thePottedPlant))
            {
                return PottedPlantNeed::PLANTNEED_NONE;
            }
            return thePottedPlant->mPlantNeed;
        }
        else
        {
            if (thePottedPlant->mWhichZenGarden != GardenType::GARDEN_AQUARIUM)
            {
                return PottedPlantNeed::PLANTNEED_NONE;
            }
            return PottedPlantNeed::PLANTNEED_FERTILIZER;
        }
    }
    if (!aTooLongSinceWatering)
    {
        return PottedPlantNeed::PLANTNEED_NONE;
    }
    if (thePottedPlant->mTimesFed < thePottedPlant->mFeedingsPerGrow)
    {
        return PottedPlantNeed::PLANTNEED_WATER;
    }
    if (aTooShortSinceWatering)
    {
        return PottedPlantNeed::PLANTNEED_NONE;
    }
    if (thePottedPlant->mPlantAge != PottedPlantAge::PLANTAGE_FULL)
    {
        return PottedPlantNeed::PLANTNEED_FERTILIZER;
    }
    if (PlantShouldRefreshNeed(thePottedPlant))
    {
        return PottedPlantNeed::PLANTNEED_NONE;
    }
    if (thePottedPlant->mPlantNeed != PottedPlantNeed::PLANTNEED_NONE)
    {
        return thePottedPlant->mPlantNeed;
    }
    return PottedPlantNeed::PLANTNEED_WATER;
}

//0x51EB70
void ZenGarden::MouseDownWithFeedingTool(int x, int y, CursorType theCursorType)
{
    Plant* aPlantToFeed = nullptr;
    {
        Plant* aPlant = nullptr;
        while (mBoard->IteratePlants(aPlant))
        {
            if (aPlant->mHighlighted && aPlant->mPottedPlantIndex != -1)
            {
                aPlantToFeed = aPlant;
                break;
            }
        }
    }

    if (theCursorType == CursorType::CURSOR_TYPE_CHOCOLATE)
    {
        TOD_ASSERT(mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_CHOCOLATE] > PURCHASE_COUNT_OFFSET);

        GridItem* aStinky = GetStinky();
        if (aStinky && aStinky->mHighlighted)
        {
            WakeStinky();
            mApp->AddTodParticle(aStinky->mPosX + 40.0f, aStinky->mPosY + 40.0f, aStinky->mRenderOrder + 1, ParticleEffect::PARTICLE_PRESENT_PICKUP);
            mApp->mPlayerInfo->mLastStinkyChocolateTime = _time64(nullptr);
            mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_CHOCOLATE]--;

            mApp->PlayFoley(FoleyType::FOLEY_WAKEUP);
            mApp->PlaySample(SOUND_MINDCONTROLLED);
        }

        if (aPlantToFeed)
        {
            mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_CHOCOLATE]--;
            FeedChocolateToPlant(aPlantToFeed);
            mApp->PlayFoley(FoleyType::FOLEY_WAKEUP);
        }
    }

    if (aPlantToFeed)
    {
        GridItem* aZenTool = mBoard->mGridItems.DataArrayAlloc();
        aZenTool->mGridItemType = GridItemType::GRIDITEM_ZEN_TOOL;
        aZenTool->mGridX = aPlantToFeed->mPlantCol;
        aZenTool->mGridY = aPlantToFeed->mRow;
        aZenTool->mPosX = aPlantToFeed->mX + 40;
        aZenTool->mPosY = aPlantToFeed->mY + 40;
        aZenTool->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_ABOVE_UI, 0, 0);

        if (theCursorType == CursorType::CURSOR_TYPE_WATERING_CAN)
        {
            if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_GOLD_WATERINGCAN])
            {
                aZenTool->mPosX = x;
                aZenTool->mPosY = y;
                Reanimation* aWateringCanReanim = mApp->AddReanimation(x, y, 0, ReanimationType::REANIM_ZENGARDEN_WATERINGCAN);
                aWateringCanReanim->PlayReanim("anim_water_area", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 8.0f);
                aZenTool->mGridItemReanimID = mApp->ReanimationGetID(aWateringCanReanim);
                aZenTool->mGridItemState = GridItemState::GRIDITEM_STATE_ZEN_TOOL_GOLD_WATERING_CAN;
                mApp->PlayFoley(FoleyType::FOLEY_WATERING);
            }
            else
            {
                Reanimation* aWateringCanReanim = mApp->AddReanimation(aPlantToFeed->mX + 32, aPlantToFeed->mY, 0, ReanimationType::REANIM_ZENGARDEN_WATERINGCAN);
                aWateringCanReanim->PlayReanim("anim_water", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 0.0f);
                aZenTool->mGridItemReanimID = mApp->ReanimationGetID(aWateringCanReanim);
                aZenTool->mGridItemState = GridItemState::GRIDITEM_STATE_ZEN_TOOL_GOLD_WATERING_CAN;
                mApp->PlayFoley(FoleyType::FOLEY_WATERING);
            }
        }
        else if (theCursorType == CursorType::CURSOR_TYPE_FERTILIZER)
        {
            Reanimation* aFertilizerReanim = mApp->AddReanimation(aPlantToFeed->mX, aPlantToFeed->mY, 0, ReanimationType::REANIM_ZENGARDEN_FERTILIZER);
            aFertilizerReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aZenTool->mGridItemReanimID = mApp->ReanimationGetID(aFertilizerReanim);
            aZenTool->mGridItemState = GridItemState::GRIDITEM_STATE_ZEN_TOOL_FERTILIZER;
            mApp->PlayFoley(FoleyType::FOLEY_FERTILIZER);

            TOD_ASSERT(mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_FERTILIZER] > PURCHASE_COUNT_OFFSET);
            mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_FERTILIZER]--;
        }
        else if (theCursorType == CursorType::CURSOR_TYPE_BUG_SPRAY)
        {
            Reanimation* aBugSprayReanim = mApp->AddReanimation(aPlantToFeed->mX + 54, aPlantToFeed->mY, 0, ReanimationType::REANIM_ZENGARDEN_BUGSPRAY);
            aBugSprayReanim->mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
            aZenTool->mGridItemReanimID = mApp->ReanimationGetID(aBugSprayReanim);
            aZenTool->mGridItemState = GridItemState::GRIDITEM_STATE_ZEN_TOOL_BUG_SPRAY;
            mApp->PlayFoley(FoleyType::FOLEY_BUGSPRAY);

            TOD_ASSERT(mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_BUG_SPRAY] > PURCHASE_COUNT_OFFSET);
            mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_BUG_SPRAY]--;
        }
        else if (theCursorType == CursorType::CURSOR_TYPE_PHONOGRAPH)
        {
            Reanimation* aPhonographReanim = mApp->AddReanimation(aPlantToFeed->mX + 20, aPlantToFeed->mY + 34, 0, ReanimationType::REANIM_ZENGARDEN_PHONOGRAPH);
            aPhonographReanim->mAnimRate = 20.0f;
            aPhonographReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
            aZenTool->mGridItemReanimID = mApp->ReanimationGetID(aPhonographReanim);
            aZenTool->mGridItemState = GridItemState::GRIDITEM_STATE_ZEN_TOOL_PHONOGRAPH;
            mApp->PlayFoley(FoleyType::FOLEY_PHONOGRAPH);
        }
    }

    mBoard->ClearCursor();
}

//0x51F220
void ZenGarden::FeedChocolateToPlant(Plant* thePlant)
{
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    aPottedPlant->mLastChocolateTime = _time64(nullptr);
    thePlant->mLaunchCounter = 60;
    mApp->AddTodParticle(thePlant->mX + 40.0f, thePlant->mY + 40.0f, thePlant->mRenderOrder + 1, ParticleEffect::PARTICLE_PRESENT_PICKUP);
}

//0x51F2A0
void ZenGarden::DoFeedingTool(int x, int y, GridItemState theToolType)
{
    if (theToolType == GridItemState::GRIDITEM_STATE_ZEN_TOOL_GOLD_WATERING_CAN)
    {
        Plant* aPlant = nullptr;
        while (mBoard->IteratePlants(aPlant))
        {
            if (mBoard->IsPlantInGoldWateringCanRange(x, y, aPlant))
            {
                PottedPlant* aPottedPlant = PottedPlantFromIndex(aPlant->mPottedPlantIndex);
                if (GetPlantsNeed(aPottedPlant) == PottedPlantNeed::PLANTNEED_WATER)
                {
                    PlantWatered(aPlant);
                }
            }
        }
        return;
    }

    int aGridX = PixelToGridX(x, y);
    int aGridY = PixelToGridY(x, y);
    Plant* aPlant = mBoard->GetTopPlantAt(aGridX, aGridY, PlantPriority::TOPPLANT_ZEN_TOOL_ORDER);
    if (aPlant)
    {
        PottedPlant* aPottedPlant = PottedPlantFromIndex(aPlant->mPottedPlantIndex);
        PottedPlantNeed aNeed = GetPlantsNeed(aPottedPlant);
        if (aNeed == PottedPlantNeed::PLANTNEED_WATER && theToolType == GridItemState::GRIDITEM_STATE_ZEN_TOOL_WATERING_CAN)
        {
            PlantWatered(aPlant);
        }
        else if (aNeed == PottedPlantNeed::PLANTNEED_FERTILIZER && theToolType == GridItemState::GRIDITEM_STATE_ZEN_TOOL_FERTILIZER)
        {
            PlantFertilized(aPlant);
        }
        else if (aNeed == PottedPlantNeed::PLANTNEED_BUGSPRAY && theToolType == GridItemState::GRIDITEM_STATE_ZEN_TOOL_BUG_SPRAY)
        {
            PlantFulfillNeed(aPlant);
        }
        else if (aNeed == PottedPlantNeed::PLANTNEED_PHONOGRAPH && theToolType == GridItemState::GRIDITEM_STATE_ZEN_TOOL_PHONOGRAPH)
        {
            PlantFulfillNeed(aPlant);
        }

        if (mBoard->mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_FERTILIZE_PLANTS && theToolType == GridItemState::GRIDITEM_STATE_ZEN_TOOL_FERTILIZER)
        {
            if (AllPlantsHaveBeenFertilized())
            {
                mApp->mBoard->mTutorialState = TutorialState::TUTORIAL_ZEN_GARDEN_COMPLETED;
                mApp->mBoard->DisplayAdvice(_S("[ADVICE_ZEN_GARDEN_CONTINUE_ADVENTURE]"), MessageStyle::MESSAGE_STYLE_HINT_TALL_FAST, AdviceType::ADVICE_NONE);
                mBoard->mMenuButton->mDisabled = false;
                mBoard->mMenuButton->mBtnNoDraw = false;
            }
            else if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_FERTILIZER] == PURCHASE_COUNT_OFFSET)
            {
                mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_FERTILIZER] = PURCHASE_COUNT_OFFSET + 5;
                mApp->mBoard->DisplayAdvice(_S("[ADVICE_ZEN_GARDEN_NEED_MORE_FERTILIZER]"), MessageStyle::MESSAGE_STYLE_HINT_TALL_FAST, AdviceType::ADVICE_NONE);
            }
        }
    }
}

//0x51F580
void ZenGarden::MouseDownWithTool(int x, int y, CursorType theCursorType)
{
    if (theCursorType == CursorType::CURSOR_TYPE_WHEEELBARROW && GetPottedPlantInWheelbarrow())
    {
        MouseDownWithFullWheelBarrow(x, y);
        mBoard->ClearCursor();
        return;
    }

    if (theCursorType == CursorType::CURSOR_TYPE_WATERING_CAN || 
        theCursorType == CursorType::CURSOR_TYPE_FERTILIZER || 
        theCursorType == CursorType::CURSOR_TYPE_BUG_SPRAY || 
        theCursorType == CursorType::CURSOR_TYPE_PHONOGRAPH || 
        theCursorType == CursorType::CURSOR_TYPE_CHOCOLATE)
    {
        MouseDownWithFeedingTool(x, y, theCursorType);
        return;
    }

    Plant* aPlant = mBoard->ToolHitTest(x, y);
    if (aPlant == nullptr || aPlant->mPottedPlantIndex == -1)
    {
        mApp->PlayFoley(FoleyType::FOLEY_DROP);
        mBoard->ClearCursor();
        return;
    }

    if (theCursorType == CursorType::CURSOR_TYPE_MONEY_SIGN)
    {
        MouseDownWithMoneySign(aPlant);
    }
    else if (theCursorType == CursorType::CURSOR_TYPE_WHEEELBARROW)
    {
        MouseDownWithEmptyWheelBarrow(aPlant);
        mBoard->ClearCursor();
    }
    else if (theCursorType == CursorType::CURSOR_TYPE_GLOVE)
    {
        mBoard->mCursorObject->mType = aPlant->mSeedType;
        mBoard->mCursorObject->mImitaterType = aPlant->mImitaterType;
        mBoard->mCursorObject->mCursorType = CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE;
        mBoard->mCursorObject->mGlovePlantID = (PlantID)mBoard->mPlants.DataArrayGetID(aPlant);
        //mBoard->mIgnoreMouseUp = true;
        mApp->PlaySample(SOUND_TAP);
    }
}

//0x51F700
void ZenGarden::MovePlant(Plant* thePlant, int theGridX, int theGridY)
{
    if (mApp->mGameMode != GameMode::GAMEMODE_CHALLENGE_ZEN_GARDEN)
    {
        return;
    }

    int aPosX = mBoard->GridToPixelX(theGridX, theGridY);
    int aPosY = mBoard->GridToPixelY(theGridX, theGridY);
    TOD_ASSERT(mBoard->GetTopPlantAt(theGridX, theGridY, PlantPriority::TOPPLANT_ANY) == nullptr);

    //bool aIsSleeping = thePlant->mIsAsleep; // unused
    thePlant->SetSleeping(false);
    Plant* aTopPlantAtGrid = mBoard->GetTopPlantAt(thePlant->mPlantCol, thePlant->mRow, PlantPriority::TOPPLANT_ONLY_UNDER_PLANT);
    if (aTopPlantAtGrid)
    {
        aTopPlantAtGrid->mX = aPosX;
        aTopPlantAtGrid->mY = aPosY;
        aTopPlantAtGrid->mPlantCol = theGridX;
        aTopPlantAtGrid->mRow = theGridY;
        aTopPlantAtGrid->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PLANT, 0, aPosY);
    }
    float aDeltaX = aPosX - thePlant->mX;
    float aDeltaY = aPosY - thePlant->mY;
    thePlant->mX = aPosX;
    thePlant->mY = aPosY;
    thePlant->mPlantCol = theGridX;
    thePlant->mRow = theGridY;
    thePlant->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PLANT, 0, aPosY + 1);

    TodParticleSystem* aParticle = mApp->ParticleTryToGet(thePlant->mParticleID);
    if (aParticle && aParticle->mEmitterList.mSize)
    {
        TodParticleEmitter* aEmitter = aParticle->mParticleHolder->mEmitters.DataArrayGet((unsigned int)aParticle->mEmitterList.GetHead()->mValue);
        aParticle->SystemMove(aEmitter->mSystemCenter.x + aDeltaX, aEmitter->mSystemCenter.y + aDeltaY);
    }

    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    aPottedPlant->mX = theGridX;
    aPottedPlant->mY = theGridY;
    if (thePlant->mState == PlantState::STATE_ZEN_GARDEN_HAPPY)
    {
        RemoveHappyEffect(thePlant);
        AddHappyEffect(thePlant);
    }

    if (aTopPlantAtGrid)
    {
        mBoard->DoPlantingEffects(theGridX, theGridY, aTopPlantAtGrid);
    }
    else
    {
        mBoard->DoPlantingEffects(theGridX, theGridY, thePlant);
    }
}

//0x51F950
float ZenGarden::PlantPottedDrawHeightOffset(SeedType theSeedType, float theScale)
{
    float aScaleOffsetFix = 0.0f;
    float aHeightOffset = 0.0f;
    if (theSeedType == SeedType::SEED_GRAVEBUSTER)
    {
        aHeightOffset += 50.0f;
        aScaleOffsetFix += 15.0f;
    }
    else if (theSeedType == SeedType::SEED_PUFFSHROOM)
    {
        aHeightOffset += 10.0f;
        aScaleOffsetFix += 24.0f;
    }
    else if (theSeedType == SeedType::SEED_SUNSHROOM)
    {
        aHeightOffset += 10.0f;
        aScaleOffsetFix += 17.0f;
    }
    else if (theSeedType == SeedType::SEED_SCAREDYSHROOM)
    {
        aHeightOffset += 5.0f;
        aScaleOffsetFix += 5.0f;
    }
    else if (theSeedType == SeedType::SEED_TANGLEKELP)
    {
        aHeightOffset -= 18.0f;
        aScaleOffsetFix += 20.0f;
    }
    else if (theSeedType == SeedType::SEED_SEASHROOM)
    {
        aHeightOffset -= 20.0f;
        aScaleOffsetFix += 15.0f;
    }
    else if (theSeedType == SeedType::SEED_LILYPAD)
    {
        aHeightOffset -= 10.0f;
        aScaleOffsetFix += 30.0f;
    }
    else if (theSeedType == SeedType::SEED_CHOMPER)
    {
        aScaleOffsetFix += 0.0f;
    }
    else if (theSeedType == SeedType::SEED_HYPNOSHROOM || theSeedType == SeedType::SEED_MARIGOLD || theSeedType == SeedType::SEED_PEASHOOTER || 
        theSeedType == SeedType::SEED_REPEATER || theSeedType == SeedType::SEED_LEFTPEATER || theSeedType == SeedType::SEED_SNOWPEA || 
        theSeedType == SeedType::SEED_THREEPEATER || theSeedType == SeedType::SEED_SUNFLOWER || theSeedType == SeedType::SEED_MARIGOLD)
    {
        aScaleOffsetFix += 10.0f;
    }
    else if (theSeedType == SeedType::SEED_STARFRUIT)
    {
        aHeightOffset += 10.0f;
        aScaleOffsetFix += 24.0f;
    }
    else if (theSeedType == SeedType::SEED_CABBAGEPULT || theSeedType == SeedType::SEED_MELONPULT)
    {
        aScaleOffsetFix += 10.0f;
        aHeightOffset += 3.0f;
    }
    else if (theSeedType == SeedType::SEED_POTATOMINE)
    {
        aScaleOffsetFix += 5.0f;
    }
    else if (theSeedType == SeedType::SEED_TORCHWOOD)
    {
        aScaleOffsetFix += 3.0f;
    }
    else if (theSeedType == SeedType::SEED_SPIKEWEED)
    {
        aScaleOffsetFix += 10.0f;
        aHeightOffset -= 13.0f;
    }
    else if (theSeedType == SeedType::SEED_BLOVER)
    {
        aScaleOffsetFix += 10.0f;
    }
    else if (theSeedType == SeedType::SEED_PUMPKINSHELL)
    {
        aScaleOffsetFix += 20.0f;
    }
    else if (theSeedType == SeedType::SEED_PLANTERN)
    {
        aScaleOffsetFix -= 1.0f;
    }

    return aHeightOffset + (aScaleOffsetFix * theScale - aScaleOffsetFix);
}

float ZenGarden::ZenPlantOffsetX(PottedPlant* thePottedPlant)
{
    int aOffsetX = 0;
    if (thePottedPlant->mFacing == PottedPlant::FacingDirection::FACING_LEFT && thePottedPlant->mSeedType == SeedType::SEED_POTATOMINE)
    {
        aOffsetX -= 6;
    }
    return aOffsetX;
}

bool ZenGarden::HasPurchasedStinky()
{
    return mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_STINKY_THE_SNAIL] != 0;
}

//0x51FB40
void ZenGarden::AddStinky()
{
    if (!HasPurchasedStinky() || mGardenType != GardenType::GARDEN_MAIN)
    {
        return;
    }

    if (!mApp->mPlayerInfo->mHasSeenStinky)
    {
        mApp->mPlayerInfo->mHasSeenStinky = 1;
        mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_STINKY_THE_SNAIL] = _time64(nullptr);
    }

    GridItem* aStinky = mBoard->mGridItems.DataArrayAlloc();
    aStinky->mGridItemType = GridItemType::GRIDITEM_STINKY;
    aStinky->mPosX = mApp->mPlayerInfo->mStinkyPosX;
    aStinky->mPosY = mApp->mPlayerInfo->mStinkyPosY;
    aStinky->mGoalX = aStinky->mPosX;
    aStinky->mGoalY = aStinky->mPosY;
    Reanimation* aReanimStinky = mApp->AddReanimation(aStinky->mPosX, aStinky->mPosY, 0, ReanimationType::REANIM_STINKY);
    aReanimStinky->OverrideScale(0.8f, 0.8f);
    aStinky->mGridItemReanimID = mApp->ReanimationGetID(aReanimStinky);

    if (mApp->mPlayerInfo->mStinkyPosX == 0)
    {
        StinkyPickGoal(aStinky);
        aStinky->mPosX = aStinky->mGoalX;
        aStinky->mPosY = aStinky->mGoalY;
    }

    if (ShouldStinkyBeAwake())
    {
        aReanimStinky->PlayReanim("anim_crawl", ReanimLoopType::REANIM_LOOP, 0, 6.0f);
        aStinky->mGridItemState = GridItemState::GRIDITEM_STINKY_WALKING_LEFT;
    }
    else
    {
        aStinky->mPosY = STINKY_SLEEP_POS_Y;
        StinkyFinishFallingAsleep(aStinky, 0);
    }

    aStinky->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PLANT, 0, aStinky->mPosY - 30.0f);
    aReanimStinky->SetPosition(aStinky->mPosX, aStinky->mPosY);
}

//0x51FCD0
void ZenGarden::StinkyPickGoal(GridItem* theStinky)
{
    float aCurDistToGoal = Distance2D(theStinky->mGoalX, theStinky->mGoalY, theStinky->mPosX, theStinky->mPosY);

    Coin* aBestCoin = nullptr;
    float aCurWeight = 0.0f;
    {
        Coin* aCoin = nullptr;
        while (mBoard->IterateCoins(aCoin))
        {
            if (!aCoin->mIsBeingCollected && aCoin->mPosY == aCoin->mGroundY)
            {
                float aWeight = Distance2D(aCoin->mPosX, aCoin->mPosY + 30.0f, theStinky->mPosX, theStinky->mPosY);
                if (aCoin->mType == CoinType::COIN_GOLD)
                {
                    aWeight -= 40.0f;
                }
                else if (aCoin->mType == CoinType::COIN_DIAMOND)
                {
                    aWeight -= 80.0f;
                }

                float aDistFromLastGoal = Distance2D(aCoin->mPosX, aCoin->mPosY + 30.0f, theStinky->mGoalX, theStinky->mGoalY);
                if (aDistFromLastGoal < 5.0f)
                {
                    aWeight -= 20.0f;
                    aWeight += TodAnimateCurve(3000, 6000, aCoin->mDisappearCounter, 0, -40, TodCurves::CURVE_LINEAR);
                }

                if (aBestCoin == nullptr || aWeight < aCurWeight)
                {
                    aBestCoin = aCoin;
                    aCurWeight = aWeight;
                }
            }
        }
    }

    if (aBestCoin)
    {
        theStinky->mGoalX = aBestCoin->mPosX;
        theStinky->mGoalY = aBestCoin->mPosY + 30.0f;
    }
    else
    {
        if (aCurDistToGoal > 10.0f)
        {
            return;
        }

        TodWeightedGridArray aPicks[MAX_GRID_SIZE_X * MAX_GRID_SIZE_Y];
        int aPickCount = 0;

        int aCount;
        SpecialGridPlacement* aSpecialGrids = GetSpecialGridPlacements(aCount);
        TOD_ASSERT(aCount < MAX_GRID_SIZE_X * MAX_GRID_SIZE_Y);

        for (int i = 0; i < aCount; i++)
        {
            SpecialGridPlacement& aGrid = aSpecialGrids[i];
            Plant* aPlant = mBoard->GetTopPlantAt(aGrid.mGridX, aGrid.mGridY, PlantPriority::TOPPLANT_ANY);
            aPicks[aPickCount].mX = aGrid.mPixelX + 15;
            aPicks[aPickCount].mY = aGrid.mPixelY + 80;

            if (aPlant)
            {
                aPicks[aPickCount].mWeight = 2000 - abs(aPicks[aPickCount].mY - theStinky->mPosY);
            }
            else
            {
                aPicks[aPickCount].mWeight = 1;
            }

            aPickCount++;
        }

        TodWeightedGridArray* aTarget = TodPickFromWeightedGridArray(aPicks, aPickCount);
        theStinky->mGoalX = aTarget->mX;
        theStinky->mGoalY = aTarget->mY;
    }

    theStinky->mGridItemCounter = 100;
    if (theStinky->mGoalX < theStinky->mPosX && theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_RIGHT)
    {
        Reanimation* aStinyReanim = mApp->ReanimationGet(theStinky->mGridItemReanimID);
        theStinky->mGridItemState = GridItemState::GRIDITEM_STINKY_TURNING_LEFT;
        aStinyReanim->PlayReanim("turn", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 6.0f);
        theStinky->mMotionTrailCount = 0;
    }
    else if (theStinky->mGoalX > theStinky->mPosX && theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_LEFT)
    {
        Reanimation* aStinyReanim = mApp->ReanimationGet(theStinky->mGridItemReanimID);
        theStinky->mGridItemState = GridItemState::GRIDITEM_STINKY_TURNING_RIGHT;
        aStinyReanim->PlayReanim("turn", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 6.0f);
        theStinky->mMotionTrailCount = 0;
    }
}

//0x520120
void ZenGarden::StinkyWakeUp(GridItem* theStinky)
{
    Reanimation* aStinkyReanim = mApp->ReanimationGet(theStinky->mGridItemReanimID);
    aStinkyReanim->PlayReanim("anim_out", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 6.0f);
    theStinky->mGridItemState = GridItemState::GRIDITEM_STINKY_WAKING_UP;

    Reanimation* aSleepingReanim = FindReanimAttachment(aStinkyReanim->GetTrackInstanceByName("shell")->mAttachmentID);
    aSleepingReanim->ReanimationDie();

    gLawnApp->mPlayerInfo->mHasWokenStinky = TRUE;
}

//0x5201D0
void ZenGarden::StinkyStartFallingAsleep(GridItem* theStinky)
{
    Reanimation* aStinkyReanim = mApp->ReanimationGet(theStinky->mGridItemReanimID);
    aStinkyReanim->PlayReanim("anim_in", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 20, 6.0f);
    theStinky->mGridItemState = GridItemState::GRIDITEM_STINKY_FALLING_ASLEEP;
}

//0x520240
void ZenGarden::StinkyFinishFallingAsleep(GridItem* theStinky, int theBlendTime)
{
    Reanimation* aStinkyReanim = mApp->ReanimationGet(theStinky->mGridItemReanimID);
    aStinkyReanim->PlayReanim("anim_out", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, theBlendTime, 0.0f);
    aStinkyReanim->mAnimRate = 0.0f;

    Reanimation* aSleepingReanim = mApp->AddReanimation(0.0f, 0.0f, 0, ReanimationType::REANIM_SLEEPING);
    aSleepingReanim->mAnimRate = 3.0f;
    aSleepingReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
    AttachReanim(aStinkyReanim->GetTrackInstanceByName("shell")->mAttachmentID, aSleepingReanim, 34.0f, 39.0f);

    theStinky->mGridItemState = GridItemState::GRIDITEM_STINKY_SLEEPING;
    if (!gLawnApp->mPlayerInfo->mHasWokenStinky)
    {
        mApp->mBoard->DisplayAdvice(_S("[ADVICE_STINKY_SLEEPING]"), MessageStyle::MESSAGE_STYLE_HINT_LONG, AdviceType::ADVICE_STINKY_SLEEPING);
    }
}

//0x5203E0
void ZenGarden::UpdateStinkyMotionTrail(GridItem* theStinky, bool theStinkyHighOnChocolate)
{
    Reanimation* aStinkyReanim = mApp->ReanimationGet(theStinky->mGridItemReanimID);
    if (!theStinkyHighOnChocolate)
    {
        theStinky->mMotionTrailCount = 0;
        return;
    }
    if (theStinky->mGridItemState != GridItemState::GRIDITEM_STINKY_WALKING_RIGHT && theStinky->mGridItemState != GridItemState::GRIDITEM_STINKY_WALKING_LEFT)
    {
        theStinky->mMotionTrailCount = 0;
        return;
    }

    if (theStinky->mMotionTrailCount == NUM_MOTION_TRAIL_FRAMES)
    {
        theStinky->mMotionTrailCount--;
    }
    if (theStinky->mMotionTrailCount > 0)
    {
        memcpy(theStinky->mMotionTrailFrames + 1, theStinky->mMotionTrailFrames, theStinky->mMotionTrailCount * sizeof(MotionTrailFrame));
    }

    theStinky->mMotionTrailFrames[0].mPosX = theStinky->mPosX;
    theStinky->mMotionTrailFrames[0].mPosY = theStinky->mPosY;
    theStinky->mMotionTrailFrames[0].mAnimTime = aStinkyReanim->mAnimTime;
    theStinky->mMotionTrailCount++;
}

//0x520470
void ZenGarden::StinkyAnimRateUpdate(GridItem* theStinky)
{
    Reanimation* aStinkyReanim = mApp->ReanimationGet(theStinky->mGridItemReanimID);
    if (IsStinkyHighOnChocolate())
    {
        if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_LEFT || 
            theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_RIGHT ||
            theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_TURNING_RIGHT || 
            theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_TURNING_LEFT)
        {
            aStinkyReanim->mAnimRate = 12.0f;
        }
    }
    else
    {
        if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_LEFT ||
            theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_RIGHT ||
            theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_TURNING_RIGHT ||
            theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_TURNING_LEFT)
        {
            aStinkyReanim->mAnimRate = 6.0f;
        }
    }
}

void ZenGarden::ResetStinkyTimers()
{
    mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_STINKY_THE_SNAIL] = 2;
    mApp->mPlayerInfo->mLastStinkyChocolateTime = 0;
}

//0x520500
void ZenGarden::StinkyUpdate(GridItem* theStinky)
{
    Reanimation* aStinkyReanim = mApp->ReanimationGet(theStinky->mGridItemReanimID);

    __time64_t aNow = _time64(nullptr);
    if (mApp->mPlayerInfo->mLastStinkyChocolateTime > aNow || 
        mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_STINKY_THE_SNAIL] > aNow)
    {
        ResetStinkyTimers();
    }

    bool aStinkyHighOnChocolate = IsStinkyHighOnChocolate();
    UpdateStinkyMotionTrail(theStinky, aStinkyHighOnChocolate);

    if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_FALLING_ASLEEP)
    {
        if (aStinkyReanim->mLoopCount > 0)
        {
            StinkyFinishFallingAsleep(theStinky, 20);
        }
        return;
    }

    if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_SLEEPING)
    {
        Reanimation* aSleepingReanim = FindReanimAttachment(aStinkyReanim->GetTrackInstanceByName("shell")->mAttachmentID);
        TOD_ASSERT(aSleepingReanim);

        if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_CHOCOLATE)
        {
            aSleepingReanim->AssignRenderGroupToPrefix("z", RENDER_GROUP_HIDDEN);
        }
        else
        {
            aSleepingReanim->AssignRenderGroupToPrefix("z", RENDER_GROUP_NORMAL);
        }

        if (ShouldStinkyBeAwake())
        {
            StinkyWakeUp(theStinky);
        }
        return;
    }

    if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WAKING_UP)
    {
        if (aStinkyReanim->mLoopCount > 0)
        {
            theStinky->mGridItemState = GridItemState::GRIDITEM_STINKY_WALKING_LEFT;
            aStinkyReanim->PlayReanim("anim_crawl", ReanimLoopType::REANIM_LOOP, 10, 6.0f);
            StinkyPickGoal(theStinky);
        }
        return;
    }

    if (!ShouldStinkyBeAwake())
    {
        if (theStinky->mPosY < STINKY_SLEEP_POS_Y)
        {
            if (theStinky->mGoalY != STINKY_SLEEP_POS_Y)
            {
                theStinky->mGoalY = STINKY_SLEEP_POS_Y + 10.0f;
            }
        }
        else
        {
            if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_LEFT)
            {
                StinkyStartFallingAsleep(theStinky);
                return;
            }
            else if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_RIGHT)
            {
                Reanimation* aReanim = mApp->ReanimationGet(theStinky->mGridItemReanimID);
                theStinky->mGridItemState = GridItemState::GRIDITEM_STINKY_TURNING_LEFT;
                aReanim->PlayReanim("turn", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 10, 6.0f);
                theStinky->mMotionTrailCount = 0;
                theStinky->mGoalX = theStinky->mPosX;
                theStinky->mGoalY = theStinky->mPosY;
                return;
            }
        }
    }

    if (theStinky->mGridItemCounter > 0)
    {
        theStinky->mGridItemCounter--;
    }

    Coin* aCoin = nullptr;
    while (mBoard->IterateCoins(aCoin))
    {
        if (!aCoin->mIsBeingCollected && Distance2D(aCoin->mPosX, aCoin->mPosY + 30.0f, theStinky->mPosX, theStinky->mPosY) < 20.0f)
        {
            aCoin->PlayCollectSound();
            aCoin->Collect();
        }
    }

    if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_LEFT || theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_RIGHT)
    {
        if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_CHOCOLATE && !IsStinkyHighOnChocolate())
        {
            if (!aStinkyReanim->IsAnimPlaying("anim_idle"))
            {
                aStinkyReanim->PlayReanim("anim_idle", ReanimLoopType::REANIM_LOOP, 10, 6.0f);
            }
        }
        else if (!aStinkyReanim->IsAnimPlaying("anim_crawl"))
        {
            aStinkyReanim->PlayReanim("anim_crawl", ReanimLoopType::REANIM_LOOP, 10, 6.0f);
        }
    }

    float aDeltaX = theStinky->mPosX - theStinky->mGoalX;
    float aDeltaY = theStinky->mPosY - theStinky->mGoalY;
    float aSpeedY = 0.5f;
    float aSpeedX = aStinkyReanim->GetTrackVelocity("_ground") * 15.0f;
    if (aStinkyHighOnChocolate)
    {
        aSpeedY = 1.0f;
        aSpeedX = max(aSpeedX, 0.5f);
    }
    else if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_CHOCOLATE)
    {
        aSpeedY = 0.0f;
        aSpeedX = 0.0f;
    }
    aSpeedY *= TodAnimateCurveFloatTime(20.0f, 5.0f, fabs(aDeltaY), 1.0f, 0.2f, TodCurves::CURVE_LINEAR);
    if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_LEFT)
    {
        theStinky->mPosX -= aSpeedX;
        if (theStinky->mPosX < theStinky->mGoalX)
        {
            theStinky->mPosX = theStinky->mGoalX;
        }
    }
    else if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_RIGHT)
    {
        theStinky->mPosX += aSpeedX;
        if (theStinky->mPosX > theStinky->mGoalX)
        {
            theStinky->mPosX = theStinky->mGoalX;
        }
    }

    if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_LEFT || theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_RIGHT)
    {
        if (fabs(aDeltaY) < aSpeedY)
        {
            theStinky->mPosY = theStinky->mGoalY;
        }
        else if (aDeltaY > 0.0f)
        {
            theStinky->mPosY -= aSpeedY;
        }
        else
        {
            theStinky->mPosY += aSpeedY;
        }

        if (fabs(aDeltaX) < 5.0f && fabs(aDeltaY) < 5.0f)
        {
            StinkyPickGoal(theStinky);
        }
        else if (theStinky->mGridItemCounter == 0)
        {
            StinkyPickGoal(theStinky);
        }
    }

    if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_TURNING_LEFT)
    {
        if (aStinkyReanim->mLoopCount > 0)
        {
            theStinky->mGridItemState = GridItemState::GRIDITEM_STINKY_WALKING_LEFT;
            aStinkyReanim->PlayReanim("anim_crawl", ReanimLoopType::REANIM_LOOP, 10, 6.0f);
        }
    }
    else if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_TURNING_RIGHT)
    {
        if (aStinkyReanim->mLoopCount > 0)
        {
            theStinky->mGridItemState = GridItemState::GRIDITEM_STINKY_WALKING_RIGHT;
            aStinkyReanim->PlayReanim("anim_crawl", ReanimLoopType::REANIM_LOOP, 10, 6.0f);
        }
    }

    StinkyAnimRateUpdate(theStinky);
    if (theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_WALKING_RIGHT || theStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_TURNING_LEFT)
    {
        aStinkyReanim->OverrideScale(-0.8f, 0.8f);
        aStinkyReanim->SetPosition(theStinky->mPosX + 69.0f, theStinky->mPosY);
    }
    else
    {
        aStinkyReanim->OverrideScale(0.8f, 0.8f);
        aStinkyReanim->SetPosition(theStinky->mPosX, theStinky->mPosY);
    }

    theStinky->mRenderOrder = Board::MakeRenderOrder(RenderLayer::RENDER_LAYER_PLANT, 0, theStinky->mPosY - 30.0f);
}

//0x520AC0
void ZenGarden::ZenToolUpdate(GridItem* theZenTool)
{
    Reanimation* aToolReanim = mApp->ReanimationTryToGet(theZenTool->mGridItemReanimID);
    if (aToolReanim == nullptr)
    {
        return;
    }

    int aPlayTime = 1;
    if (theZenTool->mGridItemState == GridItemState::GRIDITEM_STATE_ZEN_TOOL_PHONOGRAPH)
    {
        aPlayTime = 2;
    }
    if (aToolReanim->mLoopCount >= aPlayTime)
    {
        DoFeedingTool(theZenTool->mPosX, theZenTool->mPosY, theZenTool->mGridItemState);
        theZenTool->GridItemDie();
    }
}

//0x520B30
void ZenGarden::ZenGardenUpdate()
{
    if (mApp->GetDialog(Dialogs::DIALOG_STORE))
    {
        return;
    }

    mApp->UpdateCrazyDave();
    if (mBoard->mCursorObject->mCursorType != CursorType::CURSOR_TYPE_NORMAL)
    {
        mBoard->mChallenge->mChallengeState = ChallengeState::STATECHALLENGE_NORMAL;
        mBoard->mChallenge->mChallengeStateCounter = 3000;
    }
    else if (mApp->mBoard->mTutorialState == TutorialState::TUTORIAL_OFF)
    {
        if (mBoard->mChallenge->mChallengeStateCounter > 0)
        {
            mBoard->mChallenge->mChallengeStateCounter--;
        }
        if (mBoard->mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_NORMAL && mBoard->mChallenge->mChallengeStateCounter == 0)
        {
            mBoard->mChallenge->mChallengeState = ChallengeState::STATECHALLENGE_ZEN_FADING;
            mBoard->mChallenge->mChallengeStateCounter = 50;
        }
    }

    UpdatePlantNeeds();
    {
        Plant* aPlant = nullptr;
        while (mBoard->IteratePlants(aPlant))
        {
            if (aPlant->mPottedPlantIndex != -1)
            {
                PottedPlantUpdate(aPlant);
            }
        }
    }
    {
        GridItem* aGridItem = nullptr;
        while (mBoard->IterateGridItems(aGridItem))
        {
            if (aGridItem->mGridItemType == GridItemType::GRIDITEM_ZEN_TOOL)
            {
                ZenToolUpdate(aGridItem);
            }
            else if (aGridItem->mGridItemType == GridItemType::GRIDITEM_STINKY)
            {
                StinkyUpdate(aGridItem);
            }
        }
    }

    if (mBoard->mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_KEEP_WATERING && CountPlantsNeedingFertilizer() > 0)
    {
        mBoard->DisplayAdvice(_S("[ADVICE_ZEN_GARDEN_VISIT_STORE]"), MessageStyle::MESSAGE_STYLE_HINT_TALL_LONG, AdviceType::ADVICE_NONE);
        mBoard->mTutorialState = TutorialState::TUTORIAL_ZEN_GARDEN_VISIT_STORE;
        mBoard->mStoreButton->mDisabled = false;
        mBoard->mStoreButton->mBtnNoDraw = false;
    }
}

//0x520CF0
GridItem* ZenGarden::GetStinky()
{
    GridItem* aGridItem = nullptr;
    while (mBoard->IterateGridItems(aGridItem))
    {
        if (aGridItem->mGridItemType == GridItemType::GRIDITEM_STINKY)
        {
            return aGridItem;
        }
    }
    return nullptr;
}

//0x520D30
void ZenGarden::GotoNextGarden()
{
    LeaveGarden();
    mBoard->ClearAdvice(AdviceType::ADVICE_NONE);
    mBoard->mPlants.DataArrayFreeAll();
    mBoard->mCoins.DataArrayFreeAll();
    mApp->mEffectSystem->EffectSystemFreeAll();

    bool aGoToTree = false;
    if (mGardenType == GardenType::GARDEN_MAIN)
    {
        if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_MUSHROOM_GARDEN])
        {
            mGardenType = GardenType::GARDEN_MUSHROOM;
            mBoard->mBackground = BackgroundType::BACKGROUND_MUSHROOM_GARDEN;
        }
        else if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_AQUARIUM_GARDEN])
        {
            mGardenType = GardenType::GARDEN_AQUARIUM;
            mBoard->mBackground = BackgroundType::BACKGROUND_ZOMBIQUARIUM;
        }
        else if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_TREE_OF_WISDOM])
        {
            aGoToTree = true;
        }
    }
    else if (mGardenType == GardenType::GARDEN_MUSHROOM)
    {
        if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_AQUARIUM_GARDEN])
        {
            mGardenType = GardenType::GARDEN_AQUARIUM;
            mBoard->mBackground = BackgroundType::BACKGROUND_ZOMBIQUARIUM;
        }
        else if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_TREE_OF_WISDOM])
        {
            aGoToTree = true;
        }
        else
        {
            mGardenType = GardenType::GARDEN_MAIN;
            mBoard->mBackground = BackgroundType::BACKGROUND_GREENHOUSE;
        }
    }
    else if (mGardenType == GardenType::GARDEN_AQUARIUM)
    {
        if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_TREE_OF_WISDOM])
        {
            aGoToTree = true;
        }
        else
        {
            mGardenType = GardenType::GARDEN_MAIN;
            mBoard->mBackground = BackgroundType::BACKGROUND_GREENHOUSE;
        }
    }
    if (aGoToTree)
    {
        mApp->KillBoard();
        mApp->PreNewGame(GameMode::GAMEMODE_TREE_OF_WISDOM, false);
        return;
    }

    if (mBoard->mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN)
    {
        TodLoadResources("DelayLoad_MushroomGarden");
    }
    else if (mBoard->mBackground == BackgroundType::BACKGROUND_GREENHOUSE)
    {
        TodLoadResources("DelayLoad_GreenHouseGarden");
        TodLoadResources("DelayLoad_GreenHouseOverlay");
    }
    else if (mBoard->mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
    {
        TodLoadResources("DelayLoad_Zombiquarium");
        TodLoadResources("DelayLoad_GreenHouseOverlay");
    }
    else
    {
        TOD_ASSERT();
    }

    if ((mBoard->mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN || mBoard->mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM))
    {
        if (!mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_WHEEL_BARROW])
        {
            mBoard->DisplayAdvice(_S("[ADVICE_NEED_WHEELBARROW]"), MessageStyle::MESSAGE_STYLE_HINT_TALL_FAST, AdviceType::ADVICE_NEED_WHEELBARROW);
        }
    }

    ZenGardenInitLevel();
}

//0x5210F0
void ZenGarden::MouseDownWithFullWheelBarrow(int x, int y)
{
    PottedPlant* aPottedPlant = GetPottedPlantInWheelbarrow();
    TOD_ASSERT(aPottedPlant);

    if (mApp->mZenGarden->mGardenType == GardenType::GARDEN_AQUARIUM && !Plant::IsAquatic(aPottedPlant->mSeedType))
    {
        mBoard->DisplayAdvice(_S("[ZEN_ONLY_AQUATIC_PLANTS]"), MessageStyle::MESSAGE_STYLE_HINT_TALL_FAST, AdviceType::ADVICE_NONE);
        return;
    }

    int aGridX = mBoard->PixelToGridX(x, y);
    int aGridY = mBoard->PixelToGridY(x, y);
    if (aGridX == -1 || aGridY == -1 || mBoard->CanPlantAt(aGridX, aGridY, aPottedPlant->mSeedType) != PlantingReason::PLANTING_OK)
    {
        return;
    }

    aPottedPlant->mWhichZenGarden = mGardenType;
    aPottedPlant->mX = aGridX;
    aPottedPlant->mY = aGridY;
    intptr_t aPottedPlantIndex = ((intptr_t)aPottedPlant - (intptr_t)mApp->mPlayerInfo->mPottedPlant) / (intptr_t)sizeof(PottedPlant);
    Plant* aPlant = PlacePottedPlant(aPottedPlantIndex);
    mBoard->DoPlantingEffects(aPottedPlant->mX, aPottedPlant->mY, aPlant);
}

//0x521280
void ZenGarden::MouseDownWithEmptyWheelBarrow(Plant* thePlant)
{
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    RemovePottedPlant(thePlant);
    aPottedPlant->mWhichZenGarden = GardenType::GARDEN_WHEELBARROW;
    aPottedPlant->mX = 0;
    aPottedPlant->mY = 0;
    mApp->PlayFoley(FoleyType::FOLEY_PLANT);
}

//0x521310
PottedPlant* ZenGarden::GetPottedPlantInWheelbarrow()
{
    for (int i = 0; i < mApp->mPlayerInfo->mNumPottedPlants; i++)
    {
        PottedPlant* aPottedPlant = PottedPlantFromIndex(i);
        if (aPottedPlant->mWhichZenGarden == GardenType::GARDEN_WHEELBARROW)
        {
            return aPottedPlant;
        }
    }
    return nullptr;
}

SpecialGridPlacement* ZenGarden::GetSpecialGridPlacements(int& theCount)
{
    if (mBoard->mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN)
    {
        theCount = LENGTH(gMushroomGridPlacement);
        return gMushroomGridPlacement;
    }
    if (mBoard->mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
    {
        theCount = LENGTH(gAquariumGridPlacement);
        return gAquariumGridPlacement;
    }
    if (mBoard->mBackground == BackgroundType::BACKGROUND_GREENHOUSE)
    {
        theCount = LENGTH(gGreenhouseGridPlacement);
        return gGreenhouseGridPlacement;
    }
    TOD_ASSERT();
    return nullptr;
}

//0x521350
int ZenGarden::PixelToGridX(int theX, int theY)
{
    int aCount;
    SpecialGridPlacement* aSpecialGrids = GetSpecialGridPlacements(aCount);
    for (int i = 0; i < aCount; i++)
    {
        SpecialGridPlacement& aGrid = aSpecialGrids[i];
        if (theX >= aGrid.mPixelX && theX <= aGrid.mPixelX + 80 && theY >= aGrid.mPixelY && theY <= aGrid.mPixelY + 85)
        {
            return aGrid.mGridX;
        }
    }
    return -1;
}

//0x5213D0
int ZenGarden::PixelToGridY(int theX, int theY)
{
    int aCount;
    SpecialGridPlacement* aSpecialGrids = GetSpecialGridPlacements(aCount);
    for (int i = 0; i < aCount; i++)
    {
        SpecialGridPlacement& aGrid = aSpecialGrids[i];
        if (theX >= aGrid.mPixelX && theX <= aGrid.mPixelX + 80 && theY >= aGrid.mPixelY && theY <= aGrid.mPixelY + 85)
        {
            return aGrid.mGridY;
        }
    }
    return -1;
}

//0x521450
int ZenGarden::GridToPixelX(int theGridX, int theGridY)
{
    int aCount;
    SpecialGridPlacement* aSpecialGrids = GetSpecialGridPlacements(aCount);
    for (int i = 0; i < aCount; i++)
    {
        SpecialGridPlacement& aGrid = aSpecialGrids[i];
        if (theGridX == aGrid.mGridX && theGridY == aGrid.mGridY)
        {
            return aGrid.mPixelX;
        }
    }
    return -1;
}

//0x5214C0
int ZenGarden::GridToPixelY(int theGridX, int theGridY)
{
    int aCount;
    SpecialGridPlacement* aSpecialGrids = GetSpecialGridPlacements(aCount);
    for (int i = 0; i < aCount; i++)
    {
        SpecialGridPlacement& aGrid = aSpecialGrids[i];
        if (theGridX == aGrid.mGridX && theGridY == aGrid.mGridY)
        {
            return aGrid.mPixelY;
        }
    }
    return -1;
}

//0x521530
void ZenGarden::DrawBackdrop(Graphics* g)
{
    if (mGardenType != GardenType::GARDEN_AQUARIUM)
    {
        return;
    }

    if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_WHEEL_BARROW || 
        mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_WHEEELBARROW || 
        mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE)
    {
        int aCount;
        SpecialGridPlacement* aSpecialGrids = GetSpecialGridPlacements(aCount);
        for (int i = 0; i < aCount; i++)
        {
            SpecialGridPlacement& aGrid = aSpecialGrids[i];
            if (mBoard->GetTopPlantAt(aGrid.mGridX, aGrid.mGridY, PlantPriority::TOPPLANT_ZEN_TOOL_ORDER) == nullptr)
            {
                TodDrawImageCelScaled(g, IMAGE_PLANTSHADOW, aGrid.mPixelX - 35, (aGrid.mPixelY + 33), 0, 0, 1.7f, 1.7f);
            }
        }
    }
}

//0x521660
void ZenGarden::ShowTutorialArrowOnWateringCan()
{
    Rect aButtonRect = mBoard->GetShovelButtonRect();
    mBoard->GetZenButtonRect(GameObjectType::OBJECT_TYPE_WATERING_CAN, aButtonRect);
    mBoard->TutorialArrowShow(aButtonRect.mX + 10, aButtonRect.mY + 10);
    mBoard->DisplayAdvice(_S("[ADVICE_ZEN_GARDEN_PICK_UP_WATER]"), MessageStyle::MESSAGE_STYLE_ZEN_GARDEN_LONG, AdviceType::ADVICE_NONE);
    mBoard->mTutorialState = TutorialState::TUTORIAL_ZEN_GARDEN_PICKUP_WATER;
}

//0x5217A0
void ZenGarden::AdvanceCrazyDaveDialog()
{
    if (mApp->mCrazyDaveMessageIndex == -1 || mApp->GetDialog(Dialogs::DIALOG_STORE) || mApp->GetDialog(Dialogs::DIALOG_ZEN_SELL))
    {
        return;
    }

    if (mApp->mCrazyDaveMessageIndex == 2104)
    {
        ShowTutorialArrowOnWateringCan();
    }

    if (!mApp->AdvanceCrazyDaveText())
    {
        mApp->CrazyDaveLeave();
        return;
    }

    if (mApp->mCrazyDaveMessageIndex == 2102 && mApp->mPlayerInfo->mNumPottedPlants == 0)
    {
        for (int i = 0; i < 2; i++)
        {
            PottedPlant aPottedPlant;
            aPottedPlant.InitializePottedPlant(SeedType::SEED_MARIGOLD);
            aPottedPlant.mDrawVariation = (DrawVariation)RandRangeInt((int)DrawVariation::VARIATION_MARIGOLD_WHITE, (int)DrawVariation::VARIATION_MARIGOLD_LIGHT_GREEN);
            aPottedPlant.mFeedingsPerGrow = 3;
            mApp->mZenGarden->AddPottedPlant(&aPottedPlant);
        }
    }
}

//0x521880
bool ZenGarden::MouseDownZenGarden(int x, int y, int theClickCount, HitResult* theHitResult)
{
    if (mBoard->mChallenge->mChallengeState == ChallengeState::STATECHALLENGE_ZEN_FADING)
    {
        mBoard->mChallenge->mChallengeState = ChallengeState::STATECHALLENGE_NORMAL;
    }
    mBoard->mChallenge->mChallengeStateCounter = 3000;

    if (theHitResult->mObjectType == GameObjectType::OBJECT_TYPE_STINKY && mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_NORMAL)
    {
        WakeStinky();
    }
    else if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_GLOVE)
    {
        if (mBoard->CanUseGameObject(GameObjectType::OBJECT_TYPE_WHEELBARROW))
        {
            Rect aButtonRect = mBoard->GetShovelButtonRect();
            mBoard->GetZenButtonRect(GameObjectType::OBJECT_TYPE_WHEELBARROW, aButtonRect);

            PottedPlant* aPottedPlant = GetPottedPlantInWheelbarrow();
            if (aButtonRect.Contains(x, y) && aPottedPlant)
            {
                mBoard->ClearCursor();
                mBoard->mCursorObject->mType = aPottedPlant->mSeedType;
                mBoard->mCursorObject->mImitaterType = SeedType::SEED_NONE;
                mBoard->mCursorObject->mCursorType = CursorType::CURSOR_TYPE_PLANT_FROM_WHEEL_BARROW;
                return true;
            }
        }
    }
    else if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE)
    {
        if (mBoard->CanUseGameObject(GameObjectType::OBJECT_TYPE_WHEELBARROW))
        {
            Rect aButtonRect = mBoard->GetShovelButtonRect();
            mBoard->GetZenButtonRect(GameObjectType::OBJECT_TYPE_WHEELBARROW, aButtonRect);

            Plant* aPlant = mBoard->mPlants.DataArrayTryToGet(mBoard->mCursorObject->mGlovePlantID);
            if (aPlant && aButtonRect.Contains(x, y) && GetPottedPlantInWheelbarrow() == nullptr)
            {
                MouseDownWithEmptyWheelBarrow(aPlant);
                mBoard->ClearCursor();
                return true;
            }
        }
    }
    else if (theHitResult->mObjectType == GameObjectType::OBJECT_TYPE_NONE && mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_NORMAL && 
        mGardenType == GardenType::GARDEN_AQUARIUM && theClickCount <= -1)
    {
        mApp->PlaySample(SOUND_TAPGLASS);
    }

    if (mApp->mCrazyDaveMessageIndex != -1)
    {
        AdvanceCrazyDaveDialog();
        return true;
    }

    return false;
}

//0x521AC0
void ZenGarden::SetPlantAnimSpeed(Plant* thePlant)
{
    Reanimation* aBodyReanim = mApp->ReanimationGet(thePlant->mBodyReanimID);
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    bool aPlantHighOnChocolate = PlantHighOnChocolate(aPottedPlant);
    bool aPlantAtHighRate = aBodyReanim->mAnimRate >= 25.0f;
    if (aPlantAtHighRate == aPlantHighOnChocolate)
    {
        return;
    }

    float aTargetRate;
    if (thePlant->mSeedType == SeedType::SEED_PEASHOOTER || thePlant->mSeedType == SeedType::SEED_SNOWPEA || thePlant->mSeedType == SeedType::SEED_REPEATER || 
        thePlant->mSeedType == SeedType::SEED_LEFTPEATER || thePlant->mSeedType == SeedType::SEED_GATLINGPEA || thePlant->mSeedType == SeedType::SEED_SPLITPEA || 
        thePlant->mSeedType == SeedType::SEED_THREEPEATER || thePlant->mSeedType == SeedType::SEED_MARIGOLD)
    {
        aTargetRate = RandRangeFloat(15.0f, 20.0f);
    }
    else if (thePlant->mSeedType == SeedType::SEED_POTATOMINE)
    {
        aTargetRate = 12.0f;
    }
    else
    {
        aTargetRate = RandRangeFloat(10.0f, 15.0f);
    }

    if (aPlantHighOnChocolate)
    {
        aTargetRate *= 2.0f;
        aTargetRate = max(25.0f, aTargetRate);
    }

    aBodyReanim->mAnimRate = aTargetRate;
    Reanimation* aHeadReanim = mApp->ReanimationTryToGet(thePlant->mHeadReanimID);
    Reanimation* aHeadReanim2 = mApp->ReanimationTryToGet(thePlant->mHeadReanimID2);
    Reanimation* aHeadReanim3 = mApp->ReanimationTryToGet(thePlant->mHeadReanimID3);
    if (aHeadReanim)
    {
        aHeadReanim->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim->mAnimTime = aBodyReanim->mAnimTime;
    }
    if (aHeadReanim2)
    {
        aHeadReanim2->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim2->mAnimTime = aBodyReanim->mAnimTime;
    }
    if (aHeadReanim3)
    {
        aHeadReanim3->mAnimRate = aBodyReanim->mAnimRate;
        aHeadReanim3->mAnimTime = aBodyReanim->mAnimTime;
    }
}

//0x521CC0
int ZenGarden::PlantGetMinutesSinceHappy(Plant* thePlant)
{
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    int aMinutes = (_time64(nullptr) - aPottedPlant->mLastNeedFulfilledTime) / 60;
    if (PlantHighOnChocolate(aPottedPlant))
    {
        aMinutes = 0;
    }
    return aMinutes;
}

//0x521D40
void ZenGarden::PlantUpdateProduction(Plant* thePlant)
{
    thePlant->mLaunchCounter--;
    SetPlantAnimSpeed(thePlant);
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    if (PlantHighOnChocolate(aPottedPlant))
    {
        thePlant->mLaunchCounter--;
    }

    if (thePlant->mLaunchCounter <= 0)
    {
        PlantSetLaunchCounter(thePlant);
        mApp->PlayFoley(FoleyType::FOLEY_SPAWN_SUN);

        int aCoinHit = Rand(1000);
        aCoinHit += TodAnimateCurve(5, 30, PlantGetMinutesSinceHappy(thePlant), 0, 80, TodCurves::CURVE_LINEAR);
        CoinType aCoinType = CoinType::COIN_SILVER;
        if (aCoinHit < 100)
        {
            aCoinType = CoinType::COIN_GOLD;
        }
        mBoard->AddCoin(thePlant->mX, thePlant->mY, aCoinType, CoinMotion::COIN_MOTION_COIN);
    }
}

void ZenGarden::ResetPlantTimers(PottedPlant* thePottedPlant)
{
    thePottedPlant->mLastWateredTime = 0;
    thePottedPlant->mLastNeedFulfilledTime = 0;
    thePottedPlant->mLastFertilizedTime = 0;
    thePottedPlant->mLastChocolateTime = 0;
}

//0x521E70
void ZenGarden::PottedPlantUpdate(Plant* thePlant)
{
    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    __time64_t aNow = _time64(nullptr);
    if (aPottedPlant->mLastWateredTime > aNow || 
        aPottedPlant->mLastNeedFulfilledTime > aNow || 
        aPottedPlant->mLastFertilizedTime > aNow || 
        aPottedPlant->mLastChocolateTime > aNow)
    {
        ResetPlantTimers(aPottedPlant);
    }

    if (thePlant->mIsAsleep)
    {
        return;
    }
    if (thePlant->mStateCountdown > 0)
    {
        thePlant->mStateCountdown--;
    }
    if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_FULL && WasPlantNeedFulfilledToday(aPottedPlant))
    {
        PlantUpdateProduction(thePlant);
    }
    UpdatePlantEffectState(thePlant);
}

//0x521F30
void ZenGarden::DrawPlantOverlay(Graphics* g, Plant* thePlant)
{
    if (thePlant->mPottedPlantIndex == -1)
    {
        return;
    }

    PottedPlant* aPottedPlant = PottedPlantFromIndex(thePlant->mPottedPlantIndex);
    PottedPlantNeed aPlantNeed = mApp->mZenGarden->GetPlantsNeed(aPottedPlant);
    if (aPlantNeed == PottedPlantNeed::PLANTNEED_NONE)
    {
        return;
    }

    g->DrawImage(IMAGE_PLANTSPEECHBUBBLE, 50, 0);
    switch (aPlantNeed)
    {
    case PottedPlantNeed::PLANTNEED_FERTILIZER:
        g->DrawImageCel(IMAGE_ZEN_NEED_ICONS, 61, 7, 0, 0);
        break;

    case PottedPlantNeed::PLANTNEED_BUGSPRAY:
        g->DrawImageCel(IMAGE_ZEN_NEED_ICONS, 61, 7, 1, 0);
        break;

    case PottedPlantNeed::PLANTNEED_PHONOGRAPH:
        g->DrawImageCel(IMAGE_ZEN_NEED_ICONS, 60, 7, 2, 0);
        break;

    case PottedPlantNeed::PLANTNEED_WATER:
        g->DrawImage(IMAGE_WATERDROP, 67, 9);
        break;

    case PottedPlantNeed::PLANTNEED_NONE:
        break;
    }
}

//0x521FE0
void ZenGarden::WakeStinky()
{
    mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_STINKY_THE_SNAIL] = _time64(nullptr);
    mApp->PlaySample(SOUND_TAP);
    mBoard->ClearAdvice(AdviceType::ADVICE_STINKY_SLEEPING);
    gLawnApp->mPlayerInfo->mHasWokenStinky = TRUE;
}

//0x522090
bool ZenGarden::IsStinkyHighOnChocolate()
{
    return _time64(nullptr) - mApp->mPlayerInfo->mLastStinkyChocolateTime < 3600;
}

bool ZenGarden::PlantHighOnChocolate(PottedPlant* thePottedPlant)
{
    return _time64(nullptr) - thePottedPlant->mLastChocolateTime < 300;
}

bool ZenGarden::IsStinkySleeping()
{
    GridItem* aStinky = GetStinky();
    return aStinky && aStinky->mGridItemState == GridItemState::GRIDITEM_STINKY_SLEEPING;
}

//0x5220C0
bool ZenGarden::ShouldStinkyBeAwake()
{
    if (IsStinkyHighOnChocolate())
    {
        return true;
    }
    return _time64(nullptr) - mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_STINKY_THE_SNAIL] < 180;
}

//0x522110
void ZenGarden::OpenStore()
{
    LeaveGarden();
    StoreScreen* aStore = mApp->ShowStoreScreen();
    if (mBoard->mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_VISIT_STORE)
    {
        aStore->SetupForIntro(2600);
        mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_FERTILIZER] = PURCHASE_COUNT_OFFSET + 5;
    }
    aStore->mBackButton->SetLabel(_S("[STORE_BACK_TO_GAME]"));
    aStore->mPage = StorePages::STORE_PAGE_ZEN1;
    aStore->WaitForResult(true);

    if (aStore->mGoToTreeNow)
    {
        mApp->KillBoard();
        mApp->PreNewGame(GameMode::GAMEMODE_TREE_OF_WISDOM, false);
    }
    else
    {
        mApp->mMusic->MakeSureMusicIsPlaying(MusicTune::MUSIC_TUNE_ZEN_GARDEN);
        if (mBoard->mTutorialState == TutorialState::TUTORIAL_ZEN_GARDEN_VISIT_STORE)
        {
            mBoard->DisplayAdvice(_S("[ADVICE_ZEN_GARDEN_FERTILIZE]"), MessageStyle::MESSAGE_STYLE_ZEN_GARDEN_LONG, AdviceType::ADVICE_NONE);
            mBoard->mTutorialState = TutorialState::TUTORIAL_ZEN_GARDEN_FERTILIZE_PLANTS;
        }
        AddStinky();
    }
}

//0x5222D0
// GOTY @Patoke: 0x52CC50
void ZenGarden::SetupForZenTutorial()
{
    mBoard->mMenuButton->SetLabel(_S("[CONTINUE_BUTTON]"));
    mBoard->mStoreButton->mDisabled = true;
    mBoard->mStoreButton->mBtnNoDraw = true;
    mBoard->mMenuButton->mDisabled = true;
    mBoard->mMenuButton->mBtnNoDraw = true;

    mApp->CrazyDaveEnter();
    mApp->CrazyDaveTalkIndex(2100);
}

//0x5223B0
SeedType ZenGarden::PickRandomSeedType()
{
    SeedType aSeedList[40];
    int aSeedCount = 0;
    for (int i = 0; i < 40; i++)
    {
        SeedType aSeedType = (SeedType)i;
        if (aSeedType != SeedType::SEED_MARIGOLD && aSeedType != SeedType::SEED_FLOWERPOT)
        {
            aSeedList[aSeedCount] = aSeedType;
            aSeedCount++;
        }
    }
    return (SeedType)TodPickFromArray((intptr_t*)aSeedList, aSeedCount);
}

//0x5223F0
void ZenGarden::LeaveGarden()
{
    {
        GridItem* aGridItem = nullptr;
        while (mBoard->IterateGridItems(aGridItem))
        {
            if (aGridItem->mGridItemType == GridItemType::GRIDITEM_ZEN_TOOL)
            {
                DoFeedingTool(aGridItem->mPosX, aGridItem->mPosY, aGridItem->mGridItemState);
                aGridItem->GridItemDie();
            }
            else if (aGridItem->mGridItemType == GridItemType::GRIDITEM_STINKY)
            {
                mApp->mPlayerInfo->mStinkyPosX = aGridItem->mPosX;
                mApp->mPlayerInfo->mStinkyPosY = aGridItem->mPosY;
                aGridItem->GridItemDie();
            }
        }
    }
    {
        Coin* aCoin = nullptr;
        while (mBoard->IterateCoins(aCoin))
        {
            if (aCoin->mIsBeingCollected)
            {
                aCoin->ScoreCoin();
            }
            else
            {
                aCoin->Die();
            }
        }
    }
}
