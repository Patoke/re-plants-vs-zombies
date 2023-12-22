#include "CritSect.h"
#include <pthread.h>

using namespace Sexy;

////////////////////////////////////////////////////////////////////////////////

CritSect::CritSect(void)
{
	pthread_mutexattr_t mAttr;
	pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&mCriticalSection, &mAttr);
	pthread_mutexattr_destroy(&mAttr);
}

////////////////////////////////////////////////////////////////////////////////

CritSect::~CritSect(void)
{
	pthread_mutex_destroy(&mCriticalSection);
}

void CritSect::Lock()
{
	pthread_mutex_lock(&mCriticalSection);
}

void CritSect::Unlock()
{
	pthread_mutex_unlock(&mCriticalSection);
}
