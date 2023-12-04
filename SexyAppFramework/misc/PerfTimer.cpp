#include "PerfTimer.h"
#include <map>

using namespace Sexy;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
inline int QueryCounters(__int64 *lpPerformanceCount)
{
	(void)lpPerformanceCount;
	// Argh fuck it just hope it never happens
	__builtin_unreachable();
	// returns TSC only
	/*
	asm (
		"mov ebx, dword ptr [lpPerformanceCount]"
		"rdtsc"
		"	mov dword ptr [ebx], eax"
		"	mov dword ptr [ebx+4], edx"
	);
	return 1;
	*/
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
inline int DeltaCounters(__int64 *lpPerformanceCount)
{
	(void)lpPerformanceCount;
	// Argh fuck it just hope it never happens
	__builtin_unreachable();
	/*
	asm (
		"mov ebx, dword ptr [lpPerformanceCount]"
		"rdtsc"
		"	sub eax, dword ptr [ebx]"
		"	sbb edx, dword ptr [ebx+4]"
		"	mov dword ptr [ebx],   eax"
		"		mov dword ptr [ebx+4], edx"
	);
	return 1;
	*/
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static __int64 CalcCPUSpeed()
{
	int aPriority = GetThreadPriority(GetCurrentThread());
	SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_HIGHEST);
	LARGE_INTEGER	goal, current, period;
	__int64 Ticks;

	if( !QueryPerformanceFrequency( &period ) ) return 0;

	QueryPerformanceCounter(&goal);
	goal.QuadPart+=period.QuadPart/100;
	QueryCounters( &Ticks );
	do
	{
		QueryPerformanceCounter(&current);
	} while(current.QuadPart<goal.QuadPart);
	DeltaCounters( &Ticks );

	SetThreadPriority(GetCurrentThread(),aPriority);
	return( Ticks * 100 );		// Hz

}

static __int64 gCPUSpeed = 0;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
PerfTimer::PerfTimer()
{
	mDuration = 0;
	mStart.QuadPart = 0;
	mRunning = false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PerfTimer::CalcDuration()
{
	LARGE_INTEGER anEnd, aFreq;
	QueryPerformanceCounter(&anEnd);
	QueryPerformanceFrequency(&aFreq);
	mDuration = ((anEnd.QuadPart-mStart.QuadPart)*1000)/(double)aFreq.QuadPart;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PerfTimer::Start()
{
	mRunning = true;
	QueryPerformanceCounter(&mStart);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void PerfTimer::Stop()
{
	if(mRunning)
	{
		CalcDuration();
		mRunning = false;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double PerfTimer::GetDuration()
{
	if(mRunning)
		CalcDuration();

	return mDuration;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
__int64 PerfTimer::GetCPUSpeed()
{
	if(gCPUSpeed<=0)
	{
		gCPUSpeed = CalcCPUSpeed();
		if (gCPUSpeed<=0)
			gCPUSpeed = 1;
	}

	return gCPUSpeed;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int PerfTimer::GetCPUSpeedMHz()
{
	return (int)(gCPUSpeed/1000000);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct PerfInfo
{
	const char *mPerfName;
	mutable __int64 mStartTime;
	mutable __int64 mDuration;
	mutable double mMillisecondDuration;
	mutable double mLongestCall;
	mutable int mStartCount;
	mutable int mCallCount;

	PerfInfo(const char *theName) : mPerfName(theName), mStartTime(0), mDuration(0), mLongestCall(0), mStartCount(0), mCallCount(0) { }

	bool operator<(const PerfInfo &theInfo) const { return stricmp(mPerfName,theInfo.mPerfName)<0; }
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef std::set<PerfInfo> PerfInfoSet;
static PerfInfoSet gPerfInfoSet;
static bool gPerfOn = false;
static __int64 gStartTime;
static __int64 gCollateTime;
double gDuration = 0;
int gStartCount = 0;
int gPerfRecordTop = 0;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct PerfRecord
{
	const char *mName;
	__int64 mTime;
	bool mStart;

	PerfRecord() { }
	PerfRecord(const char *theName, bool start) : mName(theName), mStart(start) { QueryCounters(&mTime); }
};
typedef std::vector<PerfRecord> PerfRecordVector;
PerfRecordVector gPerfRecordVector;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static inline void InsertPerfRecord(PerfRecord &theRecord)
{
	if(theRecord.mStart)
	{
		PerfInfoSet::iterator anItr = gPerfInfoSet.insert(PerfInfo(theRecord.mName)).first;
		anItr->mCallCount++;

		if ( ++anItr->mStartCount == 1)
			anItr->mStartTime = theRecord.mTime;
	}
	else
	{
		PerfInfoSet::iterator anItr = gPerfInfoSet.find(theRecord.mName);
		if(anItr != gPerfInfoSet.end())
		{
			if( --anItr->mStartCount == 0)
			{
				__int64 aDuration = theRecord.mTime - anItr->mStartTime;
				anItr->mDuration += aDuration;

				if (aDuration > anItr->mLongestCall)
					anItr->mLongestCall = (double)aDuration;
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static inline void CollatePerfRecords()
{
	__int64 aTime1,aTime2;
	QueryCounters(&aTime1);

	for(int i=0; i<gPerfRecordTop; i++)
		InsertPerfRecord(gPerfRecordVector[i]);

	gPerfRecordTop = 0;
	QueryCounters(&aTime2);

	gCollateTime += aTime2-aTime1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static inline void PushPerfRecord(PerfRecord theRecord)
{
	if(gPerfRecordTop >= (int)gPerfRecordVector.size())
		gPerfRecordVector.push_back(theRecord);
	else
		gPerfRecordVector[gPerfRecordTop] = theRecord;

	++gPerfRecordTop;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool SexyPerf::IsPerfOn()
{
	return gPerfOn;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SexyPerf::BeginPerf(bool measurePerfOverhead)
{
	gPerfInfoSet.clear();
	gPerfRecordTop = 0;
	gStartCount = 0;
	gCollateTime = 0;

	if(!measurePerfOverhead)
		gPerfOn = true;
	
	QueryCounters(&gStartTime);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SexyPerf::EndPerf()
{
	__int64 anEndTime;
	QueryCounters(&anEndTime);

	CollatePerfRecords();

	gPerfOn = false;

	__int64 aFreq = PerfTimer::GetCPUSpeed();

	gDuration = ((double)(anEndTime - gStartTime - gCollateTime))*1000/aFreq;

	for (PerfInfoSet::iterator anItr = gPerfInfoSet.begin(); anItr != gPerfInfoSet.end(); ++anItr)
	{
		const PerfInfo &anInfo = *anItr;
		anInfo.mMillisecondDuration = (double)anInfo.mDuration*1000/aFreq;
		anInfo.mLongestCall = anInfo.mLongestCall*1000/aFreq;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SexyPerf::StartTiming(const char *theName)
{
	if(gPerfOn)
	{
		++gStartCount;
		PushPerfRecord(PerfRecord(theName,true));
	}
}

	
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void SexyPerf::StopTiming(const char *theName)
{
	if(gPerfOn)
	{
		PushPerfRecord(PerfRecord(theName,false));
		if(--gStartCount==0)
			CollatePerfRecords();
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
std::string SexyPerf::GetResults()
{
	std::string aResult;
	char aBuf[512];

	sprintf(aBuf,"Total Time: %.2f\n",gDuration);
	aResult += aBuf;
	for (PerfInfoSet::iterator anItr = gPerfInfoSet.begin(); anItr != gPerfInfoSet.end(); ++anItr)
	{
		const PerfInfo &anInfo = *anItr;
		sprintf(aBuf,"%s (%d calls, %%%.2f time): %.2f (%.2f avg, %.2f longest)\n",anInfo.mPerfName,anInfo.mCallCount,anInfo.mMillisecondDuration/gDuration*100,anInfo.mMillisecondDuration,anInfo.mMillisecondDuration/anInfo.mCallCount,anInfo.mLongestCall);
		aResult += aBuf;
	}


	return aResult;
}

