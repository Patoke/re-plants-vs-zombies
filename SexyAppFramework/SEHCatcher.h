#ifndef __SEHCATHER_H__
#define __SEHCATHER_H__

#include "Common.h"
#include "HTTPTransfer.h"
#include <imagehlp.h>

namespace Sexy
{

class SexyAppBase;

typedef BOOL (__stdcall * SYMINITIALIZEPROC)(HANDLE, LPSTR, BOOL);

typedef DWORD (__stdcall *SYMSETOPTIONSPROC)(DWORD);

typedef BOOL (__stdcall *SYMCLEANUPPROC)(HANDLE);

typedef LPCSTR (__stdcall *UNDECORATESYMBOLNAMEPROC)(LPCSTR, LPSTR, DWORD, DWORD);

typedef BOOL (__stdcall * STACKWALKPROC)
           ( DWORD, HANDLE, HANDLE, LPSTACKFRAME, LPVOID,
            PREAD_PROCESS_MEMORY_ROUTINE,PFUNCTION_TABLE_ACCESS_ROUTINE,
            PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE);

typedef LPVOID (__stdcall *SYMFUNCTIONTABLEACCESSPROC)(HANDLE, DWORD);

typedef DWORD (__stdcall *SYMGETMODULEBASEPROC)(HANDLE, DWORD);

typedef BOOL (__stdcall *SYMGETSYMFROMADDRPROC)(HANDLE, DWORD, PDWORD, PIMAGEHLP_SYMBOL);

class SEHCatcher 
{
public:	
	static SexyAppBase*		mApp;
	static HFONT			mDialogFont;
	static HFONT			mBoldFont;
	static HWND				mYesButtonWindow;
	static HWND				mNoButtonWindow;
	static HWND				mDebugButtonWindow;
	static HWND				mEditWindow;
	static bool				mHasDemoFile;
	static bool				mDebugError;
	static std::string		mErrorTitle;
	static std::string		mErrorText;
	static std::string		mUserText;
	static std::string		mUploadFileName;
	static std::wstring		mCrashMessage;
	static std::string		mSubmitHost;
	static std::wstring		mSubmitErrorMessage;
	static std::wstring		mSubmitMessage;
	static HMODULE			mImageHelpLib;
	static SYMINITIALIZEPROC mSymInitialize;
	static SYMSETOPTIONSPROC mSymSetOptions;
	static UNDECORATESYMBOLNAMEPROC mUnDecorateSymbolName;
	static SYMCLEANUPPROC	mSymCleanup;
	static STACKWALKPROC	mStackWalk;
	static SYMFUNCTIONTABLEACCESSPROC mSymFunctionTableAccess;
	static SYMGETMODULEBASEPROC mSymGetModuleBase;
	static SYMGETSYMFROMADDRPROC mSymGetSymFromAddr;
	static HTTPTransfer		mSubmitReportTransfer;
	static bool				mExiting;
	static bool				mShowUI;
	static bool				mAllowSubmit;

protected:
	static LPTOP_LEVEL_EXCEPTION_FILTER mPreviousFilter;

public:
	static void				SubmitReportThread(void *theArg);

	static LRESULT CALLBACK SEHWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK SubmitInfoWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static long __stdcall	UnhandledExceptionFilter(LPEXCEPTION_POINTERS lpExceptPtr);	
	static void				DoHandleDebugEvent(LPEXCEPTION_POINTERS lpEP);
	static bool				GetLogicalAddress(void* addr, char* szModule, DWORD len, DWORD& section, DWORD& offset);
	static std::string		GetFilename(const std::string& thePath);
	static void				WriteToFile(const std::string& theErrorText);
	static void				ShowSubmitInfoDialog();
	static void				ShowErrorDialog(const std::string& theErrorTitle, const std::string& theErrorText);	
	static bool				LoadImageHelp();
	static void				UnloadImageHelp();
	static std::string		IntelWalk(PCONTEXT theContext, int theSkipCount);
	static std::string		ImageHelpWalk(PCONTEXT theContext, int theSkipCount);
	static std::string		GetSysInfo();
	static void				GetSymbolsFromMapFile(std::string &theDebugDump);

public:
	SEHCatcher();
	~SEHCatcher();
};

extern SEHCatcher gSEHCatcher;

}

#endif 