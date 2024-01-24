#ifndef __BASS_SOUND_INSTANCE_H__
#define __BASS_SOUND_INSTANCE_H__

#include "SoundInstance.h"
#include <bass.h>

namespace Sexy {

class BassSoundManager;

class BassSoundInstance : public Sexy::SoundInstance
{
public:
	BassSoundInstance(HSAMPLE theSourceSound);
	virtual 				~BassSoundInstance();
	virtual void			Release();
	
	virtual void			SetBaseVolume(double /*theBaseVolume*/){}
	virtual void			SetBasePan(int /*theBasePan*/){}

	virtual void			SetVolume(double theVolume); 
	virtual void			SetPan(int thePosition); //-hundredth db to +hundredth db = left to right	
	virtual void			AdjustPitch(double theNumSteps);

	virtual bool			Play(bool looping, bool autoRelease);
	virtual void			Stop();
	virtual bool			IsPlaying();
	virtual bool			IsReleased();
	virtual double			GetVolume();
private:
	void					RehupVolume();
	void					RehupPan();

	HSAMPLE mSample;
	HCHANNEL mChannel;

	bool					mAutoRelease = false;
	bool					mHasPlayed = false;
	bool					mReleased = false;

	int						mPan = 0;
	double					mVolume = 1.0;	

	DWORD					mDefaultFrequency = 44100;
};

} // namespace Sexy

#endif // __BASS_SOUND_INSTANCE_H__
