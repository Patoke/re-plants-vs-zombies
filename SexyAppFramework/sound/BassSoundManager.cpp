#include "BassSoundManager.h"
#include "paklib/PakInterface.h"
#include "sound/bass.h"
#include "sound/BassLoader.h"
#include <cstring>
#include <fcntl.h>
#include <mutex>
#include <optional>

using namespace Sexy;

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

    mSourceSounds[theSfxID] = std::make_optional(gBass->BASS_SampleLoad(true, aBuf, 0, aLength, 32, 0));

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

    mSourceSounds[theSfxID] = std::make_optional(gBass->BASS_SampleLoad(true, aDestHeader, 0, aDestSize + 44, 32, 0));

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
    if (mSourceSounds[theSfxID].has_value())
    {
        gBass->BASS_SampleFree(mSourceSounds[theSfxID].value());
        mSourceSounds[theSfxID].reset();
        mSourceFileNames[theSfxID] = "";
    }
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
