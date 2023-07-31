#ifndef __TODPARTICLE_H__
#define __TODPARTICLE_H__

#include "TodList.h"
#include "DataArray.h"
#include "../SexyAppFramework/SexyVector.h"
namespace Sexy
{
	class Image;
	class Graphics;
};
using namespace std;
using namespace Sexy;

#define MAX_PARTICLES_SIZE 900
#define MAX_PARTICLE_FIELDS 4

// ######################################################################################################################################################
// ############################################################# ����Ϊ����ϵͳ����������� #############################################################
// ######################################################################################################################################################

enum ParticleFlags
{
    PARTICLE_RANDOM_LAUNCH_SPIN,        // ���������ת��ָ�������ڷ���ʱʹ�� [0, 2��] ������ĳ�ʼ��ת�Ƕ�
    PARTICLE_ALIGN_LAUNCH_SPIN,         // ���뷢��Ƕȣ�ָ�������ڷ���ʱ�ĳ�ʼ��ת�Ƕ��뷢�䷽��һ�£����ȼ��������������ת��
    PARTICLE_ALIGN_TO_PIXELS,           // ���������أ�ָ��������Ⱦʱ��������������ض�������������ֵ
    PARTICLE_SYSTEM_LOOPS,              // ϵͳѭ����ָ�����������������ڽ����������ص�������ʼ��ʱ��
    PARTICLE_PARTICLE_LOOPS,            // ����ѭ����ָ���������������ڽ����������ص�������ʼ��ʱ��
    PARTICLE_PARTICLES_DONT_FOLLOW,     // ���Ӳ����棬ָ�������ӷ�������λ���ƶ�ʱ���ѷ�������Ӳ���֮�ƶ�
    PARTICLE_RANDOM_START_TIME,         // �����ʼʱ�̣�ָ�����ӳ�ʼʱ�Ĵ���ʱ��Ϊ 0 ��������ʱ��֮������ֵ
    PARTICLE_DIE_IF_OVERLOADED,         // �������ƣ�ָ����������������ʱ���÷���������������ϵͳ�޷�������
    PARTICLE_ADDITIVE,                  // ����ģʽ��ָ��������Ⱦʱ�̶�ʹ�õ��ӣ�Additive��ģʽ
    PARTICLE_FULLSCREEN,                // ȫ��ģʽ��ָ����������Ⱦʱ��Ϊ���һ����Ļ��С�ľ���
    PARTICLE_SOFTWARE_ONLY,             // �������Ⱦ��ָ�����ӽ���δ���� 3D ����ʱ�ɱ���Ⱦ
    PARTICLE_HARDWARE_ONLY              // ��Ӳ����Ⱦ��ָ�����ӽ����ѿ��� 3D ����ʱ�ɱ���Ⱦ
};

enum ParticleFieldType
{
    FIELD_INVALID,
    FIELD_FRICTION,                     // Ħ���������ó������ӵ��ٶȰ�һ����������˥��
    FIELD_ACCELERATION,                 // ���ٶȳ����ó���������һ�����ٶ��������˶�
    FIELD_ATTRACTOR,                    // �����������ó������Ӽ��ٶȵĴ�С�ͷ�����ܵ������뷢����֮������С��Ӱ��
    FIELD_MAX_VELOCITY,                 // ���ٳ����ó��������ٶȵĴ�С���ܳ���һ��������ֵ
    FIELD_VELOCITY,                     // ���ٳ����ó��������ٶȵĴ�С����Ϊһ����ֵ
    FIELD_POSITION,                     // ��λ�����ó������ӵ�λ������Ϊһ����ֵ
    FIELD_SYSTEM_POSITION,              // ϵͳ��λ���������������ã��ó��ڷ�������λ������Ϊһ����ֵ
    FIELD_GROUND_CONSTRAINT,            // �������ƣ����ӵ�����λ�ò��ܵ��ڵ��棬�ҽӴ�����ʱ�ᴥ������Ч��
    FIELD_SHAKE,                        // �𶯣����ӵ����������� -1.0 �� +1.0 ��ƫ�ƣ�ÿ֡��������
    FIELD_CIRCLE,                       // ���������ó�������Χ�Ʒ�����������Բ���˶���������ʵΪ�����˶���
    FIELD_AWAY,                         // ���������ó��������ؾ��򲻶�Զ�뷢��������
    PARTICLE_FIELD_COUNT
};  // ���ӳ����������� TodParticleEmitter::UpdateParticleField() ������ϵͳ��λ��Ϊ TodParticleEmitter::UpdateSystemField() ������

// ################################################################################
// �� ��������ϵͳ������صĸ����͵�ʵ���� XML �еĸ�ʽ�Ķ�Ӧ��ϵʾ����ͼ
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// <Emitter>                                                         ��
//                       ����Node����                                ��
//     <SpawnRate>[.5 2] [2.5 4.5],40 [5 10]</SpawnRate>    TodEmitterDefinition
//     ����      FloatParameterTrack::mNodes        ����             ��
// </Emitter>                                                        ��
// ################################################################################
// ��!��XML ��û�и����Ķ���������ڶ�ȡ�����󱻸�������Ĭ��ֵ

// ====================================================================================================
// �� �������������ڵ㡿
// ----------------------------------------------------------------------------------------------------
// ÿ���ڵ�����������ֵ��ʱ��仯��һ���׶Ρ�
// ====================================================================================================
class FloatParameterTrackNode
{
public:
    float                       mTime;                          // �׶ε���ʼʱ��
    float                       mLowValue;                      // �׶��������������Сֵ
    float                       mHighValue;                     // �׶���������������ֵ
    TodCurves                   mCurveType;                     // �ӵ�ǰ�׶ι�������һ�׶εĻ���Ч������
    TodCurves                   mDistribution;                  // �׶�����������Сֵ�����ֵ֮��ĸ��ʷֲ�����
};

// ====================================================================================================
// �� ��������������
// ----------------------------------------------------------------------------------------------------
// ÿ�����������������һ�����Ե���ֵ��ʱ��ı仯���ɺ�ȡֵ��Χ��
// ====================================================================================================
class FloatParameterTrack
{
public:
    FloatParameterTrackNode*    mNodes;
    int                         mCountNodes;
};

// ====================================================================================================
// �� �����ӳ���
// ----------------------------------------------------------------------------------------------------
// �������������˶�����������������ӳ����Ե��Ӵ��ڣ������� 4 ����
// ====================================================================================================
class ParticleField
{
public:
    ParticleFieldType           mFieldType;                     // �������ͣ������˳��������˶���Ӱ�췽ʽ
    FloatParameterTrack         mX;                             // ����ˮƽ���򣨺����϶����ӵ�Ӱ��
    FloatParameterTrack         mY;                             // ������ֱ���������϶����ӵ�Ӱ��
};

// ====================================================================================================
// �� �����������塿
// ----------------------------------------------------------------------------------------------------
// ���ӷ������Ķ��������������������Ϊ�Ĳ����ı仯���ɺͷ�Χ��
// ====================================================================================================
class TodEmitterDefinition
{
public:
    Image*                      mImage;
    int                         mImageCol;
    int                         mImageRow;
    int                         mImageFrames;
    int                         mAnimated;
    int                         mParticleFlags;
    EmitterType                 mEmitterType;
    const char*                 mName;
    const char*                 mOnDuration;
    FloatParameterTrack         mSystemDuration;
    FloatParameterTrack         mCrossFadeDuration;
    FloatParameterTrack         mSpawnRate;
    FloatParameterTrack         mSpawnMinActive;
    FloatParameterTrack         mSpawnMaxActive;
    FloatParameterTrack         mSpawnMaxLaunched;
    FloatParameterTrack         mEmitterRadius;
    FloatParameterTrack         mEmitterOffsetX;
    FloatParameterTrack         mEmitterOffsetY;
    FloatParameterTrack         mEmitterBoxX;
    FloatParameterTrack         mEmitterBoxY;
    FloatParameterTrack         mEmitterSkewX;
    FloatParameterTrack         mEmitterSkewY;
    FloatParameterTrack         mEmitterPath;
    FloatParameterTrack         mParticleDuration;
    FloatParameterTrack         mLaunchSpeed;
    FloatParameterTrack         mLaunchAngle;
    FloatParameterTrack         mSystemRed;
    FloatParameterTrack         mSystemGreen;
    FloatParameterTrack         mSystemBlue;
    FloatParameterTrack         mSystemAlpha;
    FloatParameterTrack         mSystemBrightness;
    ParticleField*              mParticleFields;
    int                         mParticleFieldCount;
    ParticleField*              mSystemFields;
    int                         mSystemFieldCount;
    FloatParameterTrack         mParticleRed;
    FloatParameterTrack         mParticleGreen;
    FloatParameterTrack         mParticleBlue;
    FloatParameterTrack         mParticleAlpha;
    FloatParameterTrack         mParticleBrightness;
    FloatParameterTrack         mParticleSpinAngle;
    FloatParameterTrack         mParticleSpinSpeed;
    FloatParameterTrack         mParticleScale;
    FloatParameterTrack         mParticleStretch;
    FloatParameterTrack         mCollisionReflect;
    FloatParameterTrack         mCollisionSpin;
    FloatParameterTrack         mClipTop;
    FloatParameterTrack         mClipBottom;
    FloatParameterTrack         mClipLeft;
    FloatParameterTrack         mClipRight;
    FloatParameterTrack         mAnimationRate;
};

// ====================================================================================================
// �� ������ϵͳ���塿
// ----------------------------------------------------------------------------------------------------
// ����ϵͳ�Ķ������ݣ�������ϵͳ�и������ӷ������Ķ������ݵļ��ϡ�
// ====================================================================================================
class TodParticleDefinition
{
public:
    TodEmitterDefinition*       mEmitterDefs;
    int                         mEmitterDefCount;
};

extern int gParticleDefCount;                       // [0x6A9F08]
extern TodParticleDefinition* gParticleDefArray;    // [0x6A9F0C]���� LawnApp::LoadingThreadProc() �����ж�ȡ����ֵ

// ====================================================================================================
// �� �����Ӳ�����
// ----------------------------------------------------------------------------------------------------
// ��������һ������ϵͳ�����������ϵͳ�������ļ����ļ���֮��Ķ�Ӧ��ϵ��
// ====================================================================================================
class ParticleParams
{
public:
    ParticleEffect              mParticleEffect;
    const char*                 mParticleFileName;
};
extern int gParticleParamArraySize;                 // [0x6A9F10]
extern ParticleParams* gParticleParamArray;         // [0x6A9F14]

bool                            TodParticleLoadADef(TodParticleDefinition* theParticleDef, const char* theParticleFileName);
void                            TodParticleLoadDefinitions(ParticleParams* theParticleParamArray, int theParticleParamArraySize);
void                            TodParticleFreeDefinitions();

extern ParticleParams gLawnParticleArray[(int)ParticleEffect::NUM_PARTICLES];  // 0x6A0FF0

// ######################################################################################################################################################
// ############################################################ ������ʽ��ʼ����ϵͳ������� ############################################################
// ######################################################################################################################################################

enum ParticleSystemTracks
{
	TRACK_SPAWN_RATE,
	TRACK_SPAWN_MIN_ACTIVE,
	TRACK_SPAWN_MAX_ACTIVE,
	TRACK_SPAWN_MAX_LAUNCHED,
	TRACK_EMITTER_PATH,
	TRACK_SYSTEM_RED,
	TRACK_SYSTEM_GREEN,
	TRACK_SYSTEM_BLUE,
	TRACK_SYSTEM_ALPHA,
	TRACK_SYSTEM_BRIGHTNESS,
	NUM_SYSTEM_TRACKS
};

enum ParticleTracks
{
	TRACK_PARTICLE_RED,
	TRACK_PARTICLE_GREEN,
	TRACK_PARTICLE_BLUE,
	TRACK_PARTICLE_ALPHA,
	TRACK_PARTICLE_BRIGHTNESS,
	TRACK_PARTICLE_SPIN_SPEED,
	TRACK_PARTICLE_SPIN_ANGLE,
	TRACK_PARTICLE_SCALE,
	TRACK_PARTICLE_STRETCH,
	TRACK_PARTICLE_COLLISION_REFLECT,
	TRACK_PARTICLE_COLLISION_SPIN,
	TRACK_PARTICLE_CLIP_TOP,
	TRACK_PARTICLE_CLIP_BOTTOM,
	TRACK_PARTICLE_CLIP_LEFT,
	TRACK_PARTICLE_CLIP_RIGHT,
	TRACK_PARTICLE_ANIMATION_RATE,
	NUM_PARTICLE_TRACKS
};

class TodParticleSystem;
class TodParticleEmitter;
class TodParticle;
class TodParticleHolder
{
public:
	DataArray<TodParticleSystem>	mParticleSystems;
	DataArray<TodParticleEmitter>	mEmitters;
	DataArray<TodParticle>			mParticles;
	TodAllocator					mParticleListNodeAllocator;
	TodAllocator					mEmitterListNodeAllocator;

public:
    ~TodParticleHolder();

    void							InitializeHolder();
    void							DisposeHolder();
    TodParticleSystem*				AllocParticleSystemFromDef(float theX, float theY, int theRenderOrder, TodParticleDefinition* theDefinition, ParticleEffect theParticleEffect);
    TodParticleSystem*				AllocParticleSystem(float theX, float theY, int theRenderOrder, ParticleEffect theParticleEffect);
    /*inline*/ bool					IsOverLoaded();
};

class ParticleRenderParams
{
public:
	bool							mRedIsSet;
	bool							mGreenIsSet;
	bool							mBlueIsSet;
	bool							mAlphaIsSet;
	bool							mParticleScaleIsSet;
	bool							mParticleStretchIsSet;
	bool							mSpinPositionIsSet;
	bool							mPositionIsSet;
	float							mRed;
	float							mGreen;
	float							mBlue;
	float							mAlpha;
	float							mParticleScale;
	float							mParticleStretch;
	float							mSpinPosition;
	float							mPosX;
	float							mPosY;
};

class TodParticle
{
public:
	TodParticleEmitter*				mParticleEmitter;
	int								mParticleDuration;
	int								mParticleAge;
	float							mParticleTimeValue;
	float							mParticleLastTimeValue;
	float							mAnimationTimeValue;
	SexyVector2						mVelocity;
	SexyVector2						mPosition;
	int								mImageFrame;
	float							mSpinPosition;
	float							mSpinVelocity;
	ParticleID						mCrossFadeParticleID;
	int								mCrossFadeDuration;
	float							mParticleInterp[ParticleTracks::NUM_PARTICLE_TRACKS];
	float							mParticleFieldInterp[MAX_PARTICLE_FIELDS][2];
};

class TodTriangleGroup;
class TodParticleEmitter
{
public:
	TodEmitterDefinition*			mEmitterDef;													//+0x0
	TodParticleSystem*				mParticleSystem;												//+0x4
	TodList<ParticleID>				mParticleList;													//+0x8
	float							mSpawnAccum;													//+0x18
	Sexy::SexyVector2				mSystemCenter;													//+0x1C
	int								mParticlesSpawned;												//+0x24
	int								mSystemAge;														//+0x28
	int								mSystemDuration;												//+0x2C
	float							mSystemTimeValue;												//+0x30
	float							mSystemLastTimeValue;											//+0x34
	bool							mDead;															//+0x38
	Sexy::Color						mColorOverride;													//+0x3C
	bool							mExtraAdditiveDrawOverride;										//+0x4C
	float							mScaleOverride;													//+0x50
	Sexy::Image*					mImageOverride;													//+0x54
	ParticleEmitterID				mCrossFadeEmitterID;											//+0x58
	int								mEmitterCrossFadeCountDown;										//+0x5C
	int								mFrameOverride;													//+0x60
	float							mTrackInterp[ParticleSystemTracks::NUM_SYSTEM_TRACKS];			//+0x64
	float							mSystemFieldInterp[MAX_PARTICLE_FIELDS][2];						//+0x8C

public:
	void							TodEmitterInitialize(float theX, float theY, TodParticleSystem* theSystem, TodEmitterDefinition* theEmitterDef);
	void							Update();
	void							Draw(Graphics* g);
	void							SystemMove(float theX, float theY);
	static bool						GetRenderParams(TodParticle* theParticle, ParticleRenderParams* theParams);
	void							DrawParticle(Graphics* g, TodParticle* theParticle, TodTriangleGroup* theTriangleGroup);
	void							UpdateSpawning();
	bool							UpdateParticle(TodParticle* theParticle);
	TodParticle*					SpawnParticle(int theIndex, int theSpawnCount);
	bool							CrossFadeParticle(TodParticle* theParticle, TodParticleEmitter* theToEmitter);
	void							CrossFadeEmitter(TodParticleEmitter* theToEmitter);
	bool							CrossFadeParticleToName(TodParticle* theParticle, const char* theEmitterName);
	void							DeleteAll();
	void							UpdateParticleField(TodParticle* theParticle, ParticleField* theParticleField, float theParticleTimeValue, int theFieldIndex);
	void							UpdateSystemField(ParticleField* theParticleField, float theParticleTimeValue, int theFieldIndex);
    /*inline*/ float				SystemTrackEvaluate(FloatParameterTrack& theTrack, ParticleSystemTracks theSystemTrack);
	static /*inline*/ float			ParticleTrackEvaluate(FloatParameterTrack& theTrack, TodParticle* theParticle, ParticleTracks theParticleTrack);
	void							DeleteParticle(TodParticle* theParticle);
	void							DeleteNonCrossFading();
};
/*inline*/ float                    CrossFadeLerp(float theFrom, float theTo, bool theFromIsSet, bool theToIsSet, float theFraction);
void								RenderParticle(Graphics* g, TodParticle* theParticle, const Color& theColor, ParticleRenderParams* theParams, TodTriangleGroup* theTriangleGroup);

class TodParticleSystem
{
public:
	ParticleEffect					mEffectType;					//+0x0
    TodParticleDefinition*			mParticleDef;					//+0x4
    TodParticleHolder*				mParticleHolder;				//+0x8
    TodList<ParticleEmitterID>		mEmitterList;					//+0xC
    bool							mDead;							//+0x1C
    bool							mIsAttachment;					//+0x1D
    int								mRenderOrder;					//+0x20
    bool							mDontUpdate;					//+0x24

public:
    TodParticleSystem();
    ~TodParticleSystem();

    void							TodParticleInitializeFromDef(float theX, float theY, int theRenderOrder, TodParticleDefinition* theDefinition, ParticleEffect theEffectType);
    void							ParticleSystemDie();
    void							Update();
    void							Draw(Graphics* g);
    void							SystemMove(float theX, float theY);
    void							OverrideColor(const char* theEmitterName, const Color& theColor);
    void							OverrideExtraAdditiveDraw(const char* theEmitterName, bool theEnableExtraAdditiveDraw);
    void							OverrideImage(const char* theEmitterName, Image* theImage);
    void							OverrideFrame(const char* theEmitterName, int theFrame);
    void							OverrideScale(const char* theEmitterName, float theScale);
    void							CrossFade(const char* theEmitterName);
    TodParticleEmitter*				FindEmitterByName(const char* theEmitterName);
    TodEmitterDefinition*			FindEmitterDefByName(const char* theEmitterName);
};

#endif
