#ifndef __BASS_SOUND_INSTANCE_H__
#define __BASS_SOUND_INSTANCE_H__

#include "sound/SoundInstance.h"
class BassSoundInstance : public Sexy::SoundInstance
{
public:
	virtual void			Release() {}

	virtual void			SetBaseVolume(double) {} 
	virtual void			SetBasePan(int) {}

	virtual void			AdjustPitch(double) {}

	virtual void			SetVolume(double) {} 
	virtual void			SetPan(int) {} //-hundredth db to +hundredth db = left to right

	virtual bool			Play(bool, bool) { return true; }	
	virtual void			Stop() {}
	virtual bool			IsPlaying() { return false; }
	virtual bool			IsReleased() { return true; }
	virtual double			GetVolume() { return 0; }
};

#endif // __BASS_SOUND_INSTANCE_H__
