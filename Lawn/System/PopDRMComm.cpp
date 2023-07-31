#include "PopDRMComm.h"

//0x5D5AC0
PopDRMComm::PopDRMComm()
{
#ifdef _ENABLE_DRM_FOR_DEBUGGING
	mEventThreadHwnd = NULL;
	mEventThreadID = NULL;
	mEventHwnd = NULL;
	mDrmGameWnd = NULL;
	mHWnd = NULL;
	mUNKNOWN__FileMapping = NULL;
	mUNKNOWN__MapViewPtr = NULL;
	mCurrentThreadID = GetCurrentThreadId();

	mWindowMessage[PopDRMComm::PopCapDRM_EnableLocking] = RegisterClipboardFormat("PopCapDRM_EnableLocking");
	mWindowMessage[PopDRMComm::PopCapDRM_EnableTiming] = RegisterClipboardFormat("PopCapDRM_EnableTiming");
	mWindowMessage[PopDRMComm::PopCapDRM_QueryData] = RegisterClipboardFormat("PopCapDRM_QueryData");
	mWindowMessage[PopDRMComm::PopCapDRM_IPC] = RegisterClipboardFormat("PopCapDRM_IPC");
	mWindowMessage[PopDRMComm::PopCapDRM_IPC_Response] = RegisterClipboardFormat("PopCapDRM_IPC_Response");
	mWindowMessage[PopDRMComm::PopCapDRM_ForceLock] = RegisterClipboardFormat("PopCapDRM_ForceLock");
	mWindowMessage[PopDRMComm::PopCapDRM_BuyGame] = RegisterClipboardFormat("PopCapDRM_BuyGame");
	mWindowMessage[PopDRMComm::PopCapDRM_ShowScreen] = RegisterClipboardFormat("PopCapDRM_ShowScreen");
	mWindowMessage[PopDRMComm::PopCapDRM_WaitUI] = RegisterClipboardFormat("PopCapDRM_WaitUI");
	mIsWindow = false;

	InitializeCriticalSection(&mCritSect);
	Start();
#endif
}

//0x5D5B60
PopDRMComm::~PopDRMComm()
{
#ifdef _ENABLE_DRM_FOR_DEBUGGING
	if (mEventThreadHwnd)
	{
		PostThreadMessage(mEventThreadID, WM_QUIT, NULL, NULL);
		WaitForSingleObject(mEventThreadHwnd, -1);
		CloseHandle(mEventThreadHwnd);
	}

	if (mUNKNOWN__MapViewPtr)
	{
		UnmapViewOfFile(mUNKNOWN__MapViewPtr);
	}

	if (mUNKNOWN__FileMapping)
	{
		CloseHandle(mUNKNOWN__FileMapping);
	}

	DeleteCriticalSection(&mCritSect);
#endif
}

//0x5D5BC0
void PopDRMComm::Start()
{
	//mEventHwnd = CreateEvent(NULL, NULL, NULL, NULL);




}

//0x5D5C80
void PopDRMComm::EnableLocking()
{
#ifdef _ENABLE_DRM_FOR_DEBUGGING
	if (IsWindow(mHWnd))
	{
		SendMessage(mHWnd, mWindowMessage[PopDRMComm::PopCapDRM_EnableLocking], NULL, NULL);
	}
	else
	{
		mIsWindow = false;
	}
#endif
}

//0x5D5CB0
bool PopDRMComm::QueryData()
{
#ifdef _ENABLE_DRM_FOR_DEBUGGING
	if (IsWindow(mHWnd))
	{
		return SendMessage(mHWnd, mWindowMessage[PopDRMComm::PopCapDRM_QueryData], 4, NULL) != NULL;
	}
	else
	{
		mIsWindow = false;
		return false;
	}
#else
	return true;
#endif
}

//0x5D5CE0
void PopDRMComm::BuyGame()
{
#ifdef _ENABLE_DRM_FOR_DEBUGGING
	if (!IsWindow(mHWnd))
	{
		mIsWindow = false;
		return;
	}

	//sub_5D5C10
	EnterCriticalSection(&mCritSect);
	
	

	SendMessage(mHWnd, mWindowMessage[PopDRMComm::PopCapDRM_BuyGame], 0, 1);
	LeaveCriticalSection(&mCritSect);
#endif
}

//0x5D5D60
bool PopDRMComm::DoIPC()
{
#ifdef _ENABLE_DRM_FOR_DEBUGGING
	if (!mIsWindow)
	{
		mEventHwnd = CreateEvent(NULL, NULL, NULL, NULL);
		PostMessage((HWND)0xFFFF, mWindowMessage[PopDRMComm::PopCapDRM_IPC], GetCurrentProcessId(), (LPARAM)mDrmGameWnd);
		WaitForSingleObject(mEventHwnd, 100);
		CloseHandle(mEventHwnd);
	}
	return mIsWindow;
#else
	return false;
#endif
}
