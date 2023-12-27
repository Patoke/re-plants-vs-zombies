#include "Common.h"
#include "misc/MTRand.h"
#include "misc/Debug.h"
//#include <direct.h>
//#include <io.h>
#include <cstdarg>
#include <chrono>
#include <cwchar>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
//#include <aclapi.h>

#include "misc/PerfTimer.h"

//HINSTANCE Sexy::gHInstance;
bool Sexy::gDebug = false;
static Sexy::MTRand gMTRand;
namespace Sexy
{
	std::string gAppDataFolder = "";
}

int Sexy::Rand()
{
	return gMTRand.Next();
}

int Sexy::Rand(int range)
{
	return gMTRand.Next((unsigned long)range);
}

float Sexy::Rand(float range)
{
	return gMTRand.Next(range);
}

void Sexy::SRand(ulong theSeed)
{
	gMTRand.SRand(theSeed);
}

bool Sexy::CheckFor98Mill()
{
	unreachable(); // FIXME (sort of, really it just needs removing)
/*
	static bool needOsCheck = true;
	static bool is98Mill = false;

	if (needOsCheck)
	{
		// bool invalid = false; // unused
		OSVERSIONINFOEXA osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEXA));

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
		if( GetVersionExA((LPOSVERSIONINFOA)&osvi) == 0)
		{
			osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFOA);
			if ( GetVersionExA((LPOSVERSIONINFOA)&osvi) == 0)
				return false;
		}

		needOsCheck = false;
		is98Mill = osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS; // let's check Win95, 98, *AND* ME.
	}

	return is98Mill;*/
}

bool Sexy::CheckForVista()
{
	unreachable(); // FIXME (sort of, really it just needs removing)
	/*
	static bool needOsCheck = true;
	static bool isVista = false;

	if (needOsCheck)
	{
		// bool invalid = false; // unused
		OSVERSIONINFOEXA osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEXA));

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
		if( GetVersionExA((LPOSVERSIONINFOA)&osvi) == 0)
		{
			osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFOA);
			if ( GetVersionExA((LPOSVERSIONINFOA)&osvi) == 0)
				return false;
		}

		needOsCheck = false;
		isVista = osvi.dwMajorVersion >= 6;
	}

	return isVista;*/
}

std::string Sexy::GetAppDataFolder()
{
	return Sexy::gAppDataFolder;
}

void Sexy::SetAppDataFolder(const std::string& thePath)
{
	if (CheckForVista())
	{
		std::string aPath = thePath;
		if (!aPath.empty())
		{
			if (aPath[aPath.length()-1] != '\\' && aPath[aPath.length()-1] != '/')
				aPath += '\\';
		}

		Sexy::gAppDataFolder = aPath;
	}
}


std::string Sexy::URLEncode(const std::string& theString)
{
	char* aHexChars = (char*)"0123456789ABCDEF";

	std::string aString;

	for (unsigned i = 0; i < theString.length(); i++)
	{
		switch (theString[i])
		{
		case ' ':
			aString.insert(aString.end(), '+');
			break;
		case '?':
		case '&':
		case '%':
		case '+':
		case '\r':
		case '\n':
		case '\t':
			aString.insert(aString.end(), '%');
			aString.insert(aString.end(), aHexChars[(theString[i] >> 4) & 0xF]);
			aString.insert(aString.end(), aHexChars[(theString[i]     ) & 0xF]);
			break;
		default:
			aString.insert(aString.end(), theString[i]);
		}
	}

	return aString;
}

std::string Sexy::StringToUpper(const std::string& theString)
{
	std::string aString;

	for (unsigned i = 0; i < theString.length(); i++)
		aString += toupper(theString[i]);

	return aString;
}

std::wstring Sexy::StringToUpper(const std::wstring& theString)
{
	std::wstring aString;

	for (unsigned i = 0; i < theString.length(); i++)
		aString += towupper(theString[i]);

	return aString;
}

std::string Sexy::StringToLower(const std::string& theString)
{
	std::string aString;

	for (unsigned i = 0; i < theString.length(); i++)
		aString += tolower(theString[i]);

	return aString;
}

std::wstring Sexy::StringToLower(const std::wstring& theString)
{
	std::wstring aString;

	for (unsigned i = 0; i < theString.length(); ++i)
		aString += tolower(theString[i]);

	return aString;
}

std::wstring Sexy::StringToWString(const std::string &theString)
{
	std::wstring aString;
	aString.reserve(theString.length());
	for(size_t i = 0; i < theString.length(); ++i)
		aString += (unsigned char)theString[i];
	return aString;
}

std::string Sexy::WStringToString(const std::wstring &theString)
{
	size_t aRequiredLength = wcstombs( NULL, theString.c_str(), 0 );
	if (aRequiredLength < 16384)
	{
		char aBuffer[16384];
		wcstombs( aBuffer, theString.c_str(), 16384 );
		return std::string(aBuffer);
	}
	else
	{
		DBG_ASSERTE(aRequiredLength != (size_t)-1);
		if (aRequiredLength == (size_t)-1) return "";

		char* aBuffer = new char[aRequiredLength+1];
		wcstombs( aBuffer, theString.c_str(), aRequiredLength+1 );
		std::string aStr = aBuffer;
		delete[] aBuffer;
		return aStr;
	}
}

SexyString Sexy::StringToSexyString(const std::string& theString)
{
#ifdef _USE_WIDE_STRING
	return StringToWString(theString);
#else
	return SexyString(theString);
#endif
}

SexyString Sexy::WStringToSexyString(const std::wstring &theString)
{
#ifdef _USE_WIDE_STRING
	return SexyString(theString);
#else
	return WStringToString(theString);
#endif
}

std::string Sexy::SexyStringToString(const SexyString& theString)
{
#ifdef _USE_WIDE_STRING
	return WStringToString(theString);
#else
	return std::string(theString);
#endif
}

std::wstring Sexy::SexyStringToWString(const SexyString& theString)
{
#ifdef _USE_WIDE_STRING
	return std::wstring(theString);
#else
	return StringToWString(theString);
#endif
}

std::string Sexy::Trim(const std::string& theString)
{
	int aStartPos = 0;
	while ( aStartPos < (int) theString.length() && isspace(theString[aStartPos]) )
		aStartPos++;

	int anEndPos = theString.length() - 1;
	while ( anEndPos >= 0 && isspace(theString[anEndPos]) )
		anEndPos--;

	return theString.substr(aStartPos, anEndPos - aStartPos + 1);
}

std::wstring Sexy::Trim(const std::wstring& theString)
{   //0x5AFD80
	int aStartPos = 0;
	while ( aStartPos < (int) theString.length() && iswspace(theString[aStartPos]) )
		aStartPos++;

	int anEndPos = theString.length() - 1;
	while ( anEndPos >= 0 && iswspace(theString[anEndPos]) )
		anEndPos--;

	return theString.substr(aStartPos, anEndPos - aStartPos + 1);
}

bool Sexy::StringToInt(const std::string theString, int* theIntVal)
{
	*theIntVal = 0;

	if (theString.length() == 0)
		return false;

	int theRadix = 10;
	bool isNeg = false;

	unsigned i = 0;
	if (theString[i] == '-')
	{
		isNeg = true;
		i++;
	}

	for (; i < theString.length(); i++)
	{
		char aChar = theString[i];
		
		if ((theRadix == 10) && (aChar >= '0') && (aChar <= '9'))
			*theIntVal = (*theIntVal * 10) + (aChar - '0');
		else if ((theRadix == 0x10) && 
			(((aChar >= '0') && (aChar <= '9')) || 
			 ((aChar >= 'A') && (aChar <= 'F')) || 
			 ((aChar >= 'a') && (aChar <= 'f'))))
		{			
			if (aChar <= '9')
				*theIntVal = (*theIntVal * 0x10) + (aChar - '0');
			else if (aChar <= 'F')
				*theIntVal = (*theIntVal * 0x10) + (aChar - 'A') + 0x0A;
			else
				*theIntVal = (*theIntVal * 0x10) + (aChar - 'a') + 0x0A;
		}
		else if (((aChar == 'x') || (aChar == 'X')) && (i == 1) && (*theIntVal == 0))
		{
			theRadix = 0x10;
		}
		else
		{
			*theIntVal = 0;
			return false;
		}
	}

	if (isNeg)
		*theIntVal = -*theIntVal;

	return true;
}

bool Sexy::StringToInt(const std::wstring theString, int* theIntVal)
{
	*theIntVal = 0;

	if (theString.length() == 0)
		return false;

	int theRadix = 10;
	bool isNeg = false;

	unsigned i = 0;
	if (theString[i] == '-')
	{
		isNeg = true;
		i++;
	}

	for (; i < theString.length(); i++)
	{
		wchar_t aChar = theString[i];
		
		if ((theRadix == 10) && (aChar >= L'0') && (aChar <= L'9'))
			*theIntVal = (*theIntVal * 10) + (aChar - L'0');
		else if ((theRadix == 0x10) && 
			(((aChar >= L'0') && (aChar <= L'9')) || 
			 ((aChar >= L'A') && (aChar <= L'F')) || 
			 ((aChar >= L'a') && (aChar <= L'f'))))
		{			
			if (aChar <= L'9')
				*theIntVal = (*theIntVal * 0x10) + (aChar - L'0');
			else if (aChar <= L'F')
				*theIntVal = (*theIntVal * 0x10) + (aChar - L'A') + 0x0A;
			else
				*theIntVal = (*theIntVal * 0x10) + (aChar - L'a') + 0x0A;
		}
		else if (((aChar == L'x') || (aChar == L'X')) && (i == 1) && (*theIntVal == 0))
		{
			theRadix = 0x10;
		}
		else
		{
			*theIntVal = 0;
			return false;
		}
	}

	if (isNeg)
		*theIntVal = -*theIntVal;

	return true;
}

bool Sexy::StringToDouble(const std::string theString, double* theDoubleVal)
{
	*theDoubleVal = 0.0;

	if (theString.length() == 0)
		return false;

	bool isNeg = false;

	unsigned i = 0;
	if (theString[i] == '-')
	{
		isNeg = true;
		i++;
	}

	for (; i < theString.length(); i++)
	{
		char aChar = theString[i];

		if ((aChar >= '0') && (aChar <= '9'))
			*theDoubleVal = (*theDoubleVal * 10) + (aChar - '0');
		else if (aChar == '.')
		{
			i++;
			break;
		}
		else
		{
			*theDoubleVal = 0.0;
			return false;
		}
	}

	double aMult = 0.1;
	for (; i < theString.length(); i++)
	{
		char aChar = theString[i];

		if ((aChar >= '0') && (aChar <= '9'))
		{
			*theDoubleVal += (aChar - '0') * aMult;	
			aMult /= 10.0;
		}
		else
		{
			*theDoubleVal = 0.0;
			return false;
		}
	}

	if (isNeg)
		*theDoubleVal = -*theDoubleVal;

	return true;
}

bool Sexy::StringToDouble(const std::wstring theString, double* theDoubleVal)
{
	*theDoubleVal = 0.0;

	if (theString.length() == 0)
		return false;

	bool isNeg = false;

	unsigned i = 0;
	if (theString[i] == '-')
	{
		isNeg = true;
		i++;
	}

	for (; i < theString.length(); i++)
	{
		wchar_t aChar = theString[i];

		if ((aChar >= L'0') && (aChar <= L'9'))
			*theDoubleVal = (*theDoubleVal * 10) + (aChar - L'0');
		else if (aChar == L'.')
		{
			i++;
			break;
		}
		else
		{
			*theDoubleVal = 0.0;
			return false;
		}
	}

	double aMult = 0.1;
	for (; i < theString.length(); i++)
	{
		wchar_t aChar = theString[i];

		if ((aChar >= L'0') && (aChar <= L'9'))
		{
			*theDoubleVal += (aChar - L'0') * aMult;	
			aMult /= 10.0;
		}
		else
		{
			*theDoubleVal = 0.0;
			return false;
		}
	}

	if (isNeg)
		*theDoubleVal = -*theDoubleVal;

	return true;
}

// TODO: Use <locale> for localization of number output?
SexyString Sexy::CommaSeperate(int theValue)
{	
	if (theValue == 0)
		return _S("0");

	SexyString aCurString;

	int aPlace = 0;
	int aCurValue = theValue;

	while (aCurValue > 0)
	{
		if ((aPlace != 0) && (aPlace % 3 == 0))
			aCurString = _S(',') + aCurString;
		aCurString = (SexyChar) (_S('0') + (aCurValue % 10)) + aCurString;
		aCurValue /= 10;
		aPlace++;
	}

	return aCurString;
}

std::string Sexy::GetCurDir()
{
	char aDir[256];
	return getcwd(aDir, sizeof(aDir));
}

std::string Sexy::GetFullPath(const std::string& theRelPath)
{
	return GetPathFrom(theRelPath, GetCurDir());
}

std::string Sexy::GetPathFrom(const std::string& theRelPath, const std::string& theDir)
{
	std::string aDriveString;
	std::string aNewPath = theDir;

	if ((theRelPath.length() >= 2) && (theRelPath[1] == ':'))
		return theRelPath;

	char aSlashChar = '/';

	/* This test doesn't work on linux and it fails to create our delicious folder.
	if ((theRelPath.find('\\') != (size_t)-1) || (theDir.find('\\') != (size_t)-1))
		aSlashChar = '\\';*/

	if ((aNewPath.length() >= 2) && (aNewPath[1] == ':'))
	{
		aDriveString = aNewPath.substr(0, 2);
		aNewPath.erase(aNewPath.begin(), aNewPath.begin()+2);
	}

	// Append a trailing slash if necessary
	if ((aNewPath.length() > 0) && (aNewPath[aNewPath.length()-1] != '\\') && (aNewPath[aNewPath.length()-1] != '/'))
		aNewPath += aSlashChar;

	std::string aTempRelPath = theRelPath;

	for (;;)
	{
		if (aNewPath.length() == 0)
			break;

		int aFirstSlash = aTempRelPath.find('\\');
		int aFirstForwardSlash = aTempRelPath.find('/');

		if ((aFirstSlash == -1) || ((aFirstForwardSlash != -1) && (aFirstForwardSlash < aFirstSlash)))
			aFirstSlash = aFirstForwardSlash;

		if (aFirstSlash == -1)
			break;

		std::string aChDir = aTempRelPath.substr(0, aFirstSlash);

		aTempRelPath.erase(aTempRelPath.begin(), aTempRelPath.begin() + aFirstSlash + 1);						

		if (aChDir.compare("..") == 0)
		{			
			int aLastDirStart = aNewPath.length() - 1;
			while ((aLastDirStart > 0) && (aNewPath[aLastDirStart-1] != '\\') && (aNewPath[aLastDirStart-1] != '/'))
				aLastDirStart--;

			std::string aLastDir = aNewPath.substr(aLastDirStart, aNewPath.length() - aLastDirStart - 1);
			if (aLastDir.compare("..") == 0)
			{
				aNewPath += "..";
				aNewPath += aSlashChar;
			}
			else
			{
				aNewPath.erase(aNewPath.begin() + aLastDirStart, aNewPath.end());
			}
		}		
		else if (aChDir.compare("") == 0)
		{
			aNewPath = aSlashChar;
			break;
		}
		else if (aChDir.compare(".") != 0)
		{
			aNewPath += aChDir + aSlashChar;
			break;
		}
	}

	aNewPath = aDriveString + aNewPath + aTempRelPath;

	if (aSlashChar == '/')
	{
		for (;;)
		{
			int aSlashPos = aNewPath.find('\\');
			if (aSlashPos == -1)
				break;
			aNewPath[aSlashPos] = '/';
		}
	}
	else
	{
		for (;;)
		{
			int aSlashPos = aNewPath.find('/');
			if (aSlashPos == -1)
				break;
			aNewPath[aSlashPos] = '\\';
		}
	}

	return aNewPath;
}

bool Sexy::AllowAllAccess(const std::string& /*theFileName*/)
{
	unreachable(); // FIXME (probably not even important)
/*
	HMODULE aLib = LoadLibraryA("advapi32.dll");
	if (aLib == NULL)
		return false;

	BOOL (WINAPI *fnSetFileSecurity)(LPCTSTR lpFileName, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor);
	BOOL (WINAPI *fnSetSecurityDescriptorDacl)(PSECURITY_DESCRIPTOR pSecurityDescriptor, BOOL bDaclPresent, PACL pDacl, BOOL bDaclDefaulted);
	BOOL (WINAPI *fnInitializeSecurityDescriptor)(PSECURITY_DESCRIPTOR pSecurityDescriptor, DWORD dwRevision);
	BOOL (WINAPI *fnAllocateAndInitializeSid)(
	  PSID_IDENTIFIER_AUTHORITY pIdentifierAuthority,
	  BYTE nSubAuthorityCount,
	  DWORD dwSubAuthority0,
	  DWORD dwSubAuthority1,
	  DWORD dwSubAuthority2,
	  DWORD dwSubAuthority3,
	  DWORD dwSubAuthority4,
	  DWORD dwSubAuthority5,
	  DWORD dwSubAuthority6,
	  DWORD dwSubAuthority7,
	  PSID* pSid
	);
	DWORD (WINAPI *fnSetEntriesInAcl)(ULONG cCountOfExplicitEntries, PEXPLICIT_ACCESS pListOfExplicitEntries, PACL OldAcl, PACL* NewAcl);
	PVOID (WINAPI *fnFreeSid)(PSID pSid);

	*(void**)&fnSetFileSecurity = (void*)GetProcAddress(aLib, "SetFileSecurityA");
	*(void**)&fnSetSecurityDescriptorDacl = (void*)GetProcAddress(aLib, "SetSecurityDescriptorDacl");
	*(void**)&fnInitializeSecurityDescriptor = (void*)GetProcAddress(aLib, "InitializeSecurityDescriptor");
	*(void**)&fnAllocateAndInitializeSid = (void*)GetProcAddress(aLib, "AllocateAndInitializeSid");
	*(void**)&fnSetEntriesInAcl = (void*)GetProcAddress(aLib, "SetEntriesInAclA");
	*(void**)&fnFreeSid = (void*) GetProcAddress(aLib, "FreeSid");

	if (!(fnSetFileSecurity && fnSetSecurityDescriptorDacl && fnInitializeSecurityDescriptor && fnAllocateAndInitializeSid && fnSetEntriesInAcl && fnFreeSid))
	{
		FreeLibrary(aLib);
		return false;
	}


	PSID pEveryoneSID = NULL;
	SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	bool result = false;

    // Create a well-known SID for the Everyone group.
    if (fnAllocateAndInitializeSid(&SIDAuthWorld, 1,
                     SECURITY_WORLD_RID,
                     0, 0, 0, 0, 0, 0, 0,
                     &pEveryoneSID))
    {
		EXPLICIT_ACCESS ea;

		// Initialize an EXPLICIT_ACCESS structure for an ACE.
		// The ACE will allow Everyone read access to the key.
		ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
		ea.grfAccessPermissions = STANDARD_RIGHTS_ALL | SPECIFIC_RIGHTS_ALL;
		ea.grfAccessMode = SET_ACCESS;
		ea.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
		ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
		ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
		ea.Trustee.ptstrName = (LPTSTR) pEveryoneSID;

		// Create a new ACL that contains the new ACEs.
		PACL pACL = NULL; 
		if (fnSetEntriesInAcl(1, &ea, NULL, &pACL) == ERROR_SUCCESS)
		{		
			// Initialize a security descriptor.  
			PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR) new char[SECURITY_DESCRIPTOR_MIN_LENGTH]; 
						 
			if (fnInitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)) 
			{  							 
				// Add the ACL to the security descriptor. 
				if (fnSetSecurityDescriptorDacl(pSD, 
						TRUE,     // bDaclPresent flag   
						pACL, 
						FALSE))   // not a default DACL 
				{
					if (fnSetFileSecurity(theFileName.c_str(), DACL_SECURITY_INFORMATION, pSD))
						result = true;
				}
			}
		}
	}

	FreeLibrary(aLib);
	return result;*/
}

bool Sexy::Deltree(const std::string& thePath)
{
	return std::filesystem::remove_all(thePath);
	//unreachable(); // FIXME
	/*
	bool success = true;

	std::string aSourceDir = thePath;
	
	if (aSourceDir.length() < 2)
		return false;

	if ((aSourceDir[aSourceDir.length() - 1] != '\\') ||
		(aSourceDir[aSourceDir.length() - 1] != '/'))
		aSourceDir += "\\";		
	
	WIN32_FIND_DATAA aFindData;

	HANDLE aFindHandle = FindFirstFileA((aSourceDir + "*.*").c_str(), &aFindData); 
	if (aFindHandle == INVALID_HANDLE_VALUE)
		return false;
	
	do
	{		
		if ((aFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{
			if ((strcmp(aFindData.cFileName, ".") != 0) &&
				(strcmp(aFindData.cFileName, "..") != 0))
			{
				// Follow the directory
				if (!Deltree(aSourceDir + aFindData.cFileName))
					success = false;
			}
		}
		else
		{	
			std::string aFullName = aSourceDir + aFindData.cFileName;
			if (!DeleteFileA(aFullName.c_str()))
				success = false;
		}
	}
	while (FindNextFileA(aFindHandle, &aFindData));
	FindClose(aFindHandle);

	if (rmdir(thePath.c_str()) == 0)
		success = false;

	return success;*/
}

bool Sexy::FileExists(const std::string& theFileName)
{
	return std::filesystem::exists(theFileName);
	/*
	WIN32_FIND_DATAA aFindData;
	
	HANDLE aFindHandle = FindFirstFileA(theFileName.c_str(), &aFindData); 
	if (aFindHandle == INVALID_HANDLE_VALUE)
		return false;

	FindClose(aFindHandle);
	return true;*/
}

void Sexy::MkDir(const std::string& theDir)
{
	std::filesystem::create_directories(theDir);
	/*
	std::string aPath = theDir;

	int aCurPos = 0;
	for (;;)
	{
		int aSlashPos = aPath.find_first_of("\\/", aCurPos);
		if (aSlashPos == -1)
		{
			_mkdir(aPath.c_str());
			break;
		}

		aCurPos = aSlashPos+1;

		std::string aCurPath = aPath.substr(0, aSlashPos);
		_mkdir(aCurPath.c_str());
	}*/
}

std::string Sexy::GetFileName(const std::string& thePath, bool noExtension)
{
	int aLastSlash = std::max((int) thePath.rfind('\\'), (int) thePath.rfind('/'));

	if (noExtension)
	{
		int aLastDot = (int)thePath.rfind('.');
		if (aLastDot > aLastSlash)
			return thePath.substr(aLastSlash + 1, aLastDot - aLastSlash - 1);
	}

	if (aLastSlash == -1)
		return thePath;
	else
		return thePath.substr(aLastSlash + 1);
}

std::string Sexy::GetFileDir(const std::string& thePath, bool withSlash)
{
	int aLastSlash = std::max((int) thePath.rfind('\\'), (int) thePath.rfind('/'));

	if (aLastSlash == -1)
		return "";
	else
	{
		if (withSlash)
			return thePath.substr(0, aLastSlash+1);
		else
			return thePath.substr(0, aLastSlash);
	}
}

std::string Sexy::RemoveTrailingSlash(const std::string& theDirectory)
{
	int aLen = theDirectory.length();
	
	if ((aLen > 0) && ((theDirectory[aLen-1] == '\\') || (theDirectory[aLen-1] == '/')))
		return theDirectory.substr(0, aLen - 1);
	else
		return theDirectory;
}

std::string	Sexy::AddTrailingSlash(const std::string& theDirectory, bool backSlash)
{
	if (!theDirectory.empty())
	{
		char aChar = theDirectory[theDirectory.length()-1];
		if (aChar!='\\' && aChar!='/')
			return theDirectory + (backSlash?'\\':'/');
		else
			return theDirectory;
	}
	else
		return "";
}


time_t Sexy::GetFileDate(const std::string& theFileName)
{
	const auto fileTime = std::filesystem::last_write_time(theFileName);
	const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(fileTime);
	return std::chrono::system_clock::to_time_t(systemTime);

	/*
	time_t aFileDate = 0;

	WIN32_FIND_DATAA aFindData;
	HANDLE aFindHandle = ::FindFirstFileA(theFileName.c_str(), &aFindData);

	if (aFindHandle != INVALID_HANDLE_VALUE)
	{		
		FILETIME aFileTime = aFindData.ftLastWriteTime;
						
		//FileTimeToUnixTime(&aFileTime, &aFileDate, FALSE);

		LONGLONG ll = (__int64) aFileTime.dwHighDateTime << 32;
		ll = ll + aFileTime.dwLowDateTime - 116444736000000000;
		aFileDate = (time_t) (ll/10000000);

		FindClose(aFindHandle);
	}

	return aFileDate;*/
}

std::string Sexy::vformat(const char* fmt, va_list argPtr) 
{
    // We draw the line at a 1MB string.
    const int maxSize = 1000000;

    // If the string is less than 161 characters,
    // allocate it on the stack because this saves
    // the malloc/free time.
    const int bufSize = 161;
	char stackBuffer[bufSize];

    int attemptedSize = bufSize - 1;

	int numChars = 0;
#ifdef _WIN32
	numChars = _vsnprintf(stackBuffer, attemptedSize, fmt, argPtr);
#else
	numChars = vsnprintf(stackBuffer, attemptedSize, fmt, argPtr);
#endif

	//cout << "NumChars: " << numChars << endl;

    if ((numChars >= 0) && (numChars <= attemptedSize)) 
	{
		// Needed for case of 160-character printf thing
		stackBuffer[numChars] = '\0';

        // Got it on the first try.
		return std::string(stackBuffer);
    }

    // Now use the heap.
    char* heapBuffer = NULL;

    while (((numChars == -1) || (numChars > attemptedSize)) && 
		(attemptedSize < maxSize)) 
	{
        // Try a bigger size
        attemptedSize *= 2;
		heapBuffer = (char*)realloc(heapBuffer, (attemptedSize + 1));
#ifdef _WIN32
		numChars = _vsnprintf(heapBuffer, attemptedSize, fmt, argPtr);
#else
		numChars = vsnprintf(heapBuffer, attemptedSize, fmt, argPtr);
#endif
    }

	heapBuffer[numChars] = 0;

	std::string result = std::string(heapBuffer);

    free(heapBuffer);
    return result;
}

//overloaded StrFormat: should only be used by the xml strings
std::string Sexy::StrFormat(const char* fmt ...) 
{
    va_list argList;
    va_start(argList, fmt);
	std::string result = vformat(fmt, argList);
    va_end(argList);

    return result;
}

std::wstring Sexy::vformat(const wchar_t* fmt, va_list argPtr) 
{
    // We draw the line at a 1MB string.
    const int maxSize = 1000000;

    // If the string is less than 161 characters,
    // allocate it on the stack because this saves
    // the malloc/free time.
    const int bufSize = 161;
	wchar_t stackBuffer[bufSize];

    int attemptedSize = bufSize - 1;

	int numChars = 0;
#ifdef _WIN32
	numChars = _vsnwprintf(stackBuffer, attemptedSize, fmt, argPtr);
#else
	numChars = vswprintf(stackBuffer, attemptedSize, fmt, argPtr);
#endif

	//cout << "NumChars: " << numChars << endl;

    if ((numChars >= 0) && (numChars <= attemptedSize)) 
	{
		// Needed for case of 160-character printf thing
		stackBuffer[numChars] = '\0';

        // Got it on the first try.
		return std::wstring(stackBuffer);
    }

    // Now use the heap.
	wchar_t* heapBuffer = NULL;

    while (((numChars == -1) || (numChars > attemptedSize)) && 
		(attemptedSize < maxSize)) 
	{
        // Try a bigger size
        attemptedSize *= 2;
		heapBuffer = (wchar_t*)realloc(heapBuffer, (attemptedSize + 1));
#ifdef _WIN32
		numChars = _vsnwprintf(heapBuffer, attemptedSize, fmt, argPtr);
#else
		numChars = vswprintf(heapBuffer, attemptedSize, fmt, argPtr);
#endif
    }

	heapBuffer[numChars] = 0;

	std::wstring result = std::wstring(heapBuffer);

    free(heapBuffer);

    return result;
}

//overloaded StrFormat: should only be used by the xml strings
std::wstring Sexy::StrFormat(const wchar_t* fmt ...)
{
    va_list argList;
    va_start(argList, fmt);
	std::wstring result = vformat(fmt, argList);
    va_end(argList);

    return result;
}

std::string Sexy::Evaluate(const std::string& theString, const DefinesMap& theDefinesMap)
{
	std::string anEvaluatedString = theString;

	for (;;)
	{
		size_t aPercentPos = anEvaluatedString.find('%');

		if (aPercentPos == std::string::npos)
			break;
		
		size_t aSecondPercentPos = anEvaluatedString.find('%', aPercentPos + 1);
		if (aSecondPercentPos == std::string::npos)
			break;

		std::string aName = anEvaluatedString.substr(aPercentPos + 1, aSecondPercentPos - aPercentPos - 1);

		std::string aValue;
		DefinesMap::const_iterator anItr = theDefinesMap.find(aName);		
		if (anItr != theDefinesMap.end())
			aValue = anItr->second;
		else
			aValue = "";		

		anEvaluatedString.erase(anEvaluatedString.begin() + aPercentPos, anEvaluatedString.begin() + aSecondPercentPos + 1);
		anEvaluatedString.insert(anEvaluatedString.begin() + aPercentPos, aValue.begin(), aValue.begin() + aValue.length());
	}

	return anEvaluatedString;
}

std::string Sexy::XMLDecodeString(const std::string& theString)
{
	std::string aNewString;

	// unused
	//int aUTF8Len = 0;
	//int aUTF8CurVal = 0;

	for (ulong i = 0; i < theString.length(); i++)
	{
		char c = theString[i];

		if (c == '&')
		{
			int aSemiPos = theString.find(';', i);

			if (aSemiPos != -1)
			{
				std::string anEntName = theString.substr(i+1, aSemiPos-i-1);
				i = aSemiPos;
											
				if (anEntName == "lt")
					c = '<';
				else if (anEntName == "amp")
					c = '&';
				else if (anEntName == "gt")
					c = '>';
				else if (anEntName == "quot")
					c = '"';
				else if (anEntName == "apos")
					c = '\'';
				else if (anEntName == "nbsp")
					c = ' ';
				else if (anEntName == "cr")
					c = '\n';
			}
		}				
		
		aNewString += c;
	}

	return aNewString;
}

std::wstring Sexy::XMLDecodeString(const std::wstring& theString)
{
	std::wstring aNewString;

	// unused
	//int aUTF8Len = 0;
	//int aUTF8CurVal = 0;

	for (ulong i = 0; i < theString.length(); i++)
	{
		wchar_t c = theString[i];

		if (c == L'&')
		{
			int aSemiPos = theString.find(L';', i);

			if (aSemiPos != -1)
			{
				std::wstring anEntName = theString.substr(i+1, aSemiPos-i-1);
				i = aSemiPos;
											
				if (anEntName == L"lt")
					c = L'<';
				else if (anEntName == L"amp")
					c = L'&';
				else if (anEntName == L"gt")
					c = L'>';
				else if (anEntName == L"quot")
					c = L'"';
				else if (anEntName == L"apos")
					c = L'\'';
				else if (anEntName == L"nbsp")
					c = L' ';
				else if (anEntName == L"cr")
					c = L'\n';
			}
		}				
		
		aNewString += c;
	}

	return aNewString;
}

std::string Sexy::XMLEncodeString(const std::string& theString)
{
	std::string aNewString;

	bool hasSpace = false;

	for (ulong i = 0; i < theString.length(); i++)
	{
		char c = theString[i];

		if (c == ' ')
		{
			if (hasSpace)
			{
				aNewString += "&nbsp;";
				continue;
			}
			
			hasSpace = true;
		}
		else
			hasSpace = false;

		/*if ((uchar) c >= 0x80)
		{
			// Convert to UTF
			aNewString += (char) (0xC0 | ((c >> 6) & 0xFF));
			aNewString += (char) (0x80 | (c & 0x3F));
		}
		else*/
		{		
			switch (c)
			{
			case '<':
				aNewString += "&lt;";
				break;
			case '&':		
				aNewString += "&amp;";
				break;
			case '>':
				aNewString += "&gt;";
				break;
			case '"':
				aNewString += "&quot;";
				break;
			case '\'':
				aNewString += "&apos;";
				break;
			case '\n':
				aNewString += "&cr;";
				break;
			default:
				aNewString += c;
				break;
			}
		}
	}

	return aNewString;
}

std::wstring Sexy::XMLEncodeString(const std::wstring& theString)
{
	std::wstring aNewString;

	bool hasSpace = false;

	for (ulong i = 0; i < theString.length(); i++)
	{
		wchar_t c = theString[i];

		if (c == ' ')
		{
			if (hasSpace)
			{
				aNewString += L"&nbsp;";
				continue;
			}
			
			hasSpace = true;
		}
		else
			hasSpace = false;

		/*if ((uchar) c >= 0x80)
		{
			// Convert to UTF
			aNewString += (char) (0xC0 | ((c >> 6) & 0xFF));
			aNewString += (char) (0x80 | (c & 0x3F));
		}
		else*/
		{		
			switch (c)
			{
			case L'<':
				aNewString += L"&lt;";
				break;
			case L'&':		
				aNewString += L"&amp;";
				break;
			case L'>':
				aNewString += L"&gt;";
				break;
			case L'"':
				aNewString += L"&quot;";
				break;
			case L'\'':
				aNewString += L"&apos;";
				break;
			case L'\n':
				aNewString += L"&cr;";
				break;
			default:
				aNewString += c;
				break;
			}
		}
	}

	return aNewString;
}

std::string Sexy::Upper(const std::string& _data)
{
	std::string s = _data;
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	return s;
}

std::wstring Sexy::Upper(const std::wstring& _data)
{
	std::wstring s = _data;
	std::transform(s.begin(), s.end(), s.begin(), towupper);
	return s;
}

std::string Sexy::Lower(const std::string& _data)
{
	std::string s = _data;
	std::transform(s.begin(), s.end(), s.begin(), tolower);
	return s;
}

std::wstring Sexy::Lower(const std::wstring& _data)
{
	std::wstring s = _data;
	std::transform(s.begin(), s.end(), s.begin(), towlower);
	return s;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int Sexy::StrFindNoCase(const char *theStr, const char *theFind)
{
	int p1,p2;
	int cp = 0;
	const int len1 = (int)strlen(theStr);
	const int len2 = (int)strlen(theFind);
	while(cp < len1)
	{
		p1 = cp;
		p2 = 0;
		while(p1<len1 && p2<len2)
		{
			if(tolower(theStr[p1])!=tolower(theFind[p2]))
				break;

			p1++; p2++;
		}
		if(p2==len2)
			return p1-len2;

		cp++;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool Sexy::StrPrefixNoCase(const char *theStr, const char *thePrefix, int maxLength)
{
	int i;
	char c1 = 0, c2 = 0;
	for (i=0; i<maxLength; i++)
	{
		c1 = tolower(*theStr++);
		c2 = tolower(*thePrefix++);

		if (c1==0 || c2==0)
			break;

		if (c1!=c2)
			return false;
	}

	return c2==0 || i==maxLength;
}

std::wstring Sexy::UTF8StringToWString(const std::string /*theString*/)
{
	unreachable(); // FIXME
	/*
	int size = MultiByteToWideChar(CP_UTF8, 0, theString.c_str(), theString.length() + 1, nullptr, 0);
	wchar_t* buffer = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, theString.c_str(), theString.length() + 1, buffer, size);
	std::wstring result = buffer;
	delete[] buffer;
	return result;*/
}

void Sexy::SMemR(void*& _Src, void* _Dst, size_t _Size)
{
	memcpy(_Dst, _Src, _Size);
	_Src = (void*)((size_t)_Src + _Size);
}

void Sexy::SMemRStr(void*& _Src, std::string& theString)
{
	size_t aStrLen;
	SMemR(_Src, &aStrLen, sizeof(aStrLen));
	theString.resize(aStrLen);
	SMemR(_Src, (void*)theString.c_str(), aStrLen);
}

void Sexy::SMemW(void*& _Dst, const void* _Src, size_t _Size)
{
	memcpy(_Dst, _Src, _Size);
	_Dst = (void*)((uintptr_t)_Dst + _Size);
}

void Sexy::SMemWStr(void*& _Dst, const std::string& theString)
{
	size_t aStrLen = theString.size();
	SMemW(_Dst, &aStrLen, sizeof(aStrLen));
	SMemW(_Dst, theString.c_str(), aStrLen);
}
