#ifndef __AWARDSCREEN_H__
#define __AWARDSCREEN_H__

#include "ConstEnums.h"
#include "framework/widget/Widget.h"
using namespace Sexy;

class LawnApp;
class GameButton;

class AchievementScreenItem {
public:
	int mId; //+GOTY @Patoke: 0x00
	int mStartAnimTime; //+GOTY @Patoke: 0x04
	int mEndAnimTime; //+GOTY @Patoke: 0x08
	int mDestY; //+GOTY @Patoke: 0x0C
	int mStartY; //+GOTY @Patoke: 0x10
	int mY; //+GOTY @Patoke: 0x14
};

class AwardScreen : public Widget
{
private:
	enum
	{
		AwardScreen_Start = 100,
		AwardScreen_Menu = 101
	};

public:
	GameButton*							mStartButton;			//+0x88
	GameButton*							mMenuButton;			//+0x8C
	LawnApp*							mApp;					//+0x90
	int									mFadeInCounter;			//+0x94
	AwardType							mAwardType;				//+0x98 GOTY @Patoke: 0xB8
	GameButton*							mContinueButton;		//+GOTY @Patoke: 0xA8
	bool								mShowStartButtonAfterAchievements;	//+GOTY @Patoke: 0xAC
	bool								mShowMenuButtonAfterAchievements;	//+GOTY @Patoke: 0xAD
	int									mAchievementAnimTime;	//+GOTY @Patoke: 0xBC
	bool								mShowingAchievements;	//+GOTY @Patoke: 0xD8
	std::vector<AchievementScreenItem>	mAchievementItems;		//+GOTY @Patoke: 0xC0

public:
	// @Patoke: added argument
	AwardScreen(LawnApp* theApp, AwardType theAwardType, bool theShowingAchievements = false);
	virtual ~AwardScreen();

	/*inline*/ bool		IsPaperNote();
	virtual void		Resize(int theX, int theY, int theWidth, int theHeight) { Widget::Resize(theX, theY, theWidth, theHeight); }
	static void			DrawBottom(Graphics* g, const SexyString& theTitle, const SexyString& theAward, const SexyString& theMessage);
	void				DrawAwardSeed(Graphics* g);
	virtual void		Draw(Graphics* g);
	virtual void		Update();
	virtual void		AddedToManager(WidgetManager* theWidgetManager) { Widget::AddedToManager(theWidgetManager); }
	virtual void		RemovedFromManager(WidgetManager* theWidgetManager) { Widget::RemovedFromManager(theWidgetManager); }
	virtual void		KeyChar(char theChar);
	void				StartButtonPressed();
	virtual void		MouseDown(int x, int y, int theClickCount);
	virtual void		MouseUp(int x, int y, int theClickCount);
	// @Patoke: implement functions
	void				DrawAchievements(Graphics* g);
	void				AchievementsContinuePressed();
};

#endif
