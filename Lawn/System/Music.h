#ifndef __MUSIC_H__
#define __MUSIC_H__

#include <string>
#include "../../SexyAppFramework/bass.h"

class LawnApp;
namespace Sexy
{
	class MusicInterface;
};

enum MusicTune
{
	MUSIC_TUNE_NONE = -1,
	MUSIC_TUNE_DAY_GRASSWALK = 1,				// 白天草地关卡
	MUSIC_TUNE_NIGHT_MOONGRAINS,				// 黑夜草地关卡
	MUSIC_TUNE_POOL_WATERYGRAVES,				// 白天泳池关卡
	MUSIC_TUNE_FOG_RIGORMORMIST,				// 黑夜泳池关卡
	MUSIC_TUNE_ROOF_GRAZETHEROOF,				// 屋顶关卡
	MUSIC_TUNE_CHOOSE_YOUR_SEEDS,				// 选卡界面/小游戏界面
	MUSIC_TUNE_TITLE_CRAZY_DAVE_MAIN_THEME,		// 主菜单
	MUSIC_TUNE_ZEN_GARDEN,						// 禅境花园
	MUSIC_TUNE_PUZZLE_CEREBRAWL,				// 解谜模式
	MUSIC_TUNE_MINIGAME_LOONBOON,				// 小游戏
	MUSIC_TUNE_CONVEYER,						// 传送带关卡
	MUSIC_TUNE_FINAL_BOSS_BRAINIAC_MANIAC,		// 僵王博士关卡
	MUSIC_TUNE_CREDITS_ZOMBIES_ON_YOUR_LAWN,	// MV
	NUM_MUSIC_TUNES
};

enum MusicFile
{
	MUSIC_FILE_NONE = -1,
	MUSIC_FILE_MAIN_MUSIC = 1,
	MUSIC_FILE_DRUMS,
	MUSIC_FILE_HIHATS,
	MUSIC_FILE_CREDITS_ZOMBIES_ON_YOUR_LAWN,
	NUM_MUSIC_FILES
};

enum MusicBurstState
{
	MUSIC_BURST_OFF,
	MUSIC_BURST_STARTING,
	MUSIC_BURST_ON,
	MUSIC_BURST_FINISHING
};

enum MusicDrumsState
{
	MUSIC_DRUMS_OFF,
	MUSIC_DRUMS_ON_QUEUED,
	MUSIC_DRUMS_ON,
	MUSIC_DRUMS_OFF_QUEUED,
	MUSIC_DRUMS_FADING
};

class MusicFileData
{
public:
	unsigned int*				mFileData;
};
extern MusicFileData gMusicFileData[MusicFile::NUM_MUSIC_FILES];  //0x6A9ED0

class Music
{
public:
	LawnApp*					mApp;								//+0x0
	Sexy::MusicInterface*		mMusicInterface;					//+0x4
	MusicTune					mCurMusicTune;						//+0x8
	MusicFile					mCurMusicFileMain;					//+0xC
	MusicFile					mCurMusicFileDrums;					//+0x10
	MusicFile					mCurMusicFileHihats;				//+0x14
	int							mBurstOverride;						//+0x18
	float						mBaseBPM;							//+0x1C
	float						mBaseModSpeed;						//+0x20
	MusicBurstState				mMusicBurstState;					//+0x24
	int							mBurstStateCounter;					//+0x28
	MusicDrumsState				mMusicDrumsState;					//+0x2C
	int							mQueuedDrumTrackPackedOrder;		//+0x30
	int							mDrumsStateCounter;					//+0x34
	int							mPauseOffset;						//+0x38
	int							mPauseOffsetDrums;					//+0x3C
	bool						mPaused;							//+0x40
	bool						mMusicDisabled;						//+0x41
	int							mFadeOutCounter;					//+0x44
	int							mFadeOutDuration;					//+0x48

public:
	Music();

	void						MusicInit();
	void						MusicDispose() { ; }
	void						MusicUpdate();
	void						StopAllMusic();
	/*inline*/ void				PlayMusic(MusicTune theMusicTune, int theOffset = -1, int theDrumsOffset = -1);
	/*inline*/ HMUSIC			GetBassMusicHandle(MusicFile theMusicFile);
	void						StartGameMusic();
	/*inline*/ void				LoadSong(MusicFile theMusicFile, const std::string& theFileName);
	void						MusicResync();
	void						UpdateMusicBurst();
	/*inline*/ void				StartBurst();
	void						GameMusicPause(bool thePause);
	void						PlayFromOffset(MusicFile theMusicFile, int theOffset, double theVolume);
	void						MusicResyncChannel(MusicFile theMusicFileToMatch, MusicFile theMusicFileToSync);
	bool						TodLoadMusic(MusicFile theMusicFile, const std::string& theFileName);
	void						MusicTitleScreenInit();
	/*inline*/ void				MakeSureMusicIsPlaying(MusicTune theMusicTune);
	/*inline*/ void				FadeOut(int theFadeOutDuration);
	void						SetupMusicFileForTune(MusicFile theMusicFile, MusicTune theMusicTune);
	unsigned long				GetMusicOrder(MusicFile theMusicFile);
	void						MusicCreditScreenInit();
	int							GetNumLoadingTasks();
};

#endif

