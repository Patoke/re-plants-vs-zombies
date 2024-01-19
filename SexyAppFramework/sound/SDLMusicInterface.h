#ifndef __BASSMUSICINTERFACE_H__
#define __BASSMUSICINTERFACE_H__

#include "MusicInterface.h"
#include "bass.h"

namespace Sexy
{

	class SexyAppBase;

	class SDLMusicInfo
	{
	public:
		HMUSIC					mHMusic;
		HSTREAM					mHStream;
		double					mVolume;
		double					mVolumeAdd;
		double					mVolumeCap;
		bool					mStopOnFade;

	public:
		SDLMusicInfo();

		DWORD GetHandle() { return mHMusic ? mHMusic : mHStream; }
	};

	typedef std::map<int, SDLMusicInfo> SDLMusicMap;

	class SDLMusicInterface : public MusicInterface
	{
	public:
		SDLMusicMap			mMusicMap;
		int						mMaxMusicVolume;
		int						mMusicLoadFlags;

	public:
		SDLMusicInterface(HWND theHWnd);
		virtual ~SDLMusicInterface();

		virtual bool			LoadMusic(int theSongId, const std::string& theFileName);
		virtual void			PlayMusic(int theSongId, int theOffset = 0, bool noLoop = false);
		virtual void			StopMusic(int theSongId);
		virtual void			StopAllMusic();
		virtual void			UnloadMusic(int theSongId);
		virtual void			UnloadAllMusic();
		virtual void			PauseAllMusic();
		virtual void			ResumeAllMusic();
		virtual void			PauseMusic(int theSongId);
		virtual void			ResumeMusic(int theSongId);
		virtual void			FadeIn(int theSongId, int theOffset = -1, double theSpeed = 0.002, bool noLoop = false);
		virtual void			FadeOut(int theSongId, bool stopSong = true, double theSpeed = 0.004);
		virtual void			FadeOutAll(bool stopSong = true, double theSpeed = 0.004);
		virtual void			SetSongVolume(int theSongId, double theVolume);
		virtual void			SetSongMaxVolume(int theSongId, double theMaxVolume);
		virtual bool			IsPlaying(int theSongId);

		virtual void			SetVolume(double theVolume);
		virtual void			SetMusicAmplify(int theSongId, double theAmp); // default is 0.50
		virtual void			Update();

		// functions for dealing with MODs
		int						GetMusicOrder(int theSongId);
	};

}

#endif //__BASSMUSICINTERFACE_H__