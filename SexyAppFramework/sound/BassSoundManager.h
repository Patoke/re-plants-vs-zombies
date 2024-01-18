#ifndef __BASS_SOUND_MANAGER_H__
#define __BASS_SOUND_MANAGER_H__

#include "SoundManager.h"
#include "BassSoundInstance.h"
#include "sound/bass.h"
#include <mutex>

class BassSoundManager : public Sexy::SoundManager
{
public:
	virtual bool			Initialized() { return true; }

	virtual bool			LoadSound(unsigned int, const std::string&);
	virtual int				LoadSound(const std::string&);
	virtual void			ReleaseSound(unsigned int);

	virtual void			SetVolume(double) {}
	virtual bool			SetBaseVolume(unsigned int, double) { return 0; }
	virtual bool			SetBasePan(unsigned int, int) { return 0; }	

	virtual Sexy::SoundInstance*	GetSoundInstance(unsigned int) { return &mSoundInstance; }

	virtual void			ReleaseSounds() {}
	virtual void			ReleaseChannels() {}

	virtual double			GetMasterVolume() { return 0; }
	virtual void			SetMasterVolume(double) {}

	virtual void			Flush() {}
//	virtual void			SetCooperativeWindow(HWND theHWnd) {}
	virtual void			StopAllSounds() {}
	virtual int				GetFreeSoundId();
	virtual int				GetNumSounds();
private:
	BassSoundInstance 		mSoundInstance;
	std::string             mSourceFileNames[MAX_SOURCE_SOUNDS];
	std::optional<HSAMPLE> 	mSourceSounds[MAX_SOURCE_SOUNDS];

	bool LoadCompatibleSound(unsigned int theSfxID, const std::string& theFilename);
	bool LoadAUSound(unsigned int theSfxID, const std::string& theFilename);
};

#endif // __BASS_SOUND_MANAGER_H__
