#ifndef __SEXY_DIRECTXERRORSTRING_H__
#define __SEXY_DIRECTXERRORSTRING_H__
#include <string>
#include <ddraw.h>

namespace Sexy
{
	std::string GetDirectXErrorString(HRESULT theResult);
} // namespace 

#endif