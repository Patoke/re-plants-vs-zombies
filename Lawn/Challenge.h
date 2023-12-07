#ifndef __CHALLENGE_H__
#define __CHALLENGE_H__

#include "../ConstEnums.h"
#include "../GameConstants.h"
#include "../Sexy.TodLib/FilterEffect.h"
#include "graphics/Graphics.h"

#define BEGHOULED_MAX_GRIDSIZEX 8
#define BEGHOULED_MAX_GRIDSIZEY 5
#define ART_CHALLEGE_SIZE_X 8
#define MAX_PICK_GRID_SIZE 50

constexpr const int BEGHOULED_WINNING_SCORE = 75;
constexpr const int SLOT_MACHINE_WINNING_SCORE = 2000;
constexpr const int ZOMBIQUARIUM_WINNING_SCORE = 1000;
constexpr const int I_ZOMBIE_WINNING_SCORE = 5;
constexpr const int MAX_PORTALS = 4;
constexpr const int MAX_SQUIRRELS = 7;
constexpr const int MAX_SCARY_POTS = 54;
constexpr const int STORM_FLASH_TIME = 150;

using namespace Sexy;

class LawnApp;
class Board;
class Plant;
class Zombie;
class GridItem;
class SeedPacket;
class HitResult;
struct TodWeightedGridArray;

enum BeghouledUpgrade
{
    BEGHOULED_UPGRADE_REPEATER,
    BEGHOULED_UPGRADE_FUMESHROOM,
    BEGHOULED_UPGRADE_TALLNUT,
    NUM_BEGHOULED_UPGRADES
};

struct BeghouledBoardState
{
	SeedType		        mSeedType[9][6];
};

class Challenge
{
public:
	LawnApp*				mApp;                                               //+0x0
	Board*					mBoard;                                             //+0x4
	int					mBeghouledMouseCapture;                             //+0x8
    int                     mBeghouledMouseDownX;                               //+0xC
    int                     mBeghouledMouseDownY;                               //+0x10
    int                    mBeghouledEated[9][6];                              //+0x14
    int                    mBeghouledPurcasedUpgrade[NUM_BEGHOULED_UPGRADES];  //+0x4A
    int                     mBeghouledMatchesThisMove;                          //+0x50
    ChallengeState          mChallengeState;                                    //+0x54
    int                     mChallengeStateCounter;                             //+0x58
    int                     mConveyorBeltCounter;                               //+0x5C
    int                     mChallengeScore;                                    //+0x60
    int                    mShowBowlingLine;                                   //+0x64
    SeedType                mLastConveyorSeedType;                              //+0x68
    int                     mSurvivalStage;                                     //+0x6C
    int                     mSlotMachineRollCount;                              //+0x70
    ReanimationID           mReanimChallenge;                                   //+0x74
    ReanimationID           mReanimClouds[6];                                   //+0x78
    int                     mCloudsCounter[6];                                  //+0x90
    int                     mChallengeGridX;                                    //+0xA8
    int                     mChallengeGridY;                                    //+0xAC
    int                     mScaryPotterPots;                                   //+0xB0
    int                     mRainCounter;                                       //+0xB4
    int                     mTreeOfWisdomTalkIndex;                             //+0xB8

public:
    Challenge();

    void                    StartLevel();
    void                    BeghouledPopulateBoard();
    void                    LoadBeghouledBoardState(BeghouledBoardState* theState);
    SeedType                BeghouledPickSeed(int theGridX, int theGridY, BeghouledBoardState* theBoardState, int theAllowMatches);
    int                    BeghouledBoardHasMatch(BeghouledBoardState* theBoardState);
    /*inline*/ SeedType     BeghouledGetPlantAt(int theGridX, int theGridY, BeghouledBoardState* theBoardState);
    int                     BeghouledVerticalMatchLength(int theGridX, int theGridY, BeghouledBoardState* theBoardState);
    int                     BeghouledHorizontalMatchLength(int theGridX, int theGridY, BeghouledBoardState* theBoardState);
    /*inline*/ void         BeghouledDragStart(int x, int y);
    void                    BeghouledDragUpdate(int x, int y);
    inline void             BeghouledDragCancel() { mBeghouledMouseCapture = false; }
    int                    MouseMove(int x, int y);
    int                    MouseDown(int x, int y, int theClickCount, HitResult* theHitResult);
    int                    MouseUp(int x, int y);
    void                    ClearCursor();
    void                    BeghouledRemoveHorizontalMatch(int theGridX, int theGridY, BeghouledBoardState* theBoardState);
    void                    BeghouledRemoveVerticalMatch(int theGridX, int theGridY, BeghouledBoardState* theBoardState);
    void                    BeghouledRemoveMatches(BeghouledBoardState* theBoardState);
    void                    Update();
    void                    UpdateBeghouled();
    int                    UpdateBeghouledPlant(Plant* thePlant);
    void                    BeghouledFallIntoSquare(int theGridX, int theGridY, BeghouledBoardState* theBoardState);
    void                    BeghouledMakePlantsFall(BeghouledBoardState* theBoardState);
    void                    ZombieAtePlant(/*Zombie* theZombie,*/ Plant* thePlant);
    void                    DrawBackdrop(Graphics* g);
    void                    DrawArtChallenge(Graphics* g);
    void                    CheckForCompleteArtChallenge(int theGridX, int theGridY);
    /*inline*/ SeedType     GetArtChallengeSeed(int theGridX, int theGridY);
    void                    PlantAdded(Plant* thePlant);
    PlantingReason          CanPlantAt(int theGridX, int theGridY, SeedType theSeedType);
    void                    DrawBeghouled(Graphics* g);
    int                    BeghouledIsValidMove(int theFromX, int theFromY, int theToX, int theToY, BeghouledBoardState* theBoardState);
    int                    BeghouledCheckForPossibleMoves(BeghouledBoardState* theBoardState);
    void                    BeghouledCheckStuckState();
    void                    InitZombieWavesSurvival();
    /*inline*/ void         InitZombieWavesFromList(ZombieType* theZombieList, int theListLength);
    void                    InitZombieWaves();
    /*inline*/ Rect         SlotMachineGetHandleRect();
    void                    UpdateSlotMachine();
    void                    DrawSlotMachine(Graphics* g);
    int                    UpdateToolTip(int theX, int theY);
    void                    WhackAZombieSpawning();
    int                    UpdateZombieSpawning();
    void                    BeghouledClearCrater(int theCount);
    void                    MouseDownWhackAZombie(int theX, int theY);
    void                    DrawStormNight(Graphics* g);
    void                    UpdateStormyNight();
    void                    InitLevel();
    void                    SpawnZombieWave();
    void                    GraveDangerSpawnRandomGrave();
    void                    GraveDangerSpawnGraveAt(int theGridX, int theGridY);
    void                    SpawnLevelAward(int theGridX, int theGridY);
    void                    BeghouledScore(int theGridX, int theGridY, int theNumPlants, int theIsHorizontal);
    void                    DrawStormFlash(Graphics* g, int theTime, int theMaxAmount);
    void                    UpdateRainingSeeds();
    /*inline*/ void         PlayBossEnter();
    void                    UpdateConveyorBelt();
    void                    PortalStart();
    void                    UpdatePortalCombat();
    GridItem*               GetOtherPortal(GridItem* thePortal);
    void                    UpdatePortal(GridItem* thePortal);
    float                   PortalCombatRowSpawnWeight(int theGridY);
    int                    CanTargetZombieWithPortals(Plant* thePlant, Zombie* theZombie);
    GridItem*               GetPortalToRight(int theGridX, int theGridY);
    GridItem*               GetPortalAt(int theGridX, int theGridY);
    void                    MoveAPortal();
    int                     GetPortalDistanceToMower(int theGridY);
    GridItem*               GetPortalToLeft(int theGridX, int theGridY);
    void                    BeghouledPacketClicked(SeedPacket* theSeedPacket);
    void                    BeghouledShuffle();
    /*inline*/ int         BeghouledCanClearCrater();
    void                    BeghouledUpdateCraters();
    Zombie*                 ZombiquariumSpawnSnorkle();
    void                    ZombiquariumPacketClicked(SeedPacket* theSeedPacket);
    void                    ZombiquariumMouseDown(int x, int y);
    void                    ZombiquariumDropBrain(int x, int y);
    void                    ZombiquariumUpdate();
    /*inline*/ void         ShovelAddWallnuts();
    void                    ScaryPotterPlacePot(ScaryPotType theScaryPotType, ZombieType theZombieType, SeedType theSeedType, int theCount, TodWeightedGridArray* theGridArray, int theGridArrayCount);
    void                    ScaryPotterStart();
    void                    ScaryPotterUpdate();
    void                    ScaryPotterOpenPot(GridItem* theScaryPot);
    void                    ScaryPotterJackExplode(int thePosX, int thePosY);
    int                    ScaryPotterIsCompleted();
    void                    ScaryPotterChangePotType(GridItemState thePotType, int theCount);
    void                    ScaryPotterPopulate();
    /*inline*/ void         ScaryPotterDontPlaceInCol(int theCol, TodWeightedGridArray* theGridArray, int theGridArrayCount);
    void                    ScaryPotterFillColumnWithPlant(int theCol, SeedType theSeedType, TodWeightedGridArray* theGridArray, int theGridArrayCount);
    void                    PuzzleNextStageClear();
    void                    ScaryPotterMalletPot(GridItem* theScaryPot);
    static ZombieType       IZombieSeedTypeToZombieType(SeedType theSeedType);
    static /*inline*/ int  IsZombieSeedType(SeedType theSeedType);
    void                    IZombieMouseDownWithZombie(int theX, int theY, int theClickCount);
    void                    IZombieStart();
    void                    IZombiePlacePlants(SeedType theSeedType, int theCount, int theGridY = -1);
    void                    IZombieUpdate();
    void                    IZombieDrawPlant(Graphics* g, Plant* thePlant);
    void                    IZombieSetPlantFilterEffect(Plant* thePlant, FilterEffect theFilterEffect);
    /*inline*/ int          ScaryPotterCountSunInPot(GridItem* theScaryPot);
    int                     ScaryPotterCountPots();
    void                    IZombieInitLevel();
    void                    DrawRain(Graphics* g);
    void                    DrawWeather(Graphics* g);
    void                    SquirrelUpdate();
    /*inline*/ int          SquirrelCountUncaught();
    void                    SquirrelStart();
    void                    SquirrelFound(GridItem* theSquirrel);
    void                    SquirrelPeek(GridItem* theSquirrel);
    void                    SquirrelChew(GridItem* theSquirrel);
    void                    SquirrelUpdateOne(GridItem* theSquirrel);
    void                    IZombieSetupPlant(Plant* thePlant);
    void                    UpdateRain();
    int                    IZombieEatBrain(Zombie* theZombie);
    GridItem*               IZombieGetBrainTarget(Zombie* theZombie);
    /*inline*/ void         IZombiePlacePlantInSquare(SeedType theSeedType, int theGridX, int theGridY = -1);
    void                    AdvanceCrazyDaveDialog();
    void                    BeghouledFlashPlant(int theFlashX, int theFlashY, int theFromX, int theFromY, int theToX, int theToY);
    void                    BeghouledFlashAMatch();
    int                    BeghouledFlashFromBoardState(BeghouledBoardState* theBoardState, int theFromX, int theFromY, int theToX, int theToY);
    void                    IZombiePlantDropRemainingSun(Plant* thePlant);
    void                    IZombieSquishBrain(GridItem* theBrain);
    void                    IZombieScoreBrain(GridItem* theBrain);
    void                    LastStandUpdate();
    void                    WhackAZombiePlaceGraves(int theGraveCount);
    int                    BeghouledTwistSquareFromMouse(int theX, int theY, int& theGridX, int& theGridY);
    int                    BeghouledTwistValidMove(int theGridX, int theGridY, BeghouledBoardState* theBoardState);
    void                    BeghouledTwistMouseDown(int x, int y);
    int                    BeghouledTwistMoveCausesMatch(int theGridX, int theGridY, BeghouledBoardState* theBoardState);
    int                    BeghouledTwistFlashMatch(BeghouledBoardState* theBoardState, int theGridX, int theGridY);
    /*inline*/ void         BeghouledCancelMatchFlashing();
    void                    BeghouledStartFalling(ChallengeState theState);
    void                    BeghouledFillHoles(BeghouledBoardState* theBoardState, int theAllowMatches);
    /*inline*/ void         BeghouledMakeStartBoard();
    void                    BeghouledCreatePlants(BeghouledBoardState* theOldBoardState, BeghouledBoardState* theNewBoardState);
    void                    PuzzlePhaseComplete(int theGridX, int theGridY);
    /*inline*/ int         PuzzleIsAwardStage();
    void                    IZombiePlaceZombie(ZombieType theZombieType, int theGridX, int theGridY);
    void                    WhackAZombieUpdate();
    void                    LastStandCompletedStage();
    void                    TreeOfWisdomUpdate();
    void                    TreeOfWisdomFertilize();
    void                    TreeOfWisdomInit();
    /*inline*/ int         TreeOfWisdomMouseOn(int theX, int theY);
    /*inline*/ int          TreeOfWisdomGetSize();
    void                    TreeOfWisdomDraw(Graphics* g);
    /*inline*/ void         TreeOfWisdomNextGarden();
    /*inline*/ void         TreeOfWisdomToolUpdate(GridItem* theZenTool);
    void                    TreeOfWisdomOpenStore();
    void                    TreeOfWisdomLeave();
    void                    TreeOfWisdomGrow();
    /*inline*/ void         TreeOfWisdomTool(int theMouseX, int theMouseY);
    int                    TreeOfWisdomHitTest(int theX, int theY, HitResult* theHitResult);
    void                    TreeOfWisdomBabble();
    void                    TreeOfWisdomGiveWisdom();
    void                    TreeOfWisdomSayRepeat();
    int                    TreeOfWisdomCanFeed();

    GridItem*               GetPortalLeftRight(int theGridX, int theGridY, int theToLeft = true);
};

extern SeedType gArtChallengeWallnut[6][9];
extern SeedType gArtChallengeSunFlower[6][9];
extern SeedType gArtChallengeStarFruit[6][9];

class ZombieAllowedLevels
{
public:
    ZombieType                      mZombieType;
    int                             mAllowedOnLevel[50];
};
extern int gZombieWaves[NUM_LEVELS]; //0x6A34E8
extern ZombieAllowedLevels gZombieAllowedLevels[NUM_ZOMBIE_TYPES];  //0x6A35B0

#endif