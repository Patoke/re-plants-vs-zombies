#ifndef __DUMMY_SOUND_MANAGER_H__
#define __DUMMY_SOUND_MANAGER_H__

#include "SoundManager.h"
#include "DummySoundInstance.h"
class DummySoundManager : public Sexy::SoundManager
{
public:
	virtual bool			Initialized() { return true; }

	virtual bool			LoadSound(unsigned int, const std::string&) { return true; }
	virtual int				LoadSound(const std::string&) { return 0; }
	virtual void			ReleaseSound(unsigned int) {}

	virtual void			SetVolume(double) {}
	virtual bool			SetBaseVolume(unsigned int, double) { return 0; }
	virtual bool			SetBasePan(unsigned int, int) { return 0; }	

	virtual Sexy::SoundInstance*	GetSoundInstance(unsigned int) { return &dummy; }

	virtual void			ReleaseSounds() {}
	virtual void			ReleaseChannels() {}

	virtual double			GetMasterVolume() { return 0; }
	virtual void			SetMasterVolume(double) {}

	virtual void			Flush() {}
//	virtual void			SetCooperativeWindow(HWND theHWnd) {}
	virtual void			StopAllSounds() {}
	virtual int				GetFreeSoundId() { return 0; }
	virtual int				GetNumSounds() { return 0; }
private:
	DummySoundInstance dummy;
};

#endif // __DUMMY_SOUND_MANAGER_H__
