#include "ModVal.h"
#include "Common.h"
#include <fstream>

struct ModStorage
{		
	bool					mChanged;
	int						mInt; 
	double					mDouble;	
	std::string				mString;
};

struct ModPointer
{
	const char *mStrPtr;
	int mLineNum;

	ModPointer() : mStrPtr(NULL), mLineNum(0) {}
	ModPointer(const char *theStrPtr, int theLineNum) : mStrPtr(theStrPtr), mLineNum(theLineNum) {}
};

typedef std::map<int,ModPointer> ModStorageMap; // stores counters

struct FileMod
{
	bool mHasMods;
	ModStorageMap mMap;

	FileMod(bool hasMods = false) { mHasMods = hasMods; }
};

typedef std::map<std::string, int> StringToIntMap;
typedef std::map<std::string, FileMod> FileModMap;

static StringToIntMap gStringToIntMap;
time_t gLastFileTime = 0;
const char *gSampleString = NULL; // for finding the others

static FileModMap& GetFileModMap()
{
	static FileModMap aMap;
	return aMap;
}

static const char* FindFileInStringTable(const std::string &theSearch, const char *theMem, DWORD theLength, const char *theStartPos)
{
	const char *aFind = NULL;
	try
	{
		aFind = std::search(theStartPos,theMem+theLength,theSearch.c_str(),theSearch.c_str()+theSearch.length());
		if (aFind>=theMem+theLength)
			return NULL;
		else
			return aFind;
	}
	catch(...)
	{
		return NULL;
	}

	return NULL;
}

static bool ParseModValString(std::string &theStr, int *theCounter = NULL, int *theLineNum = NULL)
{
	int aPos = theStr.length()-1;
	bool foundComma = false;
	while (aPos>0)
	{
		if (!foundComma && theStr[aPos]==',')
		{
			aPos--;
			foundComma = true;
		}
		else if (isdigit(theStr[aPos]))
			aPos--;
		else
			break;
	}

	if (aPos==theStr.length()-1 || aPos==0) // no number,number to erase... or empty file
		return false;

	aPos++;
	int aCounterVal = -1, aLineNum = -1;
	if (sscanf(theStr.c_str()+aPos,"%d,%d",&aCounterVal,&aLineNum)!=2) // couldn't parse out the numbers
		return false;

	theStr.resize(aPos);
	if (theCounter) *theCounter = aCounterVal;
	if (theLineNum) *theLineNum = aLineNum;
	return true;
}

static bool FindModValsInMemoryHelper(const char *theMem, DWORD theLength)
{
	std::string aSearchStr = "SEXYMODVAL";

	FileModMap &aMap = GetFileModMap();

	bool foundOne = false;
	const char *aPtr = theMem;
	while (true)
	{
		aPtr = FindFileInStringTable(aSearchStr,theMem,theLength,aPtr);
		if (aPtr==NULL)
			break;

		int aCounter, aLineNum;
		std::string aFileName = aPtr+10; // skip SEXYMODVAL
		if (ParseModValString(aFileName,&aCounter,&aLineNum))
		{
			/* What the fuck is this?
			if (aLineNum==4105)
				_asm nop;
			*/

			FileMod &aFileMod = aMap[aFileName];
			aFileMod.mMap[aCounter] = ModPointer(aPtr-5,aLineNum);
			foundOne = true;
		}
		aPtr++;
	}

	return foundOne;
}

static void FindModValsInMemory()
{
	MEMORY_BASIC_INFORMATION mbi; 
	PVOID      pvAddress = 0; 

	const char *aMem = NULL;
	DWORD aMemLength = 0;

	int aFound = 0; (void)aFound;
	int aTotal = 0; (void)aTotal;
	memset(&mbi, 0, sizeof(MEMORY_BASIC_INFORMATION)); 
	for (; VirtualQuery(pvAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION)) == sizeof(MEMORY_BASIC_INFORMATION); pvAddress = ((BYTE*)mbi.BaseAddress) + mbi.RegionSize) 
	{ 	
		const char *anAddress = (const char*)mbi.BaseAddress;
		if (mbi.State==MEM_COMMIT && mbi.Type==MEM_IMAGE)
		{
			aTotal++;
			if (aMem!=NULL && aMem+aMemLength==anAddress) // compact these two
			{
				aMemLength += mbi.RegionSize;
				continue;
			}
			
			if (aMem!=NULL) // do find in old section
			{
				if (FindModValsInMemoryHelper(aMem,aMemLength))
				{
					aFound++;
					return;
				}

				aMem = NULL;
			}

			aMem = anAddress;
			aMemLength = mbi.RegionSize;
		}
	} 

	if (aMem!=NULL)
	{
		if (FindModValsInMemoryHelper(aMem,aMemLength))
			aFound++;
	}
}

static ModStorage* CreateFileModsHelper(const char* theFileName)
{
	ModStorage *aModStorage = new ModStorage;
	aModStorage->mChanged = false;

	// Change this thinggie
	DWORD anOldProtect;
	VirtualProtect((LPVOID) theFileName, 5, PAGE_READWRITE, &anOldProtect);
	*((char*) theFileName) = 0;
	*((ModStorage**) (theFileName+1)) = aModStorage;
	VirtualProtect((LPVOID) theFileName, 5, anOldProtect, &anOldProtect);
	
	return aModStorage;	
}


static ModStorage* CreateFileMods(const char* theFileName)
{	
	if (gSampleString==NULL)
		gSampleString = theFileName;

	std::string aFileName = theFileName+15; // skip SEXY_SEXYMODVAL
	ParseModValString(aFileName);

	FileModMap &aMap = GetFileModMap();
	aMap[aFileName].mHasMods = true; 

	return CreateFileModsHelper(theFileName);
}

int Sexy::ModVal(int theAreaNum, const char* theFileName, int theInt)
{	
	if (*theFileName != 0)
		CreateFileMods(theFileName);	

	ModStorage* aModStorage = *(ModStorage**)(theFileName+1);
	if (aModStorage->mChanged)
		return aModStorage->mInt;
	else
		return theInt;
}

double Sexy::ModVal(int theAreaNum, const char* theFileName, double theDouble)
{
	if (*theFileName != 0)
		CreateFileMods(theFileName);	
			
	ModStorage* aModStorage = *(ModStorage**)(theFileName+1);
	if (aModStorage->mChanged)
		return aModStorage->mDouble;
	else
		return theDouble;
}

float Sexy::ModVal(int theAreaNum, const char* theFileName, float theFloat)
{
	return (float) ModVal(theAreaNum, theFileName, (double) theFloat);
}

const char*	Sexy::ModVal(int theAreaNum, const char* theFileName, const char *theStr)
{
	if (*theFileName != 0)
		CreateFileMods(theFileName);	

	ModStorage* aModStorage = *(ModStorage**)(theFileName+1);
	if (aModStorage->mChanged)
		return aModStorage->mString.c_str();
	else
		return theStr;
}


void Sexy::AddModValEnum(const std::string &theEnumName, int theVal)
{
	gStringToIntMap[theEnumName] = theVal;
}

static bool ModStringToInteger(const char* theString, int* theIntVal)
{
	*theIntVal = 0;

	int theRadix = 10;
	bool isNeg = false;

	unsigned i = 0;

	if (isalpha((unsigned char)theString[i]) || theString[i]=='_') // enum
	{
		
		std::string aStr;
		while (isalnum((unsigned char)theString[i]) || theString[i]=='_')
		{
			aStr += theString[i];
			i++;
		}

		StringToIntMap::iterator anItr = gStringToIntMap.find(aStr);
		if (anItr!=gStringToIntMap.end())
		{
			*theIntVal = anItr->second;
			return true;
		}

		i = 0;
	}
		
	if (theString[i] == '-')
	{
		isNeg = true;
		i++;
	}

	for (;;)
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
		else if (aChar == ')')
		{
			if (isNeg)
				*theIntVal = -*theIntVal;
			return true;
		}
		else
		{
			*theIntVal = 0;
			return false;
		}

		i++;
	}		
}

static bool ModStringToDouble(const char* theString, double* theDoubleVal)
{
	*theDoubleVal = 0.0;

	bool isNeg = false;

	unsigned i = 0;
	if (theString[i] == '-')
	{
		isNeg = true;
		i++;
	}

	for (;;)
	{
		char aChar = theString[i];

		if ((aChar >= '0') && (aChar <= '9'))
			*theDoubleVal = (*theDoubleVal * 10) + (aChar - '0');
		else if (aChar == '.')
		{
			i++;
			break;
		}		
		else if ((aChar == ')') || ((aChar == 'f') && (theString[i+1] == ')'))) // At end
		{
			if (isNeg)
				*theDoubleVal = -*theDoubleVal;
			return true;
		}
		else
		{
			*theDoubleVal = 0.0;
			return false;
		}

		i++;
	}

	double aMult = 0.1;
	for (;;)
	{
		char aChar = theString[i];

		if ((aChar >= '0') && (aChar <= '9'))
		{
			*theDoubleVal += (aChar - '0') * aMult;	
			aMult /= 10.0;
		}
		else if ((aChar == ')') || ((aChar == 'f') && (theString[i+1] == ')'))) // At end
		{
			if (isNeg)
				*theDoubleVal = -*theDoubleVal;
			return true;
		}
		else
		{
			*theDoubleVal = 0.0;
			return false;
		}

		i++;
	}
}

static bool ModStringToString(const char* theString, std::string &theStrVal)
{
	if (theString[0]!='"')
		return false;

	std::string &aStr = theStrVal;
	aStr.erase();

	int i=1;
	while (true)
	{
		if (theString[i]=='\\')
		{
			i++;
			switch (theString[i++])
			{
			case 'n': aStr += '\n'; break;
			case 't': aStr += '\t'; break;
			case '\\': aStr += '\\'; break;
			case '"': aStr += '\"'; break;
			default: return false;
			}
		}
		else if (theString[i]=='"')
		{
			i++;
			while (isspace((unsigned char)theString[i]))
				i++;

			if (theString[i]!='"') // continued string
				return true;
			else
				break;
		}
		else if (theString[i]=='\0')
			return false;
		else
			aStr += theString[i++];
	}

	return true;
}

bool Sexy::ReparseModValues()
{
	if (gLastFileTime == 0)
	{
		char anEXEName[256];
		GetModuleFileNameA(NULL, anEXEName, 256);
		gLastFileTime = GetFileDate(anEXEName);
		
		FindModValsInMemory();
	}

	bool hasNewFiles = false;
	std::string aFileList;

	// Process each file one at a time
	FileModMap &aMap = GetFileModMap();
	FileModMap::iterator aFileModItr;
	for (aFileModItr = aMap.begin(); aFileModItr != aMap.end(); ++aFileModItr)
	{
		FileMod &aFileMod = aFileModItr->second;
		if (!aFileMod.mHasMods)
			continue;

		ModStorageMap &aModMap = aFileMod.mMap;
		std::string aFileName = aFileModItr->first;

		time_t aThisTime = GetFileDate(aFileName);
		if (aThisTime > gLastFileTime)
		{
			gLastFileTime = aThisTime;
			hasNewFiles = true;
		}

		if (aFileList.length() > 0)
			aFileList += "\r\n  ";
		aFileList += aFileName;

		int aLineNum = 1;
		int aModNum = 0;(void)aModNum;
		ModStorageMap::iterator aModMapItr = aModMap.begin();

		std::fstream aStream(aFileName.c_str(), std::ios::in);

		if (aStream.is_open())
		{
			while (!aStream.eof())
			{
				char aString[8192];
				aStream.getline(aString, 8192);

				int aCharIdx = 0;
				int aChar = 0;
				int aLastChar = 0;
				while (aString[aCharIdx] != 0)
				{
					aLastChar = aChar;
					aChar = aString[aCharIdx];
					
					if (aChar == '"')  // Skip strings
					{
						while (true)
						{
							aLastChar = aChar;
							aChar = aString[++aCharIdx];

							if (aChar=='\\' && aLastChar=='\\') // so we don't interpret \\" as an escaped quote
								aChar = 0;
							else if (aChar=='"' && aLastChar!='\\')
								break;
							else if (aChar==0)
							{
								if (aLastChar=='\\') // continuation
								{
									aCharIdx = -1;
									aChar = 0;
									aLastChar = 0;
									aLineNum++;

									aStream.getline(aString, 8192);
									if (aString[0]!=0 || !aStream.eof()) // got valid new line
										continue;
								}

								char aStr[512];
								sprintf(aStr, "ERROR in %s on line %d: Error parsing quotes", aFileName.c_str(), aLineNum);
								MessageBoxA(NULL, aStr, "MODVAL ERROR", MB_OK | MB_ICONERROR);
								return false;
							}
						}
					}
					else if (aChar=='/') // Skip C++ comments
					{
						if (aLastChar=='/') 
						{

							while (true)
							{
								aLastChar = aChar;
								aChar = aString[++aCharIdx];
								if (aChar==0) // line continuation
								{
									if (aLastChar=='\\') // continuation
									{
										aCharIdx = -1;
										aChar = 0;
										aLastChar = 0;
										aLineNum++;

										aStream.getline(aString, 8192);
										if (aString[0]!=0 || !aStream.eof()) // got valid new line
											continue;
									}
									else
									{
										aCharIdx--;
										break;
									}

									char aStr[512];
									sprintf(aStr, "ERROR in %s on line %d: Error parsing c++ comment", aFileName.c_str(), aLineNum);
									MessageBoxA(NULL, aStr, "MODVAL ERROR", MB_OK | MB_ICONERROR);
									return false;
								}
							}
						}
					}
					else if (aChar=='*') // skip C comments
					{
						if (aLastChar=='/') 
						{
							while (true)
							{
								aLastChar = aChar;
								aChar = aString[++aCharIdx];
								if (aChar=='/' && aLastChar=='*')
									break;
								else if (aChar==0) // line continuation
								{
									aCharIdx = -1;
									aChar = 0;
									aLastChar = 0;
									aLineNum++;

									aStream.getline(aString, 8192);
									if (aString[0]!=0 || !aStream.eof()) // got valid new line
										continue;

									char aStr[512];
									sprintf(aStr, "ERROR in %s on line %d: Error parsing c comment", aFileName.c_str(), aLineNum);
									MessageBoxA(NULL, aStr, "MODVAL ERROR", MB_OK | MB_ICONERROR);
									return false;
								}
							}
						}
					}
					else if (aChar == '(')
					{
						int theAreaNum = -1;
						if ((aCharIdx >= 2) && (aString[aCharIdx-1] == 'M') &&
							(!isalpha((unsigned char) aString[aCharIdx-2])))
						{
							theAreaNum = 0;							
						}
						else if ((aCharIdx >= 3) && 
							(aString[aCharIdx-1] >= '1') && (aString[aCharIdx-1] <= '9') &&
							(aString[aCharIdx-2] == 'M') &&
							(!isalpha((unsigned char) aString[aCharIdx-3])))
						{
							theAreaNum = aString[aCharIdx-1] - '0';
						}

						if (theAreaNum != -1)
						{
							while (aModMapItr!=aModMap.end() && aModMapItr->second.mLineNum<aLineNum)
								++aModMapItr;

							if (aModMapItr!=aModMap.end() && aModMapItr->second.mLineNum==aLineNum)
							{
								const char *aPtr = aModMapItr->second.mStrPtr;
								aModMapItr++;

								int anIntVal = 0;
								double aDoubleVal = 0.0;
								std::string aStrVal;

								// Try to parse out a number
								if ((ModStringToString(aString + aCharIdx + 1, aStrVal)) ||
									(ModStringToInteger(aString + aCharIdx + 1, &anIntVal)) ||
									(ModStringToDouble(aString + aCharIdx + 1, &aDoubleVal)))
								{						
									// We found a mod value!

									if (*aPtr!=0) // have stored something here
										CreateFileMods(aPtr);

									ModStorage* aModStorage = *(ModStorage**)(aPtr+1);
									aModStorage->mInt = anIntVal;
									aModStorage->mDouble = aDoubleVal;
									aModStorage->mString = aStrVal;
									aModStorage->mChanged = true;
								}
								else
								{
									char aStr[512];
									sprintf(aStr, "ERROR in %s on line %d.  Parsing Error.", aFileName.c_str(), aLineNum);
									MessageBoxA(NULL, aStr, "MODVAL ERROR", MB_OK | MB_ICONERROR);

									return false;
								}
							}
							else
							{
								// Functions can be optimized out
							}
						}
					}

					aCharIdx++;
				}

				aLineNum++;
			}
		}
		else
		{
			MessageBoxA(NULL, (std::string("ERROR: Unable to open ") + aFileName + " for reparsing.").c_str(), "MODVAL ERROR!", MB_OK | MB_ICONERROR);
			return false;
		}		
	}

	if (!hasNewFiles)
	{
		if (aFileList.length() == 0)
			aFileList = "none";
		MessageBoxA(NULL, (std::string("WARNING: No file changes detected.  Files parsed: \r\n  ") + aFileList).c_str(), "MODVAL WARNING!", MB_OK | MB_ICONWARNING);
		return false;
	}

	return true;
}
