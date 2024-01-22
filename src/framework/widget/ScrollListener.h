#ifndef __SCROLLLISTENER_H__
#define __SCROLLLISTENER_H__

namespace Sexy 
{

class ScrollListener 
{
public:
	virtual void ScrollPosition(int theId, double thePosition) = 0;
};

}

#endif // __SCROLLLISTENER_H__
