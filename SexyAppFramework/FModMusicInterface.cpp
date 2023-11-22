#include "FModMusicInterface.h"
#include "SexyAppBase.h"
#include "FModLoader.h"

using namespace Sexy;

FModMusicInfo::FModMusicInfo()
{
	mHSample = NULL;
	mHMusic = NULL;
	mVolume = 0.0;
	mVolumeAdd = 0.0;
	mVolumeCap = 1.0;
	mStopOnFade = false;
	mRepeats = false;
}

FModMusicInterface::FModMusicInterface(HWND theHWnd)
{	
	LoadFModDLL();

	mMasterVolume = 1.0;	
	mMaxMusicVolume = 100;
	mMaxSampleVolume = 100;
}

FModMusicInterface::~FModMusicInterface()
{
	FreeFModDLL();
}

FSOUND_SAMPLE* FModMusicInterface::LoadFMODSample(const std::string& theFileName)
{
	std::string aFilename = theFileName;

	// int aLastDotPos = aFilename.rfind('.'); // unused
	int aLastSlashPos = std::max((int) aFilename.rfind('\\'), (int) aFilename.rfind('/'));	
	if (aLastSlashPos < 0)
		aLastSlashPos = 0;

	std::string aCachedName = GetAppDataFolder() + "cached\\" + GetFileName(theFileName, true) + ".wav";
	MkDir(GetFileDir(aCachedName));

	FSOUND_SAMPLE* aSample = gFMod->FSOUND_Sample_Load(FSOUND_FREE, aCachedName.c_str(), 0, 0);

	if (aSample == NULL)			
	{
		aSample = gFMod->FSOUND_Sample_Load(FSOUND_FREE, (theFileName + ".ogg").c_str(), 0, 0);
	
		if (aSample != NULL)
		{
			int aMode = gFMod->FSOUND_Sample_GetMode(aSample);	
			int aFreq = 0;
			gFMod->FSOUND_Sample_GetDefaults(aSample, &aFreq, NULL, NULL, NULL);
									
			ushort aFormatTag = 1;
			ushort aChannelCount = ((aMode & FSOUND_MONO) != 0) ? 1 : 2;
			ulong aSampleRate = aFreq;
			ushort aBitCount = ((aMode & FSOUND_8BITS) != 0) ? 8 : 16;
			ushort aBlockAlign = aChannelCount*aBitCount/8;
			ulong aBytesPerSec = aSampleRate * aBlockAlign;

			int aDataSize = gFMod->FSOUND_Sample_GetLength(aSample) * aBlockAlign;
			ulong aChunkSize = 4 + 8 + 16 + 8 + aDataSize;			

			void* aPtr1;
			void* aPtr2;
			uint aLen1;
			uint aLen2;

			if (gFMod->FSOUND_Sample_Lock(aSample, 0, aDataSize, &aPtr1, &aPtr2, &aLen1, &aLen2))
			{
				FILE* fp = fopen(aCachedName.c_str(), "wb");

				if (fp != NULL)
				{
					fwrite("RIFF", 1, 4, fp);	
					fwrite(&aChunkSize, 4, 1, fp);
					fwrite("WAVE", 1, 4, fp);

					aChunkSize = 16;
					fwrite("fmt ", 1, 4, fp);
					fwrite(&aChunkSize, 1, 4, fp);
					fwrite(&aFormatTag, 2, 1, fp);
					fwrite(&aChannelCount, 2, 1, fp);
					fwrite(&aSampleRate, 4, 1, fp);
					fwrite(&aBytesPerSec, 4, 1, fp);
					fwrite(&aBlockAlign, 2, 1, fp);
					fwrite(&aBitCount, 2, 1, fp);					

					fwrite("data", 1, 4, fp);
					fwrite(&aDataSize, 4, 1, fp);
					fwrite(aPtr1, 1, aDataSize, fp);
					fclose(fp);
				}

				gFMod->FSOUND_Sample_Unlock(aSample, aPtr1, aPtr2, aLen1, aLen2);
			}
		}
	}
	
	return aSample;
}

bool FModMusicInterface::LoadSample(int theSongId, const std::string& theFileName, bool repeat)
{
	FSOUND_SAMPLE* aSample = LoadFMODSample(theFileName.c_str());

	if (aSample == NULL)
		return false;		

	FModMusicInfo aMusicInfo;
	aMusicInfo.mHSample = aSample;	
	aMusicInfo.mRepeats = repeat;
	mMusicMap.insert(FModMusicMap::value_type(theSongId, aMusicInfo));

	return true;	
}

bool FModMusicInterface::LoadSample(int theSongId, const std::string& theIntroFileName, const std::string& theRepeatFileName, bool repeat)
{
	FSOUND_SAMPLE* anIntroSample = LoadFMODSample(theIntroFileName.c_str());
	FSOUND_SAMPLE* aLoopSample = LoadFMODSample(theRepeatFileName.c_str());

	if ((anIntroSample == NULL) || (aLoopSample == NULL))
		return false;
	
	// Get info for intro
	int anIntroFreq;
	gFMod->FSOUND_Sample_GetDefaults(anIntroSample, &anIntroFreq, NULL, NULL, NULL);
	int anIntroMode = gFMod->FSOUND_Sample_GetMode(anIntroSample);	
	int anIntroLen = gFMod->FSOUND_Sample_GetLength(anIntroSample);

	// Get intro for loop
	int aLoopFreq;
	gFMod->FSOUND_Sample_GetDefaults(aLoopSample, &aLoopFreq, NULL, NULL, NULL);
	int aLoopMode = gFMod->FSOUND_Sample_GetMode(aLoopSample);	
	int aLoopLen = gFMod->FSOUND_Sample_GetLength(aLoopSample);
	
	// Formats must be the same
	if ((anIntroFreq != aLoopFreq) || (anIntroMode != aLoopMode))
		return false;

	// Create new sound to combine these two into
	FSOUND_SAMPLE* aSample = gFMod->FSOUND_Sample_Alloc(FSOUND_FREE, anIntroLen + aLoopLen, anIntroMode | FSOUND_LOOP_NORMAL, anIntroFreq, 255, 128, 0);
	
	void* aSamplePtr1;
	void* aSamplePtr2;
	uint aSampleLen1;
	uint aSampleLen2;

	int aSampleBytes = (((anIntroMode & FSOUND_MONO) != 0) ? 1 : 2) * (((anIntroMode & FSOUND_8BITS) != 0) ? 1 : 2);

	if (gFMod->FSOUND_Sample_Lock(aSample, 0, (anIntroLen + aLoopLen) * aSampleBytes, &aSamplePtr1, &aSamplePtr2, &aSampleLen1, &aSampleLen2))
	{			
		void* aSrcPtr1;
		void* aSrcPtr2;
		uint aSrcLen1;
		uint aSrcLen2;

		if (gFMod->FSOUND_Sample_Lock(anIntroSample, 0, anIntroLen * aSampleBytes, &aSrcPtr1, &aSrcPtr2, &aSrcLen1, &aSrcLen2))
		{
			memcpy(aSamplePtr1, aSrcPtr1, aSrcLen1);
			gFMod->FSOUND_Sample_Unlock(anIntroSample, aSrcPtr1, aSrcPtr2, aSrcLen1, aSrcLen2);
		}

		if (gFMod->FSOUND_Sample_Lock(aLoopSample, anIntroLen * aSampleBytes, aLoopLen * aSampleBytes, &aSrcPtr1, &aSrcPtr2, &aSrcLen1, &aSrcLen2))
		{
			memcpy((char*) aSamplePtr1 + (anIntroLen * aSampleBytes), aSrcPtr1, aSrcLen1);
			gFMod->FSOUND_Sample_Unlock(aLoopSample, aSrcPtr1, aSrcPtr2, aSrcLen1, aSrcLen2);
		}
		
		gFMod->FSOUND_Sample_Unlock(aSample, aSamplePtr1, aSamplePtr2, aSampleLen1, aSampleLen2);
	}

	gFMod->FSOUND_Sample_Free(anIntroSample);
	gFMod->FSOUND_Sample_Free(aLoopSample);

	gFMod->FSOUND_Sample_SetLoopPoints(aSample, anIntroLen, aLoopLen);

	FModMusicInfo aMusicInfo;
	aMusicInfo.mHSample = aSample;
	aMusicInfo.mRepeats = repeat;
	mMusicMap.insert(FModMusicMap::value_type(theSongId, aMusicInfo));

	return true;	
}

bool FModMusicInterface::LoadMusic(int theSongId, const std::string& theFileName)
{
	FMUSIC_MODULE* aHMusic = gFMod->FMUSIC_LoadSong(theFileName.c_str());

	if (aHMusic == NULL)
		return false;	
	
	FModMusicInfo aMusicInfo;
	aMusicInfo.mHMusic = aHMusic;	
	mMusicMap.insert(FModMusicMap::value_type(theSongId, aMusicInfo));

	return true;	
}

void FModMusicInterface::PlayMusic(int theSongId, int theOffset, bool noLoop)
{
	FModMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		FModMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = aMusicInfo->mVolumeCap;
		aMusicInfo->mVolumeAdd = 0.0;

		if (aMusicInfo->mHMusic != NULL)
		{
			gFMod->FMUSIC_SetMasterVolume(aMusicInfo->mHMusic, (int) (mMasterVolume * aMusicInfo->mVolume * mMaxMusicVolume));
			gFMod->FMUSIC_SetLooping(aMusicInfo->mHMusic, noLoop ? FSOUND_LOOP_OFF : FSOUND_LOOP_NORMAL);
			gFMod->FMUSIC_PlaySong(aMusicInfo->mHMusic);
			gFMod->FMUSIC_SetOrder(aMusicInfo->mHMusic, theOffset);		
		}
		else
		{			
			gFMod->FSOUND_PlaySound(anItr->first, aMusicInfo->mHSample);
			gFMod->FSOUND_SetLoopMode(anItr->first, aMusicInfo->mRepeats ? FSOUND_LOOP_NORMAL : FSOUND_LOOP_OFF);
			gFMod->FSOUND_SetVolume(anItr->first, (int) (mMasterVolume * aMusicInfo->mVolume * mMaxSampleVolume));
		}
	}
}

void FModMusicInterface::PauseMusic(int theSongId)
{
	FModMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		FModMusicInfo* aMusicInfo = &anItr->second;
		
		if (aMusicInfo->mHMusic != NULL)
		{
			gFMod->FMUSIC_SetPaused(aMusicInfo->mHMusic, TRUE);
		}
		else
		{
			gFMod->FSOUND_SetPaused(theSongId, TRUE);
		}
	}
}

void FModMusicInterface::ResumeMusic(int theSongId)
{
	FModMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		FModMusicInfo* aMusicInfo = &anItr->second;
		
		if (aMusicInfo->mHMusic != NULL)
		{
			gFMod->FMUSIC_SetPaused(aMusicInfo->mHMusic, FALSE);
		}
		else
		{
			gFMod->FSOUND_SetPaused(theSongId, FALSE);
		}
	}
}

void FModMusicInterface::StopMusic(int theSongId)
{
	FModMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		FModMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = 0.0;

		if (aMusicInfo->mHMusic != NULL)
		{
			gFMod->FMUSIC_StopSong(aMusicInfo->mHMusic);
		}
		else
		{
			gFMod->FSOUND_StopSound(theSongId);
		}
	}
}

void FModMusicInterface::StopAllMusic()
{
	FModMusicMap::iterator anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end())
	{
		FModMusicInfo* aMusicInfo = &anItr->second;
		aMusicInfo->mVolume = 0.0;
		
		if (aMusicInfo->mHMusic != NULL)
		{
			gFMod->FMUSIC_StopSong(aMusicInfo->mHMusic);
		}
		else
		{
			gFMod->FSOUND_StopSound(anItr->first);
		}

		++anItr;
	}
}

void FModMusicInterface::FadeIn(int theSongId, int theOffset, double theSpeed, bool noLoop)
{
	FModMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		FModMusicInfo* aMusicInfo = &anItr->second;
		
		aMusicInfo->mVolumeAdd = theSpeed;
				
		if (aMusicInfo->mHMusic != NULL)
		{	
			gFMod->FMUSIC_SetMasterVolume(aMusicInfo->mHMusic, (int) (mMasterVolume * aMusicInfo->mVolume * mMaxMusicVolume));

			gFMod->FMUSIC_SetLooping(aMusicInfo->mHMusic, noLoop ? FSOUND_LOOP_OFF : FSOUND_LOOP_NORMAL);

			if (!gFMod->FMUSIC_IsPlaying(aMusicInfo->mHMusic))
				gFMod->FMUSIC_PlaySong(aMusicInfo->mHMusic);

			if (theOffset != -1)
				gFMod->FMUSIC_SetOrder(aMusicInfo->mHMusic, theOffset);
		}
		else
		{			
			if (!gFMod->FSOUND_IsPlaying(theSongId))
			{
				gFMod->FSOUND_PlaySound(theSongId, aMusicInfo->mHSample);
				gFMod->FSOUND_SetLoopMode(anItr->first, aMusicInfo->mRepeats ? FSOUND_LOOP_NORMAL : FSOUND_LOOP_OFF);
			}

			gFMod->FSOUND_SetVolume(anItr->first, (int) (mMasterVolume * aMusicInfo->mVolume * mMaxSampleVolume));
		}
	}
}

void FModMusicInterface::FadeOut(int theSongId, bool stopSong, double theSpeed)
{
	FModMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{
		FModMusicInfo* aMusicInfo = &anItr->second;
				
		aMusicInfo->mVolumeAdd = -theSpeed;
		aMusicInfo->mStopOnFade = stopSong;
	}
}

void FModMusicInterface::FadeOutAll(bool stopSong, double theSpeed)
{
	FModMusicMap::iterator anItr = mMusicMap.begin();
	if (anItr != mMusicMap.end())
	{
		FModMusicInfo* aMusicInfo = &anItr->second;
				
		aMusicInfo->mVolumeAdd = -theSpeed;
		aMusicInfo->mStopOnFade = stopSong;

		++anItr;
	}
}

void FModMusicInterface::SetVolume(double theVolume)
{
	mMasterVolume = theVolume;

	FModMusicMap::iterator anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end())
	{
		FModMusicInfo* aMusicInfo = &anItr->second;						

		if (aMusicInfo->mHMusic != NULL)
		{
			gFMod->FMUSIC_SetMasterVolume(aMusicInfo->mHMusic, (int) (mMasterVolume * aMusicInfo->mVolume * mMaxMusicVolume));
		}
		else
		{
			gFMod->FSOUND_SetVolume(anItr->first, (int) (mMasterVolume * aMusicInfo->mVolume * mMaxSampleVolume));
		}		

		++anItr;
	}	
}

void FModMusicInterface::SetSongVolume(int theSongId, double theVolume)
{
	FModMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		FModMusicInfo* aMusicInfo = &anItr->second;

		aMusicInfo->mVolume = theVolume;
		
		if (aMusicInfo->mHMusic != NULL)
		{
			gFMod->FMUSIC_SetMasterVolume(aMusicInfo->mHMusic, (int) (mMasterVolume * aMusicInfo->mVolume * mMaxMusicVolume));
		}
		else
		{
			gFMod->FSOUND_SetVolume(anItr->first, (int) (mMasterVolume * aMusicInfo->mVolume * mMaxSampleVolume));
		}		
	}
}

bool FModMusicInterface::IsPlaying(int theSongId)
{
	FModMusicMap::iterator anItr = mMusicMap.find(theSongId);
	if (anItr != mMusicMap.end())
	{		
		FModMusicInfo* aMusicInfo = &anItr->second;

		return (gFMod->FMUSIC_IsPlaying(aMusicInfo->mHMusic) != 0);
	}
	else
		return false;	
}

void FModMusicInterface::Update()
{
	FModMusicMap::iterator anItr = mMusicMap.begin();
	while (anItr != mMusicMap.end())
	{
		FModMusicInfo* aMusicInfo = &anItr->second;

		if (aMusicInfo->mVolumeAdd != 0.0)
		{
			aMusicInfo->mVolume += aMusicInfo->mVolumeAdd;
			
			if (aMusicInfo->mVolume > aMusicInfo->mVolumeCap)
			{
				aMusicInfo->mVolume = aMusicInfo->mVolumeCap;
				aMusicInfo->mVolumeAdd = 0.0;
			}
			else if (aMusicInfo->mVolume < 0.0)
			{
				aMusicInfo->mVolume = 0.0;
				aMusicInfo->mVolumeAdd = 0.0;

				if (aMusicInfo->mStopOnFade)
				{
					if (aMusicInfo->mHMusic != NULL)
					{
						gFMod->FMUSIC_StopSong(aMusicInfo->mHMusic);
					}
					else
					{
						gFMod->FSOUND_StopSound(anItr->first);
					}
				}
			}

			if (aMusicInfo->mHMusic != NULL)
			{
				gFMod->FMUSIC_SetMasterVolume(aMusicInfo->mHMusic, (int) (mMasterVolume * aMusicInfo->mVolume * mMaxMusicVolume));
			}
			else
			{				
				gFMod->FSOUND_SetVolume(anItr->first, (int) (mMasterVolume * aMusicInfo->mVolume * mMaxSampleVolume));
			}
		}

		++anItr;
	}
}

