#include "TodDebug.h"
#include "TodCommon.h"
#include "Definition.h"
#include "Reanimator.h"
#include "../LawnApp.h"
#include "Attachment.h"
#include "ReanimAtlas.h"
#include "EffectSystem.h"
#include "../GameConstants.h"
#include "../SexyAppFramework/Font.h"
#include "../SexyAppFramework/PerfTimer.h"
#include "../SexyAppFramework/MemoryImage.h"

int gReanimatorDefCount;                     //[0x6A9EE4]
ReanimatorDefinition* gReanimatorDefArray;   //[0x6A9EE8]
int gReanimationParamArraySize;              //[0x6A9EEC]
ReanimationParams* gReanimationParamArray;   //[0x6A9EF0]

ReanimationParams gLawnReanimationArray[(int)ReanimationType::NUM_REANIMS] = { //0x6A1340
	{ ReanimationType::REANIM_LOADBAR_SPROUT,                       "reanim\\LoadBar_sprout.reanim",                    1 },
	{ ReanimationType::REANIM_LOADBAR_ZOMBIEHEAD,                   "reanim\\LoadBar_Zombiehead.reanim",                1 },
	{ ReanimationType::REANIM_SODROLL,                              "reanim\\SodRoll.reanim",                           0 },
	{ ReanimationType::REANIM_FINAL_WAVE,                           "reanim\\FinalWave.reanim",                         1 },
	{ ReanimationType::REANIM_PEASHOOTER,                           "reanim\\PeaShooterSingle.reanim",                  0 },
	{ ReanimationType::REANIM_WALLNUT,                              "reanim\\Wallnut.reanim",                           0 },
	{ ReanimationType::REANIM_LILYPAD,                              "reanim\\Lilypad.reanim",                           0 },
	{ ReanimationType::REANIM_SUNFLOWER,                            "reanim\\SunFlower.reanim",                         0 },
	{ ReanimationType::REANIM_LAWNMOWER,                            "reanim\\LawnMower.reanim",                         0 },
	{ ReanimationType::REANIM_READYSETPLANT,                        "reanim\\StartReadySetPlant.reanim",                1 },
	{ ReanimationType::REANIM_CHERRYBOMB,                           "reanim\\CherryBomb.reanim",                        0 },
	{ ReanimationType::REANIM_SQUASH,                               "reanim\\Squash.reanim",                            0 },
	{ ReanimationType::REANIM_DOOMSHROOM,                           "reanim\\DoomShroom.reanim",                        0 },
	{ ReanimationType::REANIM_SNOWPEA,                              "reanim\\SnowPea.reanim",                           0 },
	{ ReanimationType::REANIM_REPEATER,                             "reanim\\PeaShooter.reanim",                        0 },
	{ ReanimationType::REANIM_SUNSHROOM,                            "reanim\\SunShroom.reanim",                         0 },
	{ ReanimationType::REANIM_TALLNUT,                              "reanim\\Tallnut.reanim",                           0 },
	{ ReanimationType::REANIM_FUMESHROOM,                           "reanim\\Fumeshroom.reanim",                        0 },
	{ ReanimationType::REANIM_PUFFSHROOM,                           "reanim\\Puffshroom.reanim",                        0 },
	{ ReanimationType::REANIM_HYPNOSHROOM,                          "reanim\\Hypnoshroom.reanim",                       0 },
	{ ReanimationType::REANIM_CHOMPER,                              "reanim\\Chomper.reanim",                           0 },
	{ ReanimationType::REANIM_ZOMBIE,                               "reanim\\Zombie.reanim",                            0 },
	{ ReanimationType::REANIM_SUN,                                  "reanim\\Sun.reanim",                               0 },
	{ ReanimationType::REANIM_POTATOMINE,                           "reanim\\PotatoMine.reanim",                        0 },
	{ ReanimationType::REANIM_SPIKEWEED,                            "reanim\\Caltrop.reanim",                           0 },
	{ ReanimationType::REANIM_SPIKEROCK,                            "reanim\\SpikeRock.reanim",                         0 },
	{ ReanimationType::REANIM_THREEPEATER,                          "reanim\\ThreePeater.reanim",                       0 },
	{ ReanimationType::REANIM_MARIGOLD,                             "reanim\\Marigold.reanim",                          0 },
	{ ReanimationType::REANIM_ICESHROOM,                            "reanim\\IceShroom.reanim",                         0 },
	{ ReanimationType::REANIM_ZOMBIE_FOOTBALL,                      "reanim\\Zombie_football.reanim",                   0 },
	{ ReanimationType::REANIM_ZOMBIE_NEWSPAPER,                     "reanim\\Zombie_paper.reanim",                      0 },
	{ ReanimationType::REANIM_ZOMBIE_ZAMBONI,                       "reanim\\Zombie_zamboni.reanim",                    0 },
	{ ReanimationType::REANIM_SPLASH,                               "reanim\\splash.reanim",                            0 },
	{ ReanimationType::REANIM_JALAPENO,                             "reanim\\Jalapeno.reanim",                          0 },
	{ ReanimationType::REANIM_JALAPENO_FIRE,                        "reanim\\fire.reanim",                              0 },
	{ ReanimationType::REANIM_COIN_SILVER,                          "reanim\\Coin_silver.reanim",                       0 },
	{ ReanimationType::REANIM_ZOMBIE_CHARRED,                       "reanim\\Zombie_charred.reanim",                    0 },
	{ ReanimationType::REANIM_ZOMBIE_CHARRED_IMP,                   "reanim\\Zombie_charred_imp.reanim",                0 },
	{ ReanimationType::REANIM_ZOMBIE_CHARRED_DIGGER,                "reanim\\Zombie_charred_digger.reanim",             0 },
	{ ReanimationType::REANIM_ZOMBIE_CHARRED_ZAMBONI,               "reanim\\Zombie_charred_zamboni.reanim",            0 },
	{ ReanimationType::REANIM_ZOMBIE_CHARRED_CATAPULT,              "reanim\\Zombie_charred_catapult.reanim",           0 },
	{ ReanimationType::REANIM_ZOMBIE_CHARRED_GARGANTUAR,            "reanim\\Zombie_charred_gargantuar.reanim",         0 },
	{ ReanimationType::REANIM_SCRAREYSHROOM,                        "reanim\\ScaredyShroom.reanim",                     0 },
	{ ReanimationType::REANIM_PUMPKIN,                              "reanim\\Pumpkin.reanim",                           0 },
	{ ReanimationType::REANIM_PLANTERN,                             "reanim\\Plantern.reanim",                          0 },
	{ ReanimationType::REANIM_TORCHWOOD,                            "reanim\\Torchwood.reanim",                         0 },
	{ ReanimationType::REANIM_SPLITPEA,                             "reanim\\SplitPea.reanim",                          0 },
	{ ReanimationType::REANIM_SEASHROOM,                            "reanim\\SeaShroom.reanim",                         0 },
	{ ReanimationType::REANIM_BLOVER,                               "reanim\\Blover.reanim",                            0 },
	{ ReanimationType::REANIM_FLOWER_POT,                           "reanim\\Pot.reanim",                               0 },
	{ ReanimationType::REANIM_CACTUS,                               "reanim\\Cactus.reanim",                            0 },
	{ ReanimationType::REANIM_DANCER,                               "reanim\\Zombie_disco.reanim",						0 }, // @Patoke: GOTY has different reanim name
	{ ReanimationType::REANIM_TANGLEKELP,                           "reanim\\Tanglekelp.reanim",                        0 },
	{ ReanimationType::REANIM_STARFRUIT,                            "reanim\\Starfruit.reanim",                         0 },
	{ ReanimationType::REANIM_POLEVAULTER,                          "reanim\\Zombie_polevaulter.reanim",                0 },
	{ ReanimationType::REANIM_BALLOON,                              "reanim\\Zombie_balloon.reanim",                    0 },
	{ ReanimationType::REANIM_GARGANTUAR,                           "reanim\\Zombie_gargantuar.reanim",                 0 },
	{ ReanimationType::REANIM_IMP,                                  "reanim\\Zombie_imp.reanim",                        0 },
	{ ReanimationType::REANIM_DIGGER,                               "reanim\\Zombie_digger.reanim",                     0 },
	{ ReanimationType::REANIM_DIGGER_DIRT,                          "reanim\\Digger_rising_dirt.reanim",                0 },
	{ ReanimationType::REANIM_ZOMBIE_DOLPHINRIDER,                  "reanim\\Zombie_dolphinrider.reanim",               0 },
	{ ReanimationType::REANIM_POGO,                                 "reanim\\Zombie_pogo.reanim",                       0 },
	{ ReanimationType::REANIM_BACKUP_DANCER,                        "reanim\\Zombie_backup.reanim",                     0 }, // @Patoke: GOTY has different reanim name
	{ ReanimationType::REANIM_BOBSLED,                              "reanim\\Zombie_bobsled.reanim",                    0 },
	{ ReanimationType::REANIM_JACKINTHEBOX,                         "reanim\\Zombie_jackbox.reanim",                    0 },
	{ ReanimationType::REANIM_SNORKEL,                              "reanim\\Zombie_snorkle.reanim",                    0 },
	{ ReanimationType::REANIM_BUNGEE,                               "reanim\\Zombie_bungi.reanim",                      0 },
	{ ReanimationType::REANIM_CATAPULT,                             "reanim\\Zombie_catapult.reanim",                   0 },
	{ ReanimationType::REANIM_LADDER,                               "reanim\\Zombie_ladder.reanim",                     0 },
	{ ReanimationType::REANIM_PUFF,                                 "reanim\\Puff.reanim",                              0 },
	{ ReanimationType::REANIM_SLEEPING,                             "reanim\\Z.reanim",                                 0 },
	{ ReanimationType::REANIM_GRAVE_BUSTER,                         "reanim\\Gravebuster.reanim",                       0 },
	{ ReanimationType::REANIM_ZOMBIES_WON,                          "reanim\\ZombiesWon.reanim",                        1 },
	{ ReanimationType::REANIM_MAGNETSHROOM,                         "reanim\\Magnetshroom.reanim",                      0 },
	{ ReanimationType::REANIM_BOSS,                                 "reanim\\Zombie_boss.reanim",                       0 },
	{ ReanimationType::REANIM_CABBAGEPULT,                          "reanim\\Cabbagepult.reanim",                       0 },
	{ ReanimationType::REANIM_KERNELPULT,                           "reanim\\Cornpult.reanim",                          0 },
	{ ReanimationType::REANIM_MELONPULT,                            "reanim\\Melonpult.reanim",                         0 },
	{ ReanimationType::REANIM_COFFEEBEAN,                           "reanim\\Coffeebean.reanim",                        1 },
	{ ReanimationType::REANIM_UMBRELLALEAF,                         "reanim\\Umbrellaleaf.reanim",                      0 },
	{ ReanimationType::REANIM_GATLINGPEA,                           "reanim\\GatlingPea.reanim",                        0 },
	{ ReanimationType::REANIM_CATTAIL,                              "reanim\\Cattail.reanim",                           0 },
	{ ReanimationType::REANIM_GLOOMSHROOM,                          "reanim\\GloomShroom.reanim",                       0 },
	{ ReanimationType::REANIM_BOSS_ICEBALL,                         "reanim\\Zombie_boss_iceball.reanim",               1 },
	{ ReanimationType::REANIM_BOSS_FIREBALL,                        "reanim\\Zombie_boss_fireball.reanim",              1 },
	{ ReanimationType::REANIM_COBCANNON,                            "reanim\\CobCannon.reanim",                         0 },
	{ ReanimationType::REANIM_GARLIC,                               "reanim\\Garlic.reanim",                            0 },
	{ ReanimationType::REANIM_GOLD_MAGNET,                          "reanim\\GoldMagnet.reanim",                        0 },
	{ ReanimationType::REANIM_WINTER_MELON,                         "reanim\\WinterMelon.reanim",                       0 },
	{ ReanimationType::REANIM_TWIN_SUNFLOWER,                       "reanim\\TwinSunflower.reanim",                     0 },
	{ ReanimationType::REANIM_POOL_CLEANER,                         "reanim\\PoolCleaner.reanim",                       0 },
	{ ReanimationType::REANIM_ROOF_CLEANER,                         "reanim\\RoofCleaner.reanim",                       0 },
	{ ReanimationType::REANIM_FIRE_PEA,                             "reanim\\FirePea.reanim",                           0 },
	{ ReanimationType::REANIM_IMITATER,                             "reanim\\Imitater.reanim",                          0 },
	{ ReanimationType::REANIM_YETI,                                 "reanim\\Zombie_yeti.reanim",                       0 },
	{ ReanimationType::REANIM_BOSS_DRIVER,                          "reanim\\Zombie_Boss_driver.reanim",                0 },
	{ ReanimationType::REANIM_LAWN_MOWERED_ZOMBIE,                  "reanim\\LawnMoweredZombie.reanim",                 0 },
	{ ReanimationType::REANIM_CRAZY_DAVE,                           "reanim\\CrazyDave.reanim",                         1 },
	{ ReanimationType::REANIM_TEXT_FADE_ON,                         "reanim\\TextFadeOn.reanim",                        0 },
	{ ReanimationType::REANIM_HAMMER,                               "reanim\\Hammer.reanim",                            0 },
	{ ReanimationType::REANIM_SLOT_MACHINE_HANDLE,                  "reanim\\SlotMachine.reanim",                       0 },
	{ ReanimationType::REANIM_CREDITS_FOOTBALL,                     "reanim\\Credits_Football.reanim",                  1 },
	{ ReanimationType::REANIM_CREDITS_JACKBOX,                      "reanim\\Credits_Jackbox.reanim",                   1 },
	{ ReanimationType::REANIM_SELECTOR_SCREEN,                      "reanim\\SelectorScreen.reanim",                    3 },
	{ ReanimationType::REANIM_PORTAL_CIRCLE,                        "reanim\\Portal_Circle.reanim",                     0 },
	{ ReanimationType::REANIM_PORTAL_SQUARE,                        "reanim\\Portal_Square.reanim",                     0 },
	{ ReanimationType::REANIM_ZENGARDEN_SPROUT,                     "reanim\\ZenGarden_sprout.reanim",                  0 },
	{ ReanimationType::REANIM_ZENGARDEN_WATERINGCAN,                "reanim\\ZenGarden_wateringcan.reanim",             1 },
	{ ReanimationType::REANIM_ZENGARDEN_FERTILIZER,                 "reanim\\ZenGarden_fertilizer.reanim",              1 },
	{ ReanimationType::REANIM_ZENGARDEN_BUGSPRAY,                   "reanim\\ZenGarden_bugspray.reanim",                1 },
	{ ReanimationType::REANIM_ZENGARDEN_PHONOGRAPH,                 "reanim\\ZenGarden_phonograph.reanim",              1 },
	{ ReanimationType::REANIM_DIAMOND,                              "reanim\\Diamond.reanim",                           0 },
	{ ReanimationType::REANIM_ZOMBIE_HAND,                          "reanim\\Zombie_hand.reanim",                       1 },
	{ ReanimationType::REANIM_STINKY,                               "reanim\\Stinky.reanim",                            0 },
	{ ReanimationType::REANIM_RAKE,                                 "reanim\\Rake.reanim",                              0 },
	{ ReanimationType::REANIM_RAIN_CIRCLE,                          "reanim\\Rain_circle.reanim",                       0 },
	{ ReanimationType::REANIM_RAIN_SPLASH,                          "reanim\\Rain_splash.reanim",                       0 },
	{ ReanimationType::REANIM_ZOMBIE_SURPRISE,                      "reanim\\Zombie_surprise.reanim",                   0 },
	{ ReanimationType::REANIM_COIN_GOLD,                            "reanim\\Coin_gold.reanim",                         0 },
	{ ReanimationType::REANIM_TREEOFWISDOM,                         "reanim\\TreeOfWisdom.reanim",                      1 },
	{ ReanimationType::REANIM_TREEOFWISDOM_CLOUDS,                  "reanim\\TreeOfWisdomClouds.reanim",                1 },
	{ ReanimationType::REANIM_TREEOFWISDOM_TREEFOOD,                "reanim\\TreeFood.reanim",                          1 },
	{ ReanimationType::REANIM_CREDITS_MAIN,                         "reanim\\Credits_Main.reanim",                      3 },
	{ ReanimationType::REANIM_CREDITS_MAIN2,                        "reanim\\Credits_Main2.reanim",                     3 },
	{ ReanimationType::REANIM_CREDITS_MAIN3,                        "reanim\\Credits_Main3.reanim",                     3 },
	{ ReanimationType::REANIM_ZOMBIE_CREDITS_DANCE,                 "reanim\\Zombie_credits_dance.reanim",              0 },
	{ ReanimationType::REANIM_CREDITS_STAGE,                        "reanim\\Credits_stage.reanim",                     1 },
	{ ReanimationType::REANIM_CREDITS_BIGBRAIN,                     "reanim\\Credits_BigBrain.reanim",                  1 },
	{ ReanimationType::REANIM_CREDITS_FLOWER_PETALS,                "reanim\\Credits_Flower_petals.reanim",             1 },
	{ ReanimationType::REANIM_CREDITS_INFANTRY,                     "reanim\\Credits_Infantry.reanim",                  1 },
	{ ReanimationType::REANIM_CREDITS_THROAT,                       "reanim\\Credits_Throat.reanim",                    1 },
	{ ReanimationType::REANIM_CREDITS_CRAZYDAVE,                    "reanim\\Credits_CrazyDave.reanim",                 1 },
	{ ReanimationType::REANIM_CREDITS_BOSSDANCE,                    "reanim\\Credits_Bossdance.reanim",                 1 },
	{ ReanimationType::REANIM_ZOMBIE_CREDITS_SCREEN_DOOR,           "reanim\\Zombie_Credits_Screendoor.reanim",         1 },
	{ ReanimationType::REANIM_ZOMBIE_CREDITS_CONEHEAD,              "reanim\\Zombie_Credits_Conehead.reanim",           1 },
	{ ReanimationType::REANIM_CREDITS_ZOMBIEARMY1,                  "reanim\\Credits_ZombieArmy1.reanim",               1 },
	{ ReanimationType::REANIM_CREDITS_ZOMBIEARMY2,                  "reanim\\Credits_ZombieArmy2.reanim",               1 },
	{ ReanimationType::REANIM_CREDITS_TOMBSTONES,                   "reanim\\Credits_Tombstones.reanim",                1 },
	{ ReanimationType::REANIM_CREDITS_SOLARPOWER,                   "reanim\\Credits_SolarPower.reanim",                1 },
	{ ReanimationType::REANIM_CREDITS_ANYHOUR,                      "reanim\\Credits_Anyhour.reanim",                   3 },
	{ ReanimationType::REANIM_CREDITS_WEARETHEUNDEAD,               "reanim\\Credits_WeAreTheUndead.reanim",            1 },
	{ ReanimationType::REANIM_CREDITS_DISCOLIGHTS,                  "reanim\\Credits_DiscoLights.reanim",               1 },
	{ ReanimationType::REANIM_FLAG,                                 "reanim\\Zombie_FlagPole.reanim",                   0 },
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

void ReanimationFillInMissingData(float& thePrev, float& theValue)
{
	if (theValue == DEFAULT_FIELD_PLACEHOLDER)
		theValue = thePrev;  // ����ǰ֡�ϵ�ֵδ�趨������ǰһ֡����ֵ��ֵ��ǰ֡
	else
		thePrev = theValue;  // ���򣬽���ǰ֡�����ݼ�¼Ϊ��ǰһ֡�����ݡ�
}

void ReanimationFillInMissingData(void*& thePrev, void*& theValue)
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

	for (int aTrackIndex = 0; aTrackIndex < theDefinition->mTrackCount; aTrackIndex++)
	{
		ReanimatorTrack* aTrack = &theDefinition->mTracks[aTrackIndex];
		float aPrevTransX = 0.0f;
		float aPrevTransY = 0.0f;
		float aPrevSkewX = 0.0f;
		float aPrevSkewY = 0.0f;
		float aPrevScaleX = 1.0f;
		float aPrevScaleY = 1.0f;
		float aPrevFrame = 0.0f;
		float aPrevAlpha = 1.0f;
		Image* aPrevImage = nullptr;
		Font* aPrevFont = nullptr;
		const char* aPrevText = "";

		// ����ÿһ֡��������ǰһ֡����������һ֡��δ�������ݣ������¼�¼ǰһ֡������
		for (int i = 0; i < aTrack->mTransformCount; i++)
		{
			ReanimatorTransform& aTransform = aTrack->mTransforms[i];
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
	// �ͷ� Atlas
	if (theDefinition->mReanimAtlas != nullptr)
	{
		theDefinition->mReanimAtlas->ReanimAtlasDispose();
		delete theDefinition->mReanimAtlas;
		theDefinition->mReanimAtlas = nullptr;
	}

	// �ָ���������
	for (int aTrackIndex = 0; aTrackIndex < theDefinition->mTrackCount; aTrackIndex++)
	{
		ReanimatorTrack* aTrack = &theDefinition->mTracks[aTrackIndex];
		const char* aPrevText = nullptr;
		for (int i = 0; i < aTrack->mTransformCount; i++)
		{
			ReanimatorTransform& aTransform = aTrack->mTransforms[i];
			if (*aTransform.mText != '\0' && aTransform.mText == aPrevText)
				aTransform.mText = "";
			else
				aPrevText = aTransform.mText;
		}
	}

	// �ͷŶ���
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
		int aItemSize = mDefinition->mTrackCount * sizeof(ReanimatorTrackInstance);
		FindGlobalAllocator(aItemSize)->Free(mTrackInstances, aItemSize);  // �� TodAllocator ���ն���������ڴ�����
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
		return;  // �������Ѵ��� Atlas ������ Atlas ʱ��ֱ���˳�

	PerfTimer aTimer;
	aTimer.Start();
	TodHesitationTrace("preatlas");
	ReanimAtlas* aAtlas = new ReanimAtlas();
	theDefinition->mReanimAtlas = aAtlas;  // ��ֵ���� Atlas ָ��
	aAtlas->ReanimAtlasCreate(theDefinition);

	TodHesitationTrace("atlas '%s'", aParam.mReanimFileName);
	int aDuration = max(aTimer.GetDuration(), 0);
	if (aDuration > 20 && theReanimationType != ReanimationType::REANIM_NONE)  //�����ڲ�棩����ʱ������ı���
		TodTraceAndLog("LOADING:Long atlas '%s' %d ms on %s", aParam.mReanimFileName, aDuration, gGetCurrentLevelName().c_str());
}

void ReanimationPreload(ReanimationType theReanimationType)
{
	TOD_ASSERT(theReanimationType >= 0 && theReanimationType < gReanimatorDefCount);

	ReanimatorDefinition* aReanimDef = &gReanimatorDefArray[(int)theReanimationType];
	ReanimationCreateAtlas(aReanimDef, theReanimationType);
	if (aReanimDef->mReanimAtlas)
	{
		TodSandImageIfNeeded(aReanimDef->mReanimAtlas->mMemoryImage);
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

	if (theDefinition->mTrackCount != 0)
	{
		mFrameCount = mDefinition->mTracks[0].mTransformCount;
		int aItemSize = theDefinition->mTrackCount * sizeof(ReanimatorTrackInstance);
		mTrackInstances = (ReanimatorTrackInstance*)FindGlobalAllocator(aItemSize)->Calloc(aItemSize);  // ���붯�����ʵ������������ڴ�
		for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTrackCount; aTrackIndex++)  // ������ʼ��������ÿ�����ʵ��
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
void Reanimation::Update()
{
	if (mFrameCount == 0 || mDead)
		return;

	TOD_ASSERT(_finite(mAnimRate));
	mLastFrameTime = mAnimTime;  // ������һ֡��ѭ����
	mAnimTime += SECONDS_PER_UPDATE * mAnimRate / mFrameCount;  // ���µ�ǰѭ����

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

	for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTrackCount; aTrackIndex++)
	{
		ReanimatorTrackInstance* aTrack = &mTrackInstances[aTrackIndex];
		if (aTrack->mBlendCounter > 0)
			aTrack->mBlendCounter--;  // ���¹���Ļ�ϵ���ʱ

		if (aTrack->mShakeOverride != 0.0f)  // ���¹����
		{
			aTrack->mShakeX = RandRangeFloat(-aTrack->mShakeOverride, aTrack->mShakeOverride);
			aTrack->mShakeY = RandRangeFloat(-aTrack->mShakeOverride, aTrack->mShakeOverride);
		}

		if (strnicmp(mDefinition->mTracks[aTrackIndex].mName, "attacher__", 10) == 0)  // IsAttacher
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
	// �Ʋ�����ԭ����Ϊ��ȷ���������任֮�����б�ǶȲ����� �У�WP �棩��
	// ԭ�棨�Լ��ڲ�棩ʵ��Ϊ����б���� �� ʱ theTransform2 �任��Ч
	while (aSkewX2 > theTransform1.mSkewX + 180.0f)
		aSkewX2 = theTransform1.mSkewX;  // ��aSkewX2 -= 360.0f��
	while (aSkewX2 < theTransform1.mSkewX - 180.0f)
		aSkewX2 = theTransform1.mSkewX;  // ��aSkewX2 += 360.0f��
	while (aSkewY2 > theTransform1.mSkewY + 180.0f)
		aSkewY2 = theTransform1.mSkewY;  // ��aSkewY2 -= 360.0f��
	while (aSkewY2 < theTransform1.mSkewY - 180.0f)
		aSkewY2 = theTransform1.mSkewY;  // ��aSkewY2 += 360.0f��

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
	GetTransformAtTime(theTrackIndex, theTransformCurrent, &aFrameTime);  // �����֮֡�����Ȼ����ȡ�û����任
	
	ReanimatorTrackInstance* aTrack = &mTrackInstances[theTrackIndex];
	if (FloatRoundToInt(theTransformCurrent->mFrame) >= 0 && aTrack->mBlendCounter > 0)  // ����ǰ��Ϊ�հ�֡�ҹ�����ڱ任��Ϲ�����
	{
		float aBlendFactor = aTrack->mBlendCounter / (float)aTrack->mBlendTime;
		BlendTransform(theTransformCurrent, *theTransformCurrent, aTrack->mBlendTransform, aBlendFactor);  // ��ϸ�д�任�����Ϻ��ʵ�ʱ任
	}
}

//0x472020
void Reanimation::GetTransformAtTime(int theTrackIndex, ReanimatorTransform* theTransform, ReanimatorFrameTime* theFrameTime)
{
	TOD_ASSERT(theTrackIndex >= 0 && theTrackIndex < mDefinition->mTrackCount);
	ReanimatorTrack* aTrack = &mDefinition->mTracks[theTrackIndex];
	TOD_ASSERT(aTrack->mTransformCount == mDefinition->mTracks[0].mTransformCount);
	ReanimatorTransform& aTransBefore = aTrack->mTransforms[theFrameTime->mAnimFrameBeforeInt];  // ǰһ֡�ı任����
	ReanimatorTransform& aTransAfter = aTrack->mTransforms[theFrameTime->mAnimFrameAfterInt];  // ��һ֡�ı任����

	theTransform->mTransX = FloatLerp(aTransBefore.mTransX, aTransAfter.mTransX, theFrameTime->mFraction);
	theTransform->mTransY = FloatLerp(aTransBefore.mTransY, aTransAfter.mTransY, theFrameTime->mFraction);
	theTransform->mSkewX = FloatLerp(aTransBefore.mSkewX, aTransAfter.mSkewX, theFrameTime->mFraction);
	theTransform->mSkewY = FloatLerp(aTransBefore.mSkewY, aTransAfter.mSkewY, theFrameTime->mFraction);
	theTransform->mScaleX = FloatLerp(aTransBefore.mScaleX, aTransAfter.mScaleX, theFrameTime->mFraction);
	theTransform->mScaleY = FloatLerp(aTransBefore.mScaleY, aTransAfter.mScaleY, theFrameTime->mFraction);
	theTransform->mAlpha = FloatLerp(aTransBefore.mAlpha, aTransAfter.mAlpha, theFrameTime->mFraction);
	theTransform->mImage = aTransBefore.mImage;
	theTransform->mFont = aTransBefore.mFont;
	theTransform->mText = aTransBefore.mText;

	if (aTransBefore.mFrame != -1.0f && aTransAfter.mFrame == -1.0f && theFrameTime->mFraction > 0.0f && mTrackInstances[theTrackIndex].mTruncateDisappearingFrames)
		theTransform->mFrame = -1.0f;  // ����һ���ǿհ�֡�������հ�֡ʱ������������˽ض���ʧ֡����ɾȥ���ɵĹ���
	else
		theTransform->mFrame = aTransBefore.mFrame;
}

//0x4720F0
void Reanimation::MatrixFromTransform(const ReanimatorTransform& theTransform, SexyMatrix3& theMatrix)
{
	// ����б�ĽǶ�ת��Ϊ����
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
{
	if (!gSexyAppBase->Is3DAccelerated() &&  // δ���� 3D Ӳ������
		TestBit(gReanimationParamArray[(int)mReanimationType].mReanimParamFlags, (int)ReanimFlags::REANIM_FAST_DRAW_IN_SW_MODE) &&  // ��������ʹ�������Ⱦ
		FloatApproxEqual(theTransform.m01, 0.0f) && FloatApproxEqual(theTransform.m10, 0.0f) &&  // ������������бֵ��Ϊ 0
		theTransform.m00 > 0.0f && theTransform.m11 > 0.0f &&  // ��������������ֵ������ 0
		theColor == Color::White)
	{
		float aScaleX = theTransform.m00;
		float aScaleY = theTransform.m11;
		int aPosX = FloatRoundToInt(theTransform.m02 - aScaleX * theSrcRect.mWidth * 0.5f);
		int aPosY = FloatRoundToInt(theTransform.m12 - aScaleY * theSrcRect.mHeight * 0.5f);
		int aOldMode = g->GetDrawMode();  // ����ԭ����ģʽ
		g->SetDrawMode(theDrawMode);
		Rect aOldClipRect = g->mClipRect;  // ����ԭ�ü�����
		g->SetClipRect(theClipRect);

		if (FloatApproxEqual(aScaleX, 1.0f) && FloatApproxEqual(aScaleY, 1.0f))  // ���������
			g->DrawImage(theImage, aPosX, aPosY, theSrcRect);
		else
		{
			int aWidth = FloatRoundToInt(aScaleX * theSrcRect.mWidth);
			int aHeight = FloatRoundToInt(aScaleY * theSrcRect.mHeight);
			Rect aDestRect(aPosX, aPosY, aWidth, aHeight);
			g->DrawImage(theImage, aDestRect, theSrcRect);
		}

		g->SetDrawMode(aOldMode);  // ��ԭ����ģʽ
		g->SetClipRect(aOldClipRect);  // ��ԭ�ü�����
	}
	else
		TodBltMatrix(g, theImage, theTransform, theClipRect, theColor, theDrawMode, theSrcRect);
}

//0x4723B0
// GOTY @Patoke: 0x4769B0
bool Reanimation::DrawTrack(Graphics* g, int theTrackIndex, int theRenderGroup, TodTriangleGroup* theTriangleGroup)
{
	ReanimatorTransform aTransform;
	ReanimatorTrackInstance* aTrackInstance = &mTrackInstances[theTrackIndex];  // Ŀ������ָ��
	GetCurrentTransform(theTrackIndex, &aTransform);  // ȡ�õ�ǰ�����任
	int aImageFrame = FloatRoundToInt(aTransform.mFrame);  // ͼ������ͼ�������ķ���
	if (aImageFrame < 0)  // ������ͼ��ʱ������
		return false;

	Color aColor = aTrackInstance->mTrackColor;
	if (!aTrackInstance->mIgnoreColorOverride)  // ���ǹ�����Ӷ����ĸ�д��ɫ
	{
		aColor = ColorsMultiply(aColor, mColorOverride);  // �������ɫ�붯���ĸ�д��ɫ������Ƭ���׻��
	}
	if (g->GetColorizeImages())  // �� Graphics ��ɫ
	{
		aColor = ColorsMultiply(aColor, g->GetColor());  // ����ɫ���� Graphics ����ɫ������Ƭ���׻��
	}
	int aImageAlpha = ClampInt(FloatRoundToInt(aTransform.mAlpha * aColor.mAlpha), 0, 255);
	if (aImageAlpha <= 0)  // ��ͼ����ȫ͸��ʱ������
	{
		return false;
	}
	aColor.mAlpha = aImageAlpha;

	Color aExtraAdditiveColor;
	if (mEnableExtraAdditiveDraw)  // ����������ö��������ɫ��������
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
	if (aTrackInstance->mIgnoreClipRect)  // ���������Ӳü�����
	{
		aClipRect = Rect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);  // �ü���������Ϊ��Ļ����
	}

	Image* aImage = aTransform.mImage;
	ReanimAtlasImage* aAtlasImage = nullptr;
	if (mDefinition->mReanimAtlas != nullptr && aImage != nullptr)  // ��� atlas �����ҵ�ǰ�任����ͼ��aTransform.mImage ʵ��Ϊ�����͵�ͼ����ţ�
	{
		aAtlasImage = mDefinition->mReanimAtlas->GetEncodedReanimAtlas(aImage);  // ȡ����Ӧ��ͼ������
		if (aAtlasImage != nullptr)  // ����ǺϷ���ͼ����ţ��ɹ�ȡ�ö�Ӧָ��
		{
			aImage = aAtlasImage->mOriginalImage;  // �������� Sexy::Image* ���͵���ͼ��ֵ�� aImage
		}
		if (aTrackInstance->mImageOverride != nullptr)  // ���Ŀ�������ڸ�д��ͼ
		{
			aAtlasImage = nullptr;  // ��ʹ��ͼ��
		}
	}
	SexyMatrix3 aMatrix;
	bool aFullScreen = false;
	if (aImage != nullptr)  // ���������ͼ���˴�����һ����ͼ����ŷǷ�������ܵ��±���
	{
		int aCelWidth = aImage->GetCelWidth();
		int aCelHeight = aImage->GetCelHeight();
		aMatrix.LoadIdentity();
		SexyMatrix3Translation(aMatrix, aCelWidth * 0.5f, aCelHeight * 0.5f);  // ������任�������趨����ͼ������λ��
	}
	else if (aTransform.mFont != nullptr && *aTransform.mText != '\0')  // ��������������ı���Ϊ��
	{
		aMatrix.LoadIdentity();
		int aWidth = aTransform.mFont->StringWidth(aTransform.mText);
		SexyMatrix3Translation(aMatrix, -aWidth * 0.5f, aTransform.mFont->mAscent);
	}
	else
	{
		if (stricmp(mDefinition->mTracks[theTrackIndex].mName, "fullscreen"))  // �����û��ͼ��Ҳû���ı����Ҳ���ȫ�����
			return false;  // �������
		aFullScreen = true;  // ���ȫ����������������һ����Ļ��С�ľ���
	}

	if (mDefinition->mReanimAtlas != nullptr && aAtlasImage == nullptr)  // �� atlas �����õ����
		theTriangleGroup->DrawGroup(g);  // �Ȱ�ԭ�е������������

	SexyMatrix3 aTransformMatrix;
	MatrixFromTransform(aTransform, aTransformMatrix);
	SexyMatrix3Multiply(aMatrix, aTransformMatrix, aMatrix);  // �Զ����任�������� aMatrix
	SexyMatrix3Multiply(aMatrix, mOverlayMatrix, aMatrix);  // �Զ�����д�������� aMatrix
	SexyMatrix3Translation(aMatrix, aTrackInstance->mShakeX + g->mTransX - 0.5f, aTrackInstance->mShakeY + g->mTransY - 0.5f);  // ����𶯼� g ��Ӱ��

	if (aAtlasImage != nullptr)  // �������ͼ��������������� atlas������任����ͼ�񣬹�������ڸ�д��ͼ��
	{
		Rect aSrcRect(aAtlasImage->mX, aAtlasImage->mY, aAtlasImage->mWidth, aAtlasImage->mHeight);
		aImage = mDefinition->mReanimAtlas->mMemoryImage;
		if (mFilterEffect != FilterEffect::FILTER_EFFECT_NONE)  // ������������˾�
		{
			aImage = FilterEffectGetImage(aImage, mFilterEffect);  // ȡ���˾������ͼ
		}
		theTriangleGroup->AddTriangle(g, aImage, aMatrix, aClipRect, aColor, g->mDrawMode, aSrcRect);  // �������������������
		if (mEnableExtraAdditiveDraw && !aTrackInstance->mIgnoreExtraAdditiveColor)  // ����������ڶ��������ɫ�ҹ����������֮
		{
			theTriangleGroup->AddTriangle(g, aImage, aMatrix, aClipRect, aExtraAdditiveColor, Graphics::DRAWMODE_ADDITIVE, aSrcRect);
		}
		if (mEnableExtraOverlayDraw)
		{
			theTriangleGroup->AddTriangle(
				g, FilterEffectGetImage(aImage, FilterEffect::FILTER_EFFECT_WHITE), aMatrix, aClipRect, aExtraOverlayColor, Graphics::DRAWMODE_NORMAL, aSrcRect);
		}
	}
	else if (aImage != nullptr)  // ��������� atlas ������任����ͼ��
	{
		if (aTrackInstance->mImageOverride != nullptr)  // ���������ڸ�д��ͼ
		{
			aImage = aTrackInstance->mImageOverride;  // ����ͼ�滻Ϊ��д��ͼ
		}
		if (mFilterEffect != FilterEffect::FILTER_EFFECT_NONE)  // ������������˾�
		{
			aImage = FilterEffectGetImage(aImage, mFilterEffect);  // ����ͼ�滻Ϊ�˾������ͼ
		}
		while (aImageFrame >= aImage->mNumCols)
		{
			aImageFrame -= aImage->mNumCols;  // ȷ�����Ƶ��������ᳬ����ͼ���һ��
		}

		int aCelWidth = aImage->GetCelWidth();
		Rect aSrcRect(aImageFrame * aCelWidth, 0, aCelWidth, aImage->GetCelHeight());
		ReanimBltMatrix(g, aImage, aMatrix, aClipRect, aColor, g->mDrawMode, aSrcRect);  // ��������ƹ��ͼ��
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
	else if (aTransform.mFont != nullptr && *aTransform.mText != '\0')  // ���������ͼ�񵫴����ı�
	{
		TodDrawStringMatrix(g, aTransform.mFont, aMatrix, aTransform.mText, aColor);
		if (mEnableExtraAdditiveDraw)
		{
			int aOldMode = g->GetDrawMode();  // ���ݻ���ģʽ
			g->SetDrawMode(Graphics::DRAWMODE_ADDITIVE);
			TodDrawStringMatrix(g, aTransform.mFont, aMatrix, aTransform.mText, aExtraAdditiveColor);
			g->SetDrawMode(aOldMode);  // ��ԭ����ģʽ
		}
	}
	else if (aFullScreen)  // ������ͼ����ı�������ȫ��
	{
		Color aOldColor = g->GetColor();  // ������ɫ
		g->SetColor(aColor);
		g->FillRect(-g->mTransX, -g->mTransY, BOARD_WIDTH, BOARD_HEIGHT);
		g->SetColor(aOldColor);  // ��ԭ��ɫ
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
	if (mDefinition->mReanimAtlas != nullptr && aImage != nullptr)  // �������ͼ���Ҵ���ͼ�񣨷��򷵻ص� aImage Ϊ nullptr��
	{
		ReanimAtlasImage* aAtlasImage = mDefinition->mReanimAtlas->GetEncodedReanimAtlas(aImage);  // ȡ����Ӧ��ͼ������
		if (aAtlasImage != nullptr)
			aImage = aAtlasImage->mOriginalImage;  // ����ͼ����Ӧ��ԭ��ͼ
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
	if (mDefinition->mReanimAtlas != nullptr && aImage != nullptr)  // �������ͼ���Ҵ���ͼ�񣨷��򷵻ص� aImage Ϊ nullptr��
	{
		ReanimAtlasImage* aAtlasImage = mDefinition->mReanimAtlas->GetEncodedReanimAtlas(aImage);  // ȡ����Ӧ��ͼ������
		if (aAtlasImage != nullptr)
			aImage = aAtlasImage->mOriginalImage;  // ����ͼ����Ӧ��ԭ��ͼ
	}

	theMatrix.LoadIdentity();
	if (aImage != nullptr && aImageFrame >= 0)
	{
		int aCelWidth = aImage->GetCelWidth();
		int aCelHeight = aImage->GetCelHeight();
		SexyMatrix3Translation(theMatrix, aCelWidth * 0.5f, aCelHeight * 0.5f);  // ������任�������趨����ͼ������λ��
	}
	else if (aTransform.mFont != nullptr && *aTransform.mText != '\0')
		SexyMatrix3Translation(theMatrix, 0.0f, aTransform.mFont->mAscent);

	SexyTransform2D aTransformMatrix;
	MatrixFromTransform(aTransform, aTransformMatrix);
	SexyMatrix3Multiply(theMatrix, aTransformMatrix, theMatrix);  // �Զ����任�������� theMatrix
	SexyMatrix3Multiply(theMatrix, mOverlayMatrix, theMatrix);  // �Զ�����д�������� theMatrix
	SexyMatrix3Translation(theMatrix, aTrackInstance->mShakeX - 0.5f, aTrackInstance->mShakeY - 0.5f);  // ����𶯵�Ӱ��
}

//0x472D90
void Reanimation::GetFrameTime(ReanimatorFrameTime* theFrameTime)
{
	TOD_ASSERT(mFrameStart + mFrameCount <= mDefinition->mTracks[0].mTransformCount);
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
	if (theFrameTime->mAnimFrameBeforeInt >= mFrameStart + mFrameCount - 1)  // �����ǰ���ڽ�����һ֡
	{
		theFrameTime->mAnimFrameBeforeInt = mFrameStart + mFrameCount - 1;
		theFrameTime->mAnimFrameAfterInt = theFrameTime->mAnimFrameBeforeInt;  // ��ǰ���������֡����ֵΪ���һ֡
	}
	else
		theFrameTime->mAnimFrameAfterInt = theFrameTime->mAnimFrameBeforeInt + 1;  // ��һ����֡����ǰһ����֡�ĺ�һ֡
	TOD_ASSERT(theFrameTime->mAnimFrameBeforeInt >= 0 && theFrameTime->mAnimFrameAfterInt < mDefinition->mTracks[0].mTransformCount);
}

//0x472E40
void Reanimation::DrawRenderGroup(Graphics* g, int theRenderGroup)
{
	if (mDead)
		return;

	TodTriangleGroup aTriangleGroup;
	for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTrackCount; aTrackIndex++)
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
	for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTrackCount; aTrackIndex++)
		if (stricmp(mDefinition->mTracks[aTrackIndex].mName, theTrackName) == 0)
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
	if (theAttachReanim->mDefinition->mTrackCount <= 0)
		return;

	if (theAttachReanim->mFrameBasePose == -1)
		theAttachReanim->mFrameBasePose = theAttachReanim->mFrameStart;  // ����ǰ��������ʼ֡��Ϊ�任��׼֡
	AttachReanim(theAttachReanim->GetTrackInstanceByName(theTrackName)->mAttachmentID, this, 0.0f, 0.0f);
}

void Reanimation::SetBasePoseFromAnim(const char* theTrackName)
{
	int aFrameStart, aFrameCount;
	GetFramesForLayer(theTrackName, aFrameStart, aFrameCount);
	mFrameBasePose = aFrameStart;  // ����ǰ�����������ʼ֡��Ϊ�任��׼֡
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
	GetTrackBasePoseMatrix(aTrackIndex, aBasePoseMatrix);  // ȡ�ù��������̬�ı任����
	SexyVector2 aPosition = aBasePoseMatrix * SexyVector2(thePosX, thePosY);  // �Ի�����̬�ľ���任λ������
	return AttachParticle(aTrackInstance->mAttachmentID, theParticleSystem, aPosition.x, aPosition.y);
}

//0x473110
void Reanimation::GetAttachmentOverlayMatrix(int theTrackIndex, SexyTransform2D& theOverlayMatrix)
{
	ReanimatorTransform aTransform;
	GetCurrentTransform(theTrackIndex, &aTransform);  // ȡ�ú���ϡ�������д����Ȼ�任
	SexyTransform2D aTransformMatrix;
	MatrixFromTransform(aTransform, aTransformMatrix);
	SexyMatrix3Multiply(aTransformMatrix, mOverlayMatrix, aTransformMatrix);  // �Զ�����д���������ڶ����任����

	SexyTransform2D aBasePoseMatrix;
	GetTrackBasePoseMatrix(theTrackIndex, aBasePoseMatrix);  // ȡ�ù��������̬�ı任����
	SexyTransform2D aBasePoseMatrixInv;
	SexyMatrix3Inverse(aBasePoseMatrix, aBasePoseMatrixInv);  // ȡ�û�����̬�������
	theOverlayMatrix = aTransformMatrix * aBasePoseMatrixInv;
}

//0x4731D0
void Reanimation::GetFramesForLayer(const char* theTrackName, int& theFrameStart, int& theFrameCount)
{
	if (mDefinition->mTrackCount == 0)  // �������û�й��
	{
		theFrameStart = 0;
		theFrameCount = 0;
		return;
	}

	int aTrackIndex = FindTrackIndex(theTrackName);
	TOD_ASSERT(aTrackIndex >= 0 && aTrackIndex < mDefinition->mTrackCount);
	ReanimatorTrack* aTrack = &mDefinition->mTracks[aTrackIndex];
	theFrameStart = 0;
	theFrameCount = 1;
	for (int i = 0; i < aTrack->mTransformCount; i++)
		if (aTrack->mTransforms[i].mFrame >= 0.0f)
		{
			theFrameStart = i;  // ȡ����ϵ��׸��ǿհ�֡��Ϊ��ʼ֡
			break;
		}
	for (int j = theFrameStart; j < aTrack->mTransformCount; j++)
		if (aTrack->mTransforms[j].mFrame >= 0.0f)
			theFrameCount = j - theFrameStart + 1;  // ȡ����ʼ֡��������һ���ǿհ�֮֡��Ϊ֡����
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
	for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTrackCount; aTrackIndex++)
		if (stricmp(mDefinition->mTracks[aTrackIndex].mName, theTrackName) == 0)
			return true;
	return false;
}

//0x473310
void Reanimation::StartBlend(int theBlendTime)
{
	for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTrackCount; aTrackIndex++)
	{
		ReanimatorTransform aTransform;
		GetCurrentTransform(aTrackIndex, &aTransform);
		if (FloatRoundToInt(aTransform.mFrame) >= 0)  // ����ǰ�����ǰ�����ڿհ�֡
		{
			ReanimatorTrackInstance* aTrackInstance = &mTrackInstances[aTrackIndex];
			aTrackInstance->mBlendTransform = aTransform;  // ��¼��ǰ�任Ϊ��ϵĳ�ʼ��Դ���任
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
		for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTrackCount; aTrackIndex++)
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
void Reanimation::SetImageOverride(const char* theTrackName, Image* theImage)
{
	GetTrackInstanceByName(theTrackName)->mImageOverride = theImage;
}

//0x4734B0
void Reanimation::SetTruncateDisappearingFrames(const char* theTrackName, bool theTruncateDisappearingFrames)
{
	if (theTrackName == nullptr)  // �������Ĺ������Ϊ��ָ��
	{
		for (int aTrackIndex = 0; aTrackIndex < mDefinition->mTrackCount; aTrackIndex++)  // ��������ÿһ���
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
	if (aReanimDef->mTracks != nullptr)  // ������ָ�벻Ϊ��ָ�룬˵�����������Ѿ�����
		return;
	ReanimationParams* aReanimParams = &gReanimationParamArray[(int)theReanimType];
	if (theIsPreloading)
	{
		if (gSexyAppBase->mShutdown || gAppCloseRequest())  // Ԥ����ʱ�������˳�����ȡ������
			return;
	}
	else  // < ���²��ֽ��ڲ��ִ�� >
	{
		if (gAppHasUsedCheatKeys())
			TodTraceAndLog("Cheater failed to preload '%s' on %s", aReanimParams->mReanimFileName, gGetCurrentLevelName());
		else
			TodTraceAndLog("Non-cheater failed to preload '%s' on %s", aReanimParams->mReanimFileName, gGetCurrentLevelName());
	}  // < ���ϲ��ֽ��ڲ��ִ�� >

	PerfTimer aTimer;
	aTimer.Start();
	TodHesitationBracket aHesitation("Load Reanim '%s'", aReanimParams->mReanimFileName);
	if (!ReanimationLoadDefinition(aReanimParams->mReanimFileName, aReanimDef))
	{
		char aBuf[1024];
		sprintf_s<1024U>(aBuf, "Failed to load reanim '%s'", aReanimParams->mReanimFileName);
		TodErrorMessageBox(aBuf, "Error");
	}
	int aDuration = aTimer.GetDuration();
	if (aDuration > 100)  //�����ڲ�棩����ʱ������ı���
		TodTraceAndLog("LOADING:Long reanim '%s' %d ms on %s", aReanimParams->mReanimFileName, aDuration, gGetCurrentLevelName().c_str());
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

	for (int i = 0; i < gReanimationParamArraySize; i++)
	{
		ReanimationParams* aReanimationParams = &theReanimationParamArray[i];
		TOD_ASSERT(aReanimationParams->mReanimationType == i);
		if (DefinitionIsCompiled(StringToSexyString(aReanimationParams->mReanimFileName)))
			ReanimatorEnsureDefinitionLoaded(aReanimationParams->mReanimationType, true);
	}
}

//0x473870
void ReanimatorFreeDefinitions()
{
	for (int i = 0; i < gReanimatorDefCount; i++)
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
	TOD_ASSERT(aTrackIndex >= 0 && aTrackIndex < mDefinition->mTrackCount);

	ReanimatorTrack* aTrack = &mDefinition->mTracks[aTrackIndex];
	float aDis = aTrack->mTransforms[aFrameTime.mAnimFrameAfterInt].mTransX - aTrack->mTransforms[aFrameTime.mAnimFrameBeforeInt].mTransX;
	return aDis * SECONDS_PER_UPDATE * mAnimRate;  // ˲ʱ���� = ��֡��ĺ�����֮�� * һ֡��ʱ�� * ��������
}

//0x473930
bool Reanimation::IsTrackShowing(const char* theTrackName)
{
	ReanimatorFrameTime aFrameTime;
	GetFrameTime(&aFrameTime);
	int aTrackIndex = FindTrackIndex(theTrackName);
	TOD_ASSERT(aTrackIndex >= 0 && aTrackIndex < mDefinition->mTrackCount);

	return mDefinition->mTracks[aTrackIndex].mTransforms[aFrameTime.mAnimFrameAfterInt].mFrame >= 0.0f;  // ������һ����֡�Ƿ����ͼ��
}

//0x473980
void Reanimation::ShowOnlyTrack(const char* theTrackName)
{
	for (int i = 0; i < mDefinition->mTrackCount; i++)
	{
		// �������ָ��������ͬʱ�����ù����Ⱦ����Ϊ������ʾ���������ù����Ⱦ����Ϊ����
		mTrackInstances[i].mRenderGroup = stricmp(mDefinition->mTracks[i].mName, theTrackName) == 0 ? RENDER_GROUP_NORMAL : RENDER_GROUP_HIDDEN;
	}
}

//0x4739E0
// GOTY @Patoke: 0x478120
void Reanimation::AssignRenderGroupToTrack(const char* theTrackName, int theRenderGroup)
{
	for (int i = 0; i < mDefinition->mTrackCount; i++)
		if (stricmp(mDefinition->mTracks[i].mName, theTrackName) == 0)
		{
			mTrackInstances[i].mRenderGroup = theRenderGroup;  // �������׸�����ǡ��Ϊ theTrackName �Ĺ��
			return;
		}
}

//0x473A40
void Reanimation::AssignRenderGroupToPrefix(const char* theTrackName, int theRenderGroup)
{
	size_t aPrifixLength = strlen(theTrackName);
	for (int i = 0; i < mDefinition->mTrackCount; i++)
	{
		const char* const aTrackName = mDefinition->mTracks[i].mName;
		if (strlen(aTrackName) >= aPrifixLength && !strnicmp(aTrackName, theTrackName, aPrifixLength))  // ������Ƴ��ȱ��벻С��ָ��ǰ׺����
			mTrackInstances[i].mRenderGroup = theRenderGroup;
	}
}

//0x473AE0
void Reanimation::PropogateColorToAttachments()
{
	for (int i = 0; i < mDefinition->mTrackCount; i++)
		AttachmentPropogateColor(
			mTrackInstances[i].mAttachmentID, mColorOverride, mEnableExtraAdditiveDraw, mExtraAdditiveColor, mEnableExtraOverlayDraw, mExtraOverlayColor
		);
}

//0x473B70
bool Reanimation::ShouldTriggerTimedEvent(float theEventTime)
{
	TOD_ASSERT(theEventTime >= 0.0f && theEventTime <= 1.0f);
	if (mFrameCount == 0 || mLastFrameTime <= 0.0f || mAnimRate <= 0.0f)  // û�ж����򵹷Ż�δ����
		return false;

	if (mAnimTime >= mLastFrameTime)  // һ������£��ɴ����ķ�ΧΪ [mLastFrameTime, mAnimTime]
		return theEventTime >= mLastFrameTime && theEventTime < mAnimTime;
	else  // �������������һ��ѭ�������½�����һ��ѭ������ɴ����ķ�ΧΪ [0, mAnimTime] �� [mLastFrameTime, 1]
		return theEventTime >= mLastFrameTime || theEventTime < mAnimTime;
}
//0x473BF0
// GOTY @Patoke: 0x478310
void Reanimation::PlayReanim(const char* theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate)
{
	if (theBlendTime > 0)  // ����Ҫ�������ʱ����ʼ���
		StartBlend(theBlendTime);
	if (theAnimRate != 0.0f)  // ��ָ��������Ϊ 0 ʱ����ʾ���ı�ԭ�ж�������
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
	if (theTransform.mFrame == -1.0f)  // ����ǿհ�֡
		return;

	/* ����������Ƹ�ʽ��attacher__REANIMNAME__TRACKNAME[TAG1][TAG2]���� */

	const char* aReanimName = strstr(theTransform.mText, "__");  // ָ�򶯻�����ǰ��˫�»���
	if (aReanimName == nullptr)  // ����ַ����в���˫�»���
		return;
	const char* aTags = strstr(aReanimName + 2, "[");  // ��������֮��ָ�� TAG ǰ��������
	const char* aTrackName = strstr(aReanimName + 2, "__");  // ��������֮��ָ��������ǰ��˫�»���
	if (aTags && aTrackName && (unsigned int)aTags < (unsigned int)aTrackName)  // �����[��֮����˫�»��ߣ����ַ����Ƿ�
		return;

	if (aTrackName)  // ����ж���������
	{
		theAttacherInfo.mReanimName.assign(aReanimName + 2, aTrackName - aReanimName - 2);  // ȡ����˫�»���֮��Ĳ��֣�REANIMNAME��
		if (aTags)  // ����ж����ǩ
			theAttacherInfo.mTrackName.assign(aTrackName + 2, aTags - aTrackName - 2);  // ȡ�� TAG ��������֮ǰ
		else
			theAttacherInfo.mTrackName.assign(aTrackName + 2);  // ȡ���ַ�����β
	}
	else if (aTags)  // ���δ���������Ƶ������˱�ǩ
		theAttacherInfo.mReanimName.assign(aReanimName + 2, aTags - aReanimName - 2);  // ȡ˫�»�����������֮��Ĳ���
	else  // ���ֻ�����˹������
		theAttacherInfo.mReanimName.assign(aReanimName + 2);  // ��˫�»���֮��ȡ���ַ�����β

	while (aTags)  // ��ȡÿ�� TAG
	{
		const char* aTagEnds = strstr(aTags + 1, "]");
		if (aTagEnds == nullptr)  // ���û����������
			break;
		
		std::string aCode(aTags + 1, aTagEnds - aTags - 1);  // ȡ�������ڵ��ı�
		if (sscanf_s(aCode.c_str(), "%f", &theAttacherInfo.mAnimRate) != 1)  // ���Խ��ı���Ϊ������ɨ�裬���ɨ��ɹ��򽫽����Ϊ��������
		{
			if (aCode.compare("hold") == 0)
				theAttacherInfo.mLoopType = ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD;
			else if (aCode.compare("once") == 0)
				theAttacherInfo.mLoopType = ReanimLoopType::REANIM_PLAY_ONCE;
		}

		aTags = strstr(aTagEnds + 1, "[");  // ����Ѱ����һ�� TAG ����������
	}
}

//0x473EB0
void Reanimation::AttacherSynchWalkSpeed(int theTrackIndex, Reanimation* theAttachReanim, AttacherInfo& theAttacherInfo)
{
	ReanimatorTrack* aTrack = &mDefinition->mTracks[theTrackIndex];
	ReanimatorFrameTime aFrameTime;
	GetFrameTime(&aFrameTime);

	int aPlaceHolderFrameStart = aFrameTime.mAnimFrameBeforeInt;
	while (aPlaceHolderFrameStart > mFrameStart && aTrack->mTransforms[aPlaceHolderFrameStart - 1].mText == aTrack->mTransforms[aPlaceHolderFrameStart].mText)
		aPlaceHolderFrameStart--;  // ȡ��ǰ��������ĵ�һ֡
	int aPlaceHolderFrameEnd = aFrameTime.mAnimFrameBeforeInt;
	while (aPlaceHolderFrameEnd < mFrameStart + mFrameCount - 1 && aTrack->mTransforms[aPlaceHolderFrameEnd + 1].mText == aTrack->mTransforms[aPlaceHolderFrameEnd].mText)
		aPlaceHolderFrameEnd++;  // ȡ��ǰ������������һ֡
	int aPlaceHolderFrameCount = aPlaceHolderFrameEnd - aPlaceHolderFrameStart;
	ReanimatorTransform& aPlaceHolderStartTrans = aTrack->mTransforms[aPlaceHolderFrameStart];
	ReanimatorTransform& aPlaceHolderEndTrans = aTrack->mTransforms[aPlaceHolderFrameEnd];
	if (FloatApproxEqual(mAnimRate, 0.0f))  // ����������������Ϊ 0
	{
		theAttachReanim->mAnimRate = 0.0f;  // ��������������ҲΪ 0
		return;
	}
	float aPlaceHolderDistance = -(aPlaceHolderEndTrans.mTransX - aPlaceHolderStartTrans.mTransX);  // ռλ����ڵ�ǰ�����ڵ�λ��
	float aPlaceHolderSeconds = aPlaceHolderFrameCount / mAnimRate;  // ռλ����ڵ�ǰ�����ڵ�ʱ��
	if (FloatApproxEqual(aPlaceHolderSeconds, 0.0f))  // �����ǰ�������䲻�����κ�֡
	{
		theAttachReanim->mAnimRate = 0.0f;  // ��������������Ϊ 0
		return;
	}

	int aGroundTrackIndex = theAttachReanim->FindTrackIndex("_ground");
	ReanimatorTrack* aGroundTrack = &theAttachReanim->mDefinition->mTracks[aGroundTrackIndex];
	ReanimatorTransform& aTransformGuyStart = aGroundTrack->mTransforms[theAttachReanim->mFrameStart];
	ReanimatorTransform& aTransformGuyEnd = aGroundTrack->mTransforms[theAttachReanim->mFrameStart + theAttachReanim->mFrameCount - 1];
	float aGuyDistance = aTransformGuyEnd.mTransX - aTransformGuyStart.mTransX;  // ʵ�ʶ������������������ڵ�λ��
	if (aGuyDistance < FLT_EPSILON || aPlaceHolderDistance < FLT_EPSILON)  // ���ռλλ��Ϊ 0 ��ʵ�ʶ�������λ��Ϊ 0�����������޷��ƶ�
	{
		theAttachReanim->mAnimRate = 0.0f;  // ��������������Ϊ 0
		return;
	}

	float aLoops = aPlaceHolderDistance / aGuyDistance;  // �Ը�������Ŀ��λ�ƣ�ռλλ�ƣ�����������λ�ƣ��õ�����������Ҫѭ����������
	ReanimatorTransform aTransformGuyCurrent;
	theAttachReanim->GetCurrentTransform(aGroundTrackIndex, &aTransformGuyCurrent);
	AttachEffect* aAttachEffect = FindFirstAttachment(mTrackInstances[theTrackIndex].mAttachmentID);
	if (aAttachEffect != nullptr)
	{
		float aGuyCurrentDistance = aTransformGuyCurrent.mTransX - aTransformGuyStart.mTransX;  // �����������������ڵ�ǰ�Ѿ�����λ��
		float aGuyExpectedDistance = aGuyDistance * theAttachReanim->mAnimTime;  // �������˶���ռλ�������ġ�����������ǰ������λ��
		aAttachEffect->mOffset.m02 = aGuyExpectedDistance - aGuyCurrentDistance;  // ��������Ч���ĺ���任��ʹ����������λ�Ʊ�����ռλ����һ��
	}
	theAttachReanim->mAnimRate = aLoops * theAttachReanim->mFrameCount / aPlaceHolderSeconds;  // ���� = ��Ҫ���ŵ�֡�� �� ���Բ��ŵ�ʱ��
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
	if (aAttacherInfo.mReanimName.size() != 0)  // �����������趨�˵�ǰ�ĸ�����������
	{
		std::string aReanimFileName = StrFormat("reanim\\%s.reanim", aAttacherInfo.mReanimName.c_str());
		for (int i = 0; i < gReanimationParamArraySize; i++)  // �ڶ�������������Ѱ�Ҷ����ļ�����Ӧ�Ķ�������
		{
			ReanimationParams* aParams = &gReanimationParamArray[i];
			if (stricmp(aReanimFileName.c_str(), aParams->mReanimFileName) == 0)
			{
				aReanimationType = aParams->mReanimationType;
				break;
			}
		}
	}
	if (aReanimationType == ReanimationType::REANIM_NONE)  // ���û���趨��ǰ�����������ƣ���δ�ҵ���Ӧ�Ķ���
	{
		AttachmentDie(aTrackInstance->mAttachmentID);  // �������
		return;
	}

	Reanimation* aAttachReanim = FindReanimAttachment(aTrackInstance->mAttachmentID);
	if (aAttachReanim == nullptr || aAttachReanim->mReanimationType != aReanimationType)  // ���ԭ��û�и�����������ԭ�����������������趨�Ķ���
	{
		AttachmentDie(aTrackInstance->mAttachmentID);  // ���ԭ�и���
		aAttachReanim = gEffectSystem->mReanimationHolder->AllocReanimation(0.0f, 0.0f, 0, aReanimationType);  // ���´���һ��ָ���Ķ���
		aAttachReanim->mLoopType = aAttacherInfo.mLoopType;
		aAttachReanim->mAnimRate = aAttacherInfo.mAnimRate;
		AttachReanim(aTrackInstance->mAttachmentID, aAttachReanim, 0.0f, 0.0f);
		mFrameBasePose = NO_BASE_POSE;  // �趨���������������ٴ��ڻ�׼֡
	}

	if (aAttacherInfo.mTrackName.size() != 0)  // ��������˸��������Ķ������
	{
		int aAnimFrameStart, aAnimFrameCount;
		aAttachReanim->GetFramesForLayer(aAttacherInfo.mTrackName.c_str(), aAnimFrameStart, aAnimFrameCount);
		if (aAttachReanim->mFrameStart != aAnimFrameStart || aAttachReanim->mFrameCount != aAnimFrameCount)  // if (!aAttachReanim->IsAnimPlaying(����))
		{
			aAttachReanim->StartBlend(20);
			aAttachReanim->SetFramesForLayer(aAttacherInfo.mTrackName.c_str());  // ����ָ������ϵĶ���
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

	for (int i = 0; i < mDefinition->mTrackCount; i++)
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
