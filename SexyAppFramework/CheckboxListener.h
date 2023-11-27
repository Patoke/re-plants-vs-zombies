#ifndef __CHECKBOX_LISTENER__
#define __CHECKBOX_LISTENER__

namespace Sexy
{

class CheckboxListener
{
public:
	virtual void			CheckboxChecked(int theId, bool checked) = 0;
};

}

#endif //__CHECKBOX_LISTENER__
