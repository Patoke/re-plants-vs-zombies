#ifndef __SOUNDINSTANCE_H__
#define __SOUNDINSTANCE_H__

#include <windows.h>
#include "Common.h"

namespace Sexy
{

class SoundInstance
{
public:
	SoundInstance() {}
	virtual ~SoundInstance() {}
	virtual void			Release() = NULL;
		
	virtual void			SetBaseVolume(double theBaseVolume) = NULL; 
	virtual void			SetBasePan(int theBasePan) = NULL;

	virtual void			AdjustPitch(double theNumSteps) = NULL;

	virtual void			SetVolume(double theVolume) = NULL; 
	virtual void			SetPan(int thePosition) = NULL; //-hundredth db to +hundredth db = left to right

	virtual bool			Play(bool looping, bool autoRelease) = NULL;	
	virtual void			Stop() = NULL;
	virtual bool			IsPlaying() = NULL;
	virtual bool			IsReleased() = NULL;
	virtual double			GetVolume() = NULL;
};

}

#endif //__SOUNDINSTANCE_H__