#ifndef __ATTACHMENT_H__
#define __ATTACHMENT_H__

#include "DataArray.h"
#include "../ConstEnums.h"
#include "../SexyAppFramework/SexyMatrix.h"

namespace Sexy
{
    class Graphics;
}
using namespace Sexy;

#define MAX_EFFECTS_PER_ATTACHMENT 16

class Trail;
class Reanimation;
class TodParticleSystem;

class AttachEffect
{
public:
    unsigned int            mEffectID;
    EffectType              mEffectType;
    SexyTransform2D         mOffset;
    bool                    mDontDrawIfParentHidden;
    bool                    mDontPropogateColor;
};

class AttacherInfo
{
public:
    std::string             mReanimName;
    std::string             mTrackName;
    float                   mAnimRate;
    ReanimLoopType          mLoopType;

public:
    ~AttacherInfo() { ; }
};

class Attachment
{
public:
    AttachEffect            mEffectArray[MAX_EFFECTS_PER_ATTACHMENT];
    int                     mNumEffects;
    bool                    mDead;

public:
    Attachment();
    ~Attachment();

    void                    Update();
    void                    SetPosition(const SexyVector2& thePosition);
    void                    SetMatrix(const SexyTransform2D& theMatrix);
    void                    OverrideColor(const Color& theColor);
    void                    OverrideScale(float theScale);
    void                    Draw(Graphics* g, bool theParentHidden);
    void                    AttachmentDie();
    void                    Detach();
    void                    CrossFade(const char* theCrossFadeName);
    void                    PropogateColor(const Color& theColor, bool theEnableAdditiveColor, const Color& theAdditiveColor, bool theEnableOverlayColor, const Color& theOverlayColor);
};

AttachEffect*               AttachReanim(AttachmentID& theAttachmentID, Reanimation* theReanimation, float theOffsetX, float theOffsetY);
AttachEffect*               AttachParticle(AttachmentID& theAttachmentID, TodParticleSystem* theParticleSystem, float theOffsetX, float theOffsetY);
AttachEffect*               AttachTrail(AttachmentID& theAttachmentID, Trail* theTrail, float theOffsetX, float theOffsetY);
/*inline*/ void             AttachmentPropogateColor(AttachmentID& theAttachmentID, const Color& theColor, bool theEnableAdditiveColor, const Color& theAdditiveColor, bool theEnableOverlayColor, const Color& theOverlayColor);
/*inline*/ void             AttachmentOverrideColor(AttachmentID& theAttachmentID, const Color& theColor);
/*inline*/ void             AttachmentOverrideScale(AttachmentID& theAttachmentID, float theScale);
/*inline*/ void             AttachmentUpdateAndMove(AttachmentID& theAttachmentID, float theX, float theY);
/*inline*/ void             AttachmentUpdateAndSetMatrix(AttachmentID& theAttachmentID, SexyTransform2D& theMatrix);
/*inline*/ void             AttachmentDraw(AttachmentID& theAttachmentID, Graphics* g, bool theParentHidden);
/*inline*/ void             AttachmentDetach(AttachmentID& theAttachmentID);
/*inline*/ void             AttachmentDetachCrossFadeParticleType(AttachmentID& theAttachmentID, ParticleEffect theParticleEffect, const char* theCrossFadeName);
/*inline*/ void             AttachmentReanimTypeDie(AttachmentID& theAttachmentID, ReanimationType theReanimType);
/*inline*/ void             AttachmentDie(AttachmentID& theAttachmentID);
/*inline*/ void             AttachmentCrossFade(AttachmentID& theAttachmentID, const char* theCrossFadeName);
AttachEffect*               FindFirstAttachment(AttachmentID& theAttachmentID);
Reanimation*                FindReanimAttachment(AttachmentID& theAttachmentID);
AttachEffect*               CreateEffectAttachment(AttachmentID& theAttachmentID, EffectType theEffectType, unsigned int theDataID, float theOffsetX, float theOffsetY);
/*inline*/ bool             IsFullOfAttachments(AttachmentID& theAttachmentID);

class AttachmentHolder
{
public:
    DataArray<Attachment>   mAttachments;

public:
    AttachmentHolder();
    ~AttachmentHolder();

    void                    InitializeHolder();
    void                    DisposeHolder();
    Attachment*             AllocAttachment();
};

#endif