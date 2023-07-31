#ifndef __LISTLISTENER_H__
#define __LISTLISTENER_H__

namespace Sexy 
{

class ListListener 
{
public:
	virtual void ListClicked(int theId, int theIdx, int theClickCount) {}
	virtual void ListClosed(int theId) {}
	virtual void ListHiliteChanged(int theId, int theOldIdx, int theNewIdx) {}
};

}

#endif //__LISTLISTENER_H__
