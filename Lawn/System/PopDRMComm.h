#ifndef __POPDRMCOMM_H__
#define __POPDRMCOMM_H__

#include "misc/CritSect.h"
using namespace Sexy;

class PopDRMComm
{
public:
	enum
	{
		PopCapDRM_EnableLocking,
		PopCapDRM_EnableTiming,
		PopCapDRM_QueryData,
		PopCapDRM_IPC,
		PopCapDRM_IPC_Response,
		PopCapDRM_ForceLock,
		PopCapDRM_BuyGame,
		PopCapDRM_ShowScreen,
		PopCapDRM_WaitUI,
		PopCapDRM_NumMessages
	};

public:
	HWND					mDrmGameWnd;								//+00
	HWND					mHWnd;										//+0x4
	HANDLE					mUNKNOWN__FileMapping;						//+08 unknown
	LPVOID					mUNKNOWN__MapViewPtr;						//+0C unknown
	bool					mIsWindow;									//+0x10
	HANDLE					mEventHwnd;									//+0x14
	HANDLE					mEventThreadHwnd;							//+0x18
	DWORD					mEventThreadID;								//+0x1C
	DWORD					mCurrentThreadID;							//+0x20
	UINT					mWindowMessage[PopCapDRM_NumMessages];		//+0x24
	CRITICAL_SECTION		mCritSect;									//+0x48
	
public:
	PopDRMComm();  //0x5D5AC0
	~PopDRMComm();  //0x5D5B60

	void					Start();  //0x5D5BC0
	void					EnableLocking();  //0x5D5C80
	void					BuyGame();
	bool					DoIPC();  //0x5D5D60
	bool					QueryData();  //0x5D5CB0
};

#endif
