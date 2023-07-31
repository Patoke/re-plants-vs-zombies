#include "TodFoley.h"
#include "TodDebug.h"
#include "TodCommon.h"
#include "../SexyAppFramework/SoundManager.h"

int gFoleyParamArraySize;        //[0x6A9F04]
FoleyParams* gFoleyParamArray;   //[0x6A9F00]

FoleyParams gLawnFoleyParamArray[(int)FoleyType::NUM_FOLEY] = {  //0x69FAD0
	{ FoleyType::FOLEY_SUN,                     10.0f,  { &Sexy::SOUND_POINTS}, 0U },
	{ FoleyType::FOLEY_SPLAT,                   10.0f,  { &Sexy::SOUND_SPLAT, &Sexy::SOUND_SPLAT2, &Sexy::SOUND_SPLAT3}, 0U },
	{ FoleyType::FOLEY_LAWNMOWER,               10.0f,  { &Sexy::SOUND_LAWNMOWER}, 0U },
	{ FoleyType::FOLEY_THROW,                   10.0f,  { &Sexy::SOUND_THROW, &Sexy::SOUND_THROW, &Sexy::SOUND_THROW, &Sexy::SOUND_THROW2 }, 0U },
	{ FoleyType::FOLEY_SPAWN_SUN,               10.0f,  { &Sexy::SOUND_THROW}, 0U },
	{ FoleyType::FOLEY_CHOMP,                   0.0f,   { &Sexy::SOUND_CHOMP, &Sexy::SOUND_CHOMP2}, 0U },
	{ FoleyType::FOLEY_CHOMP_SOFT,              4.0f,   { &Sexy::SOUND_CHOMPSOFT}, 0U },
	{ FoleyType::FOLEY_PLANT,                   0.0f,   { &Sexy::SOUND_PLANT, &Sexy::SOUND_PLANT2}, 0U },
	{ FoleyType::FOLEY_USE_SHOVEL,              0.0f,   { &Sexy::SOUND_PLANT2}, 0U },
	{ FoleyType::FOLEY_DROP,                    0.0f,   { &Sexy::SOUND_TAP2}, 0U },
	{ FoleyType::FOLEY_BLEEP,                   0.0f,   { &Sexy::SOUND_BLEEP}, 0U },
	{ FoleyType::FOLEY_GROAN,                   0.0f,   { &Sexy::SOUND_GROAN, &Sexy::SOUND_GROAN2, &Sexy::SOUND_GROAN3, &Sexy::SOUND_GROAN4, &Sexy::SOUND_GROAN5, &Sexy::SOUND_GROAN6 }, 0U },
	{ FoleyType::FOLEY_BRAINS,                  0.0f,   { &Sexy::SOUND_GROAN, &Sexy::SOUND_GROAN2, &Sexy::SOUND_GROAN3, &Sexy::SOUND_GROAN4, &Sexy::SOUND_GROAN5, &Sexy::SOUND_GROAN6, &Sexy::SOUND_SUKHBIR4, &Sexy::SOUND_SUKHBIR5, &Sexy::SOUND_SUKHBIR6 }, 0U },
	{ FoleyType::FOLEY_SUKHBIR,                 0.0f,   { &Sexy::SOUND_GROAN, &Sexy::SOUND_GROAN2, &Sexy::SOUND_GROAN3, &Sexy::SOUND_GROAN4, &Sexy::SOUND_GROAN5, &Sexy::SOUND_GROAN6, &Sexy::SOUND_SUKHBIR, &Sexy::SOUND_SUKHBIR2, &Sexy::SOUND_SUKHBIR3 }, 0U },
	{ FoleyType::FOLEY_JACKINTHEBOX,            0.0f,   { &Sexy::SOUND_JACKINTHEBOX}, 7U },
	{ FoleyType::FOLEY_ART_CHALLENGE,           0.0f,   { &Sexy::SOUND_DIAMOND}, 0U },
	{ FoleyType::FOLEY_ZAMBONI,                 5.0f,   { &Sexy::SOUND_ZAMBONI}, 0U },
	{ FoleyType::FOLEY_THUNDER,                 10.0f,  { &Sexy::SOUND_THUNDER}, 0U },
	{ FoleyType::FOLEY_FROZEN,                  0.0f,   { &Sexy::SOUND_FROZEN}, 0U },
	{ FoleyType::FOLEY_ZOMBIESPLASH,            10.0f,  { &Sexy::SOUND_PLANT_WATER, &Sexy::SOUND_ZOMBIE_ENTERING_WATER}, 0U },
	{ FoleyType::FOLEY_BOWLINGIMPACT,           -3.0f,  { &Sexy::SOUND_BOWLINGIMPACT}, 0U },
	{ FoleyType::FOLEY_SQUISH,                  0.0f,   { &Sexy::SOUND_CHOMP, &Sexy::SOUND_CHOMP2}, 0U },
	{ FoleyType::FOLEY_TIRE_POP,                0.0f,   { &Sexy::SOUND_BALLOON_POP}, 0U },
	{ FoleyType::FOLEY_EXPLOSION,               0.0f,   { &Sexy::SOUND_EXPLOSION}, 0U },
	{ FoleyType::FOLEY_SLURP,                   2.0f,   { &Sexy::SOUND_SLURP}, 0U },
	{ FoleyType::FOLEY_LIMBS_POP,               10.0f,  { &Sexy::SOUND_LIMBS_POP}, 0U },
	{ FoleyType::FOLEY_POGO_ZOMBIE,             4.0f,   { &Sexy::SOUND_POGO_ZOMBIE}, 0U },
	{ FoleyType::FOLEY_SNOW_PEA_SPARKLES,       10.0f,  { &Sexy::SOUND_SNOW_PEA_SPARKLES}, 0U },
	{ FoleyType::FOLEY_ZOMBIE_FALLING,          10.0f,  { &Sexy::SOUND_ZOMBIE_FALLING_1, &Sexy::SOUND_ZOMBIE_FALLING_2}, 0U },
	{ FoleyType::FOLEY_PUFF,                    10.0f,  { &Sexy::SOUND_PUFF}, 0U },
	{ FoleyType::FOLEY_FUME,                    10.0f,  { &Sexy::SOUND_FUME}, 0U },
	{ FoleyType::FOLEY_COIN,                    10.0f,  { &Sexy::SOUND_COIN}, 0U },
	{ FoleyType::FOLEY_KERNEL_SPLAT,            10.0f,  { &Sexy::SOUND_KERNELPULT, &Sexy::SOUND_KERNELPULT2}, 0U },
	{ FoleyType::FOLEY_DIGGER,                  0.0f,   { &Sexy::SOUND_DIGGER_ZOMBIE}, 7U },
	{ FoleyType::FOLEY_JACK_SURPRISE,           1.0f,   { &Sexy::SOUND_JACK_SURPRISE, &Sexy::SOUND_JACK_SURPRISE, &Sexy::SOUND_JACK_SURPRISE2}, 0U },
	{ FoleyType::FOLEY_VASE_BREAKING,           -5.0f,  { &Sexy::SOUND_VASE_BREAKING}, 0U },
	{ FoleyType::FOLEY_POOL_CLEANER,            2.0f,   { &Sexy::SOUND_POOL_CLEANER}, 0U },
	{ FoleyType::FOLEY_BASKETBALL,              10.0f,  { &Sexy::SOUND_BASKETBALL}, 0U },
	{ FoleyType::FOLEY_IGNITE,                  5.0f,   { &Sexy::SOUND_IGNITE, &Sexy::SOUND_IGNITE, &Sexy::SOUND_IGNITE, &Sexy::SOUND_IGNITE2}, 0U },
	{ FoleyType::FOLEY_FIREPEA,                 10.0f,  { &Sexy::SOUND_FIREPEA}, 0U },
	{ FoleyType::FOLEY_THUMP,                   2.0f,   { &Sexy::SOUND_GARGANTUAR_THUMP}, 0U },
	{ FoleyType::FOLEY_SQUASH_HMM,              2.0f,   { &Sexy::SOUND_SQUASH_HMM, &Sexy::SOUND_SQUASH_HMM, &Sexy::SOUND_SQUASH_HMM2}, 0U },
	{ FoleyType::FOLEY_MAGNETSHROOM,            2.0f,   { &Sexy::SOUND_MAGNETSHROOM}, 0U },
	{ FoleyType::FOLEY_BUTTER,                  2.0f,   { &Sexy::SOUND_BUTTER}, 0U },
	{ FoleyType::FOLEY_BUNGEE_SCREAM,           2.0f,   { &Sexy::SOUND_BUNGEE_SCREAM, &Sexy::SOUND_BUNGEE_SCREAM2, &Sexy::SOUND_BUNGEE_SCREAM3}, 0U },
	{ FoleyType::FOLEY_BOSS_EXPLOSION_SMALL,    2.0f,   { &Sexy::SOUND_EXPLOSION}, 0U },
	{ FoleyType::FOLEY_SHIELD_HIT,              10.0f,  { &Sexy::SOUND_SHIELDHIT, &Sexy::SOUND_SHIELDHIT2}, 0U },
	{ FoleyType::FOLEY_SWING,                   2.0f,   { &Sexy::SOUND_SWING}, 0U },
	{ FoleyType::FOLEY_BONK,                    2.0f,   { &Sexy::SOUND_BONK}, 0U },
	{ FoleyType::FOLEY_RAIN,                    0.0f,   { &Sexy::SOUND_RAIN}, 5U },
	{ FoleyType::FOLEY_DOLPHIN_BEFORE_JUMPING,  0.0f,   { &Sexy::SOUND_DOLPHIN_BEFORE_JUMPING}, 0U },
	{ FoleyType::FOLEY_DOLPHIN_APPEARS,         0.0f,   { &Sexy::SOUND_DOLPHIN_APPEARS}, 0U },
	{ FoleyType::FOLEY_PLANT_WATER,             0.0f,   { &Sexy::SOUND_PLANT_WATER}, 0U },
	{ FoleyType::FOLEY_ZOMBIE_ENTERING_WATER,   0.0f,   { &Sexy::SOUND_ZOMBIE_ENTERING_WATER}, 0U },
	{ FoleyType::FOLEY_GRAVEBUSTERCHOMP,        0.0f,   { &Sexy::SOUND_GRAVEBUSTERCHOMP}, 4U },
	{ FoleyType::FOLEY_CHERRYBOMB,              0.0f,   { &Sexy::SOUND_CHERRYBOMB}, 0U },
	{ FoleyType::FOLEY_JALAPENO_IGNITE,         0.0f,   { &Sexy::SOUND_JALAPENO}, 0U },
	{ FoleyType::FOLEY_REVERSE_EXPLOSION,       0.0f,   { &Sexy::SOUND_REVERSE_EXPLOSION}, 0U },
	{ FoleyType::FOLEY_PLASTIC_HIT,             5.0f,   { &Sexy::SOUND_PLASTICHIT, &Sexy::SOUND_PLASTICHIT2}, 0U },
	{ FoleyType::FOLEY_WINMUSIC,                0.0f,   { &Sexy::SOUND_WINMUSIC}, 8U },
	{ FoleyType::FOLEY_BALLOONINFLATE,          10.0f,  { &Sexy::SOUND_BALLOONINFLATE}, 0U },
	{ FoleyType::FOLEY_BIGCHOMP,                -2.0f,  { &Sexy::SOUND_BIGCHOMP}, 0U },
	{ FoleyType::FOLEY_MELONIMPACT,             -5.0f,  { &Sexy::SOUND_MELONIMPACT, &Sexy::SOUND_MELONIMPACT2}, 0U },
	{ FoleyType::FOLEY_PLANTGROW,               -2.0f,  { &Sexy::SOUND_PLANTGROW}, 0U },
	{ FoleyType::FOLEY_SHOOP,                   -5.0f,  { &Sexy::SOUND_SHOOP}, 0U },
	{ FoleyType::FOLEY_JUICY,                   2.0f,   { &Sexy::SOUND_JUICY}, 0U },
	{ FoleyType::FOLEY_NEWSPAPER_RARRGH,        -2.0f,  { &Sexy::SOUND_NEWSPAPER_RARRGH, &Sexy::SOUND_NEWSPAPER_RARRGH2, &Sexy::SOUND_NEWSPAPER_RARRGH2}, 0U },
	{ FoleyType::FOLEY_NEWSPAPER_RIP,           -2.0f,  { &Sexy::SOUND_NEWSPAPER_RIP}, 0U },
	{ FoleyType::FOLEY_FLOOP,                   0.0f,   { &Sexy::SOUND_FLOOP}, 0U },
	{ FoleyType::FOLEY_COFFEE,                  0.0f,   { &Sexy::SOUND_COFFEE}, 0U },
	{ FoleyType::FOLEY_LOW_GROAN,               2.0f,   { &Sexy::SOUND_LOWGROAN, &Sexy::SOUND_LOWGROAN2}, 0U },
	{ FoleyType::FOLEY_PRIZE,                   0.0f,   { &Sexy::SOUND_PRIZE}, 0U },
	{ FoleyType::FOLEY_YUCK,                    1.0f,   { &Sexy::SOUND_YUCK, &Sexy::SOUND_YUCK, &Sexy::SOUND_YUCK2}, 0U },
	{ FoleyType::FOLEY_UMBRELLA,                2.0f,   { &Sexy::SOUND_THROW2}, 0U },
	{ FoleyType::FOLEY_GRASSSTEP,               2.0f,   { &Sexy::SOUND_GRASSSTEP}, 0U },
	{ FoleyType::FOLEY_SHOVEL,                  5.0f,   { &Sexy::SOUND_SHOVEL}, 0U },
	{ FoleyType::FOLEY_COB_LAUNCH,              10.0f,  { &Sexy::SOUND_COBLAUNCH}, 0U },
	{ FoleyType::FOLEY_WATERING,                10.0f,  { &Sexy::SOUND_WATERING}, 0U },
	{ FoleyType::FOLEY_POLEVAULT,               5.0f,   { &Sexy::SOUND_POLEVAULT}, 0U },
	{ FoleyType::FOLEY_GRAVESTONE_RUMBLE,       10.0f,  { &Sexy::SOUND_GRAVESTONE_RUMBLE}, 0U },
	{ FoleyType::FOLEY_DIRT_RISE,               5.0f,   { &Sexy::SOUND_DIRT_RISE}, 0U },
	{ FoleyType::FOLEY_FERTILIZER,              0.0f,   { &Sexy::SOUND_FERTILIZER}, 0U },
	{ FoleyType::FOLEY_PORTAL,                  0.0f,   { &Sexy::SOUND_PORTAL}, 0U },
	{ FoleyType::FOLEY_WAKEUP,                  0.0f,   { &Sexy::SOUND_WAKEUP}, 0U },
	{ FoleyType::FOLEY_BUGSPRAY,                0.0f,   { &Sexy::SOUND_BUGSPRAY}, 0U },
	{ FoleyType::FOLEY_SCREAM,                  0.0f,   { &Sexy::SOUND_SCREAM}, 0U },
	{ FoleyType::FOLEY_PAPER,                   0.0f,   { &Sexy::SOUND_PAPER}, 0U },
	{ FoleyType::FOLEY_MONEYFALLS,              0.0f,   { &Sexy::SOUND_MONEYFALLS}, 0U },
	{ FoleyType::FOLEY_IMP,                     5.0f,   { &Sexy::SOUND_IMP, &Sexy::SOUND_IMP2}, 0U },
	{ FoleyType::FOLEY_HYDRAULIC_SHORT,         3.0f,   { &Sexy::SOUND_HYDRAULIC_SHORT}, 0U },
	{ FoleyType::FOLEY_HYDRAULIC,               0.0f,   { &Sexy::SOUND_HYDRAULIC}, 0U },
	{ FoleyType::FOLEY_GARGANTUDEATH,           3.0f,   { &Sexy::SOUND_GARGANTUDEATH}, 0U },
	{ FoleyType::FOLEY_CERAMIC,                 0.0f,   { &Sexy::SOUND_CERAMIC}, 0U },
	{ FoleyType::FOLEY_BOSS_BOULDER_ATTACK,     0.0f,   { &Sexy::SOUND_BOSSBOULDERATTACK}, 0U },
	{ FoleyType::FOLEY_CHIME,                   0.0f,   { &Sexy::SOUND_CHIME}, 0U },
	{ FoleyType::FOLEY_CRAZY_DAVE_SHORT,        0.0f,   { &Sexy::SOUND_CRAZYDAVESHORT1, &Sexy::SOUND_CRAZYDAVESHORT2, &Sexy::SOUND_CRAZYDAVESHORT3}, 16U },
	{ FoleyType::FOLEY_CRAZY_DAVE_LONG,         0.0f,   { &Sexy::SOUND_CRAZYDAVELONG1, &Sexy::SOUND_CRAZYDAVELONG2, &Sexy::SOUND_CRAZYDAVELONG3}, 16U },
	{ FoleyType::FOLEY_CRAZY_DAVE_EXTRA_LONG,   0.0f,   { &Sexy::SOUND_CRAZYDAVEEXTRALONG1, &Sexy::SOUND_CRAZYDAVEEXTRALONG2, &Sexy::SOUND_CRAZYDAVEEXTRALONG3}, 16U },
	{ FoleyType::FOLEY_CRAZY_DAVE_CRAZY,        0.0f,   { &Sexy::SOUND_CRAZYDAVECRAZY}, 0U },
	{ FoleyType::FOLEY_PHONOGRAPH,              0.0f,   { &Sexy::SOUND_PHONOGRAPH}, 0U },
	{ FoleyType::FOLEY_DANCER,                  0.0f,   { &Sexy::SOUND_DANCER}, 6U },
	{ FoleyType::FOLEY_FINAL_FANFARE,           0.0f,   { &Sexy::SOUND_FINALFANFARE}, 0U },
	{ FoleyType::FOLEY_CRAZY_DAVE_SCREAM,       0.0f,   { &Sexy::SOUND_CRAZYDAVESCREAM}, 0U },
	{ FoleyType::FOLEY_CRAZY_DAVE_SCREAM_2,     0.0f,   { &Sexy::SOUND_CRAZYDAVESCREAM2}, 0U }
};

FoleyInstance::FoleyInstance()
{
	mInstance = nullptr;
	mRefCount = 0;
	mPaused = false;
	mStartTime = 0;
	mPauseOffset = 0;
}

//0x514ED0
FoleyTypeData::FoleyTypeData()
{
	mLastVariationPlayed = -1;
}

int TodDSoundInstance::GetSoundPosition()
{
	unsigned long dwCurrentPlayerCursor;
	mSoundBuffer->GetCurrentPosition(&dwCurrentPlayerCursor, nullptr);
	return dwCurrentPlayerCursor;
}

void TodDSoundInstance::SetSoundPosition(int thePosition)
{
	mSoundBuffer->SetCurrentPosition(thePosition);
}

void TodFoleyInitialize(FoleyParams* theFoleyParamArray, int theFoleyParamArraySize)
{
	TOD_ASSERT(gFoleyParamArray == nullptr && gFoleyParamArraySize == 0);
	gFoleyParamArray = theFoleyParamArray;
	gFoleyParamArraySize = theFoleyParamArraySize;
}

void TodFoleyDispose()
{
	gFoleyParamArray = nullptr;
	gFoleyParamArraySize = 0;
}

//0x514F70
void SoundSystemReleaseFinishedInstances(TodFoley* theSoundSystem)
{
	for (int aFoleyType = 0; aFoleyType < gFoleyParamArraySize; aFoleyType++)
		for (int i = 0; i < MAX_FOLEY_INSTANCES; i++)
		{
			FoleyInstance* aFoleyInstance = &theSoundSystem->mFoleyTypeData[aFoleyType].mFoleyInstances[i];
			if (aFoleyInstance->mRefCount == 0)
			{
				TOD_ASSERT(aFoleyInstance->mInstance == nullptr);
			}
			else if (!aFoleyInstance->mPaused)
			{
				TOD_ASSERT(aFoleyInstance->mInstance);
				if (!aFoleyInstance->mInstance->IsPlaying())
				{
					aFoleyInstance->mInstance->Release();
					aFoleyInstance->mInstance = nullptr;
					aFoleyInstance->mRefCount = 0;
				}
			}
		}
}

//0x514FE0
bool SoundSystemHasFoleyPlayedTooRecently(TodFoley* theSoundSystem, FoleyType theFoleyType)
{
	FoleyTypeData* aFoleyData = &theSoundSystem->mFoleyTypeData[(int)theFoleyType];
	for (int i = 0; i < MAX_FOLEY_INSTANCES; i++)
	{
		FoleyInstance* aFoleyInstance = &aFoleyData->mFoleyInstances[i];
		if (aFoleyInstance->mRefCount != 0 && gSexyAppBase->mUpdateCount - aFoleyInstance->mStartTime < 10)  // ��ͬ����Ч���ڽ� 10 cs �ڲ��ŵ�ʵ��
			return true;
	}
	return false;
}

FoleyParams* LookupFoley(FoleyType theFoleyType)
{
	TOD_ASSERT(theFoleyType >= 0 && theFoleyType < gFoleyParamArraySize);
	TOD_ASSERT(gFoleyParamArraySize < MAX_FOLEY_TYPES);
	FoleyParams* aFoleyParams = &gFoleyParamArray[(int)theFoleyType];
	TOD_ASSERT(aFoleyParams->mFoleyType == theFoleyType);
	return aFoleyParams;
}

FoleyInstance* SoundSystemFindInstance(TodFoley* theSoundSystem, FoleyType theFoleyType)
{
	FoleyTypeData* aFoleyData = &theSoundSystem->mFoleyTypeData[(int)theFoleyType];
	for (int i = 0; i < MAX_FOLEY_INSTANCES; i++)
	{
		FoleyInstance* aFoleyInstance = &aFoleyData->mFoleyInstances[i];
		if (aFoleyInstance->mRefCount > 0)
		{
			TOD_ASSERT(aFoleyInstance->mInstance);
			return aFoleyInstance;
		}
	}
	return nullptr;
}

FoleyInstance* SoundSystemGetFreeInstanceIndex(TodFoley* theSoundSystem, FoleyType theFoleyType)
{
	FoleyTypeData* aFoleyData = &theSoundSystem->mFoleyTypeData[(int)theFoleyType];
	for (int i = 0; i < MAX_FOLEY_INSTANCES; i++)
	{
		FoleyInstance* aFoleyInstance = &aFoleyData->mFoleyInstances[i];
		if (aFoleyInstance->mRefCount == 0)
		{
			TOD_ASSERT(aFoleyInstance->mInstance == nullptr);
			return aFoleyInstance;
		}
	}
	return nullptr;
}

//0x515020
void TodFoley::PlayFoleyPitch(FoleyType theFoleyType, float thePitch)
{
	FoleyParams* aFoleyParams = LookupFoley(theFoleyType);
	SoundSystemReleaseFinishedInstances(this);  // �ͷ��Ѳ�����ɵ���Чʵ��
	if (SoundSystemHasFoleyPlayedTooRecently(this, theFoleyType) && !TestBit(aFoleyParams->mFoleyFlags, FoleyFlags::FOLEYFLAGS_LOOP))
		return;  // ��ѭ����Ч�����ص�����

	if (TestBit(aFoleyParams->mFoleyFlags, FoleyFlags::FOLEYFLAGS_ONE_AT_A_TIME))  // ��������˲��ɵ��Ӳ���
	{
		FoleyInstance* aFoleyInstance = SoundSystemFindInstance(this, theFoleyType);
		if (aFoleyInstance != nullptr)
		{
			aFoleyInstance->mRefCount++;  // ���� 1 �����ü���
			aFoleyInstance->mStartTime = gSexyAppBase->mUpdateCount;  // ˢ�¿�ʼ��ʱ��
			return;
		}
	}
	FoleyInstance* aFoleyInstance = SoundSystemGetFreeInstanceIndex(this, theFoleyType);
	if (aFoleyInstance == nullptr)  // ����Ѿ����� 8 ����Чʵ��
		return;

	int aVariations = 0;
	int aVariationsArray[10];
	FoleyTypeData* aFoleyData = &mFoleyTypeData[(int)theFoleyType];
	for (int i = 0; i < 10; i++)
	{
		if (!TestBit(aFoleyParams->mFoleyFlags, FoleyFlags::FOLEYFLAGS_DONT_REPEAT) || aFoleyData->mLastVariationPlayed != i)  // ���δ�ظ��򲻽�ֹ�ظ�
		{
			if (aFoleyParams->mSfxID[i] == nullptr)
				break;
			aVariationsArray[aVariations] = i;
			aVariations++;
		}
	}
	TOD_ASSERT(aVariations > 0);
	int aVariation = TodPickFromArray(aVariationsArray, aVariations);
	aFoleyData->mLastVariationPlayed = aVariation;
	SoundInstance* aSoundInstance = gSexyAppBase->mSoundManager->GetSoundInstance(*aFoleyParams->mSfxID[aVariation]);
	if (aSoundInstance == nullptr)
		return;

	aFoleyInstance->mInstance = aSoundInstance;
	aFoleyInstance->mRefCount = 1;
	aFoleyInstance->mStartTime = gSexyAppBase->mUpdateCount;
	aFoleyData->mLastVariationPlayed = aVariation;
	if (thePitch != 0.0f)  // �������ָ��������
		aSoundInstance->AdjustPitch(thePitch);  // ��������
	if (TestBit(aFoleyParams->mFoleyFlags, FoleyFlags::FOLEYFLAGS_USES_MUSIC_VOLUME))  // ���������ʹ����������
		ApplyMusicVolume(aFoleyInstance);  // ����Ч����������Ϊ������һ��
	bool aIsLooping = TestBit(aFoleyParams->mFoleyFlags, FoleyFlags::FOLEYFLAGS_LOOP);
	aSoundInstance->Play(aIsLooping, false);  // ��ʽ��ʼ������Ч
}

//0x515240
void TodFoley::PlayFoley(FoleyType theFoleyType)
{
	FoleyParams* aFoleyParams = LookupFoley(theFoleyType);
	float aPitch = 0.0f;
	if (aFoleyParams->mPitchRange != 0.0f)  // ������������߷�Χ
		aPitch = Sexy::Rand(aFoleyParams->mPitchRange);  // �ڷ�Χ�����ѡȡһ������
	PlayFoleyPitch(theFoleyType, aPitch);
}

//0x515290
void TodFoley::StopFoley(FoleyType theFoleyType)
{
	SoundSystemReleaseFinishedInstances(this);
	FoleyInstance* aFoleyInstance = SoundSystemFindInstance(this, theFoleyType);
	if (aFoleyInstance == nullptr)
		return;

	TOD_ASSERT(aFoleyInstance->mRefCount > 0);
	TOD_ASSERT(aFoleyInstance->mInstance);
	aFoleyInstance->mRefCount--;  // ���� 1 �����ü���
	if (aFoleyInstance->mRefCount == 0)  // �������֮�������ã���ֱ���ͷ�
	{
		aFoleyInstance->mInstance->Release();
		aFoleyInstance->mInstance = nullptr;
	}
}

//0x5152D0
void TodFoley::GamePause(bool theEnteringPause)
{
	SoundSystemReleaseFinishedInstances(this);
	for (int aFoleyType = 0; aFoleyType < gFoleyParamArraySize; aFoleyType++)
	{
		FoleyParams* aFoleyParams = LookupFoley((FoleyType)aFoleyType);
		if (TestBit(aFoleyParams->mFoleyFlags, FoleyFlags::FOLEYFLAGS_MUTE_ON_PAUSE))  // ���ָ������ͣʱ��Ĭ
		{
			FoleyTypeData* aFoleyData = &mFoleyTypeData[aFoleyType];
			for (int i = 0; i < MAX_FOLEY_INSTANCES; i++)  // �趨ÿһ����Чʵ������ͣ���
			{
				FoleyInstance* aFoleyInstance = &aFoleyData->mFoleyInstances[i];
				if (aFoleyInstance->mRefCount != 0)  // �����Чʵ����������
				{
					TodDSoundInstance* aSoundInstance = (TodDSoundInstance*)aFoleyInstance->mInstance;
					if (theEnteringPause)
					{
						aFoleyInstance->mPaused = true;
						if (aSoundInstance->mSoundBuffer == nullptr)
						{
							aFoleyInstance->mPauseOffset = 0;
							aSoundInstance->Stop();
						}
						else
						{
							aFoleyInstance->mPauseOffset = aSoundInstance->GetSoundPosition();  // ������ͣʱ�Ĳ��Ž���
							aSoundInstance->Stop();
						}
					}
					else if (aFoleyInstance->mPaused)
					{
						aFoleyInstance->mPaused = false;
						bool aIsLooping = TestBit(aFoleyParams->mFoleyFlags, FoleyFlags::FOLEYFLAGS_LOOP);
						aSoundInstance->Play(aIsLooping, false);
						if (aSoundInstance->mSoundBuffer != nullptr)
							aSoundInstance->SetSoundPosition(aFoleyInstance->mPauseOffset);  // ��ԭ��ͣǰ�Ĳ��Ž���
					}
				}
			}
		}
	}
}

//0x5153F0
void TodFoley::CancelPausedFoley()
{
	SoundSystemReleaseFinishedInstances(this);
	for (int aFoleyType = 0; aFoleyType < gFoleyParamArraySize; aFoleyType++)
	{
		FoleyTypeData* aFoleyData = &mFoleyTypeData[aFoleyType];
		for (int i = 0; i < MAX_FOLEY_INSTANCES; i++)  // �ж�ÿһ����Чʵ������ͣ���
		{
			FoleyInstance* aFoleyInstance = &aFoleyData->mFoleyInstances[i];
			if (aFoleyInstance->mRefCount != 0 && aFoleyInstance->mPaused)  // �����Чʵ�����������Ҵ�����ͣ״̬
			{
				aFoleyInstance->mRefCount = 0;
				aFoleyInstance->mInstance->Release();
				aFoleyInstance->mInstance = nullptr;
			}
		}
	}
}

//0x515460
void TodFoley::ApplyMusicVolume(FoleyInstance* theFoleyInstance)
{
	if (gSexyAppBase->mSfxVolume < 1e-6)
		theFoleyInstance->mInstance->SetVolume(0.0);
	else
		theFoleyInstance->mInstance->SetVolume(gSexyAppBase->mMusicVolume / gSexyAppBase->mSfxVolume);  // �����õ��������ڳ�����Ч��������������������
}

//0x5154A0
void TodFoley::RehookupSoundWithMusicVolume()
{
	SoundSystemReleaseFinishedInstances(this);
	for (int aFoleyType = 0; aFoleyType < gFoleyParamArraySize; aFoleyType++)
	{
		FoleyParams* aFoleyParams = LookupFoley((FoleyType)aFoleyType);
		if (TestBit(aFoleyParams->mFoleyFlags, FoleyFlags::FOLEYFLAGS_USES_MUSIC_VOLUME))  // ����涨��ʹ����������
		{
			FoleyTypeData* aFoleyData = &mFoleyTypeData[aFoleyType];
			for (int i = 0; i < MAX_FOLEY_INSTANCES; i++)  // �趨ÿһ����Чʵ��������
			{
				FoleyInstance* aFoleyInstance = &aFoleyData->mFoleyInstances[i];
				if (aFoleyInstance->mRefCount != 0)  // �����Чʵ����������
					ApplyMusicVolume(aFoleyInstance);
			}
		}
	}
}

//0x515560
bool TodFoley::IsFoleyPlaying(FoleyType theFoleyType)
{
	SoundSystemReleaseFinishedInstances(this);
	return SoundSystemFindInstance(this, theFoleyType) != nullptr;
}
