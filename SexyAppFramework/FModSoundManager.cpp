#include "FModSoundManager.h"
#include "FModLoader.h"

using namespace Sexy;

FModSoundManager::FModSoundManager(HWND theHWnd)
{
	LoadFModDLL();

	gFMod->FSOUND_SetHWND(theHWnd);
	gFMod->FSOUND_Init(44100, 64, FSOUND_INIT_GLOBALFOCUS);

	mMasterVolume = 1.0;

	for (int i = 0; i < MAX_SOURCE_SOUNDS; i++)
		mSourceStreams[i] = NULL;
}

FModSoundManager::~FModSoundManager()
{
	for (int i = 0; i < MAX_SOURCE_SOUNDS; i++)
	{
		if (mSourceStreams[i] != NULL)
		{
			gFMod->FSOUND_Stream_Close(mSourceStreams[i]);
			mSourceStreams[i] = NULL;
		}
	}
}

bool FModSoundManager::Initialized()
{
	return true;
}

bool FModSoundManager::LoadSound(unsigned int theSfxID, const std::string& theFilename)
{	
	int aLastDotPos = theFilename.rfind('.');
	if (aLastDotPos == -1)
	{
		if (LoadSound(theSfxID, theFilename + ".wav"))
			return true;

		if (LoadSound(theSfxID, theFilename + ".mp3"))
			return true;

		if (LoadSound(theSfxID, theFilename + ".ogg"))
			return true;

		if (LoadSound(theSfxID, theFilename + ".au"))
			return true;

		return false;
	}

	FSOUND_STREAM* aStream = gFMod->FSOUND_Stream_OpenFile(theFilename.c_str(), FSOUND_2D, 0);
	if (aStream == NULL)
		return false;

	mSourceStreams[theSfxID] = aStream;
	return true;
}

bool FModSoundManager::LoadSound(const std::string& theFilename)
{
	for (int i = MAX_SOURCE_SOUNDS-1; i >= 0; i--)
	{
		if (mSourceStreams[i] == NULL)
		{
			if (!LoadSound(i, theFilename))
				return -1;
			else
				return i;
		}
	}

	return -1;
}

void FModSoundManager::SetVolume(double theVolume)
{
}

SoundInstance* FModSoundManager::GetSoundInstance(unsigned int theSfxID)
{
	FModSoundInstance* aSoundInstance = new FModSoundInstance;
	aSoundInstance->mStream = mSourceStreams[theSfxID];
	return aSoundInstance;
}

void FModSoundManager::ReleaseSounds()
{
}

void FModSoundManager::ReleaseChannels()
{
}

double FModSoundManager::GetMasterVolume()
{
	return mMasterVolume;
}

void FModSoundManager::SetMasterVolume(double theVolume)
{
}

void FModSoundManager::Flush()
{
}

void FModSoundManager::SetCooperativeWindow(HWND theHWnd)
{
}
