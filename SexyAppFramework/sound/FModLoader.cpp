#include "FModLoader.h"
#include <stdlib.h>

using namespace Sexy;

FMOD_INSTANCE* Sexy::gFMod = NULL;
static long gFModLoadCount = 0;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CheckFModFunction(unsigned int theFunc, const char *theName)
{
	if (theFunc==0)
	{
		char aBuf[1024];
		sprintf(aBuf,"%s function not found in fmod.dll",theName);
		MessageBoxA(NULL,aBuf,"Error",MB_OK | MB_ICONERROR);
		exit(0);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FMOD_INSTANCE::FMOD_INSTANCE(const char *dllName)
{
    mModule = LoadLibrary(dllName);
    if (!mModule)
		return;

    #define GETPROC(_x, _y)                                                                       \
    {                                                                                             \
		CheckFModFunction(*((uintptr_t *)&_x) = (uintptr_t)GetProcAddress(mModule, _y),#_y);    \
    }

    GETPROC(FSOUND_SetBufferSize, "_FSOUND_SetBufferSize@4");
    GETPROC(FSOUND_SetHWND, "_FSOUND_SetHWND@4");
    GETPROC(FSOUND_Init, "_FSOUND_Init@12");
    GETPROC(FSOUND_Close, "_FSOUND_Close@0");

	GETPROC(FSOUND_Sample_Load, "_FSOUND_Sample_Load@16");
    GETPROC(FSOUND_Sample_Alloc, "_FSOUND_Sample_Alloc@28");
    GETPROC(FSOUND_Sample_Free, "_FSOUND_Sample_Free@4");
    GETPROC(FSOUND_Sample_Lock, "_FSOUND_Sample_Lock@28");
    GETPROC(FSOUND_Sample_Unlock, "_FSOUND_Sample_Unlock@20");
    GETPROC(FSOUND_Sample_SetLoopPoints, "_FSOUND_Sample_SetLoopPoints@12");
    GETPROC(FSOUND_Sample_GetLength, "_FSOUND_Sample_GetLength@4");
    GETPROC(FSOUND_Sample_GetDefaults, "_FSOUND_Sample_GetDefaults@20");
    GETPROC(FSOUND_Sample_GetMode, "_FSOUND_Sample_GetMode@4");

	GETPROC(FSOUND_PlaySound, "_FSOUND_PlaySound@8");
    GETPROC(FSOUND_StopSound, "_FSOUND_StopSound@4");
    GETPROC(FSOUND_SetVolume, "_FSOUND_SetVolume@8");
    GETPROC(FSOUND_SetPaused, "_FSOUND_SetPaused@8");
    GETPROC(FSOUND_SetLoopMode, "_FSOUND_SetLoopMode@8");
    GETPROC(FSOUND_IsPlaying, "_FSOUND_IsPlaying@4");

	GETPROC(FSOUND_Stream_OpenFile, "_FSOUND_Stream_OpenFile@12");
    GETPROC(FSOUND_Stream_Close, "_FSOUND_Stream_Close@4");
    GETPROC(FSOUND_Stream_Play, "_FSOUND_Stream_Play@8");

	GETPROC(FMUSIC_LoadSong, "_FMUSIC_LoadSong@4");
    GETPROC(FMUSIC_PlaySong, "_FMUSIC_PlaySong@4");
    GETPROC(FMUSIC_StopSong, "_FMUSIC_StopSong@4");
    GETPROC(FMUSIC_SetLooping, "_FMUSIC_SetLooping@8");
    GETPROC(FMUSIC_SetOrder, "_FMUSIC_SetOrder@8");
    GETPROC(FMUSIC_SetPaused, "_FMUSIC_SetPaused@8");
    GETPROC(FMUSIC_SetMasterVolume, "_FMUSIC_SetMasterVolume@8");
    GETPROC(FMUSIC_IsPlaying, "_FMUSIC_IsPlaying@4");

///    GETPROC(FSOUND_SetOutput, "_FSOUND_SetOutput@4");
//    GETPROC(FSOUND_SetDriver, "_FSOUND_SetDriver@4");
//    GETPROC(FSOUND_SetMixer, "_FSOUND_SetMixer@4");
//    GETPROC(FSOUND_SetMinHardwareChannels, "_FSOUND_SetMinHardwareChannels@4");
//    GETPROC(FSOUND_SetMaxHardwareChannels, "_FSOUND_SetMaxHardwareChannels@4");
//    GETPROC(FSOUND_SetMemorySystem, "_FSOUND_SetMemorySystem@20");
//    GETPROC(FSOUND_SetSFXMasterVolume, "_FSOUND_SetSFXMasterVolume@4");
//    GETPROC(FSOUND_SetPanSeperation, "_FSOUND_SetPanSeperation@4");
//    GETPROC(FSOUND_SetSpeakerMode, "_FSOUND_SetSpeakerMode@4");
//    GETPROC(FSOUND_GetError, "_FSOUND_GetError@0");
//    GETPROC(FSOUND_GetVersion, "_FSOUND_GetVersion@0");
//    GETPROC(FSOUND_GetOutput, "_FSOUND_GetOutput@0");
//    GETPROC(FSOUND_GetOutputHandle, "_FSOUND_GetOutputHandle@0");
//    GETPROC(FSOUND_GetDriver, "_FSOUND_GetDriver@0");
//    GETPROC(FSOUND_GetMixer, "_FSOUND_GetMixer@0");
//    GETPROC(FSOUND_GetNumDrivers, "_FSOUND_GetNumDrivers@0");
//    GETPROC(FSOUND_GetDriverName, "_FSOUND_GetDriverName@4");
//    GETPROC(FSOUND_GetDriverCaps, "_FSOUND_GetDriverCaps@8");
//    GETPROC(FSOUND_GetOutputRate, "_FSOUND_GetOutputRate@0");
//    GETPROC(FSOUND_GetMaxChannels, "_FSOUND_GetMaxChannels@0");
//    GETPROC(FSOUND_GetMaxSamples, "_FSOUND_GetMaxSamples@0");
//    GETPROC(FSOUND_GetSFXMasterVolume, "_FSOUND_GetSFXMasterVolume@0");
//    GETPROC(FSOUND_GetNumHardwareChannels, "_FSOUND_GetNumHardwareChannels@0");
//    GETPROC(FSOUND_GetChannelsPlaying, "_FSOUND_GetChannelsPlaying@0");
//    GETPROC(FSOUND_GetCPUUsage, "_FSOUND_GetCPUUsage@0");
//    GETPROC(FSOUND_GetMemoryStats, "_FSOUND_GetMemoryStats@8");
//    GETPROC(FSOUND_Sample_Upload, "_FSOUND_Sample_Upload@12");
//    GETPROC(FSOUND_Sample_SetMode, "_FSOUND_Sample_SetMode@8");
//    GETPROC(FSOUND_Sample_SetDefaults, "_FSOUND_Sample_SetDefaults@20");
//    GETPROC(FSOUND_Sample_SetMinMaxDistance, "_FSOUND_Sample_SetMinMaxDistance@12");
//    GETPROC(FSOUND_Sample_SetMaxPlaybacks, "_FSOUND_Sample_SetMaxPlaybacks@8");
//    GETPROC(FSOUND_Sample_Get, "_FSOUND_Sample_Get@4");
//    GETPROC(FSOUND_Sample_GetName, "_FSOUND_Sample_GetName@4");
//    GETPROC(FSOUND_Sample_GetLoopPoints, "_FSOUND_Sample_GetLoopPoints@12");
//    GETPROC(FSOUND_PlaySoundEx, "_FSOUND_PlaySoundEx@16");
//    GETPROC(FSOUND_SetFrequency, "_FSOUND_SetFrequency@8");
//    GETPROC(FSOUND_SetVolumeAbsolute, "_FSOUND_SetVolumeAbsolute@8");
//    GETPROC(FSOUND_SetPan, "_FSOUND_SetPan@8");
//    GETPROC(FSOUND_SetSurround, "_FSOUND_SetSurround@8");
//    GETPROC(FSOUND_SetMute, "_FSOUND_SetMute@8");
//    GETPROC(FSOUND_SetPriority, "_FSOUND_SetPriority@8");
//    GETPROC(FSOUND_SetReserved, "_FSOUND_SetReserved@8");
//    GETPROC(FSOUND_GetFrequency, "_FSOUND_GetFrequency@4");
//    GETPROC(FSOUND_GetVolume, "_FSOUND_GetVolume@4");
//    GETPROC(FSOUND_GetPan, "_FSOUND_GetPan@4");
//    GETPROC(FSOUND_GetSurround, "_FSOUND_GetSurround@4");
//    GETPROC(FSOUND_GetMute, "_FSOUND_GetMute@4");
//    GETPROC(FSOUND_GetPriority, "_FSOUND_GetPriority@4");
//    GETPROC(FSOUND_GetReserved, "_FSOUND_GetReserved@4");
//    GETPROC(FSOUND_GetPaused, "_FSOUND_GetPaused@4");
//    GETPROC(FSOUND_GetLoopMode, "_FSOUND_GetLoopMode@4");
//    GETPROC(FSOUND_GetCurrentPosition, "_FSOUND_GetCurrentPosition@4");
//    GETPROC(FSOUND_SetCurrentPosition, "_FSOUND_SetCurrentPosition@8");
//    GETPROC(FSOUND_GetCurrentSample, "_FSOUND_GetCurrentSample@4");
//    GETPROC(FSOUND_GetCurrentLevels, "_FSOUND_GetCurrentLevels@12");
//    GETPROC(FSOUND_FX_Enable, "_FSOUND_FX_Enable@8");
//    GETPROC(FSOUND_FX_Disable, "_FSOUND_FX_Disable@4");
//    GETPROC(FSOUND_FX_SetChorus, "_FSOUND_FX_SetChorus@32");
//    GETPROC(FSOUND_FX_SetCompressor, "_FSOUND_FX_SetCompressor@28");
//    GETPROC(FSOUND_FX_SetDistortion, "_FSOUND_FX_SetDistortion@24");
//    GETPROC(FSOUND_FX_SetEcho, "_FSOUND_FX_SetEcho@24");
//    GETPROC(FSOUND_FX_SetFlanger, "_FSOUND_FX_SetFlanger@32");
//    GETPROC(FSOUND_FX_SetGargle, "_FSOUND_FX_SetGargle@12");
//    GETPROC(FSOUND_FX_SetI3DL2Reverb, "_FSOUND_FX_SetI3DL2Reverb@52");
//    GETPROC(FSOUND_FX_SetParamEQ, "_FSOUND_FX_SetParamEQ@16");
//    GETPROC(FSOUND_FX_SetWavesReverb, "_FSOUND_FX_SetWavesReverb@20");
//    GETPROC(FSOUND_Update, "_FSOUND_Update@0");
//    GETPROC(FSOUND_3D_SetAttributes, "_FSOUND_3D_SetAttributes@12");
//    GETPROC(FSOUND_3D_GetAttributes, "_FSOUND_3D_GetAttributes@12");
//    GETPROC(FSOUND_3D_Listener_SetCurrent, "_FSOUND_3D_Listener_SetCurrent@8");
//    GETPROC(FSOUND_3D_Listener_SetAttributes, "_FSOUND_3D_Listener_SetAttributes@32");
//    GETPROC(FSOUND_3D_Listener_GetAttributes, "_FSOUND_3D_Listener_GetAttributes@32");
//    GETPROC(FSOUND_3D_SetDopplerFactor, "_FSOUND_3D_SetDopplerFactor@4");
//    GETPROC(FSOUND_3D_SetDistanceFactor, "_FSOUND_3D_SetDistanceFactor@4");
//    GETPROC(FSOUND_3D_SetRolloffFactor, "_FSOUND_3D_SetRolloffFactor@4");
//    GETPROC(FSOUND_Stream_Create, "_FSOUND_Stream_Create@20");
//    GETPROC(FSOUND_Stream_PlayEx, "_FSOUND_Stream_PlayEx@16");
//    GETPROC(FSOUND_Stream_Stop, "_FSOUND_Stream_Stop@4");
//    GETPROC(FSOUND_Stream_SetEndCallback, "_FSOUND_Stream_SetEndCallback@12");
//    GETPROC(FSOUND_Stream_SetSynchCallback, "_FSOUND_Stream_SetSynchCallback@12");
//    GETPROC(FSOUND_Stream_GetSample, "_FSOUND_Stream_GetSample@4");
//    GETPROC(FSOUND_Stream_CreateDSP, "_FSOUND_Stream_CreateDSP@16");
//    GETPROC(FSOUND_Stream_SetBufferSize, "_FSOUND_Stream_SetBufferSize@4");
//    GETPROC(FSOUND_Stream_SetPosition, "_FSOUND_Stream_SetPosition@8");
//    GETPROC(FSOUND_Stream_GetPosition, "_FSOUND_Stream_GetPosition@4");
//    GETPROC(FSOUND_Stream_SetTime, "_FSOUND_Stream_SetTime@8");
//    GETPROC(FSOUND_Stream_GetTime, "_FSOUND_Stream_GetTime@4");
//    GETPROC(FSOUND_Stream_GetLength, "_FSOUND_Stream_GetLength@4");
//    GETPROC(FSOUND_Stream_GetLengthMs, "_FSOUND_Stream_GetLengthMs@4");
//    GETPROC(FSOUND_Stream_SetMode, "_FSOUND_Stream_SetMode@8");
//    GETPROC(FSOUND_Stream_GetMode, "_FSOUND_Stream_GetMode@4");
//    GETPROC(FSOUND_Stream_SetSubStream, "_FSOUND_Stream_SetSubStream@8");
//    GETPROC(FSOUND_Stream_GetNumSubStreams, "_FSOUND_Stream_GetNumSubStreams@4");
//    GETPROC(FSOUND_Stream_SetSubStreamSentence, "_FSOUND_Stream_SetSubStreamSentence@12");
//    GETPROC(FSOUND_Stream_SetLoopPoints, "_FSOUND_Stream_SetLoopPoints@12");
//    GETPROC(FSOUND_Stream_AddSynchPoint, "_FSOUND_Stream_AddSynchPoint@12");
//    GETPROC(FSOUND_Stream_DeleteSynchPoint, "_FSOUND_Stream_DeleteSynchPoint@8");
//    GETPROC(FSOUND_Stream_GetNumSynchPoints, "_FSOUND_Stream_GetNumSynchPoints@4");
//    GETPROC(FSOUND_Stream_GetOpenState, "_FSOUND_Stream_GetOpenState@4");
//    GETPROC(FSOUND_CD_Play, "_FSOUND_CD_Play@8");
//    GETPROC(FSOUND_CD_SetPlayMode, "_FSOUND_CD_SetPlayMode@8");
//    GETPROC(FSOUND_CD_Stop, "_FSOUND_CD_Stop@4");
//    GETPROC(FSOUND_CD_SetPaused, "_FSOUND_CD_SetPaused@8");
//    GETPROC(FSOUND_CD_SetVolume, "_FSOUND_CD_SetVolume@8");
//    GETPROC(FSOUND_CD_SetTrackTime, "_FSOUND_CD_SetTrackTime@8");
//    GETPROC(FSOUND_CD_Eject, "_FSOUND_CD_Eject@4");
//    GETPROC(FSOUND_CD_GetPaused, "_FSOUND_CD_GetPaused@4");
//    GETPROC(FSOUND_CD_GetTrack, "_FSOUND_CD_GetTrack@4");
//    GETPROC(FSOUND_CD_GetNumTracks, "_FSOUND_CD_GetNumTracks@4");
//    GETPROC(FSOUND_CD_GetVolume, "_FSOUND_CD_GetVolume@4");
//    GETPROC(FSOUND_CD_GetTrackLength, "_FSOUND_CD_GetTrackLength@8");
//    GETPROC(FSOUND_CD_GetTrackTime, "_FSOUND_CD_GetTrackTime@4");
//    GETPROC(FSOUND_DSP_Create, "_FSOUND_DSP_Create@12");
//    GETPROC(FSOUND_DSP_Free, "_FSOUND_DSP_Free@4");
//    GETPROC(FSOUND_DSP_SetPriority, "_FSOUND_DSP_SetPriority@8");
//    GETPROC(FSOUND_DSP_GetPriority, "_FSOUND_DSP_GetPriority@4");
//    GETPROC(FSOUND_DSP_SetActive, "_FSOUND_DSP_SetActive@8");
//    GETPROC(FSOUND_DSP_GetActive, "_FSOUND_DSP_GetActive@4");
//    GETPROC(FSOUND_DSP_GetClearUnit, "_FSOUND_DSP_GetClearUnit@0");
//    GETPROC(FSOUND_DSP_GetSFXUnit, "_FSOUND_DSP_GetSFXUnit@0");
//    GETPROC(FSOUND_DSP_GetMusicUnit, "_FSOUND_DSP_GetMusicUnit@0");
//    GETPROC(FSOUND_DSP_GetClipAndCopyUnit, "_FSOUND_DSP_GetClipAndCopyUnit@0");
//    GETPROC(FSOUND_DSP_GetFFTUnit, "_FSOUND_DSP_GetFFTUnit@0");
//    GETPROC(FSOUND_DSP_MixBuffers, "_FSOUND_DSP_MixBuffers@28");
//    GETPROC(FSOUND_DSP_ClearMixBuffer, "_FSOUND_DSP_ClearMixBuffer@0");
//    GETPROC(FSOUND_DSP_GetBufferLength, "_FSOUND_DSP_GetBufferLength@0");
//    GETPROC(FSOUND_DSP_GetBufferLengthTotal, "_FSOUND_DSP_GetBufferLengthTotal@0");
//    GETPROC(FSOUND_DSP_GetSpectrum, "_FSOUND_DSP_GetSpectrum@0");
//    GETPROC(FSOUND_Reverb_SetProperties, "_FSOUND_Reverb_SetProperties@4");
//    GETPROC(FSOUND_Reverb_GetProperties, "_FSOUND_Reverb_GetProperties@4");
//    GETPROC(FSOUND_Reverb_SetChannelProperties, "_FSOUND_Reverb_SetChannelProperties@8");
//    GETPROC(FSOUND_Reverb_GetChannelProperties, "_FSOUND_Reverb_GetChannelProperties@8");
//    GETPROC(FSOUND_Record_SetDriver, "_FSOUND_Record_SetDriver@4");
//    GETPROC(FSOUND_Record_GetNumDrivers, "_FSOUND_Record_GetNumDrivers@0");
//    GETPROC(FSOUND_Record_GetDriverName, "_FSOUND_Record_GetDriverName@4");
//    GETPROC(FSOUND_Record_GetDriver, "_FSOUND_Record_GetDriver@0");
//    GETPROC(FSOUND_Record_StartSample, "_FSOUND_Record_StartSample@8");
//    GETPROC(FSOUND_Record_Stop, "_FSOUND_Record_Stop@0");
//    GETPROC(FSOUND_Record_GetPosition, "_FSOUND_Record_GetPosition@0");
//    GETPROC(FSOUND_File_SetCallbacks, "_FSOUND_File_SetCallbacks@20");
//    GETPROC(FMUSIC_LoadSongEx, "_FMUSIC_LoadSongEx@20");
//    GETPROC(FMUSIC_FreeSong, "_FMUSIC_FreeSong@4");
//    GETPROC(FMUSIC_StopAllSongs, "_FMUSIC_StopAllSongs@0");
//    GETPROC(FMUSIC_SetZxxCallback, "_FMUSIC_SetZxxCallback@8");
//    GETPROC(FMUSIC_SetRowCallback, "_FMUSIC_SetRowCallback@12");
//    GETPROC(FMUSIC_SetOrderCallback, "_FMUSIC_SetOrderCallback@12");
//    GETPROC(FMUSIC_SetInstCallback, "_FMUSIC_SetInstCallback@12");
//    GETPROC(FMUSIC_SetSample, "_FMUSIC_SetSample@12");
//    GETPROC(FMUSIC_SetUserData, "_FMUSIC_SetUserData@8");
//    GETPROC(FMUSIC_OptimizeChannels, "_FMUSIC_OptimizeChannels@12");
//    GETPROC(FMUSIC_SetReverb, "_FMUSIC_SetReverb@4");

//    GETPROC(FMUSIC_SetMasterSpeed, "_FMUSIC_SetMasterSpeed@8");
//    GETPROC(FMUSIC_SetPanSeperation, "_FMUSIC_SetPanSeperation@8");
//    GETPROC(FMUSIC_GetName, "_FMUSIC_GetName@4");
//    GETPROC(FMUSIC_GetType, "_FMUSIC_GetType@4");
//    GETPROC(FMUSIC_GetNumOrders, "_FMUSIC_GetNumOrders@4");
//    GETPROC(FMUSIC_GetNumPatterns, "_FMUSIC_GetNumPatterns@4");
//    GETPROC(FMUSIC_GetNumInstruments, "_FMUSIC_GetNumInstruments@4");
//    GETPROC(FMUSIC_GetNumSamples, "_FMUSIC_GetNumSamples@4");
//    GETPROC(FMUSIC_GetNumChannels, "_FMUSIC_GetNumChannels@4");
//    GETPROC(FMUSIC_GetSample, "_FMUSIC_GetSample@8");
//    GETPROC(FMUSIC_GetPatternLength, "_FMUSIC_GetPatternLength@8");
//    GETPROC(FMUSIC_IsFinished, "_FMUSIC_IsFinished@4");
//    GETPROC(FMUSIC_GetMasterVolume, "_FMUSIC_GetMasterVolume@4");
//    GETPROC(FMUSIC_GetGlobalVolume, "_FMUSIC_GetGlobalVolume@4");
//    GETPROC(FMUSIC_GetOrder, "_FMUSIC_GetOrder@4");
//    GETPROC(FMUSIC_GetPattern, "_FMUSIC_GetPattern@4");
//    GETPROC(FMUSIC_GetSpeed, "_FMUSIC_GetSpeed@4");
//    GETPROC(FMUSIC_GetBPM, "_FMUSIC_GetBPM@4");
//    GETPROC(FMUSIC_GetRow, "_FMUSIC_GetRow@4");
//    GETPROC(FMUSIC_GetPaused, "_FMUSIC_GetPaused@4");
//    GETPROC(FMUSIC_GetTime, "_FMUSIC_GetTime@4");
//    GETPROC(FMUSIC_GetRealChannel, "_FMUSIC_GetRealChannel@8");
//    GETPROC(FMUSIC_GetUserData, "_FMUSIC_GetUserData@4");

#undef GETPROC
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FMOD_INSTANCE::~FMOD_INSTANCE()
{
	if (mModule)
		FreeLibrary(mModule);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Sexy::LoadFModDLL()
{
	InterlockedIncrement(&gFModLoadCount);
	if (gFMod!=NULL)
		return;

	gFMod = new FMOD_INSTANCE("fmod.dll");
	if (gFMod->mModule==NULL)
	{
		MessageBoxA(NULL,"Can't find fmod.dll." ,"Error",MB_OK | MB_ICONERROR);
		exit(0);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void Sexy::FreeFModDLL()
{
	if (gFMod!=NULL)
	{
		if (InterlockedDecrement(&gFModLoadCount) <= 0)
		{
			delete gFMod;
			gFMod = NULL;
		}
	}
}


