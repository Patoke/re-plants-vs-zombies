#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "Widget.h"

namespace Sexy
{

class CheckboxListener;
class Image;

class Checkbox : public Widget
{	
protected:
	CheckboxListener*		mListener;

public:
	int						mId;

	bool					mChecked;

	Image*					mUncheckedImage;
	Image*					mCheckedImage;

	Rect					mCheckedRect;
	Rect					mUncheckedRect;

	Color					mOutlineColor;	// These are only used if no image is specified
	Color					mBkgColor;
	Color					mCheckColor;

public:
	virtual void			SetChecked(bool checked, bool tellListener = true);
	virtual bool			IsChecked();

	virtual void			MouseDown(int x, int y, int theClickCount) { Widget::MouseDown(x, y, theClickCount); }
	virtual void			MouseDown(int x, int y, int theBtnNum, int theClickCount);
	virtual void			Draw(Graphics* g);

public:
	Checkbox(Image* theUncheckedImage, Image* theCheckedImage, int theId, CheckboxListener* theCheckboxListener);
};

}

#endif //__CHECKBOX_H__