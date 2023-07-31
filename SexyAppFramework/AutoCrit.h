#ifndef __AUTOCRIT_INCLUDED__
#define __AUTOCRIT_INCLUDED__

#include "Common.h"
#include "CritSect.h"

namespace Sexy
{

class AutoCrit
{
	LPCRITICAL_SECTION		mCritSec;
public:
	AutoCrit(LPCRITICAL_SECTION theCritSec) : 
		mCritSec(theCritSec)
	{ 
		EnterCriticalSection(mCritSec); 
	}

	AutoCrit(const CritSect& theCritSect) : 
		mCritSec((LPCRITICAL_SECTION) &theCritSect.mCriticalSection)
	{ 
		EnterCriticalSection(mCritSec); 
	}

	~AutoCrit()
	{ 
		LeaveCriticalSection(mCritSec); 
	}
};

}

#endif //__AUTOCRIT_INCLUDED__