#ifndef __HYPERLINKWIDGET_H__
#define __HYPERLINKWIDGET_H__

#include "ButtonWidget.h"

namespace Sexy
{

class HyperlinkWidget : public ButtonWidget
{
public:
	Color					mColor;
	Color					mOverColor;
	int						mUnderlineSize;
	int						mUnderlineOffset;

public:
	HyperlinkWidget(int theId, ButtonListener* theButtonListener);

	void					Draw(Graphics* g);
	void					MouseEnter();
	void					MouseLeave();	
};

}

#endif //__HYPERLINKWIDGET_H__