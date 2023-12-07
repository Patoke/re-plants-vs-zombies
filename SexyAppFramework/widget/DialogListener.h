#ifndef __DIALOGLISTENER_H__
#define __DIALOGLISTENER_H__

namespace Sexy
{

class DialogListener
{
public:
	virtual void			DialogButtonPress(int theDialogId, int theButtonId) = 0;
	virtual void			DialogButtonDepress(int theDialogId, int theButtonId) = 0;
};

}

#endif // __DIALOGLISTENER_H__
