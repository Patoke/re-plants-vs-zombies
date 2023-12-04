#include "FModSoundInstance.h"
#include "FModLoader.h"

using namespace Sexy;

FModSoundInstance::FModSoundInstance()
{
    mStream = NULL;
    mChannelNum = 0;
}

FModSoundInstance::~FModSoundInstance()
{
    Release();
}

void FModSoundInstance::Release()
{   
}

void FModSoundInstance::SetVolume(double){}
void FModSoundInstance::SetPan(int){}

bool FModSoundInstance::Play(bool looping, bool autoRelease)
{
    (void)looping;(void)autoRelease;
    if (mStream == NULL)
        return false;

    gFMod->FSOUND_Stream_Play(FSOUND_FREE, mStream);
    return true;
}

void FModSoundInstance::Stop()
{
}

bool FModSoundInstance::IsPlaying()
{
    return true;
}

bool FModSoundInstance::IsReleased()
{
    return false;
}
void   FModSoundInstance::SetBaseVolume (double /*theBaseVolume*/)  {__builtin_unreachable();}
void   FModSoundInstance::SetBasePan    (int /*theBasePan*/)        {__builtin_unreachable();}
void   FModSoundInstance::AdjustPitch   (double /*theNumSteps*/)    {__builtin_unreachable();}
double FModSoundInstance::GetVolume     ()                          {__builtin_unreachable();}
