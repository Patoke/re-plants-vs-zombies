#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include <windows.h>
#include "Common.h"

namespace Sexy
{

class SoundInstance;

#define MAX_SOURCE_SOUNDS	256
#define MAX_CHANNELS		32

class SoundManager
{
public:
	SoundManager() {}
	virtual ~SoundManager() {}

	virtual bool			Initialized() = NULL;

	virtual bool			LoadSound(unsigned int theSfxID, const std::string& theFilename) = NULL;
	virtual int				LoadSound(const std::string& theFilename) = NULL;
	virtual void			ReleaseSound(unsigned int theSfxID) = NULL;

	virtual void			SetVolume(double theVolume) = NULL;
	virtual bool			SetBaseVolume(unsigned int theSfxID, double theBaseVolume) = NULL;
	virtual bool			SetBasePan(unsigned int theSfxID, int theBasePan) = NULL;	

	virtual SoundInstance*	GetSoundInstance(unsigned int theSfxID) = NULL;

	virtual void			ReleaseSounds() = NULL;
	virtual void			ReleaseChannels() = NULL;

	virtual double			GetMasterVolume() = NULL;
	virtual void			SetMasterVolume(double theVolume) = NULL;

	virtual void			Flush() = NULL;
	virtual void			SetCooperativeWindow(HWND theHWnd, bool isWindowed) = NULL;
	virtual void			StopAllSounds() = NULL;
	virtual int				GetFreeSoundId() = NULL;
	virtual int				GetNumSounds() = NULL;

};


}

#endif //__SOUNDMANAGER_H__