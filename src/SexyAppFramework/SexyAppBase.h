#ifndef __SEXYAPPBASE_H__
#define __SEXYAPPBASE_H__

#include "Common.h"
#include "graphics/VkInterface.h"
#include "graphics/WindowInterface.h"
#include "misc/Rect.h"
#include "graphics/Color.h"
#include "misc/RegistryEmulator.h"
#include "widget/ButtonListener.h"
#include "widget/DialogListener.h"
#include "misc/Buffer.h"
#include "misc/CritSect.h"
#include "misc/Ratio.h"
#include <atomic>
#include <chrono>
#include <memory>
#include <thread>

//extern HMODULE gDDrawDLL;
//extern HMODULE gDSoundDLL;
//extern HMODULE gVersionDLL;

//extern bool gD3DInterfacePreDrawError;

namespace ImageLib
{
	class Image;
};

namespace Sexy
{

class WidgetManager;
//class DDInterface;
class Image;
//class DDImage;
class Widget;
class SoundManager;
class MusicInterface;
//class MemoryImage;
class HTTPTransfer;
class Dialog;

class ResourceManager;

class WidgetSafeDeleteInfo
{
public:
	int						mUpdateAppDepth;
	Widget*					mWidget;
};


typedef std::list<WidgetSafeDeleteInfo> WidgetSafeDeleteList;
//typedef std::set<MemoryImage*> MemoryImageSet;
typedef std::map<int, Dialog*> DialogMap;
typedef std::list<Dialog*> DialogList;
//typedef std::list<MSG> WindowsMessageList;
typedef std::vector<std::string> StringVector;
//typedef std::basic_string<TCHAR> tstring; // string of TCHARs

typedef std::map<std::string, SexyString> StringSexyStringMap;
typedef std::map<std::string, std::string> StringStringMap;
typedef std::map<std::string, std::wstring> StringWStringMap;
typedef std::map<std::string, bool> StringBoolMap;
typedef std::map<std::string, int> StringIntMap;
typedef std::map<std::string, double> StringDoubleMap;
typedef std::map<std::string, StringVector> StringStringVectorMap;

enum
{
	DEMO_MOUSE_POSITION,	
	DEMO_ACTIVATE_APP,
	DEMO_SIZE,
	DEMO_KEY_DOWN,
	DEMO_KEY_UP,
	DEMO_KEY_CHAR,
	DEMO_CLOSE,
	DEMO_MOUSE_ENTER,
	DEMO_MOUSE_EXIT,
	DEMO_LOADING_COMPLETE,
	DEMO_REGISTRY_GETSUBKEYS,
	DEMO_REGISTRY_READ,
	DEMO_REGISTRY_WRITE,
	DEMO_REGISTRY_ERASE,	
	DEMO_FILE_EXISTS,
	DEMO_FILE_READ,
	DEMO_FILE_WRITE,
	DEMO_HTTP_RESULT,
	DEMO_SYNC,
	DEMO_ASSERT_STRING_EQUAL,
	DEMO_ASSERT_INT_EQUAL,
	DEMO_MOUSE_WHEEL,
	DEMO_HANDLE_COMPLETE,
	DEMO_VIDEO_DATA,
	DEMO_IDLE = 31
};

enum {
	FPS_ShowFPS,
	FPS_ShowCoords,
	Num_FPS_Types
};

enum
{
	UPDATESTATE_PROCESS_DONE,
	UPDATESTATE_PROCESS_1,
	UPDATESTATE_PROCESS_2,
};

//typedef std::map<HANDLE, int> HandleToIntMap;
typedef std::map<std::pair<std::string, std::string>, std::unique_ptr<Image>> SharedImageMap;

class SexyAppBase : public ButtonListener, public DialogListener
{
public:
	
	ulong					mRandSeed;
		
	std::string				mCompanyName;
	std::string				mFullCompanyName;
	std::string				mProdName;	
	SexyString				mTitle;	
	std::string				mRegKey;
	std::string				mChangeDirTo;
	
	int						mRelaxUpdateBacklogCount; // app doesn't try to catch up for this many frames
	int						mPreferredX;
	int						mPreferredY;
	int						mWidth;
	int						mHeight;
	int						mFullscreenBits;
	double					mMusicVolume;
	double					mSfxVolume;
	double					mDemoMusicVolume;
	double					mDemoSfxVolume;
	bool					mNoSoundNeeded;
	bool					mWantFMod;
	bool					mCmdLineParsed;
	bool					mSkipSignatureChecks;
	bool					mStandardWordWrap;
	bool					mbAllowExtendedChars;

//	HANDLE					mMutex;
	bool					mOnlyAllowOneCopyToRun;
	UINT					mNotifyGameMessage;
	CritSect				mCritSect;	
	bool					mBetaValidate;
	uchar					mAdd8BitMaxTable[512];
	WidgetManager*			mWidgetManager;
	DialogMap				mDialogMap;
	DialogList				mDialogList;
	std::thread::id			mPrimaryThreadId;
//	bool					mSEHOccured;
	bool					mShutdown;
	bool					mExitToTop;
//	bool					mIsWindowed;
	bool					mIsPhysWindowed;
	bool					mFullScreenWindow; // uses ChangeDisplaySettings to run fullscreen with mIsWindowed true
	bool					mForceFullscreen;
	bool					mForceWindowed;	
	bool					mInitialized;	
	bool					mProcessInTimer;
	std::chrono::high_resolution_clock::time_point	mTimeLoaded;
//	HWND					mHWnd;
//	HWND					mInvisHWnd;
	bool					mIsScreenSaver;
	bool					mAllowMonitorPowersave;
//	WindowsMessageList		mDeferredMessages;
	bool					mNoDefer;	
	bool					mFullScreenPageFlip;	
	bool					mTabletPC;
//	DDInterface*			mDDInterface;
	bool					mAlphaDisabled;
	MusicInterface*			mMusicInterface;	
	bool					mReadFromRegistry;
	std::string				mRegisterLink;
	//std::string			mProductVersion;	
//	Image*					mCursorImages[NUM_CURSORS];
//	HCURSOR					mOverrideCursor;
	bool					mIsOpeningURL;
	bool					mShutdownOnURLOpen;
	std::string				mOpeningURL;
	clock_t					mOpeningURLTime;
	std::chrono::high_resolution_clock::time_point	mLastTimerTime;
	DWORD					mLastBigDelayTime;	
	double					mUnmutedMusicVolume;
	double					mUnmutedSfxVolume;	
	int						mMuteCount;
	int						mAutoMuteCount;
	bool					mDemoMute;
	bool					mMuteOnLostFocus;
//	MemoryImageSet			mMemoryImageSet;
	SharedImageMap			mSharedImageMap;
	bool					mCleanupSharedImages;
	
//	int						mNonDrawCount;
	std::chrono::high_resolution_clock::duration	mFrameTime;

	bool					mIsDrawing;
	bool					mLastDrawWasEmpty;
//	bool					mHasPendingDraw;
	int						mPendingUpdatesAcc;
//	std::chrono::high_resolution_clock::duration	mUpdateFTimeAcc;
//	std::chrono::high_resolution_clock::time_point	mLastTimeCheck;
	std::chrono::high_resolution_clock::time_point	mLastTime;
	std::chrono::high_resolution_clock::time_point	mLastUserInputTick;

	int						mSleepCount;
	int						mDrawCount;
	int						mUpdateCount;
	int						mUpdateAppState;
	int						mUpdateAppDepth;
//	double					mUpdateMultiplier;		
	bool					mPaused;
	int						mFastForwardToUpdateNum;
	bool					mFastForwardToMarker;
	bool					mFastForwardStep;
	std::chrono::high_resolution_clock::time_point	mLastDrawTick;
	std::chrono::high_resolution_clock::time_point	mNextDrawTick;
	int						mStepMode;  // 0 = off, 1 = step, 2 = waiting for step

	int						mCursorNum;
	SoundManager*			mSoundManager;
	size_t					mHandCursor;
	size_t					mDraggingCursor;
	WidgetSafeDeleteList	mSafeDeleteList;
	bool					mMouseIn;	
	bool					mRunning;
//	bool					mActive;
	bool 					mActive;
    bool 					mIsWindowed;
	bool					mMinimized;
	bool					mPhysMinimized;
	bool					mIsDisabled;
	bool					mHasFocus;
	std::chrono::high_resolution_clock::duration	mDrawTime;
	std::chrono::high_resolution_clock::time_point	mFPSStartTick;
	int						mFPSFlipCount;
	int						mFPSDirtyCount;
	std::chrono::high_resolution_clock::duration	mFPSTime;
	int						mFPSCount;
	bool					mShowFPS;
	int						mShowFPSMode;
	std::chrono::high_resolution_clock::duration	mScreenBltTime;
	bool					mAutoStartLoadingThread;
	std::atomic_bool		mLoadingThreadStarted;
	std::atomic_bool		mLoadingThreadCompleted;
	bool					mLoaded;
	bool					mYieldMainThread;
	bool					mLoadingFailed;
//	bool					mCursorThreadRunning;
	bool					mSysCursor;	
	bool					mCustomCursorsEnabled;
	bool					mCustomCursorDirty;	
	bool					mLastShutdownWasGraceful;
	bool					mIsWideWindow;
	bool					mWriteToSexyCache;
	bool					mSexyCacheBuffers;

	int						mNumLoadingThreadTasks;
	int						mCompletedLoadingThreadTasks;

	// For recording/playback of program control
	bool					mRecordingDemoBuffer;
	bool					mPlayingDemoBuffer;
	bool					mManualShutdown;
	std::string				mDemoPrefix;
	std::string				mDemoFileName;
	Buffer					mDemoBuffer;
	int						mDemoLength;
	int						mLastDemoMouseX;
	int						mLastDemoMouseY;
	int						mLastDemoUpdateCnt;
	bool					mDemoNeedsCommand;
	bool					mDemoIsShortCmd;
	int						mDemoCmdNum;
	int						mDemoCmdOrder;
	int						mDemoCmdBitPos;
	bool					mDemoLoadingComplete;
//	HandleToIntMap			mHandleToIntMap; // For waiting on handles
	int						mCurHandleNum;

	typedef std::pair<std::string, int> DemoMarker;
	typedef std::list<DemoMarker> DemoMarkerList;
	DemoMarkerList			mDemoMarkerList;

	bool					mDebugKeysEnabled;
	bool					mEnableMaximizeButton;
	bool					mCtrlDown;
	bool					mAltDown;
	bool					mAllowAltEnter;
	
	int						mSyncRefreshRate;
	bool					mWaitForVSync;
	bool					mSoftVSyncWait;
	bool					mUserChanged3DSetting;
	bool					mAutoEnable3D;
	bool					mTest3D;
	DWORD					mMinVidMemory3D;
	DWORD					mRecommendedVidMemory3D;

	bool					mWidescreenAware;
	Rect					mScreenBounds;
	bool					mEnableWindowAspect;
	Ratio					mWindowAspect;

	StringWStringMap		mStringProperties;
	StringBoolMap			mBoolProperties;
	StringIntMap			mIntProperties;
	StringDoubleMap			mDoubleProperties;
	StringStringVectorMap	mStringVectorProperties;
	ResourceManager*		mResourceManager;

	std::unique_ptr<RegistryEmulator> mRegHandle;

	WindowInterface<Vk::VkInterface> *mWindowInterface;

#ifdef ZYLOM
	uint					mZylomGameId;
#endif

	LONG					mOldWndProc;

protected:	
//	void					RehupFocus();
	void					ClearKeysDown();
	bool					ProcessDeferredMessages(bool singleMessage);
//	void					UpdateFTimeAcc();
//	virtual bool			Process(bool allowSleep = true);		
	virtual void			UpdateFrames();
	virtual void			DoUpdateFrames();
	virtual void			MakeWindow();
//	virtual void			EnforceCursor();
	virtual void			ReInitImages();
	virtual void			DeleteNativeImageData();	
	virtual void			DeleteExtraImageData();
	
	// Loading thread methods	
	virtual void			LoadingThreadCompleted();
	static void				LoadingThreadProcStub(void *theArg);	

	// Cursor thread methods
	
	void					WaitForLoadingThread();				
	void					ProcessSafeDeleteList();	
	void					RestoreScreenResolution();
	void					DoExit(int theCode);

	void					TakeScreenshot();
	void					DumpProgramInfo();	
	void					ShowMemoryUsage();			

	// Registry helpers
	bool					RegistryRead(const std::string& theValueName, uint32_t &theType, std::vector<uint8_t> &theValue, uint32_t &theLength);
//	bool					RegistryReadKey(const std::string& theValueName, uint32_t &theType, std::vector<uint8_t> &theValue, uint32_t &theLength);
	bool					RegistryWrite(const std::string& theValueName, uint32_t theType, const uchar* theValue, uint32_t theLength);

	// Demo recording helpers	
	void					ProcessDemo();

public:
	SexyAppBase();
	virtual ~SexyAppBase();

	// Common overrides:
	virtual MusicInterface*	CreateMusicInterface();
	virtual void			InitHook();
	virtual void			ShutdownHook();	
	virtual void			PreTerminate();
	virtual void			LoadingThreadProc();
	virtual void			WriteToRegistry();
	virtual void			ReadFromRegistry();
	virtual Dialog*			NewDialog(int theDialogId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode);		
	virtual void			PreDisplayHook();

	// Public methods
	virtual void			BeginPopup();
	virtual void			EndPopup();
//	virtual int				MsgBox(const std::string &theText, const std::string &theTitle = "Message", int theFlags = MB_OK);
//	virtual int				MsgBox(const std::wstring &theText, const std::wstring &theTitle = L"Message", int theFlags = MB_OK);
	virtual void			Popup(const std::string& theString);
	virtual void			Popup(const std::wstring& theString);
	virtual void			LogScreenSaverError(const std::string &theError);
	virtual void			SafeDeleteWidget(Widget* theWidget);	

	virtual void			URLOpenFailed(const std::string& theURL);
	virtual void			URLOpenSucceeded(const std::string& theURL);
	virtual bool			OpenURL(const std::string& theURL, bool shutdownOnOpen = false);	
	virtual std::string		GetProductVersion(const std::string& thePath);	

	//virtual void			SEHOccured();
	//virtual std::string		GetGameSEHInfo();
	virtual void			GetSEHWebParams(DefinesMap* theDefinesMap);
	virtual void			Shutdown();	

	virtual void			DoParseCmdLine(int argc, char* argv[]);
	virtual void			ParseCmdLine(const std::string& theCmdLine);
	virtual void			HandleCmdLineParam(const std::string& theParamName, const std::string& theParamValue);
	virtual void			HandleNotifyGameMessage(int theType); // for HWND_BROADCAST of mNotifyGameMessage (0-1000 are reserved for SexyAppBase for theType)
	virtual void			HandleGameAlreadyRunning(); 

	virtual void			Start();	
	virtual void			Init();	
//	virtual void			PreDDInterfaceInitHook();
//	virtual void			PostDDInterfaceInitHook();
	virtual bool			ChangeDirHook(const char *theIntendedPath);
	virtual void			PlaySample(int theSoundNum);
	virtual void			PlaySample(int theSoundNum, int thePan);

	virtual double			GetMasterVolume();
	virtual double			GetMusicVolume();
	virtual double			GetSfxVolume();
	virtual bool			IsMuted();

	virtual void			SetMasterVolume(double theVolume);
	virtual void			SetMusicVolume(double theVolume);
	virtual void			SetSfxVolume(double theVolume);	
	virtual void			Mute(bool autoMute = false);
	virtual void			Unmute(bool autoMute = false);

	void					StartLoadingThread();
	virtual double			GetLoadingThreadProgress();	

	void					CopyToClipboard(const std::string& theString);
	std::string				GetClipboard();

	void					SetCursor(int theCursorNum);
	int						GetCursor();
	void					EnableCustomCursors(bool enabled);	
	virtual std::unique_ptr<Image> GetImage(const std::string& theFileName, bool theDoImageSanding = false);	
//	virtual SharedImageRef	SetSharedImage(const std::string& theFileName, const std::string& theVariant, DDImage* theImage, bool* isNew);
	virtual Image*			GetSharedImage(const std::string& theFileName, const std::string& theVariant = "", bool theDoImageSanding = false);

	void					CleanSharedImages();
//	void					PrecacheAdditive(MemoryImage* theImage);
//	void					PrecacheAlpha(MemoryImage* theImage);
//	void					PrecacheNative(MemoryImage* theImage);
	void					SetCursorImage(int theCursorNum, Image* theImage);

//	DDImage*				CreateCrossfadeImage(Image* theImage1, const Rect& theRect1, Image* theImage2, const Rect& theRect2, double theFadeFactor);
	void					ColorizeImage(Image* theImage, const Color& theColor);
//	DDImage*				CreateColorizedImage(Image* theImage, const Color& theColor);
//	DDImage*				CopyImage(Image* theImage, const Rect& theRect);
//	DDImage*				CopyImage(Image* theImage);
	void					MirrorImage(Image* theImage);
	void					FlipImage(Image* theImage);
//	void					RotateImageHue(Sexy::MemoryImage *theImage, int theDelta);
	ulong					HSLToRGB(int h, int s, int l);
	ulong					RGBToHSL(int r, int g, int b);
	void					HSLToRGB(const ulong* theSource, ulong* theDest, int theSize);
	void					RGBToHSL(const ulong* theSource, ulong* theDest, int theSize);

//	void					AddMemoryImage(MemoryImage* theMemoryImage);
//	void					RemoveMemoryImage(MemoryImage* theMemoryImage);
//	void					Remove3DData(MemoryImage* theMemoryImage);
	virtual void			SwitchScreenMode();
	virtual void			SwitchScreenMode(bool wantWindowed);
	virtual void			SwitchScreenMode(bool wantWindowed, bool is3d, bool force = false);
	virtual void			SetAlphaDisabled(bool isDisabled);
	
	virtual Dialog*			DoDialog(int theDialogId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode);
	virtual Dialog*			GetDialog(int theDialogId);
	virtual void			AddDialog(int theDialogId, Dialog* theDialog);
	virtual void			AddDialog(Dialog* theDialog);
	virtual bool			KillDialog(int theDialogId, bool removeWidget, bool deleteWidget);
	virtual bool			KillDialog(int theDialogId);
	virtual bool			KillDialog(Dialog* theDialog);
	virtual int				GetDialogCount();
	virtual void			ModalOpen();
	virtual void			ModalClose();	
	virtual void			DialogButtonPress(int theDialogId, int theButtonId);
	virtual void			DialogButtonDepress(int theDialogId, int theButtonId);

	virtual void			GotFocus();
	virtual void			LostFocus();	
//	virtual bool			IsAltKeyUsed(WPARAM wParam);
	virtual bool			DebugKeyDown(int theKey);	
//	virtual bool			DebugKeyDownAsync(int theKey, bool ctrlDown, bool altDown);
	virtual void			CloseRequestAsync();
//	bool					Is3DAccelerated();
//	bool					Is3DAccelerationSupported();
//	bool					Is3DAccelerationRecommended();
	void					DemoSyncRefreshRate();
//	void					Set3DAcclerated(bool is3D, bool reinit = true);
	virtual void			Done3dTesting();
	virtual std::string		NotifyCrashHook(); // return file name that you want to upload
	
//	virtual bool			CheckSignature(const Buffer& theBuffer, const std::string& theFileName);
	virtual bool			DrawDirtyStuff();
	virtual void			Redraw();

	// Properties access methods
	bool					LoadProperties(const std::string& theFileName, bool required, bool checkSig);
	bool					LoadProperties();
	virtual void			InitPropertiesHook();

	// Resource access methods
	void					LoadResourceManifest();
	void					ShowResourceError(bool doExit = false);
	
	bool					GetBoolean(const std::string& theId);
	bool					GetBoolean(const std::string& theId, bool theDefault);	
	int						GetInteger(const std::string& theId);
	int						GetInteger(const std::string& theId, int theDefault);
	double					GetDouble(const std::string& theId);
	double					GetDouble(const std::string& theId, double theDefault);
	SexyString				GetString(const std::string& theId);
	SexyString				GetString(const std::string& theId, const SexyString& theDefault);

	StringVector			GetStringVector(const std::string& theId);

	void					SetBoolean(const std::string& theId, bool theValue);
	void					SetInteger(const std::string& theId, int theValue);
	void					SetDouble(const std::string& theId, double theValue);
	void					SetString(const std::string& theId, const std::wstring& theValue);
	
	// Demo access methods
	bool					PrepareDemoCommand(bool required);
	void					WriteDemoTimingBlock();
	void					WriteDemoBuffer();
	bool					ReadDemoBuffer(std::string &theError);//UNICODE
	void					DemoSyncBuffer(Buffer* theBuffer);
	void					DemoSyncString(std::string* theString);
	void					DemoSyncInt(int* theInt);
	void					DemoSyncBool(bool* theBool);
	void					DemoAssertStringEqual(const std::string& theString);
	void					DemoAssertIntEqual(int theInt);
	void					DemoAddMarker(const std::string& theString);
//	void					DemoRegisterHandle(HANDLE theHandle);
//	void					DemoWaitForHandle(HANDLE theHandle);
//	bool					DemoCheckHandle(HANDLE theHandle);
	

	// Registry access methods
	bool					RegistryGetSubKeys(const std::string& theKeyName, StringVector* theSubKeys);
	bool					RegistryReadString(const std::string& theValueName, std::string* theString);
	bool					RegistryReadInteger(const std::string& theValueName, int* theValue);
	bool					RegistryReadBoolean(const std::string& theValueName, bool* theValue);
	bool					RegistryReadData(const std::string &theKey, std::vector<uint8_t> &theValue, uint32_t &theLength);
	bool					RegistryWriteString(const std::string& theValueName, const std::string& theString);
	bool					RegistryWriteInteger(const std::string& theValueName, int theValue);
	bool					RegistryWriteBoolean(const std::string& theValueName, bool theValue);
	bool					RegistryWriteData(const std::string& theValueName, const uchar* theValue, ulong theLength);	
	bool					RegistryEraseKey(const SexyString& theKeyName);
	void					RegistryEraseValue(const SexyString& theValueName);

	// File access methods
	bool					WriteBufferToFile(const std::string& theFileName, const Buffer* theBuffer);
	bool					ReadBufferFromFile(const std::string& theFileName, Buffer* theBuffer, bool dontWriteToDemo = false);//UNICODE
	bool					WriteBytesToFile(const std::string& theFileName, const void *theData, unsigned long theDataLen);
	bool					FileExists(const std::string& theFileName);
	bool					EraseFile(const std::string& theFileName);

	// Misc methods
	virtual void			DoMainLoop();
	virtual bool			UpdateAppStep(bool* updated);
	virtual bool			UpdateApp();
//	int						InitDDInterface();
//	void					ClearUpdateBacklog(bool relaxForASecond = false);
	bool					IsScreenSaver();
	virtual bool			AppCanRestore();
//	static LRESULT CALLBACK	WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		
};

extern SexyAppBase* gSexyAppBase;

};

#endif //__SEXYAPPBASE_H__
