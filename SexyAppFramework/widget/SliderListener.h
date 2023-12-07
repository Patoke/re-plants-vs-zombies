#ifndef __SLIDERLISTENER_H__
#define __SLIDERLISTENER_H__

namespace Sexy
{

class SliderListener
{
public:
	virtual void			SliderVal(int theId, double theVal) = 0;
};

}

#endif //__SLIDERLISTENER_H__
