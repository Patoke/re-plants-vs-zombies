#ifndef __FMODLOADER_H__
#define __FMODLOADER_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#include "fmod.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
namespace Sexy
{

struct FMOD_INSTANCE
{
	FMOD_INSTANCE(const char *dllName);
	virtual ~FMOD_INSTANCE();

    HMODULE         mModule;

	signed char     (F_API *FSOUND_SetBufferSize)(int len_ms);
    signed char     (F_API *FSOUND_SetHWND)(void *hwnd);
    signed char     (F_API *FSOUND_Init)(int mixrate, int maxsoftwarechannels, unsigned int flags);
    void            (F_API *FSOUND_Close)();

	FSOUND_SAMPLE * (F_API *FSOUND_Sample_Load)(int index, const char *name_or_data, unsigned int mode, int memlength);
    FSOUND_SAMPLE * (F_API *FSOUND_Sample_Alloc)(int index, int length, unsigned int mode, int deffreq, int defvol, int defpan, int defpri);
    void            (F_API *FSOUND_Sample_Free)(FSOUND_SAMPLE *sptr);
    signed char     (F_API *FSOUND_Sample_Lock)(FSOUND_SAMPLE *sptr, int offset, int length, void **ptr1, void **ptr2, unsigned int *len1, unsigned int *len2);
    signed char     (F_API *FSOUND_Sample_Unlock)(FSOUND_SAMPLE *sptr, void *ptr1, void *ptr2, unsigned int len1, unsigned int len2);
    signed char     (F_API *FSOUND_Sample_SetLoopPoints)(FSOUND_SAMPLE *sptr, int loopstart, int loopend);
    unsigned int    (F_API *FSOUND_Sample_GetLength)(FSOUND_SAMPLE *sptr);
    signed char     (F_API *FSOUND_Sample_GetDefaults)(FSOUND_SAMPLE *sptr, int *deffreq, int *defvol, int *defpan, int *defpri);
    unsigned int    (F_API *FSOUND_Sample_GetMode)(FSOUND_SAMPLE *sptr);

	int             (F_API *FSOUND_PlaySound)(int channel, FSOUND_SAMPLE *sptr);
    signed char     (F_API *FSOUND_StopSound)(int channel);
    signed char     (F_API *FSOUND_SetVolume)(int channel, int vol);
    signed char     (F_API *FSOUND_SetPaused)(int channel, signed char paused);
    signed char     (F_API *FSOUND_SetLoopMode)(int channel, unsigned int loopmode);
    signed char     (F_API *FSOUND_IsPlaying)(int channel);

    FSOUND_STREAM * (F_API *FSOUND_Stream_OpenFile)(const char *filename, unsigned int mode, int memlength);
	signed char     (F_API *FSOUND_Stream_Close)(FSOUND_STREAM *stream);
    int             (F_API *FSOUND_Stream_Play)(int channel, FSOUND_STREAM *stream);

    FMUSIC_MODULE * (F_API *FMUSIC_LoadSong)(const char *name);
    signed char     (F_API *FMUSIC_PlaySong)(FMUSIC_MODULE *mod);
    signed char     (F_API *FMUSIC_StopSong)(FMUSIC_MODULE *mod);
    signed char     (F_API *FMUSIC_SetLooping)(FMUSIC_MODULE *mod, signed char looping);
    signed char     (F_API *FMUSIC_SetOrder)(FMUSIC_MODULE *mod, int order);
    signed char     (F_API *FMUSIC_SetPaused)(FMUSIC_MODULE *mod, signed char pause);
    signed char     (F_API *FMUSIC_SetMasterVolume)(FMUSIC_MODULE *mod, int volume);
    signed char     (F_API *FMUSIC_IsPlaying)(FMUSIC_MODULE *mod);
/*
    signed char     (F_API *FSOUND_SetOutput)(int outputtype);
    signed char     (F_API *FSOUND_SetDriver)(int driver);
    signed char     (F_API *FSOUND_SetMixer)(int mixer);
    signed char     (F_API *FSOUND_SetMinHardwareChannels)(int min);
    signed char     (F_API *FSOUND_SetMaxHardwareChannels)(int max);
    signed char     (F_API *FSOUND_SetMemorySystem)(void *pool, int poollen, FSOUND_ALLOCCALLBACK useralloc, FSOUND_REALLOCCALLBACK userrealloc, FSOUND_FREECALLBACK userfree);
    void            (F_API *FSOUND_SetSpeakerMode)(unsigned int speakermode);
    void            (F_API *FSOUND_SetSFXMasterVolume)(int volume);
    void            (F_API *FSOUND_SetPanSeperation)(float pansep);
    void            (F_API *FSOUND_File_SetCallbacks)(FSOUND_OPENCALLBACK  useropen, FSOUND_CLOSECALLBACK userclose, FSOUND_READCALLBACK userread, FSOUND_SEEKCALLBACK  userseek, FSOUND_TELLCALLBACK  usertell);
    int             (F_API *FSOUND_GetError)();
    float           (F_API *FSOUND_GetVersion)();
    int             (F_API *FSOUND_GetOutput)();
    void *          (F_API *FSOUND_GetOutputHandle)();
    int             (F_API *FSOUND_GetDriver)();
    int             (F_API *FSOUND_GetMixer)();
    int             (F_API *FSOUND_GetNumDrivers)();
    signed char *   (F_API *FSOUND_GetDriverName)(int id);
    signed char     (F_API *FSOUND_GetDriverCaps)(int id, unsigned int *caps);
    int             (F_API *FSOUND_GetOutputRate)();
    int             (F_API *FSOUND_GetMaxChannels)();
    int             (F_API *FSOUND_GetMaxSamples)();
    int             (F_API *FSOUND_GetSFXMasterVolume)();
    int             (F_API *FSOUND_GetNumHardwareChannels)();
    int             (F_API *FSOUND_GetChannelsPlaying)();
    float           (F_API *FSOUND_GetCPUUsage)();
    void            (F_API *FSOUND_GetMemoryStats)(unsigned int *currentalloced, unsigned int *maxalloced);
    signed char     (F_API *FSOUND_Sample_Upload)(FSOUND_SAMPLE *sptr, void *srcdata, unsigned int mode);
    signed char     (F_API *FSOUND_Sample_SetMode)(FSOUND_SAMPLE *sptr, unsigned int mode);
    signed char     (F_API *FSOUND_Sample_SetDefaults)(FSOUND_SAMPLE *sptr, int deffreq, int defvol, int defpan, int defpri);
    signed char     (F_API *FSOUND_Sample_SetMinMaxDistance)(FSOUND_SAMPLE *sptr, float min, float max);
    signed char     (F_API *FSOUND_Sample_SetMaxPlaybacks)(FSOUND_SAMPLE *sptr, int max);
    FSOUND_SAMPLE * (F_API *FSOUND_Sample_Get)(int sampno);
    char *          (F_API *FSOUND_Sample_GetName)(FSOUND_SAMPLE *sptr);
    signed char     (F_API *FSOUND_Sample_GetLoopPoints)(FSOUND_SAMPLE *sptr, int *loopstart, int *loopend);
    int             (F_API *FSOUND_PlaySoundEx)(int channel, FSOUND_SAMPLE *sptr, FSOUND_DSPUNIT *dsp, signed char startpaused);
    signed char     (F_API *FSOUND_SetFrequency)(int channel, int freq);
    signed char     (F_API *FSOUND_SetVolumeAbsolute)(int channel, int vol);
    signed char     (F_API *FSOUND_SetPan)(int channel, int pan);
    signed char     (F_API *FSOUND_SetSurround)(int channel, signed char surround);
    signed char     (F_API *FSOUND_SetMute)(int channel, signed char mute);
    signed char     (F_API *FSOUND_SetPriority)(int channel, int priority);
    signed char     (F_API *FSOUND_SetReserved)(int channel, signed char reserved);
    signed char     (F_API *FSOUND_SetCurrentPosition)(int channel, unsigned int offset);
    int             (F_API *FSOUND_GetFrequency)(int channel);
    int             (F_API *FSOUND_GetVolume)(int channel);
    int             (F_API *FSOUND_GetPan)(int channel);
    signed char     (F_API *FSOUND_GetSurround)(int channel);
    signed char     (F_API *FSOUND_GetMute)(int channel);
    int             (F_API *FSOUND_GetPriority)(int channel);
    signed char     (F_API *FSOUND_GetReserved)(int channel);
    signed char     (F_API *FSOUND_GetPaused)(int channel);
    unsigned int    (F_API *FSOUND_GetLoopMode)(int channel);
    unsigned int    (F_API *FSOUND_GetCurrentPosition)(int channel);
    FSOUND_SAMPLE * (F_API *FSOUND_GetCurrentSample)(int channel);
    signed char     (F_API *FSOUND_GetCurrentLevels)(int channel, float *l, float *r);
    int             (F_API *FSOUND_FX_Enable)(int channel, unsigned int fx);    // See FSOUND_FX_MODES 
    signed char     (F_API *FSOUND_FX_Disable)(int channel);    
    signed char     (F_API *FSOUND_FX_SetChorus)(int fxid, float WetDryMix, float Depth, float Feedback, float Frequency, int Waveform, float Delay, int Phase);
    signed char     (F_API *FSOUND_FX_SetCompressor)(int fxid, float Gain, float Attack, float Release, float Threshold, float Ratio, float Predelay);
    signed char     (F_API *FSOUND_FX_SetDistortion)(int fxid, float Gain, float Edge, float PostEQCenterFrequency, float PostEQBandwidth, float PreLowpassCutoff);
    signed char     (F_API *FSOUND_FX_SetEcho)(int fxid, float WetDryMix, float Feedback, float LeftDelay, float RightDelay, int PanDelay);
    signed char     (F_API *FSOUND_FX_SetFlanger)(int fxid, float WetDryMix, float Depth, float Feedback, float Frequency, int Waveform, float Delay, int Phase);
    signed char     (F_API *FSOUND_FX_SetGargle)(int fxid, int RateHz, int WaveShape);
    signed char     (F_API *FSOUND_FX_SetI3DL2Reverb)(int fxid, int Room, int RoomHF, float RoomRolloffFactor, float DecayTime, float DecayHFRatio, int Reflections, float ReflectionsDelay, int Reverb, float ReverbDelay, float Diffusion, float Density, float HFReference);
    signed char     (F_API *FSOUND_FX_SetParamEQ)(int fxid, float Center, float Bandwidth, float Gain);
    signed char     (F_API *FSOUND_FX_SetWavesReverb)(int fxid, float InGain, float ReverbMix, float ReverbTime, float HighFreqRTRatio);  
    void            (F_API *FSOUND_Update)();   // you must call this once a frame 
    void            (F_API *FSOUND_3D_SetDopplerFactor)(float scale);
    void            (F_API *FSOUND_3D_SetDistanceFactor)(float scale);
    void            (F_API *FSOUND_3D_SetRolloffFactor)(float scale);
    signed char     (F_API *FSOUND_3D_SetAttributes)(int channel, float *pos, float *vel);
    signed char     (F_API *FSOUND_3D_GetAttributes)(int channel, float *pos, float *vel);
    void            (F_API *FSOUND_3D_Listener_SetCurrent)(int current, int numlisteners);  // use this if you use multiple listeners / splitscreen
    void            (F_API *FSOUND_3D_Listener_SetAttributes)(float *pos, float *vel, float fx, float fy, float fz, float tx, float ty, float tz);
    void            (F_API *FSOUND_3D_Listener_GetAttributes)(float *pos, float *vel, float *fx, float *fy, float *fz, float *tx, float *ty, float *tz);
    signed char     (F_API *FSOUND_Stream_SetBufferSize)(int ms);      // call this before opening streams, not after 
    FSOUND_STREAM * (F_API *FSOUND_Stream_Create)(FSOUND_STREAMCALLBACK callback, int length, unsigned int mode, int samplerate, int userdata);
    int             (F_API *FSOUND_Stream_PlayEx)(int channel, FSOUND_STREAM *stream, FSOUND_DSPUNIT *dsp, signed char startpaused);
    signed char     (F_API *FSOUND_Stream_Stop)(FSOUND_STREAM *stream);
    signed char     (F_API *FSOUND_Stream_SetEndCallback)(FSOUND_STREAM *stream, FSOUND_STREAMCALLBACK callback, int userdata);
    signed char     (F_API *FSOUND_Stream_SetSynchCallback)(FSOUND_STREAM *stream, FSOUND_STREAMCALLBACK callback, int userdata);
    FSOUND_SAMPLE * (F_API *FSOUND_Stream_GetSample)(FSOUND_STREAM *stream);   // every stream contains a sample to playback on 
    FSOUND_DSPUNIT *(F_API *FSOUND_Stream_CreateDSP)(FSOUND_STREAM *stream, FSOUND_DSPCALLBACK callback, int priority, int param);
    signed char     (F_API *FSOUND_Stream_SetPosition)(FSOUND_STREAM *stream, unsigned int position);
    unsigned int    (F_API *FSOUND_Stream_GetPosition)(FSOUND_STREAM *stream);
    signed char     (F_API *FSOUND_Stream_SetTime)(FSOUND_STREAM *stream, int ms);
    int             (F_API *FSOUND_Stream_GetTime)(FSOUND_STREAM *stream);
    int             (F_API *FSOUND_Stream_GetLength)(FSOUND_STREAM *stream);
    int             (F_API *FSOUND_Stream_GetLengthMs)(FSOUND_STREAM *stream);
    signed char     (F_API *FSOUND_Stream_SetSubStream)(FSOUND_STREAM *stream, int index);
    int             (F_API *FSOUND_Stream_GetNumSubStreams)(FSOUND_STREAM *stream);
    signed char     (F_API *FSOUND_Stream_SetSubStreamSentence)(FSOUND_STREAM *stream, int *sentencelist, int numitems);
    signed char     (F_API *FSOUND_Stream_SetMode)(FSOUND_STREAM *stream, unsigned int mode);
    unsigned int    (F_API *FSOUND_Stream_GetMode)(FSOUND_STREAM *stream);
    signed char     (F_API *FSOUND_Stream_SetLoopPoints)(FSOUND_STREAM *stream, unsigned int loopstartpcm, unsigned int loopendpcm);
    int             (F_API *FSOUND_Stream_AddSynchPoint)(FSOUND_STREAM *stream, unsigned int pcmoffset, int userdata);
    signed char     (F_API *FSOUND_Stream_DeleteSynchPoint)(FSOUND_STREAM *stream, int index);
    int             (F_API *FSOUND_Stream_GetNumSynchPoints)(FSOUND_STREAM *stream);
    int             (F_API *FSOUND_Stream_GetOpenState)(FSOUND_STREAM *stream);
    signed char     (F_API *FSOUND_CD_Play)(char drive, int track);
    void            (F_API *FSOUND_CD_SetPlayMode)(char drive, signed char mode);
    signed char     (F_API *FSOUND_CD_Stop)(char drive);
    signed char     (F_API *FSOUND_CD_SetTrackTime)(char drive, unsigned int ms);
    signed char     (F_API *FSOUND_CD_Eject)(char drive);
    signed char     (F_API *FSOUND_CD_GetPaused)(char drive);
    int             (F_API *FSOUND_CD_GetTrack)(char drive);
    int             (F_API *FSOUND_CD_GetNumTracks)(char drive);
    int             (F_API *FSOUND_CD_GetVolume)(char drive);
    int             (F_API *FSOUND_CD_GetTrackLength)(char drive, int track); 
    int             (F_API *FSOUND_CD_GetTrackTime)(char drive);
    FSOUND_DSPUNIT *(F_API *FSOUND_DSP_Create)(FSOUND_DSPCALLBACK callback, int priority, int param);
    void            (F_API *FSOUND_DSP_Free)(FSOUND_DSPUNIT *unit);
    void            (F_API *FSOUND_DSP_SetPriority)(FSOUND_DSPUNIT *unit, int priority);
    int             (F_API *FSOUND_DSP_GetPriority)(FSOUND_DSPUNIT *unit);
    void            (F_API *FSOUND_DSP_SetActive)(FSOUND_DSPUNIT *unit, signed char active);
    signed char     (F_API *FSOUND_DSP_GetActive)(FSOUND_DSPUNIT *unit);
    FSOUND_DSPUNIT *(F_API *FSOUND_DSP_GetClearUnit)();
    FSOUND_DSPUNIT *(F_API *FSOUND_DSP_GetSFXUnit)();
    FSOUND_DSPUNIT *(F_API *FSOUND_DSP_GetMusicUnit)();
    FSOUND_DSPUNIT *(F_API *FSOUND_DSP_GetFFTUnit)();
    FSOUND_DSPUNIT *(F_API *FSOUND_DSP_GetClipAndCopyUnit)();
    signed char     (F_API *FSOUND_DSP_MixBuffers)(void *destbuffer, void *srcbuffer, int len, int freq, int vol, int pan, unsigned int mode);
    void            (F_API *FSOUND_DSP_ClearMixBuffer)();
    int             (F_API *FSOUND_DSP_GetBufferLength)();      // Length of each DSP update 
    int             (F_API *FSOUND_DSP_GetBufferLengthTotal)(); // Total buffer length due to FSOUND_SetBufferSize 
    float *         (F_API *FSOUND_DSP_GetSpectrum)();          // Array of 512 floats - call FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), TRUE)) for this to work. 
    signed char     (F_API *FSOUND_Reverb_SetProperties)(FSOUND_REVERB_PROPERTIES *prop);
    signed char     (F_API *FSOUND_Reverb_GetProperties)(FSOUND_REVERB_PROPERTIES *prop);
    signed char     (F_API *FSOUND_Reverb_SetChannelProperties)(int channel, FSOUND_REVERB_CHANNELPROPERTIES *prop);
    signed char     (F_API *FSOUND_Reverb_GetChannelProperties)(int channel, FSOUND_REVERB_CHANNELPROPERTIES *prop);
    signed char     (F_API *FSOUND_Record_SetDriver)(int outputtype);
    int             (F_API *FSOUND_Record_GetNumDrivers)();
    signed char *   (F_API *FSOUND_Record_GetDriverName)(int id);
    int             (F_API *FSOUND_Record_GetDriver)();
    signed char     (F_API *FSOUND_Record_StartSample)(FSOUND_SAMPLE *sptr, signed char loop);
    signed char     (F_API *FSOUND_Record_Stop)();
    int             (F_API *FSOUND_Record_GetPosition)();  
    FMUSIC_MODULE * (F_API *FMUSIC_LoadSongEx)(void *data, int length);
    signed char     (F_API *FMUSIC_FreeSong)(FMUSIC_MODULE *mod);
    void            (F_API *FMUSIC_StopAllSongs)();
    signed char     (F_API *FMUSIC_SetZxxCallback)(FMUSIC_MODULE *mod, FMUSIC_CALLBACK callback);
    signed char     (F_API *FMUSIC_SetRowCallback)(FMUSIC_MODULE *mod, FMUSIC_CALLBACK callback, int rowstep);
    signed char     (F_API *FMUSIC_SetOrderCallback)(FMUSIC_MODULE *mod, FMUSIC_CALLBACK callback, int orderstep);
    signed char     (F_API *FMUSIC_SetInstCallback)(FMUSIC_MODULE *mod, FMUSIC_CALLBACK callback, int instrument);
    signed char     (F_API *FMUSIC_SetSample)(FMUSIC_MODULE *mod, int sampno, FSOUND_SAMPLE *sptr);
    signed char     (F_API *FMUSIC_SetUserData)(FMUSIC_MODULE *mod, unsigned int userdata);
    signed char     (F_API *FMUSIC_OptimizeChannels)(FMUSIC_MODULE *mod, int maxchannels, int minvolume);
    signed char     (F_API *FMUSIC_SetReverb)(signed char reverb);             // MIDI only 
    signed char     (F_API *FMUSIC_SetMasterSpeed)(FMUSIC_MODULE *mode, float speed);
    signed char     (F_API *FMUSIC_SetPanSeperation)(FMUSIC_MODULE *mod, float pansep);
    char *          (F_API *FMUSIC_GetName)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetType)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetNumOrders)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetNumPatterns)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetNumInstruments)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetNumSamples)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetNumChannels)(FMUSIC_MODULE *mod);
    FSOUND_SAMPLE * (F_API *FMUSIC_GetSample)(FMUSIC_MODULE *mod, int sampno);
    int             (F_API *FMUSIC_GetPatternLength)(FMUSIC_MODULE *mod, int orderno);
    signed char     (F_API *FMUSIC_IsFinished)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetMasterVolume)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetGlobalVolume)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetOrder)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetPattern)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetSpeed)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetBPM)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetRow)(FMUSIC_MODULE *mod);
    signed char     (F_API *FMUSIC_GetPaused)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetTime)(FMUSIC_MODULE *mod);
    int             (F_API *FMUSIC_GetRealChannel)(FMUSIC_MODULE *mod, int modchannel);
    unsigned int    (F_API *FMUSIC_GetUserData)(FMUSIC_MODULE *mod);*/
} ;


extern FMOD_INSTANCE *gFMod;

void LoadFModDLL(); // exits on failure
void FreeFModDLL();

} // namespace Sexy

#endif