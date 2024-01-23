#include "BassSoundManager.h"
#include "paklib/PakInterface.h"
#include "sound/bass.h"
#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <mutex>
#include <optional>

namespace Sexy {

BassSoundManager::BassSoundManager(void* theHWnd) {
    BassMusicInterface::InitBass(theHWnd);
}

bool BassSoundManager::LoadCompatibleSound(unsigned int theSfxID, const std::string& theFilename)
{
    PFILE *aFile = p_fopen(theFilename.c_str(), "rb");
    if (aFile == NULL) return false;

    p_fseek(aFile, 0, SEEK_END);
    size_t aLength = p_ftell(aFile);
    p_fseek(aFile, 0, SEEK_SET);

    char *aBuf = (char *)malloc(aLength);
    p_fread(aBuf, aLength, 1, aFile);
    p_fclose(aFile);

    mSourceSounds[theSfxID] = std::make_optional(BASS_SampleLoad(true, aBuf, 0, aLength, MAX_CHANNELS, 0));

    free(aBuf);
    return true;
}

bool BassSoundManager::LoadAUSound(unsigned int theSfxID, const std::string& theFilename)
{
    PFILE* fp;

    fp = p_fopen(theFilename.c_str(), "rb");    

    if (fp == NULL) return false;   

    char aHeaderId[5];  
    aHeaderId[4] = '\0';    
    p_fread(aHeaderId, 1, 4, fp);   
    if ((!strcmp(aHeaderId, ".snd")) == 0)
        return false;

    ulong aHeaderSize;  
    p_fread(&aHeaderSize, 4, 1, fp);
    aHeaderSize = LONG_BIGE_TO_NATIVE(aHeaderSize);

    ulong aDataSize;
    p_fread(&aDataSize, 4, 1, fp);
    aDataSize = LONG_BIGE_TO_NATIVE(aDataSize);

    ulong anEncoding;
    p_fread(&anEncoding, 4, 1, fp);
    anEncoding = LONG_BIGE_TO_NATIVE(anEncoding);

    ulong aSampleRate;
    p_fread(&aSampleRate, 4, 1, fp);
    aSampleRate = LONG_BIGE_TO_NATIVE(aSampleRate);

    ulong aChannelCount;
    p_fread(&aChannelCount, 4, 1, fp);
    aChannelCount = LONG_BIGE_TO_NATIVE(aChannelCount);

    p_fseek(fp, aHeaderSize, SEEK_SET); 

    bool ulaw = false;

    ulong aSrcBitCount = 8;
    ulong aBitCount = 16;   
    switch (anEncoding)
    {
    case 1:
        aSrcBitCount = 8;
        aBitCount = 16;
        ulaw = true;
        break;
    case 2:
        aSrcBitCount = 8;
        aBitCount = 8;
        break;
    
    /*
    Support these formats?
    
    case 3:
        aBitCount = 16;
        break;
    case 4:
        aBitCount = 24;
        break;
    case 5:
        aBitCount = 32;
        break;*/

    default:
        return false;       
    }

    ulong aDestSize = aDataSize * aBitCount/aSrcBitCount;

    uchar* aDestHeader = new uchar[aDestSize + 44];

    uchar* aPtr = aDestHeader;
    memcpy(aPtr, "RIFF", 4);                          aPtr += 4;
    *(uint32_t *)aPtr = aDestSize + 36;                               aPtr += 4;
    memcpy(aPtr, "WAVE", 4);                          aPtr += 4;
    memcpy(aPtr, "fmt\0", 4);                         aPtr += 4;
    *(uint32_t *)aPtr = 16;                                          aPtr += 4;
    *(uint16_t *)aPtr = 1;                                           aPtr += 2;
    *(uint16_t *)aPtr = aChannelCount;                               aPtr += 2;
    *(uint32_t *)aPtr = aSampleRate;                                 aPtr += 4;
    *(uint32_t *)aPtr = aSampleRate * aBitCount * aChannelCount / 8; aPtr += 4;
    *(uint16_t *)aPtr = aBitCount * aChannelCount / 8;               aPtr += 2;
    *(uint16_t *)aPtr = aBitCount;                                   aPtr += 2;
    memcpy(aPtr, "data", 4);                          aPtr += 4;
    *(uint32_t *)aPtr = aDestSize;                                   aPtr += 4;

    short* aDestBuffer = (short*)(aDestHeader + 44);

    if (ulaw)
    {
        uchar* aSrcBuffer = new uchar[aDataSize];

        ulong aReadSize = p_fread(aSrcBuffer, 1, aDataSize, fp);
        p_fclose(fp);
        if (aReadSize != aDataSize) return false;

        for (ulong i = 0; i < aDataSize; i++)
        {
            int ch = aSrcBuffer[i];

            int sign = (ch < 128) ? -1 : 1;
            ch = ch | 0x80;
            if (ch > 239)
                ch = ((0xF0 | 15) - ch) * 2;
            else if (ch > 223)
                ch = (((0xE0 | 15) - ch) * 4) + 32;
            else if (ch > 207)
                ch = (((0xD0 | 15) - ch) * 8) + 96;
            else if (ch > 191)
                ch = (((0xC0 | 15) - ch) * 16) + 224;
            else if (ch > 175)
                ch = (((0xB0 | 15) - ch) * 32) + 480;
            else if (ch > 159)
                ch = (((0xA0 | 15) - ch) * 64) + 992;
            else if (ch > 143)
                ch = (((0x90 | 15) - ch) * 128) + 2016;
            else if (ch > 128)
                ch = (((0x80 | 15) - ch) * 256) + 4064;
            else
                ch = 0xff;          

            aDestBuffer[i] = sign * ch * 4;
        }

        delete [] aSrcBuffer;
    } else {
        ulong aReadSize = p_fread(aDestBuffer, 1, aDataSize, fp);
        p_fclose(fp);
        if (aReadSize != aDataSize) return false;
    }

    mSourceSounds[theSfxID] = std::make_optional(BASS_SampleLoad(true, aDestHeader, 0, aDestSize + 44, MAX_CHANNELS, 0));

    return true;
}

bool BassSoundManager::LoadSound(unsigned int theSfxID, const std::string& theFilename)
{
    if ((theSfxID < 0) || (theSfxID >= MAX_SOURCE_SOUNDS))
        return false;

    ReleaseSound(theSfxID);

    mSourceFileNames[theSfxID] = theFilename;

    std::string aFilename = theFilename;
    //std::string aCachedName;

    /* Disabling caching because caching is hard.
    if ((aFilename.length() > 2) && (aFilename[0] != '\\') && (aFilename[0] != '/') &&
        (aFilename[1] != ':'))
    {
        // Not an absolute path
        aCachedName = GetAppDataFolder() + "cached\\" + aFilename + ".wav";
        if (LoadCompatibleSound(theSfxID, aCachedName))
            return true;
        MkDir(GetFileDir(aCachedName));
    }*/

    if (LoadCompatibleSound(theSfxID, aFilename + ".wav"))  
        return true;
    
    if (LoadCompatibleSound(theSfxID, aFilename + ".ogg"))
    {       
        //WriteWAV(theSfxID, aCachedName, aFilename + ".ogg");
        return true;
    }

    if (LoadAUSound(theSfxID, aFilename + ".au"))
    {
        //WriteWAV(theSfxID, aCachedName, aFilename + ".au");
        return true;
    }

    return false;
}


int BassSoundManager::LoadSound(const std::string& theFilename)
{
    int i;
    for (i = 0; i < MAX_SOURCE_SOUNDS; i++)
        if (mSourceFileNames[i] == theFilename)
            return i;

    for (i = MAX_SOURCE_SOUNDS-1; i >= 0; i--)
    {       
        if (!mSourceSounds[i].has_value())
        {
            if (!LoadSound(i, theFilename))
                return -1;
            else
                return i;
        }
    }

    return -1;
}

void BassSoundManager::ReleaseSound(unsigned int theSfxID)
{
    if (Exists(theSfxID))
    {
        BASS_SampleFree(mSourceSounds[theSfxID].value());
        mSourceSounds[theSfxID].reset();
        mSourceFileNames[theSfxID] = "";
    }
}

void BassSoundManager::ReleaseSounds() {
    for (unsigned int i = 0; i < MAX_SOURCE_SOUNDS; ++i) {
        ReleaseSound(i);
    }
}

inline bool BassSoundManager::Exists(unsigned int theSfxID) {
    return (theSfxID < MAX_SOURCE_SOUNDS) && (mSourceSounds[theSfxID].has_value());
}

void BassSoundManager::SetVolume(double theVolume) {
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, (int) (theVolume * 10000));
}

double BassSoundManager::PanDBToNorm(int dbpan) {
    auto sign = [] (int val) {
        return (0 < val) - (val < 0);
    };
    return sign(dbpan)*pow(10.0, -abs(dbpan) / 2000.0);
};

void BassSoundManager::SetBaseVolumeAndPan(HSAMPLE theSample, std::optional<double> theBaseVolume, std::optional<int> theBasePan) {
    BASS_SAMPLE info;
    BASS_SampleGetInfo(theSample, &info);

    if (theBasePan.has_value()) {
        auto sign = [] (int val) {
            return (0 < val) - (val < 0);
        };
        info.pan = sign(theBasePan.value())*pow(10.0, -abs(theBasePan.value()) / 2000.0);
    }

    if (theBaseVolume.has_value()) {
        info.volume = theBaseVolume.value();
    }

    BASS_SampleSetInfo(theSample, &info);
}

bool BassSoundManager::SetBaseVolume(unsigned int theSfxID, double theBaseVolume) {
    if (!Exists(theSfxID)) return false;

    SetBaseVolumeAndPan(mSourceSounds[theSfxID].value(), theBaseVolume, {});

    return true;
}

bool BassSoundManager::SetBasePan(unsigned int theSfxID, int theBasePan) {
    if (!Exists(theSfxID)) return false;

    SetBaseVolumeAndPan(mSourceSounds[theSfxID].value(), {}, theBasePan);

    return true;
} 

int BassSoundManager::GetFreeSoundId()
{
    for (int i=0; i<MAX_SOURCE_SOUNDS; i++)
    {
        if (!mSourceSounds[i].has_value())
            return i;
    }

    return -1;
}

int BassSoundManager::GetNumSounds()
{
    int aCount = 0;
    for (int i=0; i<MAX_SOURCE_SOUNDS; i++)
    {
        if (mSourceSounds[i].has_value())
            aCount++;
    }

    return aCount;
}

void BassSoundManager::ReleaseFreeChannels()
{
    for (int i = 0; i < MAX_CHANNELS; i++)
        if (mPlayingSounds[i].has_value() && mPlayingSounds[i].value()->IsReleased())
        {
            mPlayingSounds[i].reset();
        }
}

int BassSoundManager::FindFreeChannel()
{
    static auto timer = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    if (now - timer > std::chrono::duration<double>(1))
    {
        ReleaseFreeChannels();
        timer = now;
    }

    for (int i = 0; i < MAX_CHANNELS; i++)
    {
        if (!mPlayingSounds[i].has_value())
            return i;

        if (mPlayingSounds[i].value()->IsReleased())
        {
            mPlayingSounds[i].reset();
            return i;
        }
    }
    
    return -1;
}

SoundInstance* BassSoundManager::GetSoundInstance(unsigned int theSfxID)
{
    if (!Exists(theSfxID)) return nullptr;

    int aFreeChannel = FindFreeChannel();
    if (aFreeChannel < 0)
        return nullptr;
    
    mPlayingSounds[aFreeChannel] = std::make_unique<BassSoundInstance>(mSourceSounds[theSfxID].value());

    return mPlayingSounds[aFreeChannel].value().get();
}

} // Namespace Sexy
