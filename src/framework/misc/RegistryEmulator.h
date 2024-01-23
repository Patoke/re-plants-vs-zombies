#ifndef REGISTRY_EMULATOR_H
#define REGISTRY_EMULATOR_H

#include "framework/Common.h"

/***********************************************************************************************/
/*  Copied from github.com/Alexpux/mingw-w64/blob/master/mingw-w64-tools/widl/include/winnt.h  */
/***********************************************************************************************/

#define REG_NONE						0	/* no type */
#define REG_SZ							1	/* string type (ASCII) */
#define REG_EXPAND_SZ					2	/* string, includes %ENVVAR% (expanded by caller) (ASCII) */
#define REG_BINARY						3	/* binary format, callerspecific */
/* YES, REG_DWORD == REG_DWORD_LITTLE_ENDIAN */
#define REG_DWORD						4	/* DWORD in little endian format */
#define REG_DWORD_LITTLE_ENDIAN			4	/* DWORD in little endian format */
#define REG_DWORD_BIG_ENDIAN			5	/* DWORD in big endian format  */
#define REG_LINK						6	/* symbolic link (UNICODE) */
#define REG_MULTI_SZ					7	/* multiple strings, delimited by \0, terminated by \0\0 (ASCII) */
#define REG_RESOURCE_LIST				8	/* resource list? huh? */
#define REG_FULL_RESOURCE_DESCRIPTOR	9	/* full resource descriptor? huh? */
#define REG_RESOURCE_REQUIREMENTS_LIST	10
#define REG_QWORD						11	/* QWORD in little endian format */
#define REG_QWORD_LITTLE_ENDIAN			11	/* QWORD in little endian format */

/***********************************************************************************************/

class RegistryEmulator {
public:
	RegistryEmulator(const std::string);
	RegistryEmulator() : RegistryEmulator("registry.dat") {}
	~RegistryEmulator();
	bool Read(const std::string& theValueName, uint32_t &theType, std::vector<uint8_t> &theValue, uint32_t &theLength);
	void Write(const std::string& theValueName, uint32_t theType, const uchar* theValue, uint32_t theLength);
	bool Erase(const std::string& theValueName);
	size_t Flush();
private:
	const std::string mREG_FILENAME;
	std::vector<uint8_t> mRegVec;

	size_t FindKey(const std::string& theValueName);
	size_t GetNextKey(size_t theKeyIdx);
	void   DeleteKey(size_t theKeyIdx);
};

void RegistryEmulatorTest();

#endif // REGISTRY_EMULATOR_H
