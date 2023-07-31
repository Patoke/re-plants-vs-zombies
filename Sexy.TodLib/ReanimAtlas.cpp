#include "TodDebug.h"
#include "TodCommon.h"
#include "Reanimator.h"
#include "ReanimAtlas.h"
#include "../SexyAppFramework/PerfTimer.h"
#include "../SexyAppFramework/MemoryImage.h"

//0x470250
ReanimAtlas::ReanimAtlas()
{
	mImageCount = 0;
	mMemoryImage = nullptr;
}

void ReanimAtlas::ReanimAtlasDispose()
{
	if (mMemoryImage)
	{
		delete mMemoryImage;
		mMemoryImage = nullptr;
	}
	mImageCount = 0;
}

ReanimAtlasImage* ReanimAtlas::GetEncodedReanimAtlas(Image* theImage)
{
	if (theImage == nullptr || (int)theImage > 1000)
		return nullptr;

	int aAtlasIndex = (int)theImage - 1;
	TOD_ASSERT(aAtlasIndex >= 0 && aAtlasIndex < mImageCount);
	return &mImageArray[aAtlasIndex];
}

//0x470290
MemoryImage* ReanimAtlasMakeBlankMemoryImage(int theWidth, int theHeight)
{
	MemoryImage* aImage = new MemoryImage();

	int aBitsCount = theWidth * theHeight;
	aImage->mBits = new unsigned long[aBitsCount + 1];
	aImage->mWidth = theWidth;
	aImage->mHeight = theHeight;
	aImage->mHasTrans = true;
	aImage->mHasAlpha = true;
	memset(aImage->mBits, 0, aBitsCount * 4);
	aImage->mBits[aBitsCount] = Sexy::MEMORYCHECK_ID;
	return aImage;
}

//0x470340
bool sSortByNonIncreasingHeight(const ReanimAtlasImage& image1, const ReanimAtlasImage& image2)
{
	//if (image1->mHeight != image2->mHeight)
	//	return image1->mHeight > image2->mHeight;
	//else if (image1->mWidth != image2->mWidth)
	//	return image1->mWidth > image2->mWidth;
	//else
	//	return (unsigned int)image1 > (unsigned int)image2;

	if (image1.mHeight != image2.mHeight)
		return image1.mHeight > image2.mHeight;
	else if (image1.mWidth != image2.mWidth)
		return image1.mWidth > image2.mWidth;
	else
		return (unsigned int)&image1 > (unsigned int)&image2;
}

static int GetClosestPowerOf2Above(int theNum)
{
	int aPower2 = 1;
	while (aPower2 < theNum)
		aPower2 <<= 1;

	return aPower2;
}

//0x470370
int ReanimAtlas::PickAtlasWidth()
{
	int totalArea = 0;
	int aMaxWidth = 0;
	for (int i = 0; i < mImageCount; i++)
	{
		ReanimAtlasImage* aImage = &mImageArray[i];
		totalArea += aImage->mWidth * aImage->mHeight;
		if (aMaxWidth <= aImage->mWidth + 2)
			aMaxWidth = aImage->mWidth + 2;
	}

	int aWidth = FloatRoundToInt(sqrt(totalArea));  // �ٶ�Ϊ����������ʱ��������ı߳�
	return GetClosestPowerOf2Above(min(max(aWidth, aMaxWidth), 2048));  // ȡ���߳����͡������ͼ�Ŀ�ȡ��Ľϴ�ֵ���Ҳ����� 2048����������ȡ�� 2 ����������
}

//0x470420
bool ReanimAtlas::ImageFits(int theImageCount, const Rect& rectTest, int theMaxWidth)
{
	if (rectTest.mX + rectTest.mWidth > theMaxWidth)
		return false;

	for (int i = 0; i < theImageCount; i++)  // ������ͼ�����ǰ theImageCount ����ͼ���жϸ��������Ƿ���ĳ��ͼռ�õ������г�ͻ
	{
		ReanimAtlasImage* aImage = &mImageArray[i];
		if (Rect(aImage->mX, aImage->mY, aImage->mWidth, aImage->mHeight).Inflate(1, 1).Intersects(rectTest))  // ��ͼռ������Ϊ���������������� 1 ����
			return false;
	}
	return true;
}

//0x4704C0
bool ReanimAtlas::ImageFindPlaceOnSide(ReanimAtlasImage* theAtlasImageToPlace, int theImageCount, int theMaxWidth, bool theToRight)
{
	Rect rectTest;
	rectTest.mWidth = theAtlasImageToPlace->mWidth + 2;
	rectTest.mHeight = theAtlasImageToPlace->mHeight + 2;

	for (int i = 0; i < theImageCount; i++)
	{
		ReanimAtlasImage* aImage = &mImageArray[i];
		if (theToRight)  // ����涨�˾���
		{
			rectTest.mX = aImage->mX + aImage->mWidth + 1;
			rectTest.mY = aImage->mY;
		}
		else  // ��������·�
		{
			rectTest.mX = aImage->mX;
			rectTest.mY = aImage->mY + aImage->mHeight + 1;
		}

		if (ImageFits(theImageCount, rectTest, theMaxWidth))  // ���ͼƬ�ܹ��ŵ���
		{
			theAtlasImageToPlace->mX = rectTest.mX;
			theAtlasImageToPlace->mY = rectTest.mY;
			if (theToRight)
				theAtlasImageToPlace->mX += 1;
			else
				theAtlasImageToPlace->mY += 1;

			return true;
		}
	}

	return false;
}

bool ReanimAtlas::ImageFindPlace(ReanimAtlasImage* theAtlasImageToPlace, int theImageCount, int theMaxWidth)
{
	return 
		ImageFindPlaceOnSide(theAtlasImageToPlace, theImageCount, theMaxWidth, true) || 
		ImageFindPlaceOnSide(theAtlasImageToPlace, theImageCount, theMaxWidth, false);  // �ֱ����ھ��Һ;��µ�λ�÷���ͼƬ
}

bool ReanimAtlas::PlaceAtlasImage(ReanimAtlasImage* theAtlasImageToPlace, int theImageCount, int theMaxWidth)
{
	if (theImageCount == 0)
	{
		theAtlasImageToPlace->mX = 1;
		theAtlasImageToPlace->mY = 1;
		return true;
	}

	if (ImageFindPlace(theAtlasImageToPlace, theImageCount, theMaxWidth))
		return true;

	TOD_ASSERT();
	return false;
}

//0x470580
void ReanimAtlas::ArrangeImages(int& theAtlasWidth, int& theAtlasHeight)
{
	std::sort(mImageArray, mImageArray + mImageCount, sSortByNonIncreasingHeight);  // ������ͼ��ͼƬ���߶Ƚ�������
	theAtlasWidth = PickAtlasWidth();
	theAtlasHeight = 0;

	for (int i = 0; i < mImageCount; i++)
	{
		ReanimAtlasImage* aImage = &mImageArray[i];
		PlaceAtlasImage(aImage, i, theAtlasWidth);

		/* 
			�˴�ԭΪ��theAtlasHeight = max(GetClosestPowerOf2Above(aImage->mY + aImage->mHeight), theAtlasHeight);��
			������ max ��չ��ʱ���ᵼ�� GetClosestPowerOf2Above(aImage->mY + aImage->mHeight) ���ظ����㣬�������޸�
		*/ 

		int aImageHeight = GetClosestPowerOf2Above(aImage->mY + aImage->mHeight);
		theAtlasHeight = max(aImageHeight, theAtlasHeight);
	}
}

void ReanimAtlas::AddImage(Image* theImage)
{
	if (theImage->mNumCols == 1 && theImage->mNumRows == 1)
	{
		TOD_ASSERT(mImageCount < MAX_REANIM_IMAGES);

		ReanimAtlasImage* aImage = &mImageArray[mImageCount++];
		aImage->mHeight = theImage->mHeight;
		aImage->mWidth = theImage->mWidth;
		aImage->mOriginalImage = theImage;
	}
}

int ReanimAtlas::FindImage(Image* theImage)
{
	for (int i = 0; i < mImageCount; i++)
		if (mImageArray[i].mOriginalImage == theImage)
			return i;

	return -1;
}

//0x470680
void ReanimAtlas::ReanimAtlasCreate(ReanimatorDefinition* theReanimDef)
{
	PerfTimer aTimer;
	aTimer.Start();

	for (int aTrackIndex = 0; aTrackIndex < theReanimDef->mTrackCount; aTrackIndex++)
	{
		ReanimatorTrack* aTrack = &theReanimDef->mTracks[aTrackIndex];
		for (int aKeyIndex = 0; aKeyIndex < aTrack->mTransformCount; aKeyIndex++)  // ����ÿһ֡�ϵ���ͼ
		{
			Image* aImage = aTrack->mTransforms[aKeyIndex].mImage;
			// ���������ͼ������ͼ�Ŀ��߾������� 254 ���أ�����ͬ����ͼδ������ͼ��ͼƬ������
			if (aImage != nullptr && aImage->mWidth <= 254 && aImage->mHeight <= 254 && FindImage(aImage) < 0)
				AddImage(aImage);  // �Ƚ�����������У�������ȷ����λ��ͼ���е�λ��
		}
	}

	int aAtlasWidth, aAtlasHeight;
	ArrangeImages(aAtlasWidth, aAtlasHeight);

	for (int aTrackIndex = 0; aTrackIndex < theReanimDef->mTrackCount; aTrackIndex++)
	{
		ReanimatorTrack* aTrack = &theReanimDef->mTracks[aTrackIndex];
		for (int aKeyIndex = 0; aKeyIndex < aTrack->mTransformCount; aKeyIndex++)  // ����ÿһ֡�ϵ���ͼ
		{
			Image*& aImage = aTrack->mTransforms[aKeyIndex].mImage;
			if (aImage != nullptr && aImage->mWidth <= 254 && aImage->mHeight <= 254)
			{
				int aImageIndex = FindImage(aImage);
				TOD_ASSERT(aImageIndex >= 0);
				aImage = (Image*)(aImageIndex + 1);  // �� ��ͼƬ�������е������Ϊ Image* �޸Ķ�������
			}
		}
	}

	mMemoryImage = ReanimAtlasMakeBlankMemoryImage(aAtlasWidth, aAtlasHeight);
	Graphics aMemoryGraphis(mMemoryImage);
	for (int aImageIndex = 0; aImageIndex < mImageCount; aImageIndex++)
	{
		ReanimAtlasImage* aImage = &mImageArray[aImageIndex];
		aMemoryGraphis.DrawImage(aImage->mOriginalImage, aImage->mX, aImage->mY);  // ��ԭ��ͼ������ͼ����
	}
	FixPixelsOnAlphaEdgeForBlending(mMemoryImage);  // ������͸�����ص���ɫ����Ϊ����Χ������ɫ��ƽ��ֵ
}
