#ifndef __MESSAGEWIDGET_H__
#define __MESSAGEWIDGET_H__

#include "../ConstEnums.h"
#include "../SexyAppFramework/Common.h"

#define MAX_MESSAGE_LENGTH 128
#define MAX_REANIM_LINES 5

class LawnApp;
namespace Sexy
{
	class Font;
	class Graphics;
}

class MessageWidget
{
public:
	LawnApp*			mApp;									//+0x0
	SexyChar			mLabel[MAX_MESSAGE_LENGTH];				//+0x4
	int					mDisplayTime;							//+0x84
	int					mDuration;								//+0x88
	MessageStyle		mMessageStyle;							//+0x8C
	ReanimationID		mTextReanimID[MAX_MESSAGE_LENGTH];		//+0x90
	ReanimationType		mReanimType;							//+0x290
	int					mSlideOffTime;							//+0x294
	SexyChar			mLabelNext[MAX_MESSAGE_LENGTH];			//+0x298
	MessageStyle		mMessageStyleNext;						//+0x318

public:
	MessageWidget(LawnApp* theApp);
	~MessageWidget() { ClearReanim(); }

	/*inline*/ void		SetLabel(const SexyString& theNewLabel, MessageStyle theMessageStyle);
	void				Update();
	void				Draw(Sexy::Graphics* g);
	void				ClearReanim();
	/*inline*/ void		ClearLabel();
	inline bool			IsBeingDisplayed() { return mDuration != 0; }
	/*inline*/ Font*	GetFont();
	void				DrawReanimatedText(Sexy::Graphics* g, Sexy::Font* theFont, const Sexy::Color& theColor, float thePosY);
	void				LayoutReanimText();
};

#endif