#include "Insets.h"

using namespace Sexy;

Insets::Insets() :
	mLeft(0),
	mTop(0),
	mRight(0),
	mBottom(0)
{
		/*int a = 0; what the fuck?*/
}

Insets::Insets(int theLeft, int theTop, int theRight, int theBottom) :
	mLeft(theLeft),
	mTop(theTop),
	mRight(theRight),
	mBottom(theBottom)
{
}

Insets::Insets(const Insets& theInsets) :
	mLeft(theInsets.mLeft),
	mTop(theInsets.mTop),
	mRight(theInsets.mRight),
	mBottom(theInsets.mBottom)
{
}

