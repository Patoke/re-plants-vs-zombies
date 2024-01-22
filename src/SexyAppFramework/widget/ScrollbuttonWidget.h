#ifndef __SCROLLBUTTONWIDGET_H__
#define __SCROLLBUTTONWIDGET_H__

#include "widget/ButtonWidget.h"

namespace Sexy 
{
	
class ButtonListener;

class ScrollbuttonWidget : public ButtonWidget 
{
public:
	bool				mHorizontal;

	// mType is a new way of doing things (1 = up, 2 = down, 3 = left, 4 = right).  
	// This overrides mHorizontal and mId.
	int					mType; 

public:
	ScrollbuttonWidget(int theId, ButtonListener *theButtonListener, int theType = 0);
	virtual ~ScrollbuttonWidget();

	void				Draw(Graphics *g);
};

}

#endif //__SCROLLBUTTONWIDGET_H__
