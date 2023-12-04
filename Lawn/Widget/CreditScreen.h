#ifndef __CREDITSCREEN_H__
#define __CREDITSCREEN_H__

#include "../../ConstEnums.h"
#include "widget/Widget.h"
#include "misc/PerfTimer.h"
#include "widget/ButtonListener.h"

using namespace Sexy;

enum CreditsPhase
{
	CREDITS_MAIN1,
	CREDITS_MAIN2,
	CREDITS_MAIN3,
	CREDITS_END
};

enum CreditLayer
{
	CREDIT_LAYER_BACKGROUND,
	CREDIT_LAYER_ZOMBIE,
	CREDIT_LAYER_TOP
};

enum CreditWordType
{
	WORD_AA,
	WORD_EE,
	WORD_AW,
	WORD_OH,
	WORD_OFF
};

enum CreditBrainType
{
	BRAIN_FLY_ON,
	BRAIN_FAST_ON,
	BRAIN_NEXT_WORD,
	BRAIN_FAST_OFF,
	BRAIN_FLY_OFF,
	BRAIN_OFF
};

class CreditsTiming
{
public:
	float					mFrame;						//+0x0
	CreditWordType			mWordType;					//+0x4
	int						mWordX;						//+0x8
	CreditBrainType			mBrainType;					//+0xC
};

class TodsHackyUnprotectedPerfTimer : public PerfTimer
{
public:
	void					SetStartTime(int theTimeMillisecondsAgo);
};

class GameButton;
class LawnApp;
class LawnStoneButton;
class NewLawnButton;
class Reanimation;

class CreditScreen : public Widget, public ButtonListener
{
protected:
	enum
	{
		Credits_Button_Replay,
		Credits_Button_MainMenu
	};

public:
	GameButton*				mCloseButton;				//+0x8C
	LawnApp*				mApp;						//+0x90
	CreditsPhase			mCreditsPhase;				//+0x94
	int						mCreditsPhaseCounter;		//+0x98
	ReanimationID			mCreditsReanimID;			//+0x9C
	ParticleSystemID		mFogParticleID;				//+0xA0
	int						mBlinkCountdown;			//+0xA4
	LawnStoneButton*		mMainMenuButton;			//+0xA8
	NewLawnButton*			mReplayButton;				//+0xAC
	Widget*					mOverlayWidget;				//+0xB0
	bool					mDrawBrain;					//+0xB4
	float					mBrainPosX;					//+0xB8
	float					mBrainPosY;					//+0xBC
	int						mUpdateCount;				//+0xC0
	int						mDrawCount;					//+0xC4
	PerfTimer				mTimerSinceStart;			//+0xC8
	bool					mDontSync;					//+0xE0
	bool					mCreditsPaused;				//+0xE1
	double					mOriginalMusicVolume;		//+0xE8
	bool					mPreloaded;					//+0xF0
	int						mLastDrawCount;				//+0xF4

public:
	CreditScreen(LawnApp* theApp);
	virtual ~CreditScreen();

	virtual void			Update();
	virtual void			Draw(Graphics* g);
	virtual void			KeyChar(SexyChar theChar);
	virtual void			KeyDown(KeyCode theKey);
	virtual void			MouseUp(int x, int y, int theClickCount);
	virtual void			AddedToManager(WidgetManager* theWidgetManager);
	virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
	virtual void			ButtonPress(int theId);
	virtual void			ButtonDepress(int theId);
	virtual void			ButtonDownTick(int){}
	virtual void			ButtonMouseEnter(int){}
	virtual void			ButtonMouseLeave(int){}
	virtual void			ButtonMouseMove(int, int, int){}
	Reanimation*			PlayReanim(int aIndex);
	void					JumpToFrame(CreditsPhase thePhase, float theFrame);
	void					GetTiming(CreditsTiming** theBeforeTiming, CreditsTiming** theAfterTiming, float* theFraction);
	static Reanimation*		FindSubReanim(Reanimation* theReanim, ReanimationType theReanimType);
	void					DrawFogEffect(Graphics* g, float theTime);
	void					UpdateBlink();
	void					TurnOffTongues(Reanimation* theReanim, int aParentTrack);
	void					DrawFinalCredits(Graphics* g);
	void					DrawOverlay(Graphics* g);
	void					UpdateMovie();
	void					PauseCredits();
	void					PreLoadCredits();
};

void						DrawDisco(Graphics* g, float aCenterX, float aCenterY, float theTime);
void						DrawReanimToPreload(Graphics* g, ReanimationType theReanimType);

class CreditsOverlay : public Widget
{
public:
	CreditScreen*			mParent;

public:
	CreditsOverlay(CreditScreen* theCreditScreen);

	void					Draw(Graphics* g);
};

#endif
