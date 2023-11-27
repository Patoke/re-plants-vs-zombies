#ifndef __BUTTONLISTENER_H__
#define __BUTTONLISTENER_H__

namespace Sexy
{

class ButtonListener
{
public:
	virtual void			ButtonPress(int theId) = 0;
	virtual void			ButtonDepress(int theId) = 0;
	virtual void			ButtonDownTick(int theId) = 0;
	virtual void			ButtonMouseEnter(int theId) = 0;
	virtual void			ButtonMouseLeave(int theId) = 0;
	virtual void			ButtonMouseMove(int theId, int theX, int theY) = 0;
};

}

#endif //__BUTTONLISTENER_H__
