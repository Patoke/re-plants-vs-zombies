#ifndef __MODVAL_H__
#define __MODVAL_H__

#include <string>

/*
 This module allows for dynamic modification of integer and floating-point 
 constants at run-time.

 Place the M() around values you want to allow to be changed at run-time. 
 When you want to modify a value, simply change the value in the source
 code file and call ReparseModValues() within your program to re-read the
 source files, checking for changes in constants.  Your program will need
 to contain some trigger such as a key combination to trigger the 
 ReparseModValues() call.

 Example:
	x = x + M(2.1);

 Caveats:
	This module determines which files to parse through (during
	ReparseModValues()) at run-time, so if a M() macro has not yet been
	executed within a particular source file, its value will not be 
	updated.

 Performance:
	There a small setup cost the first time each M() value is accessed
	after program startup and reparsing, but after that there is just the 
	tiny overhead of a function call and a few vector dereferences.

 */

namespace Sexy
{

#if defined(SEXY_DISABLE_MODVAL) || defined(RELEASEFINAL)
#define M(val)  (val)
#define M1(val) (val)
#define M2(val) (val)
#define M3(val) (val)
#define M4(val) (val)
#define M5(val) (val)
#define M6(val) (val)
#define M7(val) (val)
#define M8(val) (val)
#define M9(val) (val)
#else
#define MODVAL_STR_COUNTER2(x,y,z) x#y","#z
#define MODVAL_STR_COUNTER1(x,y,z) MODVAL_STR_COUNTER2(x,y,z)
#define MODVAL_STR_COUNTER(x) MODVAL_STR_COUNTER1(x,__COUNTER__,__LINE__)
#define M(val) ModVal(0, MODVAL_STR_COUNTER("SEXY_SEXYMODVAL" __FILE__), (val))
#define M1(val) M(val)
#define M2(val) M(val)
#define M3(val) M(val)
#define M4(val) M(val)
#define M5(val) M(val)
#define M6(val) M(val)
#define M7(val) M(val)
#define M8(val) M(val)
#define M9(val) M(val)
#endif

int				ModVal(int theAreaNum, const char* theFileName, int theInt);
double			ModVal(int theAreaNum, const char* theFileName, double theDouble);
float			ModVal(int theAreaNum, const char* theFileName, float theFloat);
const char*		ModVal(int theAreaNum, const char* theFileName, const char *theStr);
bool			ReparseModValues();
void			AddModValEnum(const std::string &theEnumName, int theVal);

}

#endif //__MODVAL_H__