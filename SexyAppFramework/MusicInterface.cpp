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
	(void)theSongId;(void)theFileName;
	return false;
}

void MusicInterface::PlayMusic(int, int, bool){}
void MusicInterface::StopMusic(int){}
void MusicInterface::PauseMusic(int){}
void MusicInterface::ResumeMusic(int){}
void MusicInterface::StopAllMusic(){}
void MusicInterface::UnloadMusic(int){}
void MusicInterface::UnloadAllMusic(){}
void MusicInterface::PauseAllMusic(){}
void MusicInterface::ResumeAllMusic(){}
void MusicInterface::FadeIn(int, int, double, bool){}
void MusicInterface::FadeOut(int, bool, double){}
void MusicInterface::FadeOutAll(bool, double){}
void MusicInterface::SetSongVolume(int, double){}
void MusicInterface::SetSongMaxVolume(int, double){}

bool MusicInterface::IsPlaying(int theSongId)
{
	(void)theSongId;
	return false;
}

void MusicInterface::SetVolume(double){}
void MusicInterface::SetMusicAmplify(int, double){}
void MusicInterface::Update(){}
