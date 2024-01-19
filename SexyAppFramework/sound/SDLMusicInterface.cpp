#include "SDLMusicInterface.h"
#include "BassLoader.h"
#include "paklib/PakInterface.h"
#include <SDL_mixer.h>

using namespace Sexy;

#define BASS2_MUSIC_RAMP			BASS_MUSIC_RAMP	// normal ramping

#define BASS_CONFIG_BUFFER			0

SDLMusicInfo::SDLMusicInfo()
{
	mVolume = 0.0;
	mVolumeAdd = 0.0;
	mVolumeCap = 1.0;
	mStopOnFade = false;
	mHMusic = 0;
	mHStream = 0;
}

SDLMusicInterface::SDLMusicInterface(HWND theHWnd)
{
	LoadBassDLL();

	MIXERCONTROLDETAILS mcd;
	MIXERCONTROLDETAILS_UNSIGNED mxcd_u;
	MIXERLINECONTROLS mxlc;
	MIXERCONTROL mlct;
	MIXERLINE mixerLine;
	HMIXEROBJ phmx;
	MIXERCAPS pmxcaps;

	mixerOpen((HMIXER*)&phmx, 0, 0, 0, MIXER_OBJECTF_MIXER);
	mixerGetDevCaps(0, &pmxcaps, sizeof(pmxcaps));

	mxlc.cbStruct = sizeof(mxlc);
	mxlc.cbmxctrl = sizeof(mlct);
	mxlc.pamxctrl = &mlct;
	mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mixerLine.cbStruct = sizeof(mixerLine);
	mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
	mixerGetLineInfo(phmx, &mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE);
	mxlc.dwLineID = mixerLine.dwLineID;
	mixerGetLineControls(phmx, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE);

	mcd.cbStruct = sizeof(mcd);
	mcd.dwControlID = mlct.dwControlID;
	mcd.cChannels = 1;
	mcd.cMultipleItems = 0;
	mcd.cbDetails = sizeof(mxcd_u);
	mcd.paDetails = &mxcd_u;

	mixerGetControlDetails(phmx, &mcd, 0L);

	//return mxcd_u.dwValue;	

	BOOL success; (void)success;

	success = gBass->BASS_Init(1, 44100, 0, theHWnd, NULL);
	gBass->BASS_SetConfig(BASS_CONFIG_BUFFER, 2000);

	mixerSetControlDetails(phmx, &mcd, 0L);

	gBass->BASS_Start();

	mixerClose((HMIXER)phmx);

	mMaxMusicVolume = 40;

	mMusicLoadFlags = gBass->mVersion2 ? BASS_MUSIC_LOOP | BASS2_MUSIC_RAMP : BASS_MUSIC_LOOP;

	Mix_Init(
		MIX_INIT_FLAC	|
		MIX_INIT_MOD	|
		MIX_INIT_MP3	|
		MIX_INIT_OGG	|
		MIX_INIT_OPUS
	);
}

SDLMusicInterface::~SDLMusicInterface()
{
	gBass->BASS_Stop();
	gBass->BASS_Free();

	FreeBassDLL();

	Mix_Quit();
}

bool SDLMusicInterface::LoadMusic(int theSongId, const std::string& theFileName)
{
	HMUSIC aHMusic = 0;
	HSTREAM aStream = 0;

	std::string anExt;
	size_t aDotPos = theFileName.find_last_of('.');
	if (aDotPos != std::string::npos)
		anExt = StringToLower(theFileName.substr(aDotPos + 1));

	if (anExt == "wav" || anExt == "ogg" || anExt == "mp3")
		aStream = gBass->BASS_StreamCreateFile(FALSE, (void*)theFileName.c_str(), 0, 0, 0);
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

		aHMusic = gBass->BASS_MusicLoad(FALSE, (void*)theFileName.c_str(), 0, 0, BASS_MUSIC_LOOP | BASS2_MUSIC_RAMP, 0);

		delete[] aData;
	}

	if (aHMusic == 0 && aStream == 0)
		return false;

	SDLMusicInfo aMusicInfo;
	aMusicInfo.mHMusic = aHMusic;
	aMusicInfo.mHStream = aStream;
	mMusicMap.insert(SDLMusicMap::value_type(theSongId, aMusicInfo));

	return true;
}

void SDLMusicInterface::PlayMusic(int theSongId, int theOffset, bool noLoop)
{
	SDLMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = aMusicInfo->mVolumeCap;
		aMusicInfo->mVolumeAdd = 0.0;
		aMusicInfo->mStopOnFade = noLoop;
		gBass->BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, (int)(aMusicInfo->mVolume));

		gBass->BASS_ChannelStop(aMusicInfo->GetHandle());
		if (aMusicInfo->mHMusic)
		{
			if (gBass->mVersion2)
				gBass->BASS_MusicPlayEx(aMusicInfo->mHMusic, theOffset, BASS_MUSIC_POSRESET | BASS2_MUSIC_RAMP | (noLoop ? 0 : BASS_MUSIC_LOOP), TRUE);
			else
				gBass->BASS_MusicPlayEx(aMusicInfo->mHMusic, theOffset, noLoop ? 0 : BASS_MUSIC_LOOP, TRUE);
		}
		else
		{
			BOOL flush = theOffset == -1 ? FALSE : TRUE;
			gBass->BASS_StreamPlay(aMusicInfo->mHStream, flush, noLoop ? 0 : BASS_MUSIC_LOOP);
			if (theOffset > 0)
				gBass->BASS_ChannelSetPosition(aMusicInfo->mHStream, theOffset, BASS_POS_BYTE);
		}
	}
}

void SDLMusicInterface::StopMusic(int theSongId)
{
	SDLMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = 0.0;
		gBass->BASS_ChannelStop(aMusicInfo->GetHandle());
	}
}

void SDLMusicInterface::StopAllMusic()
{
	SDLMusicMap::iterator anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = 0.0;
		gBass->BASS_ChannelStop(aMusicInfo->GetHandle());
		++anItr;
	}
}

void SDLMusicInterface::UnloadMusic(int theSongId)
{
	StopMusic(theSongId);

	SDLMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;
		if (aMusicInfo->mHStream)
			gBass->BASS_StreamFree(aMusicInfo->mHStream);
		else if (aMusicInfo->mHMusic)
			gBass->BASS_MusicFree(aMusicInfo->mHMusic);

		mMusicMap.erase(anItr);
	}
}

void SDLMusicInterface::UnloadAllMusic()
{
	StopAllMusic();
	for (SDLMusicMap::iterator anItr = mMusicMap.begin(); anItr != mMusicMap.end(); ++anItr)
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;
		if (aMusicInfo->mHStream)
			gBass->BASS_StreamFree(aMusicInfo->mHStream);
		else if (aMusicInfo->mHMusic)
			gBass->BASS_MusicFree(aMusicInfo->mHMusic);
	}
	mMusicMap.clear();
}

void SDLMusicInterface::PauseMusic(int theSongId)
{
	SDLMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;
		gBass->BASS_ChannelPause(aMusicInfo->GetHandle());
	}
}

void SDLMusicInterface::PauseAllMusic()
{
	for (SDLMusicMap::iterator anItr = mMusicMap.begin(); anItr != mMusicMap.end(); ++anItr)
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;
		if (gBass->BASS_ChannelIsActive(aMusicInfo->GetHandle()) == BASS_ACTIVE_PLAYING)
			gBass->BASS_ChannelPause(aMusicInfo->GetHandle());
	}
}

void SDLMusicInterface::ResumeAllMusic()
{
	for (SDLMusicMap::iterator anItr = mMusicMap.begin(); anItr != mMusicMap.end(); ++anItr)
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;

		if (gBass->BASS_ChannelIsActive(aMusicInfo->GetHandle()) == BASS_ACTIVE_PAUSED)
			gBass->BASS_ChannelResume(aMusicInfo->GetHandle());
	}
}

void SDLMusicInterface::ResumeMusic(int theSongId)
{
	SDLMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;
		gBass->BASS_ChannelResume(aMusicInfo->GetHandle());
	}
}

void SDLMusicInterface::FadeIn(int theSongId, int theOffset, double theSpeed, bool noLoop)
{
	SDLMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolumeAdd = theSpeed;
		aMusicInfo->mStopOnFade = noLoop;

		gBass->BASS_ChannelStop(aMusicInfo->GetHandle());
		gBass->BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, (int)(aMusicInfo->mVolume));
		if (aMusicInfo->mHMusic)
		{
			if (theOffset == -1)
				gBass->BASS_MusicPlay(aMusicInfo->mHMusic);
			else
			{
				gBass->BASS_MusicPlayEx(aMusicInfo->mHMusic, theOffset, BASS2_MUSIC_RAMP | (noLoop ? 0 : BASS_MUSIC_LOOP), TRUE);
			}
		}
		else
		{
			BOOL flush = theOffset == -1 ? FALSE : TRUE;
			gBass->BASS_StreamPlay(aMusicInfo->mHStream, flush, noLoop ? 0 : BASS_MUSIC_LOOP);
			if (theOffset > 0)
				gBass->BASS_ChannelSetPosition(aMusicInfo->mHStream, theOffset, BASS_POS_BYTE);
		}

	}
}

void SDLMusicInterface::FadeOut(int theSongId, bool stopSong, double theSpeed)
{
	SDLMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;

		if (aMusicInfo->mVolume != 0.0)
		{
			aMusicInfo->mVolumeAdd = -theSpeed;
		}

		aMusicInfo->mStopOnFade = stopSong;
	}
}

void SDLMusicInterface::FadeOutAll(bool stopSong, double theSpeed)
{
	SDLMusicMap::iterator anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolumeAdd = -theSpeed;
		aMusicInfo->mStopOnFade = stopSong;

		++anItr;
	}
}

void SDLMusicInterface::SetVolume(double theVolume)
{
	// int aVolume = (int) (theVolume * mMaxMusicVolume); // unused
	gBass->BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, (int)(theVolume * 10000));
	gBass->BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, (int)(theVolume * 10000));
}

void SDLMusicInterface::SetSongVolume(int theSongId, double theVolume)
{
	SDLMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolume = theVolume;
		gBass->BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, (int)(aMusicInfo->mVolume));
	}
}

void SDLMusicInterface::SetSongMaxVolume(int theSongId, double theMaxVolume)
{
	SDLMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolumeCap = theMaxVolume;
		aMusicInfo->mVolume = std::min(aMusicInfo->mVolume, theMaxVolume);
		gBass->BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, (int)(aMusicInfo->mVolume));
	}
}

bool SDLMusicInterface::IsPlaying(int theSongId)
{
	SDLMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;
		return gBass->BASS_ChannelIsActive(aMusicInfo->GetHandle()) == BASS_ACTIVE_PLAYING;
	}

	return false;
}

void SDLMusicInterface::SetMusicAmplify(int theSongId, double theAmp)
{
	SDLMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;
		gBass->BASS_MusicSetAmplify(aMusicInfo->GetHandle(), (int)(theAmp * 100));
	}
}

void SDLMusicInterface::Update()
{
	SDLMusicMap::iterator anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;

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
					gBass->BASS_ChannelStop(aMusicInfo->GetHandle());
			}

			//gBass->BASS_ChannelSetAttributes(aMusicInfo->GetHandle(), -1, (int) (aMusicInfo->mVolume*100), -101);
			gBass->BASS_ChannelSetAttribute(aMusicInfo->GetHandle(), BASS_ATTRIB_VOL, (int)(aMusicInfo->mVolume));
		}

		++anItr;
	}
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// MODs are broken up into several orders or patterns. This returns the current order a song is on.
int SDLMusicInterface::GetMusicOrder(int theSongId)
{
	SDLMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		SDLMusicInfo* aMusicInfo = &anItr->second;
		int aPosition = gBass->BASS_ChannelGetPosition(aMusicInfo->GetHandle(), BASS_POS_MUSIC_ORDER);
		return aPosition;
	}
	return -1;
}