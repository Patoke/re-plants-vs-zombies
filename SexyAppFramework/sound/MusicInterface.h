#ifndef __MUSICINTERFACE_H__
#define __MUSICINTERFACE_H__

namespace Sexy
{

class MusicInterface
{
public:
	MusicInterface() {}
	virtual ~MusicInterface() {}

	virtual bool			LoadMusic(int theSongId, const std::string& theFileName) = 0;
	virtual void			PlayMusic(int theSongId, int theOffset = 0, bool noLoop = false) = 0;
	virtual void			StopMusic(int theSongId) = 0;
	virtual void			PauseMusic(int theSongId) = 0;
	virtual void			ResumeMusic(int theSongId) = 0;
	virtual void			StopAllMusic() = 0;

	virtual void			UnloadMusic(int theSongId) = 0;
	virtual void			UnloadAllMusic() = 0;
	virtual void			PauseAllMusic() = 0;
	virtual void			ResumeAllMusic() = 0;
	
	virtual void			FadeIn(int theSongId, int theOffset = -1, double theSpeed = 0.002, bool noLoop = false) = 0;
	virtual void			FadeOut(int theSongId, bool stopSong = true, double theSpeed = 0.004) = 0;
	virtual void			FadeOutAll(bool stopSong = true, double theSpeed = 0.004) = 0;
	virtual void			SetSongVolume(int theSongId, double theVolume) = 0;
	virtual void			SetSongMaxVolume(int theSongId, double theMaxVolume) = 0;
	virtual bool			IsPlaying(int theSongId) = 0;
	
	virtual void			SetVolume(double theVolume) = 0;
	virtual void			SetMusicAmplify(int theSongId, double theAmp) = 0;
	virtual void			Update() = 0;
};
}

#endif //__MUSICINTERFACE_H__
