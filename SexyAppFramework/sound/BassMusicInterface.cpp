#include "bass.h"
#include "BassMusicInterface.h"
#include "paklib/PakInterface.h"

using namespace Sexy;

#define BASS2_MUSIC_RAMP			BASS_MUSIC_RAMP	// normal ramping

#define BASS_CONFIG_BUFFER			0

BassMusicInfo::BassMusicInfo()
{	
	mVolume = 0.0;
	mVolumeAdd = 0.0;
	mVolumeCap = 1.0;
	mStopOnFade = false;
	mHMusic = 0;
	mHStream = 0;
}

BassMusicInterface::BassMusicInterface(void *theHWnd)
{
	BOOL success;(void)success;

	success = BASS_Init(1, 44100, 0, theHWnd, NULL);
	BASS_SetConfig(BASS_CONFIG_BUFFER, 2000);	

	BASS_Start();

	mMaxMusicVolume = 40;

	mMusicLoadFlags = BASS_MUSIC_LOOP | BASS2_MUSIC_RAMP;
}

BassMusicInterface::~BassMusicInterface()
{
	BASS_Stop();
	BASS_Free();
}

bool BassMusicInterface::LoadMusic(int theSongId, const std::string& theFileName)
{
	HMUSIC aHMusic = 0;
	HSTREAM aStream = 0;
	
	std::string anExt;
	size_t aDotPos = theFileName.find_last_of('.');
	if (aDotPos!=std::string::npos)
		anExt = StringToLower(theFileName.substr(aDotPos+1));

	if (anExt=="wav" || anExt=="ogg" || anExt=="mp3")
		aStream = BASS_StreamCreateFile(FALSE, (void*) theFileName.c_str(), 0, 0, 0);
	else
	{
		PFILE* aFP = p_fopen(theFileName.c_str(), "rb");
		if (aFP == NULL)
			return false;

		p_fseek(aFP, 0, SEEK_END);
		int aSize = p_ftell(aFP);
		p_fseek(aFP, 0, SEEK_SET);

		uchar* aData = new uchar[aSize];
		p_fread(aData, 1, aSize, aFP);
		p_fclose(aFP);

		aHMusic = BASS_MusicLoad(FALSE, (void*) theFileName.c_str(), 0, 0, BASS_MUSIC_LOOP | BASS2_MUSIC_RAMP, 0);

		delete[] aData;
	}

	if (aHMusic==0 && aStream==0)
		return false;
	
	BassMusicInfo aMusicInfo;	
	aMusicInfo.mHMusic = aHMusic;
	aMusicInfo.mHStream = aStream;
	mMusicMap.insert(BassMusicMap::value_type(theSongId, aMusicInfo));

	return true;	
}

BOOL BASS_MusicPlayEx(HMUSIC handle, DWORD pos, int flags, BOOL reset)
{
	(void)reset;
	//int anOffset = MAKEMUSICPOS(pos,0);

	BASS_ChannelStop(handle);
	BASS_ChannelSetPosition(handle, MAKELONG(pos,0), BASS_POS_MUSIC_ORDER);
	BASS_ChannelFlags(handle, flags, -1);

	return BASS_ChannelPlay(handle, false/*reset*/); // What's wrong with actually using the reset flag?
}

BOOL BASS_StreamPlay(HSTREAM handle, BOOL flush, DWORD flags)
{
	BASS_ChannelFlags(handle, flags, -1);
	return BASS_ChannelPlay(handle, flush);
}

BOOL BASS_ChannelResume(DWORD handle)
{
	return BASS_ChannelPlay(handle, false);
}

BOOL BASS_MusicPlay(HMUSIC handle)
{
	return BASS_ChannelPlay(handle, true);
}

BOOL BASS_MusicSetAmplify(HMUSIC handle, DWORD amp)
{
	BASS_ChannelSetAttribute(handle, BASS_ATTRIB_MUSIC_AMPLIFY, amp);
	return true;
}

void BassMusicInterface::PlayMusic(int theSongId, int theOffset, bool noLoop)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = aMusicInfo->mVolumeCap;
		aMusicInfo->mVolumeAdd = 0.0;
		aMusicInfo->mStopOnFade = noLoop;
		BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, (int) (aMusicInfo->mVolume));

		BASS_ChannelStop(aMusicInfo->GetHandle());
		if (aMusicInfo->mHMusic)
		{
			BASS_MusicPlayEx(aMusicInfo->mHMusic, theOffset, BASS_MUSIC_POSRESET | BASS2_MUSIC_RAMP | (noLoop ? 0 : BASS_MUSIC_LOOP), TRUE);
		}
		else
		{
			BOOL flush = theOffset == -1 ? FALSE : TRUE;
			BASS_StreamPlay(aMusicInfo->mHStream, flush, noLoop ? 0 : BASS_MUSIC_LOOP);
			if (theOffset > 0)
				BASS_ChannelSetPosition(aMusicInfo->mHStream, theOffset, BASS_POS_BYTE);
		}
	}
}

void BassMusicInterface::StopMusic(int theSongId)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = 0.0;
		BASS_ChannelStop(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::StopAllMusic()
{
	BassMusicMap::iterator anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = 0.0;
		BASS_ChannelStop(aMusicInfo->GetHandle());
		++anItr;
	}
}

void BassMusicInterface::UnloadMusic(int theSongId)
{
	StopMusic(theSongId);
	
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		if (aMusicInfo->mHStream)
			BASS_StreamFree(aMusicInfo->mHStream);
		else if (aMusicInfo->mHMusic)
			BASS_MusicFree(aMusicInfo->mHMusic);

		mMusicMap.erase(anItr);
	}
}

void BassMusicInterface::UnloadAllMusic()
{
	StopAllMusic();
	for (BassMusicMap::iterator anItr = mMusicMap.begin(); anItr != mMusicMap.end(); ++anItr)
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		if (aMusicInfo->mHStream)
			BASS_StreamFree(aMusicInfo->mHStream);
		else if (aMusicInfo->mHMusic)
			BASS_MusicFree(aMusicInfo->mHMusic);
	}
	mMusicMap.clear();
}

void BassMusicInterface::PauseMusic(int theSongId)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		BASS_ChannelPause(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::PauseAllMusic()
{
	for (BassMusicMap::iterator anItr = mMusicMap.begin(); anItr != mMusicMap.end(); ++anItr)
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		if (BASS_ChannelIsActive(aMusicInfo->GetHandle()) == BASS_ACTIVE_PLAYING)
			BASS_ChannelPause(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::ResumeAllMusic()
{
	for (BassMusicMap::iterator anItr = mMusicMap.begin(); anItr != mMusicMap.end(); ++anItr)
	{
		BassMusicInfo* aMusicInfo = &anItr->second;

		if (BASS_ChannelIsActive(aMusicInfo->GetHandle()) == BASS_ACTIVE_PAUSED)
			BASS_ChannelResume(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::ResumeMusic(int theSongId)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
		BASS_ChannelResume(aMusicInfo->GetHandle());
	}
}

void BassMusicInterface::FadeIn(int theSongId, int theOffset, double theSpeed, bool noLoop)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
				
		aMusicInfo->mVolumeAdd = theSpeed;
		aMusicInfo->mStopOnFade = noLoop;

		BASS_ChannelStop(aMusicInfo->GetHandle());
		BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, (int) (aMusicInfo->mVolume));
		if (aMusicInfo->mHMusic)
		{
			if (theOffset == -1)
				BASS_MusicPlay(aMusicInfo->mHMusic);
			else
			{
				BASS_MusicPlayEx(aMusicInfo->mHMusic, theOffset, BASS2_MUSIC_RAMP | (noLoop ? 0 : BASS_MUSIC_LOOP), TRUE);
			}
		}
		else
		{
			BOOL flush = theOffset == -1 ? FALSE : TRUE;
			BASS_StreamPlay(aMusicInfo->mHStream, flush, noLoop ? 0 : BASS_MUSIC_LOOP);
			if (theOffset > 0)
				BASS_ChannelSetPosition(aMusicInfo->mHStream, theOffset, BASS_POS_BYTE);
		}
	}
}

void BassMusicInterface::FadeOut(int theSongId, bool stopSong, double theSpeed)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		BassMusicInfo* aMusicInfo = &anItr->second;
		
		if (aMusicInfo->mVolume != 0.0)
		{
			aMusicInfo->mVolumeAdd = -theSpeed;			
		}

		aMusicInfo->mStopOnFade = stopSong;
	}
}

void BassMusicInterface::FadeOutAll(bool stopSong, double theSpeed)
{
	BassMusicMap::iterator anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;
				
		aMusicInfo->mVolumeAdd = -theSpeed;
		aMusicInfo->mStopOnFade = stopSong;

		++anItr;
	}
}

void BassMusicInterface::SetVolume(double theVolume)
{
	// int aVolume = (int) (theVolume * mMaxMusicVolume); // unused
	BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, (int) (theVolume * 10000));
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, (int) (theVolume * 10000));
}

void BassMusicInterface::SetSongVolume(int theSongId, double theVolume)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		BassMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolume = theVolume;
		BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, (int) (aMusicInfo->mVolume));
	}
}

void BassMusicInterface::SetSongMaxVolume(int theSongId, double theMaxVolume)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		BassMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolumeCap = theMaxVolume;
		aMusicInfo->mVolume = std::min(aMusicInfo->mVolume, theMaxVolume);
		BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, (int) (aMusicInfo->mVolume));
	}
}

bool BassMusicInterface::IsPlaying(int theSongId)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		BassMusicInfo* aMusicInfo = &anItr->second;
		return BASS_ChannelIsActive(aMusicInfo->GetHandle()) == BASS_ACTIVE_PLAYING;
	}

	return false;
}

void BassMusicInterface::SetMusicAmplify(int theSongId, double theAmp)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		BassMusicInfo* aMusicInfo = &anItr->second;		
		BASS_MusicSetAmplify(aMusicInfo->GetHandle(), (int) (theAmp * 100));
	}
}

void BassMusicInterface::Update()
{
	BassMusicMap::iterator anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end())
	{
		BassMusicInfo* aMusicInfo = &anItr->second;

		if (aMusicInfo->mVolumeAdd != 0.0)
		{
			aMusicInfo->mVolume += aMusicInfo->mVolumeAdd;
			
			if (aMusicInfo->mVolume > aMusicInfo->mVolumeCap)
			{
				aMusicInfo->mVolume = aMusicInfo->mVolumeCap;
				aMusicInfo->mVolumeAdd = 0.0;
			}
			else if (aMusicInfo->mVolume < 0.0)
			{
				aMusicInfo->mVolume = 0.0;
				aMusicInfo->mVolumeAdd = 0.0;

				if (aMusicInfo->mStopOnFade)
					BASS_ChannelStop(aMusicInfo->GetHandle());
			}

			//gBass->BASS_ChannelSetAttributes(aMusicInfo->GetHandle(), -1, (int) (aMusicInfo->mVolume*100), -101);
			BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, (int) (aMusicInfo->mVolume));
		}

		++anItr;
	}	
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// MODs are broken up into several orders or patterns. This returns the current order a song is on.
int BassMusicInterface::GetMusicOrder(int theSongId)
{
	BassMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		BassMusicInfo* aMusicInfo = &anItr->second;
		int aPosition = BASS_ChannelGetPosition(aMusicInfo->GetHandle(), BASS_POS_MUSIC_ORDER);
		return aPosition;
	}
	return -1;
}
