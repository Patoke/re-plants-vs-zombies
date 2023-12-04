#include "Board.h"
#include "Cutscene.h"
#include "ZenGarden.h"
#include "../LawnApp.h"
#include "CursorObject.h"
#include "../Resources.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "widget/WidgetManager.h"

//0x438640
CursorObject::CursorObject()
{
    mType = SeedType::SEED_NONE;
    mImitaterType = SeedType::SEED_NONE;
    mSeedBankIndex = -1;
    mX = 0;
    mY = 0;
    mCursorType = CursorType::CURSOR_TYPE_NORMAL;
    mCoinID = CoinID::COINID_NULL;
    mDuplicatorPlantID = PlantID::PLANTID_NULL;
    mCobCannonPlantID = PlantID::PLANTID_NULL;
    mGlovePlantID = PlantID::PLANTID_NULL;
    mReanimCursorID = ReanimationID::REANIMATIONID_NULL;
    
    if (mApp->IsWhackAZombieLevel())
    {
        ReanimatorEnsureDefinitionLoaded(ReanimationType::REANIM_HAMMER, true);
        Reanimation* aHammerReanim = mApp->AddReanimation(-25.0f, 16.0f, 0, ReanimationType::REANIM_HAMMER);
        aHammerReanim->mIsAttachment = true;
        aHammerReanim->PlayReanim("anim_whack_zombie", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 24.0f);
        aHammerReanim->mAnimTime = 1.0f;
        mReanimCursorID = mApp->ReanimationGetID(aHammerReanim);
    }

    mWidth = 80;
    mHeight = 80;
}

//0x438780
void CursorObject::Update()
{
    if (mApp->mGameScene != GameScenes::SCENE_PLAYING && !mBoard->mCutScene->IsInShovelTutorial())
    {
        mVisible = false;
        return;
    }

    if (!mApp->mWidgetManager->mMouseIn)
    {
        mVisible = false;
        return;
    }

    Reanimation* aCursorReanim = mApp->ReanimationTryToGet(mReanimCursorID);
    if (aCursorReanim)
    {
        aCursorReanim->Update();
    }

    mVisible = true;
    mX = mApp->mWidgetManager->mLastMouseX - 25;
    mY = mApp->mWidgetManager->mLastMouseY - 35;
}

void CursorObject::Die()
{
    mApp->RemoveReanimation(mReanimCursorID);
}

//0x438820
void CursorObject::Draw(Graphics* g)
{
    switch (mCursorType)
    {
    case CursorType::CURSOR_TYPE_SHOVEL:
        g->DrawImage(IMAGE_SHOVEL, 10, -30);
        break;

    case CursorType::CURSOR_TYPE_WATERING_CAN:
        if (mApp->mPlayerInfo->mPurchases[(int)StoreItem::STORE_ITEM_GOLD_WATERINGCAN])
        {
            g->DrawImage(IMAGE_ZEN_GOLDTOOLRETICLE, -62, -37);
            g->DrawImage(IMAGE_WATERINGCANGOLD, -3, 12);
        }
        else
        {
            g->DrawImage(IMAGE_WATERINGCAN, -3, 12);
        }
        break;

    case CursorType::CURSOR_TYPE_FERTILIZER:
        g->DrawImage(IMAGE_FERTILIZER, -15, 0);
        break;

    case CursorType::CURSOR_TYPE_BUG_SPRAY:
        g->DrawImage(IMAGE_BUG_SPRAY, -9, -1);
        break;

    case CursorType::CURSOR_TYPE_PHONOGRAPH:
        g->DrawImage(IMAGE_PHONOGRAPH, -17, 10);
        break;

    case CursorType::CURSOR_TYPE_CHOCOLATE:
        g->DrawImage(IMAGE_CHOCOLATE, -2, -8);
        break;

    case CursorType::CURSOR_TYPE_GLOVE:
        g->DrawImage(IMAGE_ZEN_GARDENGLOVE, -17, 15);
        break;

    case CursorType::CURSOR_TYPE_MONEY_SIGN:
        g->DrawImage(IMAGE_ZEN_MONEYSIGN, -17, -10);
        break;

    case CursorType::CURSOR_TYPE_TREE_FOOD:
        g->DrawImage(IMAGE_TREEFOOD, -15, 0);
        break;

    case CursorType::CURSOR_TYPE_WHEEELBARROW:
    {
        PottedPlant* aPottedPlant = mApp->mZenGarden->GetPottedPlantInWheelbarrow();
        if (aPottedPlant)
        {
            g->DrawImage(IMAGE_ZEN_WHEELBARROW, -20, -23);
            if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_SMALL)
            {
                mApp->mZenGarden->DrawPottedPlant(g, 10.0f, -35.0f, aPottedPlant, 0.6f, true);
            }
            else if (aPottedPlant->mPlantAge == PottedPlantAge::PLANTAGE_MEDIUM)
            {
                mApp->mZenGarden->DrawPottedPlant(g, 15.0f, -25.0f, aPottedPlant, 0.5f, true);
            }
            else
            {
                mApp->mZenGarden->DrawPottedPlant(g, 21.0f, -15.0f, aPottedPlant, 0.4f, true);
            }
        }
        else
        {
            g->DrawImage(IMAGE_ZEN_WHEELBARROW, -20, -30);
        }

        break;
    }

    case CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE:
    {
        Plant* aPlant = mBoard->mPlants.DataArrayGet((unsigned int)mGlovePlantID);
        PottedPlant* aPottedPlant = &mApp->mPlayerInfo->mPottedPlant[aPlant->mPottedPlantIndex];
        if (mBoard->mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN || mBoard->mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
        {
            mApp->mZenGarden->DrawPottedPlant(g, -10.0f, -10.0f, aPottedPlant, 1.0f, false);
        }
        else
        {
            mApp->mZenGarden->DrawPottedPlant(g, -22.0f, -38.0f, aPottedPlant, 1.0f, true);
        }

        break;
    }

    case CursorType::CURSOR_TYPE_PLANT_FROM_WHEEL_BARROW:
    {
        PottedPlant* aPottedPlant = mApp->mZenGarden->GetPottedPlantInWheelbarrow();
        if (mBoard->mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN || mBoard->mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
        {
            mApp->mZenGarden->DrawPottedPlant(g, -10.0f, -10.0f, aPottedPlant, 1.0f, false);
        }
        else
        {
            mApp->mZenGarden->DrawPottedPlant(g, -22.0f, -38.0f, aPottedPlant, 1.0f, true);
        }

        break;
    }

    case CursorType::CURSOR_TYPE_PLANT_FROM_BANK:
    case CursorType::CURSOR_TYPE_PLANT_FROM_USABLE_COIN:
    case CursorType::CURSOR_TYPE_PLANT_FROM_DUPLICATOR:
    {
        float aOffsetX = -10.0f;
        float aOffsetY = PlantDrawHeightOffset(mBoard, nullptr, mType, -1, -1) - 10.0f;
        if (Plant::IsFlying(mType) || mType == SeedType::SEED_GRAVEBUSTER)
        {
            aOffsetY += 30.0f;
        }
        aOffsetY -= 15.0f;
        if (mApp->IsIZombieLevel())
        {
            aOffsetX -= 55.0f;
            aOffsetY -= 70.0f;
        }

        Plant::DrawSeedType(g, mType, mImitaterType, DrawVariation::VARIATION_NORMAL, aOffsetX, aOffsetY);
        break;
    }
    
    case CursorType::CURSOR_TYPE_HAMMER:
        mApp->ReanimationGet(mReanimCursorID)->Draw(g);
        break;

    case CursorType::CURSOR_TYPE_COBCANNON_TARGET:
    {
        HitResult aHitResult;
        mBoard->MouseHitTest(mBoard->mPrevMouseX, mBoard->mPrevMouseY, &aHitResult);
        if (aHitResult.mObjectType == GameObjectType::OBJECT_TYPE_NONE && mBoard->mPrevMouseY >= 80)
        {
            g->DrawImageCel(IMAGE_COBCANNON_TARGET, -11, 7, 0);
        }

        break;
    }
    case CursorType::CURSOR_TYPE_NORMAL:
        break;
    }
}

//0x438D50
CursorPreview::CursorPreview()
{
    mX = 0;
    mY = 0;
    mGridX = 0;
    mGridY = 0;
    mVisible = false;
    mWidth = 80;
    mHeight = 80;
}

//0x438DA0
void CursorPreview::Update()
{
    if (mApp->mGameScene != GameScenes::SCENE_PLAYING && !mBoard->mCutScene->IsInShovelTutorial())
    {
        mVisible = false;
        return;
    }

    SeedType aSeedType = mBoard->GetSeedTypeInCursor();
    int aMouseX = mApp->mWidgetManager->mLastMouseX;
    int aMouseY = mApp->mWidgetManager->mLastMouseY;
    mGridX = mBoard->PlantingPixelToGridX(aMouseX, aMouseY, aSeedType);
    mGridY = mBoard->PlantingPixelToGridY(aMouseX, aMouseY, aSeedType);
    if (mGridX >= 0 && mGridX < MAX_GRID_SIZE_X && mGridY >= 0 && mGridY <= MAX_GRID_SIZE_Y)
    {
        bool aShow = false;
        if (mBoard->IsPlantInCursor() && mBoard->CanPlantAt(mGridX, mGridY, aSeedType) == PlantingReason::PLANTING_OK)
        {
            aShow = true;
        }
        else if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_WHEEELBARROW)
        {
            if (mApp->mZenGarden->GetPottedPlantInWheelbarrow() && mBoard->CanPlantAt(mGridX, mGridY, aSeedType) == PlantingReason::PLANTING_OK)
            {
                aShow = true;
            }
        }

        if (aShow)
        {
            mX = mBoard->GridToPixelX(mGridX, mGridY);
            mY = mBoard->GridToPixelY(mGridX, mGridY);
            mVisible = true;
            return;
        }
    }

    mVisible = false;
}

//0x438EB0
void CursorPreview::Draw(Graphics* g)
{
    SeedType aSeedType = mBoard->GetSeedTypeInCursor();
    if (aSeedType == SeedType::SEED_NONE)
        return;

    g->SetColorizeImages(true);
    g->SetColor(Color(255, 255, 255, 100));

    PottedPlant* aPottedPlant = nullptr;
    if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_WHEEELBARROW || mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_WHEEL_BARROW)
    {
        aPottedPlant = mApp->mZenGarden->GetPottedPlantInWheelbarrow();
    }
    else if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_PLANT_FROM_GLOVE)
    {
        aPottedPlant = &mApp->mPlayerInfo->mPottedPlant[mBoard->mPlants.DataArrayGet((unsigned int)mBoard->mCursorObject->mGlovePlantID)->mPottedPlantIndex];
    }

    if (aPottedPlant)
    {
        bool aDrawPot = true;
        if (mBoard->mBackground == BackgroundType::BACKGROUND_MUSHROOM_GARDEN || mBoard->mBackground == BackgroundType::BACKGROUND_ZOMBIQUARIUM)
        {
            aDrawPot = false;
        }
        mApp->mZenGarden->DrawPottedPlant(g, 0.0f, 0.0f, aPottedPlant, 1.0f, aDrawPot);
    }
    else
    {
        float aOffsetX, aOffsetY;
        if (mApp->IsIZombieLevel())
        {
            float aHeight = PlantDrawHeightOffset(mBoard, nullptr, aSeedType, mGridX, mGridY);
            if (aSeedType == SeedType::SEED_ZOMBIE_GARGANTUAR)
            {
                aHeight -= 30.0f;
            }

            aOffsetY = aHeight - 78.0f;
            aOffsetX = -49.0f;
        }
        else
        {
            aOffsetY = PlantDrawHeightOffset(mBoard, nullptr, aSeedType, mGridX, mGridY);
            aOffsetX = 0.0f;
        }

        Plant::DrawSeedType(g, mBoard->mCursorObject->mType, mBoard->mCursorObject->mImitaterType, DrawVariation::VARIATION_NORMAL, aOffsetX, aOffsetY);
    }

    if (mApp->mGameMode == GameMode::GAMEMODE_CHALLENGE_COLUMN)
    {
        for (int y = 0; y < MAX_GRID_SIZE_Y; y++)
        {
            if (y != mGridY && mBoard->CanPlantAt(mGridX, y, aSeedType) == PlantingReason::PLANTING_OK)
            {
                float aOffsetY = 85.0f * (y - mGridY) + PlantDrawHeightOffset(mBoard, nullptr, aSeedType, mGridX, y);
                Plant::DrawSeedType(g, mBoard->mCursorObject->mType, mBoard->mCursorObject->mImitaterType, DrawVariation::VARIATION_NORMAL, 0.0f, aOffsetY);
            }
        }
    }

    g->SetColorizeImages(false);
}
