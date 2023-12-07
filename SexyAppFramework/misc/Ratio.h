#ifndef __RATIO_H__
#define __RATIO_H__

namespace Sexy
{
	struct Ratio
	{
		Ratio();
		Ratio(int theNumerator, int theDenominator);
		void Set(int theNumerator, int theDenominator);
		bool operator==(const Ratio& theRatio) const;
		bool operator!=(const Ratio& theRatio) const;
		bool operator<(const Ratio& theRatio) const;
		int operator*(int theInt) const;
		int operator/(int theInt) const;
		int mNumerator;
		int mDenominator;
	};

	inline bool Ratio::operator==(const Ratio& theRatio) const
	{
		return mNumerator == theRatio.mNumerator && mDenominator == theRatio.mDenominator;
	}

	inline bool Ratio::operator!=(const Ratio& theRatio) const
	{
		return ! (*this == theRatio);
	}

	inline bool Ratio::operator<(const Ratio& theRatio) const
	{
		return (mNumerator*theRatio.mDenominator/mDenominator < theRatio.mNumerator)
			|| (mNumerator < theRatio.mNumerator*mDenominator/theRatio.mDenominator);
	}

	inline int Ratio::operator*(int theInt) const
	{
		return theInt * mNumerator / mDenominator;
	}

	inline int Ratio::operator/(int theInt) const
	{
		return theInt * mDenominator / mNumerator;
	}

	inline int operator*(int theInt, const Ratio& theRatio)
	{
		return theInt * theRatio.mNumerator / theRatio.mDenominator;
	}

	inline int operator/(int theInt, const Ratio& theRatio)
	{
		return theInt * theRatio.mDenominator / theRatio.mNumerator;
	}

}

#endif
