#ifndef __AWARDSCREEN_H__
#define __AWARDSCREEN_H__

#include "../../ConstEnums.h"
#include "../../SexyAppFramework/Widget.h"
using namespace Sexy;

class LawnApp;
class GameButton;
class AwardScreen : public Widget
{
private:
	enum
	{
		AwardScreen_Start = 100,
		AwardScreen_Menu = 101
	};

public:
	GameButton*			mStartButton;		//+0x88
	GameButton*			mMenuButton;		//+0x8C
	LawnApp*			mApp;				//+0x90
	int					mFadeInCounter;		//+0x94
	AwardType			mAwardType;			//+0x98

public:
	AwardScreen(LawnApp* theApp, AwardType theAwardType);
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
};

#endif
