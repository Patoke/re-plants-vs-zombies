#include "MusicInterface.h"
#include "SexyAppBase.h"

using namespace Sexy;

MusicInterface::MusicInterface()
{		
}

MusicInterface::~MusicInterface()
{
}

bool MusicInterface::LoadMusic(int theSongId, const std::string& theFileName)
{
	return false;
}

void MusicInterface::PlayMusic(int theSongId, int theOffset, bool noLoop)	
{
}

void MusicInterface::StopMusic(int theSongId)
{
}

void MusicInterface::PauseMusic(int theSongId)
{
}

void MusicInterface::ResumeMusic(int theSongId)
{
}

void MusicInterface::StopAllMusic()		
{
}

void MusicInterface::UnloadMusic(int theSongId)
{
}

void MusicInterface::UnloadAllMusic()
{
}

void MusicInterface::PauseAllMusic()
{
}

void MusicInterface::ResumeAllMusic()
{
}

void MusicInterface::FadeIn(int theSongId, int theOffset, double theSpeed, bool noLoop)
{
}

void MusicInterface::FadeOut(int theSongId, bool stopSong, double theSpeed)
{
}

void MusicInterface::FadeOutAll(bool stopSong, double theSpeed)
{
}

void MusicInterface::SetSongVolume(int theSongId, double theVolume)
{
}

void MusicInterface::SetSongMaxVolume(int theSongId, double theMaxVolume)
{
}

bool MusicInterface::IsPlaying(int theSongId)
{
	return false;
}

void MusicInterface::SetVolume(double theVolume)
{
}

void MusicInterface::SetMusicAmplify(int theSongId, double theAmp)
{
}

void MusicInterface::Update()
{
}
