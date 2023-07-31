#ifndef _H_CritSect
#define _H_CritSect

#include "Common.h"

class CritSync;

namespace Sexy
{

class CritSect 
{
private:
	CRITICAL_SECTION mCriticalSection;
	friend class AutoCrit;

public:
	CritSect(void);
	~CritSect(void);
};

}

#endif // _H_CritSect
