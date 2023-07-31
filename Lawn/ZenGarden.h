#ifndef __ZENGARDEN_H__
#define __ZENGARDEN_H__

#include "../ConstEnums.h"

#define ZEN_MAX_GRIDSIZE_X 8
#define ZEN_MAX_GRIDSIZE_Y 4

constexpr const float STINKY_SLEEP_POS_Y = 461.0f;

class LawnApp;
class Board;
class Plant;
class GridItem;
class HitResult;
class PottedPlant;
namespace Sexy
{
    class Graphics;
}
using namespace Sexy;

class SpecialGridPlacement
{
public:
    int                     mPixelX;
    int                     mPixelY;
    int                     mGridX;
    int                     mGridY;
};

class ZenGarden
{
public:
    LawnApp*                mApp;           //+0x0
    Board*                  mBoard;         //+0x4
    GardenType              mGardenType;    //+0x8

public:
    ZenGarden();

    void                    ZenGardenInitLevel(bool theJustSwitchingGardens);
    /*inline*/ void         DrawPottedPlantIcon(Graphics* g, float x, float y, PottedPlant* thePottedPlant);
    void                    DrawPottedPlant(Graphics* g, float x, float y, PottedPlant* thePottedPlant, float theScale, bool theDrawPot);
    bool                    IsZenGardenFull(bool theIncludeDroppedPresents);
    void                    FindOpenZenGardenSpot(int& theSpotX, int& theSpotY);
    void                    AddPottedPlant(PottedPlant* thePottedPlant);
    void                    MouseDownWithTool(int x, int y, CursorType theCursorType);
    void                    MovePlant(Plant* thePlant, int theGridX, int theGridY);
    void                    MouseDownWithMoneySign(Plant* thePlant);
    Plant*                  PlacePottedPlant(int thePottedPlantIndex);
    float                   PlantPottedDrawHeightOffset(SeedType theSeedType, float theScale);
    static float            ZenPlantOffsetX(PottedPlant* thePottedPlant);
    int                     GetPlantSellPrice(Plant* thePlant);
    void                    ZenGardenUpdate();
    void                    MouseDownWithFullWheelBarrow(int x, int y);
    void                    MouseDownWithEmptyWheelBarrow(Plant* thePlant);
    void                    GotoNextGarden();
    /*inline*/ PottedPlant* GetPottedPlantInWheelbarrow();
    void                    RemovePottedPlant(Plant* thePlant);
    SpecialGridPlacement*   GetSpecialGridPlacements(int& theCount);
    int                     PixelToGridX(int theX, int theY);
    int                     PixelToGridY(int theX, int theY);
    int                     GridToPixelX(int theGridX, int theGridY);
    int                     GridToPixelY(int theGridX, int theGridY);
    void                    DrawBackdrop(Graphics* g);
    bool                    MouseDownZenGarden(int x, int y, int theClickCount, HitResult* theHitResult);
    void                    PlantFulfillNeed(Plant* thePlant);
    void                    PlantWatered(Plant* thePlant);
    PottedPlantNeed         GetPlantsNeed(PottedPlant* thePottedPlant);
    void                    MouseDownWithFeedingTool(int x, int y, CursorType theCursorType);
    void                    DrawPlantOverlay(Graphics* g, Plant* thePlant);
    PottedPlant*            PottedPlantFromIndex(int thePottedPlantIndex);
    bool                    WasPlantNeedFulfilledToday(PottedPlant* thePottedPlant);
    void                    PottedPlantUpdate(Plant* thePlant);
    void                    AddHappyEffect(Plant* thePlant);
    void                    RemoveHappyEffect(Plant* thePlant);
    void                    PlantUpdateProduction(Plant* thePlant);
    bool                    CanDropPottedPlantLoot();
    void                    ShowTutorialArrowOnWateringCan();
    bool                    PlantsNeedWater();
    void                    ZenGardenStart();
    void                    UpdatePlantEffectState(Plant* thePlant);
    bool                    CanUseGameObject(GameObjectType);
    void                    ZenToolUpdate(GridItem* theZenTool);
    void                    DoFeedingTool(int x, int y, GridItemState theToolType);
    void                    AddStinky();
    void                    StinkyUpdate(GridItem* theStinky);
    void                    OpenStore();
    GridItem*               GetStinky();
    void                    StinkyPickGoal(GridItem* theStinky);
    bool                    PlantShouldRefreshNeed(PottedPlant* thePottedPlant);
    void                    PlantFertilized(Plant* thePlant);
    bool                    WasPlantFertilizedInLastHour(PottedPlant* thePottedPlant);
    void                    SetupForZenTutorial();
    bool                    HasPurchasedStinky();
    int                     CountPlantsNeedingFertilizer();
    bool                    AllPlantsHaveBeenFertilized();
    void                    WakeStinky();
    bool                    ShouldStinkyBeAwake();
    bool                    IsStinkySleeping();
    static SeedType         PickRandomSeedType();
    void                    StinkyWakeUp(GridItem* theStinky);
    void                    StinkyStartFallingAsleep(GridItem* theStinky);
    void                    StinkyFinishFallingAsleep(GridItem* theStinky, int theBlendTime);
    void                    AdvanceCrazyDaveDialog();
    void                    LeaveGarden();
    bool                    CanDropChocolate();
    void                    FeedChocolateToPlant(Plant* thePlant);
    bool                    PlantHighOnChocolate(PottedPlant* thePottedPlant);
    bool                    PlantCanHaveChocolate(Plant* thePlant);
    void                    SetPlantAnimSpeed(Plant* thePlant);
    void                    UpdateStinkyMotionTrail(GridItem* theStinky, bool theStinkyHighOnChocolate);
    void                    ResetPlantTimers(PottedPlant* thePottedPlant);
    void                    ResetStinkyTimers();
    void                    UpdatePlantNeeds();
    void                    RefreshPlantNeeds(PottedPlant* thePottedPlant);
    void                    PlantSetLaunchCounter(Plant* thePlant);
    int                     PlantGetMinutesSinceHappy(Plant* thePlant);
    /*inline*/ bool         IsStinkyHighOnChocolate();
    void                    StinkyAnimRateUpdate(GridItem* theStinky);
    /*inline*/ bool         PlantCanBeWatered(Plant* thePlant);
};

#endif
