#ifndef __QUANTIZE_H__
#define __QUANTIZE_H__

#include "Common.h"

namespace Sexy
{

bool Quantize8Bit(const ulong* theSrcBits, int theWidth, int theHeight, uchar* theDestColorIndices, ulong* theDestColorTable);

}

#endif //__QUANTIZE_H__