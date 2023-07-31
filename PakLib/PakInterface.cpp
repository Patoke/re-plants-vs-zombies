#include "PakInterface.h"
#include <windows.h>
#include <direct.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

enum
{
	FILEFLAGS_END = 0x80
};

PakInterface* gPakInterface = new PakInterface();

static std::string StringToUpper(const std::string& theString)
{
	std::string aString;

	for (unsigned i = 0; i < theString.length(); i++)
		aString += toupper(theString[i]);

	return aString;
}

PakInterface::PakInterface()
{
	if (GetPakPtr() == NULL)
		*gPakInterfaceP = this;
}

PakInterface::~PakInterface()
{
}

bool PakInterface::AddPakFile(const std::string& theFileName)
{
	HANDLE aFileHandle = CreateFile(theFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (aFileHandle == INVALID_HANDLE_VALUE)
		return false;

	int aFileSize = GetFileSize(aFileHandle, 0);

	HANDLE aFileMapping = CreateFileMapping(aFileHandle, NULL, PAGE_READONLY, 0, aFileSize, NULL);
	if (aFileMapping == NULL)
	{
		CloseHandle(aFileHandle);
		return false;
	}

	void* aPtr = MapViewOfFile(aFileMapping, FILE_MAP_READ, 0, 0, aFileSize);
	if (aPtr == NULL)
	{
		CloseHandle(aFileMapping);
		CloseHandle(aFileHandle);
		return false;
	}

	mPakCollectionList.push_back(PakCollection());
	PakCollection* aPakCollection = &mPakCollectionList.back();

	aPakCollection->mFileHandle = aFileHandle;
	aPakCollection->mMappingHandle = aFileMapping;
	aPakCollection->mDataPtr = aPtr;
	
	PakRecordMap::iterator aRecordItr = mPakRecordMap.insert(PakRecordMap::value_type(StringToUpper(theFileName), PakRecord())).first;
	PakRecord* aPakRecord = &(aRecordItr->second);
	aPakRecord->mCollection = aPakCollection;
	aPakRecord->mFileName = theFileName;
	aPakRecord->mStartPos = 0;
	aPakRecord->mSize = aFileSize;
	
	PFILE* aFP = FOpen(theFileName.c_str(), "rb");
	if (aFP == NULL)
		return false;

	ulong aMagic = 0;
	FRead(&aMagic, sizeof(ulong), 1, aFP);
	if (aMagic != 0xBAC04AC0)
	{
		FClose(aFP);
		return false;
	}

	ulong aVersion = 0;
	FRead(&aVersion, sizeof(ulong), 1, aFP);
	if (aVersion > 0)
	{
		FClose(aFP);
		return false;
	}

	int aPos = 0;

	for (;;)
	{
		uchar aFlags = 0;
		int aCount = FRead(&aFlags, 1, 1, aFP);
		if ((aFlags & FILEFLAGS_END) || (aCount == 0))
			break;

		uchar aNameWidth = 0;
		char aName[256];
		FRead(&aNameWidth, 1, 1, aFP);
		FRead(aName, 1, aNameWidth, aFP);
		aName[aNameWidth] = 0;

		int aSrcSize = 0;
		FRead(&aSrcSize, sizeof(int), 1, aFP);
		FILETIME aFileTime;
		FRead(&aFileTime, sizeof(FILETIME), 1, aFP);

		PakRecordMap::iterator aRecordItr = mPakRecordMap.insert(PakRecordMap::value_type(StringToUpper(aName), PakRecord())).first;
		PakRecord* aPakRecord = &(aRecordItr->second);
		aPakRecord->mCollection = aPakCollection;
		aPakRecord->mFileName = aName;
		aPakRecord->mStartPos = aPos;
		aPakRecord->mSize = aSrcSize;
		aPakRecord->mFileTime = aFileTime;

		aPos += aSrcSize;
	}

	int anOffset = FTell(aFP);

	// Now fix file starts
	aRecordItr = mPakRecordMap.begin();
	while (aRecordItr != mPakRecordMap.end())
	{
		PakRecord* aPakRecord = &(aRecordItr->second);
		if (aPakRecord->mCollection == aPakCollection)
			aPakRecord->mStartPos += anOffset;
		++aRecordItr;
	}

	FClose(aFP);

	return true;
}

//0x5D84D0
static void FixFileName(const char* theFileName, char* theUpperName)
{
	// ���·���Ƿ�Ϊ���̷���ʼ�ľ���·��
	if ((theFileName[0] != 0) && (theFileName[1] == ':'))
	{
		char aDir[256];
		getcwd(aDir, 256);  // ȡ�õ�ǰ����·��
		int aLen = strlen(aDir);
		aDir[aLen++] = '\\';
		aDir[aLen] = 0;

		// �ж� theFileName �ļ��Ƿ�λ�ڵ�ǰĿ¼��
		if (strnicmp(aDir, theFileName, aLen) == 0)
			theFileName += aLen;  // ���ǣ����������̷�����ǰĿ¼�Ĳ��֣�ת��Ϊ���·��
	}

	bool lastSlash = false;
	const char* aSrc = theFileName;
	char* aDest = theUpperName;

	for (;;)
	{
		char c = *(aSrc++);

		if ((c == '\\') || (c == '/'))
		{
			// ͳһתΪ��б�ܣ��Ҷ��б�ܵ������ֻ����һ��
			if (!lastSlash)
				*(aDest++) = '\\';
			lastSlash = true;
		}
		else if ((c == '.') && (lastSlash) && (*aSrc == '.'))
		{
			// We have a '/..' on our hands
			aDest--;
			while ((aDest > theUpperName + 1) && (*(aDest-1) != '\\'))  // ���˵���һ��Ŀ¼
				--aDest;
			aSrc++;
			// �˴������硰a\b\..\c����·����Ϊ��a\c��
		}
		else
		{
			*(aDest++) = toupper((uchar) c);
			if (c == 0)
				break;
			lastSlash = false;				
		}
	}
}

//0x5D85C0
PFILE* PakInterface::FOpen(const char* theFileName, const char* anAccess)
{
	if ((stricmp(anAccess, "r") == 0) || (stricmp(anAccess, "rb") == 0) || (stricmp(anAccess, "rt") == 0))
	{
		char anUpperName[256];
		FixFileName(theFileName, anUpperName);
		
		PakRecordMap::iterator anItr = mPakRecordMap.find(anUpperName);
		if (anItr != mPakRecordMap.end())
		{			
			PFILE* aPFP = new PFILE;
			aPFP->mRecord = &anItr->second;
			aPFP->mPos = 0;
			aPFP->mFP = NULL;
			return aPFP;
		}
	}

	FILE* aFP = fopen(theFileName, anAccess);
	if (aFP == NULL)
		return NULL;
	PFILE* aPFP = new PFILE;
	aPFP->mRecord = NULL;
	aPFP->mPos = 0;
	aPFP->mFP = aFP;
	return aPFP;
}

//0x5D8780
int PakInterface::FClose(PFILE* theFile)
{
	if (theFile->mRecord == NULL)
		fclose(theFile->mFP);
	delete theFile;
	return 0;
}

//0x5D87B0
int PakInterface::FSeek(PFILE* theFile, long theOffset, int theOrigin)
{
	if (theFile->mRecord != NULL)
	{
		if (theOrigin == SEEK_SET)
			theFile->mPos = theOffset;
		else if (theOrigin == SEEK_END)
			theFile->mPos = theFile->mRecord->mSize - theOffset;
		else if (theOrigin == SEEK_CUR)
			theFile->mPos += theOffset;

		// ��ǰָ��λ�ò��ܳ��������ļ��Ĵ�С���Ҳ���С�� 0
		theFile->mPos = max(min(theFile->mPos, theFile->mRecord->mSize), 0);
		return 0;
	}
	else
		return fseek(theFile->mFP, theOffset, theOrigin);
}

//0x5D8830
int PakInterface::FTell(PFILE* theFile)
{
	if (theFile->mRecord != NULL)
		return theFile->mPos;
	else
		return ftell(theFile->mFP);	
}

//0x5D8850
size_t PakInterface::FRead(void* thePtr, int theElemSize, int theCount, PFILE* theFile)
{
	if (theFile->mRecord != NULL)
	{
		// ʵ�ʶ�ȡ���ֽ������ܳ�����ǰ��Դ�ļ�ʣ��ɶ�ȡ���ֽ���
		int aSizeBytes = min(theElemSize*theCount, theFile->mRecord->mSize - theFile->mPos);

		// ȡ�������� pak �п�ʼ��ȡ��λ�õ�ָ��
		uchar* src = (uchar*) theFile->mRecord->mCollection->mDataPtr + theFile->mRecord->mStartPos + theFile->mPos;
		uchar* dest = (uchar*) thePtr;
		for (int i = 0; i < aSizeBytes; i++)
			*(dest++) = (*src++) ^ 0xF7; // 'Decrypt'
		theFile->mPos += aSizeBytes;  // ��ȡ��ɺ��ƶ���ǰ��ȡλ�õ�ָ��
		return aSizeBytes / theElemSize;  // ����ʵ�ʶ�ȡ������
	}
	
	return fread(thePtr, theElemSize, theCount, theFile->mFP);	
}

int PakInterface::FGetC(PFILE* theFile)
{
	if (theFile->mRecord != NULL)
	{
		for (;;)
		{
			if (theFile->mPos >= theFile->mRecord->mSize)
				return EOF;		
			char aChar = *((char*) theFile->mRecord->mCollection->mDataPtr + theFile->mRecord->mStartPos + theFile->mPos++) ^ 0xF7;
			if (aChar != '\r')
				return (uchar) aChar;
		}
	}

	return fgetc(theFile->mFP);
}

int PakInterface::UnGetC(int theChar, PFILE* theFile)
{
	if (theFile->mRecord != NULL)
	{
		// This won't work if we're not pushing the same chars back in the stream
		theFile->mPos = max(theFile->mPos - 1, 0);
		return theChar;
	}

	return ungetc(theChar, theFile->mFP);
}

char* PakInterface::FGetS(char* thePtr, int theSize, PFILE* theFile)
{
	if (theFile->mRecord != NULL)
	{
		int anIdx = 0;
		while (anIdx < theSize)
		{
			if (theFile->mPos >= theFile->mRecord->mSize)
			{
				if (anIdx == 0)
					return NULL;
				break;
			}
			char aChar = *((char*) theFile->mRecord->mCollection->mDataPtr + theFile->mRecord->mStartPos + theFile->mPos++) ^ 0xF7;
			if (aChar != '\r')
				thePtr[anIdx++] = aChar;
			if (aChar == '\n')
				break;
		}
		thePtr[anIdx] = 0;
		return thePtr;
	}

	return fgets(thePtr, theSize, theFile->mFP);
}

int PakInterface::FEof(PFILE* theFile)
{
	if (theFile->mRecord != NULL)
		return theFile->mPos >= theFile->mRecord->mSize;
	else
		return feof(theFile->mFP);
}

bool PakInterface::PFindNext(PFindData* theFindData, LPWIN32_FIND_DATA lpFindFileData)
{
	PakRecordMap::iterator anItr;
	if (theFindData->mLastFind.size() == 0)
		anItr = mPakRecordMap.begin();
	else
	{
		anItr = mPakRecordMap.find(theFindData->mLastFind);
		if (anItr != mPakRecordMap.end())
			anItr++;
	}

	while (anItr != mPakRecordMap.end())
	{
		const char* aFileName = anItr->first.c_str();
		PakRecord* aPakRecord = &anItr->second;

		int aStarPos = (int) theFindData->mFindCriteria.find('*');
		if (aStarPos != -1)
		{
			if (strncmp(theFindData->mFindCriteria.c_str(), aFileName, aStarPos) == 0)
			{				
				// First part matches
				const char* anEndData = theFindData->mFindCriteria.c_str() + aStarPos + 1;
				if ((*anEndData == 0) || (strcmp(anEndData, ".*") == 0) ||								
					(strcmp(theFindData->mFindCriteria.c_str() + aStarPos + 1, 
					aFileName + strlen(aFileName) - (theFindData->mFindCriteria.length() - aStarPos) + 1) == 0))
				{
					// Matches before and after star
					memset(lpFindFileData, 0, sizeof(lpFindFileData));
					
					int aLastSlashPos = (int) anItr->second.mFileName.rfind('\\');
					if (aLastSlashPos == -1)
						strcpy(lpFindFileData->cFileName, anItr->second.mFileName.c_str());
					else
						strcpy(lpFindFileData->cFileName, anItr->second.mFileName.c_str() + aLastSlashPos + 1);

					const char* aEndStr = aFileName + strlen(aFileName) - (theFindData->mFindCriteria.length() - aStarPos) + 1;
					if (strchr(aEndStr, '\\') != NULL)
						lpFindFileData->dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;

					lpFindFileData->nFileSizeLow = aPakRecord->mSize;
					lpFindFileData->ftCreationTime = aPakRecord->mFileTime;
					lpFindFileData->ftLastWriteTime = aPakRecord->mFileTime;
					lpFindFileData->ftLastAccessTime = aPakRecord->mFileTime;
					theFindData->mLastFind = aFileName;

					return true;
				}
			}
		}

		++anItr;
	}

	return false;
}

HANDLE PakInterface::FindFirstFile(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
	PFindData* aFindData = new PFindData;

	char anUpperName[256];
	FixFileName(lpFileName, anUpperName);
	aFindData->mFindCriteria = anUpperName;
	aFindData->mWHandle = INVALID_HANDLE_VALUE;

	if (PFindNext(aFindData, lpFindFileData))
		return (HANDLE) aFindData;

	aFindData->mWHandle = ::FindFirstFile(aFindData->mFindCriteria.c_str(), lpFindFileData);
	if (aFindData->mWHandle != INVALID_HANDLE_VALUE)
		return (HANDLE) aFindData;

	delete aFindData;
	return INVALID_HANDLE_VALUE;
}

BOOL PakInterface::FindNextFile(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData)
{
	PFindData* aFindData = (PFindData*) hFindFile;

	if (aFindData->mWHandle == INVALID_HANDLE_VALUE)
	{
		if (PFindNext(aFindData, lpFindFileData))
			return TRUE;

		aFindData->mWHandle = ::FindFirstFile(aFindData->mFindCriteria.c_str(), lpFindFileData);
		return (aFindData->mWHandle != INVALID_HANDLE_VALUE);			
	}
	
	return ::FindNextFile(aFindData->mWHandle, lpFindFileData);
}

BOOL PakInterface::FindClose(HANDLE hFindFile)
{
	PFindData* aFindData = (PFindData*) hFindFile;

	if (aFindData->mWHandle != INVALID_HANDLE_VALUE)
		::FindClose(aFindData->mWHandle);

	delete aFindData;
	return TRUE;
}

