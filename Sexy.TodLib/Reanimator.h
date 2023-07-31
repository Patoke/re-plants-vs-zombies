#ifndef __REANIMATION_H__
#define __REANIMATION_H__

#include "DataArray.h"
#include "FilterEffect.h"
#include "../SexyAppFramework/SexyMatrix.h"
using namespace std;
using namespace Sexy;

class Reanimation;
class ReanimAtlas;
class AttacherInfo;
class AttachEffect;
class TodTriangleGroup;
class TodParticleSystem;
class ReanimatorTransform;
class ReanimatorDefinition;
namespace Sexy
{
    class Font;
    class Image;
    class Graphics;
    class MemoryImage;
};

// ######################################################################################################################################################
// ############################################################### ����Ϊ��������������� ###############################################################
// ######################################################################################################################################################

constexpr const float DEFAULT_FIELD_PLACEHOLDER = -10000.0f;
constexpr const double SECONDS_PER_UPDATE = 0.01f;

enum ReanimFlags
{
    REANIM_NO_ATLAS,
    REANIM_FAST_DRAW_IN_SW_MODE
};

class ReanimatorTrack
{
public:
    const char*                     mName;                          //+0x0���������
    ReanimatorTransform*            mTransforms;                    //+0x4��ÿһ֡�Ķ����任������
    int                             mTransformCount;                //+0x8�������任��������֡����
    
public:
    ReanimatorTrack() : mName(""), mTransforms(nullptr), mTransformCount(0) { }
};

// ====================================================================================================
// �� �����������塿
// ----------------------------------------------------------------------------------------------------
// ��������һ�ֶ���������ö����������ļ����ļ�������־֮��Ķ�Ӧ��ϵ��
// ====================================================================================================
class ReanimatorDefinition
{
public:
    ReanimatorTrack*                mTracks;
    int                             mTrackCount;
    float                           mFPS;
    ReanimAtlas*                    mReanimAtlas;

public:
    ReanimatorDefinition() : mTracks(nullptr), mTrackCount(0), mFPS(12.0f), mReanimAtlas(nullptr) { }
};
extern int gReanimatorDefCount;                     //[0x6A9EE4]
extern ReanimatorDefinition* gReanimatorDefArray;   //[0x6A9EE8]

// ====================================================================================================
// �� ������������
// ----------------------------------------------------------------------------------------------------
// ��������һ�ֶ���������ö����������ļ����ļ�������־֮��Ķ�Ӧ��ϵ��
// ====================================================================================================
class ReanimationParams
{
public:
    ReanimationType                 mReanimationType;
    const char*                     mReanimFileName;
    int                             mReanimParamFlags;
};
extern int gReanimationParamArraySize;              //[0x6A9EEC]
extern ReanimationParams* gReanimationParamArray;   //[0x6A9EF0]

/*inline*/ void                     ReanimationFillInMissingData(float& thePrev, float& theValue);
/*inline*/ void                     ReanimationFillInMissingData(void*& thePrev, void*& theValue);
bool                                ReanimationLoadDefinition(const SexyString& theFileName, ReanimatorDefinition* theDefinition);
void                                ReanimationFreeDefinition(ReanimatorDefinition* theDefinition);
void _cdecl	                        ReanimatorEnsureDefinitionLoaded(ReanimationType theReanimType, bool theIsPreloading);
void                                ReanimatorLoadDefinitions(ReanimationParams* theReanimationParamArray, int theReanimationParamArraySize);
void                                ReanimatorFreeDefinitions();

extern ReanimationParams gLawnReanimationArray[(int)ReanimationType::NUM_REANIMS];  //0x6A1340

// ######################################################################################################################################################
// ############################################################## ������ʽ��ʼ����������� ##############################################################
// ######################################################################################################################################################

enum
{
    RENDER_GROUP_HIDDEN = -1,
    RENDER_GROUP_NORMAL = 0
};

constexpr const int NO_BASE_POSE = -2;

class ReanimationHolder
{
public:
    DataArray<Reanimation>          mReanimations;

public:
    ReanimationHolder() { ; }
    ~ReanimationHolder();

    void                            InitializeHolder();
    void                            DisposeHolder();
    Reanimation*                    AllocReanimation(float theX, float theY, int theRenderOrder, ReanimationType theReanimationType);
};

// ====================================================================================================
// �� ��������ʱ�䡿
// ----------------------------------------------------------------------------------------------------
// ��������������ǰ���ڲ��ŵ�ʱ��λ�á�
// ====================================================================================================
class ReanimatorFrameTime
{
public:
    float                           mFraction;                      //+0x0����֮֡���Ѿ����ı���
    int                             mAnimFrameBeforeInt;            //+0x4��ǰһ������֡
    int                             mAnimFrameAfterInt;             //+0x8����һ������֡
};

class ReanimatorTransform
{
public:
    float                           mTransX;
    float                           mTransY;
    float                           mSkewX;
    float                           mSkewY;
    float                           mScaleX;
    float                           mScaleY;
    float                           mFrame;
    float                           mAlpha;
    Image*                          mImage;
    Font*                           mFont;
    const char*                     mText;

public:
    ReanimatorTransform();
};

class ReanimatorTrackInstance
{
public:
    int                             mBlendCounter;                  //+0x0
    int                             mBlendTime;                     //+0x4
    ReanimatorTransform             mBlendTransform;                //+0x8
    float                           mShakeOverride;                 //+0x34
    float                           mShakeX;                        //+0x38
    float                           mShakeY;                        //+0x3C
    AttachmentID                    mAttachmentID;                  //+0x40
    Image*                          mImageOverride;                 //+0x44
    int                             mRenderGroup;                   //+0x48
    Color                           mTrackColor;                    //+0x4C
    bool                            mIgnoreClipRect;                //+0x5C
    bool                            mTruncateDisappearingFrames;    //+0x5D
    bool                            mIgnoreColorOverride;           //+0x5E
    bool                            mIgnoreExtraAdditiveColor;      //+0x5F

public:
    ReanimatorTrackInstance();
};

class Reanimation
{
public:
    ReanimationType                 mReanimationType;
    float                           mAnimTime;
    float                           mAnimRate;
    ReanimatorDefinition*           mDefinition;
    ReanimLoopType                  mLoopType;
    bool                            mDead;
    int                             mFrameStart;
    int                             mFrameCount;
    int                             mFrameBasePose;
    SexyTransform2D                 mOverlayMatrix;
    Color                           mColorOverride;
    ReanimatorTrackInstance*        mTrackInstances;
    int                             mLoopCount;
    ReanimationHolder*              mReanimationHolder;
    bool                            mIsAttachment;
    int                             mRenderOrder;
    Color                           mExtraAdditiveColor;
    bool                            mEnableExtraAdditiveDraw;
    Color                           mExtraOverlayColor;
    bool                            mEnableExtraOverlayDraw;
    float                           mLastFrameTime;
    FilterEffect                    mFilterEffect;

public:
    Reanimation();
    ~Reanimation();

    void                            ReanimationInitialize(float theX, float theY, ReanimatorDefinition* theDefinition);
    /*inline*/ void                 ReanimationInitializeType(float theX, float theY, ReanimationType theReanimType);
    void                            ReanimationDie();
    void                            Update();
    /*inline*/ void                 Draw(Graphics* g);
    void                            DrawRenderGroup(Graphics* g, int theRenderGroup);
    bool                            DrawTrack(Graphics* g, int theTrackIndex, int theRenderGroup, TodTriangleGroup* theTriangleGroup);
    void                            GetCurrentTransform(int theTrackIndex, ReanimatorTransform* theTransformCurrent);
    void                            GetTransformAtTime(int theTrackIndex, ReanimatorTransform* theTransform, ReanimatorFrameTime* theFrameTime);
    void                            GetFrameTime(ReanimatorFrameTime* theFrameTime);
    int                             FindTrackIndex(const char* theTrackName);
    void                            AttachToAnotherReanimation(Reanimation* theAttachReanim, const char* theTrackName);
    void                            GetAttachmentOverlayMatrix(int theTrackIndex, SexyTransform2D& theOverlayMatrix);
    /*inline*/ void                 SetFramesForLayer(const char* theTrackName);
    static void                     MatrixFromTransform(const ReanimatorTransform& theTransform, SexyMatrix3& theMatrix);
    bool                            TrackExists(const char* theTrackName);
    void                            StartBlend(int theBlendTime);
    /*inline*/ void                 SetShakeOverride(const char* theTrackName, float theShakeAmount);
    /*inline*/ void                 SetPosition(float theX, float theY);
    /*inline*/ void                 OverrideScale(float theScaleX, float theScaleY);
    float                           GetTrackVelocity(const char* theTrackName);
    /*inline*/ void                 SetImageOverride(const char* theTrackName, Image* theImage);
    /*inline*/ Image*               GetImageOverride(const char* theTrackName);
    void                            ShowOnlyTrack(const char* theTrackName);
    void                            GetTrackMatrix(int theTrackIndex, SexyTransform2D& theMatrix);
    void                            AssignRenderGroupToTrack(const char* theTrackName, int theRenderGroup);
    void                            AssignRenderGroupToPrefix(const char* theTrackName, int theRenderGroup);
    void                            PropogateColorToAttachments();
    bool                            ShouldTriggerTimedEvent(float theEventTime);
    void                            TodTriangleGroupDraw(Graphics* g, TodTriangleGroup* theTriangleGroup) { ; }
    Image*                          GetCurrentTrackImage(const char* theTrackName);
    AttachEffect*                   AttachParticleToTrack(const char* theTrackName, TodParticleSystem* theParticleSystem, float thePosX, float thePosY);
    void                            GetTrackBasePoseMatrix(int theTrackIndex, SexyTransform2D& theBasePosMatrix);
    bool                            IsTrackShowing(const char* theTrackName);
    /*inline*/ void                 SetTruncateDisappearingFrames(const char* theTrackName = nullptr, bool theTruncateDisappearingFrames = false);
    /*inline*/ void                 PlayReanim(const char* theTrackName, ReanimLoopType theLoopType, int theBlendTime, float theAnimRate);
    void                            ReanimationDelete();
    ReanimatorTrackInstance*        GetTrackInstanceByName(const char* theTrackName);
    void                            GetFramesForLayer(const char* theTrackName, int& theFrameStart, int& theFrameCount);
    void                            UpdateAttacherTrack(int theTrackIndex);
    static void                     ParseAttacherTrack(const ReanimatorTransform& theTransform, AttacherInfo& theAttacherInfo);
    void                            AttacherSynchWalkSpeed(int theTrackIndex, Reanimation* theAttachReanim, AttacherInfo& theAttacherInfo);
    /*inline*/ bool                 IsAnimPlaying(const char* theTrackName);
    void                            SetBasePoseFromAnim(const char* theTrackName);
    void                            ReanimBltMatrix(Graphics* g, Image* theImage, SexyMatrix3& theTransform, const Rect& theClipRect, const Color& theColor, int theDrawMode, const Rect& theSrcRect);
    Reanimation*                    FindSubReanim(ReanimationType theReanimType);
};

void                                ReanimationCreateAtlas(ReanimatorDefinition* theDefinition, ReanimationType theReanimationType);
void                                ReanimationPreload(ReanimationType theReanimationType);
void                                BlendTransform(ReanimatorTransform* theResult, const ReanimatorTransform& theTransform1, const ReanimatorTransform& theTransform2, float theBlendFactor);

#endif

