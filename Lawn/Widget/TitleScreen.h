#ifndef __TITLESCREEN_H__
#define __TITLESCREEN_H__

#include "widget/Widget.h"
#include "widget/ButtonListener.h"

using namespace Sexy;

enum TitleState
{
	TITLESTATE_WAITING_FOR_FIRST_DRAW,
	TITLESTATE_POPCAP_LOGO,
	TITLESTATE_PARTNER_LOGO,
	TITLESTATE_SCREEN
};

namespace Sexy
{
	class HyperlinkWidget;
}

class LawnApp;

class TitleScreen :public Sexy::Widget, public Sexy::ButtonListener
{
public:
	enum
	{
		TitleScreen_Start,
		TitleScreen_Register
	};

public:
	HyperlinkWidget*		mStartButton;				//+0x8C
	float					mCurBarWidth;				//+0x90
	float					mTotalBarWidth; 			//+0x94
	float					mBarVel;					//+0x98
	float					mBarStartProgress;			//+0x9C
	bool					mRegisterClicked;			//+0xA0
	bool					mLoadingThreadComplete;		//+0xA1
	int						mTitleAge;					//+0xA4
	KeyCode					mQuickLoadKey;				//+0xA8
	bool					mNeedRegister;				//+0xAC
	bool					mNeedShowRegisterBox;		//+0xAD
	bool					mDrawnYet;					//+0xAE
	bool					mNeedToInit;				//+0xAF
	float					mPrevLoadingPercent;		//+0xB0
	TitleState				mTitleState;				//+0xB4
	int						mTitleStateCounter;			//+0xB8
	int						mTitleStateDuration;		//+0xBC
	bool					mDisplayPartnerLogo;		//+0xC0
	bool					mLoaderScreenIsLoaded;		//+0xC1
	LawnApp*				mApp;						//+0xC4

public:
	TitleScreen(LawnApp* theApp);
	virtual ~TitleScreen();

	virtual void			Update();
	virtual void			Draw(Graphics* g);
	virtual void			Resize(int theX, int theY, int theWidth, int theHeight);
	virtual void			AddedToManager(WidgetManager* theWidgetManager);
	virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
	virtual void			ButtonPress(int theId);
	virtual void			ButtonDepress(int theId);
	virtual void			ButtonDownTick(int){}
	virtual void			ButtonMouseEnter(int){}
	virtual void			ButtonMouseLeave(int){}
	virtual void			ButtonMouseMove(int, int, int){}
	virtual void			MouseDown(int x, int y, int theClickCount);
	virtual void			KeyDown(KeyCode theKey);
	void					SetRegistered();
	void					DrawToPreload(Graphics* g);
};

#endif
