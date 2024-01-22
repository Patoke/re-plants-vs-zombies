#include "Common.h"
#include "TodDebug.h"
#include "TodCommon.h"
#include "Definition.h"
#include "Reanimator.h"
#include "LawnApp.h"
#include "Attachment.h"
#include "ReanimAtlas.h"
#include "EffectSystem.h"
#include "GameConstants.h"
#include "graphics/Font.h"
#include "misc/PerfTimer.h"
//#include "graphics/MemoryImage.h"
#include <bits/chrono.h>
#include <chrono>
#include <string>

unsigned int gReanimatorDefCount;                     //[0x6A9EE4]
ReanimatorDefinition* gReanimatorDefArray;   //[0x6A9EE8]
unsigned int gReanimationParamArraySize;              //[0x6A9EEC]
ReanimationParams* gReanimationParamArray;   //[0x6A9EF0]

ReanimationParams gLawnReanimationArray[(int)ReanimationType::NUM_REANIMS] = { //0x6A1340
	{ ReanimationType::REANIM_LOADBAR_SPROUT,                       "LoadBar_sprout.reanim",                    1 },
	{ ReanimationType::REANIM_LOADBAR_ZOMBIEHEAD,                   "LoadBar_Zombiehead.reanim",                1 },
	{ ReanimationType::REANIM_SODROLL,                              "SodRoll.reanim",                           0 },
	{ ReanimationType::REANIM_FINAL_WAVE,                           "FinalWave.reanim",                         1 },
	{ ReanimationType::REANIM_PEASHOOTER,                           "PeaShooterSingle.reanim",                  0 },
	{ ReanimationType::REANIM_WALLNUT,                              "Wallnut.reanim",                           0 },
	{ ReanimationType::REANIM_LILYPAD,                              "Lilypad.reanim",                           0 },
	{ ReanimationType::REANIM_SUNFLOWER,                            "SunFlower.reanim",                         0 },
	{ ReanimationType::REANIM_LAWNMOWER,                            "LawnMower.reanim",                         0 },
	{ ReanimationType::REANIM_READYSETPLANT,                        "StartReadySetPlant.reanim",                1 },
	{ ReanimationType::REANIM_CHERRYBOMB,                           "CherryBomb.reanim",                        0 },
	{ ReanimationType::REANIM_SQUASH,                               "Squash.reanim",                            0 },
	{ ReanimationType::REANIM_DOOMSHROOM,                           "DoomShroom.reanim",                        0 },
	{ ReanimationType::REANIM_SNOWPEA,                              "SnowPea.reanim",                           0 },
	{ ReanimationType::REANIM_REPEATER,                             "PeaShooter.reanim",                        0 },
	{ ReanimationType::REANIM_SUNSHROOM,                            "SunShroom.reanim",                         0 },
	{ ReanimationType::REANIM_TALLNUT,                              "Tallnut.reanim",                           0 },
	{ ReanimationType::REANIM_FUMESHROOM,                           "Fumeshroom.reanim",                        0 },
	{ ReanimationType::REANIM_PUFFSHROOM,                           "Puffshroom.reanim",                        0 },
	{ ReanimationType::REANIM_HYPNOSHROOM,                          "Hypnoshroom.reanim",                       0 },
	{ ReanimationType::REANIM_CHOMPER,                              "Chomper.reanim",                           0 },
	{ ReanimationType::REANIM_ZOMBIE,                               "Zombie.reanim",                            0 },
	{ ReanimationType::REANIM_SUN,                                  "Sun.reanim",                               0 },
	{ ReanimationType::REANIM_POTATOMINE,                           "PotatoMine.reanim",                        0 },
	{ ReanimationType::REANIM_SPIKEWEED,                            "Caltrop.reanim",                           0 },
	{ ReanimationType::REANIM_SPIKEROCK,                            "SpikeRock.reanim",                         0 },
	{ ReanimationType::REANIM_THREEPEATER,                          "ThreePeater.reanim",                       0 },
	{ ReanimationType::REANIM_MARIGOLD,                             "Marigold.reanim",                          0 },
	{ ReanimationType::REANIM_ICESHROOM,                            "IceShroom.reanim",                         0 },
	{ ReanimationType::REANIM_ZOMBIE_FOOTBALL,                      "Zombie_football.reanim",                   0 },
	{ ReanimationType::REANIM_ZOMBIE_NEWSPAPER,                     "Zombie_paper.reanim",                      0 },
	{ ReanimationType::REANIM_ZOMBIE_ZAMBONI,                       "Zombie_zamboni.reanim",                    0 },
	{ ReanimationType::REANIM_SPLASH,                               "splash.reanim",                            0 },
	{ ReanimationType::REANIM_JALAPENO,                             "Jalapeno.reanim",                          0 },
	{ ReanimationType::REANIM_JALAPENO_FIRE,                        "fire.reanim",                              0 },
	{ ReanimationType::REANIM_COIN_SILVER,                          "Coin_silver.reanim",                       0 },
	{ ReanimationType::REANIM_ZOMBIE_CHARRED,                       "Zombie_charred.reanim",                    0 },
	{ ReanimationType::REANIM_ZOMBIE_CHARRED_IMP,                   "Zombie_charred_imp.reanim",                0 },
	{ ReanimationType::REANIM_ZOMBIE_CHARRED_DIGGER,                "Zombie_charred_digger.reanim",             0 },
	{ ReanimationType::REANIM_ZOMBIE_CHARRED_ZAMBONI,               "Zombie_charred_zamboni.reanim",            0 },
	{ ReanimationType::REANIM_ZOMBIE_CHARRED_CATAPULT,              "Zombie_charred_catapult.reanim",           0 },
	{ ReanimationType::REANIM_ZOMBIE_CHARRED_GARGANTUAR,            "Zombie_charred_gargantuar.reanim",         0 },
	{ ReanimationType::REANIM_SCRAREYSHROOM,                        "ScaredyShroom.reanim",                     0 },
	{ ReanimationType::REANIM_PUMPKIN,                              "Pumpkin.reanim",                           0 },
	{ ReanimationType::REANIM_PLANTERN,                             "Plantern.reanim",                          0 },
	{ ReanimationType::REANIM_TORCHWOOD,                            "Torchwood.reanim",                         0 },
	{ ReanimationType::REANIM_SPLITPEA,                             "SplitPea.reanim",                          0 },
	{ ReanimationType::REANIM_SEASHROOM,                            "SeaShroom.reanim",                         0 },
	{ ReanimationType::REANIM_BLOVER,                               "Blover.reanim",                            0 },
	{ ReanimationType::REANIM_FLOWER_POT,                           "Pot.reanim",                               0 },
	{ ReanimationType::REANIM_CACTUS,                               "Cactus.reanim",                            0 },
	{ ReanimationType::REANIM_DANCER,                               "Zombie_disco.reanim",						0 }, // @Patoke: GOTY has different reanim name
	{ ReanimationType::REANIM_TANGLEKELP,                           "Tanglekelp.reanim",                        0 },
	{ ReanimationType::REANIM_STARFRUIT,                            "Starfruit.reanim",                         0 },
	{ ReanimationType::REANIM_POLEVAULTER,                          "Zombie_polevaulter.reanim",                0 },
	{ ReanimationType::REANIM_BALLOON,                              "Zombie_balloon.reanim",                    0 },
	{ ReanimationType::REANIM_GARGANTUAR,                           "Zombie_gargantuar.reanim",                 0 },
	{ ReanimationType::REANIM_IMP,                                  "Zombie_imp.reanim",                        0 },
	{ ReanimationType::REANIM_DIGGER,                               "Zombie_digger.reanim",                     0 },
	{ ReanimationType::REANIM_DIGGER_DIRT,                          "Digger_rising_dirt.reanim",                0 },
	{ ReanimationType::REANIM_ZOMBIE_DOLPHINRIDER,                  "Zombie_dolphinrider.reanim",               0 },
	{ ReanimationType::REANIM_POGO,                                 "Zombie_pogo.reanim",                       0 },
	{ ReanimationType::REANIM_BACKUP_DANCER,                        "Zombie_backup.reanim",                     0 }, // @Patoke: GOTY has different reanim name
	{ ReanimationType::REANIM_BOBSLED,                              "Zombie_bobsled.reanim",                    0 },
	{ ReanimationType::REANIM_JACKINTHEBOX,                         "Zombie_jackbox.reanim",                    0 },
	{ ReanimationType::REANIM_SNORKEL,                              "Zombie_snorkle.reanim",                    0 },
	{ ReanimationType::REANIM_BUNGEE,                               "Zombie_bungi.reanim",                      0 },
	{ ReanimationType::REANIM_CATAPULT,                             "Zombie_catapult.reanim",                   0 },
	{ ReanimationType::REANIM_LADDER,                               "Zombie_ladder.reanim",                     0 },
	{ ReanimationType::REANIM_PUFF,                                 "Puff.reanim",                              0 },
	{ ReanimationType::REANIM_SLEEPING,                             "Z.reanim",                                 0 },
	{ ReanimationType::REANIM_GRAVE_BUSTER,                         "Gravebuster.reanim",                       0 },
	{ ReanimationType::REANIM_ZOMBIES_WON,                          "ZombiesWon.reanim",                        1 },
	{ ReanimationType::REANIM_MAGNETSHROOM,                         "Magnetshroom.reanim",                      0 },
	{ ReanimationType::REANIM_BOSS,                                 "Zombie_boss.reanim",                       0 },
	{ ReanimationType::REANIM_CABBAGEPULT,                          "Cabbagepult.reanim",                       0 },
	{ ReanimationType::REANIM_KERNELPULT,                           "Cornpult.reanim",                          0 },
	{ ReanimationType::REANIM_MELONPULT,                            "Melonpult.reanim",                         0 },
	{ ReanimationType::REANIM_COFFEEBEAN,                           "Coffeebean.reanim",                        1 },
	{ ReanimationType::REANIM_UMBRELLALEAF,                         "Umbrellaleaf.reanim",                      0 },
	{ ReanimationType::REANIM_GATLINGPEA,                           "GatlingPea.reanim",                        0 },
	{ ReanimationType::REANIM_CATTAIL,                              "Cattail.reanim",                           0 },
	{ ReanimationType::REANIM_GLOOMSHROOM,                          "GloomShroom.reanim",                       0 },
	{ ReanimationType::REANIM_BOSS_ICEBALL,                         "Zombie_boss_iceball.reanim",               1 },
	{ ReanimationType::REANIM_BOSS_FIREBALL,                        "Zombie_boss_fireball.reanim",              1 },
	{ ReanimationType::REANIM_COBCANNON,                            "CobCannon.reanim",                         0 },
	{ ReanimationType::REANIM_GARLIC,                               "Garlic.reanim",                            0 },
	{ ReanimationType::REANIM_GOLD_MAGNET,                          "GoldMagnet.reanim",                        0 },
	{ ReanimationType::REANIM_WINTER_MELON,                         "WinterMelon.reanim",                       0 },
	{ ReanimationType::REANIM_TWIN_SUNFLOWER,                       "TwinSunflower.reanim",                     0 },
	{ ReanimationType::REANIM_POOL_CLEANER,                         "PoolCleaner.reanim",                       0 },
	{ ReanimationType::REANIM_ROOF_CLEANER,                         "RoofCleaner.reanim",                       0 },
	{ ReanimationType::REANIM_FIRE_PEA,                             "FirePea.reanim",                           0 },
	{ ReanimationType::REANIM_IMITATER,                             "Imitater.reanim",                          0 },
	{ ReanimationType::REANIM_YETI,                                 "Zombie_yeti.reanim",                       0 },
	{ ReanimationType::REANIM_BOSS_DRIVER,                          "Zombie_Boss_driver.reanim",                0 },
	{ ReanimationType::REANIM_LAWN_MOWERED_ZOMBIE,                  "LawnMoweredZombie.reanim",                 0 },
	{ ReanimationType::REANIM_CRAZY_DAVE,                           "CrazyDave.reanim",                         1 },
	{ ReanimationType::REANIM_TEXT_FADE_ON,                         "TextFadeOn.reanim",                        0 },
	{ ReanimationType::REANIM_HAMMER,                               "Hammer.reanim",                            0 },
	{ ReanimationType::REANIM_SLOT_MACHINE_HANDLE,                  "SlotMachine.reanim",                       0 },
	{ ReanimationType::REANIM_CREDITS_FOOTBALL,                     "Credits_Football.reanim",                  1 },
	{ ReanimationType::REANIM_CREDITS_JACKBOX,                      "Credits_Jackbox.reanim",                   1 },
	{ ReanimationType::REANIM_SELECTOR_SCREEN,                      "SelectorScreen.reanim",                    3 },
	{ ReanimationType::REANIM_PORTAL_CIRCLE,                        "Portal_Circle.reanim",                     0 },
	{ ReanimationType::REANIM_PORTAL_SQUARE,                        "Portal_Square.reanim",                     0 },
	{ ReanimationType::REANIM_ZENGARDEN_SPROUT,                     "ZenGarden_sprout.reanim",                  0 },
	{ ReanimationType::REANIM_ZENGARDEN_WATERINGCAN,                "ZenGarden_wateringcan.reanim",             1 },
	{ ReanimationType::REANIM_ZENGARDEN_FERTILIZER,                 "ZenGarden_fertilizer.reanim",              1 },
	{ ReanimationType::REANIM_ZENGARDEN_BUGSPRAY,                   "ZenGarden_bugspray.reanim",                1 },
	{ ReanimationType::REANIM_ZENGARDEN_PHONOGRAPH,                 "ZenGarden_phonograph.reanim",              1 },
	{ ReanimationType::REANIM_DIAMOND,                              "Diamond.reanim",                           0 },
	{ ReanimationType::REANIM_ZOMBIE_HAND,                          "Zombie_hand.reanim",                       1 },
	{ ReanimationType::REANIM_STINKY,                               "Stinky.reanim",                            0 },
	{ ReanimationType::REANIM_RAKE,                                 "Rake.reanim",                              0 },
	{ ReanimationType::REANIM_RAIN_CIRCLE,                          "Rain_circle.reanim",                       0 },
	{ ReanimationType::REANIM_RAIN_SPLASH,                          "Rain_splash.reanim",                       0 },
	{ ReanimationType::REANIM_ZOMBIE_SURPRISE,                      "Zombie_surprise.reanim",                   0 },
	{ ReanimationType::REANIM_COIN_GOLD,                            "Coin_gold.reanim",                         0 },
	{ ReanimationType::REANIM_TREEOFWISDOM,                         "TreeOfWisdom.reanim",                      1 },
	{ ReanimationType::REANIM_TREEOFWISDOM_CLOUDS,                  "TreeOfWisdomClouds.reanim",                1 },
	{ ReanimationType::REANIM_TREEOFWISDOM_TREEFOOD,                "TreeFood.reanim",                          1 },
	{ ReanimationType::REANIM_CREDITS_MAIN,                         "Credits_Main.reanim",                      3 },
	{ ReanimationType::REANIM_CREDITS_MAIN2,                        "Credits_Main2.reanim",                     3 },
	{ ReanimationType::REANIM_CREDITS_MAIN3,                        "Credits_Main3.reanim",                     3 },
	{ ReanimationType::REANIM_ZOMBIE_CREDITS_DANCE,                 "Zombie_credits_dance.reanim",              0 },
	{ ReanimationType::REANIM_CREDITS_STAGE,                        "Credits_stage.reanim",                     1 },
	{ ReanimationType::REANIM_CREDITS_BIGBRAIN,                     "Credits_BigBrain.reanim",                  1 },
	{ ReanimationType::REANIM_CREDITS_FLOWER_PETALS,                "Credits_Flower_petals.reanim",             1 },
	{ ReanimationType::REANIM_CREDITS_INFANTRY,                     "Credits_Infantry.reanim",                  1 },
	{ ReanimationType::REANIM_CREDITS_THROAT,                       "Credits_Throat.reanim",                    1 },
	{ ReanimationType::REANIM_CREDITS_CRAZYDAVE,                    "Credits_CrazyDave.reanim",                 1 },
	{ ReanimationType::REANIM_CREDITS_BOSSDANCE,                    "Credits_Bossdance.reanim",                 1 },
	{ ReanimationType::REANIM_ZOMBIE_CREDITS_SCREEN_DOOR,           "Zombie_Credits_Screendoor.reanim",         1 },
	{ ReanimationType::REANIM_ZOMBIE_CREDITS_CONEHEAD,              "Zombie_Credits_Conehead.reanim",           1 },
	{ ReanimationType::REANIM_CREDITS_ZOMBIEARMY1,                  "Credits_ZombieArmy1.reanim",               1 },
	{ ReanimationType::REANIM_CREDITS_ZOMBIEARMY2,                  "Credits_ZombieArmy2.reanim",               1 },
	{ ReanimationType::REANIM_CREDITS_TOMBSTONES,                   "Credits_Tombstones.reanim",                1 },
	{ ReanimationType::REANIM_CREDITS_SOLARPOWER,                   "Credits_SolarPower.reanim",                1 },
	{ ReanimationType::REANIM_CREDITS_ANYHOUR,                      "Credits_Anyhour.reanim",                   3 },
	{ ReanimationType::REANIM_CREDITS_WEARETHEUNDEAD,               "Credits_WeAreTheUndead.reanim",            1 },
	{ ReanimationType::REANIM_CREDITS_DISCOLIGHTS,                  "Credits_DiscoLights.reanim",               1 },
	{ ReanimationType::REANIM_FLAG,                                 "Zombie_FlagPole.reanim",                   0 },
};

//0x471540
ReanimatorTransform::ReanimatorTransform() :
	mTransX(DEFAULT_FIELD_PLACEHOLDER),
	mTransY(DEFAULT_FIELD_PLACEHOLDER),
	mSkewX(DEFAULT_FIELD_PLACEHOLDER),
	mSkewY(DEFAULT_FIELD_PLACEHOLDER),
	mScaleX(DEFAULT_FIELD_PLACEHOLDER),
	mScaleY(DEFAULT_FIELD_PLACEHOLDER),
	mFrame(DEFAULT_FIELD_PLACEHOLDER),
	mAlpha(DEFAULT_FIELD_PLACEHOLDER),
	mImage(nullptr),
	mFont(nullptr),
	mText("") { }

inline void ReanimationFillInMissingData(float& thePrev, float& theValue)
{
	if (theValue == DEFAULT_FIELD_PLACEHOLDER)
		theValue = thePrev;  // 若当前帧上的值未设定，则以前一帧的数值赋值当前帧
	else
		thePrev = theValue;  // 否则，将当前帧的数据记录为“前一帧的数据”
}

inline void ReanimationFillInMissingData(void*& thePrev, void*& theValue)
{
	if (theValue == nullptr)
		theValue = thePrev;
	else
		thePrev = theValue;
}

//0x4715F0 : (*def, eax = string& fileName)  //esp -= 0x4
bool ReanimationLoadDefinition(const SexyString& theFileName, ReanimatorDefinition* theDefinition)
{
	if (!DefinitionLoadXML(theFileName, &gReanimatorDefMap, theDefinition))
		return false;

	for (int aTrackIndex = 0; aTrackIndex < theDefinition->mTracks.count; aTrackIndex++)
	{
		ReanimatorTrack* aTrack = &theDefinition->mTracks.tracks[aTrackIndex];
		float aPrevTransX = 0.0f;
		float aPrevTransY = 0.0f;
		float aPrevSkewX = 0.0f;
		float aPrevSkewY = 0.0f;
		float aPrevScaleX = 1.0f;
		float aPrevScaleY = 1.0f;
		float aPrevFrame = 0.0f;
		float aPrevAlpha = 1.0f;
		Image* aPrevImage = nullptr;
		_Font* aPrevFont = nullptr;
		const char* aPrevText = "";

		// 遍历每一帧，依次用前一帧的数据填充后一帧的未定义数据，并重新记录前一帧的数据
		for (int i = 0; i < aTrack->mTransforms.count; i++)
		{
			ReanimatorTransform& aTransform = aTrack->mTransforms.mTransforms[i];
			ReanimationFillInMissingData(aPrevTransX, aTransform.mTransX);
			ReanimationFillInMissingData(aPrevTransY, aTransform.mTransY);
			ReanimationFillInMissingData(aPrevSkewX, aTransform.mSkewX);
			ReanimationFillInMissingData(aPrevSkewY, aTransform.mSkewY);
			ReanimationFillInMissingData(aPrevScaleX, aTransform.mScaleX);
			ReanimationFillInMissingData(aPrevScaleY, aTransform.mScaleY);
			ReanimationFillInMissingData(aPrevFrame, aTransform.mFrame);
			ReanimationFillInMissingData(aPrevAlpha, aTransform.mAlpha);
			ReanimationFillInMissingData((void*&)aPrevImage, (void*&)aTransform.mImage);
			ReanimationFillInMissingData((void*&)aPrevFont, (void*&)aTransform.mFont);
			if (*aTransform.mText == '\0')
				aTransform.mText = aPrevText;
			else
				aPrevText = aTransform.mText;
		}
	}
	return true;
}

//0x4717D0
void ReanimationFreeDefinition(ReanimatorDefinition* theDefinition)
{
	// 释放 Atlas
	if (theDefinition->mReanimAtlas != nullptr)
	{
		theDefinition->mReanimAtlas->ReanimAtlasDispose();
		delete theDefinition->mReanimAtlas;
		theDefinition->mReanimAtlas = nullptr;
	}

	// 恢复定义数据
	for (int aTrackIndex = 0; aTrackIndex < theDefinition->mTracks.count; aTrackIndex++)
	{
		ReanimatorTrack* aTrack = &theDefinition->mTracks.tracks[aTrackIndex];
		const char* aPrevText = nullptr;
		for (int i = 0; i < aTrack->mTransforms.count; i++)
		{
			ReanimatorTransform& aTransform = aTrack->mTransforms.mTransforms[i];
			if (*aTransform.mText != '\0' && aTransform.mText == aPrevText)
				aTransform.mText = "";
			else
				aPrevText = aTransform.mText;
		}
	}

	// 释放定义
	DefinitionFreeMap(&gReanimatorDefMap, theDefinition);
}

//0x471890
ReanimatorTrackInstance::ReanimatorTrackInstance()
{
	mBlendCounter = 0;
	mBlendTime = 0;
	mShakeOverride = 0.0f;
	mShakeX = 0.0f;
	mShakeY = 0.0f;
	mAttachmentID = AttachmentID::ATTACHMENTID_NULL;
	mRenderGroup = RENDER_GROUP_NORMAL;
	mIgnoreClipRect = false;
	mImageOverride = nullptr;
	mTruncateDisappearingFrames = true;
	mTrackColor = Color::White;
	mIgnoreColorOverride = false;
	mIgnoreExtraAdditiveColor = false;
}

//0x471920
Reanimation::Reanimation()
{
	mAnimTime = 0;
	mAnimRate = 12.0f;
	mDefinition = nullptr;
	mLoopType = ReanimLoopType::REANIM_PLAY_ONCE;
	mLastFrameTime = -1.0f;
	mDead = false;
	mFrameStart = 0;
	mFrameCount = 0;
	mFrameBasePose = -1;
	mOverlayMatrix.LoadIdentity();
	mColorOverride = Color::White;
	mExtraAdditiveColor = Color::White;
	mEnableExtraAdditiveDraw = false;
	mExtraOverlayColor = Color::White;
	mEnableExtraOverlayDraw = false;
	mLoopCount = 0;
	mIsAttachment = false;
	mRenderOrder = 0;
	mReanimationHolder = nullptr;
	mTrackInstances = nullptr;
	mFilterEffect = FilterEffect::FILTER_EFFECT_NONE;
	mReanimationType = ReanimationType::REANIM_NONE;
}

//0x471A20
Reanimation::~Reanimation()
{
	ReanimationDie();
	ReanimationDelete();
}

void Reanimation::ReanimationDelete()
{
	TOD_ASSERT(mDead);
	if (mTrackInstances != nullptr)
	{
		int aItemSize = mDefinition->mTracks.count * sizeof(ReanimatorTrackInstance);
		FindGlobalAllocator(aItemSize)->Free(mTrackInstances, aItemSize);  // 由 TodAllocator 回收动画轨道的内存区域
		mTrackInstances = nullptr;
	}
}

//0x471A60
void Reanimation::ReanimationInitializeType(float theX, float theY, ReanimationType theReanimType)
{
	TOD_ASSERT(theReanimType >= 0 && theReanimType < gReanimatorDefCount);
	ReanimatorEnsureDefinitionLoaded(theReanimType, false);
	mReanimationType = theReanimType;
	ReanimationInitialize(theX, theY, &gReanimatorDefArray[(int)theReanimType]);
}

//0x471A90
void ReanimationCreateAtlas(ReanimatorDefinition* theDefinition, ReanimationType theReanimationType)
{
	ReanimationParams& aParam = gReanimationParamArray[(int)theReanimationType];
	if (theDefinition->mReanimAtlas != nullptr || TestBit(aParam.mReanimParamFlags, ReanimFlags::REANIM_NO_ATLAS))
		return;  // 当动画已存在 Atlas 或无需 Atlas 时，直接退出

	auto aTimer = std::chrono::high_resolution_clock::now();

	TodHesitationTrace("preatlas");
	ReanimAtlas* aAtlas = new ReanimAtlas();
	theDefinition->mReanimAtlas = aAtlas;  // 赋值动画 Atlas 指针
	aAtlas->ReanimAtlasCreate(theDefinition);

	TodHesitationTrace("atlas '%s'", aParam.mReanimFileName);
	//int aDuration = std::max(aTimer.GetDuration(), 0.0);
	int aDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - aTimer).count();
	if (aDuration > 20 && theReanimationType != ReanimationType::REANIM_NONE)  //（仅内测版）创建时间过长的报告
		TodTraceAndLog("loading:  Long atlas '%s' %d ms on %s", aParam.mReanimFileName, aDuration, gGetCurrentLevelName().c_str());
}

void ReanimationPreload(ReanimationType theReanimationType)
{
	TOD_ASSERT(theReanimationType >= 0 && theReanimationType < gReanimatorDefCount);

	ReanimatorDefinition* aReanimDef = &gReanimatorDefArray[(int)theReanimationType];
	ReanimationCreateAtlas(aReanimDef, theReanimationType);
	if (aReanimDef->mReanimAtlas)
	{
		unreachable();
		//TodSandImageIfNeeded(aReanimDef->mReanimAtlas->mMemoryImage);
	}
}

//0x471B00
void Reanimation::ReanimationInitialize(float theX, float theY, ReanimatorDefinition* theDefinition)
{
	TOD_ASSERT(mTrackInstances == nullptr);
	ReanimationCreateAtlas(theDefinition, mReanimationType);
	mDead = false;
	SetPosition(theX, theY);
	mDefinition = theDefinition;
	mAnimRate = theDefinition->mFPS;
	mLastFrameTime = -1.0f;

	if (theDefinition->mTracks.count != 0)
	{
		mFrameCount = mDefinition->mTracks.tracks[0].mTransforms.count;
		int aItemSize = theDefinition->mTracks.count * sizeof(ReanimatorTrackInstance);
		mTrackInstances = (ReanimatorTrackInstance*)FindGlobalAllocator(aItemSize)->Calloc(aItemSize);  // 申请动画轨道实例数组所需的内存
		for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTracks.count; aTrackIndex++)  // 遍历初始化数组中每个轨道实例
		{
			ReanimatorTrackInstance* aTrack = &mTrackInstances[aTrackIndex];
			if (aTrack != nullptr)
				new (aTrack)ReanimatorTrackInstance();
		}
	}
	else
		mFrameCount = 0;
}

//0x471BC0
// GOTY @Patoke: 0x4761C0
void Reanimation::Update()
{
	if (mFrameCount == 0 || mDead)
		return;

	TOD_ASSERT(std::isfinite(mAnimRate));
	mLastFrameTime = mAnimTime;  // 更新上一帧的循环率
	mAnimTime += SECONDS_PER_UPDATE * mAnimRate / mFrameCount;  // 更新当前循环率

	if (mAnimRate > 0)
	{
		switch (mLoopType)
		{
		case ReanimLoopType::REANIM_LOOP:
		case ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME:
			while (mAnimTime >= 1.0f)
			{
				mLoopCount++;
				mAnimTime -= 1.0f;
			}
			break;
		case ReanimLoopType::REANIM_PLAY_ONCE:
		case ReanimLoopType::REANIM_PLAY_ONCE_FULL_LAST_FRAME:
			if (mAnimTime >= 1.0f)
			{
				mLoopCount = 1;
				mAnimTime = 1.0f;
				mDead = true;
			}
			break;
		case ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD:
		case ReanimLoopType::REANIM_PLAY_ONCE_FULL_LAST_FRAME_AND_HOLD:
			if (mAnimTime >= 1.0f)
			{
				mLoopCount = 1;
				mAnimTime = 1.0f;
			}
			break;
		default:
			TOD_ASSERT(false);
			break;
		}
	}
	else
	{
		switch (mLoopType)
		{
		case ReanimLoopType::REANIM_LOOP:
		case ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME:
			while (mAnimTime < 0.0f)
			{
				mLoopCount++;
				mAnimTime += 1.0f;
			}
			break;
		case ReanimLoopType::REANIM_PLAY_ONCE:
		case ReanimLoopType::REANIM_PLAY_ONCE_FULL_LAST_FRAME:
			if (mAnimTime < 0.0f)
			{
				mLoopCount = 1;
				mAnimTime = 0.0f;
				mDead = true;
			}
			break;
		case ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD:
		case ReanimLoopType::REANIM_PLAY_ONCE_FULL_LAST_FRAME_AND_HOLD:
			if (mAnimTime < 0.0f)
			{
				mLoopCount = 1;
				mAnimTime = 0.0f;
			}
			break;
		default:
			TOD_ASSERT(false);
			break;
		}
	}

	for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTracks.count; aTrackIndex++)
	{
		ReanimatorTrackInstance* aTrack = &mTrackInstances[aTrackIndex];
		if (aTrack->mBlendCounter > 0)
			aTrack->mBlendCounter--;  // 更新轨道的混合倒计时

		if (aTrack->mShakeOverride != 0.0f)  // 更新轨道震动
		{
			aTrack->mShakeX = RandRangeFloat(-aTrack->mShakeOverride, aTrack->mShakeOverride);
			aTrack->mShakeY = RandRangeFloat(-aTrack->mShakeOverride, aTrack->mShakeOverride);
		}

		if (strncasecmp(mDefinition->mTracks.tracks[aTrackIndex].mName, "attacher__", 10) == 0)  // IsAttacher
			UpdateAttacherTrack(aTrackIndex);

		if (aTrack->mAttachmentID != AttachmentID::ATTACHMENTID_NULL)
		{
			SexyTransform2D aOverlayMatrix;
			GetAttachmentOverlayMatrix(aTrackIndex, aOverlayMatrix);
			AttachmentUpdateAndSetMatrix(aTrack->mAttachmentID, aOverlayMatrix);
		}
	}
}

//0x471E50
void BlendTransform(ReanimatorTransform* theResult, const ReanimatorTransform& theTransform1, const ReanimatorTransform& theTransform2, float theBlendFactor)
{
	theResult->mTransX = FloatLerp(theTransform1.mTransX, theTransform2.mTransX, theBlendFactor);
	theResult->mTransY = FloatLerp(theTransform1.mTransY, theTransform2.mTransY, theBlendFactor);
	theResult->mScaleX = FloatLerp(theTransform1.mScaleX, theTransform2.mScaleX, theBlendFactor);
	theResult->mScaleY = FloatLerp(theTransform1.mScaleY, theTransform2.mScaleY, theBlendFactor);
	theResult->mAlpha = FloatLerp(theTransform1.mAlpha, theTransform2.mAlpha, theBlendFactor);

	float aSkewX2 = theTransform2.mSkewX;
	float aSkewY2 = theTransform2.mSkewY;
	// 推测这里原意是为了确保从两个变换之间的倾斜角度不超过 π（WP 版），
	// 原版（以及内测版）实际为，倾斜超过 π 时 theTransform2 变换无效
	while (aSkewX2 > theTransform1.mSkewX + 180.0f)
		aSkewX2 = theTransform1.mSkewX;  // （aSkewX2 -= 360.0f）
	while (aSkewX2 < theTransform1.mSkewX - 180.0f)
		aSkewX2 = theTransform1.mSkewX;  // （aSkewX2 += 360.0f）
	while (aSkewY2 > theTransform1.mSkewY + 180.0f)
		aSkewY2 = theTransform1.mSkewY;  // （aSkewY2 -= 360.0f）
	while (aSkewY2 < theTransform1.mSkewY - 180.0f)
		aSkewY2 = theTransform1.mSkewY;  // （aSkewY2 += 360.0f）

	theResult->mSkewX = FloatLerp(theTransform1.mSkewX, aSkewX2, theBlendFactor);
	theResult->mSkewY = FloatLerp(theTransform1.mSkewY, aSkewY2, theBlendFactor);
	theResult->mFrame = theTransform1.mFrame;
	theResult->mFont = theTransform1.mFont;
	theResult->mText = theTransform1.mText;
	theResult->mImage = theTransform1.mImage;
}

//0x471F90
// GOTY @Patoke: 0x476580
void Reanimation::GetCurrentTransform(int theTrackIndex, ReanimatorTransform* theTransformCurrent)
{
	ReanimatorFrameTime aFrameTime;
	GetFrameTime(&aFrameTime);
	GetTransformAtTime(theTrackIndex, theTransformCurrent, &aFrameTime);  // 结合两帧之间的自然补间取得基础变换
	
	ReanimatorTrackInstance* aTrack = &mTrackInstances[theTrackIndex];
	if (FloatRoundToInt(theTransformCurrent->mFrame) >= 0 && aTrack->mBlendCounter > 0)  // 若当前不为空白帧且轨道处于变换混合过程中
	{
		float aBlendFactor = aTrack->mBlendCounter / (float)aTrack->mBlendTime;
		BlendTransform(theTransformCurrent, *theTransformCurrent, aTrack->mBlendTransform, aBlendFactor);  // 结合覆写变换计算混合后的实际变换
	}
}

//0x472020
void Reanimation::GetTransformAtTime(int theTrackIndex, ReanimatorTransform* theTransform, ReanimatorFrameTime* theFrameTime)
{
	TOD_ASSERT(theTrackIndex >= 0 && theTrackIndex < mDefinition->mTracks.count);
	ReanimatorTrack* aTrack = &mDefinition->mTracks.tracks[theTrackIndex];
	TOD_ASSERT(aTrack->mTransforms.count == mDefinition->mTracks.tracks[0].mTransforms.count);
	ReanimatorTransform& aTransBefore = aTrack->mTransforms.mTransforms[theFrameTime->mAnimFrameBeforeInt]; // 前一帧的变换定义
	ReanimatorTransform& aTransAfter  = aTrack->mTransforms.mTransforms[theFrameTime->mAnimFrameAfterInt];  // 后一帧的变换定义

	auto theFactor = theFrameTime->mFraction;
	theTransform->mTransX = FloatLerp(aTransBefore.mTransX, aTransAfter.mTransX, theFactor);
	theTransform->mTransY = FloatLerp(aTransBefore.mTransY, aTransAfter.mTransY, theFactor);
	theTransform->mSkewX  = FloatLerp(aTransBefore.mSkewX,  aTransAfter.mSkewX,  theFactor);
	theTransform->mSkewY  = FloatLerp(aTransBefore.mSkewY,  aTransAfter.mSkewY,  theFactor);
	theTransform->mScaleX = FloatLerp(aTransBefore.mScaleX, aTransAfter.mScaleX, theFactor);
	theTransform->mScaleY = FloatLerp(aTransBefore.mScaleY, aTransAfter.mScaleY, theFactor);
	theTransform->mAlpha  = FloatLerp(aTransBefore.mAlpha,  aTransAfter.mAlpha,  theFactor);
	theTransform->mImage = aTransBefore.mImage;
	theTransform->mFont = aTransBefore.mFont;
	theTransform->mText = aTransBefore.mText;

	if (aTransBefore.mFrame != -1.0f && aTransAfter.mFrame == -1.0f && theFrameTime->mFraction > 0.0f && mTrackInstances[theTrackIndex].mTruncateDisappearingFrames)
		theTransform->mFrame = -1.0f;  // 当从一个非空白帧过渡至空白帧时，若轨道设置了截断消失帧，则删去过渡的过程
	else
		theTransform->mFrame = aTransBefore.mFrame;
}

//0x4720F0
void Reanimation::MatrixFromTransform(const ReanimatorTransform& theTransform, SexyMatrix3& theMatrix)
{
	// 将倾斜的角度转化为弧度
	float aSkewX = -DEG_TO_RAD(theTransform.mSkewX);
	float aSkewY = -DEG_TO_RAD(theTransform.mSkewY);

	theMatrix.m00 = cos(aSkewX) * theTransform.mScaleX;
	theMatrix.m10 = -sin(aSkewX) * theTransform.mScaleX;
	theMatrix.m20 = 0.0f;
	theMatrix.m01 = sin(aSkewY) * theTransform.mScaleY;
	theMatrix.m11 = cos(aSkewY) * theTransform.mScaleY;
	theMatrix.m21 = 0.0f;
	theMatrix.m02 = theTransform.mTransX;
	theMatrix.m12 = theTransform.mTransY;
	theMatrix.m22 = 1.0f;
}

//0x472190
void Reanimation::ReanimBltMatrix(Graphics* g, Image* theImage, SexyMatrix3& theTransform, const Rect& theClipRect, const Color& theColor, int theDrawMode, const Rect& theSrcRect)
{/*
	if (!gSexyAppBase->Is3DAccelerated() &&  // 未开启 3D 硬件加速
		TestBit(gReanimationParamArray[(int)mReanimationType].mReanimParamFlags, (int)ReanimFlags::REANIM_FAST_DRAW_IN_SW_MODE) &&  // 动画允许使用软件渲染
		FloatApproxEqual(theTransform.m01, 0.0f) && FloatApproxEqual(theTransform.m10, 0.0f) &&  // 横向和纵向的倾斜值均为 0
		theTransform.m00 > 0.0f && theTransform.m11 > 0.0f &&  // 横向和纵向的拉伸值均大于 0
		theColor == Color::White)
	{
		float aScaleX = theTransform.m00;
		float aScaleY = theTransform.m11;
		int aPosX = FloatRoundToInt(theTransform.m02 - aScaleX * theSrcRect.mWidth * 0.5f);
		int aPosY = FloatRoundToInt(theTransform.m12 - aScaleY * theSrcRect.mHeight * 0.5f);
		int aOldMode = g->GetDrawMode();  // 备份原绘制模式
		g->SetDrawMode(theDrawMode);
		Rect aOldClipRect = g->mClipRect;  // 备份原裁剪矩形
		g->SetClipRect(theClipRect);

		if (FloatApproxEqual(aScaleX, 1.0f) && FloatApproxEqual(aScaleY, 1.0f))  // 如果无拉伸
			g->DrawImage(theImage, aPosX, aPosY, theSrcRect);
		else
		{
			int aWidth = FloatRoundToInt(aScaleX * theSrcRect.mWidth);
			int aHeight = FloatRoundToInt(aScaleY * theSrcRect.mHeight);
			Rect aDestRect(aPosX, aPosY, aWidth, aHeight);
			g->DrawImage(theImage, aDestRect, theSrcRect);
		}

		g->SetDrawMode(aOldMode);  // 还原绘制模式
		g->SetClipRect(aOldClipRect);  // 还原裁剪矩形
	}
	else*/
	TodBltMatrix(g, theImage, theTransform, theClipRect, theColor, theDrawMode, theSrcRect);
}

//0x4723B0
// GOTY @Patoke: 0x4769B0
bool Reanimation::DrawTrack(Graphics* g, int theTrackIndex, int theRenderGroup, TodTriangleGroup* theTriangleGroup)
{
	(void)theRenderGroup;
	ReanimatorTransform aTransform;
	ReanimatorTrackInstance* aTrackInstance = &mTrackInstances[theTrackIndex];  // 目标轨道的指针
	GetCurrentTransform(theTrackIndex, &aTransform);  // 取得当前动画变换
	int aImageFrame = FloatRoundToInt(aTransform.mFrame);  // 图像在贴图中所处的份数
	if (aImageFrame < 0)  // 不存在图像时，返回
		return false;

	Color aColor = aTrackInstance->mTrackColor;
	if (!aTrackInstance->mIgnoreColorOverride)  // 除非轨道无视动画的覆写颜色
	{
		aColor = ColorsMultiply(aColor, mColorOverride);  // 将轨道颜色与动画的覆写颜色进行正片叠底混合
	}
	if (g->GetColorizeImages())  // 若 Graphics 着色
	{
		aColor = ColorsMultiply(aColor, g->GetColor());  // 将颜色再与 Graphics 的颜色进行正片叠底混合
	}
	int aImageAlpha = ClampInt(FloatRoundToInt(aTransform.mAlpha * aColor.mAlpha), 0, 255);
	if (aImageAlpha <= 0)  // 当图像完全透明时，返回
	{
		return false;
	}
	aColor.mAlpha = aImageAlpha;

	Color aExtraAdditiveColor;
	if (mEnableExtraAdditiveDraw)  // 如果动画启用额外叠加颜色（高亮）
	{
		aExtraAdditiveColor = mExtraAdditiveColor;
		aExtraAdditiveColor.mAlpha = ColorComponentMultiply(mExtraAdditiveColor.mAlpha, aImageAlpha);
	}
	Color aExtraOverlayColor;
	if (mEnableExtraOverlayDraw)
	{
		aExtraOverlayColor = mExtraOverlayColor;
		aExtraOverlayColor.mAlpha = ColorComponentMultiply(mExtraOverlayColor.mAlpha, aImageAlpha);
	}

	Rect aClipRect = g->mClipRect;
	if (aTrackInstance->mIgnoreClipRect)  // 如果轨道无视裁剪矩形
	{
		aClipRect = Rect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);  // 裁剪矩形重置为屏幕矩形
	}

	Image* aImage = aTransform.mImage;
	ReanimAtlasImage* aAtlasImage = nullptr;
	if (mDefinition->mReanimAtlas != nullptr && aImage != nullptr)  // 如果 atlas 存在且当前变换存在图像（aTransform.mImage 实际为整数型的图集编号）
	{
		aAtlasImage = mDefinition->mReanimAtlas->GetEncodedReanimAtlas(aImage);  // 取得相应的图集数据
		if (aAtlasImage != nullptr)  // 如果是合法的图集编号，成功取得对应指针
		{
			aImage = aAtlasImage->mOriginalImage;  // 将真正的 Sexy::Image* 类型的贴图赋值给 aImage
		}
		if (aTrackInstance->mImageOverride != nullptr)  // 如果目标轨道存在覆写贴图
		{
			aAtlasImage = nullptr;  // 不使用图集
		}
	}
	SexyMatrix3 aMatrix;
	bool aFullScreen = false;
	if (aImage != nullptr)  // 如果存在贴图。此处若上一步中图集编号非法，则可能导致崩溃
	{
		int aCelWidth = aImage->GetCelWidth();
		int aCelHeight = aImage->GetCelHeight();
		aMatrix.LoadIdentity();
		SexyMatrix3Translation(aMatrix, aCelWidth * 0.5f, aCelHeight * 0.5f);  // 将矩阵变换的坐标设定在贴图的中心位置
	}
	else if (aTransform.mFont != nullptr && *aTransform.mText != '\0')  // 如果存在字体且文本不为空
	{
		aMatrix.LoadIdentity();
		int aWidth = aTransform.mFont->StringWidth(aTransform.mText);
		SexyMatrix3Translation(aMatrix, -aWidth * 0.5f, aTransform.mFont->mAscent);
	}
	else
	{
		if (strcasecmp(mDefinition->mTracks.tracks[theTrackIndex].mName, "fullscreen"))  // 如果既没有图像也没有文本，且不是全屏轨道
			return false;  // 无需绘制
		aFullScreen = true;  // 标记全屏轨道，后续会填充一个屏幕大小的矩形
	}

	if (mDefinition->mReanimAtlas != nullptr && aAtlasImage == nullptr)  // 有 atlas 但不用的情况
		theTriangleGroup->DrawGroup(g);  // 先把原有的三角组绘制了

	SexyMatrix3 aTransformMatrix;
	MatrixFromTransform(aTransform, aTransformMatrix);
	SexyMatrix3Multiply(aMatrix, aTransformMatrix, aMatrix);  // 以动画变换矩阵作用 aMatrix
	SexyMatrix3Multiply(aMatrix, mOverlayMatrix, aMatrix);  // 以动画覆写矩阵作用 aMatrix
	SexyMatrix3Translation(aMatrix, aTrackInstance->mShakeX + g->mTransX - 0.5f, aTrackInstance->mShakeY + g->mTransY - 0.5f);  // 轨道震动及 g 的影响

	if (aAtlasImage != nullptr)  // 如果存在图集（动画定义存在 atlas，轨道变换存在图像，轨道不存在覆写贴图）
	{
		Rect aSrcRect(aAtlasImage->mX, aAtlasImage->mY, aAtlasImage->mWidth, aAtlasImage->mHeight);
		
		aImage = mDefinition->mReanimAtlas->mMemoryImage.get();
		if (mFilterEffect != FilterEffect::FILTER_EFFECT_NONE)  // 如果动画存在滤镜
		{
			aImage = FilterEffectGetImage(aImage, mFilterEffect);  // 取得滤镜后的贴图
		}
		theTriangleGroup->AddTriangle(g, aImage, aMatrix, aClipRect, aColor, g->mDrawMode, aSrcRect);  // 向三角组中添加三角形
		if (mEnableExtraAdditiveDraw && !aTrackInstance->mIgnoreExtraAdditiveColor)  // 如果动画存在额外叠加颜色且轨道不能无视之
		{
			theTriangleGroup->AddTriangle(g, aImage, aMatrix, aClipRect, aExtraAdditiveColor, Graphics::DRAWMODE_ADDITIVE, aSrcRect);
		}
		if (mEnableExtraOverlayDraw)
		{
			theTriangleGroup->AddTriangle(
				g, FilterEffectGetImage(aImage, FilterEffect::FILTER_EFFECT_WHITE), aMatrix, aClipRect, aExtraOverlayColor, Graphics::DRAWMODE_NORMAL, aSrcRect);
		}
	}
	else if (aImage != nullptr)  // 如果不存在 atlas 但轨道变换存在图像
	{
		if (aTrackInstance->mImageOverride != nullptr)  // 如果轨道存在覆写贴图
		{
			aImage = aTrackInstance->mImageOverride;  // 将贴图替换为覆写贴图
		}
		if (mFilterEffect != FilterEffect::FILTER_EFFECT_NONE)  // 如果动画存在滤镜
		{
			aImage = FilterEffectGetImage(aImage, mFilterEffect);  // 将贴图替换为滤镜后的贴图
		}
		while (aImageFrame >= aImage->mNumCols)
		{
			aImageFrame -= aImage->mNumCols;  // 确保绘制的列数不会超过贴图最后一列
		}

		int aCelWidth = aImage->GetCelWidth();
		Rect aSrcRect(aImageFrame * aCelWidth, 0, aCelWidth, aImage->GetCelHeight());
		ReanimBltMatrix(g, aImage, aMatrix, aClipRect, aColor, g->mDrawMode, aSrcRect);  // 带矩阵绘制轨道图像
		if (mEnableExtraAdditiveDraw)
		{
			ReanimBltMatrix(g, aImage, aMatrix, aClipRect, aExtraAdditiveColor, Graphics::DRAWMODE_ADDITIVE, aSrcRect);
		}
		if (mEnableExtraOverlayDraw)
		{
			Image* aOverlayImage = FilterEffectGetImage(aImage, FilterEffect::FILTER_EFFECT_WHITE);
			ReanimBltMatrix(g, aOverlayImage, aMatrix, aClipRect, aExtraOverlayColor, Graphics::DRAWMODE_NORMAL, aSrcRect);
		}
	}
	else if (aTransform.mFont != nullptr && *aTransform.mText != '\0')  // 如果不存在图像但存在文本
	{
		TodDrawStringMatrix(g, aTransform.mFont, aMatrix, aTransform.mText, aColor);
		if (mEnableExtraAdditiveDraw)
		{
			int aOldMode = g->GetDrawMode();  // 备份绘制模式
			g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
			TodDrawStringMatrix(g, aTransform.mFont, aMatrix, aTransform.mText, aExtraAdditiveColor);
			g->SetDrawMode(aOldMode);  // 还原绘制模式
		}
	}
	else if (aFullScreen)  // 不存在图像和文本，但是全屏
	{
		Color aOldColor = g->GetColor();  // 备份颜色
		g->SetColor(aColor);
		g->FillRect(-g->mTransX, -g->mTransY, BOARD_WIDTH, BOARD_HEIGHT);
		g->SetColor(aOldColor);  // 还原颜色
	}
	return true;
}

//0x472B70
Image* Reanimation::GetCurrentTrackImage(const char* theTrackName)
{
	int aTrackIndex = FindTrackIndex(theTrackName);
	ReanimatorTransform aTransform;
	GetCurrentTransform(aTrackIndex, &aTransform);

	Image* aImage = aTransform.mImage;
	if (mDefinition->mReanimAtlas != nullptr && aImage != nullptr)  // 如果存在图集且存在图像（否则返回的 aImage 为 nullptr）
	{
		ReanimAtlasImage* aAtlasImage = mDefinition->mReanimAtlas->GetEncodedReanimAtlas(aImage);  // 取得相应的图集数据
		if (aAtlasImage != nullptr)
			aImage = aAtlasImage->mOriginalImage;  // 返回图集对应的原贴图
	}
	return aImage;
}

//0x472C00
void Reanimation::GetTrackMatrix(int theTrackIndex, SexyTransform2D& theMatrix)
{
	ReanimatorTrackInstance* aTrackInstance = &mTrackInstances[theTrackIndex];
	ReanimatorTransform aTransform;
	GetCurrentTransform(theTrackIndex, &aTransform);
	int aImageFrame = FloatRoundToInt(aTransform.mFrame);
	Image* aImage = aTransform.mImage;
	if (mDefinition->mReanimAtlas != nullptr && aImage != nullptr)  // 如果存在图集且存在图像（否则返回的 aImage 为 nullptr）
	{
		ReanimAtlasImage* aAtlasImage = mDefinition->mReanimAtlas->GetEncodedReanimAtlas(aImage);  // 取得相应的图集数据
		if (aAtlasImage != nullptr)
			aImage = aAtlasImage->mOriginalImage;  // 返回图集对应的原贴图
	}

	theMatrix.LoadIdentity();
	if (aImage != nullptr && aImageFrame >= 0)
	{
		int aCelWidth = aImage->GetCelWidth();
		int aCelHeight = aImage->GetCelHeight();
		SexyMatrix3Translation(theMatrix, aCelWidth * 0.5f, aCelHeight * 0.5f);  // 将矩阵变换的坐标设定在贴图的中心位置
	}
	else if (aTransform.mFont != nullptr && *aTransform.mText != '\0')
		SexyMatrix3Translation(theMatrix, 0.0f, aTransform.mFont->mAscent);

	SexyTransform2D aTransformMatrix;
	MatrixFromTransform(aTransform, aTransformMatrix);
	SexyMatrix3Multiply(theMatrix, aTransformMatrix, theMatrix);  // 以动画变换矩阵作用 theMatrix
	SexyMatrix3Multiply(theMatrix, mOverlayMatrix, theMatrix);  // 以动画覆写矩阵作用 theMatrix
	SexyMatrix3Translation(theMatrix, aTrackInstance->mShakeX - 0.5f, aTrackInstance->mShakeY - 0.5f);  // 轨道震动的影响
}

//0x472D90
void Reanimation::GetFrameTime(ReanimatorFrameTime* theFrameTime)
{
	TOD_ASSERT(mFrameStart + mFrameCount <= mDefinition->mTracks.tracks[0].mTransforms.count);
	int aFrameCount;
	if (mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_FULL_LAST_FRAME || mLoopType == ReanimLoopType::REANIM_LOOP_FULL_LAST_FRAME ||
		mLoopType == ReanimLoopType::REANIM_PLAY_ONCE_FULL_LAST_FRAME_AND_HOLD)
		aFrameCount = mFrameCount;
	else
		aFrameCount = mFrameCount - 1;
	float aAnimPosition = mFrameStart + mAnimTime * aFrameCount;
	float aAnimFrameBefore = floor(aAnimPosition);
	theFrameTime->mFraction = aAnimPosition - aAnimFrameBefore;
	theFrameTime->mAnimFrameBeforeInt = FloatRoundToInt(aAnimFrameBefore);
	if (theFrameTime->mAnimFrameBeforeInt >= mFrameStart + mFrameCount - 1)  // 如果当前处于结束的一帧
	{
		theFrameTime->mAnimFrameBeforeInt = mFrameStart + mFrameCount - 1;
		theFrameTime->mAnimFrameAfterInt = theFrameTime->mAnimFrameBeforeInt;  // 将前、后的整数帧均赋值为最后一帧
	}
	else
		theFrameTime->mAnimFrameAfterInt = theFrameTime->mAnimFrameBeforeInt + 1;  // 后一整数帧等于前一整数帧的后一帧
	TOD_ASSERT(theFrameTime->mAnimFrameBeforeInt >= 0 && theFrameTime->mAnimFrameAfterInt < mDefinition->mTracks.tracks[0].mTransforms.count);
}

//0x472E40
void Reanimation::DrawRenderGroup(Graphics* g, int theRenderGroup)
{
	if (mDead)
		return;

	TodTriangleGroup aTriangleGroup;
	for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTracks.count; aTrackIndex++)
	{
		ReanimatorTrackInstance* aTrackInstance = &mTrackInstances[aTrackIndex];
		if (aTrackInstance->mRenderGroup == theRenderGroup)
		{
			bool aTrackDrawn = DrawTrack(g, aTrackIndex, theRenderGroup, &aTriangleGroup);
			if (aTrackInstance->mAttachmentID != AttachmentID::ATTACHMENTID_NULL)
			{
				aTriangleGroup.DrawGroup(g);
				AttachmentDraw(aTrackInstance->mAttachmentID, g, !aTrackDrawn);
			}
		}
	}
	aTriangleGroup.DrawGroup(g);
}

void Reanimation::Draw(Graphics* g)
{ 
	DrawRenderGroup(g, RENDER_GROUP_NORMAL);
}

//0x472F30
// GOTY @Patoke: 0x477640
int Reanimation::FindTrackIndex(const char* theTrackName)
{
	for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTracks.count; aTrackIndex++)
		if (strcasecmp(mDefinition->mTracks.tracks[aTrackIndex].mName, theTrackName) == 0)
			return aTrackIndex;

	TodTrace("Can't find track '%s'", theTrackName);
	return 0;
}

// GOTY @Patoke: 0x464B18
ReanimatorTrackInstance* Reanimation::GetTrackInstanceByName(const char* theTrackName)
{
	return &mTrackInstances[FindTrackIndex(theTrackName)];
}

//0x472F80
void Reanimation::AttachToAnotherReanimation(Reanimation* theAttachReanim, const char* theTrackName)
{
	if (theAttachReanim->mDefinition->mTracks.count <= 0)
		return;

	if (theAttachReanim->mFrameBasePose == -1)
		theAttachReanim->mFrameBasePose = theAttachReanim->mFrameStart;  // 将当前动作的起始帧作为变换基准帧
	AttachReanim(theAttachReanim->GetTrackInstanceByName(theTrackName)->mAttachmentID, this, 0.0f, 0.0f);
}

void Reanimation::SetBasePoseFromAnim(const char* theTrackName)
{
	int aFrameStart, aFrameCount;
	GetFramesForLayer(theTrackName, aFrameStart, aFrameCount);
	mFrameBasePose = aFrameStart;  // 将当前轨道动画的起始帧作为变换基准帧
}

//0x472FD0
void Reanimation::GetTrackBasePoseMatrix(int theTrackIndex, SexyTransform2D& theBasePosMatrix)
{
	if (mFrameBasePose == NO_BASE_POSE)
	{
		theBasePosMatrix.LoadIdentity();
		return;
	}

	int aBasePos = mFrameBasePose == -1 ? mFrameStart : mFrameBasePose;
	ReanimatorFrameTime aStartTime = { 0.0f, aBasePos, aBasePos + 1 };
	ReanimatorTransform aTransformStart;
	GetTransformAtTime(theTrackIndex, &aTransformStart, &aStartTime);
	MatrixFromTransform(aTransformStart, theBasePosMatrix);
}

//0x473070
AttachEffect* Reanimation::AttachParticleToTrack(const char* theTrackName, TodParticleSystem* theParticleSystem, float thePosX, float thePosY)
{
	int aTrackIndex = FindTrackIndex(theTrackName);
	ReanimatorTrackInstance* aTrackInstance = &mTrackInstances[aTrackIndex];
	SexyTransform2D aBasePoseMatrix;
	GetTrackBasePoseMatrix(aTrackIndex, aBasePoseMatrix);  // 取得轨道基础形态的变换矩阵
	SexyVector2 aPosition = aBasePoseMatrix * SexyVector2(thePosX, thePosY);  // 以基础形态的矩阵变换位置向量
	return AttachParticle(aTrackInstance->mAttachmentID, theParticleSystem, aPosition.x, aPosition.y);
}

//0x473110
// GOTY @Patoke: 0x477810
void Reanimation::GetAttachmentOverlayMatrix(int theTrackIndex, SexyTransform2D& theOverlayMatrix)
{
	ReanimatorTransform aTransform;
	GetCurrentTransform(theTrackIndex, &aTransform);  // 取得含混合、不含覆写的自然变换
	SexyTransform2D aTransformMatrix;
	MatrixFromTransform(aTransform, aTransformMatrix);
	SexyMatrix3Multiply(aTransformMatrix, mOverlayMatrix, aTransformMatrix);  // 以动画覆写矩阵作用于动画变换矩阵

	SexyTransform2D aBasePoseMatrix;
	GetTrackBasePoseMatrix(theTrackIndex, aBasePoseMatrix);  // 取得轨道基础形态的变换矩阵
	SexyTransform2D aBasePoseMatrixInv;
	SexyMatrix3Inverse(aBasePoseMatrix, aBasePoseMatrixInv);  // 取得基础形态矩阵的逆
	theOverlayMatrix = aTransformMatrix * aBasePoseMatrixInv;
}

//0x4731D0
void Reanimation::GetFramesForLayer(const char* theTrackName, int& theFrameStart, int& theFrameCount)
{
	if (mDefinition->mTracks.count == 0)  // 如果动画没有轨道
	{
		theFrameStart = 0;
		theFrameCount = 0;
		return;
	}

	int aTrackIndex = FindTrackIndex(theTrackName);
	TOD_ASSERT(aTrackIndex >= 0 && aTrackIndex < mDefinition->mTracks.count);
	ReanimatorTrack* aTrack = &mDefinition->mTracks.tracks[aTrackIndex];
	theFrameStart = 0;
	theFrameCount = 1;
	for (int i = 0; i < aTrack->mTransforms.count; i++)
		if (aTrack->mTransforms.mTransforms[i].mFrame >= 0.0f)
		{
			theFrameStart = i;  // 取轨道上的首个非空白帧作为起始帧
			break;
		}
	for (int j = theFrameStart; j < aTrack->mTransforms.count; j++)
		if (aTrack->mTransforms.mTransforms[j].mFrame >= 0.0f)
			theFrameCount = j - theFrameStart + 1;  // 取从起始帧至轨道最后一个非空白帧之间为帧数量
}

//0x473280
void Reanimation::SetFramesForLayer(const char* theTrackName)
{
	if (mAnimRate >= 0)
		mAnimTime = 0.0f;
	else
		mAnimTime = 0.9999999f;
	mLastFrameTime = -1.0f;
	GetFramesForLayer(theTrackName, mFrameStart, mFrameCount);
}

//0x4732C0
bool Reanimation::TrackExists(const char* theTrackName)
{
	for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTracks.count; aTrackIndex++)
		if (strcasecmp(mDefinition->mTracks.tracks[aTrackIndex].mName, theTrackName) == 0)
			return true;
	return false;
}

//0x473310
void Reanimation::StartBlend(int theBlendTime)
{
	for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTracks.count; aTrackIndex++)
	{
		ReanimatorTransform aTransform;
		GetCurrentTransform(aTrackIndex, &aTransform);
		if (FloatRoundToInt(aTransform.mFrame) >= 0)  // 若当前轨道当前不处于空白帧
		{
			ReanimatorTrackInstance* aTrackInstance = &mTrackInstances[aTrackIndex];
			aTrackInstance->mBlendTransform = aTransform;  // 记录当前变换为混合的初始（源）变换
			aTrackInstance->mBlendTime = theBlendTime;
			aTrackInstance->mBlendCounter = theBlendTime;
			aTrackInstance->mBlendTransform.mFont = nullptr;
			aTrackInstance->mBlendTransform.mText = "";
			aTrackInstance->mBlendTransform.mImage = nullptr;
		}
	}
}

//0x4733F0
void Reanimation::ReanimationDie()
{
	if (!mDead)
	{
		mDead = true;
		for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTracks.count; aTrackIndex++)
		{
			TOD_ASSERT(mTrackInstances);
			AttachmentDie(mTrackInstances[aTrackIndex].mAttachmentID);
		}
	}
}

void Reanimation::SetShakeOverride(const char* theTrackName, float theShakeAmount)
{ 
	GetTrackInstanceByName(theTrackName)->mShakeOverride = theShakeAmount;
}

void Reanimation::SetPosition(float theX, float theY) 
{ 
	mOverlayMatrix.m02 = theX;
	mOverlayMatrix.m12 = theY;
}

void Reanimation::OverrideScale(float theScaleX, float theScaleY)
{
	mOverlayMatrix.m00 = theScaleX;
	mOverlayMatrix.m11 = theScaleY;
}

//0x473470
Image* Reanimation::GetImageOverride(const char* theTrackName)
{
	return GetTrackInstanceByName(theTrackName)->mImageOverride;
}

//0x473490
// GOTY @Patoke: 0x477BB0
void Reanimation::SetImageOverride(const char* theTrackName, Image* theImage)
{
	GetTrackInstanceByName(theTrackName)->mImageOverride = theImage;
}

//0x4734B0
void Reanimation::SetTruncateDisappearingFrames(const char* theTrackName, bool theTruncateDisappearingFrames)
{
	if (theTrackName == nullptr)  // 若给出的轨道名称为空指针
	{
		for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTracks.count; aTrackIndex++)  // 依次设置每一轨道
			mTrackInstances[aTrackIndex].mTruncateDisappearingFrames = theTruncateDisappearingFrames;
	}
	else
		GetTrackInstanceByName(theTrackName)->mTruncateDisappearingFrames = theTruncateDisappearingFrames;
}

void ReanimationHolder::DisposeHolder()
{
	mReanimations.DataArrayDispose();
}

//0x473500
ReanimationHolder::~ReanimationHolder()
{
	DisposeHolder();
}

void ReanimationHolder::InitializeHolder()
{
	mReanimations.DataArrayInitialize(1024U, "reanims");
}

//0x473590
Reanimation* ReanimationHolder::AllocReanimation(float theX, float theY, int theRenderOrder, ReanimationType theReanimationType)
{
	TOD_ASSERT(mReanimations.mSize != mReanimations.mMaxSize);
	Reanimation* aReanim = mReanimations.DataArrayAlloc();
	aReanim->mRenderOrder = theRenderOrder;
	aReanim->mReanimationHolder = this;
	aReanim->ReanimationInitializeType(theX, theY, theReanimationType);
	return aReanim;
}

//0x4735E0
void ReanimatorEnsureDefinitionLoaded(ReanimationType theReanimType, bool theIsPreloading)
{
	TOD_ASSERT(theReanimType >= 0 && theReanimType < gReanimatorDefCount);
	ReanimatorDefinition* aReanimDef = &gReanimatorDefArray[(int)theReanimType];
	if (aReanimDef->mTracks.tracks != nullptr)  // 如果轨道指针不为空指针，说明定义数据已经加载
		return;
	ReanimationParams* aReanimParams = &gReanimationParamArray[(int)theReanimType];
	std::string aFileName = "reanim/" + std::string(aReanimParams->mReanimFileName);
	if (theIsPreloading)
	{
		if (gSexyAppBase->mShutdown || gAppCloseRequest())  // 预加载时若程序退出，则取消加载
			return;
	}
	else  // < 以下部分仅内测版执行 >
	{
		if (gAppHasUsedCheatKeys())
			TodTraceAndLog("Cheater failed to preload '%s' on %s", aFileName.c_str(), gGetCurrentLevelName().c_str());
		else
			TodTraceAndLog("Non-cheater failed to preload '%s' on %s", aFileName.c_str(), gGetCurrentLevelName().c_str());
	}  // < 以上部分仅内测版执行 >

	auto aTimer = std::chrono::high_resolution_clock::now();

	TodHesitationBracket aHesitation("Load Reanim '%s'", aReanimParams->mReanimFileName);
	if (!ReanimationLoadDefinition(aFileName, aReanimDef))
	{
		char aBuf[1024];
		sprintf(aBuf, "Failed to load reanim '%s'", aFileName.c_str());
		TodErrorMessageBox(aBuf, "Error");
	}
	int aDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - aTimer).count();
	if (aDuration > 100)  //（仅内测版）创建时间过长的报告
		TodTraceAndLog("loading:  Long reanim '%s' %d ms on %s", aFileName.c_str(), aDuration, gGetCurrentLevelName().c_str());
}

//0x473750
void ReanimatorLoadDefinitions(ReanimationParams* theReanimationParamArray, int theReanimationParamArraySize)
{
	TodHesitationBracket aHesitation(_S("ReanimatorLoadDefinitions"));
	TOD_ASSERT(!gReanimationParamArray && !gReanimatorDefArray);
	gReanimationParamArraySize = theReanimationParamArraySize;
	gReanimationParamArray = theReanimationParamArray;
	gReanimatorDefCount = theReanimationParamArraySize;
	gReanimatorDefArray = new ReanimatorDefinition[theReanimationParamArraySize];

	for (unsigned int i = 0; i < gReanimationParamArraySize; i++)
	{
		ReanimationParams* aReanimationParams = &theReanimationParamArray[i];
		TOD_ASSERT(aReanimationParams->mReanimationType == i);
		if (DefinitionIsCompiled("reanim/" + StringToSexyString(aReanimationParams->mReanimFileName)))
			ReanimatorEnsureDefinitionLoaded(aReanimationParams->mReanimationType, true);
	}
}

//0x473870
void ReanimatorFreeDefinitions()
{
	for (unsigned int i = 0; i < gReanimatorDefCount; i++)
		ReanimationFreeDefinition(&gReanimatorDefArray[i]);

	delete[] gReanimatorDefArray;
	gReanimatorDefArray = nullptr;
	gReanimatorDefCount = 0;
	gReanimationParamArray = nullptr;
	gReanimationParamArraySize = 0;
}

//0x4738D0
float Reanimation::GetTrackVelocity(const char* theTrackName)
{
	ReanimatorFrameTime aFrameTime;
	GetFrameTime(&aFrameTime);
	int aTrackIndex = FindTrackIndex(theTrackName);
	TOD_ASSERT(aTrackIndex >= 0 && aTrackIndex < mDefinition->mTracks.count);

	ReanimatorTrack* aTrack = &mDefinition->mTracks.tracks[aTrackIndex];
	float aDis = aTrack->mTransforms.mTransforms[aFrameTime.mAnimFrameAfterInt].mTransX - aTrack->mTransforms.mTransforms[aFrameTime.mAnimFrameBeforeInt].mTransX;
	return aDis * SECONDS_PER_UPDATE * mAnimRate;  // 瞬时速率 = 两帧间的横坐标之差 * 一帧的时长 * 动画速率
}

//0x473930
bool Reanimation::IsTrackShowing(const char* theTrackName)
{
	ReanimatorFrameTime aFrameTime;
	GetFrameTime(&aFrameTime);
	int aTrackIndex = FindTrackIndex(theTrackName);
	TOD_ASSERT(aTrackIndex >= 0 && aTrackIndex < mDefinition->mTracks.count);

	return mDefinition->mTracks.tracks[aTrackIndex].mTransforms.mTransforms[aFrameTime.mAnimFrameAfterInt].mFrame >= 0.0f;  // 返回下一整数帧是否存在图像
}

//0x473980
void Reanimation::ShowOnlyTrack(const char* theTrackName)
{
	for (int i = 0; i < mDefinition->mTracks.count; i++)
	{
		// 轨道名与指定名称相同时，设置轨道渲染分组为正常显示，否则设置轨道渲染分组为隐藏
		mTrackInstances[i].mRenderGroup = strcasecmp(mDefinition->mTracks.tracks[i].mName, theTrackName) == 0 ? RENDER_GROUP_NORMAL : RENDER_GROUP_HIDDEN;
	}
}

//0x4739E0
// GOTY @Patoke: 0x478120
void Reanimation::AssignRenderGroupToTrack(const char* theTrackName, int theRenderGroup)
{
	for (int i = 0; i < mDefinition->mTracks.count; i++)
		if (strcasecmp(mDefinition->mTracks.tracks[i].mName, theTrackName) == 0)
		{
			mTrackInstances[i].mRenderGroup = theRenderGroup;  // 仅设置首个名称恰好为 theTrackName 的轨道
			return;
		}
}

//0x473A40
// GOTY @Patoke: 0x478170
void Reanimation::AssignRenderGroupToPrefix(const char* theTrackName, int theRenderGroup)
{
	size_t aPrifixLength = strlen(theTrackName);
	for (int i = 0; i < mDefinition->mTracks.count; i++)
	{
		const char* const aTrackName = mDefinition->mTracks.tracks[i].mName;
		if (strlen(aTrackName) >= aPrifixLength && !strncasecmp(aTrackName, theTrackName, aPrifixLength))  // 轨道名称长度必须不小于指定前缀长度
			mTrackInstances[i].mRenderGroup = theRenderGroup;
	}
}

//0x473AE0
void Reanimation::PropogateColorToAttachments()
{
	for (int i = 0; i < mDefinition->mTracks.count; i++)
		AttachmentPropogateColor(
			mTrackInstances[i].mAttachmentID, mColorOverride, mEnableExtraAdditiveDraw, mExtraAdditiveColor, mEnableExtraOverlayDraw, mExtraOverlayColor
		);
}

//0x473B70
bool Reanimation::ShouldTriggerTimedEvent(float theEventTime)
{
	TOD_ASSERT(theEventTime >= 0.0f && theEventTime <= 1.0f);
	if (mFrameCount == 0 || mLastFrameTime <= 0.0f || mAnimRate <= 0.0f)  // 没有动画或倒放或未播放
		return false;

	if (mAnimTime >= mLastFrameTime)  // 一般情况下，可触发的范围为 [mLastFrameTime, mAnimTime]
		return theEventTime >= mLastFrameTime && theEventTime < mAnimTime;
	else  // 若动画正好完成一次循环而重新进入下一次循环，则可触发的范围为 [0, mAnimTime] ∪ [mLastFrameTime, 1]
		return theEventTime >= mLastFrameTime || theEventTime < mAnimTime;
}
//0x473BF0
// GOTY @Patoke: 0x478310
void Reanimation::PlayReanim(const char* theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate)
{
	if (theBlendTime > 0)  // 当需要补间过渡时，开始混合
		StartBlend(theBlendTime);
	if (theAnimRate != 0.0f)  // 当指定的速率为 0 时，表示不改变原有动画速率
		mAnimRate = theAnimRate;

	mLoopType = theLoopType;
	mLoopCount = 0;
	SetFramesForLayer(theTrackName);
}

//0x473C60
void Reanimation::ParseAttacherTrack(const ReanimatorTransform& theTransform, AttacherInfo& theAttacherInfo)
{
	theAttacherInfo.mReanimName = "";
	theAttacherInfo.mTrackName = "";
	theAttacherInfo.mAnimRate = 12.0f;
	theAttacherInfo.mLoopType = ReanimLoopType::REANIM_LOOP;
	if (theTransform.mFrame == -1.0f)  // 如果是空白帧
		return;

	/* 附属轨道名称格式：attacher__REANIMNAME__TRACKNAME[TAG1][TAG2]…… */

	const char* aReanimName = strstr(theTransform.mText, "__");  // 指向动画名称前的双下划线
	if (aReanimName == nullptr)  // 如果字符串中不含双下划线
		return;
	const char* aTags = strstr(aReanimName + 2, "[");  // 动画名称之后，指向 TAG 前的中括号
	const char* aTrackName = strstr(aReanimName + 2, "__");  // 动画名称之后，指向轨道名称前的双下划线
	if (aTags && aTrackName && ((uintptr_t)aTags < (uintptr_t)aTrackName))  // 如果“[”之后还有双下划线，则字符串非法
		return;

	if (aTrackName)  // 如果有定义轨道名称
	{
		theAttacherInfo.mReanimName.assign(aReanimName + 2, aTrackName - aReanimName - 2);  // 取两处双下划线之间的部分（REANIMNAME）
		if (aTags)  // 如果有定义标签
			theAttacherInfo.mTrackName.assign(aTrackName + 2, aTags - aTrackName - 2);  // 取到 TAG 的中括号之前
		else
			theAttacherInfo.mTrackName.assign(aTrackName + 2);  // 取到字符串结尾
	}
	else if (aTags)  // 如果未定义轨道名称但定义了标签
		theAttacherInfo.mReanimName.assign(aReanimName + 2, aTags - aReanimName - 2);  // 取双下划线至中括号之间的部分
	else  // 如果只定义了轨道名称
		theAttacherInfo.mReanimName.assign(aReanimName + 2);  // 从双下划线之后取到字符串结尾

	while (aTags)  // 读取每个 TAG
	{
		const char* aTagEnds = strstr(aTags + 1, "]");
		if (aTagEnds == nullptr)  // 如果没有右中括号
			break;
		
		std::string aCode(aTags + 1, aTagEnds - aTags - 1);  // 取中括号内的文本
		if (sscanf(aCode.c_str(), "%f", &theAttacherInfo.mAnimRate) != 1)  // 尝试将文本作为浮点数扫描，如果扫描成功则将结果作为动画速率
		{
			if (aCode.compare("hold") == 0)
				theAttacherInfo.mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
			else if (aCode.compare("once") == 0)
				theAttacherInfo.mLoopType = ReanimLoopType::REANIM_PLAY_ONCE;
		}

		aTags = strstr(aTagEnds + 1, "[");  // 继续寻找下一个 TAG 的左中括号
	}
}

//0x473EB0
void Reanimation::AttacherSynchWalkSpeed(int theTrackIndex, Reanimation* theAttachReanim, AttacherInfo& theAttacherInfo)
{
	(void)theAttacherInfo;
	ReanimatorTrack* aTrack = &mDefinition->mTracks.tracks[theTrackIndex];
	ReanimatorFrameTime aFrameTime;
	GetFrameTime(&aFrameTime);

	int aPlaceHolderFrameStart = aFrameTime.mAnimFrameBeforeInt;
	while (aPlaceHolderFrameStart > mFrameStart && aTrack->mTransforms.mTransforms[aPlaceHolderFrameStart - 1].mText == aTrack->mTransforms.mTransforms[aPlaceHolderFrameStart].mText)
		aPlaceHolderFrameStart--;  // 取当前所在区间的第一帧
	int aPlaceHolderFrameEnd = aFrameTime.mAnimFrameBeforeInt;
	while (aPlaceHolderFrameEnd < mFrameStart + mFrameCount - 1 && aTrack->mTransforms.mTransforms[aPlaceHolderFrameEnd + 1].mText == aTrack->mTransforms.mTransforms[aPlaceHolderFrameEnd].mText)
		aPlaceHolderFrameEnd++;  // 取当前所在区间的最后一帧
	int aPlaceHolderFrameCount = aPlaceHolderFrameEnd - aPlaceHolderFrameStart;
	ReanimatorTransform& aPlaceHolderStartTrans = aTrack->mTransforms.mTransforms[aPlaceHolderFrameStart];
	ReanimatorTransform& aPlaceHolderEndTrans = aTrack->mTransforms.mTransforms[aPlaceHolderFrameEnd];
	if (FloatApproxEqual(mAnimRate, 0.0f))  // 如果动画自身的速率为 0
	{
		theAttachReanim->mAnimRate = 0.0f;  // 附属动画的速率也为 0
		return;
	}
	float aPlaceHolderDistance = -(aPlaceHolderEndTrans.mTransX - aPlaceHolderStartTrans.mTransX);  // 占位轨道在当前区间内的位移
	float aPlaceHolderSeconds = aPlaceHolderFrameCount / mAnimRate;  // 占位轨道在当前区间内的时长
	if (FloatApproxEqual(aPlaceHolderSeconds, 0.0f))  // 如果当前所在区间不存在任何帧
	{
		theAttachReanim->mAnimRate = 0.0f;  // 附属动画的速率为 0
		return;
	}

	int aGroundTrackIndex = theAttachReanim->FindTrackIndex("_ground");
	ReanimatorTrack* aGroundTrack = &theAttachReanim->mDefinition->mTracks.tracks[aGroundTrackIndex];
	ReanimatorTransform& aTransformGuyStart = aGroundTrack->mTransforms.mTransforms[theAttachReanim->mFrameStart];
	ReanimatorTransform& aTransformGuyEnd = aGroundTrack->mTransforms.mTransforms[theAttachReanim->mFrameStart + theAttachReanim->mFrameCount - 1];
	float aGuyDistance = aTransformGuyEnd.mTransX - aTransformGuyStart.mTransX;  // 实际动画在完整动作周期内的位移
	if (aGuyDistance < FLT_EPSILON || aPlaceHolderDistance < FLT_EPSILON)  // 如果占位位移为 0 或实际动画周期位移为 0，则附属动画无法移动
	{
		theAttachReanim->mAnimRate = 0.0f;  // 附属动画的速率为 0
		return;
	}

	float aLoops = aPlaceHolderDistance / aGuyDistance;  // 以附属动画目标位移（占位位移）除以其周期位移，得到附属动画需要循环的周期数
	ReanimatorTransform aTransformGuyCurrent;
	theAttachReanim->GetCurrentTransform(aGroundTrackIndex, &aTransformGuyCurrent);
	AttachEffect* aAttachEffect = FindFirstAttachment(mTrackInstances[theTrackIndex].mAttachmentID);
	if (aAttachEffect != nullptr)
	{
		float aGuyCurrentDistance = aTransformGuyCurrent.mTransX - aTransformGuyStart.mTransX;  // 附属动画在其周期内当前已经过的位移
		float aGuyExpectedDistance = aGuyDistance * theAttachReanim->mAnimTime;  // 以匀速运动的占位轨道计算的、附属动画当前的理论位移
		aAttachEffect->mOffset.m02 = aGuyExpectedDistance - aGuyCurrentDistance;  // 调整附属效果的横向变换以使附属动画的位移保持与占位动画一致
	}
	theAttachReanim->mAnimRate = aLoops * theAttachReanim->mFrameCount / aPlaceHolderSeconds;  // 速率 = 需要播放的帧数 ÷ 可以播放的时长
}

//0x4740B0
void Reanimation::UpdateAttacherTrack(int theTrackIndex)
{
	ReanimatorTrackInstance* aTrackInstance = &mTrackInstances[theTrackIndex];
	ReanimatorTransform aTransform;
	GetCurrentTransform(theTrackIndex, &aTransform);
	AttacherInfo aAttacherInfo;
	ParseAttacherTrack(aTransform, aAttacherInfo);

	ReanimationType aReanimationType = ReanimationType::REANIM_NONE;
	if (aAttacherInfo.mReanimName.size() != 0)  // 如果附属轨道设定了当前的附属动画名称
	{
		std::string aReanimFileName = StrFormat("%s.reanim", aAttacherInfo.mReanimName.c_str());
		for (unsigned int i = 0; i < gReanimationParamArraySize; i++)  // 在动画参数数组中寻找动画文件名对应的动画类型
		{
			ReanimationParams* aParams = &gReanimationParamArray[i];
			if (strcasecmp(aReanimFileName.c_str(), aParams->mReanimFileName) == 0)
			{
				aReanimationType = aParams->mReanimationType;
				break;
			}
		}
	}
	if (aReanimationType == ReanimationType::REANIM_NONE)  // 如果没有设定当前附属动画名称，或未找到相应的动画
	{
		AttachmentDie(aTrackInstance->mAttachmentID);  // 清除附件
		return;
	}

	Reanimation* aAttachReanim = FindReanimAttachment(aTrackInstance->mAttachmentID);
	if (aAttachReanim == nullptr || aAttachReanim->mReanimationType != aReanimationType)  // 如果原先没有附属动画，或原附属动画不是上述设定的动画
	{
		AttachmentDie(aTrackInstance->mAttachmentID);  // 清除原有附件
		aAttachReanim = gEffectSystem->mReanimationHolder->AllocReanimation(0.0f, 0.0f, 0, aReanimationType);  // 重新创建一个指定的动画
		aAttachReanim->mLoopType = aAttacherInfo.mLoopType;
		aAttachReanim->mAnimRate = aAttacherInfo.mAnimRate;
		AttachReanim(aTrackInstance->mAttachmentID, aAttachReanim, 0.0f, 0.0f);
		mFrameBasePose = NO_BASE_POSE;  // 设定附属动画后，自身不再存在基准帧
	}

	if (aAttacherInfo.mTrackName.size() != 0)  // 如果定义了附属动画的动作轨道
	{
		int aAnimFrameStart, aAnimFrameCount;
		aAttachReanim->GetFramesForLayer(aAttacherInfo.mTrackName.c_str(), aAnimFrameStart, aAnimFrameCount);
		if (aAttachReanim->mFrameStart != aAnimFrameStart || aAttachReanim->mFrameCount != aAnimFrameCount)  // if (!aAttachReanim->IsAnimPlaying(……))
		{
			aAttachReanim->StartBlend(20);
			aAttachReanim->SetFramesForLayer(aAttacherInfo.mTrackName.c_str());  // 播放指定轨道上的动作
		}

		if (aAttachReanim->mAnimRate == 12.0f && aAttacherInfo.mTrackName.compare("anim_walk") == 0 && aAttachReanim->TrackExists("_ground"))
			AttacherSynchWalkSpeed(theTrackIndex, aAttachReanim, aAttacherInfo);
		else
			aAttachReanim->mAnimRate = aAttacherInfo.mAnimRate;
		aAttachReanim->mLoopType = aAttacherInfo.mLoopType;
	}

	Color aColor = ColorsMultiply(mColorOverride, aTrackInstance->mTrackColor);
	aColor.mAlpha = ClampInt(FloatRoundToInt(aTransform.mAlpha * aColor.mAlpha), 0, 255);
	AttachmentPropogateColor(aTrackInstance->mAttachmentID, aColor, mEnableExtraAdditiveDraw, mExtraAdditiveColor, mEnableExtraOverlayDraw, mExtraOverlayColor);
}

//0x4745B0
bool Reanimation::IsAnimPlaying(const char* theTrackName)
{
	int aFrameStart, aFrameCount;
	GetFramesForLayer(theTrackName, aFrameStart, aFrameCount);
	return mFrameStart == aFrameStart && mFrameCount == aFrameCount;
}

//0x4745F0
Reanimation* Reanimation::FindSubReanim(ReanimationType theReanimType)
{
	if (mReanimationType == theReanimType)
		return this;

	for (int i = 0; i < mDefinition->mTracks.count; i++)
	{
		Reanimation* aReanimation = FindReanimAttachment(mTrackInstances[i].mAttachmentID);
		if (aReanimation != nullptr)
		{
			Reanimation* aSubReanim = aReanimation->FindSubReanim(theReanimType);
			if (aSubReanim != nullptr)
				return aSubReanim;
		}
	}

	return nullptr;
}
