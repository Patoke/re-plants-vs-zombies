#ifndef __AUTOCRIT_INCLUDED__
#define __AUTOCRIT_INCLUDED__

#include "Common.h"
#include "CritSect.h"

namespace Sexy
{

class AutoCrit
{
    CritSect *mCritSec;
public:
    AutoCrit(CritSect& theCritSect) :
        mCritSec(&theCritSect)
    {
        mCritSec->Lock();
    }

    ~AutoCrit()
    {
        mCritSec->Unlock();
    }
};
}

#endif //__AUTOCRIT_INCLUDED__
