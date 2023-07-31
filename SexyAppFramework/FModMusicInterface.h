#ifndef __FMODMUSICINTERFACE_H__
#define __FMODMUSICINTERFACE_H__

#include "MusicInterface.h"
#include "fmod.h"

namespace Sexy
{

class SexyAppBase;

class FModMusicInfo
{
public:
	FSOUND_SAMPLE*			mHSample;
	FMUSIC_MODULE*			mHMusic;
	double					mVolume;
	double					mVolumeAdd;		
	double					mVolumeCap;		
	bool					mStopOnFade;
	bool					mRepeats;

public:
	FModMusicInfo();
};

typedef std::map<int, FModMusicInfo> FModMusicMap;

class FModMusicInterface : public MusicInterface
{
public:	
	FModMusicMap			mMusicMap;
	double					mMasterVolume;
	int						mMaxMusicVolume;
	int						mMaxSampleVolume;

public:
	FSOUND_SAMPLE*			LoadFMODSample(const std::string& theFileName);

public:
	FModMusicInterface(HWND theHWnd);
	virtual ~FModMusicInterface();
		
	virtual bool			LoadMusic(int theSongId, const std::string& theFileName);
	virtual bool			LoadSample(int theSongId, const std::string& theFileName, bool repeat);
	virtual bool			LoadSample(int theSongId, const std::string& theIntroFileName, const std::string& theRepeatFileName, bool repeat);
	virtual void			PlayMusic(int theSongId, int theOffset = 0, bool noLoop = false);	
	virtual void			StopMusic(int theSongId);
	virtual void			PauseMusic(int theSongId);
	virtual void			ResumeMusic(int theSongId);
	virtual void			StopAllMusic();		
	virtual void			FadeIn(int theSongId, int theOffset = -1, double theSpeed = 0.002, bool noLoop = false);
	virtual void			FadeOut(int theSongId, bool stopSong = true, double theSpeed = 0.004);
	virtual void			FadeOutAll(bool stopSong = true, double theSpeed = 0.004);
	virtual void			SetSongVolume(int theSongId, double theVolume);
	virtual bool			IsPlaying(int theSongId);

	virtual void			SetVolume(double theVolume);
	virtual void			Update();
};

}

#endif //__FMODMUSICINTERFACE_H__
