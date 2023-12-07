#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "GameObject.h"
#include "../GameConstants.h"

#define MAX_ZOMBIE_FOLLOWERS 4
#define NUM_BOBSLED_FOLLOWERS 3
#define NUM_BACKUP_DANCERS 4
#define NUM_BOSS_BUNGEES 3

constexpr const int ZOMBIE_START_RANDOM_OFFSET = 40;
constexpr const int BUNGEE_ZOMBIE_HEIGHT = 3000;
constexpr const int RENDER_GROUP_SHIELD = 1;
constexpr const int RENDER_GROUP_ARMS = 2;
constexpr const int RENDER_GROUP_OVER_SHIELD = 3;
constexpr const int RENDER_GROUP_BOSS_BACK_LEG = 4;
constexpr const int RENDER_GROUP_BOSS_FRONT_LEG = 5;
constexpr const int RENDER_GROUP_BOSS_BACK_ARM = 6;
constexpr const int RENDER_GROUP_BOSS_FIREBALL_ADDITIVE = 7;
constexpr const int RENDER_GROUP_BOSS_FIREBALL_TOP = 8;
constexpr const int ZOMBIE_LIMP_SPEED_FACTOR = 2;
constexpr const int POGO_BOUNCE_TIME = 80;
constexpr const int DOLPHIN_JUMP_TIME = 120;
constexpr const int JackInTheBoxZombieRadius = 115;
constexpr const int JackInTheBoxPlantRadius = 90;
constexpr const int BOBSLED_CRASH_TIME = 150;
constexpr const int ZOMBIE_BACKUP_DANCER_RISE_HEIGHT = -200;
constexpr const int BOSS_FLASH_HEALTH_FRACTION = 10;
constexpr const int TICKS_BETWEEN_EATS = 4;
constexpr const int DAMAGE_PER_EAT = TICKS_BETWEEN_EATS;
constexpr const float THOWN_ZOMBIE_GRAVITY = 0.05f;
constexpr const float CHILLED_SPEED_FACTOR = 0.4f;
constexpr const float CLIP_HEIGHT_LIMIT = -100.0f;
constexpr const float CLIP_HEIGHT_OFF = -200.0f;
const Color ZOMBIE_MINDCONTROLLED_COLOR = Color(128, 0, 192, 255);

enum ZombieAttackType
{
    ATTACKTYPE_CHEW,
    ATTACKTYPE_DRIVE_OVER,
    ATTACKTYPE_VAULT,
    ATTACKTYPE_LADDER
};

enum ZombieParts
{
    PART_BODY,
    PART_HEAD,
    PART_HEAD_EATING,
    PART_TONGUE,
    PART_ARM,
    PART_HAIR,
    PART_HEAD_YUCKY,
    PART_ARM_PICKAXE,
    PART_ARM_POLEVAULT,
    PART_ARM_LEASH,
    PART_ARM_FLAG,
    PART_POGO,
    PART_DIGGER
};

class ZombieDrawPosition
{
public:
    int                             mHeadX;
    int                             mHeadY;
    int                             mArmY;
    float                           mBodyY;
    float                           mImageOffsetX;
    float                           mImageOffsetY;
    float                           mClipHeight;
};

class Plant;
class Reanimation;
class TodParticleSystem;
class Zombie : public GameObject
{
public:
    enum
    {
        ZOMBIE_WAVE_DEBUG = -1,
        ZOMBIE_WAVE_CUTSCENE = -2,
        ZOMBIE_WAVE_UI = -3,
        ZOMBIE_WAVE_WINNER = -4
    };

public:
	ZombieType			            mZombieType;                                //+0x24
	ZombiePhase			            mZombiePhase;                               //+0x28
	float				            mPosX;                                      //+0x2C
	float				            mPosY;                                      //+0x30
	float				            mVelX;                                      //+0x34
    int                             mAnimCounter;                               //+0x38
    int                             mGroanCounter;                              //+0x3C
    int                             mAnimTicksPerFrame;                         //+0x40
    int                             mAnimFrames;                                //+0x44
    int                             mFrame;                                     //+0x48
    int                             mPrevFrame;                                 //+0x4C
    bool                            mVariant;                                   //+0x50
    bool                            mIsEating;                                  //+0x51
    int                             mJustGotShotCounter;                        //+0x54
    int                             mShieldJustGotShotCounter;                  //+0x58
    int                             mShieldRecoilCounter;                       //+0x5C
    int                             mZombieAge;                                 //+0x60
    ZombieHeight                    mZombieHeight;                              //+0x64
    int                             mPhaseCounter;                              //+0x68
    int                             mFromWave;                                  //+0x6C
    bool                            mDroppedLoot;                               //+0x70
    int                             mZombieFade;                                //+0x74
    bool                            mFlatTires;                                 //+0x78
    int                             mUseLadderCol;                              //+0x7C
    int                             mTargetCol;                                 //+0x80
    float                           mAltitude;                                  //+0x84
    bool                            mHitUmbrella;                               //+0x88
    Rect                            mZombieRect;                                //+0x8C
    Rect                            mZombieAttackRect;                          //+0x9C
    int                             mChilledCounter;                            //+0xAC
    int                             mButteredCounter;                           //+0xB0
    int                             mIceTrapCounter;                            //+0xB4
    bool                            mMindControlled;                            //+0xB8
    bool                            mBlowingAway;                               //+0xB9
    bool                            mHasHead;                                   //+0xBA
    bool                            mHasArm;                                    //+0xBB
    bool                            mHasObject;                                 //+0xBC
    bool                            mInPool;                                    //+0xBD
    bool                            mOnHighGround;                              //+0xBE
    bool                            mYuckyFace;                                 //+0xBF
    int                             mYuckyFaceCounter;                          //+0xC0
    HelmType                        mHelmType;                                  //+0xC4
    int                             mBodyHealth;                                //+0xC8
    int                             mBodyMaxHealth;                             //+0xCC
    int                             mHelmHealth;                                //+0xD0
    int                             mHelmMaxHealth;                             //+0xD4
    ShieldType                      mShieldType;                                //+0xD8
    int                             mShieldHealth;                              //+0xDC
    int                             mShieldMaxHealth;                           //+0xE0
    int                             mFlyingHealth;                              //+0xE4
    int                             mFlyingMaxHealth;                           //+0xE8
    bool                            mDead;                                      //+0xEC
    ZombieID                        mRelatedZombieID;                           //+0xF0
    ZombieID                        mFollowerZombieID[MAX_ZOMBIE_FOLLOWERS];    //+0xF4
    bool                            mPlayingSong;                               //+0x104
    int                             mParticleOffsetX;                           //+0x108
    int                             mParticleOffsetY;                           //+0x10C
    AttachmentID                    mAttachmentID;                              //+0x110
    int                             mSummonCounter;                             //+0x114
    ReanimationID                   mBodyReanimID;                              //+0x118
    float                           mScaleZombie;                               //+0x11C
    float                           mVelZ;                                      //+0x120
    float                           mOriginalAnimRate;                          //+0x124
    PlantID                         mTargetPlantID;                             //+0x128
    int                             mBossMode;                                  //+0x12C
    int                             mTargetRow;                                 //+0x130
    int                             mBossBungeeCounter;                         //+0x134
    int                             mBossStompCounter;                          //+0x138
    int                             mBossHeadCounter;                           //+0x13C
    ReanimationID                   mBossFireBallReanimID;                      //+0x140
    ReanimationID                   mSpecialHeadReanimID;                       //+0x144
    int                             mFireballRow;                               //+0x148
    bool                            mIsFireBall;                                //+0x14C
    ReanimationID                   mMoweredReanimID;                           //+0x150
    int                             mLastPortalX;                               //+0x154

public:
    Zombie();
    ~Zombie();

    void                            ZombieInitialize(int theRow, ZombieType theType, bool theVariant, Zombie* theParentZombie, int theFromWave);
    void                            Animate();
    void                            CheckIfPreyCaught();
    void                            EatZombie(Zombie* theZombie);
    void                            EatPlant(Plant* thePlant);
    void                            Update();
    void                            DieNoLoot();
    /*inline*/ void                 DieWithLoot();
    void                            Draw(Graphics* g);
//  void                            DrawZombie(Graphics* g, const ZombieDrawPosition& theDrawPos);
//  void                            DrawZombieWithParts(Graphics* g, const ZombieDrawPosition& theDrawPos);
    void                            DrawZombiePart(Graphics* g, Image* theImage, int theFrame, int theRow, const ZombieDrawPosition& theDrawPos);
    void                            DrawBungeeCord(Graphics* g, int theOffsetX);
    void                            TakeDamage(int theDamage, unsigned int theDamageFlags);
    /*inline*/ void                 SetRow(int theRow);
    float                           GetPosYBasedOnRow(int theRow);
    void                            ApplyChill(bool theIsIceTrap);
    void                            UpdateZombieBungee();
    void                            BungeeLanding();
    bool                            EffectedByDamage(unsigned int theDamageRangeFlags);
    void                            PickRandomSpeed();
    void                            UpdateZombiePolevaulter();
    void                            UpdateZombieDolphinRider();
    void                            PickBungeeZombieTarget(int theColumn);
    int                             CountBungeesTargetingSunFlowers();
    Plant*                          FindPlantTarget(ZombieAttackType theAttackType);
    void                            CheckSquish(ZombieAttackType theAttackType);
    void                            RiseFromGrave(int theCol, int theRow);
    void                            UpdateZombieRiseFromGrave();
    void                            UpdateDamageStates(unsigned int theDamageFlags);
    void                            UpdateZombiePool();
    void                            CheckForPool();
    void                            GetDrawPos(ZombieDrawPosition& theDrawPos);
    void                            UpdateZombieHighGround();
    void                            CheckForHighGround();
    bool                            IsOnHighGround();
    void                            DropLoot();
    bool                            TrySpawnLevelAward();
    /*inline*/ void                 StartZombieSound();
    void                            StopZombieSound();
    void                            UpdateZombieJackInTheBox();
    void                            DrawZombieHead(Graphics* g, const ZombieDrawPosition& theDrawPos, int theFrame);
    void                            UpdateZombiePosition();
    Rect                            GetZombieRect();
    Rect                            GetZombieAttackRect();
    void                            UpdateZombieWalking();
    void                            UpdateZombieBobsled();
    void                            BobsledCrash();
    Plant*                          IsStandingOnSpikeweed();
    void                            CheckForZombieStep();
    void                            CountExpectedMowers() { ; }
    /*inline*/ void                 OverrideParticleColor(TodParticleSystem* aParticle);
    /*inline*/ void                 OverrideParticleScale(TodParticleSystem* aParticle);
    void                            PoolSplash(bool theInToPoolSound);
    void                            UpdateZombieFlyer();
    void                            UpdateZombiePogo();
    void                            UpdateZombieNewspaper();
    void                            LandFlyer(unsigned int theDamageFlags);
    void                            UpdateZombieDigger();
    bool                            IsWalkingBackwards();
    TodParticleSystem*              AddAttachedParticle(int thePosX, int thePosY, ParticleEffect theEffect);
    void                            PogoBreak(unsigned int theDamageFlags);
    void                            UpdateZombieFalling();
    void                            UpdateZombieDancer();
    ZombieID                        SummonBackupDancer(int theRow, int thePosX);
    void                            SummonBackupDancers();
    int                             GetDancerFrame();
    void                            BungeeStealTarget();
    void                            BungeeLiftTarget();
    void                            UpdateYuckyFace();
    void                            DrawIceTrap(Graphics* g, const ZombieDrawPosition& theDrawPos, bool theFront);
    void                            HitIceTrap();
    int                             GetHelmDamageIndex();
    int                             GetShieldDamageIndex();
    void                            DrawReanim(Graphics* g, const ZombieDrawPosition& theDrawPos, int theBaseRenderGroup);
    void                            UpdatePlaying();
    bool                            NeedsMoreBackupDancers();
    void                            ConvertToNormalZombie();
    void                            UpdateDancerWalking() { ; }
    void                            StartEating();
    void                            StopEating();
    void                            UpdateAnimSpeed();
    /*inline*/ void                 ReanimShowPrefix(const char* theTrackPrefix, int theRenderGroup);
    void                            PlayDeathAnim(unsigned int theDamageFlags);
    void                            UpdateDeath();
    void                            DrawShadow(Graphics* g);
    bool                            HasShadow();
    Reanimation*                    LoadReanim(ReanimationType theReanimationType);
    /*inline*/ int                  TakeFlyingDamage(int theDamage, unsigned int theDamageFlags);
    int                             TakeShieldDamage(int theDamage, unsigned int theDamageFlags);
    int                             TakeHelmDamage(int theDamage, unsigned int theDamageFlags);
    void                            TakeBodyDamage(int theDamage, unsigned int theDamageFlags);
    void                            AttachShield();
    void                            DetachShield();
    void                            UpdateReanim();
    void                            GetTrackPosition(const char* theTrackName, float& thePosX, float& thePosY);
    void                            LoadPlainZombieReanim();
    void                            ShowDoorArms(bool theShow);
    /*inline*/ void                 ReanimShowTrack(const char* theTrackName, int theRenderGroup);
    /*inline*/ void                 PlayZombieAppearSound();
    void                            StartMindControlled();
    bool                            IsFlying();
    void                            DropHead(unsigned int theDamageFlags);
    bool                            CanTargetPlant(Plant* thePlant, ZombieAttackType theAttackType);
    void                            UpdateZombieCatapult();
    Plant*                          FindCatapultTarget();
    void                            ZombieCatapultFire(Plant* thePlant);
    void                            UpdateClimbingLadder();
    void                            UpdateZombieGargantuar();
    int                             GetBodyDamageIndex();
    void                            ApplyBurn();
    void                            UpdateBurn();
    bool                            ZombieNotWalking();
    Zombie*                         FindZombieTarget();
    /*inline*/ void                 PlayZombieReanim(const char* theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate);
    void                            UpdateZombieBackupDancer();
    ZombiePhase                     GetDancerPhase();
    bool                            IsMovingAtChilledSpeed();
    void                            StartWalkAnim(int theBlendTime);
    Reanimation*                    AddAttachedReanim(int thePosX, int thePosY, ReanimationType theReanimType);
    void                            DragUnder();
    static /*inline*/ void          SetupDoorArms(Reanimation* aReanim, bool theShow);
    static void                     SetupReanimLayers(Reanimation* aReanim, ZombieType theZombieType);
    /*inline*/ bool                 IsOnBoard();
    void                            DrawButter(Graphics* g, const ZombieDrawPosition& theDrawPos);
    bool                            IsImmobilizied();
    void                            ApplyButter();
    float                           ZombieTargetLeadX(float theTime);
    void                            UpdateZombieImp();
    void                            SquishAllInSquare(int theX, int theY, ZombieAttackType theAttackType);
    void                            RemoveIceTrap();
    bool                            IsBouncingPogo();
    int                             GetBobsledPosition();
    void                            DrawBobsledReanim(Graphics* g, const ZombieDrawPosition& theDrawPos, bool theBeforeZombie);
    void                            BobsledDie();
    void                            BobsledBurn();
    bool                            IsBobsledTeamWithSled();
    bool                            CanBeFrozen();
    bool                            CanBeChilled();
    void                            UpdateZombieSnorkel();
    void                            ReanimIgnoreClipRect(const char* theTrackName, bool theIgnoreClipRect);
    void                            SetAnimRate(float theAnimRate);
    void                            ApplyAnimRate(float theAnimRate);
    /*inline*/ bool                 IsDeadOrDying();
    void                            DrawDancerReanim(Graphics* g);
    void                            DrawBungeeReanim(Graphics* g);
    void                            DrawBungeeTarget(Graphics* g);
    void                            BungeeDie();
    void                            ZamboniDeath(unsigned int theDamageFlags);
    void                            CatapultDeath(unsigned int theDamageFlags);
    bool                            SetupDrawZombieWon(Graphics* g);
    void                            WalkIntoHouse();
    void                            UpdateZamboni();
    void                            UpdateZombieChimney();
    void                            UpdateLadder();
    void                            DropArm(unsigned int theDamageFlags);
    bool                            CanLoseBodyParts();
    void                            DropHelm(unsigned int theDamageFlags);
    void                            DropShield(unsigned int theDamageFlags);
    void                            ReanimReenableClipping();
    void                            UpdateBoss();
    void                            BossPlayIdle();
    void                            BossRVLanding();
    void                            BossStompContact();
    bool                            BossAreBungeesDone();
    void                            BossBungeeSpawn();
    void                            BossSpawnAttack();
    void                            BossBungeeAttack();
    void                            BossRVAttack();
    void                            BossSpawnContact();
    void                            BossBungeeLeave();
    void                            BossStompAttack();
    bool                            BossCanStompRow(int theRow);
    void                            BossDie();
    void                            BossHeadAttack();
    void                            BossHeadSpitContact();
    void                            BossHeadSpit();
    void                            UpdateBossFireball();
    void                            BossDestroyFireball();
    void                            BossDestroyIceballInRow();
    void                            DiggerLoseAxe();
    void                            BungeeDropZombie(Zombie* theDroppedZombie, int theGridX, int theGridY);
    void                            ShowYuckyFace(bool theShow);
    void                            AnimateChewSound();
    void                            AnimateChewEffect();
    void                            UpdateActions();
    void                            CheckForBoardEdge();
    void                            UpdateYeti();
    void                            DrawBossPart(Graphics* g, BossPart theBossPart);
    void                            BossSetupReanim();
    void                            MowDown();
    void                            UpdateMowered();
    void                            DropFlag();
    void                            DropPole();
    void                            DrawBossBackArm(Graphics* g, const ZombieDrawPosition& theDrawPos);
    static void                     PreloadZombieResources(ZombieType theZombieType);
    void                            BossStartDeath();
    void                            RemoveColdEffects();
    void                            BossHeadSpitEffect();
    void                            DrawBossFireBall(Graphics* g);
    void                            UpdateZombiePeaHead();
    void                            UpdateZombieJalapenoHead();
    void                            ApplyBossSmokeParticles(bool theEnable);
    void                            UpdateZombiquarium();
    bool                            ZombiquariumFindClosestBrain();
    void                            UpdateZombieGatlingHead();
    void                            UpdateZombieSquashHead();
    bool                            IsTanglekelpTarget();
    bool                            HasYuckyFaceImage();
    bool                            IsTangleKelpTarget();
    bool                            IsFireResistant();
    /*inline*/ void                 EnableMustache(bool theEnableMustache);
    /*inline*/ void                 EnableFuture(bool theEnableFuture);
    /*inline*/ void                 EnableDance();
    void                            BungeeDropPlant();
    void                            RemoveButter();
    void                            BalloonPropellerHatSpin(bool theSpinning);
    void                            DoDaisies();
    static /*inline*/ bool          ZombieTypeCanGoOnHighGround(ZombieType theZombieType);
    static /*inline*/ bool          ZombieTypeCanGoInPool(ZombieType theZombieType);
    void                            SetupWaterTrack(const char* theTrackName);
    void                            BurnRow(int theRow);
    void                            SetupReanimForLostHead();
    void                            SetupReanimForLostArm(unsigned int theDamageFlags);
    bool                            IsSquashTarget(Plant* theExcept);
    static /*inline*/ bool			IsZombotany(ZombieType theZombieType);
};

class ZombieDefinition
{
public:
    ZombieType                      mZombieType;
    ReanimationType                 mReanimationType;
    int                             mZombieValue;
    int                             mStartingLevel;
    int                             mFirstAllowedWave;
    int                             mPickWeight;
    const SexyChar*                 mZombieName;
};
extern ZombieDefinition gZombieDefs[NUM_ZOMBIE_TYPES];  //0x69DA80

/*inline*/ ZombieDefinition&            GetZombieDefinition(ZombieType theZombieType);

#endif