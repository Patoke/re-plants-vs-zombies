
#include "Ratio.h"

namespace Sexy
{

	Ratio::Ratio()
		: mNumerator(1)
		, mDenominator(1)
	{
	}

	Ratio::Ratio(int theNumerator, int theDenominator)
	{
		Set(theNumerator, theDenominator);
	}

	void Ratio::Set(int theNumerator, int theDenominator)
	{
		// find the greatest-common-denominator of theNumerator and theDenominator.
		int t;
		int a = theNumerator;
		int b = theDenominator;
		while (b != 0)
		{
			t = b;
			b = a % b;
			a = t;
		}

		// divide by the g-c-d to reduce mNumerator/mDenominator to lowest terms.
		mNumerator = theNumerator/a;
		mDenominator = theDenominator/a;
	}

}
