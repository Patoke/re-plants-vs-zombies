#ifndef __DUMMY_MUSIC_INTERFACE_H__
#define __DUMMY_MUSIC_INTERFACE_H__

#include "MusicInterface.h"
#include "SexyAppBase.h"

using namespace Sexy;

class DummyMusicInterface : public MusicInterface
{
public:
	DummyMusicInterface() {}
	virtual ~DummyMusicInterface() {};

	virtual bool			LoadMusic(int, const std::string&){return false;}
	virtual void 			PlayMusic(int, int, bool){}
	virtual void 			StopMusic(int){}
	virtual void 			PauseMusic(int){}
	virtual void 			ResumeMusic(int){}
	virtual void 			StopAllMusic(){}
	virtual void 			UnloadMusic(int){}
	virtual void 			UnloadAllMusic(){}
	virtual void 			PauseAllMusic(){}
	virtual void 			ResumeAllMusic(){}
	virtual void 			FadeIn(int, int, double, bool){}
	virtual void 			FadeOut(int, bool, double){}
	virtual void 			FadeOutAll(bool, double){}
	virtual void 			SetSongVolume(int, double){}
	virtual void 			SetSongMaxVolume(int, double){}
	virtual bool			IsPlaying(int){return false;};
	
	virtual void			SetVolume(double){}
	virtual void			SetMusicAmplify(int, double){}
	virtual void			Update(){}
};

#endif // __DUMMY_MUSIC_INTERFACE_H__
