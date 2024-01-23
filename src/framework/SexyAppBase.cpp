#include "SexyAppBase.h"

#include "Common.h"

#include "graphics/Color.h"
#include "graphics/VkImage.h"
#include "graphics/WindowInterface.h"

#include "misc/RegistryEmulator.h"
#include "sound/BassSoundManager.h"
#include "sound/DummySoundManager.h"
#include "sound/SoundManager.h"

#include "widget/WidgetManager.h"
#include "widget/Widget.h"
#include "widget/Dialog.h"

#include "misc/Debug.h"
#include "misc/KeyCodes.h"
#include "misc/Rect.h"
#include "misc/PropertiesParser.h"
#include "misc/PerfTimer.h"
#include "misc/MTRand.h"
#include "misc/ResourceManager.h"
#include "misc/Debug.h"

#include "sound/BassMusicInterface.h"
#include "sound/DummyMusicInterface.h"

#include "paklib/PakInterface.h"

#include <bits/chrono.h>
#include <bits/iterator_concepts.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <thread>
#include <time.h>
#include <math.h>

#include <string>
#include <chrono>
#include <utility>

// For TodResourceManager, since inheratance goes the other way lol.
#include "todlib/TodCommon.h"

using namespace Sexy;

const int DEMO_FILE_ID = 0x42BEEF78;
const int DEMO_VERSION = 2;

SexyAppBase* Sexy::gSexyAppBase = NULL;

static bool gScreenSaverActive = false;

#ifndef SPI_GETSCREENSAVERRUNNING
#define SPI_GETSCREENSAVERRUNNING 114
#endif

//HotSpot: 11 4
//Size: 32 32
unsigned char gFingerCursorData[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xc3, 
	0xff, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff, 
	0xc0, 0x07, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xfc, 0x40, 0x01, 0xff, 0xfc, 0x00, 0x01, 0xff, 
	0xfc, 0x00, 0x01, 0xff, 0xfc, 0x00, 0x01, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0x00, 0x01, 
	0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0, 
	0x03, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 
	0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 
	0x18, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x1b, 0x60, 0x00, 0x00, 0x1b, 0x68, 0x00, 
	0x00, 0x1b, 0x6c, 0x00, 0x01, 0x9f, 0xec, 0x00, 0x01, 0xdf, 0xfc, 0x00, 0x00, 0xdf, 0xfc, 
	0x00, 0x00, 0x5f, 0xfc, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 
	0xf8, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 
	0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00
};

//HotSpot: 15 10
//Size: 32 32
unsigned char gDraggingCursorData[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xe0, 
	0x01, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff, 0xe0, 0x00, 0xff, 0xff, 
	0xe0, 0x00, 0xff, 0xfe, 0x60, 0x00, 0xff, 0xfc, 0x20, 0x00, 0xff, 0xfc, 0x00, 0x00, 0xff, 
	0xfe, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x80, 0x00, 
	0xff, 0xff, 0x80, 0x01, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xe0, 0x01, 0xff, 0xff, 0xf0, 
	0x03, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 
	0x80, 0x00, 0x00, 0x01, 0xb0, 0x00, 0x00, 0x0d, 0xb0, 0x00, 0x00, 0x0d, 0xb6, 0x00, 0x00, 
	0x0d, 0xb6, 0x00, 0x00, 0x0d, 0xb6, 0x00, 0x00, 0x0d, 0xb6, 0x00, 0x00, 0x0d, 0xb6, 0x00, 
	0x01, 0x8d, 0xb6, 0x00, 0x01, 0xcf, 0xfe, 0x00, 0x00, 0xef, 0xfe, 0x00, 0x00, 0xff, 0xfe, 
	0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x1f, 
	0xfc, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 
	0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00
};
//static DDImage* gFPSImage = NULL; 

//////////////////////////////////////////////////////////////////////////

/*
typedef HRESULT (WINAPI *SHGetFolderPathFunc)(HWND, int, HANDLE, DWORD, LPTSTR);
void* GetSHGetFolderPath(const char* theDLL, HMODULE* theMod)
{
	HMODULE aMod = LoadLibrary(theDLL);
	SHGetFolderPathFunc aFunc = NULL;

	if (aMod != NULL)
	{
		*((void**)&aFunc) = (void*)GetProcAddress(aMod, "SHGetFolderPathA");
		if (aFunc == NULL)
		{
			FreeLibrary(aMod);
			aMod = NULL;
		}
	}	

	*theMod = aMod;
	return (void *)aFunc;
}*/

//////////////////////////////////////////////////////////////////////////

SexyAppBase::SexyAppBase()
{
	gSexyAppBase = this;

	//gVersionDLL = LoadLibraryA("version.dll");
	//gDDrawDLL = LoadLibraryA("ddraw.dll");
	//gDSoundDLL = LoadLibraryA("dsound.dll");
	//gGetLastInputInfoFunc = (GetLastInputInfoFunc) GetProcAddress(GetModuleHandleA("user32.dll"),"GetLastInputInfo");

	//ImageLib::InitJPEG2000();

//	mMutex = NULL;
	mNotifyGameMessage = 0;

#ifdef _DEBUG
	mOnlyAllowOneCopyToRun = false;
#else
	mOnlyAllowOneCopyToRun = true;
#endif

	//unreachable();
	// TODO
	// Extract product version
	//char aPath[_MAX_PATH];
	//GetModuleFileNameA(NULL, aPath, 256);
	//mProductVersion = GetProductVersion(aPath);	
	//mChangeDirTo = GetFileDir(aPath);


	mNoDefer = false;	
	mFullScreenPageFlip = true; // should we page flip in fullscreen?
	mTimeLoaded = std::chrono::high_resolution_clock::now();
	mProdName = "Product";
	mTitle = _S("SexyApp");
	mShutdown = false;
	mExitToTop = false;
	mWidth = 640;
	mHeight = 480;
	mFullscreenBits = 16;
	mIsWindowed = true;
	mIsPhysWindowed = true;
	mFullScreenWindow = false;
	mPreferredX = -1;
	mPreferredY = -1;
	mIsScreenSaver = false;
	mAllowMonitorPowersave = true;
	//mHWnd = NULL;
//	mDDInterface = NULL;	
	mMusicInterface = NULL;
	//mInvisHWnd = NULL;
	mFrameTime = std::chrono::milliseconds(10);
//	mNonDrawCount = 0;
	mDrawCount = 0;
	mSleepCount = 0;
	mUpdateCount = 0;	
	mUpdateAppState = 0;
	mUpdateAppDepth = 0;
	mPendingUpdatesAcc = 0;
//	mUpdateFTimeAcc = std::chrono::high_resolution_clock::duration(0);
//	mHasPendingDraw = true;
	mIsDrawing = false;
	mLastDrawWasEmpty = false;	
//	mLastTimeCheck = std::chrono::high_resolution_clock::now();
//	mUpdateMultiplier = 1;
	mPaused = false;
	mFastForwardToUpdateNum = 0;
	mFastForwardToMarker = false;
	mFastForwardStep = false;
	mSoundManager = NULL;
	mCursorNum = CURSOR_POINTER;
	mMouseIn = false;
	mRunning = false;
	mActive = true;
	mProcessInTimer = false;
	mMinimized = false;	
	mPhysMinimized = false;
	mIsDisabled = false;
	mLoaded = false;	
	mYieldMainThread = false; 
	mLoadingFailed = false;	
	mLoadingThreadStarted = false;
	mAutoStartLoadingThread = true;
	mLoadingThreadCompleted = false;
	//mCursorThreadRunning = false;
	mNumLoadingThreadTasks = 0;
	mCompletedLoadingThreadTasks = 0;	
	mLastDrawTick = std::chrono::high_resolution_clock::now();
	mNextDrawTick = std::chrono::high_resolution_clock::now();
	mSysCursor = true;
	mForceFullscreen = false;
	mForceWindowed = false;
	mHasFocus = true;			
	mCustomCursorsEnabled = false;	
	mCustomCursorDirty = false;
	//mOverrideCursor = NULL;
	mIsOpeningURL = false;		
	mInitialized = false;	
	mLastShutdownWasGraceful = true;	
	mReadFromRegistry = false;	
	mCmdLineParsed = false;
	mSkipSignatureChecks = false;	
	mCtrlDown = false;
	mAltDown = false;
	mAllowAltEnter = true;
	mStepMode = 0;
	mCleanupSharedImages = false;
	mStandardWordWrap = true;
	mbAllowExtendedChars = true;
	mEnableMaximizeButton = false;
	mWriteToSexyCache = true;
	mSexyCacheBuffers = false;

	mMusicVolume = 0.85;
	mSfxVolume = 0.85;
	mDemoMusicVolume = mDemoSfxVolume = 0.0;
	mMuteCount = 0;	
	mAutoMuteCount = 0;
	mDemoMute = false;
	mMuteOnLostFocus = true;
	mCurHandleNum = 0;
	mFPSTime = std::chrono::milliseconds(0);
	mFPSStartTick = std::chrono::high_resolution_clock::now();
	mFPSFlipCount = 0;
	mFPSCount = 0;
	mFPSDirtyCount = 0;
	mShowFPS = false;
	mShowFPSMode = FPS_ShowFPS;
	mDrawTime = std::chrono::milliseconds(0);
	mScreenBltTime = std::chrono::milliseconds(0);
	mAlphaDisabled = false;	
	mDebugKeysEnabled = false;
	mOldWndProc = 0;
	mNoSoundNeeded = false;
	mWantFMod = false;

	mSyncRefreshRate = 100;
	mWaitForVSync = false;
	mSoftVSyncWait = true;
	mUserChanged3DSetting = false;
	mAutoEnable3D = false;
	mTest3D = false;
	mMinVidMemory3D = 6;
	mRecommendedVidMemory3D = 14;
	mRelaxUpdateBacklogCount = 0;
	mWidescreenAware = false;
	mEnableWindowAspect = false;
	mWindowAspect.Set(4, 3);
	mIsWideWindow = false;

	int i;

	/*
	for (i = 0; i < NUM_CURSORS; i++)
		mCursorImages[i] = NULL;*/	

	for (i = 0; i < 256; i++)
		mAdd8BitMaxTable[i] = i;

	for (i = 256; i < 512; i++)
		mAdd8BitMaxTable[i] = 255;
	
	// Set default strings.  Init could read in overrides from partner.xml
	SetString("DIALOG_BUTTON_OK",		L"OK");
	SetString("DIALOG_BUTTON_CANCEL",	L"CANCEL");

	SetString("UPDATE_CHECK_TITLE",		L"Update Check");
	SetString("UPDATE_CHECK_BODY",		L"Checking if there are any updates available for this product ...");

	SetString("UP_TO_DATE_TITLE",		L"Up to Date");	
	SetString("UP_TO_DATE_BODY",		L"There are no updates available for this product at this time.");
	SetString("NEW_VERSION_TITLE",		L"New Version");
	SetString("NEW_VERSION_BODY",		L"There is an update available for this product.  Would you like to visit the web site to download it?");

	mDemoPrefix = "sexyapp";
	mDemoFileName = mDemoPrefix + ".dmo";
	mPlayingDemoBuffer = false;
	mManualShutdown = false;
	mRecordingDemoBuffer = false;
	mLastDemoMouseX = 0;
	mLastDemoMouseY = 0;
	mLastDemoUpdateCnt = 0;
	mDemoNeedsCommand = true;
	mDemoLoadingComplete = false;
	mDemoLength = 0;
	mDemoCmdNum = 0;
	mDemoCmdOrder = -1; // Means we haven't processed any demo commands yet
	mDemoCmdBitPos = 0;

	mWidgetManager = new WidgetManager(this);
	mResourceManager = new TodResourceManager(this);

	//mPrimaryThreadId = 0;
	/*
	if (GetSystemMetrics(86)) // check for tablet pc
	{
		mTabletPC = true;
		mFullScreenPageFlip = false; // so that tablet keyboard can show up
	}
	else*/
	mTabletPC = false;	

	//gSEHCatcher.mApp = this;	
	
	//std::wifstream stringsFile(_wfopen(L".\\properties\\fstrings", L"rb"));
	//
	//if(!stringsFile)
	//{
	//	MessageBox(NULL, "file missing: 'install-folder\\properties\\fstrings' Please re-install", "FATAL ERROR", MB_OK);
	//	DoExit(1);
	//}
	//std::getline(stringsFile, mString_HardwareAccelSwitchedOn);
	//std::getline(stringsFile, mString_HardwareAccelConfirm);
	//std::getline(stringsFile, mString_HardwareAccelNotWorking);
	//std::getline(stringsFile, mString_SetColorDepth);
	//std::getline(stringsFile, mString_FailedInitDirectDrawColon);
	//std::getline(stringsFile, mString_UnableOpenProperties);
	//std::getline(stringsFile, mString_SigCheckFailed);
	//std::getline(stringsFile, mString_InvalidCommandLineParam);
	//std::getline(stringsFile, mString_RequiresDirectX);
	//std::getline(stringsFile, mString_YouNeedDirectX);
	//std::getline(stringsFile, mString_FailedInitDirectDraw);
	//std::getline(stringsFile, mString_FatalError);
	//std::getline(stringsFile, mString_UnexpectedErrorOccured);
	//std::getline(stringsFile, mString_PleaseHelpBy);
	//std::getline(stringsFile, mString_FailedConnectPopcap);
	//stringsFile.close();
}

SexyAppBase::~SexyAppBase()
{
	Shutdown();

	/*
	if (!showedMsgBox && gD3DInterfacePreDrawError && !IsScreenSaver())
	{
		printf("%s\n", 
			GetString("HARDWARE_ACCEL_NOT_WORKING", 
					_S("Hardware Acceleration may not have been working correctly during this session.\r\n")
					_S("If you noticed graphics problems, you may want to turn off Hardware Acceleration.\r\n")
					_S("Would you like to keep Hardware Acceleration switched on?")
					_S("Too bad bitch I took out the message box feature it's staying on")).c_str());

		//if (aResult==IDNO)
		//	RegistryWriteBoolean("Is3D", false);
	}*/


	DialogMap::iterator aDialogItr = mDialogMap.begin();
	while (aDialogItr != mDialogMap.end())
	{
		mWidgetManager->RemoveWidget(aDialogItr->second);
		delete aDialogItr->second;
		++aDialogItr;
	}
	mDialogMap.clear();
	mDialogList.clear();
	
	/*
	if (mInvisHWnd != NULL)
	{
		HWND aWindow = mInvisHWnd;
		mInvisHWnd = NULL;
		SetWindowLongPtr(aWindow, GWLP_USERDATA, 0);
		DestroyWindow(aWindow);
	}*/
	
	delete mWidgetManager;	
	delete mResourceManager;
	
	/* The shared image map contains unique_ptr, so it should destruct these automatically.
	SharedImageMap::iterator aSharedImageItr = mSharedImageMap.begin();
	while (aSharedImageItr != mSharedImageMap.end())
	{
		SharedImage* aSharedImage = &aSharedImageItr->second;
		DBG_ASSERTE(aSharedImage->mRefCount == 0);
		unreachable(); // FIXME	
		//delete aSharedImage->mImage;
		mSharedImageMap.erase(aSharedImageItr++);		
	}*/
	
	//delete mDDInterface;
	delete mWindowInterface;
	delete mMusicInterface;
	delete mSoundManager;			

	/* FIXME
	if (mHWnd != NULL)
	{
		HWND aWindow = mHWnd;
		mHWnd = NULL;
		
		SetWindowLongPtr(aWindow, GWLP_USERDATA, 0);

		//char aStr[256];
		//sprintf(aStr, "HWND: %d\r\n", aWindow);
		//OutputDebugString(aStr);
				
		DestroyWindow(aWindow);
	}*/

	WaitForLoadingThread();	

	/* The cursors are also unique_ptr so they don't need to be freed.
	DestroyCursor(mHandCursor);
	DestroyCursor(mDraggingCursor);	
	*/

	gSexyAppBase = NULL;

	WriteDemoBuffer();

	/*
	if (mMutex != NULL)
		::CloseHandle(mMutex);
	*/

	/*
	FreeLibrary(gDDrawDLL);
	FreeLibrary(gDSoundDLL);
	FreeLibrary(gVersionDLL);
	*/
}
/*
static BOOL CALLBACK ChangeDisplayWindowEnumProc(HWND hwnd, LPARAM lParam)
{
	typedef std::map<HWND,RECT> WindowMap;
	static WindowMap aMap;

	if (lParam==0 && aMap.find(hwnd)==aMap.end()) // record
	{
		RECT aRect;
		if (!IsIconic(hwnd) && IsWindowVisible(hwnd))
		{
			if (GetWindowRect(hwnd,&aRect))
			{
//				char aBuf[4096];
//				GetWindowText(hwnd,aBuf,4000);
//				DWORD aProcessId = 0;
//				GetWindowThreadProcessId(hwnd,&aProcessId);
//				SEXY_TRACE(StrFormat("%s %d - %d %d %d %d",aBuf,aProcessId,aRect.left,aRect.top,aRect.right,aRect.bottom).c_str());
				aMap[hwnd] = aRect;
			}
		}
	}
	else 
	{
		WindowMap::iterator anItr = aMap.find(hwnd);
		if (anItr != aMap.end())
		{
			RECT &r = anItr->second;
			MoveWindow(hwnd,r.left,r.top,abs(r.right-r.left),abs(r.bottom-r.top),TRUE);
		}
	}
	return TRUE;
}*/

/*
void SexyAppBase::ClearUpdateBacklog(bool relaxForASecond)
{
	mLastTimeCheck = std::chrono::high_resolution_clock::now();
	mUpdateFTimeAcc = std::chrono::high_resolution_clock::duration(0);

	if (relaxForASecond)
		mRelaxUpdateBacklogCount = 1000;
}*/

bool SexyAppBase::IsScreenSaver()
{
	return mIsScreenSaver;
}

bool SexyAppBase::AppCanRestore()
{
	return !mIsDisabled;
}

bool SexyAppBase::ReadDemoBuffer(std::string &theError)
{
	FILE* aFP = fopen(mDemoFileName.c_str(), "rb");

	if (aFP == NULL)
	{
		theError = "Demo file not found: " + mDemoFileName;
		return false;
	}

	struct AutoFile { FILE *f; AutoFile(FILE *file) : f(file) {} ~AutoFile() { fclose(f); } };
	AutoFile aCloseFile(aFP);

	ulong aFileID;
	fread(&aFileID, 4, 1, aFP);

	DBG_ASSERTE(aFileID == DEMO_FILE_ID);
	if (aFileID != DEMO_FILE_ID)
	{
		theError = "Invalid demo file.";
		return false;
	}


	ulong aVersion;
	fread(&aVersion, 4, 1, aFP);
	
	fread(&mRandSeed, 4, 1, aFP);
	SRand(mRandSeed);

	ushort aStrLen = 4;
	fread(&aStrLen, 2, 1, aFP);
	if (aStrLen > 255)
		aStrLen = 255;
	char aStr[256];
	fread(aStr, 1, aStrLen, aFP);
	aStr[aStrLen] = '\0';

	unreachable(); // FIXME
	/*
	DBG_ASSERTE(mProductVersion == aStr);
	if (mProductVersion != aStr)
	{
		theError = "This demo file appears to be for '" + std::string(aStr) + "'";
		return false;
	}*/

	int aFilePos = ftell(aFP);
	fseek(aFP, 0, SEEK_END);
	int aBytesLeft = ftell(aFP) - aFilePos;
	fseek(aFP, aFilePos, SEEK_SET);

	uchar* aBuffer;
	// read marker list
	if (aVersion >= 2) 
	{
		int aSize;
		fread(&aSize, 4, 1, aFP);
		aBytesLeft -= 4;

		if (aSize >= aBytesLeft)
		{
			theError = "Invalid demo file.";
			return false;
		}

		Buffer aMarkerBuffer;

		aBuffer = new uchar[aSize];
		fread(aBuffer, 1, aSize, aFP);
		aMarkerBuffer.WriteBytes(aBuffer, aSize);
		aMarkerBuffer.SeekFront();

		int aNumItems = aMarkerBuffer.ReadLong();
		int i;
		for (i=0; i<aNumItems && !aMarkerBuffer.AtEnd(); i++)
		{
			mDemoMarkerList.push_back(DemoMarker());
			DemoMarker &aMarker = mDemoMarkerList.back();
			aMarker.first = aMarkerBuffer.ReadString();
			aMarker.second = aMarkerBuffer.ReadLong();
		}

		if (i!=aNumItems)
		{
			theError = "Invalid demo file.";
			return false;
		}

		aBytesLeft -= aSize;

		delete [] aBuffer;			
	}

	// Read demo commands
	fread(&mDemoLength, 4, 1, aFP);
	aBytesLeft -= 4;
	
	if (aBytesLeft <= 0)
	{
		theError = "Invalid demo file.";
		return false;
	}


	aBuffer = new uchar[aBytesLeft];
	fread(aBuffer, 1, aBytesLeft, aFP);		

	mDemoBuffer.WriteBytes(aBuffer, aBytesLeft);
	mDemoBuffer.SeekFront();

	delete [] aBuffer;
	return true;
}

void SexyAppBase::WriteDemoBuffer()
{
	if (mRecordingDemoBuffer)
	{
		FILE* aFP = fopen(mDemoFileName.c_str(), "w+b");

		if (aFP != NULL)
		{
			ulong aFileID = DEMO_FILE_ID;
			fwrite(&aFileID, 4, 1, aFP);		

			ulong aVersion = DEMO_VERSION;
			fwrite(&aVersion, 4, 1, aFP);
			
			fwrite(&mRandSeed, 4, 1, aFP);

			unreachable();
			/* FIXME
			ushort aStrLen = mProductVersion.length();
			fwrite(&aStrLen, 2, 1, aFP);		
			fwrite(mProductVersion.c_str(), 1, mProductVersion.length(), aFP);*/

			Buffer aMarkerBuffer;
			aMarkerBuffer.WriteLong(mDemoMarkerList.size());
			for (DemoMarkerList::iterator aMarkerItr = mDemoMarkerList.begin(); aMarkerItr != mDemoMarkerList.end(); ++aMarkerItr)
			{
				aMarkerBuffer.WriteString(aMarkerItr->first);
				aMarkerBuffer.WriteLong(aMarkerItr->second);
			}
			int aMarkerBufferSize = aMarkerBuffer.GetDataLen();
			fwrite(&aMarkerBufferSize, 4, 1, aFP);
			fwrite(aMarkerBuffer.GetDataPtr(), aMarkerBufferSize, 1, aFP);

			ulong aDemoLength = mUpdateCount;
			fwrite(&aDemoLength, 4, 1, aFP);

			fwrite(mDemoBuffer.GetDataPtr(), 1, mDemoBuffer.GetDataLen(), aFP);
			fclose(aFP);
		}		
	}
}

void SexyAppBase::DemoSyncBuffer(Buffer* theBuffer)
{
	if (mPlayingDemoBuffer)
	{
		if (mManualShutdown)
			return;

		PrepareDemoCommand(true);
		mDemoNeedsCommand = true;
		
		DBG_ASSERTE(!mDemoIsShortCmd);
		DBG_ASSERTE(mDemoCmdNum == DEMO_SYNC);

		ulong aLen = mDemoBuffer.ReadLong();
		
		theBuffer->Clear();
		for (int i = 0; i < (int) aLen; i++)
			theBuffer->WriteByte(mDemoBuffer.ReadByte());		
	}
	else if (mRecordingDemoBuffer)
	{
		WriteDemoTimingBlock();
		mDemoBuffer.WriteNumBits(0, 1);
		mDemoBuffer.WriteNumBits(DEMO_SYNC, 5);		
		mDemoBuffer.WriteLong(theBuffer->GetDataLen());
		mDemoBuffer.WriteBytes((uchar*) theBuffer->GetDataPtr(), theBuffer->GetDataLen());
	}
}

void SexyAppBase::DemoSyncString(std::string* theString)
{
	Buffer aBuffer;
	aBuffer.WriteString(*theString);
	DemoSyncBuffer(&aBuffer);
	*theString = aBuffer.ReadString();
}

void SexyAppBase::DemoSyncInt(int* theInt)
{
	Buffer aBuffer;
	aBuffer.WriteLong(*theInt);
	DemoSyncBuffer(&aBuffer);
	*theInt = aBuffer.ReadLong();
}

void SexyAppBase::DemoSyncBool(bool* theBool)
{
	Buffer aBuffer;
	aBuffer.WriteBoolean(*theBool);
	DemoSyncBuffer(&aBuffer);
	*theBool = aBuffer.ReadBoolean();
}

void SexyAppBase::DemoAssertStringEqual(const std::string& theString)
{
	if (mPlayingDemoBuffer)
	{
		if (mManualShutdown)
			return;

		PrepareDemoCommand(true);
		mDemoNeedsCommand = true;
		
		DBG_ASSERTE(!mDemoIsShortCmd);
		DBG_ASSERTE(mDemoCmdNum == DEMO_ASSERT_STRING_EQUAL);

		std::string aString = mDemoBuffer.ReadString();
		DBG_ASSERTE(aString == theString);
	}
	else if (mRecordingDemoBuffer)
	{
		WriteDemoTimingBlock();
		mDemoBuffer.WriteNumBits(0, 1);
		mDemoBuffer.WriteNumBits(DEMO_ASSERT_STRING_EQUAL, 5);				
		mDemoBuffer.WriteString(theString);
	}
}

void SexyAppBase::DemoAddMarker(const std::string& theString)
{
	if (mPlayingDemoBuffer)
	{
		mFastForwardToMarker = false;
	}
	else if (mRecordingDemoBuffer)
	{
		mDemoMarkerList.push_back(DemoMarker(theString,mUpdateCount));
	}
}

/*
void SexyAppBase::DemoRegisterHandle(HANDLE theHandle)
{
	if ((mRecordingDemoBuffer) || (mPlayingDemoBuffer))
	{
		// Insert the handle into a map with an auto-incrementing number so
		//  we can match up the auto-incrementing numbers with the handle
		//  later on, as handles may not be the same between executions
		std::pair<HandleToIntMap::iterator, bool> aPair = mHandleToIntMap.insert(HandleToIntMap::value_type(theHandle, mCurHandleNum));
		(void)aPair; // unused in Release mode
		DBG_ASSERT(aPair.second);
		mCurHandleNum++;
	}
}*/

/*
void SexyAppBase::DemoWaitForHandle(HANDLE theHandle)
{
	WaitForSingleObject(theHandle, INFINITE);
	
	if ((mRecordingDemoBuffer) || (mPlayingDemoBuffer))
	{
		// Remove the handle from our waiting map
		HandleToIntMap::iterator anItr = mHandleToIntMap.find(theHandle);					
		DBG_ASSERT(anItr != mHandleToIntMap.end());
		mHandleToIntMap.erase(anItr);
	}
}*/

/*
bool SexyAppBase::DemoCheckHandle(HANDLE theHandle)
{
	if (mPlayingDemoBuffer)
	{	
		// We only need to try to get the result if we think we are waiting for one	
		if (gSexyAppBase->PrepareDemoCommand(false))
		{
			if ((!gSexyAppBase->mDemoIsShortCmd) && (gSexyAppBase->mDemoCmdNum == DEMO_HANDLE_COMPLETE))
			{
				// Find auto-incrementing handle num from handle
				HandleToIntMap::iterator anItr = mHandleToIntMap.find(theHandle);					
				DBG_ASSERT(anItr != mHandleToIntMap.end());

				int anOldBufferPos = gSexyAppBase->mDemoBuffer.mReadBitPos;

				// Since we don't require a demo result entry to be here, we must verify
				//  that this is referring to us
				int aDemoHandleNum = gSexyAppBase->mDemoBuffer.ReadLong();
				
				if (aDemoHandleNum == anItr->second)
				{
					// Alright, this was the handle we were waiting for!
					gSexyAppBase->mDemoNeedsCommand = true;

					// Actually wait for our local buddy to complete
					WaitForSingleObject(theHandle, INFINITE);
					mHandleToIntMap.erase(anItr);

					return true;
				}
				else
				{
					// Not us, go back
					gSexyAppBase->mDemoBuffer.mReadBitPos = anOldBufferPos;
				}
			}
		}

		return false;	
	}
	else 
	{
		if (WaitForSingleObject(theHandle, 0) == WAIT_OBJECT_0)
		{
			if (mRecordingDemoBuffer)
			{
				// Find auto-incrementing handle num from handle
				HandleToIntMap::iterator anItr = mHandleToIntMap.find(theHandle);					
				DBG_ASSERT(anItr != mHandleToIntMap.end());

				gSexyAppBase->WriteDemoTimingBlock();
				gSexyAppBase->mDemoBuffer.WriteNumBits(0, 1);
				gSexyAppBase->mDemoBuffer.WriteNumBits(DEMO_HANDLE_COMPLETE, 5);
				gSexyAppBase->mDemoBuffer.WriteLong(anItr->second);

				mHandleToIntMap.erase(anItr);
			}

			return true;
		}

		return false;
	}
}*/

void SexyAppBase::DemoAssertIntEqual(int theInt)
{
	if (mPlayingDemoBuffer)
	{
		if (mManualShutdown)
			return;

		PrepareDemoCommand(true);
		mDemoNeedsCommand = true;
		
		DBG_ASSERTE(!mDemoIsShortCmd);
		DBG_ASSERTE(mDemoCmdNum == DEMO_ASSERT_INT_EQUAL);

		int anInt = mDemoBuffer.ReadLong();
		(void)anInt; // unused in Release mode
		DBG_ASSERTE(anInt == theInt);
	}
	else if (mRecordingDemoBuffer)
	{		
		WriteDemoTimingBlock();
		mDemoBuffer.WriteNumBits(0, 1);
		mDemoBuffer.WriteNumBits(DEMO_ASSERT_INT_EQUAL, 5);				
		mDemoBuffer.WriteLong(theInt);
	}
}

Dialog* SexyAppBase::NewDialog(int theDialogId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode)
{	
	Dialog* aDialog = new Dialog(NULL, NULL, theDialogId, isModal, theDialogHeader,	theDialogLines, theDialogFooter, theButtonMode);		
	return aDialog;
}

Dialog* SexyAppBase::DoDialog(int theDialogId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode)
{
	KillDialog(theDialogId);

	Dialog* aDialog = NewDialog(theDialogId, isModal, theDialogHeader, theDialogLines, theDialogFooter, theButtonMode);		

	AddDialog(theDialogId, aDialog);

	return aDialog;
}


Dialog*	SexyAppBase::GetDialog(int theDialogId)
{
	DialogMap::iterator anItr = mDialogMap.find(theDialogId);

	if (anItr != mDialogMap.end())	
		return anItr->second;

	return NULL;
}

bool SexyAppBase::KillDialog(int theDialogId, bool removeWidget, bool deleteWidget)
{
	DialogMap::iterator anItr = mDialogMap.find(theDialogId);

	if (anItr != mDialogMap.end())
	{
		Dialog* aDialog = anItr->second;

		// set the result to something else so DoMainLoop knows that the dialog is gone 
		// in case nobody else sets mResult		
		if (aDialog->mResult == -1) 
			aDialog->mResult = 0;
		
		DialogList::iterator aListItr = std::find(mDialogList.begin(),mDialogList.end(),aDialog);
		if (aListItr != mDialogList.end())
			mDialogList.erase(aListItr);
		
		mDialogMap.erase(anItr);

		if (removeWidget || deleteWidget)
			mWidgetManager->RemoveWidget(aDialog);

		if (aDialog->IsModal())
		{			
			ModalClose();
			mWidgetManager->RemoveBaseModal(aDialog);
		}				

		if (deleteWidget)
			SafeDeleteWidget(aDialog);
		
		return true;
	}

	return false;
}

bool SexyAppBase::KillDialog(int theDialogId)
{
	return KillDialog(theDialogId,true,true);
}

bool SexyAppBase::KillDialog(Dialog* theDialog)
{
	return KillDialog(theDialog->mId);
}

int SexyAppBase::GetDialogCount()
{
	return mDialogMap.size();
}

void SexyAppBase::AddDialog(int theDialogId, Dialog* theDialog)
{
	KillDialog(theDialogId);

	if (theDialog->mWidth == 0)
	{
		// Set the dialog position ourselves
		int aWidth = mWidth/2;
		theDialog->Resize((mWidth - aWidth)/2, mHeight / 5, aWidth, theDialog->GetPreferredHeight(aWidth));
	}

	mDialogMap.insert(DialogMap::value_type(theDialogId, theDialog));
	mDialogList.push_back(theDialog);

	mWidgetManager->AddWidget(theDialog);
	if (theDialog->IsModal())
	{
		mWidgetManager->AddBaseModal(theDialog);
		ModalOpen();
	}
}

void SexyAppBase::AddDialog(Dialog* theDialog)
{
	AddDialog(theDialog->mId, theDialog);
}

void SexyAppBase::ModalOpen()
{
}

void SexyAppBase::ModalClose()
{
}

void SexyAppBase::DialogButtonPress(int theDialogId, int theButtonId)
{	
	if (theButtonId == Dialog::ID_YES)
		ButtonPress(2000 + theDialogId);
	else if (theButtonId == Dialog::ID_NO)
		ButtonPress(3000 + theDialogId);	
}

void SexyAppBase::DialogButtonDepress(int theDialogId, int theButtonId)
{
	if (theButtonId == Dialog::ID_YES)
		ButtonDepress(2000 + theDialogId);
	else if (theButtonId == Dialog::ID_NO)
		ButtonDepress(3000 + theDialogId);
}

void SexyAppBase::GotFocus()
{	
}

void SexyAppBase::LostFocus()
{	
}

void SexyAppBase::URLOpenFailed(const std::string& theURL)
{
	(void)theURL;
	mIsOpeningURL = false;
}

void SexyAppBase::URLOpenSucceeded(const std::string& theURL)
{	
	(void)theURL;
	mIsOpeningURL = false;

	if (mShutdownOnURLOpen)
		Shutdown();	
}

bool SexyAppBase::OpenURL(const std::string& theURL, bool shutdownOnOpen)
{
	if ((!mIsOpeningURL) || (theURL != mOpeningURL))
	{
		mShutdownOnURLOpen = shutdownOnOpen;
		mIsOpeningURL = true;
		mOpeningURL = theURL;
		mOpeningURLTime = clock();		

		unreachable();
		/* FIXME
		if ((intptr_t) ShellExecuteA(NULL, "open", theURL.c_str(), NULL, NULL, SW_SHOWNORMAL) > 32)
		{
			return true;
		}
		else
		{
			URLOpenFailed(theURL);
			return false;
		}*/
	}

	return true;
}

std::string SexyAppBase::GetProductVersion(const std::string& /*thePath*/)
{
	unreachable();
	/* TODO
	// Dynamically Load Version.dll
	typedef DWORD (APIENTRY *GetFileVersionInfoSizeFunc)(LPSTR lptstrFilename, LPDWORD lpdwHandle);
	typedef BOOL (APIENTRY *GetFileVersionInfoFunc)(LPSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData);
	typedef BOOL (APIENTRY *VerQueryValueFunc)(const LPVOID pBlock, LPSTR lpSubBlock, LPVOID * lplpBuffer, PUINT puLen);

	static GetFileVersionInfoSizeFunc aGetFileVersionInfoSizeFunc = NULL;
	static GetFileVersionInfoFunc aGetFileVersionInfoFunc = NULL;
	static VerQueryValueFunc aVerQueryValueFunc = NULL;

	if (aGetFileVersionInfoSizeFunc==NULL)
	{
		aGetFileVersionInfoSizeFunc = (GetFileVersionInfoSizeFunc)GetProcAddress(gVersionDLL,"GetFileVersionInfoSizeA");
		aGetFileVersionInfoFunc = (GetFileVersionInfoFunc)GetProcAddress(gVersionDLL,"GetFileVersionInfoA");
		aVerQueryValueFunc = (VerQueryValueFunc)GetProcAddress(gVersionDLL,"VerQueryValueA");
	}

	// Get Product Version
	std::string aProductVersion;
	
	uint aSize = aGetFileVersionInfoSizeFunc((char*) thePath.c_str(), 0);
	if (aSize > 0)		
	{
		uchar* aVersionBuffer = new uchar[aSize];
		aGetFileVersionInfoFunc((char*) thePath.c_str(), 0, aSize, aVersionBuffer);	
		char* aBuffer;
		if (aVerQueryValueFunc(aVersionBuffer, 
				  (char*)"\\StringFileInfo\\040904B0\\ProductVersion",
				  (void**) &aBuffer, 
				  &aSize))
		{
			aProductVersion = aBuffer;
		}
		else if (aVerQueryValueFunc(aVersionBuffer, 
				  (char*)"\\StringFileInfo\\040904E4\\ProductVersion", 
				  (void**) &aBuffer, 
				  &aSize))
		{
			aProductVersion = aBuffer;
		}

		delete[] aVersionBuffer;	
	}

	return aProductVersion;*/
}

void SexyAppBase::WaitForLoadingThread()
{
	while ((mLoadingThreadStarted) && (!mLoadingThreadCompleted))
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

/*
void SexyAppBase::SetCursorImage(int theCursorNum, Image* theImage)
{
	if ((theCursorNum >= 0) && (theCursorNum < NUM_CURSORS))
	{
		mCursorImages[theCursorNum] = theImage;
		EnforceCursor();
	}
}*/

void SexyAppBase::TakeScreenshot()
{
	unreachable();
	/* TODO
	if (mDDInterface==NULL || mDDInterface->mDrawSurface==NULL)
		return;

	// Get free image name
	std::string anImageDir = GetAppDataFolder() + "_screenshots";
	MkDir(anImageDir);
	anImageDir += "/";

	WIN32_FIND_DATAA aData;
	int aMaxId = 0;
	std::string anImagePrefix = "image";
	HANDLE aHandle = FindFirstFileA((anImageDir + "*.png").c_str(), &aData);
	if (aHandle!=INVALID_HANDLE_VALUE)
	{
		do {
			int aNum = 0;
			if (sscanf(aData.cFileName,(anImagePrefix + "%d.png").c_str(), &aNum)==1)
			{
				if (aNum>aMaxId)
					aMaxId = aNum;
			}

		} 
		while(FindNextFileA(aHandle,&aData));
		FindClose(aHandle);
	}
	std::string anImageName = anImageDir + anImagePrefix + StrFormat("%d.png",aMaxId+1);

	// Capture screen
	LPDIRECTDRAWSURFACE aSurface = mDDInterface->mDrawSurface;
	
	// Temporarily set the mDrawSurface to NULL so DDImage::Check3D 
	// returns false so we can lock the surface.
	mDDInterface->mDrawSurface = NULL; 
	
	DDImage anImage(mDDInterface);
	anImage.SetSurface(aSurface);
	anImage.GetBits();
	anImage.DeleteDDSurface();
	mDDInterface->mDrawSurface = aSurface; 

	if (anImage.mBits==NULL)
		return;
		
	// Write image
	ImageLib::Image aSaveImage;
	aSaveImage.mBits = anImage.mBits;
	aSaveImage.mWidth = anImage.mWidth;
	aSaveImage.mHeight = anImage.mHeight;
	ImageLib::WritePNGImage(anImageName, &aSaveImage);
	aSaveImage.mBits = NULL;*/
		

/*
	keybd_event(VK_MENU,0,0,0);
    keybd_event(VK_SNAPSHOT,0,0,0);
    keybd_event(VK_MENU,0,KEYEVENTF_KEYUP,0);
	if (OpenClipboard(mHWnd))
	{
		HBITMAP aBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
		if (aBitmap!=NULL)
		{
			BITMAP anObject;
			ZeroMemory(&anObject,sizeof(anObject));
			GetObject(aBitmap,sizeof(anObject),&anObject);

			BITMAPINFO anInfo;
			ZeroMemory(&anInfo,sizeof(anInfo));
			BITMAPINFOHEADER &aHeader = anInfo.bmiHeader;
			aHeader.biBitCount = 32;
			aHeader.biPlanes = 1;
			aHeader.biHeight = -abs(anObject.bmHeight);
			aHeader.biWidth = abs(anObject.bmWidth);
			aHeader.biSize = sizeof(aHeader);
			aHeader.biSizeImage = aHeader.biHeight*aHeader.biWidth*4;
			ImageLib::Image aSaveImage;
			aSaveImage.mBits = new DWORD[abs(anObject.bmWidth*anObject.bmHeight)];
			aSaveImage.mWidth = abs(anObject.bmWidth);
			aSaveImage.mHeight = abs(anObject.bmHeight);

			HDC aDC = GetDC(NULL);
			if (GetDIBits(aDC,aBitmap,0,aSaveImage.mHeight,aSaveImage.mBits,&anInfo,DIB_RGB_COLORS))
				ImageLib::WritePNGImage(anImageName, &aSaveImage);

			ReleaseDC(NULL,aDC);
		}
		CloseClipboard();
	}*/

	//ClearUpdateBacklog();
}

void SexyAppBase::DumpProgramInfo()
{
	unreachable();
	/* TODO
	Deltree(GetAppDataFolder() + "_dump");

	for (;;)
	{
		if (mkdir((GetAppDataFolder() + "_dump").c_str()))
			break;
		Sleep(100);
	}

	std::fstream aDumpStream((GetAppDataFolder() + "_dump\\imagelist.html").c_str(), std::ios::out);

	time_t aTime;
	time(&aTime);
	tm* aTM = localtime(&aTime);

	aDumpStream << "<HTML><BODY BGCOLOR=EEEEFF><CENTER><FONT SIZE=+2><B>" << asctime(aTM) << "</B></FONT><BR>" << std::endl;

	int anImgNum = 0;

	int aThumbWidth = 64;
	int aThumbHeight = 64;

	ImageLib::Image anImageLibImage;
	anImageLibImage.mWidth = aThumbWidth;
	anImageLibImage.mHeight = aThumbHeight;
	anImageLibImage.mBits = new unsigned long[aThumbWidth*aThumbHeight];	

	typedef std::multimap<int, MemoryImage*, std::greater<int> > SortedImageMap;

	int aTotalMemory = 0;

	SortedImageMap aSortedImageMap;
	MemoryImageSet::iterator anItr = mMemoryImageSet.begin();
	while (anItr != mMemoryImageSet.end())
	{
		MemoryImage* aMemoryImage = *anItr;				

		int aNumPixels = aMemoryImage->mWidth*aMemoryImage->mHeight;

		DDImage* aDDImage = dynamic_cast<DDImage*>(aMemoryImage);

		int aBitsMemory = 0;
		int aSurfaceMemory = 0;
		int aPalletizedMemory = 0;
		int aNativeAlphaMemory = 0;
		int aRLAlphaMemory = 0;
		int aRLAdditiveMemory = 0;
		int aTextureMemory = 0;

		int aMemorySize = 0;
		if (aMemoryImage->mBits != NULL)
			aBitsMemory = aNumPixels * 4;
		if ((aDDImage != NULL) && (aDDImage->mSurface != NULL))
			aSurfaceMemory = aNumPixels * 4; // Assume 32bit screen...
		if (aMemoryImage->mColorTable != NULL)
			aPalletizedMemory = aNumPixels + 256*4;
		if (aMemoryImage->mNativeAlphaData != NULL)
		{
			if (aMemoryImage->mColorTable != NULL)
				aNativeAlphaMemory = 256*4;
			else
				aNativeAlphaMemory = aNumPixels * 4;
		}
		if (aMemoryImage->mRLAlphaData != NULL)
			aRLAlphaMemory = aNumPixels;
		if (aMemoryImage->mRLAdditiveData != NULL)
			aRLAdditiveMemory = aNumPixels;
		if (aMemoryImage->mD3DData != NULL)
			aTextureMemory += ((TextureData*)aMemoryImage->mD3DData)->mTexMemSize;

		aMemorySize = aBitsMemory + aSurfaceMemory + aPalletizedMemory + aNativeAlphaMemory + aRLAlphaMemory + aRLAdditiveMemory + aTextureMemory;
		aTotalMemory += aMemorySize;

		aSortedImageMap.insert(SortedImageMap::value_type(aMemorySize, aMemoryImage));

		++anItr;
	}

	aDumpStream << "Total Image Allocation: " << CommaSeperate(aTotalMemory).c_str() << " bytes<BR>";
	aDumpStream << "<TABLE BORDER=1 CELLSPACING=0 CELLPADDING=4>";

	int aTotalMemorySize = 0;
	int aTotalBitsMemory = 0;
	int aTotalSurfaceMemory = 0;
	int aTotalPalletizedMemory = 0;
	int aTotalNativeAlphaMemory = 0;
	int aTotalRLAlphaMemory = 0;
	int aTotalRLAdditiveMemory = 0;
	int aTotalTextureMemory = 0;

	SortedImageMap::iterator aSortedItr = aSortedImageMap.begin();
	while (aSortedItr != aSortedImageMap.end())
	{
		MemoryImage* aMemoryImage = aSortedItr->second;				

		char anImageName[256];
		sprintf(anImageName, "img%04d.png", anImgNum);

		char aThumbName[256];
		sprintf(aThumbName, "thumb%04d.jpg", anImgNum);
		
		aDumpStream << "<TR>" << std::endl;

		aDumpStream << "<TD><A HREF=" << anImageName << "><IMG SRC=" << aThumbName << " WIDTH=" << aThumbWidth << " HEIGHT=" << aThumbHeight << "></A></TD>" << std::endl;
		
		int aNumPixels = aMemoryImage->mWidth*aMemoryImage->mHeight;

		DDImage* aDDImage = dynamic_cast<DDImage*>(aMemoryImage);

		int aMemorySize = aSortedItr->first;

		int aBitsMemory = 0;
		int aSurfaceMemory = 0;
		int aPalletizedMemory = 0;
		int aNativeAlphaMemory = 0;
		int aRLAlphaMemory = 0;
		int aRLAdditiveMemory = 0;
		int aTextureMemory = 0;
		std::string aTextureFormatName;
		
		if (aMemoryImage->mBits != NULL)
			aBitsMemory = aNumPixels * 4;
		if ((aDDImage != NULL) && (aDDImage->mSurface != NULL))
			aSurfaceMemory = aNumPixels * 4; // Assume 32bit screen...
		if (aMemoryImage->mColorTable != NULL)
			aPalletizedMemory = aNumPixels + 256*4;
		if (aMemoryImage->mNativeAlphaData != NULL)
		{
			if (aMemoryImage->mColorTable != NULL)
				aNativeAlphaMemory = 256*4;
			else
				aNativeAlphaMemory = aNumPixels * 4;
		}
		if (aMemoryImage->mRLAlphaData != NULL)
			aRLAlphaMemory = aNumPixels;
		if (aMemoryImage->mRLAdditiveData != NULL)
			aRLAdditiveMemory = aNumPixels;		
		if (aMemoryImage->mD3DData != NULL)
		{
			aTextureMemory += ((TextureData*)aMemoryImage->mD3DData)->mTexMemSize;

			switch (((TextureData*)aMemoryImage->mD3DData)->mPixelFormat)
			{				
			case PixelFormat_A8R8G8B8: aTextureFormatName = "A8R8G8B8"; break;
			case PixelFormat_A4R4G4B4: aTextureFormatName = "A4R4G4B4"; break;
			case PixelFormat_R5G6B5: aTextureFormatName = "R5G6B5"; break;
			case PixelFormat_Palette8: aTextureFormatName = "Palette8"; break;
			case PixelFormat_Unknown: break;
			}			
		}

		aTotalMemorySize		+= aMemorySize;
		aTotalBitsMemory		+= aBitsMemory;
		aTotalTextureMemory		+= aTextureMemory;
		aTotalSurfaceMemory		+= aSurfaceMemory;
		aTotalPalletizedMemory	+= aPalletizedMemory;
		aTotalNativeAlphaMemory	+= aNativeAlphaMemory;
		aTotalRLAlphaMemory		+= aRLAlphaMemory;
		aTotalRLAdditiveMemory	+= aRLAdditiveMemory;

				

		char aStr[256];
		sprintf(aStr, "%d x %d<BR>%s bytes", aMemoryImage->mWidth, aMemoryImage->mHeight, CommaSeperate(aMemorySize).c_str());
		aDumpStream << "<TD ALIGN=RIGHT>" << aStr << "</TD>" << std::endl;

		aDumpStream << "<TD>" << SexyStringToString(((aBitsMemory != 0) ? _S("mBits<BR>") + CommaSeperate(aBitsMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;	
		aDumpStream << "<TD>" << SexyStringToString(((aPalletizedMemory != 0) ? _S("Palletized<BR>") + CommaSeperate(aPalletizedMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;				
		aDumpStream << "<TD>" << SexyStringToString(((aSurfaceMemory != 0) ? _S("DDSurface<BR>") + CommaSeperate(aSurfaceMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;		
		aDumpStream << "<TD>" << SexyStringToString(((aMemoryImage->mD3DData!=NULL) ? _S("Texture<BR>") + StringToSexyString(aTextureFormatName) + _S("<BR>") + CommaSeperate(aTextureMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;

		aDumpStream << "<TD>" << SexyStringToString(((aMemoryImage->mIsVolatile) ? _S("Volatile") : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aMemoryImage->mForcedMode) ? _S("Forced") : _S("&nbsp;"))) << "</TD>" << std::endl;		
		aDumpStream << "<TD>" << SexyStringToString(((aMemoryImage->mHasAlpha) ? _S("HasAlpha") : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aMemoryImage->mHasTrans) ? _S("HasTrans") : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aNativeAlphaMemory != 0) ? _S("NativeAlpha<BR>") + CommaSeperate(aNativeAlphaMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aRLAlphaMemory != 0) ? _S("RLAlpha<BR>") + CommaSeperate(aRLAlphaMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << SexyStringToString(((aRLAdditiveMemory != 0) ? _S("RLAdditive<BR>") + CommaSeperate(aRLAdditiveMemory) : _S("&nbsp;"))) << "</TD>" << std::endl;
		aDumpStream << "<TD>" << (aMemoryImage->mFilePath.empty()? "&nbsp;":aMemoryImage->mFilePath) << "</TD>" << std::endl;

		aDumpStream << "</TR>" << std::endl;
		
		// Write thumb

		MemoryImage aCopiedImage(*aMemoryImage);

		ulong* aBits = aCopiedImage.GetBits();

		ulong* aThumbBitsPtr = anImageLibImage.mBits;

		for (int aThumbY = 0; aThumbY < aThumbHeight; aThumbY++)
			for (int aThumbX = 0; aThumbX < aThumbWidth; aThumbX++)
			{
				int aSrcX = (int) (aCopiedImage.mWidth  * (aThumbX + 0.5)) / aThumbWidth;
				int aSrcY = (int) (aCopiedImage.mHeight * (aThumbY + 0.5)) / aThumbHeight;
				
				*(aThumbBitsPtr++) = aBits[aSrcX + (aSrcY*aCopiedImage.mWidth)];
			}

		ImageLib::WriteJPEGImage((GetAppDataFolder() + std::string("_dump\\") + aThumbName).c_str(), &anImageLibImage);

		// Write high resolution image

		ImageLib::Image anFullImage;
		anFullImage.mBits = aCopiedImage.GetBits();
		anFullImage.mWidth = aCopiedImage.GetWidth();
		anFullImage.mHeight = aCopiedImage.GetHeight();

		ImageLib::WritePNGImage((GetAppDataFolder() + std::string("_dump\\") + anImageName).c_str(), &anFullImage);

		anFullImage.mBits = NULL;

		anImgNum++;

		aSortedItr++;		
	}

	aDumpStream << "<TD>Totals</TD>" << std::endl;
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalMemorySize)) << "</TD>" << std::endl;	
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalBitsMemory)) << "</TD>" << std::endl;	
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalPalletizedMemory)) << "</TD>" << std::endl;				
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalSurfaceMemory)) << "</TD>" << std::endl;		
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalTextureMemory)) << "</TD>" << std::endl;		
	aDumpStream << "<TD>&nbsp;</TD>" << std::endl;
	aDumpStream << "<TD>&nbsp;</TD>" << std::endl;
	aDumpStream << "<TD>&nbsp;</TD>" << std::endl;
	aDumpStream << "<TD>&nbsp;</TD>" << std::endl;
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalNativeAlphaMemory)) << "</TD>" << std::endl;
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalRLAlphaMemory)) << "</TD>" << std::endl;
	aDumpStream << "<TD>" << SexyStringToString(CommaSeperate(aTotalRLAdditiveMemory)) << "</TD>" << std::endl;
	aDumpStream << "<TD>&nbsp;</TD>" << std::endl;

	aDumpStream << "</TABLE></CENTER></BODY></HTML>" << std::endl;
	*/
}

double SexyAppBase::GetLoadingThreadProgress()
{
	if (mLoaded)
		return 1.0;
	if (!mLoadingThreadStarted)
		return 0.0;
	if (mNumLoadingThreadTasks == 0)
		return 0.0;
	return std::min(mCompletedLoadingThreadTasks / (double) mNumLoadingThreadTasks, 1.0);
}

bool SexyAppBase::RegistryWrite(const std::string& theValueName, uint32_t theType, const uchar* theValue, uint32_t theLength)
{
	if (mRegKey.length() == 0)
		return false;

	if (mPlayingDemoBuffer)
	{
		if (mManualShutdown)
			return true;

		PrepareDemoCommand(true);
		mDemoNeedsCommand = true;

		DBG_ASSERTE(!mDemoIsShortCmd);		
		DBG_ASSERTE(mDemoCmdNum == DEMO_REGISTRY_WRITE);

		return mDemoBuffer.ReadNumBits(1, false) != 0;		
	}

	//HKEY aGameKey;
	
	std::string aKeyName = RemoveTrailingSlash("SOFTWARE\\" + mRegKey);
	std::string aValueName;

	int aSlashPos = (int) theValueName.rfind('\\');
	if (aSlashPos != -1)
	{
		aKeyName += "\\" + theValueName.substr(0, aSlashPos);
		aValueName = theValueName.substr(aSlashPos + 1);
	}
	else
	{
		aValueName = theValueName;
	}

	mRegHandle->Write(aValueName, theType, theValue, theLength);

	if (mRecordingDemoBuffer)
	{
		WriteDemoTimingBlock();
		mDemoBuffer.WriteNumBits(0, 1);
		mDemoBuffer.WriteNumBits(DEMO_REGISTRY_WRITE, 5);
		mDemoBuffer.WriteNumBits(1, 1); // success
	}

	return true;
}

bool SexyAppBase::RegistryWriteString(const std::string& theValueName, const std::string& theString)
{
	return RegistryWrite(theValueName, REG_SZ, (uchar*) theString.c_str(), theString.length());
}

bool SexyAppBase::RegistryWriteInteger(const std::string& theValueName, int theValue)
{
	return RegistryWrite(theValueName, REG_DWORD, (uchar*) &theValue, sizeof(int));
}

bool SexyAppBase::RegistryWriteBoolean(const std::string& theValueName, bool theValue)
{
	int aValue = theValue ? 1 : 0;
	return RegistryWrite(theValueName, REG_DWORD, (uchar*) &aValue, sizeof(int));
}

bool SexyAppBase::RegistryWriteData(const std::string& theValueName, const uchar* theValue, ulong theLength)
{
	return RegistryWrite(theValueName, REG_BINARY, (uchar*) theValue, theLength);
}

void SexyAppBase::WriteToRegistry()
{
	RegistryWriteInteger("MusicVolume", (int) (mMusicVolume * 100));
	RegistryWriteInteger("SfxVolume", (int) (mSfxVolume * 100));
	RegistryWriteInteger("Muted", (mMuteCount - mAutoMuteCount > 0) ? 1 : 0);
	RegistryWriteInteger("ScreenMode", mIsWindowed ? 0 : 1);
	RegistryWriteInteger("PreferredX", mPreferredX);
	RegistryWriteInteger("PreferredY", mPreferredY);
	RegistryWriteInteger("CustomCursors", mCustomCursorsEnabled ? 1 : 0);		
	RegistryWriteInteger("InProgress", 0);
	RegistryWriteBoolean("WaitForVSync", mWaitForVSync);
}

bool SexyAppBase::RegistryEraseKey(const SexyString& _theKeyName)
{
	std::string theKeyName = SexyStringToStringFast(_theKeyName);
	if (mRegKey.length() == 0)
		return false;

	if (mPlayingDemoBuffer)
	{
		if (mManualShutdown)
			return true;

		PrepareDemoCommand(true);
		mDemoNeedsCommand = true;

		DBG_ASSERTE(!mDemoIsShortCmd);		
		DBG_ASSERTE(mDemoCmdNum == DEMO_REGISTRY_ERASE);

		return mDemoBuffer.ReadNumBits(1, false) != 0;		
	}
	
	std::string aKeyName = RemoveTrailingSlash("SOFTWARE\\" + mRegKey) + "\\" + theKeyName;

	//int aResult = RegDeleteKeyA(HKEY_CURRENT_USER, aKeyName.c_str());
	if (!mRegHandle->Erase(aKeyName))
	{
		if (mRecordingDemoBuffer)
		{
			WriteDemoTimingBlock();
			mDemoBuffer.WriteNumBits(0, 1);
			mDemoBuffer.WriteNumBits(DEMO_REGISTRY_ERASE, 5);
			mDemoBuffer.WriteNumBits(0, 1); // failure
		}

		return false;
	}		

	if (mRecordingDemoBuffer)
	{
		WriteDemoTimingBlock();
		mDemoBuffer.WriteNumBits(0, 1);
		mDemoBuffer.WriteNumBits(DEMO_REGISTRY_ERASE, 5);
		mDemoBuffer.WriteNumBits(1, 1); // success
	}

	return true;
}

void SexyAppBase::RegistryEraseValue(const SexyString& /*_theValueName*/)
{
	unreachable();
	/* TODO
	std::string theValueName = SexyStringToStringFast(_theValueName);
	if (mRegKey.length() == 0)
		return;

	HKEY aGameKey;	
	std::string aKeyName = RemoveTrailingSlash("SOFTWARE\\" + mRegKey);
	std::string aValueName;

	int aSlashPos = (int) theValueName.rfind('\\');
	if (aSlashPos != -1)
	{
		aKeyName += "\\" + theValueName.substr(0, aSlashPos);
		aValueName = theValueName.substr(aSlashPos + 1);
	}
	else
	{
		aValueName = theValueName;
	}

	int aResult = RegOpenKeyExA(HKEY_CURRENT_USER, aKeyName.c_str(), 0, KEY_WRITE, &aGameKey);
	if (aResult == ERROR_SUCCESS)
	{		
		RegDeleteValueA(aGameKey, aValueName.c_str());
		RegCloseKey(aGameKey);
	}*/
}

bool SexyAppBase::RegistryGetSubKeys(const std::string& /*theKeyName*/, StringVector* /*theSubKeys*/)
{
	unreachable();
	/* TODO
	theSubKeys->clear();

	if (mRegKey.length() == 0)
		return false;

	if (mPlayingDemoBuffer)
	{
		if (mManualShutdown)
			return true;

		PrepareDemoCommand(true);
		mDemoNeedsCommand = true;

		DBG_ASSERTE(!mDemoIsShortCmd);		
		DBG_ASSERTE(mDemoCmdNum == DEMO_REGISTRY_GETSUBKEYS);

		bool success = mDemoBuffer.ReadNumBits(1, false) != 0;
		if (!success)
			return false;

		int aNumKeys = mDemoBuffer.ReadLong();

		for (int i = 0; i < aNumKeys; i++)
			theSubKeys->push_back(mDemoBuffer.ReadString());

		return true;
	}
	else
	{
		HKEY aKey;
		
		std::string aKeyName = RemoveTrailingSlash(RemoveTrailingSlash("SOFTWARE\\" + mRegKey) + "\\" + theKeyName);	
		int aResult = RegOpenKeyExA(HKEY_CURRENT_USER, aKeyName.c_str(), 0, KEY_READ, &aKey);
		
		if (aResult == ERROR_SUCCESS)
		{		
			for (int anIdx = 0; ; anIdx++)
			{
				char aStr[1024];

				aResult = RegEnumKeyA(aKey, anIdx, aStr, 1024);
				if (aResult != ERROR_SUCCESS)
					break;

				theSubKeys->push_back(aStr);
			}

			RegCloseKey(aKey);

			if (mRecordingDemoBuffer)
			{
				WriteDemoTimingBlock();
				mDemoBuffer.WriteNumBits(0, 1);
				mDemoBuffer.WriteNumBits(DEMO_REGISTRY_GETSUBKEYS, 5);
				mDemoBuffer.WriteNumBits(1, 1); // success
				mDemoBuffer.WriteLong(theSubKeys->size());

				for (int i = 0; i < (int) theSubKeys->size(); i++)
					mDemoBuffer.WriteString((*theSubKeys)[i]);				
			}

			return true;
		}
		else
		{
			if (mRecordingDemoBuffer)
			{
				WriteDemoTimingBlock();
				mDemoBuffer.WriteNumBits(0, 1);
				mDemoBuffer.WriteNumBits(DEMO_REGISTRY_GETSUBKEYS, 5);
				mDemoBuffer.WriteNumBits(0, 1); // failure
			}

			return false;	
		}
	}*/
}


bool SexyAppBase::RegistryRead(const std::string& theValueName, uint32_t &theType, std::vector<uint8_t> &theValue, uint32_t &theLength)
{
	if (mRegKey.length() == 0)
		return false;

	if (mPlayingDemoBuffer)
	{
		if (mManualShutdown)
			return false;

		PrepareDemoCommand(true);
		mDemoNeedsCommand = true;
				
		DBG_ASSERTE(!mDemoIsShortCmd);		
		DBG_ASSERTE(mDemoCmdNum == DEMO_REGISTRY_READ);

		bool success = mDemoBuffer.ReadNumBits(1, false) != 0;
		if (!success)
			return false;

		theType = mDemoBuffer.ReadLong();

		uint32_t aLen = mDemoBuffer.ReadLong();
		theValue.resize(aLen);
		theLength = aLen;
		
		if (theLength >= aLen)
		{
			mDemoBuffer.ReadBytes(theValue.data(), aLen);
			return true;
		}
		else
		{
			for (int i = 0; i < (int) aLen; i++)
				mDemoBuffer.ReadByte();
			return false;
		}		
	}
	else
	{
		//HKEY aGameKey;

		std::string aKeyName = RemoveTrailingSlash("SOFTWARE\\" + mRegKey);
		std::string aValueName;

		int aSlashPos = (int) theValueName.rfind('\\');
		if (aSlashPos != -1)
		{
			aKeyName += "\\" + theValueName.substr(0, aSlashPos);
			aValueName = theValueName.substr(aSlashPos + 1);
		}
		else
		{
			aValueName = theValueName;
		}

		if(!mRegHandle->Read(theValueName, theType, theValue, theLength)) {
			if (mRecordingDemoBuffer)
			{
				WriteDemoTimingBlock();
				mDemoBuffer.WriteNumBits(0, 1);
				mDemoBuffer.WriteNumBits(DEMO_REGISTRY_READ, 5);
				mDemoBuffer.WriteNumBits(0, 1); // failure
			}
			return false;
		}

		if (mRecordingDemoBuffer)
		{
			WriteDemoTimingBlock();
			mDemoBuffer.WriteNumBits(0, 1);
			mDemoBuffer.WriteNumBits(DEMO_REGISTRY_READ, 5);
			mDemoBuffer.WriteNumBits(1, 1); // success
			mDemoBuffer.WriteLong(theType);
			mDemoBuffer.WriteLong(theLength);
			mDemoBuffer.WriteBytes(theValue.data(), theLength);
		}
		return true;
	}
}

bool SexyAppBase::RegistryReadString(const std::string& theKey, std::string* theString)
{
	std::vector<uint8_t>aStr;
	
	uint32_t aType;
	uint32_t aLen;
	if (!RegistryRead(theKey, aType, aStr, aLen))
		return false;

	if (aType != REG_SZ)
		return false;

	aStr.push_back('\0');

	*theString = (char *)aStr.data();
	return true;
}

bool SexyAppBase::RegistryReadInteger(const std::string& theKey, int* theValue)
{
	uint32_t aType;
	std::vector<uint8_t>aLong;
	uint32_t aLen;
	if (!RegistryRead(theKey, aType, aLong, aLen))
		return false;

	if (aType != REG_DWORD)
		return false;

	*theValue = *(int*)aLong.data();
	return true;
}

bool SexyAppBase::RegistryReadBoolean(const std::string& theKey, bool* theValue)
{
	int aValue;
	if (!RegistryReadInteger(theKey, &aValue))
		return false;
	
	*theValue = aValue != 0;
	return true;
}

bool SexyAppBase::RegistryReadData(const std::string &theKey, std::vector<uint8_t> &theValue, uint32_t &theLength)
{
	uint32_t aType;
	if (!RegistryRead(theKey, aType, theValue, theLength))
		return false;

	if (aType != REG_BINARY)
		return false;
	
	return true;
}

void SexyAppBase::ReadFromRegistry()
{
	mReadFromRegistry = true;
	mRegKey = SexyStringToString(GetString("RegistryKey", StringToSexyString(mRegKey)));

	if (mRegKey.length() == 0)
		return;

	int anInt;
	if (RegistryReadInteger("MusicVolume", &anInt))
		mMusicVolume = anInt / 100.0;
	
	if (RegistryReadInteger("SfxVolume", &anInt))
		mSfxVolume = anInt / 100.0;

	if (RegistryReadInteger("Muted", &anInt))
		mMuteCount = anInt;

	if (RegistryReadInteger("ScreenMode", &anInt))
		mIsWindowed = anInt == 0;

	RegistryReadInteger("PreferredX", &mPreferredX);
	RegistryReadInteger("PreferredY", &mPreferredY);	

	if (RegistryReadInteger("CustomCursors", &anInt))
		EnableCustomCursors(anInt != 0);	
			
	RegistryReadBoolean("WaitForVSync", &mWaitForVSync);	

	if (RegistryReadInteger("InProgress", &anInt))
		mLastShutdownWasGraceful = anInt == 0;

	if (!IsScreenSaver()) {
		RegistryWriteInteger("InProgress", 1);
	}
}

bool SexyAppBase::WriteBytesToFile(const std::string& theFileName, const void *theData, unsigned long theDataLen)
{
	if (mPlayingDemoBuffer)
	{
		if (mManualShutdown)
			return true;

		PrepareDemoCommand(true);
		mDemoNeedsCommand = true;

		DBG_ASSERTE(!mDemoIsShortCmd);
		DBG_ASSERTE(mDemoCmdNum == DEMO_FILE_WRITE);

		bool success = mDemoBuffer.ReadNumBits(1, false) != 0;
		if (!success)
			return false;

		return true;
	}

	MkDir(GetFileDir(theFileName));
	FILE* aFP = fopen(theFileName.c_str(), "w+b");

	if (aFP == NULL)
	{
		if (mRecordingDemoBuffer)
		{
			WriteDemoTimingBlock();
			mDemoBuffer.WriteNumBits(0, 1);
			mDemoBuffer.WriteNumBits(DEMO_FILE_WRITE, 5);
			mDemoBuffer.WriteNumBits(0, 1); // failure
		}

		return false;
	}

	fwrite(theData, 1, theDataLen, aFP);
	fclose(aFP);

	if (mRecordingDemoBuffer)
	{
		WriteDemoTimingBlock();
		mDemoBuffer.WriteNumBits(0, 1);
		mDemoBuffer.WriteNumBits(DEMO_FILE_WRITE, 5);
		mDemoBuffer.WriteNumBits(1, 1); // success
	}

	if (mSexyCacheBuffers && mWriteToSexyCache)
	{
		GetFullPath(theFileName);
		/*
		unsigned char* aSetData = (unsigned char*)gSexyCache->AllocSetData(GetFullPath(theFileName), "Buffer", theDataLen + 1);
		if (aSetData)
		{
			*aSetData = 1;
			memcpy(aSetData, theData, theDataLen);
			gSexyCache->SetData(aSetData);
			gSexyCache->FreeSetData(aSetData);
			gSexyCache->SetFileDeps(GetFullPath(theFileName), "Buffer", GetFullPath(theFileName));
		}
		*/
	}

	return true;
}

bool SexyAppBase::WriteBufferToFile(const std::string& theFileName, const Buffer* theBuffer)
{
	return WriteBytesToFile(theFileName,theBuffer->GetDataPtr(),theBuffer->GetDataLen());
}


bool SexyAppBase::ReadBufferFromFile(const std::string& theFileName, Buffer* theBuffer, bool dontWriteToDemo)
{
	if ((mPlayingDemoBuffer) && (!dontWriteToDemo))
	{
		if (mManualShutdown)
			return false;

		PrepareDemoCommand(true);
		mDemoNeedsCommand = true;
		
		DBG_ASSERTE(!mDemoIsShortCmd);		
		DBG_ASSERTE(mDemoCmdNum == DEMO_FILE_READ);

		bool success = mDemoBuffer.ReadNumBits(1, false) != 0;
		if (!success)
			return false;

		ulong aLen = mDemoBuffer.ReadLong();		
				
		theBuffer->Clear();
		for (int i = 0; i < (int) aLen; i++)
			theBuffer->WriteByte(mDemoBuffer.ReadByte());

		return true;		
	}
	else
	{
		PFILE* aFP = p_fopen(theFileName.c_str(), "rb");

		if (aFP == NULL)
		{
			if ((mRecordingDemoBuffer) && (!dontWriteToDemo))
			{
				WriteDemoTimingBlock();
				mDemoBuffer.WriteNumBits(0, 1);
				mDemoBuffer.WriteNumBits(DEMO_FILE_READ, 5);
				mDemoBuffer.WriteNumBits(0, 1); // failure				
			}

			return false;
		}
		
		p_fseek(aFP, 0, SEEK_END);
		int aFileSize = p_ftell(aFP);
		p_fseek(aFP, 0, SEEK_SET);
		
		uchar* aData = new uchar[aFileSize];

		p_fread(aData, 1, aFileSize, aFP);
		p_fclose(aFP);

		theBuffer->Clear();
		theBuffer->SetData(aData, aFileSize);

		if ((mRecordingDemoBuffer) && (!dontWriteToDemo))
		{
			WriteDemoTimingBlock();
			mDemoBuffer.WriteNumBits(0, 1);
			mDemoBuffer.WriteNumBits(DEMO_FILE_READ, 5);
			mDemoBuffer.WriteNumBits(1, 1); // success			
			mDemoBuffer.WriteLong(aFileSize);
			mDemoBuffer.WriteBytes(aData, aFileSize);
		}

		delete [] aData;

		return true;
	}
}

bool SexyAppBase::FileExists(const std::string& theFileName)
{
	if (mPlayingDemoBuffer)
	{
		if (mManualShutdown)
			return true;

		PrepareDemoCommand(true);
		mDemoNeedsCommand = true;
		
		DBG_ASSERTE(!mDemoIsShortCmd);		
		DBG_ASSERTE(mDemoCmdNum == DEMO_FILE_EXISTS);		

		bool success = mDemoBuffer.ReadNumBits(1, false) != 0;		
		return success;
	}
	else
	{
		PFILE* aFP = p_fopen(theFileName.c_str(), "rb");

		if (mRecordingDemoBuffer)
		{
			WriteDemoTimingBlock();
			mDemoBuffer.WriteNumBits(0, 1);
			mDemoBuffer.WriteNumBits(DEMO_FILE_EXISTS, 5);
			mDemoBuffer.WriteNumBits((aFP != NULL) ? 1 : 0, 1); 
		}

		if (aFP == NULL)		
			return false;		
		
		p_fclose(aFP);
		return true;
	}
}

bool SexyAppBase::EraseFile(const std::string& theFileName)
{
	if (mPlayingDemoBuffer)
		return true;

	return std::filesystem::remove(theFileName);
}

/*
void SexyAppBase::SEHOccured()
{
	SetMusicVolume(0);
	::ShowWindow(mHWnd, SW_HIDE);
	mSEHOccured = true;
	EnforceCursor();
}*/
/*
std::string SexyAppBase::GetGameSEHInfo()
{
	int aSecLoaded = (GetTickCount() - mTimeLoaded) / 1000;

	char aTimeStr[16];
	sprintf(aTimeStr, "%02d:%02d:%02d", (aSecLoaded/60/60), (aSecLoaded/60)%60, aSecLoaded%60);
	
	char aThreadIdStr[16];
	sprintf(aThreadIdStr, "%lX", mPrimaryThreadId);

	std::string anInfoString = 
		"Product: " + mProdName + "\r\n" +		
		"Version: " + mProductVersion + "\r\n";			

	anInfoString +=
		"Time Loaded: " + std::string(aTimeStr) + "\r\n"
		"Fullscreen: " + (mIsWindowed ? std::string("No") : std::string("Yes")) + "\r\n"
		"Primary ThreadId: " + aThreadIdStr + "\r\n";	

	return anInfoString;						
}*/


void SexyAppBase::GetSEHWebParams(DefinesMap*){}

void SexyAppBase::ShutdownHook()
{
}

void SexyAppBase::Shutdown()
{
	if (/*(mPrimaryThreadId != 0) && */(std::this_thread::get_id() != mPrimaryThreadId))
	{
		mLoadingFailed = true;
	}
	else if (!mShutdown)
	{
		mExitToTop = true;
		mShutdown = true;
		ShutdownHook();

		if (mPlayingDemoBuffer)
		{
			//if the music/sfx volume is 0, then it means that in playback
			//someone pressed the "S" key to mute sounds (or that the 
			//sound volume was set to 0 in the first place). Out of politeness,
			//return the system sound volume to what it last was in the game.
			SetMusicVolume(mDemoMusicVolume);
			SetSfxVolume(mDemoSfxVolume);
		}

		/*
		// Blah
		while (mCursorThreadRunning)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			//Sleep(10);
		}*/
		
		if (mMusicInterface != NULL)
			mMusicInterface->StopAllMusic();

		//RestoreScreenResolution();

		if (mReadFromRegistry)
			WriteToRegistry();

		//ImageLib::CloseJPEG2000();
	}
}

void SexyAppBase::RestoreScreenResolution()
{
	if (mFullScreenWindow)
	{
		unreachable();
		/* TODO
		EnumWindows(ChangeDisplayWindowEnumProc,0); // get any windows that appeared while we were running
		ChangeDisplaySettings(NULL,0);
		EnumWindows(ChangeDisplayWindowEnumProc,1); // restore window pos
		mFullScreenWindow = false;*/
	}
}

void SexyAppBase::DoExit(int theCode)
{
	RestoreScreenResolution();
	exit(theCode);
}

void SexyAppBase::UpdateFrames()
{
	mUpdateCount++;

	if (!mMinimized)
	{
		if (mWidgetManager->UpdateFrame())
			++mFPSDirtyCount;
	}

	mMusicInterface->Update();

	static bool has_shown = false;
	if (!has_shown) {
		printf("warning:  The image cleanup logic is probably busted since the app uses refrence counts\n");
		has_shown = true;
	}
	// TODO
	//CleanSharedImages();
}

void SexyAppBase::DoUpdateFrames()
{
	SEXY_AUTO_PERF("SexyAppBase::DoUpdateFrames");

	if ((mLoadingThreadCompleted) && (!mLoaded))
	{
		mLoaded = true;
		mYieldMainThread = false;
		LoadingThreadCompleted();
	}

	UpdateFrames();
}

void SexyAppBase::Redraw()
{
	SEXY_AUTO_PERF("SexyAppBase::Redraw");

	// Do mIsDrawing check because we could enter here at a bad time if any windows messages
	//  are processed during WidgetManager->Draw
	if ((mIsDrawing) || (mShutdown))
		return;

	if (gScreenSaverActive)
		return;

	mWindowInterface->Draw();

	/*==================================================*
	 |     Here lies a bunch of error recovery code     |
	 |    Their strategy was to  recover from errors    |
	 |         Mine is to hope they never happen        |
	 *==================================================*/

	mFPSFlipCount++;
}

///////////////////////////// FPS Stuff
//static PerfTimer gFPSTimer;
static int gFrameCount;
//static int gFPSDisplay;
//static bool gForceDisplay = false;
static void CalculateFPS()
{
	gFrameCount++;

	unreachable();
	/* TODO
	static SysFont aFont(gSexyAppBase,"Tahoma",8);
	if (gFPSImage==NULL)
	{
		gFPSImage = new DDImage(gSexyAppBase->mDDInterface);
		gFPSImage->Create(50,aFont.GetHeight()+4);
		gFPSImage->SetImageMode(false,false);
		gFPSImage->SetVolatile(true);
		gFPSImage->mPurgeBits = false;
		gFPSImage->mWantDDSurface = true;
		gFPSImage->PurgeBits();
	}

	if (gFPSTimer.GetDuration() >= 1000 || gForceDisplay)
	{
		gFPSTimer.Stop();
		if (!gForceDisplay)
			gFPSDisplay = (int)(gFrameCount*1000/gFPSTimer.GetDuration() + 0.5f);
		else
		{
			gForceDisplay = false;
			gFPSDisplay = 0;
		}

		gFPSTimer.Start();
		gFrameCount = 0;

		Graphics aDrawG(gFPSImage);
		aDrawG.SetFont(&aFont);
		SexyString aFPS = StrFormat(_S("FPS: %d"), gFPSDisplay);
		aDrawG.SetColor(0x000000);
		aDrawG.FillRect(0,0,gFPSImage->GetWidth(),gFPSImage->GetHeight());
		aDrawG.SetColor(0xFFFFFF);
		aDrawG.DrawString(aFPS,2,aFont.GetAscent());
		//gFPSImage->mKeepBits = false;
		//gFPSImage->GenerateDDSurface();
		gFPSImage->mBitsChangedCount++;
	}*/
}

///////////////////////////// FPS Stuff to draw mouse coords
static void FPSDrawCoords(int /*theX*/, int /*theY*/)
{
	unreachable();
	/* TODO
	static SysFont aFont(gSexyAppBase,"Tahoma",8);
	if (gFPSImage==NULL)
	{
		gFPSImage = new DDImage(gSexyAppBase->mDDInterface);
		gFPSImage->Create(50,aFont.GetHeight()+4);
		gFPSImage->SetImageMode(false,false);
		gFPSImage->SetVolatile(true);
		gFPSImage->mPurgeBits = false;
		gFPSImage->mWantDDSurface = true;
		gFPSImage->PurgeBits();
	}

	Graphics aDrawG(gFPSImage);
	aDrawG.SetFont(&aFont);
	SexyString aFPS = StrFormat(_S("%d,%d"),theX,theY);
	aDrawG.SetColor(0x000000);
	aDrawG.FillRect(0,0,gFPSImage->GetWidth(),gFPSImage->GetHeight());
	aDrawG.SetColor(0xFFFFFF);
	aDrawG.DrawString(aFPS,2,aFont.GetAscent());	
	gFPSImage->mBitsChangedCount++;*/
}

///////////////////////////// Demo TimeLeft Stuff
//static DDImage* gDemoTimeLeftImage = NULL;
static void CalculateDemoTimeLeft()
{
	unreachable();
	/* TODO
	static SysFont aFont(gSexyAppBase,"Tahoma",8);
	static DWORD aLastTick = 0;

	if (gDemoTimeLeftImage==NULL)
	{
		gDemoTimeLeftImage = new DDImage(gSexyAppBase->mDDInterface);
		gDemoTimeLeftImage->Create(50,aFont.GetHeight()+4);
		gDemoTimeLeftImage->SetImageMode(false,false);
		gDemoTimeLeftImage->SetVolatile(true);
		gDemoTimeLeftImage->mPurgeBits = false;
		gDemoTimeLeftImage->mWantDDSurface = true;
		gDemoTimeLeftImage->PurgeBits();
	}

	DWORD aTick = GetTickCount();
	if (aTick - aLastTick < 1000/gSexyAppBase->mUpdateMultiplier)
		return;

	aLastTick = aTick;

	int aNumUpdatesLeft = gSexyAppBase->mDemoLength - gSexyAppBase->mUpdateCount;
	Graphics aDrawG(gDemoTimeLeftImage);
	aDrawG.SetFont(&aFont);

	int aTotalSeconds = aNumUpdatesLeft*gSexyAppBase->mFrameTime/1000;
	int aSeconds = aTotalSeconds%60;
	int aMinutes = (aTotalSeconds/60)%60;
	int anHours = (aTotalSeconds/3600);

	SexyString aFPS = StrFormat(_S("%02d:%02d:%02d"), anHours,aMinutes,aSeconds);
	aDrawG.SetColor(0x000000);
	aDrawG.FillRect(0,0,gDemoTimeLeftImage->GetWidth(),gDemoTimeLeftImage->GetHeight());
	aDrawG.SetColor(0xFFFFFF);
	aDrawG.DrawString(aFPS,2,aFont.GetAscent());	
	gDemoTimeLeftImage->mBitsChangedCount++;*/
}

static void UpdateScreenSaverInfo(std::chrono::high_resolution_clock::time_point /*theTick*/)
{
	if (gSexyAppBase->IsScreenSaver() || !gSexyAppBase->mIsPhysWindowed)
		return;

	static bool has_shown = false;
	if (!has_shown) {
		printf("warning:  UpdateScreenSaverInfo is a stub\n");
		has_shown = true;
	}
	/* TODO (?? I mean, we're not using screensavers)
	// Get screen saver timeout		
	static DWORD aPeriodicTick = 0;
	static DWORD aScreenSaverTimeout = 60000;
	static BOOL aScreenSaverEnabled = TRUE;

	if (theTick-aPeriodicTick > 10000)
	{
		aPeriodicTick = theTick;

		int aTimeout = 0;

		SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT,0,&aTimeout,0);
		SystemParametersInfo(SPI_GETSCREENSAVEACTIVE,0,&aScreenSaverEnabled,0);
		aTimeout-=2;

		if (aTimeout < 1)
			aTimeout = 1;

		aScreenSaverTimeout = aTimeout*1000;

		if (!aScreenSaverEnabled)
			gScreenSaverActive = false;
	}

	// Get more accurate last user input time
	if (gGetLastInputInfoFunc)
	{
		LASTINPUTINFO anInfo;
		anInfo.cbSize = sizeof(anInfo);
		if (gGetLastInputInfoFunc(&anInfo))
		{
			if (anInfo.dwTime > theTick)
				anInfo.dwTime = theTick;

			gSexyAppBase->mLastUserInputTick = anInfo.dwTime;
		}
	}

	if (!aScreenSaverEnabled)
		return;

	DWORD anIdleTime = theTick - gSexyAppBase->mLastUserInputTick;
	if (gScreenSaverActive)
	{
		BOOL aBool = FALSE;
		if (SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &aBool, 0))
		{
			if (aBool) // screen saver not off yet
				return;
		}

		if (anIdleTime < aScreenSaverTimeout)
		{
			gScreenSaverActive = false;
			gSexyAppBase->mWidgetManager->MarkAllDirty();
		}
	}
	else if (anIdleTime > aScreenSaverTimeout)
		gScreenSaverActive = true;*/
}

bool SexyAppBase::DrawDirtyStuff()
{
	SEXY_AUTO_PERF("SexyAppBase::DrawDirtyStuff");
	MTAutoDisallowRand aDisallowRand;

	/* gNeverFails
	if (gIsFailing) // just try to reinit
	{
		Redraw(NULL);
		mHasPendingDraw = false;
		mLastDrawWasEmpty = true;		
		return false;
	}*/

	if (mShowFPS)
	{
		switch(mShowFPSMode)
		{
			case FPS_ShowFPS: CalculateFPS(); break;
			case FPS_ShowCoords:
				if (mWidgetManager!=NULL)
					FPSDrawCoords(mWidgetManager->mLastMouseX, mWidgetManager->mLastMouseY); 
				break;
		}

		if (mPlayingDemoBuffer)
			CalculateDemoTimeLeft();
	}

	auto aStartTime = std::chrono::high_resolution_clock::now();

	// Update user input and screen saver info
	static auto aPeriodicTick = aStartTime;
	if (aStartTime-aPeriodicTick > std::chrono::milliseconds(1000))
	{
		aPeriodicTick = aStartTime;
		UpdateScreenSaverInfo(aStartTime);
	}

	if (gScreenSaverActive)
	{
		//mHasPendingDraw = false;
		mLastDrawWasEmpty = true;		
		return false;
	}

	mIsDrawing = true;
	//auto wait_time = std::chrono::high_resolution_clock::now();
	bool drewScreen = mWidgetManager->DrawScreen();
	//std::cout << "update frames took: " << (std::chrono::high_resolution_clock::now() - wait_time)/std::chrono::duration<double>(1/60.0) << " frames."<< std::endl;
	mIsDrawing = false;

	if ((drewScreen || (aStartTime - mLastDrawTick >= std::chrono::milliseconds(1000)) || (mCustomCursorDirty)) &&
		((aStartTime - mNextDrawTick) >= std::chrono::milliseconds(0)))
	{
		mLastDrawWasEmpty = false;

		mDrawCount++;		

		auto aMidTime = std::chrono::high_resolution_clock::now();

		mFPSCount++;
		mFPSTime += aMidTime - aStartTime;

		mDrawTime += aMidTime - aStartTime;

		if (mShowFPS)
		{
			unreachable();
			/* TODO
			Graphics g(mDDInterface->GetScreenImage());
			g.DrawImage(gFPSImage,mWidth-gFPSImage->GetWidth()-10,mHeight-gFPSImage->GetHeight()-10);
		
			if (mPlayingDemoBuffer)
				g.DrawImage(gDemoTimeLeftImage,mWidth-gDemoTimeLeftImage->GetWidth()-10,mHeight-gFPSImage->GetHeight()-gDemoTimeLeftImage->GetHeight()-15);
			*/
		}

		if (mWaitForVSync && mIsPhysWindowed && mSoftVSyncWait)
		{
			unreachable();
			/* TODO
			auto aTick = std::chrono::high_resolution_clock::now();
			if (aTick-mLastDrawTick < mDDInterface->mMillisecondsPerFrame)
				Sleep(mDDInterface->mMillisecondsPerFrame - (aTick-mLastDrawTick));
			*/
		}

		auto aPreScreenBltTime = std::chrono::high_resolution_clock::now();
		mLastDrawTick = aPreScreenBltTime;

		//mWindowInterface->GetScreenImage()->FillRect({0,0,mWidth,mHeight}, Color(255, 0, 255, 128), 0);
		Redraw();


		auto aEndTime = std::chrono::high_resolution_clock::now();

		mScreenBltTime = aEndTime - aPreScreenBltTime;

#ifdef _DEBUG
		/*if (mFPSTime >= 5000) // Show FPS about every 5 seconds
		{
			ulong aTickNow = GetTickCount();

			OutputDebugString(StrFormat(_S("Theoretical FPS: %d\r\n"), (int) (mFPSCount * 1000 / mFPSTime)).c_str());
			OutputDebugString(StrFormat(_S("Actual      FPS: %d\r\n"), (mFPSFlipCount * 1000) / max((aTickNow - mFPSStartTick), 1)).c_str());
			OutputDebugString(StrFormat(_S("Dirty Rate     : %d\r\n"), (mFPSDirtyCount * 1000) / max((aTickNow - mFPSStartTick), 1)).c_str());

			mFPSTime = 0;
			mFPSCount = 0;
			mFPSFlipCount = 0;
			mFPSStartTick = aTickNow;
			mFPSDirtyCount = 0;
		}*/
#endif

		if ((mLoadingThreadStarted) && (!mLoadingThreadCompleted))
		{
			auto aTotalTime = aEndTime - aStartTime;


			mNextDrawTick += std::chrono::milliseconds(35) + ((aTotalTime > std::chrono::milliseconds(15))?aTotalTime:std::chrono::milliseconds(15));

			if ((aEndTime - mNextDrawTick) >= std::chrono::milliseconds(0))			
				mNextDrawTick = aEndTime;			

			/*char aStr[256];
			sprintf(aStr, "Next Draw Time: %d\r\n", mNextDrawTick);
			OutputDebugString(aStr);*/
		}
		else
			mNextDrawTick = aEndTime;

		mCustomCursorDirty = false;

		return true;
	}
	else
	{
		mLastDrawWasEmpty = true;
		return false;
	}
}

void SexyAppBase::LogScreenSaverError(const std::string &/*theError*/)
{
	unreachable();
	/* TODO
	static bool firstTime = true;
	char aBuf[512];

	const char *aFlag = firstTime?"w":"a+";
	firstTime = false;

	FILE *aFile = fopen("ScrError.txt",aFlag);
	if (aFile != NULL)
	{
		fprintf(aFile,"%s %s %lu\n",theError.c_str(),_strtime(aBuf),GetTickCount());
		fclose(aFile);
	}*/
}

void SexyAppBase::BeginPopup()
{
	if (!mIsPhysWindowed)
	{
		unreachable();
		/* TODO
		if (mDDInterface && mDDInterface->mDD)
		{
			mDDInterface->mDD->FlipToGDISurface();
			mNoDefer = true;
		}
		*/
	}
}

void SexyAppBase::EndPopup()
{
	if (!mIsPhysWindowed)
		mNoDefer = false;

	//ClearUpdateBacklog();
	ClearKeysDown();

	if (mWidgetManager->mDownButtons)
	{
		mWidgetManager->DoMouseUps();
		unreachable(); // TODO
		// ReleaseCapture();
	}
}

/*
int SexyAppBase::MsgBox(const std::string& theText, const std::string& theTitle, int theFlags)
{
//	if (mDDInterface && mDDInterface->mDD)
//		mDDInterface->mDD->FlipToGDISurface();
	if (IsScreenSaver())
	{
		LogScreenSaverError(theText);
		return IDOK;
	}

	BeginPopup();
	int aResult = MessageBoxA(mHWnd, theText.c_str(), theTitle.c_str(), theFlags);
	EndPopup();

	return aResult;
}*/

/*
int SexyAppBase::MsgBox(const std::wstring& theText, const std::wstring& theTitle, int theFlags)
{
//	if (mDDInterface && mDDInterface->mDD)
//		mDDInterface->mDD->FlipToGDISurface();
	if (IsScreenSaver())
	{
		LogScreenSaverError(WStringToString(theText));
		return IDOK;
	}

	BeginPopup();
	int aResult = MessageBoxW(mHWnd, theText.c_str(), theTitle.c_str(), theFlags);
	EndPopup();

	return aResult;
}*/

void SexyAppBase::Popup(const std::string& theString)
{
	if (IsScreenSaver())
	{
		LogScreenSaverError(theString);
		return;
	}

	BeginPopup();
	if (!mShutdown)
		printf("%s: %s\n",SexyStringToString(GetString("FATAL_ERROR", _S("FATAL ERROR"))).c_str(), theString.c_str());
		//::MessageBoxA(mHWnd, theString.c_str(), SexyStringToString(GetString("FATAL_ERROR", _S("FATAL ERROR"))).c_str(), MB_APPLMODAL | MB_ICONSTOP);
	EndPopup();
}

void SexyAppBase::Popup(const std::wstring& theString)
{
	if (IsScreenSaver())
	{
		LogScreenSaverError(WStringToString(theString));
		return;
	}

	BeginPopup();
	if (!mShutdown)
		printf("%ls: %ls\n",SexyStringToWString(GetString("FATAL_ERROR", _S("FATAL ERROR"))).c_str(), theString.c_str());
		//::MessageBoxW(mHWnd, theString.c_str(), SexyStringToWString(GetString("FATAL_ERROR", _S("FATAL ERROR"))).c_str(), MB_APPLMODAL | MB_ICONSTOP);
	EndPopup();
}

void SexyAppBase::SafeDeleteWidget(Widget* theWidget)
{
	WidgetSafeDeleteInfo aWidgetSafeDeleteInfo;
	aWidgetSafeDeleteInfo.mUpdateAppDepth = mUpdateAppDepth;
	aWidgetSafeDeleteInfo.mWidget = theWidget;
	mSafeDeleteList.push_back(aWidgetSafeDeleteInfo);
}

/*
BOOL CALLBACK EnumCloseThing2(HWND hwnd, LPARAM lParam)
{
	//CloseWindow(hwnd);
	char aClassName[256];
	if (GetClassNameA(hwnd, aClassName, 256) != 0)
	{
		if (strcmp(aClassName, "Internet Explorer_Server") == 0)
		{
			DestroyWindow(hwnd);			
		}
		else
		{
			EnumChildWindows(hwnd, EnumCloseThing2, lParam);
		}
	}

	return TRUE;
}*/

/*
BOOL CALLBACK EnumCloseThing(HWND hwnd, LPARAM lParam)
{
	//CloseWindow(hwnd);
	char aClassName[256];
	if (GetClassNameA(hwnd, aClassName, 256) != 0)
	{
		if (strcmp(aClassName, "AmWBC_WClass") == 0)
		{
			EnumChildWindows(hwnd, EnumCloseThing2, lParam);
		}
	}

	return TRUE;
}*/

/*
static INT_PTR CALLBACK MarkerListDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	(void)lParam;
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			HWND aListBox = GetDlgItem(hwnd,100);

			DWORD       dwExtent = 0;
			HDC         hDCListBox;
			HFONT       hFontOld, hFontNew;
			TEXTMETRIC  tm; 
			RECT aRect;
			SIZE aSize;

			hDCListBox = GetDC(aListBox); 
			hFontNew = (HFONT)SendMessage(aListBox, WM_GETFONT, 0, 0); 
			hFontOld = (HFONT)SelectObject(hDCListBox, hFontNew); 
			GetTextMetrics(hDCListBox, (LPTEXTMETRIC)&tm); 
			GetClientRect(hwnd, &aRect);
			MoveWindow(aListBox,10,10,aRect.right-aRect.left-20,aRect.bottom-aRect.top-20,FALSE);
			for (SexyAppBase::DemoMarkerList::iterator anItr = gSexyAppBase->mDemoMarkerList.begin(); anItr != gSexyAppBase->mDemoMarkerList.end(); ++anItr)
			{
				if (anItr->second <= gSexyAppBase->mUpdateCount)
					continue;

				int aTotalSeconds = (gSexyAppBase->mDemoLength - anItr->second)*gSexyAppBase->mFrameTime/1000;
				int aSeconds = aTotalSeconds%60;
				int aMinutes = (aTotalSeconds/60)%60;
				int anHours = (aTotalSeconds/3600);

				SexyString aStr = StrFormat(_S("%s (%02d:%02d:%02d)"), anItr->first.c_str(),anHours,aMinutes,aSeconds);				
				GetTextExtentPoint32(hDCListBox, aStr.c_str(), aStr.length(), &aSize);
				dwExtent = std::max (aSize.cx + tm.tmAveCharWidth, (LONG)dwExtent);
				SendMessage(aListBox, LB_SETHORIZONTALEXTENT, dwExtent, 0);
				LRESULT anIndex = SendMessage(aListBox, LB_ADDSTRING, 0, (LPARAM)aStr.c_str());
				SendMessage(aListBox, LB_SETITEMDATA, anIndex, anItr->second);
			}
	
			SelectObject(hDCListBox, hFontOld);
			ReleaseDC(aListBox, hDCListBox); 

			return TRUE;
		}

		case WM_CLOSE:
			EndDialog(hwnd,0);
			return TRUE;

		case WM_COMMAND:
			if (HIWORD(wParam)==LBN_DBLCLK)
			{
				HWND aListBox = GetDlgItem(hwnd,100);

				int anIndex = SendMessage(aListBox,LB_GETCURSEL,0,0);
				if (anIndex >= 0)
				{
					int anUpdateTime = SendMessage(aListBox,LB_GETITEMDATA,anIndex,0);
					if (anUpdateTime > gSexyAppBase->mUpdateCount)
					{
						gSexyAppBase->mFastForwardToUpdateNum = anUpdateTime;
						EndDialog(hwnd,0);
					}
				}
				return TRUE;
			}
			break;

	}

	return FALSE;
}*/

typedef WORD* LPWORD;
[[maybe_unused]]
static LPWORD lpdwAlign ( LPWORD lpIn)
{
    intptr_t ul;

    ul = (intptr_t)lpIn;
    ul +=3;
    ul >>=2;
    ul <<=2;
    return (LPWORD)ul;
}

[[maybe_unused]]
static int ListDemoMarkers()
{
	unreachable();
	/* TODO
	HGLOBAL hgbl;
    LPDLGTEMPLATE lpdt;
    LPDLGITEMTEMPLATE lpdit;
    LPWORD lpw;
    LPWSTR lpwsz;
    LRESULT ret;
    int nchar;

    hgbl = GlobalAlloc(GMEM_ZEROINIT, 1024);
    if (!hgbl)
        return -1;
 
    lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);
 
    // Define a dialog box. 
    lpdt->style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION | DS_SETFONT;
    lpdt->cdit = 1;  // number of controls
    lpdt->x  = 10;  lpdt->y  = 10;
    lpdt->cx = 200; lpdt->cy = 200;

    lpw = (LPWORD) (lpdt + 1);
    *lpw++ = 0;   // no menu
    *lpw++ = 0;   // predefined dialog box class (by default)

    lpwsz = (LPWSTR) lpw;
    nchar = MultiByteToWideChar (CP_ACP, 0, "Marker List", -1, lpwsz, 50);
    lpw   += nchar;
	*lpw++ = 8;
	lpwsz = (LPWSTR) lpw;
    nchar = MultiByteToWideChar (CP_ACP, 0, "Tahoma", -1, lpwsz, 50);
	lpw += nchar;

	// Define Listbox
    lpw = lpdwAlign (lpw); // align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE) lpw;
    lpdit->x  = 5; lpdit->y  = 5;
    lpdit->cx = 190; lpdit->cy = 195;
    lpdit->id = 100;  
	lpdit->style = WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL | LBS_NOTIFY;
	lpdit->dwExtendedStyle = WS_EX_CLIENTEDGE;
    lpw = (LPWORD) (lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0083;    // listbox class
	*lpw++ = 0;			// no window text
    *lpw++ = 0;			// no creation data


    GlobalUnlock(hgbl); 
    ret = DialogBoxIndirect(gHInstance, (LPDLGTEMPLATE) hgbl, gSexyAppBase->mHWnd, (DLGPROC) MarkerListDialogProc); 
    GlobalFree(hgbl); 

	gSexyAppBase->mLastTime = timeGetTime();

    return ret;*/
}

/*
static INT_PTR CALLBACK JumpToTimeDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	(void)lParam;
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			HWND anEdit = GetDlgItem(hwnd,100);
			HKEY aGameKey;
			std::string aKeyName = RemoveTrailingSlash("SOFTWARE\\" + gSexyAppBase->mRegKey);
			if (RegOpenKeyExA(HKEY_CURRENT_USER, aKeyName.c_str(), 0, KEY_READ | KEY_WRITE, &aGameKey) == ERROR_SUCCESS)
			{
				char aBuf[1024];
				DWORD aLength = 1000;
				DWORD aType = REG_SZ;
				if (RegQueryValueExA(aGameKey, "DemoJumpTime", 0, &aType, (uchar*) aBuf, &aLength) == ERROR_SUCCESS)
				{
					aBuf[aLength] = 0;
					SetWindowTextA(anEdit,aBuf);
					SendMessage(anEdit,EM_SETSEL,0,-1);
				}
				RegCloseKey(aGameKey);
			}
			return TRUE;
		}
		break;

		case WM_CLOSE:
			EndDialog(hwnd,0);
			return TRUE;

		case WM_COMMAND:
			if (HIWORD(wParam)==BN_CLICKED)
			{
				if (LOWORD(wParam)==IDOK)
				{
					char aBuf[512];
					HWND anEdit = GetDlgItem(hwnd,100);
					GetWindowTextA(anEdit,aBuf,500);

					HKEY aGameKey;
					std::string aKeyName = RemoveTrailingSlash("SOFTWARE\\" + gSexyAppBase->mRegKey);
					if (RegOpenKeyExA(HKEY_CURRENT_USER, aKeyName.c_str(), 0, KEY_READ | KEY_WRITE, &aGameKey) == ERROR_SUCCESS)
					{
						RegSetValueExA(aGameKey, "DemoJumpTime", 0, REG_SZ, (const BYTE*)aBuf, strlen(aBuf)+1);
						RegCloseKey(aGameKey);
					}

					int aTime = 0;
					char *aPtr = strtok(aBuf,":");
					while (aPtr != NULL)
					{
						aTime *= 60;
						aTime += atoi(aPtr);
						aPtr = strtok(NULL,":");
					}
					aTime++;

					int aNumFrames = aTime*1000/gSexyAppBase->mFrameTime;
					gSexyAppBase->mFastForwardToUpdateNum = gSexyAppBase->mDemoLength - aNumFrames;					


				}
					
				EndDialog(hwnd,0);
				return TRUE;
			}
			break;
	}

	return FALSE;
}*/

[[maybe_unused]]
static int DemoJumpToTime()
{
	unreachable();
	/* TODO
	HGLOBAL hgbl;
    LPDLGTEMPLATE lpdt;
    LPDLGITEMTEMPLATE lpdit;
    LPWORD lpw;
    LPWSTR lpwsz;
    LRESULT ret;
    int nchar;

    hgbl = GlobalAlloc(GMEM_ZEROINIT, 1024);
    if (!hgbl)
        return -1;
 
    lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);
 
    // Define a dialog box. 
    lpdt->style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION | DS_SETFONT;
    lpdt->cdit = 3;  // number of controls
    lpdt->x  = 10;  lpdt->y  = 10;
    lpdt->cx = 200; lpdt->cy = 50;

    lpw = (LPWORD) (lpdt + 1);
    *lpw++ = 0;   // no menu
    *lpw++ = 0;   // predefined dialog box class (by default)

    lpwsz = (LPWSTR) lpw;
    nchar = MultiByteToWideChar (CP_ACP, 0, "Jump To Time", -1, lpwsz, 50);
    lpw   += nchar;
	*lpw++ = 8;
	lpwsz = (LPWSTR) lpw;
    nchar = MultiByteToWideChar (CP_ACP, 0, "Tahoma", -1, lpwsz, 50);
	lpw += nchar;

	// Define Edit
    lpw = lpdwAlign (lpw); // align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE) lpw;
    lpdit->x  = 5; lpdit->y  = 5;
    lpdit->cx = 190; lpdit->cy = 15;
    lpdit->id = 100;  
	lpdit->style = WS_VISIBLE | WS_CHILD;
	lpdit->dwExtendedStyle = WS_EX_CLIENTEDGE;
    lpw = (LPWORD) (lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0081;    // edit class
	*lpw++ = 0;			// no window text
    *lpw++ = 0;			// no creation data

	// Define Button
    lpw = lpdwAlign (lpw); // align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE) lpw;
    lpdit->x  = 30; lpdit->y  = 25;
    lpdit->cx = 60; lpdit->cy = 15;
    lpdit->id = IDOK;  
	lpdit->style = WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
//	lpdit->dwExtendedStyle = WS_EX_CLIENTEDGE;
    lpw = (LPWORD) (lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0080;    // button class
	lpwsz = (LPWSTR) lpw;
	nchar = MultiByteToWideChar (CP_ACP, 0, "Ok", -1, lpwsz, 50);
    lpw   += nchar;
    lpw = lpdwAlign (lpw); // align creation data on DWORD boundary
    *lpw++ = 0;			// no creation data


	// Define Button
    lpw = lpdwAlign (lpw); // align DLGITEMTEMPLATE on DWORD boundary
    lpdit = (LPDLGITEMTEMPLATE) lpw;
    lpdit->x  = 100; lpdit->y  = 25;
    lpdit->cx = 60; lpdit->cy = 15;
    lpdit->id = IDCANCEL;  
	lpdit->style = WS_VISIBLE | WS_CHILD;
//	lpdit->dwExtendedStyle = WS_EX_CLIENTEDGE;
    lpw = (LPWORD) (lpdit + 1);
    *lpw++ = 0xFFFF;
    *lpw++ = 0x0080;    // button class
	lpwsz = (LPWSTR) lpw;
	nchar = MultiByteToWideChar (CP_ACP, 0, "Cancel", -1, lpwsz, 50);
    lpw   += nchar;
    lpw = lpdwAlign (lpw); // align creation data on DWORD boundary
    *lpw++ = 0;			// no creation data




    GlobalUnlock(hgbl); 
    ret = DialogBoxIndirect(gHInstance, (LPDLGTEMPLATE) hgbl, gSexyAppBase->mHWnd, (DLGPROC) JumpToTimeDialogProc); 
    GlobalFree(hgbl); 

	gSexyAppBase->mLastTime = timeGetTime();

    return ret; */
}

[[maybe_unused]]
static void ToggleDemoSoundVolume()
{
	if (gSexyAppBase->GetMusicVolume() == 0.0)
		gSexyAppBase->SetMusicVolume(gSexyAppBase->mDemoMusicVolume);
	else
	{
		gSexyAppBase->mDemoMusicVolume = gSexyAppBase->mMusicVolume;
		gSexyAppBase->SetMusicVolume(0.0);
	}

	if (gSexyAppBase->GetSfxVolume() == 0.0)
		gSexyAppBase->SetSfxVolume(gSexyAppBase->mDemoSfxVolume);
	else
	{
		gSexyAppBase->mDemoSfxVolume = gSexyAppBase->mSfxVolume;
		gSexyAppBase->SetSfxVolume(0.0);
	}
}

//static DWORD gPowerSaveTick = 0;
/*
static bool ScreenSaverWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT &theResult)
{
	static bool gCreated = false;
	static int gMouseMoveCount = 0;
	static int gLastMouseX = 0, gLastMouseY = 0;
	static bool gClosed = false;
	typedef BOOL (WINAPI *VERIFYPWDPROC)(HWND);
	static VERIFYPWDPROC aPasswordFunc = NULL;
	HMODULE aPasswordLib = NULL;

	if (gClosed)
		return false;

	switch (uMsg)
	{
		case WM_CREATE:
		{
			if (gCreated)
				return false;

			gCreated = true;
			POINT aMousePoint;
			GetCursorPos(&aMousePoint);
			gLastMouseX = aMousePoint.x;
			gLastMouseY = aMousePoint.y;

			// Password checking stuff for 95/98/ME
			OSVERSIONINFO aVersion;
			aVersion.dwOSVersionInfoSize = sizeof(aVersion);
			GetVersionEx(&aVersion);
			if (aVersion.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
			{
				HKEY hKey;
				if (RegOpenKey(HKEY_CURRENT_USER, REGSTR_PATH_SCREENSAVE ,&hKey) == ERROR_SUCCESS)
				{
					DWORD aCheckPwd = 0;
					DWORD aSize = sizeof(DWORD);
					DWORD aType;
					LONG aResult = RegQueryValueEx(hKey, REGSTR_VALUE_USESCRPASSWORD, NULL, &aType, (PBYTE) &aCheckPwd, &aSize);
					if (aResult==ERROR_SUCCESS && aCheckPwd)
					{
						aPasswordLib = LoadLibrary(TEXT("PASSWORD.CPL"));
						if (aPasswordLib)
						{
							aPasswordFunc = (VERIFYPWDPROC)GetProcAddress(aPasswordLib, "VerifyScreenSavePwd");
							// prevents user from ctrl-alt-deleting the screensaver etc to avoid typing in a password
							int aPrev;
							SystemParametersInfo(SPI_SCREENSAVERRUNNING, TRUE, &aPrev, 0); 
						}

					}
					RegCloseKey(hKey);
				}
			}
			return false;
		}
		break;

		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_CLOSE:
				case SC_SCREENSAVE:
				case SC_NEXTWINDOW:
				case SC_PREVWINDOW:
					theResult = FALSE;
					return true;

				default:
					return false;
			}
		}
		break;

		case WM_MOUSEMOVE:
		{
			int aMouseX = LOWORD(lParam);
			int aMouseY = HIWORD(lParam);
//			SEXY_TRACE(StrFormat("SCR MouseMove: %d %d",aMouseX,aMouseY).c_str());
			if (aMouseX!=gLastMouseX || aMouseY!=gLastMouseY)
			{
				gLastMouseX = aMouseX;
				gLastMouseY = aMouseY;
				gMouseMoveCount++;
			}

			if (gMouseMoveCount < 4)
			{
				theResult = 0;
				return true;
			}
		}
		break;

		case WM_NCACTIVATE:
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
		{
			if (wParam != FALSE)
				return false;
		}
		break;

		case WM_CLOSE:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			break;

		default:
			return false;
	}

	if (gSexyAppBase!=NULL && gSexyAppBase->mHWnd!=hWnd) // wrong window
		return false;

	if (GetTickCount()-gPowerSaveTick < 1000) // powersave just went on so ignore certain messages that seem to come on certain os's at that time
	{
		switch (uMsg)
		{
			case WM_MOUSEMOVE: 
			case WM_NCACTIVATE:
			case WM_ACTIVATE: 
			case WM_ACTIVATEAPP:
			case WM_CLOSE: 
				return false;
		}
	}

	if (aPasswordFunc && gSexyAppBase!=NULL && gSexyAppBase->mInitialized) // need to verify password before closing
	{
		if (gSexyAppBase!=NULL && gSexyAppBase->mDDInterface!=NULL && gSexyAppBase->mDDInterface->mDD!=NULL)
		{
			gSexyAppBase->mDDInterface->mDD->FlipToGDISurface();	// so we can see the password dialog
			gSexyAppBase->mNoDefer = true;							// so the app doesn't draw over the password dialog
		}
	    
		gClosed = true; // prevent this function from doing anything while in the password dialog
		BOOL aPasswordResult = aPasswordFunc(hWnd);
		gClosed = false; // let this functino work again

		if (gSexyAppBase!=NULL)
		{
			gSexyAppBase->mNoDefer = false;
			gSexyAppBase->ClearUpdateBacklog();
		}

		if (!aPasswordResult) // bad password
		{
			// Get new mouse coordinate
			POINT aMousePoint;
			GetCursorPos(&aMousePoint);
			gLastMouseX = aMousePoint.x;
			gLastMouseY = aMousePoint.y;
			gMouseMoveCount = 0;

			return false;
		}


		// can turn this SPI_SCREENSAVERRUNNING off now since screensaver is about to stop
		int aPrev;
		SystemParametersInfo(SPI_SCREENSAVERRUNNING, FALSE, &aPrev, 0); 

		// good password -> close and unload dll
		FreeLibrary(aPasswordLib);
		aPasswordLib = NULL;
		aPasswordFunc = NULL;
	}
	
	// Screen saver should shutdown
	gClosed = true;
	PostMessage(hWnd, WM_CLOSE, 0, 0);*/

/*	const char *str = "";
	switch (uMsg)
	{
		case WM_CREATE: str="WM_CREATE"; break;
		case WM_SYSCOMMAND: str="WM_SYSCOMMAND"; break;
		case WM_MOUSEMOVE: str="WM_MOUSEMOVE"; break;
		case WM_NCACTIVATE: str="WM_NCACTIVATE"; break;
		case WM_ACTIVATE: str="WM_ACTIVATE"; break;
		case WM_ACTIVATEAPP: str="WM_ACTIVATEAPP"; break;
		case WM_CLOSE: str="WM_CLOSE"; break;
		case WM_LBUTTONDOWN: str="WM_LBUTTONDOWN"; break;
		case WM_RBUTTONDOWN: str="WM_RBUTTONDOWN"; break;
		case WM_MBUTTONDOWN: str="WM_MBUTTONDOWN"; break;
		case WM_KEYDOWN: str="WM_KEYDOWN"; break;
		case WM_SYSKEYDOWN: str="WM_SYSKEYDOWN"; break;
	}

	SEXY_TRACE(StrFormat("Scr shutdown: %s",str).c_str());*/
	//return false;
//}
/*
LRESULT CALLBACK SexyAppBase::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (gSexyAppBase!=NULL && gSexyAppBase->IsScreenSaver())
	{
		LRESULT aResult;
		if (ScreenSaverWindowProc(hWnd,uMsg,wParam,lParam,aResult))
			return aResult;
	}

	SexyAppBase* aSexyApp = (SexyAppBase*)(intptr_t)GetWindowLongPtr(hWnd, GWLP_USERDATA);	
	switch (uMsg)
	{		
//  TODO: switch to killfocus/setfocus?
//	case WM_KILLFOCUS:
//	case WM_SETFOCUS:
//	if ((aSexyApp != NULL) && (!aSexyApp->mPlayingDemoBuffer))
//	{
//		if (hWnd == aSexyApp->mHWnd)
//			aSexyApp->mActive = uMsg==WM_SETFOCUS;
//	}
//	//Fallthrough

	case WM_ACTIVATEAPP:
		if ((aSexyApp != NULL) && (!aSexyApp->mPlayingDemoBuffer))
		{
			if (hWnd == aSexyApp->mHWnd)
			{
				aSexyApp->mActive = wParam != 0;
			}
		}
		//Fallthrough	

	case WM_SIZE:
	case WM_MOVE:
	case WM_TIMER:
	case WM_LBUTTONDOWN:		
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:		
	case WM_MBUTTONUP:		
	case WM_MOUSEMOVE:			
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	case WM_CHAR:	
	case WM_CLOSE:
	case WM_MOUSEWHEEL:
	case WM_DISPLAYCHANGE:
	case WM_SYSCOLORCHANGE:
		{
*//*			if (aSexyApp!=NULL && aSexyApp->mProcessInTimer && !aSexyApp->mShutdown && aSexyApp->mRunning)
			{
				if (uMsg==WM_TIMER && wParam==101)
				{
					for (int i=0; i<5; i++)
					{
						if (GetQueueStatus(QS_INPUT | QS_PAINT))
							break;

						if (!aSexyApp->Process(false))
							break;
					}
				}
					
				break;
			}*/

			/*if ((aSexyApp != NULL) && (aSexyApp->mNoDefer))
			{
				// Check to see if we should be windowed
				WINDOWPLACEMENT aWindowPlacement;
				aWindowPlacement.length = sizeof(aWindowPlacement);
				if (GetWindowPlacement(aSexyApp->mHWnd, &aWindowPlacement))
				{
					if (aWindowPlacement.showCmd == SW_SHOWMINIMIZED)
					{
						aSexyApp->Redraw(NULL);
					}
				}
			}*/
/*
			if ((aSexyApp != NULL) && (!aSexyApp->mNoDefer))
			{
				bool keyDown = (uMsg==WM_KEYDOWN) || (uMsg==WM_SYSKEYDOWN);

				if ((keyDown) || (uMsg==WM_KEYUP) || (uMsg == WM_SYSKEYUP))
				{	
					if (wParam == VK_CONTROL)
						aSexyApp->mCtrlDown = keyDown;
					if (wParam == VK_MENU)
						aSexyApp->mAltDown = keyDown;
				}
*/
				/*
				if ((keyDown) && (aSexyApp->DebugKeyDownAsync(wParam, aSexyApp->mCtrlDown, aSexyApp->mAltDown)))
					return 0;
				*/
/*	
				if (aSexyApp->mPlayingDemoBuffer)
				{
					if (uMsg==WM_CHAR)
					{
						switch (wParam)
						{
						case '+':
							aSexyApp->mUpdateMultiplier *= 1.5;
							break;
						case '-':
							aSexyApp->mUpdateMultiplier /= 1.5;
							break;
						case '=':
							aSexyApp->mUpdateMultiplier = 1;
							break;
						case 'p':
						case 'P':
							aSexyApp->mPaused = !aSexyApp->mPaused;
							aSexyApp->mLastTimeCheck = std::chrono::high_resolution_clock::now();
							aSexyApp->mUpdateFTimeAcc = 0.0;
							break;

						case 'n':
						case 'N':
							aSexyApp->mFastForwardToUpdateNum = aSexyApp->mUpdateCount+1;
							aSexyApp->mFastForwardStep = true;
							break;

						case 'm':
						case 'M':
							aSexyApp->mFastForwardToMarker = true;
							break;

						case 'l':
						case 'L':
							ListDemoMarkers();
							break;

						case 'j':
						case 'J':
							DemoJumpToTime();
							break;

						case 's':
						case 'S':
							ToggleDemoSoundVolume();
							break;

						case '4':
							// Fast foward to 120 seconds before it ends
							aSexyApp->mFastForwardToUpdateNum = aSexyApp->mDemoLength - (120000 / aSexyApp->mFrameTime);
							break;
						case '5':
							// Fast foward to 90 seconds before it ends
							aSexyApp->mFastForwardToUpdateNum = aSexyApp->mDemoLength - (90000 / aSexyApp->mFrameTime);
							break;
						case '6':
							// Fast foward to 60 seconds before it ends
							aSexyApp->mFastForwardToUpdateNum = aSexyApp->mDemoLength - (60000 / aSexyApp->mFrameTime);
							break;
						case '7':
							// Fast foward to 30 seconds before it ends
							aSexyApp->mFastForwardToUpdateNum = aSexyApp->mDemoLength - (30000 / aSexyApp->mFrameTime);
							break;
						case '8':
							// Fast foward to 10 seconds before it ends
							aSexyApp->mFastForwardToUpdateNum = aSexyApp->mDemoLength - (10000 / aSexyApp->mFrameTime);
							break;
						case '9':
							// Fast foward to 5 seconds before it ends
							aSexyApp->mFastForwardToUpdateNum = aSexyApp->mDemoLength - (5000 / aSexyApp->mFrameTime);
							break;
						case '0':						
							// Fast forward to the end
							aSexyApp->mFastForwardToUpdateNum = aSexyApp->mDemoLength;
							break;

						}
					}
					else if (uMsg==WM_KEYDOWN)
						aSexyApp->DebugKeyDown(wParam);
				}

				bool pushMessage = true;

				if (aSexyApp->mDeferredMessages.size() > 0)
				{
					// Don't add any more messages after WM_CLOSE
					MSG* aMsg = &aSexyApp->mDeferredMessages.back();

					if (aMsg->message == WM_CLOSE)
						pushMessage = false;
					if ((uMsg == WM_TIMER) && (uMsg == aMsg->message))
						pushMessage = false; // Don't need more timer messages


					if (pushMessage && (uMsg==WM_SYSCOLORCHANGE || uMsg==WM_DISPLAYCHANGE)) // kill duplicate SysColorChange() events.
					{
						WindowsMessageList::iterator aMsgListItr = aSexyApp->mDeferredMessages.begin();
						while (pushMessage && aMsgListItr != aSexyApp->mDeferredMessages.end())
						{
							MSG& aMsg = *aMsgListItr;

							if (aMsg.message == WM_SYSCOLORCHANGE || aMsg.message == WM_DISPLAYCHANGE)
								pushMessage = false;

							++aMsgListItr;
						}
					}
				}

				if (pushMessage)
				{
					MSG msg;
					msg.hwnd = hWnd;
					msg.message = uMsg;
					msg.lParam = lParam;
					msg.wParam = wParam;	
					
					aSexyApp->mDeferredMessages.push_back(msg);
				}

				if (uMsg == WM_SIZE)
				{
					aSexyApp->mPhysMinimized = wParam == SIZE_MINIMIZED;
				}
				else if (uMsg == WM_SYSKEYDOWN)
				{
					if (wParam != VK_F4)
						return 0;		
				}
				else if (uMsg == WM_CLOSE)
				{
*/					/*char aStr[256];
					sprintf(aStr, "CLOSED HWND: %d\r\n", hWnd);
					OutputDebugString(aStr);*/
					
/*					aSexyApp->CloseRequestAsync();
					return 0;
				}
			}			
		}
		break;

	case WM_ENABLE:
		if (aSexyApp != NULL)
		{
			aSexyApp->mIsDisabled = wParam == 0;
		}
		break;

	case WM_QUERYOPEN:
		if ((aSexyApp != NULL) && (!aSexyApp->AppCanRestore()))
			return 0;
		break;

	case WM_SYSCHAR:
		if ((aSexyApp != NULL) && (aSexyApp->IsAltKeyUsed(wParam)))
			return 0;
		break;

	case WM_NCLBUTTONDOWN:
		if (aSexyApp!=NULL)
		{
//			aSexyApp->mProcessInTimer = true;
			LRESULT aResult = DefWindowProc(hWnd, uMsg, wParam, lParam);
//			aSexyApp->mProcessInTimer = false;
			aSexyApp->ClearUpdateBacklog();
			return aResult;
		}
		break;


	case WM_SYSCOMMAND:
		if (wParam==SC_MONITORPOWER)
		{
			gPowerSaveTick = GetTickCount();
			if (aSexyApp!=NULL && (!aSexyApp->mAllowMonitorPowersave || !aSexyApp->mLoaded))
				return FALSE;
		}
		if (wParam==SC_SCREENSAVE && aSexyApp!=NULL && (!aSexyApp->mLoaded || !aSexyApp->mIsPhysWindowed))
			return FALSE;

		break;
*/
/*	case WM_DISPLAYCHANGE:
		SEXY_TRACE("WM_DISPLAYCHANGE 1");
		if (aSexyApp!=NULL && aSexyApp->mIsWindowed && aSexyApp->mDDInterface!=NULL && aSexyApp->mHWnd==hWnd && aSexyApp->mLoaded)
		{
			SEXY_TRACE("WM_DISPLAYCHANGE 2");
			aSexyApp->mDDInterface->Init(aSexyApp->mHWnd,aSexyApp->mIsWindowed);
			aSexyApp->mWidgetManager->mImage = aSexyApp->mDDInterface->GetScreenImage();
			aSexyApp->mWidgetManager->MarkAllDirty();
		}
		break;*/
/*
	case WM_DESTROY:
		{
			char aStr[256];
			sprintf(aStr, "DESTROYED HWND: %p\r\n", hWnd);
			OutputDebugStringA(aStr);
		}
		break;	
	case WM_SETCURSOR:
		if (!aSexyApp->mSEHOccured)
			aSexyApp->EnforceCursor();
		return TRUE;		
	case WM_ERASEBKGND:
		return TRUE;		
	case WM_ENDSESSION:
		aSexyApp->Shutdown();
		break;
	case WM_PAINT:
		if ((aSexyApp->mInitialized) && (!gInAssert) && (!aSexyApp->mSEHOccured))
		{
			RECT aClientRect;
			GetClientRect(hWnd, &aClientRect);			

			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);			
			
			if (aSexyApp->mRunning)
				aSexyApp->Redraw(NULL);

			EndPaint(hWnd, &ps);

			return 0;
		}
		break;
	}

	if ((aSexyApp != NULL) && (uMsg == aSexyApp->mNotifyGameMessage) && (hWnd == aSexyApp->mHWnd))
	{
		// Oh, we are trying to open another instance of ourselves.
		// Bring up the original window instead
		aSexyApp->HandleNotifyGameMessage(wParam);		
		return 0;
	}

	if (gSexyAppBase->mIsWideWindow)
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	else
		return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}*/


void SexyAppBase::HandleNotifyGameMessage(int /*theType*/)
{
	unreachable();
	/* TODO
	if (theType==0) // bring to front message
	{
		WINDOWPLACEMENT aWindowPlacement;
		aWindowPlacement.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(mHWnd, &aWindowPlacement);		

		if (aWindowPlacement.showCmd == SW_SHOWMINIMIZED)
			ShowWindow(mHWnd, SW_RESTORE);

		::SetForegroundWindow(mHWnd);
	}*/
}

void SexyAppBase::ClearKeysDown()
{
	if (mWidgetManager != NULL) // fix stuck alt-key problem
	{
		for (int aKeyNum = 0; aKeyNum < 0xFF; aKeyNum++)
			mWidgetManager->mKeyDown[aKeyNum] = false;
	}
	mCtrlDown = false;
	mAltDown = false;
}

void SexyAppBase::WriteDemoTimingBlock()
{
	// Demo writing functions can only be called from the main thread and after SexyAppBase::Init
	DBG_ASSERTE(std::this_thread::get_id() == mPrimaryThreadId);

	while (mUpdateCount - mLastDemoUpdateCnt > 15)
	{
		mDemoBuffer.WriteNumBits(15, 4);
		mLastDemoUpdateCnt += 15;

		mDemoBuffer.WriteNumBits(0, 1);
		mDemoBuffer.WriteNumBits(DEMO_IDLE, 5);
		mDemoCmdOrder++;
	}
	
	mDemoBuffer.WriteNumBits(mUpdateCount - mLastDemoUpdateCnt, 4);
	mLastDemoUpdateCnt = mUpdateCount;
	mDemoCmdOrder++;
}

int aNumBigMoveMessages = 0;
int aNumSmallMoveMessages = 0;
int aNumTimerMessages = 0;

// Required unused in release mode
bool SexyAppBase::PrepareDemoCommand([[maybe_unused]] bool required)
{
	if (mDemoNeedsCommand)
	{
		mDemoCmdBitPos = mDemoBuffer.mReadBitPos;

		mLastDemoUpdateCnt += mDemoBuffer.ReadNumBits(4, false);

		mDemoIsShortCmd = mDemoBuffer.ReadNumBits(1, false) == 1;

		if (mDemoIsShortCmd)		
			mDemoCmdNum = mDemoBuffer.ReadNumBits(1, false);
		else
			mDemoCmdNum = mDemoBuffer.ReadNumBits(5, false);

		mDemoNeedsCommand = false;

		mDemoCmdOrder++;
	}

	DBG_ASSERTE((mUpdateCount == mLastDemoUpdateCnt) || (!required));

	return mUpdateCount == mLastDemoUpdateCnt;
}

void SexyAppBase::ProcessDemo()
{
	if (mPlayingDemoBuffer)
	{
		// At end of demo buffer?  How dare you!
		DBG_ASSERTE(!mDemoBuffer.AtEnd());

		while ((!mShutdown) && (mUpdateCount == mLastDemoUpdateCnt) && (!mDemoBuffer.AtEnd()))
		{
			if (PrepareDemoCommand(false))
			{
				mDemoNeedsCommand = true;
				
				if (mDemoIsShortCmd)
				{
					switch (mDemoCmdNum)
					{
					case 0:
						{
							int aDeltaX = mDemoBuffer.ReadNumBits(6, true);
							int aDeltaY = mDemoBuffer.ReadNumBits(6, true);
							mLastDemoMouseX += aDeltaX;
							mLastDemoMouseY += aDeltaY;

							mWidgetManager->MouseMove(mLastDemoMouseX, mLastDemoMouseY);
						}
						break;
					case 1:
						{
							bool down = mDemoBuffer.ReadNumBits(1, false) != 0;
							int aBtnCount = mDemoBuffer.ReadNumBits(3, true);							
		
							if (down)
								mWidgetManager->MouseDown(mLastDemoMouseX, mLastDemoMouseY, aBtnCount);
							else
								mWidgetManager->MouseUp(mLastDemoMouseX, mLastDemoMouseY, aBtnCount);
						}
						break;
					}
				}
				else
				{				
					switch (mDemoCmdNum)
					{
					case DEMO_MOUSE_POSITION:
						{
							mLastDemoMouseX = mDemoBuffer.ReadNumBits(12, false);
							mLastDemoMouseY = mDemoBuffer.ReadNumBits(12, false);

							mWidgetManager->MouseMove(mLastDemoMouseX, mLastDemoMouseY);						
						}
						break;
					case DEMO_ACTIVATE_APP:
						{
							mActive = mDemoBuffer.ReadNumBits(1, false) != 0;

							mWindowInterface->RehupFocus();
							
							if ((mActive) && (!mIsWindowed))
								mWidgetManager->MarkAllDirty();

							if ((mIsOpeningURL) && (!mActive))
								URLOpenSucceeded(mOpeningURL);
						}
						break;
					case DEMO_SIZE:
						{
							bool isMinimized = mDemoBuffer.ReadBoolean();

							if ((!mShutdown) && (isMinimized != mMinimized))
							{
								mMinimized = isMinimized;

								// We don't want any sounds (or music) playing while its minimized
								if (mMinimized)
									Mute(true);
								else
								{
									Unmute(true);
									mWidgetManager->MarkAllDirty();
								}
							}

							mWindowInterface->RehupFocus();
						}
						break;
					case DEMO_MOUSE_WHEEL:
						{
							int aScroll = mDemoBuffer.ReadNumBits(8, true);
							mWidgetManager->MouseWheel(aScroll);
						}
						break;
					case DEMO_KEY_DOWN:
						{
							KeyCode aKeyCode = (KeyCode) mDemoBuffer.ReadNumBits(8, false);
							mWidgetManager->KeyDown(aKeyCode);
						}
						break;
					case DEMO_KEY_UP:
						{
							KeyCode aKeyCode = (KeyCode) mDemoBuffer.ReadNumBits(8, false);
							mWidgetManager->KeyUp(aKeyCode);
						}
						break;
					case DEMO_KEY_CHAR:
						{
							int sizeMult = (int)mDemoBuffer.ReadNumBits(1, false) + 1; // will be 1 for single, 2 for double
							SexyChar aChar = (SexyChar) mDemoBuffer.ReadNumBits(8*sizeMult, false);
							mWidgetManager->KeyChar(aChar);
						}
						break;
					case DEMO_CLOSE:
						Shutdown();
						break;
					case DEMO_MOUSE_ENTER:
						mMouseIn = true;
						mWindowInterface->EnforceCursor();
						break;
					case DEMO_MOUSE_EXIT:
						mWidgetManager->MouseExit(mLastDemoMouseX, mLastDemoMouseY);
						mMouseIn = false;
						mWindowInterface->EnforceCursor();
						break;
					case DEMO_LOADING_COMPLETE:
						mDemoLoadingComplete = true;
						break;
					case DEMO_VIDEO_DATA:
						mIsWindowed = mDemoBuffer.ReadBoolean();
						mSyncRefreshRate = mDemoBuffer.ReadByte();
						break;
					case DEMO_IDLE:
						break;
					default:
						DBG_ASSERTE("Invalid Demo Command" == 0);
						break;
					}
				}
			}
		}
	}
}

void SexyAppBase::ShowMemoryUsage()
{
	unreachable();
	/* TODO
	DWORD aTotal = 0;
	DWORD aFree = 0;

	if (mDDInterface->mDD7 != NULL)
	{
		DDSCAPS2 aCaps;
		ZeroMemory(&aCaps,sizeof(aCaps));
		aCaps.dwCaps = DDSCAPS_VIDEOMEMORY;	
		mDDInterface->mDD7->GetAvailableVidMem(&aCaps,&aTotal,&aFree);
	}

	MemoryImageSet::iterator anItr = mMemoryImageSet.begin();
	typedef std::pair<int,int> FormatUsage;
	typedef std::map<PixelFormat,FormatUsage> FormatMap;
	FormatMap aFormatMap;
	int aTextureMemory = 0;
	while (anItr != mMemoryImageSet.end())
	{
		MemoryImage* aMemoryImage = *anItr;				
		if (aMemoryImage->mD3DData != NULL)
		{
			TextureData *aData = (TextureData*)aMemoryImage->mD3DData;
			aTextureMemory += aData->mTexMemSize;

			FormatUsage &aUsage = aFormatMap[aData->mPixelFormat];
			aUsage.first++;
			aUsage.second += aData->mTexMemSize;
		}

		++anItr;
	}

	std::string aStr;

	const char *aDesc;
	if (Is3DAccelerationRecommended())
		aDesc = "Recommended";
	else if (Is3DAccelerationSupported())
		aDesc = "Supported";
	else
		aDesc = "Unsupported";

	aStr += StrFormat("3D-Mode is %s (3D is %s on this system)\r\n\r\n",Is3DAccelerated()?"On":"Off",aDesc);

	aStr += StrFormat("Num Images: %d\r\n",(int)mMemoryImageSet.size());
	aStr += StrFormat("Num Sounds: %d\r\n",mSoundManager->GetNumSounds());
	aStr += StrFormat("Video Memory: %s/%s KB\r\n", SexyStringToString(CommaSeperate((aTotal-aFree)/1024)).c_str(), SexyStringToString(CommaSeperate(aTotal/1024)).c_str());
	aStr += StrFormat("Texture Memory: %s KB\r\n",CommaSeperate(aTextureMemory/1024).c_str());

	FormatUsage aUsage = aFormatMap[PixelFormat_A8R8G8B8];
	aStr += StrFormat("A8R8G8B8: %d - %s KB\r\n",aUsage.first,SexyStringToString(CommaSeperate(aUsage.second/1024)).c_str());
	aUsage = aFormatMap[PixelFormat_A4R4G4B4];
	aStr += StrFormat("A4R4G4B4: %d - %s KB\r\n",aUsage.first,SexyStringToString(CommaSeperate(aUsage.second/1024)).c_str());
	aUsage = aFormatMap[PixelFormat_R5G6B5];
	aStr += StrFormat("R5G6B5: %d - %s KB\r\n",aUsage.first,SexyStringToString(CommaSeperate(aUsage.second/1024)).c_str());
	aUsage = aFormatMap[PixelFormat_Palette8];
	aStr += StrFormat("Palette8: %d - %s KB\r\n",aUsage.first,SexyStringToString(CommaSeperate(aUsage.second/1024)).c_str());
	
	MsgBox(aStr,"Video Stats",MB_OK);
	mLastTime = timeGetTime();
	*/
}

/*
bool SexyAppBase::IsAltKeyUsed(WPARAM wParam)
{
	int aChar = tolower(wParam);
	switch (aChar)
	{
		case 13: // alt-enter
		case 'r':
			return true;		
		default: 
			return false;
	}
}*/

bool SexyAppBase::DebugKeyDown(int /*theKey*/)
{
	unreachable();
	/* TODO
	if ((theKey == 'R') && (mWidgetManager->mKeyDown[KEYCODE_MENU]))
	{	
#ifndef RELEASEFINAL
		if (ReparseModValues())
			PlaySoundA("c:\\windows\\media\\Windows XP Menu Command.wav", NULL, SND_ASYNC);				
		else
		{
			for (int aKeyNum = 0; aKeyNum < 0xFF; aKeyNum++) // prevent alt from getting stuck
				mWidgetManager->mKeyDown[aKeyNum] = false;
		}
#endif
	}
	else if (theKey == VK_F3)
	{
		if(mWidgetManager->mKeyDown[KEYCODE_SHIFT])
		{
			mShowFPS = true;
			if (++mShowFPSMode >= Num_FPS_Types)
				mShowFPSMode = 0;
		}
		else
			mShowFPS = !mShowFPS;

		mWidgetManager->MarkAllDirty();

		if (mShowFPS)
		{
			gFPSTimer.Start();
			gFrameCount = 0;
			gFPSDisplay = 0;
			gForceDisplay = true;
		}
	}
	else if (theKey == VK_F8)
	{
		if(mWidgetManager->mKeyDown[KEYCODE_SHIFT])
		{
			Set3DAcclerated(!Is3DAccelerated());

			char aBuf[512];
			sprintf(aBuf,"3D-Mode: %s",Is3DAccelerated()?"ON":"OFF");
			MsgBox(aBuf,"Mode Switch",MB_OK);
			mLastTime = timeGetTime();
		}
		else
			ShowMemoryUsage();

		return true;
	}
	else if (theKey == VK_F10)
	{
#ifndef RELEASEFINAL
		if (mWidgetManager->mKeyDown[KEYCODE_CONTROL])
		{
			if (mUpdateMultiplier==0.25)
				mUpdateMultiplier = 1.0;
			else
				mUpdateMultiplier = 0.25;
		}
		else if(mWidgetManager->mKeyDown[KEYCODE_SHIFT])
		{
			mStepMode = 0;
			ClearUpdateBacklog();
		}
		else
			mStepMode = 1;
#endif

		return true;
	}
	else if (theKey == VK_F11)
	{
		if (mWidgetManager->mKeyDown[KEYCODE_SHIFT])
			DumpProgramInfo();
		else
			TakeScreenshot();

		return true;
	}
	else if (theKey == VK_F2)
	{
		bool isPerfOn = !SexyPerf::IsPerfOn();
		if (isPerfOn)
		{
//			MsgBox("Perf Monitoring: ON", "Perf Monitoring", MB_OK);
			ClearUpdateBacklog();
			SexyPerf::BeginPerf();
		}
		else
		{
			SexyPerf::EndPerf();
			MsgBox(SexyPerf::GetResults().c_str(), "Perf Results", MB_OK);
			ClearUpdateBacklog();
		}
	}
	else
		return false;

	return false;*/
}

/*
bool SexyAppBase::DebugKeyDownAsync(int theKey, bool ctrlDown, bool altDown)
{
	return false;
}
*/

void SexyAppBase::CloseRequestAsync()
{
}

// Why did I defer messages?  Oh, incase a dialog comes up such as a crash
//  it won't keep crashing and stuff
/*
bool SexyAppBase::ProcessDeferredMessages(bool singleMessage)
{
	while (mDeferredMessages.size() > 0)
	{
		MSG aMsg = mDeferredMessages.front();
		mDeferredMessages.pop_front();

		UINT uMsg = aMsg.message;
		LPARAM lParam = aMsg.lParam;
		WPARAM wParam = aMsg.wParam;
		HWND hWnd = aMsg.hwnd;

		if ((mRecordingDemoBuffer) && (!mShutdown))
		{
			switch (uMsg)
			{
//  TODO: switch to killfocus/setfocus?
//			case WM_SETFOCUS:
//			case WM_KILLFOCUS:
//				if (hWnd == mHWnd)
//				{					
//					WriteDemoTimingBlock();
//					mDemoBuffer.WriteNumBits(0, 1);
//					mDemoBuffer.WriteNumBits(DEMO_ACTIVATE_APP, 5);
//					mDemoBuffer.WriteNumBits(uMsg==WM_SETFOCUS ? 1 : 0, 1);
//				}
//				break;			
	
			case WM_ACTIVATEAPP:
				if (hWnd == mHWnd)
				{					
					WriteDemoTimingBlock();
					mDemoBuffer.WriteNumBits(0, 1);
					mDemoBuffer.WriteNumBits(DEMO_ACTIVATE_APP, 5);
					mDemoBuffer.WriteNumBits((wParam != 0) ? 1 : 0, 1);
				}
				break;			

			case WM_SIZE:
				{
					bool isMinimized = wParam == SIZE_MINIMIZED;

					WriteDemoTimingBlock();
					mDemoBuffer.WriteNumBits(0, 1);
					mDemoBuffer.WriteNumBits(DEMO_SIZE, 5);
					mDemoBuffer.WriteBoolean(isMinimized);		
				}
				break;
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_MOUSEMOVE:
				{
					int aCurX = (short) LOWORD(lParam);
					int aCurY = (short) HIWORD(lParam);

					int aDiffX = aCurX - mLastDemoMouseX;
					int aDiffY = aCurY - mLastDemoMouseY;

					if ((abs(aCurX - mLastDemoMouseX) < 32) && (abs(aCurY - mLastDemoMouseY) < 32))
					{
						if ((aDiffX != 0) || (aDiffY != 0))
						{
							WriteDemoTimingBlock();
							mDemoBuffer.WriteNumBits(1, 1);
							mDemoBuffer.WriteNumBits(0, 1);
							mDemoBuffer.WriteNumBits(aDiffX, 6);
							mDemoBuffer.WriteNumBits(aDiffY, 6);
						}						
					}
					else
					{
						WriteDemoTimingBlock();
						mDemoBuffer.WriteNumBits(0, 1);
						mDemoBuffer.WriteNumBits(DEMO_MOUSE_POSITION, 5);
						mDemoBuffer.WriteNumBits(aCurX, 12);
						mDemoBuffer.WriteNumBits(aCurY, 12);
					}

					bool down = true;
					int aBtnNum = 0;
					switch (uMsg)
					{
						case WM_LBUTTONDOWN:
							aBtnNum = 1;
							break;
						case WM_RBUTTONDOWN:
							aBtnNum = -1;
							break;
						case WM_MBUTTONDOWN:
							aBtnNum = 3;
							break;
						case WM_LBUTTONDBLCLK:
							aBtnNum = 2;
							break;
						case WM_RBUTTONDBLCLK:
							aBtnNum = -2;
							break;
						case WM_LBUTTONUP:
							aBtnNum = 1;
							down = false;
							break;
						case WM_RBUTTONUP:
							aBtnNum = -1;
							down = false;
							break;
						case WM_MBUTTONUP:
							aBtnNum = 3;
							down = false;
							break;
					}

					if (aBtnNum != 0)
					{
						WriteDemoTimingBlock();
						mDemoBuffer.WriteNumBits(1, 1);
						mDemoBuffer.WriteNumBits(1, 1);
						mDemoBuffer.WriteNumBits(down ? 1 : 0, 1);
						mDemoBuffer.WriteNumBits(aBtnNum, 3);
					}

					mLastDemoMouseX = aCurX;
					mLastDemoMouseY = aCurY;
				}
				break;
			case WM_MOUSEWHEEL:
				{
					int aZDelta = ((short)HIWORD(wParam)) / 120;

					WriteDemoTimingBlock();
					mDemoBuffer.WriteNumBits(0, 1);
					mDemoBuffer.WriteNumBits(DEMO_MOUSE_WHEEL, 5);
					mDemoBuffer.WriteNumBits(aZDelta, 8);
				}
				break;
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				{
					KeyCode aKeyCode = (KeyCode) wParam;

					WriteDemoTimingBlock();
					mDemoBuffer.WriteNumBits(0, 1);
					mDemoBuffer.WriteNumBits(DEMO_KEY_DOWN, 5);
					mDemoBuffer.WriteNumBits(aKeyCode, 8);
				}
				break;
			case WM_KEYUP:	
			case WM_SYSKEYUP:
				{
					KeyCode aKeyCode = (KeyCode) wParam;

					WriteDemoTimingBlock();
					mDemoBuffer.WriteNumBits(0, 1);
					mDemoBuffer.WriteNumBits(DEMO_KEY_UP, 5);
					mDemoBuffer.WriteNumBits((int) aKeyCode, 8);
				}
				break;
			case WM_CHAR:
				{
					SexyChar aChar = (SexyChar) wParam;

					WriteDemoTimingBlock();
					mDemoBuffer.WriteNumBits(0, 1);
					mDemoBuffer.WriteNumBits(DEMO_KEY_CHAR, 5);
					mDemoBuffer.WriteNumBits(sizeof(SexyChar) == 2, 1);
					mDemoBuffer.WriteNumBits(aChar, sizeof(SexyChar)*8);
				}				
				break;	
			case WM_CLOSE:
				if ((hWnd == mHWnd) || (hWnd == mInvisHWnd))
				{
					WriteDemoTimingBlock();
					mDemoBuffer.WriteNumBits(0, 1);
					mDemoBuffer.WriteNumBits(DEMO_CLOSE, 5);
				}
				break;
			}

			// int aBufferSize = mDemoBuffer.GetDataLen(); // Why?
		}

		if (!mPlayingDemoBuffer)
		{
			switch (uMsg)
			{	
//  TODO: switch to killfocus/setfocus?
//			case WM_KILLFOCUS:
//			case WM_SETFOCUS:
			case WM_ACTIVATEAPP:
				if ((hWnd == mHWnd) && (!gInAssert) && (!mSEHOccured) && (!mShutdown))
				{
//					mActive = uMsg==WM_SETFOCUS;

					RehupFocus();
					
					if ((mActive) && (!mIsWindowed))
						mWidgetManager->MarkAllDirty();

					if ((mIsOpeningURL) && (!mActive))			
						URLOpenSucceeded(mOpeningURL);
				}
				break;	
			case WM_LBUTTONDOWN:		
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
			case WM_LBUTTONUP:		
			case WM_RBUTTONUP:		
			case WM_MBUTTONUP:
			case WM_MOUSEMOVE:		
				if ((!gInAssert) && (!mSEHOccured))
				{
					int x = (short) LOWORD(lParam);
					int y = (short) HIWORD(lParam);
					mWidgetManager->RemapMouse(x, y);

					mLastUserInputTick = mLastTimerTime;
					
					mWidgetManager->MouseMove(x, y);		

					if (!mMouseIn)
					{
						if (mRecordingDemoBuffer)
						{
							WriteDemoTimingBlock();
							mDemoBuffer.WriteNumBits(0, 1);							
							mDemoBuffer.WriteNumBits(DEMO_MOUSE_ENTER, 5);
						}

						mMouseIn = true;
						EnforceCursor();
					}

					switch (uMsg)
					{
					case WM_LBUTTONDOWN:		
						SetCapture(hWnd);
						mWidgetManager->MouseDown(x, y, 1);						
						break;
					case WM_RBUTTONDOWN:
						SetCapture(hWnd);
						mWidgetManager->MouseDown(x, y, -1);						
						break;
					case WM_MBUTTONDOWN:
						SetCapture(hWnd);
						mWidgetManager->MouseDown(x, y, 3);						
						break;
					case WM_LBUTTONDBLCLK:
						SetCapture(hWnd);
						mWidgetManager->MouseDown(x, y, 2);						
						break;
					case WM_RBUTTONDBLCLK:
						SetCapture(hWnd);
						mWidgetManager->MouseDown(x, y, -2);						
						break;
					case WM_LBUTTONUP:		
						if ((mWidgetManager->mDownButtons & ~1) == 0)
							ReleaseCapture();
						mWidgetManager->MouseUp(x, y, 1);						
						break;
					case WM_RBUTTONUP:		
						if ((mWidgetManager->mDownButtons & ~2) == 0)
							ReleaseCapture();
						mWidgetManager->MouseUp(x, y, -1);								
						break;
					case WM_MBUTTONUP:		
						if ((mWidgetManager->mDownButtons & ~4) == 0)
							ReleaseCapture();
						mWidgetManager->MouseUp(x, y, 3);								
						break;
					}
				}
				break;		
			case WM_MOUSEWHEEL:
				{
					char aZDelta = ((short)HIWORD(wParam)) / 120;
					mWidgetManager->MouseWheel(aZDelta);
				}
				break;
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				mLastUserInputTick = mLastTimerTime;

				if (wParam==VK_RETURN && uMsg==WM_SYSKEYDOWN && !mForceFullscreen && !mForceWindowed && mAllowAltEnter)
				{
					SwitchScreenMode(!mIsWindowed);
					ClearKeysDown();
					break;
				}
				else if ((wParam == 'D') && (mWidgetManager != NULL) && (mWidgetManager->mKeyDown[KEYCODE_CONTROL]) && (mWidgetManager->mKeyDown[KEYCODE_MENU]))
				{
					PlaySoundA("c:\\windows\\media\\Windows XP Menu Command.wav", NULL, SND_ASYNC);
					mDebugKeysEnabled = !mDebugKeysEnabled;						
				}

				if (mDebugKeysEnabled)
				{
					if (DebugKeyDown(wParam))
						break;
				}

				mWidgetManager->KeyDown((KeyCode) wParam);
				break;

			case WM_KEYUP:
			case WM_SYSKEYUP:
				mLastUserInputTick = mLastTimerTime;
				mWidgetManager->KeyUp((KeyCode) wParam);
				break;
			case WM_CHAR:		
				mLastUserInputTick = mLastTimerTime;
				mWidgetManager->KeyChar((SexyChar) wParam);		
				break;
			case WM_MOVE:
				{
					if ((hWnd == mHWnd) && (mIsWindowed))
					{
						WINDOWPLACEMENT aWindowPlacment;
						aWindowPlacment.length = sizeof(aWindowPlacment);

						GetWindowPlacement(hWnd, &aWindowPlacment);
						if ((aWindowPlacment.showCmd == SW_SHOW) ||
							(aWindowPlacment.showCmd == SW_SHOWNORMAL))
						{										
							mPreferredX = aWindowPlacment.rcNormalPosition.left;
							mPreferredY = aWindowPlacment.rcNormalPosition.top;
						}
					}
				}
				break;
			case WM_SIZE:
				{	
					bool isMinimized = wParam == SIZE_MINIMIZED;
					
					if ((hWnd == mHWnd) && (!mShutdown) && (isMinimized != mMinimized))
					{
						mMinimized = isMinimized;

						// We don't want any sounds (or music) playing while its minimized
						if (mMinimized)
						{
							if (mMuteOnLostFocus)
								Mute(true);
						}
						else
						{
							if (mMuteOnLostFocus)
								Unmute(true);

							mWidgetManager->MarkAllDirty();
						}
					}

					RehupFocus();
					if (wParam==SIZE_MAXIMIZED)
						SwitchScreenMode(false);
				}
				break;
			case WM_TIMER:
				if ((!gInAssert) && (!mSEHOccured) && (mRunning))
				{	
					DWORD aTimeNow = GetTickCount();
					if (aTimeNow - mLastTimerTime > 500)
						mLastBigDelayTime = aTimeNow;
		
					mLastTimerTime = aTimeNow;

					if ((mIsOpeningURL) &&						
						(aTimeNow - mLastBigDelayTime > 5000))
					{
						if ((aTimeNow - mOpeningURLTime > 8000) && (!mActive))
						{
							//TODO: Have some demo message thing
							URLOpenSucceeded(mOpeningURL);
						}
						else if ((aTimeNow - mOpeningURLTime > 12000) && (mActive))						
						{
							URLOpenFailed(mOpeningURL);
						}
					}

					POINT aULCorner = {0, 0};
					::ClientToScreen(hWnd, &aULCorner);

					POINT aBRCorner = {mDDInterface->mDisplayWidth, mDDInterface->mDisplayHeight};
					::ClientToScreen(hWnd, &aBRCorner);

					POINT aPoint;
					::GetCursorPos(&aPoint);			
					
					HWND aWindow = ::WindowFromPoint(aPoint);
					bool isMouseIn = (aWindow == hWnd) && 
						(aPoint.x >= aULCorner.x) && (aPoint.y >= aULCorner.y) &&
						(aPoint.x < aBRCorner.x) && (aPoint.y < aBRCorner.y);

					if (mMouseIn != isMouseIn)
					{
						if ((mRecordingDemoBuffer) && (!mShutdown))
						{
							WriteDemoTimingBlock();
							mDemoBuffer.WriteNumBits(0, 1);

							if (isMouseIn)
								mDemoBuffer.WriteNumBits(DEMO_MOUSE_ENTER, 5);
							else
								mDemoBuffer.WriteNumBits(DEMO_MOUSE_EXIT, 5);
						}

						if (!isMouseIn)
						{
							int x = aPoint.x - aULCorner.x;
							int y = aPoint.y - aULCorner.y;
							mWidgetManager->RemapMouse(x, y);
							mWidgetManager->MouseExit(x, y);
						}

						mMouseIn = isMouseIn;
						EnforceCursor();
					}			
				}
				break;

			case WM_SYSCOLORCHANGE:
			case WM_DISPLAYCHANGE:
				mWidgetManager->SysColorChangedAll();
				mWidgetManager->MarkAllDirty();
				break;
			}
		}

		switch (uMsg)
		{
		case WM_CLOSE:
			if ((hWnd == mHWnd) || (hWnd == mInvisHWnd))
			{
				// This should short-circuit all demo calls, otherwise we will get
				//  all sorts of weird asserts because we are changing
				//  program flow
				mManualShutdown = true;

				Shutdown();
			}
			break;
		}

		if (singleMessage)
			break;
	}

	return (mDeferredMessages.size() > 0);
}*/

void SexyAppBase::Done3dTesting()
{
}

// return file name that you want to upload
std::string	SexyAppBase::NotifyCrashHook()
{
	return "";
}

void SexyAppBase::MakeWindow()
{
	mWindowInterface = new Vk::VkInterface(mWidth, mHeight, mWidgetManager);
	mWindowInterface->EnforceCursor();

	if ((mPlayingDemoBuffer) || (mIsWindowed && !mFullScreenWindow))
	{
		mIsPhysWindowed = true;
	}
	else
	{
		mIsPhysWindowed = false;
	}

	/*char aStr[256];
	sprintf(aStr, "HWND: %d\r\n", mHWnd);
	OutputDebugString(aStr);*/


	// SetWindowLongPtr(mHWnd, GWLP_USERDATA, (intptr_t) this);	


	// Enable 3d setting
	bool is3D = false;
	bool is3DOptionSet = RegistryReadBoolean("Is3D", &is3D);
	if (is3DOptionSet)
	{
		if (mAutoEnable3D)
		{
			mAutoEnable3D = false;
			mTest3D = true;
		}

		if (is3D)
			mTest3D = true;

		//mDDInterface->mIs3D = is3D;
	}

	/* DD init code, unnecesary, we do this in the WindowInterface
	int aResult = InitDDInterface();

	if (mDDInterface->mD3DTester!=NULL && mDDInterface->mD3DTester->ResultsChanged())
		RegistryEraseValue(_S("Is3D"));

	if ((mIsWindowed) && (aResult == DDInterface::RESULT_INVALID_COLORDEPTH))
	{
		if (mForceWindowed)
		{
			Popup(GetString("PLEASE_SET_COLOR_DEPTH", _S("Please set your desktop color depth to 16 bit.")));
			DoExit(1);
		}
		else
		{
			mForceFullscreen = true;
			SwitchScreenMode(false);
		}
		return;
	}
	else if ((!mIsWindowed) && 
		((aResult == DDInterface::RESULT_EXCLUSIVE_FAIL) ||
		 (aResult == DDInterface::RESULT_DISPCHANGE_FAIL)))
	{
		mForceWindowed = true;
		SwitchScreenMode(true);
	}
	else if (aResult == DDInterface::RESULT_3D_FAIL)
	{
		Set3DAcclerated(false);
		return;
	}
	else if (aResult != DDInterface::RESULT_OK)
	{
		if (Is3DAccelerated())
		{
			Set3DAcclerated(false);
			return;
		}
		else
		{
			Popup(GetString("FAILED_INIT_DIRECTDRAW", _S("Failed to initialize DirectDraw: ")) + StringToSexyString(DDInterface::ResultToString(aResult) + " " + mDDInterface->mErrorString));
			DoExit(1);
		}
	}
	*/

	bool isActive = mActive;

	mPhysMinimized = false;
	if (mMinimized)
	{
		if (mMuteOnLostFocus)
			Unmute(true);

		mMinimized = false;
		isActive = mActive; // set this here so we don't call RehupFocus again.
		mWindowInterface->RehupFocus();
	}
	
	if (isActive != mActive)
		mWindowInterface->RehupFocus();

	//ReInitImages();
	
	mWidgetManager->mImage = mWindowInterface->GetScreenImage();
	mWidgetManager->MarkAllDirty();

	//SetTimer(mHWnd, 100, mFrameTime, NULL);
}

void SexyAppBase::DeleteNativeImageData()
{
	unreachable();
	/* TODO
	MemoryImageSet::iterator anItr = mMemoryImageSet.begin();
	while (anItr != mMemoryImageSet.end())
	{
		MemoryImage* aMemoryImage = *anItr;		
		aMemoryImage->DeleteNativeData();			
		++anItr;
	}*/
}

void SexyAppBase::DeleteExtraImageData()
{
	unreachable();
	/* TODO
	AutoCrit anAutoCrit(mDDInterface->mCritSect);
	MemoryImageSet::iterator anItr = mMemoryImageSet.begin();
	while (anItr != mMemoryImageSet.end())
	{
		MemoryImage* aMemoryImage = *anItr;
		aMemoryImage->DeleteExtraBuffers();
		++anItr;
	}*/
}

void SexyAppBase::ReInitImages()
{
	unreachable();
	/* TODO
	MemoryImageSet::iterator anItr = mMemoryImageSet.begin();
	while (anItr != mMemoryImageSet.end())
	{
		MemoryImage* aMemoryImage = *anItr;				
		aMemoryImage->ReInit();
		++anItr;
	}*/
}


void SexyAppBase::LoadingThreadProc()
{
}

void SexyAppBase::LoadingThreadCompleted()
{
}

void SexyAppBase::LoadingThreadProcStub(void *theArg)
{
	SexyAppBase* aSexyApp = (SexyAppBase*) theArg;
	
	aSexyApp->LoadingThreadProc();		

	printf( "Resource Loading Time: %ldms\n", std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::high_resolution_clock::now() - aSexyApp->mTimeLoaded).count()
	);

	aSexyApp->mLoadingThreadCompleted = true;
}

void SexyAppBase::StartLoadingThread()
{
	if (!mLoadingThreadStarted)
	{
		mYieldMainThread = true; 
		//::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);		
		mLoadingThreadStarted = true;

		std::thread thread_obj(LoadingThreadProcStub, this);
		thread_obj.detach();
	}
}

/*
void SexyAppBase::CursorThreadProc()
{
	
	::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
	POINT aLastCursorPos = {0, 0};
	int aLastDrawCount = 0;

	while (!mShutdown)
	{
//		if (mProcessInTimer)
//			PostMessage(mHWnd,WM_TIMER,101,0);

		POINT aCursorPos;

		if (mPlayingDemoBuffer)
		{
			aCursorPos.x = mLastDemoMouseX;
			aCursorPos.y = mLastDemoMouseY;
		}
		else
		{
			::GetCursorPos(&aCursorPos);
			::ScreenToClient(mHWnd, &aCursorPos);
		}

		if (aLastDrawCount != mDrawCount)
		{
			// We did a draw so we may have committed a pending mNextCursorX/Y 
			aLastCursorPos.x = mDDInterface->mCursorX;
			aLastCursorPos.y = mDDInterface->mCursorY;
		}

		if ((aCursorPos.x != aLastCursorPos.x) ||
			(aCursorPos.y != aLastCursorPos.y))
		{	
			DWORD aTimeNow = timeGetTime();
			if (aTimeNow - mNextDrawTick > mDDInterface->mMillisecondsPerFrame + 5)
			{
				// Do the special drawing if we are rendering at less than full framerate				
				mDDInterface->SetCursorPos(aCursorPos.x, aCursorPos.y);
				aLastCursorPos = aCursorPos;
			}
			else
			{
				// Set them up to get assigned in the next screen redraw
				mDDInterface->mNextCursorX = aCursorPos.x;
				mDDInterface->mNextCursorY = aCursorPos.y;
			}			
		}		

		Sleep(10);
	}
	
	mCursorThreadRunning = false;
}*/


/*
void SexyAppBase::CursorThreadProcStub(void *theArg)
{
	
	CoInitialize(NULL);
	SexyAppBase* aSexyApp = (SexyAppBase*) theArg;
	aSexyApp->CursorThreadProc();
}*/

void SexyAppBase::SwitchScreenMode(bool wantWindowed, bool /*is3d*/, bool force)
{
	if (mForceFullscreen)
		wantWindowed = false;

	if (mIsWindowed == wantWindowed && !force)
	{
		//Set3DAcclerated(is3d);
		return;
	}

	unreachable();
	/* TODO

	// Set 3d acceleration preference
	Set3DAcclerated(is3d,false);

	// Always make the app windowed when playing demos, in order to
	//  make it easier to track down bugs.  We place this after the
	//  sanity check just so things get re-initialized and stuff
	//if (mPlayingDemoBuffer)
	//	wantWindowed = true;

	mIsWindowed = wantWindowed;	

	MakeWindow();
	
	// We need to do this check to allow IE to get focus instead of
	//  stealing it away for ourselves
	if (!mIsOpeningURL)
	{
		::ShowWindow(mHWnd, SW_NORMAL);
		::SetForegroundWindow(mHWnd);
	}
	else
	{
		// Show it but don't activate it
		::ShowWindow(mHWnd, SW_SHOWNOACTIVATE);
	}

	if (mSoundManager!=NULL)
	{
		mSoundManager->SetCooperativeWindow(mHWnd);
	}	

	mLastTime = timeGetTime();*/
}

void SexyAppBase::SwitchScreenMode(bool wantWindowed)
{
	SwitchScreenMode(wantWindowed, true);
}

void SexyAppBase::SwitchScreenMode()
{
	SwitchScreenMode(mIsWindowed, true, true);
}

void SexyAppBase::SetAlphaDisabled(bool isDisabled)
{
	if (mAlphaDisabled != isDisabled)
	{
		unreachable();
		/* TODO
		mAlphaDisabled = isDisabled;
		mDDInterface->SetVideoOnlyDraw(mAlphaDisabled);		
		mWidgetManager->mImage = mDDInterface->GetScreenImage();
		mWidgetManager->MarkAllDirty();
		*/
	}
}

/* TODO
void SexyAppBase::EnforceCursor()
{

	bool wantSysCursor = true;

	if (!mMouseIn)
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));	
		if (mDDInterface->SetCursorImage(NULL))
			mCustomCursorDirty = true;
	}
	else
	{
		if ((mCursorImages[mCursorNum] == NULL) || 
			((!mPlayingDemoBuffer) && (!mCustomCursorsEnabled) && (mCursorNum != CURSOR_CUSTOM)))
		{
			if (mOverrideCursor != NULL)
				::SetCursor(mOverrideCursor);
			else if (mCursorNum == CURSOR_POINTER)
				::SetCursor(::LoadCursor(NULL, IDC_ARROW));
			else if (mCursorNum == CURSOR_HAND)
				::SetCursor(mHandCursor);
			else if (mCursorNum == CURSOR_TEXT)
				::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
			else if (mCursorNum == CURSOR_DRAGGING)
				::SetCursor(mDraggingCursor);
			else if (mCursorNum == CURSOR_CIRCLE_SLASH)
				::SetCursor(::LoadCursor(NULL, IDC_NO));
			else if (mCursorNum == CURSOR_SIZEALL)
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			else if (mCursorNum == CURSOR_SIZENESW)
				::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			else if (mCursorNum == CURSOR_SIZENS)
				::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			else if (mCursorNum == CURSOR_SIZENWSE)
				::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			else if (mCursorNum == CURSOR_SIZEWE)
				::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			else if (mCursorNum == CURSOR_WAIT)
				::SetCursor(::LoadCursor(NULL, IDC_WAIT));
			else if (mCursorNum == CURSOR_CUSTOM) 
				::SetCursor(NULL); // Default to not showing anything
			else if (mCursorNum == CURSOR_NONE)
				::SetCursor(NULL);			
			else
				::SetCursor(::LoadCursor(NULL, IDC_ARROW));

			if (mDDInterface->SetCursorImage(NULL))
				mCustomCursorDirty = true;
		}
		else
		{
			if (mDDInterface->SetCursorImage(mCursorImages[mCursorNum]))
				mCustomCursorDirty = true;

			if (!mPlayingDemoBuffer)
			{
				::SetCursor(NULL);
			}
			else
			{
				// Give the NO cursor in the client area and an arrow on the title bar

				POINT aULCorner = {0, 0};
				::ClientToScreen(mHWnd, &aULCorner);

				POINT aBRCorner = {mWidth, mHeight};
				::ClientToScreen(mHWnd, &aBRCorner);

				POINT aPoint;
				::GetCursorPos(&aPoint);			
									
				if ((aPoint.x >= aULCorner.x) && (aPoint.y >= aULCorner.y) &&
					(aPoint.x < aBRCorner.x) && (aPoint.y < aBRCorner.y))
				{
					::SetCursor(::LoadCursor(NULL, IDC_NO));
				}
				else
				{
					::SetCursor(::LoadCursor(NULL, IDC_ARROW));
				}
			
			}

			wantSysCursor = false;
		}
	}

	if (wantSysCursor != mSysCursor)
	{
		mSysCursor = wantSysCursor;

		// Don't hide the hardware cursor when playing back a demo buffer
//		if (!mPlayingDemoBuffer)
//			::ShowCursor(mSysCursor);
	}
}*/

void SexyAppBase::ProcessSafeDeleteList()
{
	MTAutoDisallowRand aDisallowRand;

	WidgetSafeDeleteList::iterator anItr = mSafeDeleteList.begin();
	while (anItr != mSafeDeleteList.end())
	{
		WidgetSafeDeleteInfo* aWidgetSafeDeleteInfo = &(*anItr);
		if (mUpdateAppDepth <= aWidgetSafeDeleteInfo->mUpdateAppDepth)
		{
			delete aWidgetSafeDeleteInfo->mWidget;
			anItr = mSafeDeleteList.erase(anItr);
		}
		else
			++anItr;
	}
}

/*
void SexyAppBase::UpdateFTimeAcc()
{
	auto aCurTime = std::chrono::high_resolution_clock::now();
	
	//if (mLastTimeCheck != 0)
	//{			
	auto aDeltaTime = aCurTime - mLastTimeCheck;		

	mUpdateFTimeAcc = ((mUpdateFTimeAcc + aDeltaTime) < (std::chrono::milliseconds(200)) ? (mUpdateFTimeAcc + aDeltaTime) : (std::chrono::milliseconds(200)));

	if (mRelaxUpdateBacklogCount > 0) {
		auto aRelaxDelta = std::chrono::milliseconds(mRelaxUpdateBacklogCount) - aDeltaTime;
		mRelaxUpdateBacklogCount = 
			(aRelaxDelta > std::chrono::milliseconds(0)) ?
			std::chrono::duration_cast<std::chrono::milliseconds>(aRelaxDelta).count() :
			0;
	}
	//}

	mLastTimeCheck = aCurTime;
}*/

//int aNumCalls = 0;
//DWORD aLastCheck = 0;

/*
bool SexyAppBase::Process(bool allowSleep)
{
	if (mLoadingFailed)
		Shutdown();

	// Make sure we're not paused
	if (!mPaused)
	{	
		ProcessSafeDeleteList();
		DoUpdateFrames();
		DrawDirtyStuff();
		ProcessSafeDeleteList();
	}

	return true;
}*/

/*void SexyAppBase::DoMainLoop()
{
	Dialog* aDialog = NULL;
	if (theModalDialogId != -1)
	{
		aDialog = GetDialog(theModalDialogId);
		DBG_ASSERTE(aDialog != NULL);
		if (aDialog == NULL)
			return;
	}

	while (!mShutdown)
	{		
		MSG msg;
		while ((PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) && (!mShutdown))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ProcessDemo();		
		ProcessDeferredMessages();

		if ((aDialog != NULL) && (aDialog->mResult != -1))
			return;

		if (!mShutdown)
		{
			//++aCount;
			Process();
		}		
	}
}*/

void SexyAppBase::DoMainLoop()
{
	while (!mShutdown)
	{
		if (mExitToTop)
			mExitToTop = false;
		UpdateApp();
	}
}

/*==========================================================*
 |               — WARNING HERE BE DRAGONS —                |
 | UpdateAppStep is called in a loop by dialogs. This means |
 | ProcessSafeDeleteList()   can only  be    called  in  an |
 | invocation of this function different to DoUpdateFrames  |
 |                                                          |
 |    times attempted to fix this fucked up function: 1     |
 *==========================================================*/
bool SexyAppBase::UpdateAppStep(bool* updated)
{
	static auto timer = std::chrono::high_resolution_clock::now();
	constexpr auto frame_length = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(1.0/80));

	if (updated != NULL)
		*updated = false;

	if (mExitToTop)
		return false;

	mUpdateAppDepth++;
	
	if (mUpdateAppState == UPDATESTATE_PROCESS_DONE) {
		mUpdateAppState = UPDATESTATE_PROCESS_1;

		mWindowInterface->PollEvents();
	} else if (mUpdateAppState == UPDATESTATE_PROCESS_1) {
		if (updated != NULL) *updated = true;
		mUpdateAppState = UPDATESTATE_PROCESS_2;

		if (mLoadingFailed)
			Shutdown();

		// Make sure we're not paused
		if (!mPaused) {
			// update as many frames as needed before the next frame is to come
			double delta_ticks = (std::chrono::high_resolution_clock::now() - timer)/frame_length;

			DoUpdateFrames();
			timer += frame_length;
			// Are we ahead of drawing?
			if (delta_ticks < 1) DrawDirtyStuff();
		}
	} else if (mUpdateAppState == UPDATESTATE_PROCESS_2) {
		mUpdateAppState = UPDATESTATE_PROCESS_DONE;
		ProcessSafeDeleteList();

		constexpr auto time_offset = std::chrono::duration<double>(0);
		// Are we ahead of where the processing frames should be?
    	if (std::chrono::high_resolution_clock::now() - timer < time_offset) {
    		mSleepCount += 1;
    		std::this_thread::sleep_until(timer - time_offset);
    	}
	}

	mUpdateAppDepth--;

	return true;
}

bool SexyAppBase::UpdateApp()
{
	bool updated;
	for (;;)
	{
		if (!UpdateAppStep(&updated))
			return false;
		if (updated)
			return true;
	}
}

/*
int SexyAppBase::InitDDInterface()
{
	PreDDInterfaceInitHook();
	DeleteNativeImageData();
	int aResult = mDDInterface->Init(mHWnd, mIsPhysWindowed);
	DemoSyncRefreshRate();
	if ( DDInterface::RESULT_OK == aResult )
	{
		mScreenBounds.mX = ( mWidth - mDDInterface->mWidth ) / 2;
		mScreenBounds.mY = ( mHeight - mDDInterface->mHeight ) / 2;
		mScreenBounds.mWidth = mDDInterface->mWidth;
		mScreenBounds.mHeight = mDDInterface->mHeight;
		mWidgetManager->Resize(mScreenBounds, mDDInterface->mPresentationRect);
		PostDDInterfaceInitHook();
	}
	return aResult;
	
}*/

void SexyAppBase::PreTerminate()
{
}

void SexyAppBase::Start()
{
	if (mShutdown)
		return;

	if (mAutoStartLoadingThread)
		StartLoadingThread();

	mWindowInterface->ShowWindow();
	/* FIXME
	::ShowWindow(mHWnd, SW_SHOW);	
	::SetFocus(mHWnd);

	timeBeginPeriod(1);
	*/

	//int aCount = 0; // unused
	//int aSleepCount = 0; // unused

	auto aStartTime = std::chrono::high_resolution_clock::now();		

	mRunning = true;
	mLastTime = aStartTime;
	mLastUserInputTick = aStartTime;
	mLastTimerTime = aStartTime;

	DoMainLoop();
	ProcessSafeDeleteList();

	mRunning = false;

	WaitForLoadingThread();

	//char aString[256];
	printf("Seconds       = %g\n", std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - aStartTime).count());
	//sprintf(aString, "Count         = %d\r\n", aCount);
	//sprintf(aString, "Sleep Count   = %d\r\n", mSleepCount);
	printf("Sleep Count   = %d\n", mSleepCount);
	//sprintf(aString, "Update Count  = %d\r\n", mUpdateCount);
	printf("Update Count  = %d\n", mUpdateCount);
	//sprintf(aString, "Draw Count    = %d\r\n", mDrawCount);
	printf("Draw Count    = %d\n", mDrawCount);
	//OutputDebugStringA(aString);
	printf("Draw Time     = %ld\n", std::chrono::duration_cast<std::chrono::milliseconds>(mDrawTime).count());
	//sprintf(aString, "Draw Time     = %d\r\n", mDrawTime);
	printf("Screen Blt    = %ld\n", std::chrono::duration_cast<std::chrono::milliseconds>(mScreenBltTime).count());
	//sprintf(aString, "Screen Blt    = %d\r\n", mScreenBltTime);
	
	//OutputDebugStringA(aString);
	if (mDrawTime+mScreenBltTime > std::chrono::milliseconds(0))
	{
		printf("Avg FPS       = %d\n", mDrawCount*1000/((int)(1000*(std::chrono::duration_cast<std::chrono::duration<float>>(mDrawTime+mScreenBltTime)).count())));
		//sprintf(aString, "Avg FPS       = %d\r\n", (mDrawCount*1000)/(mDrawTime+mScreenBltTime));
	}

	//timeEndPeriod(1);	

	PreTerminate();

	WriteToRegistry();
}

/*
bool SexyAppBase::CheckSignature(const Buffer& theBuffer, const std::string& theFileName)
{
	(void)theBuffer;(void)theFileName;
	// Add your own signature checking code here
	return false;
}
*/

bool SexyAppBase::LoadProperties(const std::string& theFileName, bool required, bool checkSig)
{
	Buffer aBuffer;
	if (!ReadBufferFromFile(theFileName, &aBuffer))
	{
		if (!required)
			return true;
		else
		{
			Popup(GetString("UNABLE_OPEN_PROPERTIES", _S("Unable to open properties file ")) + StringToSexyString(theFileName));
			return false;
		}
	}
	if (checkSig)
	{
		//if (!CheckSignature(aBuffer, theFileName))
		//{
			//Popup(GetString("PROPERTIES_SIG_FAILED", _S("Signature check failed on ")) + StringToSexyString(theFileName + "'"));
			//return false;
		//}
	}

	PropertiesParser aPropertiesParser(this);

	// Load required language-file properties
		if (!aPropertiesParser.ParsePropertiesBuffer(aBuffer))
		{
			Popup(aPropertiesParser.GetErrorText());		
			return false;
		}
		else
			return true;
}

bool SexyAppBase::LoadProperties()
{
	// Load required language-file properties
	return LoadProperties("properties/default.xml", true, false);
}

void SexyAppBase::LoadResourceManifest()
{
	if (!mResourceManager->ParseResourcesFile("properties/resources.xml"))
		ShowResourceError(true);
}

void SexyAppBase::ShowResourceError(bool doExit)
{
	Popup(mResourceManager->GetErrorText());	
	if (doExit)
		DoExit(0);
}

bool SexyAppBase::GetBoolean(const std::string& theId)
{
	StringBoolMap::iterator anItr = mBoolProperties.find(theId);
	DBG_ASSERTE(anItr != mBoolProperties.end());
	
	if (anItr != mBoolProperties.end())	
		return anItr->second;
	else
		return false;
}

bool SexyAppBase::GetBoolean(const std::string& theId, bool theDefault)
{
	StringBoolMap::iterator anItr = mBoolProperties.find(theId);	
	
	if (anItr != mBoolProperties.end())	
		return anItr->second;
	else
		return theDefault;	
}

int SexyAppBase::GetInteger(const std::string& theId)
{
	StringIntMap::iterator anItr = mIntProperties.find(theId);
	DBG_ASSERTE(anItr != mIntProperties.end());
	
	if (anItr != mIntProperties.end())	
		return anItr->second;
	else
		return false;
}

int SexyAppBase::GetInteger(const std::string& theId, int theDefault)
{
	StringIntMap::iterator anItr = mIntProperties.find(theId);	
	
	if (anItr != mIntProperties.end())	
		return anItr->second;
	else
		return theDefault;	
}

double SexyAppBase::GetDouble(const std::string& theId)
{
	StringDoubleMap::iterator anItr = mDoubleProperties.find(theId);
	DBG_ASSERTE(anItr != mDoubleProperties.end());
	
	if (anItr != mDoubleProperties.end())	
		return anItr->second;
	else
		return false;
}

double SexyAppBase::GetDouble(const std::string& theId, double theDefault)
{
	StringDoubleMap::iterator anItr = mDoubleProperties.find(theId);	
	
	if (anItr != mDoubleProperties.end())	
		return anItr->second;
	else
		return theDefault;	
}

SexyString SexyAppBase::GetString(const std::string& theId)
{
	StringWStringMap::iterator anItr = mStringProperties.find(theId);
	DBG_ASSERTE(anItr != mStringProperties.end());
	
	if (anItr != mStringProperties.end())	
		return WStringToSexyString(anItr->second);
	else
		return _S("");
}

SexyString SexyAppBase::GetString(const std::string& theId, const SexyString& theDefault)
{
	StringWStringMap::iterator anItr = mStringProperties.find(theId);	
	
	if (anItr != mStringProperties.end())	
		return WStringToSexyString(anItr->second);
	else
		return theDefault;	
}

StringVector SexyAppBase::GetStringVector(const std::string& theId)
{
	StringStringVectorMap::iterator anItr = mStringVectorProperties.find(theId);
	DBG_ASSERTE(anItr != mStringVectorProperties.end());
	
	if (anItr != mStringVectorProperties.end())	
		return anItr->second;
	else
		return StringVector();
}

void SexyAppBase::SetString(const std::string& theId, const std::wstring& theValue)
{
	std::pair<StringWStringMap::iterator, bool> aPair = mStringProperties.insert(StringWStringMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}


void SexyAppBase::SetBoolean(const std::string& theId, bool theValue)
{
	std::pair<StringBoolMap::iterator, bool> aPair = mBoolProperties.insert(StringBoolMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}

void SexyAppBase::SetInteger(const std::string& theId, int theValue)
{
	std::pair<StringIntMap::iterator, bool> aPair = mIntProperties.insert(StringIntMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}

void SexyAppBase::SetDouble(const std::string& theId, double theValue)
{
	std::pair<StringDoubleMap::iterator, bool> aPair = mDoubleProperties.insert(StringDoubleMap::value_type(theId, theValue));
	if (!aPair.second) // Found it, change value
		aPair.first->second = theValue;
}

void SexyAppBase::DoParseCmdLine(int argc, char* argv[])
{
	/*
	char* aCmdLine = GetCommandLineA();	
	char* aCmdLinePtr = aCmdLine;
	if (aCmdLinePtr[0] == '"')
	{
		aCmdLinePtr = strchr(aCmdLinePtr + 1, '"');
		if (aCmdLinePtr != NULL)
			aCmdLinePtr++;
	}*/

	// something to parse?
	if (argc > 1) {
		std::string aCmdLine{}; // Create a space seprated list of command line arguments

		aCmdLine.append(argv[1]);
		for (int i = 2; i < argc; ++i) {
			aCmdLine.append(" ").append(argv[i]);
		}
		ParseCmdLine(aCmdLine);
	}

	mCmdLineParsed = true;
}

void SexyAppBase::ParseCmdLine(const std::string& theCmdLine)
{
	// Command line example:  -play -demofile="game demo.dmo"
	// Results in HandleCmdLineParam("-play", ""); HandleCmdLineParam("-demofile", "game demo.dmo");
	std::string aCurParamName;
	std::string aCurParamValue;

	//int aSpacePos = 0; // Unused
	bool inQuote = false;
	bool onValue = false;

	for (size_t i = 0; i < theCmdLine.length(); i++)
	{
		char c = theCmdLine[i];
		bool atEnd = false;
		
		if (c == '"')
		{
			inQuote = !inQuote;

			if (!inQuote)
				atEnd = true;
		}
		else if ((c == ' ') && (!inQuote))
			atEnd = true;
		else if (c == '=')
			onValue = true;
		else if (onValue)
			aCurParamValue += c;
		else
			aCurParamName += c;
		
		if (i == theCmdLine.length() - 1)
			atEnd = true;
		
		if (atEnd && !aCurParamName.empty())
		{
			HandleCmdLineParam(aCurParamName, aCurParamValue);
			aCurParamName = "";
			aCurParamValue = "";
			onValue = false;
		}	
	}
}

static int GetMaxDemoFileNum(const std::string& /*theDemoPrefix*/, int /*theMaxToKeep*/, bool /*doErase*/)
{
	unreachable();
	/* TODO
	WIN32_FIND_DATAA aData;
	HANDLE aHandle = FindFirstFileA((theDemoPrefix + "*.dmo").c_str(), &aData);
	if (aHandle==INVALID_HANDLE_VALUE)
		return 0;

	typedef std::set<int> IntSet;
	IntSet aSet;

	do {
		int aNum = 0;
		if (sscanf(aData.cFileName,(theDemoPrefix + "%d.dmo").c_str(), &aNum)==1)
			aSet.insert(aNum);

	} while(FindNextFileA(aHandle,&aData));
	FindClose(aHandle);

	IntSet::iterator anItr = aSet.begin();
	if ((int)aSet.size()>theMaxToKeep-1 && doErase)
		DeleteFile(StrFormat((theDemoPrefix + "%d.dmo").c_str(),*anItr).c_str());

	if (aSet.empty())
		return 0;

	anItr = aSet.end();
	--anItr;
	return (*anItr);
	*/
}

void SexyAppBase::HandleCmdLineParam(const std::string& theParamName, const std::string& theParamValue)
{
	if (theParamName == "-play")
	{
		mPlayingDemoBuffer = true;
		mRecordingDemoBuffer = false;
	}
	else if (theParamName == "-recnum")
	{
		int aNum = atoi(theParamValue.c_str());
		if (aNum<=0)
			aNum=5;

		int aDemoFileNum = GetMaxDemoFileNum(mDemoPrefix, aNum, true) + 1;
		mDemoFileName = SexyStringToString(StrFormat(StringToSexyString(mDemoPrefix + "%d.dmo").c_str(),aDemoFileNum));
		if (mDemoFileName.length() < 2 || (mDemoFileName[1] != ':' && mDemoFileName[2] != '\\'))
		{
			mDemoFileName = GetAppDataFolder() + mDemoFileName;
		}
		mRecordingDemoBuffer = true;
		mPlayingDemoBuffer = false;
	}
	else if (theParamName == "-playnum")
	{
		int aNum = atoi(theParamValue.c_str())-1;
		if (aNum<0)
			aNum=0;

		int aDemoFileNum = GetMaxDemoFileNum(mDemoPrefix, aNum, false)-aNum;
		mDemoFileName = SexyStringToString(StrFormat(StringToSexyString(mDemoPrefix + "%d.dmo").c_str(),aDemoFileNum));
		mRecordingDemoBuffer = false;
		mPlayingDemoBuffer = true;
	}
	else if (theParamName == "-record")
	{
		mRecordingDemoBuffer = true;
		mPlayingDemoBuffer = false;
	}
	else if (theParamName == "-demofile")
	{
		mDemoFileName = theParamValue;
		if (mDemoFileName.length() < 2 || (mDemoFileName[1] != ':' && mDemoFileName[2] != '\\'))
		{
			mDemoFileName = GetAppDataFolder() + mDemoFileName;
		}
	}	
	else if (theParamName == "-crash")
	{
		// Try to access NULL
		char* a = 0;
		*a = '!';
	}
	else if (theParamName == "-screensaver")
	{
		mIsScreenSaver = true;
	}
	else if (theParamName == "-changedir")
	{
		mChangeDirTo = theParamValue;
	}
	else
	{
		Popup(GetString("INVALID_COMMANDLINE_PARAM", _S("Invalid command line parameter: ")) + StringToSexyString(theParamName));
		DoExit(0);
	}
}

void SexyAppBase::PreDisplayHook()
{
}

/*
void SexyAppBase::PreDDInterfaceInitHook()
{
}

void SexyAppBase::PostDDInterfaceInitHook()
{
}*/

bool SexyAppBase::ChangeDirHook(const char *theIntendedPath)
{
	(void)theIntendedPath;
	return false;
}

MusicInterface* SexyAppBase::CreateMusicInterface()
{
	if (mNoSoundNeeded)
		return new DummyMusicInterface();
	else if (mWantFMod) {
		unreachable();
		/* TODO
		return new FModMusicInterface();*/
	}
	else {
		printf("Bass dll currently loaded without HWnd, likely to cause problems on Windows\n");
		return new BassMusicInterface(NULL);
	}
}

void SexyAppBase::InitPropertiesHook()
{
}

void SexyAppBase::InitHook()
{
}

void SexyAppBase::Init()
{
	mPrimaryThreadId = std::this_thread::get_id();	
	
	if (mShutdown)
		return;

	/*
	if (gDDrawDLL==NULL || gDSoundDLL==NULL)
	{
		MessageBox(NULL, 
						GetString("APP_REQUIRES_DIRECTX", _S("This application requires DirectX to run.  You can get DirectX at http://www.microsoft.com/directx")).c_str(),
						GetString("YOU_NEED_DIRECTX", _S("You need DirectX")).c_str(), 
						MB_OK | MB_ICONERROR);
		DoExit(0);
	}*/

	InitPropertiesHook();

	mRegKey = "PlantsVsZombies";
	mRegHandle = std::make_unique<RegistryEmulator>();
	ReadFromRegistry();	

	/* TODO
	if (CheckForVista())
	{
		HMODULE aMod;
		SHGetFolderPathFunc aFunc = (SHGetFolderPathFunc)GetSHGetFolderPath("shell32.dll", &aMod);
		if (aFunc == NULL || aMod == NULL)
			// SHGetFolderPathFunc is shadowed!
			// SHGetFolderPathFunc aFunc = (SHGetFolderPathFunc)GetSHGetFolderPath("shfolder.dll", &aMod);
			aFunc = (SHGetFolderPathFunc)GetSHGetFolderPath("shfolder.dll", &aMod);

		if (aMod != NULL)
		{
			char aPath[MAX_PATH];
			aFunc(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, aPath);

			std::string aDataPath = RemoveTrailingSlash(aPath) + "\\" + mFullCompanyName + "\\" + mProdName;
			SetAppDataFolder(aDataPath + "\\");
			//MkDir(aDataPath);
			//AllowAllAccess(aDataPath);
			if (mDemoFileName.length() < 2 || (mDemoFileName[1] != ':' && mDemoFileName[2] != '\\'))
			{
				mDemoFileName = GetAppDataFolder() + mDemoFileName;
			}

			FreeLibrary(aMod);
		}
	}*/
	
	if (!mCmdLineParsed) {
		mCmdLineParsed = true;
	}

	if (IsScreenSaver())	
		mOnlyAllowOneCopyToRun = false;	

	/* TODO
	if(gHInstance==NULL)
		gHInstance = (HINSTANCE)GetModuleHandle(NULL);
	*/

	// Change directory

	if (!ChangeDirHook(mChangeDirTo.c_str()))
		chdir(mChangeDirTo.c_str());

	/*
	gPakInterface->AddPakFile("main.pak");
	*/

	/* TODO
	// Create a message we can use to talk to ourselves inter-process
	mNotifyGameMessage = RegisterWindowMessage((_S("Notify") + StringToSexyString(mProdName)).c_str());

	// Create a globally unique mutex
	mMutex = CreateMutex(NULL, TRUE, (StringToSexyString(mProdName) + _S("Mutex")).c_str());
	
	
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
		HandleGameAlreadyRunning();
	*/

	mRandSeed = clock();
	SRand(mRandSeed);	

	// Set up demo recording stuff
	if (mPlayingDemoBuffer)
	{
		std::string anError;
		if (!ReadDemoBuffer(anError))
		{
			mPlayingDemoBuffer = false;
			Popup(anError);
			DoExit(0);
		}
	}

	srand(clock());

	mIsWideWindow = sizeof(SexyChar) == sizeof(wchar_t);

	/* TODO
	WNDCLASS wc;
	wc.style = CS_DBLCLKS;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = NULL;
	wc.hCursor = NULL;
	wc.hIcon = ::LoadIconA(gHInstance, "IDI_MAIN_ICON");
	wc.hInstance = gHInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = _S("MainWindow");
	wc.lpszMenuName = NULL;	
	bool success = RegisterClass(&wc) != 0;
	(void)success; // success unused in release mode
	DBG_ASSERTE(success);

	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = NULL;
	wc.hCursor = NULL;
	wc.hIcon = NULL;
	wc.hInstance = gHInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = _S("InvisWindow");
	wc.lpszMenuName = NULL;	
	success = RegisterClass(&wc) != 0;
	DBG_ASSERTE(success);*/

	/* TODO
	mInvisHWnd = CreateWindowEx(
			0,
			_S("InvisWindow"),
			mTitle.c_str(),
			0,
			0,
			0,
			0,
			0,
			NULL,
			NULL,
			gHInstance,
			0);	
	SetWindowLongPtr(mInvisHWnd, GWLP_USERDATA, (intptr_t) this);
	*/

	// Let app do something before showing window, or switching to fullscreen mode
	// NOTE: Moved call to PreDisplayHook above mIsWindowed and GetSystemsMetrics
	// checks because the checks below use values that could change in PreDisplayHook.
	// PreDisplayHook must call mWidgetManager->Resize if it changes mWidth or mHeight.

	PreDisplayHook();

	mWidgetManager->Resize(Rect(0, 0, mWidth, mHeight), Rect(0, 0, mWidth, mHeight));

	// Check to see if we CAN run windowed or not...
	/* @Minerscale: We can. It's <current year>
	if (mIsWindowed && !mFullScreenWindow)
	{
		// How can we be windowed if our screen isn't even big enough?
		if ((mWidth >= GetSystemMetrics(SM_CXFULLSCREEN)) ||
			(mHeight >= GetSystemMetrics(SM_CYFULLSCREEN)))
		{
			mIsWindowed = false;
			mForceFullscreen = true;
		}
	}*/

	if (mFullScreenWindow) // change resoultion using ChangeDisplaySettings
	{
		unreachable();
		/* TODO
		EnumWindows(ChangeDisplayWindowEnumProc,0); // record window pos
		DEVMODE dm;
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm );

		// Switch resolutions
		if (dm.dmPelsWidth!=(unsigned int)mWidth || dm.dmPelsHeight!=(unsigned int)mHeight || (dm.dmBitsPerPel!=16 && dm.dmBitsPerPel!=32))
		{
			dm.dmPelsWidth = mWidth;
			dm.dmPelsHeight = mHeight;
			dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

			if (dm.dmBitsPerPel!=16 && dm.dmBitsPerPel!=32) // handle 24-bit/256 color case
			{
				dm.dmBitsPerPel = 16;
				dm.dmFields |= DM_BITSPERPEL;
			}

			if (ChangeDisplaySettings(&dm,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			{
				mFullScreenWindow = false;
				mIsWindowed = false;
			}
		}*/
	}

	MakeWindow();

	mHandCursor = mWindowInterface->CreateCursor(11, 4, 32, 32, gFingerCursorData, gFingerCursorData+sizeof(gFingerCursorData)/2); 
	mDraggingCursor = mWindowInterface->CreateCursor(15, 10, 32, 32, gDraggingCursorData, gDraggingCursorData+sizeof(gDraggingCursorData)/2);

	if (mPlayingDemoBuffer)
	{
		// Get video data
		
		PrepareDemoCommand(true);
		mDemoNeedsCommand = true;
		
		DBG_ASSERTE(!mDemoIsShortCmd);
		DBG_ASSERTE(mDemoCmdNum == DEMO_VIDEO_DATA);

		mIsWindowed = mDemoBuffer.ReadBoolean();
		mSyncRefreshRate = mDemoBuffer.ReadByte();
	}

	if (mSoundManager == NULL) {
		// TODO add HWnd information to bass
		mSoundManager = new BassSoundManager(NULL);
	}

	SetSfxVolume(mSfxVolume);
	
	mMusicInterface = CreateMusicInterface();	

	SetMusicVolume(mMusicVolume);	

	if (IsScreenSaver())
	{
		SetCursor(CURSOR_NONE);
	}

	InitHook();

	mInitialized = true;
}

void SexyAppBase::HandleGameAlreadyRunning()
{
	if(mOnlyAllowOneCopyToRun)
	{
		unreachable();
		/* TODO
		// Notify the other window and then shut ourselves down
		if (mNotifyGameMessage != 0)
			PostMessage(HWND_BROADCAST, mNotifyGameMessage, 0, 0);
		*/

		DoExit(0);
	}
}

void SexyAppBase::CopyToClipboard(const std::string& /*theString*/)
{
	if (mPlayingDemoBuffer)
		return;

	unreachable();
	/* TODO
	HGLOBAL				aGlobalHandle;
	char*				theData;	
	WCHAR*				theWData;

	if (OpenClipboard(mHWnd))
	{
		EmptyClipboard();

		aGlobalHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, theString.length()+1);
		theData = (char*) GlobalLock(aGlobalHandle);
		strcpy(theData, theString.c_str());
		GlobalUnlock(aGlobalHandle);
		SetClipboardData(CF_TEXT, aGlobalHandle);
		SetClipboardData(CF_OEMTEXT, aGlobalHandle);		
		SetClipboardData(CF_LOCALE, aGlobalHandle);

		int aSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, theString.c_str(), theString.length(), NULL, 0);
		aGlobalHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (aSize + 1) * sizeof(WCHAR));
		theWData = (WCHAR*) GlobalLock(aGlobalHandle);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, theString.c_str(), theString.length(), theWData, aSize);
		theWData[aSize] = '\0';
		GlobalUnlock(aGlobalHandle);
		SetClipboardData(CF_UNICODETEXT, aGlobalHandle);
		
		CloseClipboard();
	}
	*/
}

std::string	SexyAppBase::GetClipboard()
{
	unreachable();
	/* TODO
	HGLOBAL				aGlobalHandle;	
	std::string			aString;

	if (!mPlayingDemoBuffer)
	{
		if (OpenClipboard(mHWnd))
		{
			aGlobalHandle = GetClipboardData(CF_TEXT);	
			if (aGlobalHandle != NULL)
			{
				char* theData = (char*) GlobalLock(aGlobalHandle);
				if (theData != NULL)
				{
					aString = theData;		
					GlobalUnlock(aGlobalHandle);
				}
			}
			
			CloseClipboard();
		}
	}

	DemoSyncString(&aString);

	return aString;*/
}

void SexyAppBase::SetCursor(int theCursorNum)
{
	mCursorNum = theCursorNum;
	mWindowInterface->EnforceCursor();
}

int SexyAppBase::GetCursor()
{
	return mCursorNum;
}

void SexyAppBase::EnableCustomCursors(bool enabled)
{
	mCustomCursorsEnabled = enabled;
}

std::unique_ptr<Sexy::Image> SexyAppBase::GetImage(const std::string &theFileName, bool theDoImageSanding)
{
	std::unique_ptr<ImageLib::Image> aLoadedImage = ImageLib::GetImage(theFileName, true, theDoImageSanding);
	
	if (aLoadedImage == nullptr)
		return nullptr;

	return std::make_unique<Vk::VkImage>(*aLoadedImage);
}

/*
Sexy::DDImage* SexyAppBase::CreateCrossfadeImage(Sexy::Image* theImage1, const Rect& theRect1, Sexy::Image* theImage2, const Rect& theRect2, double theFadeFactor)
{
	MemoryImage* aMemoryImage1 = dynamic_cast<MemoryImage*>(theImage1);
	MemoryImage* aMemoryImage2 = dynamic_cast<MemoryImage*>(theImage2);

	if ((aMemoryImage1 == NULL) || (aMemoryImage2 == NULL))
		return NULL;

	if ((theRect1.mX < 0) || (theRect1.mY < 0) || 
		(theRect1.mX + theRect1.mWidth > theImage1->GetWidth()) ||
		(theRect1.mY + theRect1.mHeight > theImage1->GetHeight()))
	{
		DBG_ASSERTE("Crossfade Rect1 out of bounds");
		return NULL;
	}

	if ((theRect2.mX < 0) || (theRect2.mY < 0) || 
		(theRect2.mX + theRect2.mWidth > theImage2->GetWidth()) ||
		(theRect2.mY + theRect2.mHeight > theImage2->GetHeight()))
	{
		DBG_ASSERTE("Crossfade Rect2 out of bounds");
		return NULL;
	}

	int aWidth = theRect1.mWidth;
	int aHeight = theRect1.mHeight;

	DDImage* anImage = new DDImage(mDDInterface);
	anImage->Create(aWidth, aHeight);

	ulong* aDestBits = anImage->GetBits();
	ulong* aSrcBits1 = aMemoryImage1->GetBits();
	ulong* aSrcBits2 = aMemoryImage2->GetBits();

	int aSrc1Width = aMemoryImage1->GetWidth();
	int aSrc2Width = aMemoryImage2->GetWidth();
	ulong aMult = (int) (theFadeFactor*256);
	ulong aOMM = (256 - aMult);

	for (int y = 0; y < aHeight; y++)
	{
		ulong* s1 = &aSrcBits1[(y+theRect1.mY)*aSrc1Width+theRect1.mX];
		ulong* s2 = &aSrcBits2[(y+theRect2.mY)*aSrc2Width+theRect2.mX];
		ulong* d = &aDestBits[y*aWidth];

		for (int x = 0; x < aWidth; x++)
		{
			ulong p1 = *s1++;
			ulong p2 = *s2++;

			//p1 = 0;
			//p2 = 0xFFFFFFFF;

			*d++ = 
				((((p1 & 0x000000FF)*aOMM + (p2 & 0x000000FF)*aMult)>>8) & 0x000000FF) |
				((((p1 & 0x0000FF00)*aOMM + (p2 & 0x0000FF00)*aMult)>>8) & 0x0000FF00) |
				((((p1 & 0x00FF0000)*aOMM + (p2 & 0x00FF0000)*aMult)>>8) & 0x00FF0000) |
				((((p1 >> 24)*aOMM + (p2 >> 24)*aMult)<<16) & 0xFF000000);
		}
	}

	anImage->BitsChanged();
	
	return anImage;
}*/

void SexyAppBase::ColorizeImage(Image* /*theImage*/, const Color& /*theColor*/)
{
	unreachable();
	/* TODO
	MemoryImage* aSrcMemoryImage = dynamic_cast<MemoryImage*>(theImage);

	if (aSrcMemoryImage == NULL)
		return;

	ulong* aBits;	
	int aNumColors;

	if (aSrcMemoryImage->mColorTable == NULL)
	{
		aBits = aSrcMemoryImage->GetBits();		
		aNumColors = theImage->GetWidth()*theImage->GetHeight();				
	}
	else
	{
		aBits = aSrcMemoryImage->mColorTable;		
		aNumColors = 256;				
	}
						
	if ((theColor.mAlpha <= 255) && (theColor.mRed <= 255) && 
		(theColor.mGreen <= 255) && (theColor.mBlue <= 255))
	{
		for (int i = 0; i < aNumColors; i++)
		{
			ulong aColor = aBits[i];

			aBits[i] = 
				((((aColor & 0xFF000000) >> 8) * theColor.mAlpha) & 0xFF000000) |
				((((aColor & 0x00FF0000) * theColor.mRed) >> 8) & 0x00FF0000) |
				((((aColor & 0x0000FF00) * theColor.mGreen) >> 8) & 0x0000FF00)|
				((((aColor & 0x000000FF) * theColor.mBlue) >> 8) & 0x000000FF);
		}
	}
	else
	{
		for (int i = 0; i < aNumColors; i++)
		{
			ulong aColor = aBits[i];

			int aAlpha = ((aColor >> 24) * theColor.mAlpha) / 255;
			int aRed = (((aColor >> 16) & 0xFF) * theColor.mRed) / 255;
			int aGreen = (((aColor >> 8) & 0xFF) * theColor.mGreen) / 255;
			int aBlue = ((aColor & 0xFF) * theColor.mBlue) / 255;

			if (aAlpha > 255)
				aAlpha = 255;
			if (aRed > 255)
				aRed = 255;
			if (aGreen > 255)
				aGreen = 255;
			if (aBlue > 255)
				aBlue = 255;

			aBits[i] = (aAlpha << 24) | (aRed << 16) | (aGreen << 8) | (aBlue);
		}
	}	

	aSrcMemoryImage->BitsChanged();*/
}

/*
DDImage* SexyAppBase::CreateColorizedImage(Image* theImage, const Color& theColor)
{
	MemoryImage* aSrcMemoryImage = dynamic_cast<MemoryImage*>(theImage);

	if (aSrcMemoryImage == NULL)
		return NULL;

	DDImage* anImage = new DDImage(mDDInterface);
	
	anImage->Create(theImage->GetWidth(), theImage->GetHeight());
	
	ulong* aSrcBits;
	ulong* aDestBits;
	int aNumColors;

	if (aSrcMemoryImage->mColorTable == NULL)
	{
		aSrcBits = aSrcMemoryImage->GetBits();
		aDestBits = anImage->GetBits();
		aNumColors = theImage->GetWidth()*theImage->GetHeight();				
	}
	else
	{
		aSrcBits = aSrcMemoryImage->mColorTable;
		aDestBits = anImage->mColorTable = new ulong[256];
		aNumColors = 256;
		
		anImage->mColorIndices = new uchar[anImage->mWidth*theImage->mHeight];
		memcpy(anImage->mColorIndices, aSrcMemoryImage->mColorIndices, anImage->mWidth*theImage->mHeight);
	}
						
	if ((theColor.mAlpha <= 255) && (theColor.mRed <= 255) && 
		(theColor.mGreen <= 255) && (theColor.mBlue <= 255))
	{
		for (int i = 0; i < aNumColors; i++)
		{
			ulong aColor = aSrcBits[i];

			aDestBits[i] = 
				((((aColor & 0xFF000000) >> 8) * theColor.mAlpha) & 0xFF000000) |
				((((aColor & 0x00FF0000) * theColor.mRed) >> 8) & 0x00FF0000) |
				((((aColor & 0x0000FF00) * theColor.mGreen) >> 8) & 0x0000FF00)|
				((((aColor & 0x000000FF) * theColor.mBlue) >> 8) & 0x000000FF);
		}
	}
	else
	{
		for (int i = 0; i < aNumColors; i++)
		{
			ulong aColor = aSrcBits[i];

			int aAlpha = ((aColor >> 24) * theColor.mAlpha) / 255;
			int aRed = (((aColor >> 16) & 0xFF) * theColor.mRed) / 255;
			int aGreen = (((aColor >> 8) & 0xFF) * theColor.mGreen) / 255;
			int aBlue = ((aColor & 0xFF) * theColor.mBlue) / 255;

			if (aAlpha > 255)
				aAlpha = 255;
			if (aRed > 255)
				aRed = 255;
			if (aGreen > 255)
				aGreen = 255;
			if (aBlue > 255)
				aBlue = 255;

			aDestBits[i] = (aAlpha << 24) | (aRed << 16) | (aGreen << 8) | (aBlue);
		}
	}	

	anImage->BitsChanged();

	return anImage;
}*/

/*
DDImage* SexyAppBase::CopyImage(Image* theImage, const Rect& theRect)
{
	DDImage* anImage = new DDImage(mDDInterface);

	anImage->Create(theRect.mWidth, theRect.mHeight);
	
	Graphics g(anImage);
	g.DrawImage(theImage, -theRect.mX, -theRect.mY);

	anImage->CopyAttributes(theImage);

	return anImage;
}*/

/*
DDImage* SexyAppBase::CopyImage(Image* theImage)
{
	return CopyImage(theImage, Rect(0, 0, theImage->GetWidth(), theImage->GetHeight()));
}*/

void SexyAppBase::MirrorImage(Image* /*theImage*/)
{
	unreachable();
	/* TODO
	MemoryImage* aSrcMemoryImage = dynamic_cast<MemoryImage*>(theImage);	

	uint32_t* aSrcBits = aSrcMemoryImage->GetBits();

	int aPhysSrcWidth = aSrcMemoryImage->mWidth;
	for (int y = 0; y < aSrcMemoryImage->mHeight; y++)
	{
		uint32_t* aLeftBits = aSrcBits + (y * aPhysSrcWidth);		
		uint32_t* aRightBits = aLeftBits + (aPhysSrcWidth - 1);

		for (int x = 0; x < (aPhysSrcWidth >> 1); x++)
		{
			uint32_t aSwap = *aLeftBits;

			*(aLeftBits++) = *aRightBits;
			*(aRightBits--) = aSwap;
		}
	}

	aSrcMemoryImage->BitsChanged();*/
}

void SexyAppBase::FlipImage(Image* /*theImage*/)
{
	unreachable();
	/* TODO
	MemoryImage* aSrcMemoryImage = dynamic_cast<MemoryImage*>(theImage);

	uint32_t* aSrcBits = aSrcMemoryImage->GetBits();

	int aPhysSrcHeight = aSrcMemoryImage->mHeight;
	int aPhysSrcWidth = aSrcMemoryImage->mWidth;
	for (int x = 0; x < aPhysSrcWidth; x++)
	{
		uint32_t* aTopBits    = aSrcBits + x;
		uint32_t* aBottomBits = aTopBits + (aPhysSrcWidth * (aPhysSrcHeight - 1));

		for (int y = 0; y < (aPhysSrcHeight >> 1); y++)
		{
			uint32_t aSwap = *aTopBits;

			*aTopBits = *aBottomBits;
			aTopBits += aPhysSrcWidth;
			*aBottomBits = aSwap;
			aBottomBits -= aPhysSrcWidth;
		}
	}

	aSrcMemoryImage->BitsChanged();	*/
}

/*
void SexyAppBase::RotateImageHue(Sexy::MemoryImage *theImage, int theDelta)
{
	while (theDelta < 0)
		theDelta += 256;

	int aSize = theImage->mWidth * theImage->mHeight;
	DWORD *aPtr = theImage->GetBits();
	for (int i=0; i<aSize; i++)
	{
		DWORD aPixel = *aPtr;
		int alpha = aPixel&0xff000000;
		int r = (aPixel>>16)&0xff;
		int g = (aPixel>>8) &0xff;
		int b = aPixel&0xff;

		int maxval = std::max(r, std::max(g, b));
		int minval = std::min(r, std::min(g, b));
		int h = 0;
		int s = 0;
		int l = (minval+maxval)/2;
		int delta = maxval - minval;

		if (delta != 0)
		{			
			s = (delta * 256) / ((l <= 128) ? (minval + maxval) : (512 - maxval - minval));
			
			if (r == maxval)
				h = (g == minval ? 1280 + (((maxval-b) * 256) / delta) :  256 - (((maxval - g) * 256) / delta));
			else if (g == maxval)
				h = (b == minval ?  256 + (((maxval-r) * 256) / delta) :  768 - (((maxval - b) * 256) / delta));
			else
				h = (r == minval ?  768 + (((maxval-g) * 256) / delta) : 1280 - (((maxval - r) * 256) / delta));
			
			h /= 6;
		}

		h += theDelta;
		if (h >= 256)
			h -= 256;

		double v= (l < 128) ? (l * (255+s))/255 :
				(l+s-l*s/255);
		
		int y = (int) (2*l-v);

		int aColorDiv = (6 * h) / 256;
		int x = (int)(y+(v-y)*((h - (aColorDiv * 256 / 6)) * 6)/255);
		if (x > 255)
			x = 255;

		int z = (int) (v-(v-y)*((h - (aColorDiv * 256 / 6)) * 6)/255);
		if (z < 0)
			z = 0;
		
		switch (aColorDiv)
		{
			case 0: r = (int) v; g = x; b = y; break;
			case 1: r = z; g= (int) v; b = y; break;
			case 2: r = y; g= (int) v; b = x; break;
			case 3: r = y; g = z; b = (int) v; break;
			case 4: r = x; g = y; b = (int) v; break;
			case 5: r = (int) v; g = y; b = z; break;
			default: r = (int) v; g = x; b = y; break;
		}

		*aPtr++ = alpha | (r<<16) | (g << 8) | (b);	 

	}

	theImage->BitsChanged();
}*/

ulong SexyAppBase::HSLToRGB(int h, int s, int l)
{
	int r;
	int g;
	int b;

	double v= (l < 128) ? (l * (255+s))/255 :
			(l+s-l*s/255);
	
	int y = (2*l-v);

	int aColorDiv = (6 * h) / 256;
	int x = y+(v-y)*((h - (int)(aColorDiv * 256 / 6)) * 6)/255;
	if (x > 255)
		x = 255;

	int z = v-(v-y)*((h - (int)(aColorDiv * 256 / 6)) * 6)/255;
	if (z < 0)
		z = 0;
	
	switch (aColorDiv)
	{
		case 0: r = (int) v; g = x; b = y; break;
		case 1: r = z; g= (int) v; b = y; break;
		case 2: r = y; g= (int) v; b = x; break;
		case 3: r = y; g = z; b = (int) v; break;
		case 4: r = x; g = y; b = (int) v; break;
		case 5: r = (int) v; g = y; b = z; break;
		default: r = (int) v; g = x; b = y; break;
	}

	return 0xFF000000 | (r << 16) | (g << 8) | (b);
}

ulong SexyAppBase::RGBToHSL(int r, int g, int b)
{					
	int maxval = std::max(r, std::max(g, b));
	int minval = std::min(r, std::min(g, b));
	int hue = 0;
	int saturation = 0;
	int luminosity = (minval+maxval)/2;
	int delta = maxval - minval;

	if (delta != 0)
	{			
		saturation = (delta * 256) / ((luminosity <= 128) ? (minval + maxval) : (512 - maxval - minval));
		
		if (r == maxval)
			hue = (g == minval ? 1280 + (((maxval-b) * 256) / delta) :  256 - (((maxval - g) * 256) / delta));
		else if (g == maxval)
			hue = (b == minval ?  256 + (((maxval-r) * 256) / delta) :  768 - (((maxval - b) * 256) / delta));
		else
			hue = (r == minval ?  768 + (((maxval-g) * 256) / delta) : 1280 - (((maxval - r) * 256) / delta));
		
		hue /= 6;
	}

	return 0xFF000000 | (hue) | (saturation << 8) | (luminosity << 16);	 
}

void SexyAppBase::HSLToRGB(const ulong* theSource, ulong* theDest, int theSize)
{
	for (int i = 0; i < theSize; i++)
	{
		ulong src = theSource[i];
		theDest[i] = (src & 0xFF000000) | (HSLToRGB((src & 0xFF), (src >> 8) & 0xFF, (src >> 16) & 0xFF) & 0x00FFFFFF);
	}
}

void SexyAppBase::RGBToHSL(const ulong* theSource, ulong* theDest, int theSize)
{
	for (int i = 0; i < theSize; i++)
	{
		ulong src = theSource[i];
		theDest[i] = (src & 0xFF000000) | (RGBToHSL(((src >> 16) & 0xFF), (src >> 8) & 0xFF, (src & 0xFF)) & 0x00FFFFFF);
	}
}

/*
void SexyAppBase::PrecacheAdditive(MemoryImage* theImage)
{
	theImage->GetRLAdditiveData(mDDInterface);
}

void SexyAppBase::PrecacheAlpha(MemoryImage* theImage)
{
	theImage->GetRLAlphaData();
}

void SexyAppBase::PrecacheNative(MemoryImage* theImage)
{
	theImage->GetNativeAlphaData(mDDInterface);
}*/


void SexyAppBase::PlaySample(int theSoundNum)
{
	if (!mSoundManager)
		return;
	
	SoundInstance* aSoundInstance = mSoundManager->GetSoundInstance(theSoundNum);
	if (aSoundInstance != NULL)
	{
		aSoundInstance->Play(false, true);
	}
}


void SexyAppBase::PlaySample(int theSoundNum, int thePan)
{
	if (!mSoundManager)
		return;

	SoundInstance* aSoundInstance = mSoundManager->GetSoundInstance(theSoundNum);
	if (aSoundInstance != NULL)
	{
		aSoundInstance->SetPan(thePan);
		aSoundInstance->Play(false, true);
	}
}

bool SexyAppBase::IsMuted()
{
	return mMuteCount > 0;
}

void SexyAppBase::Mute(bool autoMute)
{	
	mMuteCount++;
	if (autoMute)
		mAutoMuteCount++;

	SetMusicVolume(mMusicVolume);
	SetSfxVolume(mSfxVolume);
}

void SexyAppBase::Unmute(bool autoMute)
{	
	if (mMuteCount > 0)
	{
		mMuteCount--;
		if (autoMute)
			mAutoMuteCount--;
	}

	SetMusicVolume(mMusicVolume);
	SetSfxVolume(mSfxVolume);
}


double SexyAppBase::GetMusicVolume()
{
	return mMusicVolume;
}

void SexyAppBase::SetMusicVolume(double theVolume)
{
	mMusicVolume = theVolume;

	if (mMusicInterface != NULL)
		mMusicInterface->SetVolume((mMuteCount > 0) ? 0.0 : mMusicVolume);
}

double SexyAppBase::GetSfxVolume()
{
	return mSfxVolume;
}

void SexyAppBase::SetSfxVolume(double theVolume)
{
	mSfxVolume = theVolume;

	if (mSoundManager != NULL)
		mSoundManager->SetVolume((mMuteCount > 0) ? 0.0 : mSfxVolume);
}

double SexyAppBase::GetMasterVolume()
{
	return mSoundManager->GetMasterVolume();
}

void SexyAppBase::SetMasterVolume(double theMasterVolume)
{
	mSfxVolume = theMasterVolume;
	mSoundManager->SetMasterVolume(mSfxVolume);
}

/*
void SexyAppBase::AddMemoryImage(MemoryImage* theMemoryImage)
{
	AutoCrit anAutoCrit(mDDInterface->mCritSect);
	mMemoryImageSet.insert(theMemoryImage);
}*/

/*
void SexyAppBase::RemoveMemoryImage(MemoryImage* theMemoryImage)
{
	AutoCrit anAutoCrit(mDDInterface->mCritSect);
	MemoryImageSet::iterator anItr = mMemoryImageSet.find(theMemoryImage);
	if (anItr != mMemoryImageSet.end())
		mMemoryImageSet.erase(anItr);

	Remove3DData(theMemoryImage);
}*/

/*
void SexyAppBase::Remove3DData(MemoryImage* theMemoryImage)
{
	if (mDDInterface)
		mDDInterface->Remove3DData(theMemoryImage);
}*/

void SexyAppBase::DemoSyncRefreshRate()
{
	unreachable();
	/* TODO
	mSyncRefreshRate = mDDInterface->mRefreshRate;
	*/
	if (mRecordingDemoBuffer)
	{
		WriteDemoTimingBlock();
		mDemoBuffer.WriteNumBits(0, 1);
		mDemoBuffer.WriteNumBits(DEMO_VIDEO_DATA, 5);
		mDemoBuffer.WriteBoolean(mIsWindowed);
		uchar aByte = (uchar) mSyncRefreshRate;
		mDemoBuffer.WriteByte(aByte);		
	}
}

/*
SharedImageRef SexyAppBase::SetSharedImage(const std::string& theFileName, const std::string& theVariant, DDImage* theImage, bool* isNew)
{
	std::string anUpperFileName = StringToUpper(theFileName);
	std::string anUpperVariant = StringToUpper(theVariant);

	std::pair<SharedImageMap::iterator, bool> aResultPair;
	SharedImageRef aSharedImageRef;
	
	{
		AutoCrit anAutoCrit(mDDInterface->mCritSect);
		aResultPair = mSharedImageMap.insert(SharedImageMap::value_type(SharedImageMap::key_type(anUpperFileName, anUpperVariant), SharedImage()));
		aSharedImageRef = &aResultPair.first->second;
	}

	if (isNew != NULL)
		*isNew = aResultPair.second;

	if (aResultPair.second)
	{
		aSharedImageRef.mSharedImage->mImage = theImage;
	}

	return aSharedImageRef;
}*/


Image* SexyAppBase::GetSharedImage(const std::string& theFileName, const std::string& theVariant, bool theDoImageSanding)
{
	std::string anUpperFileName = StringToUpper(theFileName);
	std::string anUpperVariant = StringToUpper(theVariant);

	std::pair<SharedImageMap::iterator, bool> aResultPair;

	// Get the image and add it to the map if it doesn't exist.
	aResultPair = mSharedImageMap.insert(
		SharedImageMap::value_type(
			SharedImageMap::key_type(anUpperFileName, anUpperVariant),
			GetImage(theFileName, theDoImageSanding)
		));

	// This represents an old path which is not implemented.
	// Pass in a '!' as the first char of the file name to create a new image
	if ((theFileName.length() > 0) && (theFileName[0] == '!')) {
		// aSharedImageRef.mSharedImage->mImage = new DDImage(mDDInterface);
		unreachable();
	}
	/*
	if (aResultPair.second)
	{
		// Pass in a '!' as the first char of the file name to create a new image
		if ((theFileName.length() > 0) && (theFileName[0] == '!')) {
			aSharedImageRef.mSharedImage->mImage = new DDImage(mDDInterface);
		}
		else {
			aSharedImageRef.mSharedImage->mImage = GetImage(theFileName,false);
		}
	}*/

	return aResultPair.first->second.get();
}

void SexyAppBase::CleanSharedImages()
{
	unreachable();
	/* TODO
	AutoCrit anAutoCrit(mDDInterface->mCritSect);

	if (mCleanupSharedImages)
	{
		// Delete shared images with reference counts of 0
		// This doesn't occur in ~SharedImageRef because sometimes we can not only access the image
		//  through the SharedImageRef returned by GetSharedImage, but also by calling GetSharedImage
		//  again with the same params -- so we can have instances where we do the 'final' deref on
		//  an image but immediately re-request it via GetSharedImage
		SharedImageMap::iterator aSharedImageItr = mSharedImageMap.begin();
		while (aSharedImageItr != mSharedImageMap.end())
		{
			SharedImage* aSharedImage = &aSharedImageItr->second;
			if (aSharedImage->mRefCount == 0)
			{
				delete aSharedImage->mImage;
				mSharedImageMap.erase(aSharedImageItr++);
			}
			else
				++aSharedImageItr;
		}

		mCleanupSharedImages = false;
	}*/
}
