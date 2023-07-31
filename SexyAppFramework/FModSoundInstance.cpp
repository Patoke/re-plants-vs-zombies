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
	
void FModSoundInstance::SetVolume(double theVolume)
{
}

void FModSoundInstance::SetPan(int thePosition)
{
}

bool FModSoundInstance::Play(bool looping, bool autoRelease)
{
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
