#pragma warning( disable : 4786 )

#include "CritSect.h"
#define NOMINMAX 1
#include <windows.h>

using namespace Sexy;

////////////////////////////////////////////////////////////////////////////////

CritSect::CritSect(void)
{
	InitializeCriticalSection(&mCriticalSection);
}

////////////////////////////////////////////////////////////////////////////////

CritSect::~CritSect(void)
{
	DeleteCriticalSection(&mCriticalSection);
}
