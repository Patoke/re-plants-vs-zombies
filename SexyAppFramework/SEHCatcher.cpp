#include "SEHCatcher.h"
#include "SexyAppBase.h"
#include <fstream>
#include <process.h>

#ifdef ZYLOM
#include "zylomso.h"
using namespace zylom::zylomso;
#endif

using namespace Sexy;

LPTOP_LEVEL_EXCEPTION_FILTER SEHCatcher::mPreviousFilter;
SexyAppBase*                SEHCatcher::mApp = NULL;
HFONT                       SEHCatcher::mDialogFont = NULL;
HFONT                       SEHCatcher::mBoldFont = NULL;
bool                        SEHCatcher::mHasDemoFile = false;
bool                        SEHCatcher::mDebugError = false;
std::string                 SEHCatcher::mErrorTitle;
std::string                 SEHCatcher::mErrorText;
std::string                 SEHCatcher::mUserText;
std::string                 SEHCatcher::mUploadFileName;
HWND                        SEHCatcher::mYesButtonWindow = NULL;
HWND                        SEHCatcher::mNoButtonWindow = NULL;
HWND                        SEHCatcher::mDebugButtonWindow = NULL;
HWND                        SEHCatcher::mEditWindow = NULL;
HMODULE                     SEHCatcher::mImageHelpLib = NULL;
SYMINITIALIZEPROC           SEHCatcher::mSymInitialize = NULL;
SYMSETOPTIONSPROC           SEHCatcher::mSymSetOptions = NULL;
UNDECORATESYMBOLNAMEPROC    SEHCatcher::mUnDecorateSymbolName = NULL;
SYMCLEANUPPROC              SEHCatcher::mSymCleanup = NULL;
STACKWALKPROC               SEHCatcher::mStackWalk = NULL;
SYMFUNCTIONTABLEACCESSPROC  SEHCatcher::mSymFunctionTableAccess = NULL;
SYMGETMODULEBASEPROC        SEHCatcher::mSymGetModuleBase = NULL;
SYMGETSYMFROMADDRPROC       SEHCatcher::mSymGetSymFromAddr = NULL;
HTTPTransfer                SEHCatcher::mSubmitReportTransfer;
bool                        SEHCatcher::mExiting = false;
bool                        SEHCatcher::mShowUI = true;
bool                        SEHCatcher::mAllowSubmit = true;

std::wstring                    SEHCatcher::mCrashMessage = L"An unexpected error has occured!";
std::string                     SEHCatcher::mSubmitHost;
std::wstring                    SEHCatcher::mSubmitMessage = L"Please help out by providing as much information as you can about this crash.";
std::wstring                    SEHCatcher::mSubmitErrorMessage = L"Failed to connect to server.";

static bool gUseDefaultFonts = true;

struct 
{
	DWORD   dwExceptionCode;
	char    *szMessage;
} gMsgTable[] = {
   { STATUS_SEGMENT_NOTIFICATION,     (char *)"Segment Notification" },
   { STATUS_BREAKPOINT,               (char *)"Breakpoint" },
   { STATUS_SINGLE_STEP,              (char *)"Single step" },
   { STATUS_WAIT_0,                   (char *)"Wait 0" },
   { STATUS_ABANDONED_WAIT_0,         (char *)"Abandoned Wait 0" },
   { STATUS_USER_APC,                 (char *)"User APC" },
   { STATUS_TIMEOUT,                  (char *)"Timeout" },
   { STATUS_PENDING,                  (char *)"Pending" },
   { STATUS_GUARD_PAGE_VIOLATION,     (char *)"Guard Page Violation" },
   { STATUS_DATATYPE_MISALIGNMENT,    (char *)"Data Type Misalignment" },
   { STATUS_ACCESS_VIOLATION,         (char *)"Access Violation" },
   { STATUS_IN_PAGE_ERROR,            (char *)"In Page Error" },
   { STATUS_NO_MEMORY,                (char *)"No Memory" },
   { STATUS_ILLEGAL_INSTRUCTION,      (char *)"Illegal Instruction" },
   { STATUS_NONCONTINUABLE_EXCEPTION, (char *)"Noncontinuable Exception" },
   { STATUS_INVALID_DISPOSITION,      (char *)"Invalid Disposition" },
   { STATUS_ARRAY_BOUNDS_EXCEEDED,    (char *)"Array Bounds Exceeded" },
   { STATUS_FLOAT_DENORMAL_OPERAND,   (char *)"Float Denormal Operand" },
   { STATUS_FLOAT_DIVIDE_BY_ZERO,     (char *)"Divide by Zero" },
   { STATUS_FLOAT_INEXACT_RESULT,     (char *)"Float Inexact Result" },
   { STATUS_FLOAT_INVALID_OPERATION,  (char *)"Float Invalid Operation" },
   { STATUS_FLOAT_OVERFLOW,           (char *)"Float Overflow" },
   { STATUS_FLOAT_STACK_CHECK,        (char *)"Float Stack Check" },
   { STATUS_FLOAT_UNDERFLOW,          (char *)"Float Underflow" },
   { STATUS_INTEGER_DIVIDE_BY_ZERO,   (char *)"Integer Divide by Zero" },
   { STATUS_INTEGER_OVERFLOW,         (char *)"Integer Overflow" },
   { STATUS_PRIVILEGED_INSTRUCTION,   (char *)"Privileged Instruction" },
   { STATUS_STACK_OVERFLOW,           (char *)"Stack Overflow" },
   { STATUS_CONTROL_C_EXIT,           (char *)"Ctrl+C Exit" },
   { 0xFFFFFFFF,                      (char *)"" }
};


SEHCatcher::SEHCatcher() 
{   
	mPreviousFilter = SetUnhandledExceptionFilter(UnhandledExceptionFilter);    
}

SEHCatcher::~SEHCatcher() 
{ 
	SetUnhandledExceptionFilter(mPreviousFilter); 
}

long __stdcall SEHCatcher::UnhandledExceptionFilter(LPEXCEPTION_POINTERS lpExceptPtr)
{
	if (mApp != NULL)
		mApp->SEHOccured(); 

	DoHandleDebugEvent(lpExceptPtr);
	
	if (!mDebugError)
		SetErrorMode(SEM_NOGPFAULTERRORBOX);
	
	return EXCEPTION_CONTINUE_SEARCH;   
}

bool SEHCatcher::LoadImageHelp()
{
	mImageHelpLib = LoadLibraryA("IMAGEHLP.DLL");
	if (!mImageHelpLib)
		return false;

	mSymInitialize = (SYMINITIALIZEPROC) GetProcAddress(mImageHelpLib, "SymInitialize");
	if (!mSymInitialize)
		return false;

	mSymSetOptions = (SYMSETOPTIONSPROC) GetProcAddress(mImageHelpLib, "SymSetOptions");
	if (!mSymSetOptions)
		return false;

	mSymCleanup = (SYMCLEANUPPROC) GetProcAddress(mImageHelpLib, "SymCleanup");
	if (!mSymCleanup)
		return false;

	mUnDecorateSymbolName = (UNDECORATESYMBOLNAMEPROC) GetProcAddress(mImageHelpLib, "UnDecorateSymbolName");
	if (!mUnDecorateSymbolName)
		return false;

	mStackWalk = (STACKWALKPROC) GetProcAddress(mImageHelpLib, "StackWalk");
	if (!mStackWalk)
		return false;

	mSymFunctionTableAccess = (SYMFUNCTIONTABLEACCESSPROC) GetProcAddress(mImageHelpLib, "SymFunctionTableAccess");
	if (!mSymFunctionTableAccess)
		return false;

	mSymGetModuleBase = (SYMGETMODULEBASEPROC) GetProcAddress(mImageHelpLib, "SymGetModuleBase");
	if (!mSymGetModuleBase)
		return false;

	mSymGetSymFromAddr = (SYMGETSYMFROMADDRPROC) GetProcAddress(mImageHelpLib, "SymGetSymFromAddr" );
	if (!mSymGetSymFromAddr)
		return false;    

	mSymSetOptions(SYMOPT_DEFERRED_LOADS);

	// Get image filename of the main executable
	char filepath[MAX_PATH], *lastdir, *pPath;
	DWORD filepathlen = GetModuleFileNameA ( NULL, filepath, sizeof(filepath));
	(void)filepathlen;
		
	lastdir = strrchr (filepath, '/');
	if (lastdir == NULL) lastdir = strrchr (filepath, '\\');
	if (lastdir != NULL) lastdir[0] = '\0';

	// Initialize the symbol table routines, supplying a pointer to the path
	pPath = filepath;
	if (strlen (filepath) == 0) pPath = NULL;

	 if (!mSymInitialize (GetCurrentProcess(), pPath, TRUE))
		return false;

	return true;
}

void SEHCatcher::UnloadImageHelp()
{
	if (mImageHelpLib != NULL)
		FreeLibrary(mImageHelpLib);
}

static bool StrToLongHex(const std::string& aString, DWORD* theValue)
{
	*theValue = 0;

	for (int i = 0; i < (int)aString.length(); i++)
	{
		char aChar = aString[i];

		int aValue;
		if ((aChar >= '0') && (aChar <= '9'))
			aValue = aChar - '0';
		else if ((aChar >= 'A') && (aChar <= 'F'))
			aValue = (aChar - 'A') + 10;
		else if ((aChar >= 'a') && (aChar <= 'f'))
			aValue = (aChar - 'a') + 10;
		else
			return false;

		*theValue += aValue << ((aString.length()-i-1)*4);
	}

	return true;
}

void SEHCatcher::GetSymbolsFromMapFile(std::string &theDebugDump)
{
	// DWORD aTick = GetTickCount(); // unused
	WIN32_FIND_DATAA aFindData;
	HANDLE aFindHandle = FindFirstFileA("*.map",&aFindData);
	if (aFindHandle==INVALID_HANDLE_VALUE)
		return;

	FindClose(aFindHandle);

	typedef std::pair<DWORD,DWORD> SymbolPair;
	typedef std::map<SymbolPair,std::string> SymbolMap;
	typedef std::pair<std::string,int> LineNumPair;
	typedef std::map<SymbolPair,LineNumPair> LineNumMap;

/**/
	SymbolMap aSymbolMap;
	LineNumMap aLineNumMap;
	std::fstream aFile(aFindData.cFileName, std::ios::in);

	if (!aFile.is_open())
		return;

	// Parse map file
	std::string aCurLinesFile;
	while (!aFile.eof())
	{
		char aStr[4096];
		aFile.getline(aStr, 4096);

		std::string aString = aStr;

		if ((aString.length() > 22) && (aString[5] == ':'))
		{
			std::string aFoundPreStr = aString.substr(1, 4);
			std::string aFoundPostStr = aString.substr(6, 8);

			DWORD aFoundPreVal;
			DWORD aFoundPostVal;

			if (StrToLongHex(aFoundPreStr, &aFoundPreVal) && StrToLongHex(aFoundPostStr, &aFoundPostVal))
			{
				int aSpacePos = aString.find(' ', 21);

				if ((aString[20] == ' ') && (aSpacePos >= 0))
					aSymbolMap[SymbolPair(aFoundPreVal,aFoundPostVal)] =  aString.substr(21, aSpacePos-21);
			}
		}
		else if (strcmp(aString.substr(0, strlen("Line numbers")).c_str(), "Line numbers") == 0)
		{
			int aSegmentPos = aString.rfind(')');
			if (aSegmentPos == -1)
				aSegmentPos = aString.length();

			// int aPreLen = strlen("Line numbers for "); // unused

			int aStartPos = aString.find('(');

			aCurLinesFile = aString.substr(aStartPos + 1, aSegmentPos - aStartPos - 1);
		}
		else if ((aCurLinesFile.length() > 0) && (aString.length() == 80))
		{   
			// Line number stuff

			for (int i = 0; i < 4; i++)
			{
				int aStartLoc = 20 * i;

				int aLine = atoi(aString.substr(aStartLoc, 6).c_str());

				std::string aFoundPreStr = aString.substr(aStartLoc + 7, 4);
				std::string aFoundPostStr = aString.substr(aStartLoc + 12, 8);

				DWORD aFoundPreVal;
				DWORD aFoundPostVal;

				if (StrToLongHex(aFoundPreStr, &aFoundPreVal) && StrToLongHex(aFoundPostStr, &aFoundPostVal))
				{
					aLineNumMap[SymbolPair(aFoundPreVal,aFoundPostVal)] = LineNumPair(aCurLinesFile,aLine);
				}
			}
		}
	}

	// Parse stack trace
	for (int i = 0; i < (int)theDebugDump.length(); i++)
	{
		if (theDebugDump.at(i) == ':')
		{
			std::string aFindPreStr = theDebugDump.substr(i-4, 4);
			std::string aFindPostStr = theDebugDump.substr(i+1, 8);

			DWORD aFindPreVal;
			DWORD aFindPostVal;

			if (StrToLongHex(aFindPreStr, &aFindPreVal) && StrToLongHex(aFindPostStr, &aFindPostVal))
			{

				int aBestDist = -1;
				SymbolMap::iterator aSymbolItr  = aSymbolMap.lower_bound(SymbolPair(aFindPreVal,aFindPostVal));
				if (aSymbolItr!=aSymbolMap.end() && aSymbolItr!=aSymbolMap.begin() && aSymbolItr->first.second!=aFindPostVal)
					--aSymbolItr;

				if (aSymbolItr!=aSymbolMap.end() && aSymbolItr->first.first==aFindPreVal)
					aBestDist = aFindPostVal - aSymbolItr->first.second;

				if (aBestDist != -1)
				{
					std::string &aBestName = aSymbolItr->second;

					char aSymbolName[4096];

					if (mUnDecorateSymbolName(aBestName.c_str(), aSymbolName, 4096, 
						UNDNAME_NO_ALLOCATION_MODEL | UNDNAME_NO_ACCESS_SPECIFIERS | 
						UNDNAME_NO_THROW_SIGNATURES | UNDNAME_NO_MEMBER_TYPE) == 0)
						strcpy(aSymbolName, aBestName.c_str());

					if (aBestDist != 0)
					{
						sprintf(aSymbolName+strlen(aSymbolName), "+0x%X", aBestDist);
					}

					std::string aNewText = aSymbolName;

					LineNumMap::iterator aLineNumItr = aLineNumMap.lower_bound(SymbolPair(aFindPreVal,aFindPostVal));
					if (aLineNumItr!=aLineNumMap.end() && aLineNumItr!=aLineNumMap.begin() && aLineNumItr->first.second!=aFindPostVal)
						--aLineNumItr;

					if (aLineNumItr!=aLineNumMap.end() && aLineNumItr->first.first==aFindPreVal)
					{                           
						std::string &aBestFile = aLineNumItr->second.first;
						int aBestLine = aLineNumItr->second.second;
						int aBestLineDist =  aFindPostVal - aLineNumItr->first.second;

						char aDistStr[4096];
						sprintf(aDistStr, "\r\n    %s line %d +0x%X", aBestFile.c_str(), aBestLine, aBestLineDist);
						aNewText += aDistStr;
					}

					theDebugDump.erase(i-4, 13);
					theDebugDump.insert(i-4, aNewText.c_str());
				}
			}
		}
	}

//  MessageBox(NULL,StrFormat("%d",GetTickCount()-aTick).c_str(),"Time",MB_OK);
}

void SEHCatcher::DoHandleDebugEvent(LPEXCEPTION_POINTERS lpEP)
{
	bool hasImageHelp = LoadImageHelp();

	std::string anErrorTitle;
	std::string aDebugDump;

	char aBuffer[2048];
   
		///////////////////////////
	// first name the exception 
	char  *szName = NULL;
	for (int i=0; gMsgTable[i].dwExceptionCode != 0xFFFFFFFF; i++) 
	{
		if (gMsgTable[i].dwExceptionCode == lpEP->ExceptionRecord->ExceptionCode) 
		{
			szName = gMsgTable[i].szMessage;
			break;
		}
	}   

	if (szName != NULL)
	{       
		sprintf(aBuffer,"Exception: %s (code 0x%lx) at address %p in thread %lX\r\n",
				szName, lpEP->ExceptionRecord->ExceptionCode, 
				lpEP->ExceptionRecord->ExceptionAddress, GetCurrentThreadId());
	}
	else
	{
		sprintf(aBuffer,"Unknown exception: (code 0x%lx) at address %p in thread %lX\r\n",
				lpEP->ExceptionRecord->ExceptionCode, 
				lpEP->ExceptionRecord->ExceptionAddress, GetCurrentThreadId());
	}

	aDebugDump += aBuffer;  

	///////////////////////////////////////////////////////////
	// Get logical address of the module where exception occurs
	DWORD section, offset;
	GetLogicalAddress(lpEP->ExceptionRecord->ExceptionAddress, aBuffer, sizeof(aBuffer), section, offset);
	aDebugDump += "Module: " + GetFilename(aBuffer) + "\r\n";       
	sprintf(aBuffer, "Logical Address: %04lX:%08lX\r\n", section, offset);
	aDebugDump += aBuffer;  
	
	aDebugDump += "\r\n";   

	anErrorTitle = StrFormat("Exception at %04X:%08X", section, offset);

	std::string aWalkString;    

	if (hasImageHelp)
		aWalkString = ImageHelpWalk(lpEP->ContextRecord, 0);

	if (aWalkString.length() == 0)
		aWalkString = IntelWalk(lpEP->ContextRecord, 0);

	aDebugDump += aWalkString;

	aDebugDump += "\r\n";

	sprintf(aBuffer, ("RAX:%016llX RBX:%016llX RCX:%016llX RDX:%016llX RSI:%016llX RDI:%016llX\r\n"),
			lpEP->ContextRecord->Rax, lpEP->ContextRecord->Rbx, lpEP->ContextRecord->Rcx, lpEP->ContextRecord->Rdx, lpEP->ContextRecord->Rsi, lpEP->ContextRecord->Rdi);
	aDebugDump += aBuffer;
	sprintf(aBuffer, ("R8:%016llX R9:%016llX R10:%016llX R11:%016llX R12:%016llX R13:%016llX R14:%016llX R15:%016llX\r\n"),
			lpEP->ContextRecord->R8, lpEP->ContextRecord->R9, lpEP->ContextRecord->R10, lpEP->ContextRecord->R11, lpEP->ContextRecord->R12, lpEP->ContextRecord->R13, lpEP->ContextRecord->R14, lpEP->ContextRecord->R15);
	aDebugDump += aBuffer;
	sprintf(aBuffer, "RIP:%016llX RSP:%016llX  RBP:%016llX\r\n", lpEP->ContextRecord->Rip, lpEP->ContextRecord->Rsp, lpEP->ContextRecord->Rbp); 
	aDebugDump += aBuffer;
	sprintf(aBuffer, "CS:%04X SS:%04X DS:%04X ES:%04X FS:%04X GS:%04X\r\n", lpEP->ContextRecord->SegCs, lpEP->ContextRecord->SegSs, lpEP->ContextRecord->SegDs, lpEP->ContextRecord->SegEs, lpEP->ContextRecord->SegFs, lpEP->ContextRecord->SegGs );
	aDebugDump += aBuffer;
	sprintf(aBuffer, "Flags:%08lX\r\n", lpEP->ContextRecord->EFlags );
	aDebugDump += aBuffer;

	aDebugDump += "\r\n";
	aDebugDump += GetSysInfo(); 

	if (mApp != NULL)
	{
		std::string aGameSEHInfo = mApp->GetGameSEHInfo();
		if (aGameSEHInfo.length() > 0)
		{
			aDebugDump += "\r\n";
			aDebugDump += aGameSEHInfo;
		}

		mApp->CopyToClipboard(aDebugDump);
	}       

	if (hasImageHelp)
		GetSymbolsFromMapFile(aDebugDump);

	WriteToFile(aDebugDump);

#ifdef ZYLOM
	ZylomGS_StandAlone_SendBugReport((char*) aDebugDump.c_str());
#else
	if (mApp != NULL)
	{
		if (mApp->mRecordingDemoBuffer)
		{
			// Make sure we have enough update block things in there to
			//  get to the final crashing update
			mApp->WriteDemoTimingBlock();
			mApp->mDemoBuffer.WriteNumBits(0, 1);
			mApp->mDemoBuffer.WriteNumBits(DEMO_IDLE, 5);
			mApp->WriteDemoBuffer();        
			mUploadFileName = mApp->mDemoFileName;
		}   

		mHasDemoFile = mApp->mRecordingDemoBuffer;
		std::string anUploadFile = mApp->NotifyCrashHook();
		if (!anUploadFile.empty())
		{
			mUploadFileName = anUploadFile;
			mHasDemoFile = true;
		}

		mApp->mRecordingDemoBuffer = false;
		mApp->mPlayingDemoBuffer = false;
	}

	if (mShowUI)
		ShowErrorDialog(anErrorTitle, aDebugDump);
#endif

	//::MessageBox(NULL, aDebugDump.c_str(), "ERROR", MB_ICONERROR);    

	UnloadImageHelp();  
}

std::string SEHCatcher::IntelWalk(PCONTEXT theContext, int theSkipCount)
{
	std::string aDebugDump;
	char aBuffer[2048];

	intptr_t pc = theContext->Rip;
	intptr_t *pFrame, *pPrevFrame;
	
	pFrame = (intptr_t*)theContext->Rbp;

	for (;;)
	{
		char szModule[MAX_PATH] = "";
		DWORD section = 0, offset = 0;

		GetLogicalAddress((PVOID)pc, szModule, sizeof(szModule), section, offset);

		sprintf(aBuffer, "%016llX  %p  %04lX:%08lX %s\r\n",
				  pc, pFrame, section, offset, GetFilename(szModule).c_str());
		aDebugDump += aBuffer;

		pc = pFrame[1];

		pPrevFrame = pFrame;

		pFrame = (intptr_t*)(pFrame[0]); // proceed to next higher frame on stack

		if ((intptr_t)pFrame & 3)    // Frame pointer must be aligned on a
			break;                  // DWORD boundary.  Bail if not so.

		if (pFrame <= pPrevFrame)
			break;

		// Can two DWORDs be read from the supposed frame address?          
		if (IsBadWritePtr(pFrame, sizeof(PVOID)*2))
			break;
	};

	return aDebugDump;
}

std::string SEHCatcher::ImageHelpWalk(PCONTEXT theContext, int theSkipCount)
{
	char aBuffer[2048];
	std::string aDebugDump;

	STACKFRAME sf;
	memset( &sf, 0, sizeof(sf) );   

	// Initialize the STACKFRAME structure for the first call.  This is only
	// necessary for Intel CPUs, and isn't mentioned in the documentation.
	sf.AddrPC.Offset       = theContext->Rip;
	sf.AddrPC.Mode         = AddrModeFlat;
	sf.AddrStack.Offset    = theContext->Rsp;
	sf.AddrStack.Mode      = AddrModeFlat;
	sf.AddrFrame.Offset    = theContext->Rbp;
	sf.AddrFrame.Mode      = AddrModeFlat;
	
	int aLevelCount = 0;
	(void)aLevelCount; // Unused

	for (;;)
	{
		if (!mStackWalk(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(), GetCurrentThread(),
						&sf, NULL  /*theContext*/, NULL, mSymFunctionTableAccess, mSymGetModuleBase, 0))
		{
			DWORD lastErr = GetLastError();
			sprintf(aBuffer, "StackWalk failed (error %ld)\r\n", lastErr);
			aDebugDump += aBuffer;
			break;
		}

		if ((sf.AddrFrame.Offset == 0) || (sf.AddrPC.Offset == 0))
			break;

		if (theSkipCount > 0)
		{
			theSkipCount--;
			continue;
		}

		BYTE symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 512];
		PIMAGEHLP_SYMBOL pSymbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
		pSymbol->SizeOfStruct = sizeof(symbolBuffer);
		pSymbol->MaxNameLength = 512;
			
		DWORD symDisplacement = 0;  // Displacement of the input address,
									// relative to the start of the symbol
			
		if (mSymGetSymFromAddr(GetCurrentProcess(), sf.AddrPC.Offset, &symDisplacement, pSymbol))
		{
			char aUDName[256];
			mUnDecorateSymbolName(pSymbol->Name, aUDName, 256, 
								 UNDNAME_NO_ALLOCATION_MODEL | UNDNAME_NO_ALLOCATION_LANGUAGE | 
								 UNDNAME_NO_MS_THISTYPE | UNDNAME_NO_ACCESS_SPECIFIERS | 
								 UNDNAME_NO_THISTYPE | UNDNAME_NO_MEMBER_TYPE | 
								 UNDNAME_NO_RETURN_UDT_MODEL | UNDNAME_NO_THROW_SIGNATURES |
								 UNDNAME_NO_SPECIAL_SYMS);
				
			sprintf(aBuffer, "%016llX %016llX %s+%lX\r\n", 
					sf.AddrFrame.Offset, sf.AddrPC.Offset, aUDName, symDisplacement);
		}
		else // No symbol found.  Print out the logical address instead.
		{
			char szModule[MAX_PATH];            
			DWORD section = 0, offset = 0;

			GetLogicalAddress((PVOID)sf.AddrPC.Offset, szModule, sizeof(szModule), section, offset);                
			sprintf(aBuffer, "%016llX %016llX %08lX:%08lX %s\r\n", sf.AddrFrame.Offset, sf.AddrPC.Offset, section, offset, GetFilename(szModule).c_str());
		}
		aDebugDump += aBuffer;
		
		sprintf(aBuffer, "Params: %016llX %016llX %016llX %016llX\r\n", sf.Params[0], sf.Params[1], sf.Params[2], sf.Params[3]);
		aDebugDump += aBuffer;      
		aDebugDump += "\r\n";

		aLevelCount++;
	}

	return aDebugDump;
}

bool SEHCatcher::GetLogicalAddress(void* addr, char* szModule, DWORD len, DWORD& section, DWORD& offset)
{
	MEMORY_BASIC_INFORMATION mbi;

	if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
		return false;

	intptr_t hMod = (intptr_t)mbi.AllocationBase;

	if (!GetModuleFileNameA((HMODULE) hMod, szModule, len))
		return false;

	// Point to the DOS header in memory
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER) hMod;

	// From the DOS header, find the NT (PE) header
	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS) (hMod + pDosHdr->e_lfanew);

	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNtHdr);

	intptr_t rva = (intptr_t) addr - hMod; // RVA is offset from module load address

	// Iterate through the section table, looking for the one that encompasses
	// the linear address.
	for (unsigned i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++, pSection++)
	{
		DWORD sectionStart = pSection->VirtualAddress;
		DWORD sectionEnd = sectionStart + std::max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

		// Is the address in this section???
		if ((rva >= sectionStart) && (rva <= sectionEnd))
		{
			// Yes, address is in the section.  Calculate section and offset,
			// and store in the "section" & "offset" params, which were
			// passed by reference.
			section = i+1;
			offset = rva - sectionStart;
			return true;
		}
	}

	return false; // Should never get here!
}

std::string SEHCatcher::GetFilename(const std::string& thePath)
{
	int aLastSlash = std::max((int) thePath.rfind('\\'), (int) thePath.rfind('/')); 

	if (aLastSlash >= 0)
	{
		return thePath.substr(aLastSlash+1);
	}
	else
		return thePath;
}

HWND gEditWindow = NULL;
int aCount = 0;

static LRESULT CALLBACK SEHProgressWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{       
	case WM_COMMAND:
		{
			HWND hwndCtl = (HWND) lParam;
			if (hwndCtl == SEHCatcher::mNoButtonWindow)
			{
				// Cancel it, man!
				SEHCatcher::mSubmitReportTransfer.Abort();
			}
		}
		break;
	case WM_CLOSE:
		SEHCatcher::mSubmitReportTransfer.Abort();
		return 0;
	case WM_TIMER:
		{
			if (wParam == 0)
			{
				int aResult = SEHCatcher::mSubmitReportTransfer.GetResultCode();

				if ((aResult != HTTPTransfer::RESULT_NOT_COMPLETED) &&
					(aResult != HTTPTransfer::RESULT_NOT_STARTED))
				{
					if ((aResult != HTTPTransfer::RESULT_DONE) &&
						(aResult != HTTPTransfer::RESULT_ABORTED))
					{
						SEHCatcher::mSubmitReportTransfer.Reset();

						if (::MessageBoxW(hWnd, SEHCatcher::mSubmitErrorMessage.c_str(), 
							L"Connection Failed", MB_RETRYCANCEL | MB_APPLMODAL) == IDRETRY)
						{
							_beginthread(SEHCatcher::SubmitReportThread, 0, 0);
							return 0;
						}
					}

					SEHCatcher::mExiting = true;
				}
			}
			else if (wParam == 1)
			{
				++aCount;

				std::string aNewString = "Please Wait";

				for (int i = 0; i < (aCount % 10); i++)
					aNewString += " .";
				
				SetWindowTextA(gEditWindow, aNewString.c_str());
			}
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static void CreateProgressWindow()
{
	WNDCLASSA wc;
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = ::GetSysColorBrush(COLOR_BTNFACE);
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = ::LoadIcon(NULL, IDI_ERROR);
	wc.hInstance = gHInstance;
	wc.lpfnWndProc = SEHProgressWindowProc;
	wc.lpszClassName = "SEHProgressWindow";
	wc.lpszMenuName = NULL; 
	RegisterClassA(&wc);

	RECT aRect;
		aRect.left = 0;
		aRect.top = 0;
		aRect.right = 240;
		aRect.bottom = 64;

	DWORD aWindowStyle = WS_CLIPCHILDREN | WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	BOOL worked = AdjustWindowRect(&aRect, aWindowStyle, FALSE);
	(void)worked; // unused, naughty naughty

	HWND aHWnd = CreateWindowA("SEHProgressWindow", "Submitting Report",
		aWindowStyle,
		64, 64,
		aRect.right - aRect.left, 
		aRect.bottom - aRect.top,
		NULL,
		NULL,
		gHInstance,
		0);

	// Check every 20ms to see if the transfer has completed
	SetTimer(aHWnd, 0, 20, NULL);

	// Every second we should change the edit text
	SetTimer(aHWnd, 1, 1000, NULL);

	gEditWindow = CreateWindowA("EDIT", 
		"Please Wait",
				
		WS_VISIBLE | WS_CHILD | ES_READONLY, 
		24, 10,
		240-8-8,
		24,
		aHWnd,
		NULL,
		gHInstance,
		0); 
	if (!gUseDefaultFonts)
		SendMessage(gEditWindow, WM_SETFONT, (WPARAM) SEHCatcher::mBoldFont, 0);

	SEHCatcher::mNoButtonWindow = CreateWindowA("BUTTON", "Abort", 
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_PUSHBUTTON, 
		(240 - 96)/2, 64 - 22 - 6,
		96,
		22,
		aHWnd,
		NULL,
		gHInstance,
		0);
	if (!gUseDefaultFonts)
		SendMessage(SEHCatcher::mNoButtonWindow, WM_SETFONT, (WPARAM) SEHCatcher::mDialogFont, 0);

	ShowWindow(aHWnd, SW_NORMAL);
}

LRESULT CALLBACK SEHCatcher::SEHWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		{
			HWND hwndCtl = (HWND) lParam;
			if (hwndCtl == mYesButtonWindow)
			{
				// Hide current window and bring up score submitting stuff
				ShowWindow(hWnd, SW_HIDE);

				ShowSubmitInfoDialog();
			}           
			else if (hwndCtl == mNoButtonWindow)
			{
				mExiting = true;
			}
			else if (hwndCtl == mDebugButtonWindow)
			{
				mDebugError = true;
				mExiting = true;
			}
		}
		break;
	case WM_CLOSE:
		mExiting = true;
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK SEHCatcher::SubmitInfoWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{       
	case WM_COMMAND:
		{
			HWND hwndCtl = (HWND) lParam;
			if (hwndCtl == mYesButtonWindow)
			{
				// Hide current window and bring up score submitting stuff
				ShowWindow(hWnd, SW_HIDE);

				char aStr[8192];
				GetWindowTextA(mEditWindow, aStr, 8192);
				mUserText = aStr;

				CreateProgressWindow();
				_beginthread(SubmitReportThread, 0, 0);
			}
			else if (hwndCtl == mNoButtonWindow)
			{
				mExiting = true;
			}
		}
		break;
	case WM_CLOSE:
		mExiting = true;
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void SEHCatcher::WriteToFile(const std::string& theErrorText)
{
	std::fstream aStream("crash.txt", std::ios::out);
	aStream << theErrorText.c_str() << std::endl;
}

void SEHCatcher::SubmitReportThread(void *theArg)
{
	std::string aSeperator = "---------------------------7d3e1f30eec";
	
	DefinesMap aSEHWebParams;

	mApp->GetSEHWebParams(&aSEHWebParams);

	std::string aContent;

	DefinesMap::iterator anItr = aSEHWebParams.begin();
	while (anItr != aSEHWebParams.end())
	{
		aContent += "--" + aSeperator + "\r\n"
		"Content-Disposition: form-data; name=\"" + anItr->first + "\"\r\n" +       
		"\r\n" +
		anItr->second + "\r\n";
		
		++anItr;
	}

	aContent += 
		
		/*"--" + aSeperator + "\r\n"
		"Content-Disposition: form-data; name=\"username\"\r\n" +       
		"\r\n" +
		mApp->mUserName + "\r\n" +  */

		"--" + aSeperator + "\r\n"
		"Content-Disposition: form-data; name=\"prod\"\r\n" +       
		"\r\n" +
		mApp->mProdName + "\r\n" +  

		"--" + aSeperator + "\r\n"
		"Content-Disposition: form-data; name=\"version\"\r\n" +        
		"\r\n" +
		mApp->mProductVersion + "\r\n" +

		/*"--" + aSeperator + "\r\n"
		"Content-Disposition: form-data; name=\"buildnum\"\r\n" +       
		"\r\n" +
		StrFormat("%d", mApp->mBuildNum) + "\r\n" +

		"--" + aSeperator + "\r\n"
		"Content-Disposition: form-data; name=\"builddate\"\r\n" +      
		"\r\n" +
		mApp->mBuildDate + "\r\n" +

		"--" + aSeperator + "\r\n"
		"Content-Disposition: form-data; name=\"referid\"\r\n" +        
		"\r\n" +
		mApp->mReferId + "\r\n" +*/

		"--" + aSeperator + "\r\n"
		"Content-Disposition: form-data; name=\"usertext\"\r\n" +
		"\r\n" +
		mUserText + "\r\n" +

		"--" + aSeperator + "\r\n"
		"Content-Disposition: form-data; name=\"errortitle\"\r\n" +     
		"\r\n" +
		mErrorTitle + "\r\n" +

		"--" + aSeperator + "\r\n"
		"Content-Disposition: form-data; name=\"errortext\"\r\n" +      
		"\r\n" +
		mErrorText + "\r\n";

	if (mHasDemoFile)
	{
		Buffer aBuffer;
		mApp->ReadBufferFromFile(mUploadFileName, &aBuffer, true);

		aContent += 
			"--" + aSeperator + "\r\n"
			"Content-Disposition: form-data; name=\"demofile\"; filename=\"popcap.dmo\"\r\n" +
			"Content-Type: application/octet-stream\r\n" +
			"\r\n";     

		aContent.insert(aContent.end(), (char*) aBuffer.GetDataPtr(), (char*) aBuffer.GetDataPtr() + aBuffer.GetDataLen());         
				
		aContent += "\r\n";
	}

	aContent +=
		"--" + aSeperator + "--\r\n";

	std::string aSendString = 
		"POST /deluxe_error.php HTTP/1.1\r\n"
		"Content-Type: multipart/form-data; boundary=" + aSeperator + "\r\n"
		"User-Agent: Mozilla/4.0 (compatible; popcap)\r\n" +
		"Host: " + mSubmitHost + "\r\n" +
		"Content-Length: " + StrFormat("%d", aContent.length()) + "\r\n" +
		"Connection: close\r\n" +
		"\r\n" + aContent;
	
	mSubmitReportTransfer.SendRequestString(mSubmitHost, aSendString);
}

void SEHCatcher::ShowSubmitInfoDialog()
{
	WNDCLASSA wc;
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = ::GetSysColorBrush(COLOR_BTNFACE);
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = ::LoadIcon(NULL, IDI_ERROR);
	wc.hInstance = gHInstance;
	wc.lpfnWndProc = SubmitInfoWindowProc;
	wc.lpszClassName = "SubmitInfoWindow";
	wc.lpszMenuName = NULL; 
	RegisterClassA(&wc);

	RECT aRect;
		aRect.left = 0;
		aRect.top = 0;
		aRect.right = 400;
		aRect.bottom = 300;

	DWORD aWindowStyle = WS_CLIPCHILDREN | WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	BOOL worked = AdjustWindowRect(&aRect, aWindowStyle, FALSE);
	(void)worked; // unused, naughty naughty

	HWND aHWnd = CreateWindowA("SubmitInfoWindow", "Error Details",
		aWindowStyle,
		64+16, 64+16,
		aRect.right - aRect.left, 
		aRect.bottom - aRect.top,
		NULL,
		NULL,
		gHInstance,
		0);

	HWND aLabelWindow = CreateWindowW(L"EDIT", mSubmitMessage.c_str(),
				
		WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY, 
		8, 8,
		400-8-8,
		84,
		aHWnd,
		NULL,
		gHInstance,
		0);

	HDC aDC = ::GetDC(aLabelWindow);
	int aFontHeight = -MulDiv(9, 96, 72);
	::ReleaseDC(aLabelWindow, aDC);
	HFONT aBoldArialFont = CreateFontA(aFontHeight, 0, 0, 0, FW_BOLD, 0, 0,
			false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Arial");

	if (!gUseDefaultFonts)
		SendMessage(aLabelWindow, WM_SETFONT, (WPARAM) aBoldArialFont, 0);

	mEditWindow = CreateWindowA("EDIT", "", 
		WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER | WS_VSCROLL, 
		8, 300-168-24-8-8,
		400-8-8,
		168,
		aHWnd,
		NULL,
		gHInstance,
		0); 

	aDC = ::GetDC(mEditWindow);
	aFontHeight = -MulDiv(8, 96, 72);
	::ReleaseDC(mEditWindow, aDC);
	HFONT aCourierNewFont = CreateFontA(aFontHeight, 0, 0, 0, FW_NORMAL, 0, 0,
			false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Courier New");
	(void)aCourierNewFont; // Unused, TODO work out if can be removed
	if (!gUseDefaultFonts)
		SendMessage(mEditWindow, WM_SETFONT, (WPARAM) mDialogFont, 0);
	SetFocus(mEditWindow);

	int aButtonWidth = (400 - 8 - 8 - 8) / 2;
	int aCurX = 8;

	aWindowStyle = WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_PUSHBUTTON;

	if (mApp == NULL)
		aWindowStyle |= WS_DISABLED;

	mYesButtonWindow = CreateWindowA("BUTTON", "Continue", 
		aWindowStyle, 
		aCurX, 300-24-8,
		aButtonWidth,
		24,
		aHWnd,
		NULL,
		gHInstance,
		0);
	if (!gUseDefaultFonts)
		SendMessage(mYesButtonWindow, WM_SETFONT, (WPARAM) aBoldArialFont, 0);

	aCurX += aButtonWidth+8;

	mNoButtonWindow = CreateWindowA("BUTTON", "Abort", 
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_PUSHBUTTON, 
		aCurX, 300-24-8,
		aButtonWidth,
		24,
		aHWnd,
		NULL,
		gHInstance,
		0);
	if (!gUseDefaultFonts)
		SendMessage(mNoButtonWindow, WM_SETFONT, (WPARAM) aBoldArialFont, 0);

	ShowWindow(aHWnd, SW_NORMAL);
}

void SEHCatcher::ShowErrorDialog(const std::string& theErrorTitle, const std::string& theErrorText)
{
	OSVERSIONINFO aVersionInfo;
	aVersionInfo.dwOSVersionInfoSize = sizeof(aVersionInfo);
	GetVersionEx(&aVersionInfo);

	// Setting fonts on 98 causes weirdo crash things in GDI upon the second crash.
	//  That's no good. 
	gUseDefaultFonts = aVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT;

	int aHeight = -MulDiv(8, 96, 72);
	mDialogFont = ::CreateFontA(aHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE,
			false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Tahoma");

	aHeight = -MulDiv(10, 96, 72);
	mBoldFont = ::CreateFontA(aHeight, 0, 0, 0, FW_BOLD, FALSE, FALSE,
			false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Tahoma"); 

	::SetCursor(::LoadCursor(NULL, IDC_ARROW));

	mErrorTitle = theErrorTitle;
	mErrorText = theErrorText;  


	WNDCLASSA wc;
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = ::GetSysColorBrush(COLOR_BTNFACE);
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = ::LoadIcon(NULL, IDI_ERROR);
	wc.hInstance = gHInstance;
	wc.lpfnWndProc = SEHWindowProc;
	wc.lpszClassName = "SEHWindow";
	wc.lpszMenuName = NULL; 
	RegisterClassA(&wc);

	RECT aRect;
		aRect.left = 0;
		aRect.top = 0;
		aRect.right = 400;
		aRect.bottom = 300;

	DWORD aWindowStyle = WS_CLIPCHILDREN | WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	BOOL worked = AdjustWindowRect(&aRect, aWindowStyle, FALSE);
	(void)worked; // unused, naughty naughty

	HWND aHWnd = CreateWindowW(L"SEHWindow", L"Fatal Error!",
		aWindowStyle,
		64, 64,
		aRect.right - aRect.left, 
		aRect.bottom - aRect.top,
		NULL,
		NULL,
		gHInstance,
		0);

	HWND aLabelWindow = CreateWindowW(L"EDIT", 
		mCrashMessage.c_str(),
		WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY, 
		8, 8,
		400-8-8,
		84,
		aHWnd,
		NULL,
		gHInstance,
		0);
	
	int aFontHeight = -MulDiv(9, 96, 72);   
	HFONT aBoldArialFont = CreateFontA(aFontHeight, 0, 0, 0, FW_BOLD, 0, 0,
			false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Arial");

	if (!gUseDefaultFonts)
		SendMessage(aLabelWindow, WM_SETFONT, (WPARAM) aBoldArialFont, 0);      

	HWND anEditWindow = CreateWindowA("EDIT", theErrorText.c_str(), 
		WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER | WS_VSCROLL | ES_READONLY, 
		8, 300-168-24-8-8,
		400-8-8,
		168,
		aHWnd,
		NULL,
		gHInstance,
		0); 
	
	aFontHeight = -MulDiv(8, 96, 72);   
	HFONT aCourierNewFont = CreateFontA(aFontHeight, 0, 0, 0, FW_NORMAL, 0, 0,
			false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Courier New");
	if (!gUseDefaultFonts)
		SendMessage(anEditWindow, WM_SETFONT, (WPARAM) aCourierNewFont, 0);

	aWindowStyle = WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_PUSHBUTTON;

	if (mApp == NULL)
		aWindowStyle |= WS_DISABLED;

#ifdef _DEBUG
	bool doDebugButton = true;
#else
	bool doDebugButton = false;
#endif

	bool canSubmit = mAllowSubmit && !mSubmitHost.empty();  
	int aNumButtons = 1 + (doDebugButton ? 1 : 0) + (canSubmit ? 1 : 0);
	
	int aButtonWidth = (400 - 8 - 8 - (aNumButtons - 1)*8) / aNumButtons;       
		
	int aCurX = 8;

	if (canSubmit)
	{
		mYesButtonWindow = CreateWindowA("BUTTON", "Send Report", 
			aWindowStyle, 
			aCurX, 300-24-8,
			aButtonWidth,
			24,
			aHWnd,
			NULL,
			gHInstance,
			0);
		if (!gUseDefaultFonts)
			SendMessage(mYesButtonWindow, WM_SETFONT, (WPARAM) aBoldArialFont, 0);

		aCurX += aButtonWidth + 8;  
	}

	if (doDebugButton)
	{
		mDebugButtonWindow = CreateWindowA("BUTTON", "Debug", 
			aWindowStyle, 
			aCurX, 300-24-8,
			aButtonWidth,
			24,
			aHWnd,
			NULL,
			gHInstance,
			0);     
		if (!gUseDefaultFonts)
			SendMessage(mDebugButtonWindow, WM_SETFONT, (WPARAM) aBoldArialFont, 0);        

		aCurX += aButtonWidth + 8;
	}

	mNoButtonWindow = CreateWindowA("BUTTON", "Close Now", 
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_PUSHBUTTON, 
		aCurX, 300-24-8,
		aButtonWidth,
		24,
		aHWnd,
		NULL,
		gHInstance,
		0);

	if (!gUseDefaultFonts)
		SendMessage(mNoButtonWindow, WM_SETFONT, (WPARAM) aBoldArialFont, 0);

	ShowWindow(aHWnd, SW_NORMAL);

	MSG msg;
	while ((GetMessage(&msg, NULL, 0, 0) > 0) && (!mExiting))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}   

	DestroyWindow(aHWnd);

	DeleteObject(mDialogFont);
	DeleteObject(mBoldFont);
	DeleteObject(aBoldArialFont);
	DeleteObject(aCourierNewFont);
}

std::string SEHCatcher::GetSysInfo()
{
	std::string aDebugDump;

	OSVERSIONINFOA aVersionInfo;
	aVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionExA(&aVersionInfo); 

	aDebugDump += "Windows Ver: ";
	if (aVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		aDebugDump += "NT ";
	else
		aDebugDump += "9x ";

	char aVersionStr[20];
	sprintf(aVersionStr, "%ld.%ld", aVersionInfo.dwMajorVersion, aVersionInfo.dwMinorVersion);
	aDebugDump += aVersionStr;
	aDebugDump += " ";
	aDebugDump += aVersionInfo.szCSDVersion;
	aDebugDump += " ";
	sprintf(aVersionStr, "%ld", aVersionInfo.dwBuildNumber);
	aDebugDump += "Build ";
	aDebugDump += aVersionStr;
	aDebugDump += "\r\n";

	HMODULE aMod;
	char aPath[256];

	if (mApp != NULL)
	{
		aMod = LoadLibraryA("ddraw.dll");
		if (aMod != NULL)
		{
			GetModuleFileNameA(aMod, aPath, 256);           
			aDebugDump += "DDraw Ver: " + mApp->GetProductVersion(aPath) + "\r\n";
			FreeLibrary(aMod);
		}

		aMod = LoadLibraryA("dsound.dll");
		if (aMod != NULL)
		{
			GetModuleFileNameA(aMod, aPath, 256);           
			aDebugDump += "DSound Ver: " + mApp->GetProductVersion(aPath) + "\r\n";
			FreeLibrary(aMod);
		}
	}   

	return aDebugDump;
}   

