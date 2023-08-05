#ifndef __ACHIEVEMENTSSCREEN_H__
#define __ACHIEVEMENTSSCREEN_H__

#include "../../ConstEnums.h"
#include "../../SexyAppFramework/Widget.h"

class LawnApp;

using namespace Sexy;

class AchievementsWidget : public Widget {
public:
	LawnApp*	mApp;                       //+GOTY @Patoke: 0xA8
	int			mScrollDirection;			//+GOTY @Patoke: 0xAC
	Rect		mMoreRockRect;				//+GOTY @Patoke: 0xC0
	int			mScrollValue;				//+GOTY @Patoke: 0xB0
	int			mScrollDecay;				//+GOTY @Patoke: 0xB4
	int			mDefaultScrollValue;		//+GOTY @Patoke: 0xB8
	bool		mDidPressMoreButton;		//+GOTY @Patoke: 0xBC

	AchievementsWidget(LawnApp* theApp);
	virtual ~AchievementsWidget();

	virtual void                Update();
	virtual void                Draw(Graphics* g);
	virtual void                KeyDown(KeyCode theKey);
	virtual void                MouseDown(int x, int y, int theClickCount);
	virtual void                MouseUp(int x, int y, int theClickCount);
	virtual void				MouseWheel(int theDelta);
};

#endif