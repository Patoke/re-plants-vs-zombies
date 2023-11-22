#ifndef __BASSLOADER_H__
#define __BASSLOADER_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#include "bass.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
namespace Sexy
{

struct BASS_INSTANCE
{
	BASS_INSTANCE(const char *dllName);
	virtual ~BASS_INSTANCE();

	BOOL BASS_MusicSetAmplify(HMUSIC handle, DWORD amp);
	BOOL BASS_MusicPlay(HMUSIC handle);
	BOOL BASS_MusicPlayEx(HMUSIC handle, DWORD pos, int flags, BOOL reset);
	BOOL BASS_ChannelResume(DWORD handle);
	BOOL BASS_StreamPlay(HSTREAM handle, BOOL flush, DWORD flags);



    HMODULE         mModule;
	bool			mVersion2;

	DWORD(WINAPI *BASS_GetVersion)();

	BOOL(WINAPI *BASS_Init)(int device, DWORD freq, DWORD flags, HWND win);
	BOOL(WINAPI *BASS_Init2)(int device, DWORD freq, DWORD flags, HWND win, GUID* clsid);
	void(WINAPI *BASS_Free)();
	BOOL(WINAPI *BASS_Stop)();
	BOOL(WINAPI *BASS_Start)();
	void(WINAPI *BASS_SetGlobalVolumes)(int musvol, int samvol, int strvol);
	BOOL(WINAPI *BASS_SetVolume)(DWORD volume);
	BOOL(WINAPI *BASS_GetVolume)();
	BOOL(WINAPI *BASS_GetInfo)(BASS_INFO* info);

	DWORD(WINAPI *BASS_SetConfig)(DWORD option, DWORD value); 
	DWORD(WINAPI *BASS_GetConfig)(DWORD option); 

	BOOL(WINAPI *BASS_ChannelStop)(DWORD handle);
	BOOL(WINAPI *BASS_ChannelPlay)(DWORD handle, BOOL restart);
	BOOL(WINAPI *BASS_ChannelPause)(DWORD handle);
	BOOL(WINAPI *BASS_ChannelSetAttribute)(DWORD handle, DWORD attrib, float value);
	BOOL(WINAPI *BASS_ChannelGetAttribute)(DWORD handle, DWORD attrib, float *value);
	BOOL(WINAPI *BASS_ChannelSetPosition)(DWORD handle, QWORD pos, DWORD mode);
	QWORD(WINAPI *BASS_ChannelGetPosition)(DWORD handle, DWORD mode);
	DWORD (WINAPI *BASS_ChannelFlags)(DWORD handle, DWORD flags, DWORD mask);
	DWORD(WINAPI *BASS_ChannelIsActive)(DWORD handle);
	//BOOL (WINAPI *BASS_ChannelSlideAttributes)(DWORD handle, int freq, int volume, int pan, DWORD time);
	BOOL (WINAPI *BASS_ChannelSlideAttribute)(DWORD handle, DWORD attrib, float value, DWORD time);
	DWORD (WINAPI *BASS_ChannelIsSliding)(DWORD handle);
	DWORD (WINAPI *BASS_ChannelGetLevel)(DWORD handle);
	HFX (WINAPI *BASS_ChannelSetFX)(DWORD handle, DWORD theType, int priority);
	BOOL (WINAPI *BASS_ChannelRemoveFX)(DWORD handle, HFX fx);
	QWORD (WINAPI *BASS_ChannelGetLength)(DWORD handle);
	DWORD (WINAPI *BASS_ChannelGetData)(DWORD handle, void* buffer, DWORD length);
	BOOL (WINAPI *BASS_ChannelPreBuf)(DWORD handle, DWORD length);
	HSYNC (WINAPI *BASS_ChannelSetSync)(DWORD handle, DWORD theType, QWORD theParam, SYNCPROC* proc, DWORD user);
	BOOL (WINAPI *BASS_ChannelRemoveSync)(DWORD handle, HSYNC sync);
	
	HMUSIC(WINAPI *BASS_MusicLoad)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags);
	HMUSIC(WINAPI *BASS_MusicLoad2)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags, DWORD freq);
	void(WINAPI *BASS_MusicFree)(HMUSIC handle);

	HSTREAM(WINAPI *BASS_StreamCreateFile)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD flags); 
	void(WINAPI *BASS_StreamFree)(HSTREAM handle);

	BOOL (WINAPI *BASS_FXSetParameters)(HFX handle, void *par);
	BOOL (WINAPI *BASS_FXGetParameters)(HFX handle, void *par);

	//DWORD(WINAPI *BASS_MusicGetAttribute)(DWORD handle, DWORD attrib);
	//void(WINAPI *BASS_MusicSetAttribute)(DWORD handle, DWORD attrib,DWORD value);

	//DWORD (WINAPI *BASS_MusicGetOrders)(HMUSIC handle);
	//DWORD (WINAPI *BASS_MusicGetOrderPosition)(HMUSIC handle);

	HPLUGIN (WINAPI *BASS_PluginLoad)(char *file, DWORD flags);

	HSAMPLE (WINAPI *BASS_SampleLoad)(BOOL mem, void *file, DWORD offset, DWORD length, DWORD max, DWORD flags);
	BOOL (WINAPI *BASS_SampleFree)(HSAMPLE handle);
	BOOL (WINAPI *BASS_SampleSetInfo)(HSAMPLE handle, BASS_SAMPLE *info);
	BOOL (WINAPI *BASS_SampleGetInfo)(HSAMPLE handle, BASS_SAMPLE *info);
	HCHANNEL (WINAPI *BASS_SampleGetChannel)(HSAMPLE handle, BOOL onlynew);
	BOOL (WINAPI *BASS_SampleStop)(HSAMPLE handle);	

	int (WINAPI *BASS_ErrorGetCode)();
};


extern BASS_INSTANCE *gBass;

BASS_INSTANCE* BASS_CreateInstance(char *dllName);
void BASS_FreeInstance(BASS_INSTANCE *instance);
void LoadBassDLL(); // exits on failure
void FreeBassDLL();

} // namespace Sexy

#endif