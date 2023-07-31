#include "Board.h"
#include "GridItem.h"
#include "Challenge.h"
#include "ZenGarden.h"
#include "../LawnApp.h"
#include "SeedPacket.h"
#include "CursorObject.h"
#include "../Resources.h"
#include "MessageWidget.h"
#include "System/ReanimationLawn.h"
#include "../Sexy.TodLib/TodFoley.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/TodParticle.h"
#include "../SexyAppFramework/WidgetManager.h"

using namespace Sexy;

//0x44CFA0
GridItem::GridItem()
{
    mApp = (LawnApp*)gSexyAppBase;
    mPosX = 0.0f;
    mPosY = 0.0f;
    mBoard = mApp->mBoard;
    mGoalX = 0.0f;
    mGoalY = 0.0f;
    mGridItemType = GridItemType::GRIDITEM_NONE;
    mGridX = 0;
    mGridY = 0;
    mGridItemCounter = 0;
    mRenderOrder = 0;
    mDead = false;
    mGridItemReanimID = ReanimationID::REANIMATIONID_NULL;
    mGridItemParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
    mZombieType = ZombieType::ZOMBIE_INVALID;
    mSeedType = SeedType::SEED_NONE;
    mScaryPotType = ScaryPotType::SCARYPOT_NONE;
    mHighlighted = false;
    mTransparentCounter = 0;
    mSunCount = 0;
    mMotionTrailCount = 0;
}

//0x44D000
void GridItem::GridItemDie()
{
    mDead = true;
    
    Reanimation* aGridItemReanim = mApp->ReanimationTryToGet(mGridItemReanimID);
    if (aGridItemReanim)
    {
        aGridItemReanim->ReanimationDie();
        mGridItemReanimID = ReanimationID::REANIMATIONID_NULL;
    }

    TodParticleSystem* aGridItemParticle = mApp->ParticleTryToGet(mGridItemParticleID);
    if (aGridItemParticle)
    {
        aGridItemParticle->ParticleSystemDie();
    }
}

//0x44D070
void GridItem::DrawGridItemOverlay(Graphics* g)
{
    if (mGridItemType == GridItemType::GRIDITEM_STINKY)
    {
        if (mBoard->mCursorObject->mCursorType == CursorType::CURSOR_TYPE_CHOCOLATE && !mApp->mZenGarden->IsStinkyHighOnChocolate())
        {
            g->DrawImage(IMAGE_PLANTSPEECHBUBBLE, mPosX + 50.0f, mPosY - 36.0f);
            TodDrawImageScaledF(g, IMAGE_CHOCOLATE, mPosX + 63.0f, mPosY - 28.0f, 0.44f, 0.44f);
        }
    }
}

//0x44D140
void GridItem::DrawGridItem(Graphics* g)
{
    switch (mGridItemType)
    {
    case GridItemType::GRIDITEM_GRAVESTONE:         DrawGraveStone(g);                              break;
    case GridItemType::GRIDITEM_CRATER:             DrawCrater(g);                                  break;
    case GridItemType::GRIDITEM_LADDER:             DrawLadder(g);                                  break;
    case GridItemType::GRIDITEM_PORTAL_CIRCLE:                                                      break;
    case GridItemType::GRIDITEM_PORTAL_SQUARE:                                                      break;
    case GridItemType::GRIDITEM_ZEN_TOOL:                                                           break;
    case GridItemType::GRIDITEM_RAKE:                                                               break;
    case GridItemType::GRIDITEM_BRAIN:              g->DrawImageF(IMAGE_BRAIN, mPosX, mPosY);       break;
    case GridItemType::GRIDITEM_SCARY_POT:          DrawScaryPot(g);                                break;
    case GridItemType::GRIDITEM_SQUIRREL:           DrawSquirrel(g);                                break;
    case GridItemType::GRIDITEM_STINKY:             DrawStinky(g);                                  break;
    case GridItemType::GRIDITEM_IZOMBIE_BRAIN:      DrawIZombieBrain(g);                            break;
    default:                                        TOD_ASSERT();                                   break;
    }

    Reanimation* aGridItemReanim = mApp->ReanimationTryToGet(mGridItemReanimID);
    if (aGridItemReanim)
    {
        aGridItemReanim->Draw(g);
    }

    TodParticleSystem* aGridItemParticle = mApp->ParticleTryToGet(mGridItemParticleID);
    if (aGridItemParticle)
    {
        aGridItemParticle->Draw(g);
    }
}

//0x44D250
void GridItem::DrawIZombieBrain(Graphics* g)
{
    if (mGridItemState == GridItemState::GRIDITEM_STATE_BRAIN_SQUISHED)
    {
        TodDrawImageScaledF(g, IMAGE_BRAIN, mPosX, mPosY + 20.0f, 1.0f, 0.25f);
        return;
    }

    if (mBoard->mAdvice->mDuration > 0 && mBoard->mHelpIndex == AdviceType::ADVICE_I_ZOMBIE_EAT_ALL_BRAINS)
    {
        Color aFlashingColor = GetFlashingColor(mBoard->mMainCounter, 75);
        g->SetColorizeImages(true);
        g->SetColor(aFlashingColor);
    }

    g->DrawImageF(IMAGE_BRAIN, mPosX, mPosY);
    if (mTransparentCounter > 0)
    {
        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        g->SetColorizeImages(true);
        g->SetColor(Color(255, 255, 255, ClampInt(mTransparentCounter * 3, 0, 255)));
        g->DrawImageF(IMAGE_BRAIN, mPosX, mPosY);
        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
        g->SetColorizeImages(false);
    }

    g->SetColorizeImages(false);
}

//0x44D3A0
void GridItem::DrawGraveStone(Graphics* g)
{
    if (mGridItemCounter <= 0)
        return;

    int aHeightPosition = TodAnimateCurve(0, 100, mGridItemCounter, 1000, 0, TodCurves::CURVE_EASE_IN_OUT);
    int aGridCelLook = mBoard->mGridCelLook[mGridX][mGridY];
    int aGridCelOffsetX = mBoard->mGridCelOffset[mGridX][mGridY][0];
    int aGridCelOffsetY = mBoard->mGridCelOffset[mGridX][mGridY][1];
    int aCelWidth = IMAGE_TOMBSTONES->GetCelWidth();
    int aCelHeight = IMAGE_TOMBSTONES->GetCelHeight();
    int aGraveCol = aGridCelLook % 5;
    int aGraveRow;
    if (mGridY == 0)
    {
        aGraveRow = 1;
    }
    else if (mGridItemState == GridItemState::GRIDITEM_STATE_GRAVESTONE_SPECIAL)
    {
        aGraveRow = 0;
    }
    else
    {
        aGraveRow = 2 + aGridCelLook % 2;
    }

    int aVisibleHeight = TodAnimateCurve(0, 1000, aHeightPosition, aCelHeight, 0, TodCurves::CURVE_EASE_IN_OUT);
    int aExtraBottomClip = TodAnimateCurve(0, 50, aHeightPosition, 0, 14, TodCurves::CURVE_EASE_IN_OUT);
    int aVisibleHeightDirt = TodAnimateCurve(500, 1000, aHeightPosition, aCelHeight, 0, TodCurves::CURVE_EASE_IN_OUT);
    int aExtraTopClip = 0;
    Plant* aPlant = mBoard->GetTopPlantAt(mGridX, mGridY, PlantPriority::TOPPLANT_ONLY_NORMAL_POSITION);
    if (aPlant && aPlant->mState == PlantState::STATE_GRAVEBUSTER_EATING)
    {
        aExtraTopClip = TodAnimateCurveFloat(400, 0, aPlant->mStateCountdown, 10.0f, 40.0f, TodCurves::CURVE_LINEAR);
    }

    Rect aSrcRect(aCelWidth * aGraveCol, aCelHeight * aGraveRow + aExtraTopClip, aCelWidth, aVisibleHeight - aExtraBottomClip - aExtraTopClip);
    Rect aSrcRectDirt(aCelWidth * aGraveCol, aCelHeight * aGraveRow, aCelWidth, aVisibleHeightDirt);
    int x = mBoard->GridToPixelX(mGridX, mGridY) + aGridCelOffsetX - 4;
    int y = mBoard->GridToPixelY(mGridX, mGridY) + aCelHeight + aGridCelOffsetY - 9;
    g->DrawImage(IMAGE_TOMBSTONES, x, y - aVisibleHeight + aExtraTopClip, aSrcRect);
    g->DrawImage(IMAGE_TOMBSTONE_MOUNDS, x, y - aVisibleHeightDirt, aSrcRectDirt);
}

//0x44D690
void GridItem::DrawStinky(Graphics* g)
{
    Reanimation* aStinkyReanim = mApp->ReanimationGet(mGridItemReanimID);
    float aOriginalTime = aStinkyReanim->mAnimTime;

    TOD_ASSERT(mMotionTrailCount < NUM_MOTION_TRAIL_FRAMES);
    for (int i = mMotionTrailCount - 1; i >= 0; i--)
    {
        if (i % 2)
        {
            MotionTrailFrame& aFrame = mMotionTrailFrames[i];
            float aDiffX = aFrame.mPosX - mPosX;
            float aDiffY = aFrame.mPosY - mPosY;

            int anAlpha = TodAnimateCurve(0, 11, i, 64, 16, TodCurves::CURVE_LINEAR);
            g->SetColor(Color(255, 255, 255, anAlpha));
            g->SetColorizeImages(true);
            aStinkyReanim->mAnimTime = aFrame.mAnimTime;

            g->mTransX += aDiffX;
            g->mTransY += aDiffY;
            aStinkyReanim->Draw(g);

            g->SetColorizeImages(false);
            g->mTransX -= aDiffX;
            g->mTransY -= aDiffY;
        }
    }
    aStinkyReanim->mAnimTime = aOriginalTime;

    if (mGridItemType == GridItemType::GRIDITEM_STINKY && mHighlighted)
    {
        aStinkyReanim->mEnableExtraAdditiveDraw = true;
        aStinkyReanim->mExtraAdditiveColor = Color(255, 255, 255, 196);
    }
    aStinkyReanim->Draw(g);
    aStinkyReanim->mEnableExtraAdditiveDraw = false;
}

//0x44D860
void GridItem::DrawCrater(Graphics* g)
{
    float aXPos = mBoard->GridToPixelX(mGridX, mGridY) - 8.0f;
    float aYPos = mBoard->GridToPixelY(mGridX, mGridY) + 40.0f;
    if (mGridItemCounter < 25)
    {
        int anAlpha = TodAnimateCurve(25, 0, mGridItemCounter, 255, 0, TodCurves::CURVE_LINEAR);
        g->SetColor(Color(255, 255, 255, anAlpha));
        g->SetColorizeImages(true);
    }

    bool fading = mGridItemCounter < 9000;
    Image* aImage = IMAGE_CRATER;
    int aCelCol = 0;

    if (mBoard->IsPoolSquare(mGridX, mGridY))
    {
        if (mBoard->StageIsNight())
        {
            aImage = IMAGE_CRATER_WATER_NIGHT;
        }
        else
        {
            aImage = IMAGE_CRATER_WATER_DAY;
        }

        if (fading)
        {
            aCelCol = 1;
        }

        float aPos = mGridY * PI + mGridX * PI * 0.25f;
        float aTime = mBoard->mMainCounter * PI * 2.0f / 200.0f;
        aYPos += sin(aPos + aTime) * 2.0f;
    }
    else if (mBoard->StageHasRoof())
    {
        if (mGridX < 5)
        {
            aImage = IMAGE_CRATER_ROOF_LEFT;
            aXPos += 16.0f;
            aYPos += -16.0f;
        }
        else
        {
            aImage = IMAGE_CRATER_ROOF_CENTER;
            aXPos += 18.0f;
            aYPos += -9.0f;
        }

        if (fading)
        {
            aCelCol = 1;
        }
    }
    else if (mBoard->StageIsNight())
    {
        aCelCol = 1;
        if (fading)
        {
            aImage = IMAGE_CRATER_FADING;
        }
    }
    else if (fading)
    {
        aImage = IMAGE_CRATER_FADING;
    }

    TodDrawImageCelF(g, aImage, aXPos, aYPos, aCelCol, 0);
    g->SetColorizeImages(false);
}

//0x44DB00
void GridItem::DrawScaryPot(Graphics* g)
{
    int aImageCol = (int)mGridItemState - (int)GridItemState::GRIDITEM_STATE_SCARY_POT_QUESTION;
    TOD_ASSERT(aImageCol >= 0 && aImageCol < 3);

    int aXPos = mBoard->GridToPixelX(mGridX, mGridY) - 5;
    int aYPos = mBoard->GridToPixelY(mGridX, mGridY) - 15;
    TodDrawImageCelCenterScaledF(g, IMAGE_PLANTSHADOW2, aXPos - 5.0f, aYPos + 72.0f, 0, 1.3f, 1.3f);

    if (mTransparentCounter > 0)
    {
        g->DrawImageCel(IMAGE_SCARY_POT, aXPos, aYPos, aImageCol, 0);

        Graphics aInsideGraphics(*g);
        if (mScaryPotType == ScaryPotType::SCARYPOT_SEED)
        {
            aInsideGraphics.mScaleX = 0.7f;
            aInsideGraphics.mScaleY = 0.7f;
            DrawSeedPacket(&aInsideGraphics, aXPos + 23.0f, aYPos + 33.0f, mSeedType, SeedType::SEED_NONE, 0.0f, 255, false, false);
        }
        else if (mScaryPotType == ScaryPotType::SCARYPOT_ZOMBIE)
        {
            aInsideGraphics.mScaleX = 0.4f;
            aInsideGraphics.mScaleY = 0.4f;
            float aOffsetX = 6.0f;
            float aOffsetY = 19.0f;
            if (mZombieType == ZombieType::ZOMBIE_FOOTBALL)
            {
                aInsideGraphics.mScaleX = 0.4f;
                aInsideGraphics.mScaleY = 0.4f;
                aOffsetX = 1.0f;
                aOffsetY = 16.0f;
            }
            if (mZombieType == ZombieType::ZOMBIE_GARGANTUAR)
            {
                aInsideGraphics.mScaleX = 0.3f;
                aInsideGraphics.mScaleY = 0.3f;
                aOffsetX += 9.0f;
                aOffsetY += 7.0f;
            }
            mApp->mReanimatorCache->DrawCachedZombie(&aInsideGraphics, aXPos + aOffsetX, aYPos + aOffsetY, mZombieType);
        }
        else if (mScaryPotType == ScaryPotType::SCARYPOT_SUN)
        {
            int aSuns = mBoard->mChallenge->ScaryPotterCountSunInPot(this);

            Reanimation aReanim;
            aReanim.ReanimationInitializeType(0.0f, 0.0f, ReanimationType::REANIM_SUN);
            aReanim.OverrideScale(0.5f, 0.5f);

            for (int i = 0; i < aSuns; i++)
            {
                float aOffsetX = 42.0f;
                float aOffsetY = 62.0f;
                switch (i)
                {
                case 1:     aOffsetX += 3.0f;       aOffsetY -= 20.0f;          break;
                case 2:     aOffsetX -= 6.0f;       aOffsetY -= 10.0f;          break;
                case 3:     aOffsetX += 6.0f;       aOffsetY -= 5.0f;           break;
                case 4:     aOffsetX += 5.0f;       aOffsetY -= 15.0f;          break;
                }

                aReanim.SetPosition(aXPos + aOffsetX, aYPos + aOffsetY);
                aReanim.Draw(g);
            }
        }

        int aAlpha = TodAnimateCurve(0, 50, mTransparentCounter, 255, 58, TodCurves::CURVE_LINEAR);
        g->SetColorizeImages(true);
        g->SetColor(Color(255, 255, 255, aAlpha));
    }

    g->DrawImageCel(IMAGE_SCARY_POT, aXPos, aYPos, aImageCol, 1);
    if (mHighlighted)
    {
        g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
        g->SetColorizeImages(true);
        if (mTransparentCounter == 0)
        {
            g->SetColor(Color(255, 255, 255, 196));
        }
        g->DrawImageCel(IMAGE_SCARY_POT, aXPos, aYPos, aImageCol, 1);
        g->SetDrawMode(Graphics::DRAWMODE_NORMAL);
    }

    g->SetColorizeImages(false);
}

//0x44DFD0
void GridItem::DrawLadder(Graphics* g)
{
    int aXPos = mBoard->GridToPixelX(mGridX, mGridY);
    int aYPos = mBoard->GridToPixelY(mGridX, mGridY);
    TodDrawImageScaledF(g, IMAGE_REANIM_ZOMBIE_LADDER_5, aXPos + 25.0f, aYPos - 4.0f, 0.8f, 0.8f);
}

void GridItem::DrawSquirrel(Graphics* g)
{
    int aXPos = mBoard->GridToPixelX(mGridX, mGridY);
    int aYPos = mBoard->GridToPixelY(mGridX, mGridY);
    if (mGridItemState == GridItemState::GRIDITEM_STATE_SQUIRREL_PEEKING)
    {
        aYPos += TodAnimateCurve(50, 0, mGridItemCounter, 0, -40, TodCurves::CURVE_BOUNCE_SLOW_MIDDLE);
    }
    else if (mGridItemState == GridItemState::GRIDITEM_STATE_SQUIRREL_RUNNING_UP)
    {
        aYPos += TodAnimateCurve(50, 0, mGridItemCounter, 100, 0, TodCurves::CURVE_EASE_IN);
    }
    else if (mGridItemState == GridItemState::GRIDITEM_STATE_SQUIRREL_RUNNING_DOWN)
    {
        aYPos += TodAnimateCurve(50, 0, mGridItemCounter, -100, 0, TodCurves::CURVE_EASE_IN);
    }
    else if (mGridItemState == GridItemState::GRIDITEM_STATE_SQUIRREL_RUNNING_LEFT)
    {
        aXPos += TodAnimateCurve(50, 0, mGridItemCounter, 80, 0, TodCurves::CURVE_EASE_IN);
    }
    else if (mGridItemState == GridItemState::GRIDITEM_STATE_SQUIRREL_RUNNING_RIGHT)
    {
        aXPos += TodAnimateCurve(50, 0, mGridItemCounter, -80, 0, TodCurves::CURVE_EASE_IN);
    }

    g->DrawImage(IMAGE_SQUIRREL, aXPos, aYPos);
}

//0x44E090
void GridItem::AddGraveStoneParticles()
{
    int aXOffset = mBoard->mGridCelOffset[mGridX][mGridY][0];
    int aYOffset = mBoard->mGridCelOffset[mGridX][mGridY][1];
    int aXPos = mBoard->GridToPixelX(mGridX, mGridY) + 14 + aXOffset;
    int aYPos = mBoard->GridToPixelY(mGridX, mGridY) + 78 + aYOffset;
    mApp->AddTodParticle(aXPos, aYPos, mRenderOrder + 1, ParticleEffect::PARTICLE_GRAVE_STONE_RISE);
    mApp->PlayFoley(FoleyType::FOLEY_DIRT_RISE);
}

//0x44E1B0
void GridItem::OpenPortal()
{
    float aXPos = mGridX * 80.0f - 6.0f;
    float aYPos = mBoard->GridToPixelY(0, mGridY) - 65.0f;
    Reanimation* aPortalReanim = mApp->ReanimationTryToGet(mGridItemReanimID);
    if (aPortalReanim == nullptr)
    {
        ReanimationType aReanimType = ReanimationType::REANIM_PORTAL_CIRCLE;
        if (mGridItemType == GridItemType::GRIDITEM_PORTAL_SQUARE)
        {
            aYPos += 25.0f;
            aXPos -= 4.0f;
            aReanimType = ReanimationType::REANIM_PORTAL_SQUARE;
        }
        aPortalReanim = mApp->AddReanimation(aXPos, aYPos, 0, aReanimType);
        aPortalReanim->mIsAttachment = true;
        mGridItemReanimID = mApp->ReanimationGetID(aPortalReanim);
    }
    else
    {
        aPortalReanim->SetPosition(aXPos, aYPos);
    }

    TodParticleSystem* aPortalParticle = mApp->ParticleTryToGet(mGridItemParticleID);
    if (aPortalParticle)
    {
        aPortalParticle->ParticleSystemDie();
        mGridItemParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
    }

    aPortalReanim->PlayReanim("anim_appear", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 12.0f);
    mApp->PlayFoley(FoleyType::FOLEY_PORTAL);
}

//0x44E360
void GridItem::ClosePortal()
{
    Reanimation* aPortalReanim = mApp->ReanimationTryToGet(mGridItemReanimID);
    if (aPortalReanim)
    {
        aPortalReanim->PlayReanim("anim_disappear", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 12.0f);
    }

    TodParticleSystem* aPortalParticle = mApp->ParticleTryToGet(mGridItemParticleID);
    if (aPortalParticle)
    {
        aPortalParticle->ParticleSystemDie();
        mGridItemParticleID = ParticleSystemID::PARTICLESYSTEMID_NULL;
    }

    mGridItemState = GridItemState::GRIDITEM_STATE_PORTAL_CLOSED;
}

bool GridItem::IsOpenPortal()
{
	return mGridItemState != GridItemState::GRIDITEM_STATE_PORTAL_CLOSED && 
		(mGridItemType == GridItemType::GRIDITEM_PORTAL_CIRCLE || mGridItemType == GridItemType::GRIDITEM_PORTAL_SQUARE);
}

//0x44E400
void GridItem::UpdatePortal()
{
    Reanimation* aPortalReanim = mApp->ReanimationGet(mGridItemReanimID);
    if (mGridItemState == GridItemState::GRIDITEM_STATE_PORTAL_CLOSED)
    {
        if (aPortalReanim->mLoopCount > 0)
        {
            GridItemDie();
        }
    }
    else if (aPortalReanim->mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD && aPortalReanim->mLoopCount > 0)
    {
        aPortalReanim->PlayReanim("anim_pulse", ReanimLoopType::REANIM_LOOP, 0, 12.0f);

        ParticleEffect aEffect = ParticleEffect::PARTICLE_PORTAL_CIRCLE;
        float aXPos = mGridX * 80.0f + 13.0f;
        float aYPos = mBoard->GridToPixelY(0, mGridY) - 39.0f;
        if (mGridItemType == GridItemType::GRIDITEM_PORTAL_SQUARE)
        {
            aEffect = ParticleEffect::PARTICLE_PORTAL_SQUARE;
            aXPos -= 8.0f;
            aYPos += 15.0f;
        }

        TodParticleSystem* aParticle = mApp->AddTodParticle(aXPos, aYPos, 0, aEffect);
        mGridItemParticleID = mApp->ParticleGetID(aParticle);
    }
}

//0x44E520
void GridItem::UpdateScaryPot()
{
    if (mApp->mTodCheatKeys && mApp->mWidgetManager->mKeyDown[(int)KeyCode::KEYCODE_SHIFT])
    {
        if (mTransparentCounter < 50)
        {
            mTransparentCounter++;
        }
        return;
    }

    Plant* aPlant = nullptr;
    while (mBoard->IteratePlants(aPlant))
    {
        if (aPlant->mSeedType == SeedType::SEED_PLANTERN && !aPlant->NotOnGround())
        {
            int aDiffX = abs(aPlant->mPlantCol - mGridX);
            int aDiffY = abs(aPlant->mRow - mGridY);
            if (max(aDiffX, aDiffY) <= 1)
            {
                if (mTransparentCounter < 50)
                {
                    mTransparentCounter++;
                }
                return;
            }
        }
    }

    if (mTransparentCounter > 0)
    {
        mTransparentCounter--;
    }
}

void GridItem::UpdateBrain()
{
    if (mGridItemState == GridItemState::GRIDITEM_STATE_BRAIN_SQUISHED)
    {
        mGridItemCounter--;
        if (mGridItemCounter <= 0)
        {
            GridItemDie();
        }
    }

    if (mTransparentCounter > 0)
    {
        mTransparentCounter--;
    }
}

//0x44E5E0
void GridItem::Update()
{
    Reanimation* aGridItemReanim = mApp->ReanimationTryToGet(mGridItemReanimID);
    if (aGridItemReanim)
    {
        aGridItemReanim->Update();
    }

    TodParticleSystem* aGridItemParticle = mApp->ParticleTryToGet(mGridItemParticleID);
    if (aGridItemParticle)
    {
        aGridItemParticle->Update();
    }

    if (mGridItemType == GridItemType::GRIDITEM_PORTAL_CIRCLE || mGridItemType == GridItemType::GRIDITEM_PORTAL_SQUARE)
    {
        UpdatePortal();
    }
    if (mGridItemType == GridItemType::GRIDITEM_SCARY_POT)
    {
        UpdateScaryPot();
    }
    if (mGridItemType == GridItemType::GRIDITEM_RAKE)
    {
        UpdateRake();
    }
    if (mGridItemType == GridItemType::GRIDITEM_IZOMBIE_BRAIN)
    {
        UpdateBrain();
    }
}

//0x44E6A0
Zombie* GridItem::RakeFindZombie()
{
    Rect aRakeRect(mPosX, mPosY, 63, 80);

    Zombie* aZombie = nullptr;
    while (mBoard->IterateZombies(aZombie))
    {
        if (!aZombie->IsDeadOrDying() && !aZombie->IsBobsledTeamWithSled() && aZombie->mRow - mGridY == 0 && aZombie->EffectedByDamage(1U))
        {
            Rect aZombieRect = aZombie->GetZombieRect();
            if (GetRectOverlap(aRakeRect, aZombieRect) >= 0)
            {
                return aZombie;
            }
        }
    }

    return nullptr;
}

//0x44E780
void GridItem::UpdateRake()
{
    if (mGridItemState == GridItemState::GRIDITEM_STATE_RAKE_ATTRACTING || mGridItemState == GridItemState::GRIDITEM_STATE_RAKE_WAITING)
    {
        if (RakeFindZombie())
        {
            Reanimation* aRakeReanim = mApp->ReanimationGet(mGridItemReanimID);
            aRakeReanim->mAnimRate = 20.0f;
            mGridItemCounter = 200;
            mGridItemState = GridItemState::GRIDITEM_STATE_RAKE_TRIGGERED;
            mApp->PlayFoley(FoleyType::FOLEY_SWING);
        }
    }
    else if (mGridItemState == GridItemState::GRIDITEM_STATE_RAKE_TRIGGERED)
    {
        Reanimation* aRakeReanim = mApp->ReanimationGet(mGridItemReanimID);
        if (aRakeReanim && aRakeReanim->ShouldTriggerTimedEvent(0.8f))
        {
            Zombie* aZombie = RakeFindZombie();
            if (aZombie)
            {
                aZombie->TakeDamage(1800, 0U);
                mApp->PlayFoley(FoleyType::FOLEY_BONK);
            }
        }

        mGridItemCounter--;
        if (mGridItemCounter == 0)
        {
            GridItemDie();
        }
    }
}
