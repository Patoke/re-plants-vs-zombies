#include "Image.h"
#include "Graphics.h"

using namespace Sexy;

Image::Image()
{
	mWidth = 0;
	mHeight = 0;

	mNumRows = 1;
	mNumCols = 1;

	mAnimInfo = NULL;
	mDrawn = false;
}

Image::Image(const Image& theImage) :
	mWidth(theImage.mWidth),
	mHeight(theImage.mHeight),
	mNumRows(theImage.mNumRows),
	mNumCols(theImage.mNumCols)
{
	mDrawn = false;
	if (theImage.mAnimInfo != NULL)
		mAnimInfo = new AnimInfo(*theImage.mAnimInfo);
	else
		mAnimInfo = NULL;
}

Image::~Image()
{
	delete mAnimInfo;
}

int Image::GetWidth()
{
	return mWidth;
}

int	Image::GetHeight()
{
	return mHeight;
}

int Image::GetCelHeight()
{
	return mHeight / mNumRows;
}

int Image::GetCelWidth()
{
	return mWidth / mNumCols;
}

Rect Image::GetCelRect(int theCel)
{
	int h = GetCelHeight();
	int w = GetCelWidth();
	int x = (theCel % mNumCols) * w;
	int y = (theCel / mNumCols) * h;

	return Rect(x, y, w, h);
}

Rect Image::GetCelRect(int theCol, int theRow)
{
	int h = GetCelHeight();
	int w = GetCelWidth();
	int x = theCol * w;
	int y = theRow * h;

	return Rect(x, y, w, h);
}

AnimInfo::AnimInfo()
{
	mAnimType = AnimType_None;
	mFrameDelay = 1;
	mNumCels = 1;
}

void AnimInfo::SetPerFrameDelay(int theFrame, int theTime)
{
	if ((int)mPerFrameDelay.size()<=theFrame)
		mPerFrameDelay.resize(theFrame+1);

	mPerFrameDelay[theFrame] = theTime;
}

void AnimInfo::Compute(int theNumCels, int theBeginFrameTime, int theEndFrameTime)
{
	int i;

	mNumCels = theNumCels;
	if (mNumCels<=0)
		mNumCels = 1;

	if (mFrameDelay<=0)
		mFrameDelay = 1;

	if (mAnimType==AnimType_PingPong && mNumCels>1)
	{
		mFrameMap.resize(theNumCels*2-2);
		int index = 0;
		for (i=0; i<theNumCels; i++)
			mFrameMap[index++] = i;
		for (i=theNumCels-2; i>=1; i--)
			mFrameMap[index++] = i;
	}

	if (!mFrameMap.empty())
		mNumCels = (int)mFrameMap.size();

	if (theBeginFrameTime>0) 
		SetPerFrameDelay(0,theBeginFrameTime);

	if (theEndFrameTime>0)
		SetPerFrameDelay(mNumCels-1,theEndFrameTime);

	if (!mPerFrameDelay.empty())
	{
		mTotalAnimTime = 0;
		mPerFrameDelay.resize(mNumCels);

		for (i=0; i<mNumCels; i++)
		{
			if (mPerFrameDelay[i]<=0)
				mPerFrameDelay[i] = mFrameDelay;
				
			mTotalAnimTime += mPerFrameDelay[i];
		}
	}
	else
		mTotalAnimTime = mFrameDelay*mNumCels;

	if (!mFrameMap.empty())
		mFrameMap.resize(mNumCels);
}
	
int AnimInfo::GetPerFrameCel(int theTime)
{
	for (int i=0; i<mNumCels; i++)
	{
		theTime -= mPerFrameDelay[i];
		if (theTime<0)
			return i;
	}

	return mNumCels-1;
}

int AnimInfo::GetCel(int theTime)
{
	if (mAnimType==AnimType_Once && theTime>=mTotalAnimTime)
	{
		if (!mFrameMap.empty())
			return mFrameMap[mFrameMap.size()-1];
		else
			return mNumCels-1;
	}

	theTime = theTime%mTotalAnimTime;

	int aFrame;
	if (!mPerFrameDelay.empty())
		aFrame = GetPerFrameCel(theTime);
	else
		aFrame = (theTime/mFrameDelay)%mNumCels;

	if (mFrameMap.empty())
		return aFrame;
	else
		return mFrameMap[aFrame];	
}

int	Image::GetAnimCel(int theTime)
{
	if (mAnimInfo==NULL)
		return 0;
	else
		return mAnimInfo->GetCel(theTime);
}

Rect Image::GetAnimCelRect(int theTime)
{
	Rect aRect;
	int aCel = GetAnimCel(theTime);
	int aCelWidth = GetCelWidth();
	int aCelHeight = GetCelHeight();
	if (mNumCols>1)
		return Rect(aCel*aCelWidth,0,aCelWidth,mHeight);
	else
		return Rect(0,aCel*aCelHeight,mWidth,aCelHeight);
}

void Image::CopyAttributes(Image *from)
{
	mNumCols = from->mNumCols;
	mNumRows = from->mNumRows;
	delete mAnimInfo;
	mAnimInfo = NULL;
	if (from->mAnimInfo != NULL)
		mAnimInfo = new AnimInfo(*from->mAnimInfo);
}

Graphics* Image::GetGraphics()
{
	Graphics* g = new Graphics(this);	

	return g;
}

void Image::DrawRect(const Rect& theRect, const Color& theColor, int theDrawMode)
{
	FillRect(Rect(theRect.mX, theRect.mY, theRect.mWidth + 1, 1), theColor, theDrawMode);
	FillRect(Rect(theRect.mX, theRect.mY + theRect.mHeight, theRect.mWidth + 1, 1), theColor, theDrawMode);
	FillRect(Rect(theRect.mX, theRect.mY + 1, 1, theRect.mHeight - 1), theColor, theDrawMode);
	FillRect(Rect(theRect.mX + theRect.mWidth, theRect.mY + 1, 1, theRect.mHeight - 1), theColor, theDrawMode);
}

void Image::FillScanLines(Span* theSpans, int theSpanCount, const Color& theColor, int theDrawMode)
{
	for (int i = 0; i < theSpanCount; i++)
	{
		Span* aSpan = &theSpans[i];		
		FillRect(Rect(aSpan->mX, aSpan->mY, aSpan->mWidth, 1), theColor, theDrawMode);		
	}
}
