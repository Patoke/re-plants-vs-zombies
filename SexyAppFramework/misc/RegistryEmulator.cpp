#include "RegistryEmulator.h"

#include <filesystem>

RegistryEmulator::RegistryEmulator(const std::string theFileName) : mREG_FILENAME{theFileName} {
    FILE *aRegFile;
    if (!std::filesystem::exists(mREG_FILENAME)) return; // We're done here

    aRegFile = fopen(mREG_FILENAME.c_str(), "rb");
    if (!aRegFile) throw std::runtime_error("Cannot open registry file!");

    fseek(aRegFile, 0, SEEK_END);
    size_t aLength = ftell(aRegFile);
    mRegVec.resize(aLength);
    fseek(aRegFile, 0, SEEK_SET);
    fread(mRegVec.data(), aLength, 1, aRegFile);
    fclose(aRegFile);
}

RegistryEmulator::~RegistryEmulator() {
    Flush();
}

size_t RegistryEmulator::FindKey(const std::string& theValueName) {
    if (mRegVec.size() == 0) return SIZE_MAX; // Empty file
    size_t aKeyIdx = 0;

    do {
        if (strcmp(theValueName.c_str(), (char *)&mRegVec[aKeyIdx]) == 0) break;
    } while ((aKeyIdx = GetNextKey(aKeyIdx)) != SIZE_MAX);

    return aKeyIdx;
}

size_t RegistryEmulator::Flush() {
    FILE *aRegFile = fopen(mREG_FILENAME.c_str(), "wb");
    size_t ret = fwrite(mRegVec.data(), mRegVec.size(), 1, aRegFile);
    fclose(aRegFile);
    return ret;
}

size_t RegistryEmulator::GetNextKey(size_t theKeyIdx) {
    // Are we even able to read this?
    if(theKeyIdx >= mRegVec.size()) return SIZE_MAX;

    while(mRegVec[theKeyIdx] != '\0') theKeyIdx++;  // theValueName

    theKeyIdx++;                                    // '\0'
    theKeyIdx += sizeof(uint32_t);                  // theType

    // Can we read the next entry's size? If not, we're at the end.
    if ((theKeyIdx + sizeof(uint32_t)) >= mRegVec.size()) return SIZE_MAX;

    uint32_t aLength;
    memcpy(&aLength, &mRegVec[theKeyIdx], sizeof(aLength));
    theKeyIdx += sizeof(uint32_t);                  // theLength
    theKeyIdx += aLength;                           // theValue

    return theKeyIdx;
}

bool RegistryEmulator::Read(const std::string& theValueName, uint32_t &theType, std::vector<uint8_t> &theValue, uint32_t &theLength) {
    size_t aKeyIdx = FindKey(theValueName);
    if(aKeyIdx == SIZE_MAX) return false;

    while(mRegVec[aKeyIdx] != '\0') aKeyIdx++;  // theValueName
    aKeyIdx++;                                  // '\0'

    memcpy(&theType, &mRegVec[aKeyIdx], sizeof(theType));
    aKeyIdx += sizeof(theType);
    memcpy(&theLength, &mRegVec[aKeyIdx], sizeof(theLength));
    aKeyIdx += sizeof(theLength);
    theValue.resize(theLength);
    memcpy(theValue.data(), &mRegVec[aKeyIdx], theLength);
    return true;
}

void RegistryEmulator::DeleteKey(size_t theKeyIdx) {
    if (theKeyIdx == SIZE_MAX) return; // Invalid Keys are not allowed

    size_t aNextKeyIdx = GetNextKey(theKeyIdx);

    // Remove old key. If it's the last element, remove it with resize()
    if (aNextKeyIdx == SIZE_MAX)
        mRegVec.resize(theKeyIdx);
    else
        mRegVec.erase(mRegVec.begin() + theKeyIdx, mRegVec.begin() + aNextKeyIdx);
}

bool RegistryEmulator::Erase(const std::string& theValueName) {
    size_t aKeyIdx = FindKey(theValueName);
    if (aKeyIdx == SIZE_MAX) return false;

    DeleteKey(aKeyIdx);
    return true;
}

void RegistryEmulator::Write(const std::string& theValueName, uint32_t theType, const uint8_t* theValue, uint32_t theLength) {
    size_t aKeyIdx = FindKey(theValueName);

    std::vector<uint8_t> aNewEntry;
    {
        const size_t aValueSize = theValueName.size() + 1;
        const size_t anEntryLength = aValueSize + sizeof(theType) + sizeof(theLength) + theLength;
        
        aNewEntry.resize(anEntryLength);
        uint8_t *anEntryPtr = aNewEntry.data();
        memcpy(anEntryPtr, theValueName.c_str(), aValueSize);
        anEntryPtr += aValueSize;
        memcpy(anEntryPtr, &theType, sizeof(theType));
        anEntryPtr += sizeof(theType);
        memcpy(anEntryPtr, &theLength, sizeof(theLength));
        anEntryPtr += sizeof(theLength);
        memcpy(anEntryPtr, theValue, theLength);
    }

    DeleteKey(aKeyIdx);

    mRegVec.insert(
        (aKeyIdx != SIZE_MAX) ? (mRegVec.begin() + aKeyIdx) : mRegVec.end(),
        aNewEntry.begin(),
        aNewEntry.end()
    );
}

void RegistryEmulatorTest() {
    RegistryEmulator aRegHandle("testreg.dat");

    std::vector<uint8_t> aReadTest;
    uint32_t aType;
    uint32_t aLength;
    if (aRegHandle.Read("PENGIS", aType, aReadTest, aLength)) {
        aReadTest.push_back('\0');
        printf("%s\n", (char *)aReadTest.data());
        printf("%d\n", aType);
        printf("%d\n", aLength);
    }

    char *test_string = (char *)"I HATE MINORITIES I HATE MINORITIES";
    aRegHandle.Write("MY_NAME_JEFF", REG_SZ, (uint8_t *)test_string, strlen(test_string));
    aRegHandle.Write("PENGIS", REG_SZ, (uint8_t *)test_string, strlen(test_string));
    //aRegHandle.Write("PEEPEEPOOPOO", REG_SZ, (uint8_t *)test_string, strlen(test_string));
    aRegHandle.Erase("PENGIS");
}
