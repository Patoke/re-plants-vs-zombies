#ifndef __BASS_SOUND_MANAGER_H__
#define __BASS_SOUND_MANAGER_H__

#include "SoundManager.h"
#include "BassSoundInstance.h"
#include <bass.h>
#include "BassMusicInterface.h"
#include <array>
#include <memory>

namespace Sexy
{

class BassSoundManager : public Sexy::SoundManager
{
public:
    BassSoundManager(HWND theHWnd);
    static double           PanDBToNorm(int dbpan);

    virtual bool            Initialized() { return Sexy::BassMusicInterface::gBassLoaded; }

    virtual bool            LoadSound(unsigned int theSfxID, const std::string& theFilename);
    virtual int             LoadSound(const std::string& theFilename);
    virtual void            ReleaseSound(unsigned int theSfxID);

    virtual void            SetVolume(double theVolume);
    virtual bool            SetBaseVolume(unsigned int theSfxID, double theBaseVolume);
    virtual bool            SetBasePan(unsigned int theSfxID, int theBasePan);

    virtual Sexy::SoundInstance*    GetSoundInstance(unsigned int theSfxID);

    virtual void            ReleaseSounds();
    virtual void            ReleaseChannels() {}

    virtual double          GetMasterVolume() { return 1.0; }
    virtual void            SetMasterVolume(double) {}

    virtual void            Flush() {}
//  virtual void            SetCooperativeWindow(HWND theHWnd) {}
    virtual void            StopAllSounds() {}
    virtual int             GetFreeSoundId();
    virtual int             GetNumSounds();
private:
    std::array<std::optional<std::unique_ptr<BassSoundInstance>>, MAX_CHANNELS> mPlayingSounds;
    std::array<std::string, MAX_SOURCE_SOUNDS>                                  mSourceFileNames;
    std::array<std::optional<HSAMPLE>, MAX_SOURCE_SOUNDS>                       mSourceSounds;

    bool LoadCompatibleSound(unsigned int theSfxID, const std::string& theFilename);
    bool LoadAUSound(unsigned int theSfxID, const std::string& theFilename);
    inline bool Exists(unsigned int theSfxID);
    void SetBaseVolumeAndPan(HSAMPLE theSample, std::optional<double> theBaseVolume, std::optional<int> theBasePan);
    int FindFreeChannel();
    void ReleaseFreeChannels();
};

}

#endif // __BASS_SOUND_MANAGER_H__
