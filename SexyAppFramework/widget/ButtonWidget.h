#ifndef __BUTTONWIDGET_H__
#define __BUTTONWIDGET_H__

#include "Widget.h"

namespace Sexy
{

class Image;
class ButtonListener;

class ButtonWidget : public Widget
{
public:
	enum {
		BUTTON_LABEL_LEFT	= -1,
		BUTTON_LABEL_CENTER,
		BUTTON_LABEL_RIGHT
	};
	enum
	{
		COLOR_LABEL,
		COLOR_LABEL_HILITE,
		COLOR_DARK_OUTLINE,
		COLOR_LIGHT_OUTLINE,
		COLOR_MEDIUM_OUTLINE,
		COLOR_BKG,
		NUM_COLORS
	};

	int						mId;	
	SexyString				mLabel;
	int						mLabelJustify;
	_Font*					mFont;
	Image*					mButtonImage;
	Image*					mOverImage;
	Image*					mDownImage;	
	Image*					mDisabledImage;
	Rect					mNormalRect;
	Rect					mOverRect;
	Rect					mDownRect;
	Rect					mDisabledRect;

	bool					mInverted;
	bool					mBtnNoDraw;
	bool					mFrameNoDraw;
	ButtonListener*			mButtonListener;

	double					mOverAlpha;
	double					mOverAlphaSpeed;
	double					mOverAlphaFadeInSpeed;

	bool					HaveButtonImage(Image *theImage, const Rect &theRect);
	virtual void			DrawButtonImage(Graphics *g, Image *theImage, const Rect &theRect, int x, int y);
	

public:
	ButtonWidget(int theId, ButtonListener* theButtonListener);
	virtual ~ButtonWidget();
	
	virtual void			SetFont(_Font* theFont);
	virtual bool			IsButtonDown();
	virtual void			Draw(Graphics* g);
	virtual void			SetDisabled(bool isDisabled);
	virtual void			MouseEnter();
	virtual void			MouseLeave();
	virtual void			MouseMove(int theX, int theY);
	virtual void			MouseDown(int theX, int theY, int theClickCount) { Widget::MouseDown(theX, theY, theClickCount); }
	virtual void			MouseDown(int theX, int theY, int theBtnNum, int theClickCount);
	virtual void			MouseUp(int theX, int theY) { Widget::MouseUp(theX, theY); }
	virtual void			MouseUp(int theX, int theY, int theClickCount) { Widget::MouseUp(theX, theY, theClickCount); }
	virtual void			MouseUp(int theX, int theY, int theBtnNum, int theClickCount);
	virtual void			Update();
};

}

#endif //__BUTTONWIDGET_H__
