#ifndef __LAWNDIALOG_H__
#define __LAWNDIALOG_H__

#include "../../Sexy.TodLib/TodCommon.h"
#include "../../SexyAppFramework/Dialog.h"

constexpr const int DIALOG_HEADER_OFFSET = 45;

class LawnApp;
class LawnDialog;
class Reanimation;
class LawnStoneButton;
class ReanimationWidget;
namespace Sexy
{
	class Checkbox;
	class DialogButton;
	class CheckboxListener;
}

using namespace std;
using namespace Sexy;

class ReanimationWidget : public Widget
{
public:
    LawnApp*				mApp;					//+0x88
    Reanimation*			mReanim;				//+0x8C
    LawnDialog*				mLawnDialog;			//+0x90
    float					mPosX;					//+0x94
    float					mPosY;					//+0x98

public:
	ReanimationWidget();
	virtual ~ReanimationWidget();

	/*inline*/ void			Dispose();
	virtual void			Draw(Graphics*);
	virtual void			Update();
	void					AddReanimation(float x, float y, ReanimationType theReanimationType);
};

class LawnDialog : public Dialog
{
public:
	LawnApp*				mApp;					//+0x150
	int						mButtonDelay;			//+0x154
	ReanimationWidget*		mReanimation;			//+0x158
	bool					mDrawStandardBack;		//+0x15C
	LawnStoneButton*		mLawnYesButton;			//+0x160
	LawnStoneButton*		mLawnNoButton;			//+0x164
	bool					mTallBottom;			//+0x168
	bool					mVerticalCenterText;	//+0x169

public:
	LawnDialog(LawnApp* theApp, int theId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode);
	~LawnDialog();

	int						GetLeft();
	int						GetWidth();
	int						GetTop();
	virtual void			SetButtonDelay(int theDelay);
	virtual void			Update();
	virtual void			ButtonPress(int theId);
	virtual void			ButtonDepress(int theId);
	virtual void			CheckboxChecked(int theId, bool theChecked);
	virtual void			KeyDown(KeyCode theKey);
	virtual void			AddedToManager(WidgetManager* theWidgetManager);
	virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
	virtual void			Resize(int theX, int theY, int theWidth, int theHeight);
	virtual void			Draw(Graphics* g);
	void					CalcSize(int theExtraX, int theExtraY);
};

class GameOverDialog : public LawnDialog
{
public:
	DialogButton*			mMenuButton;

public:
	GameOverDialog(const SexyString& theMessage, bool theShowChallengeName);
	virtual ~GameOverDialog();

	virtual void			ButtonDepress(int theId);
	virtual void			AddedToManager(WidgetManager* theWidgetManager);
	virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
	virtual void			MouseDrag(int x, int y);
};

#endif
