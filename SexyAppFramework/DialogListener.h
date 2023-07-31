#ifndef __DIALOGLISTENER_H__
#define __DIALOGLISTENER_H__

namespace Sexy
{

class DialogListener
{
public:
	virtual void			DialogButtonPress(int theDialogId, int theButtonId) {}
	virtual void			DialogButtonDepress(int theDialogId, int theButtonId) {}
};

}

#endif // __DIALOGLISTENER_H__