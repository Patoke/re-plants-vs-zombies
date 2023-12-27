#ifndef _H_CritSect
#define _H_CritSect

#include "Common.h"

class CritSync;

namespace Sexy
{

class CritSect 
{
private:
	//CRITICAL_SECTION mCriticalSection;
	//pthread_mutex_t mCriticalSection;
	friend class AutoCrit;

public:
	CritSect(void);
	~CritSect(void);
	
	void Lock();
	void Unlock();
};
}

#endif // _H_CritSect
