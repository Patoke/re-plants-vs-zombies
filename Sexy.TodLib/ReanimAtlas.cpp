#include "Common.h"
#include "TodDebug.h"
#include "TodCommon.h"
#include "Reanimator.h"
#include "ReanimAtlas.h"
#include "graphics/VkImage.h"
#include "misc/PerfTimer.h"
//#include "graphics/MemoryImage.h"
#include <chrono>
#include <memory>

//0x470250
ReanimAtlas::ReanimAtlas()
{
	mImageCount = 0;
	//unreachable();
	mMemoryImage = nullptr;
}

void ReanimAtlas::ReanimAtlasDispose()
{
	//unreachable();
	/* TODO
	if (mMemoryImage)
	{
		delete mMemoryImage;
		mMemoryImage = nullptr;
	}
	mImageCount = 0;*/
}

ReanimAtlasImage* ReanimAtlas::GetEncodedReanimAtlas(Image* theImage)
{
	if (theImage == nullptr || (intptr_t)theImage > 1000)
		return nullptr;

	intptr_t aAtlasIndex = (intptr_t)theImage - 1;
	TOD_ASSERT(aAtlasIndex >= 0 && aAtlasIndex < mImageCount);
	return &mImageArray[aAtlasIndex];
}

//0x470290
/*
MemoryImage* ReanimAtlasMakeBlankMemoryImage(int theWidth, int theHeight)
{
	MemoryImage* aImage = new MemoryImage();

	int aBitsCount = theWidth * theHeight;
	aImage->mBits = new uint32_t[aBitsCount + 1];
	aImage->mWidth = theWidth;
	aImage->mHeight = theHeight;
	aImage->mHasTrans = true;
	aImage->mHasAlpha = true;
	memset(aImage->mBits, 0, aBitsCount * 4);
	aImage->mBits[aBitsCount] = Sexy::MEMORYCHECK_ID;
	return aImage;
}*/

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
		return (uintptr_t)&image1 > (uintptr_t)&image2;
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

	int aWidth = FloatRoundToInt(sqrt(totalArea));  // 假定为正方向区域时，正方向的边长
	return GetClosestPowerOf2Above(std::min(std::max(aWidth, aMaxWidth), 2048));  // 取“边长”和“最宽贴图的宽度”的较大值（且不超过 2048），并向上取至 2 的整数次幂
}

//0x470420
bool ReanimAtlas::ImageFits(int theImageCount, const Rect& rectTest, int theMaxWidth)
{
	if (rectTest.mX + rectTest.mWidth > theMaxWidth)
		return false;

	for (int i = 0; i < theImageCount; i++)  // 遍历贴图数组的前 theImageCount 个贴图，判断给定矩形是否与某贴图占用的区域有冲突
	{
		ReanimAtlasImage* aImage = &mImageArray[i];
		if (Rect(aImage->mX, aImage->mY, aImage->mWidth, aImage->mHeight).Inflate(1, 1).Intersects(rectTest))  // 贴图占用区域为自身区域及向外延伸 1 像素
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
		if (theToRight)  // 如果规定了居右
		{
			rectTest.mX = aImage->mX + aImage->mWidth + 1;
			rectTest.mY = aImage->mY;
		}
		else  // 否则居于下方
		{
			rectTest.mX = aImage->mX;
			rectTest.mY = aImage->mY + aImage->mHeight + 1;
		}

		if (ImageFits(theImageCount, rectTest, theMaxWidth))  // 如果图片能够放得下
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
		ImageFindPlaceOnSide(theAtlasImageToPlace, theImageCount, theMaxWidth, false);  // 分别尝试在居右和居下的位置放置图片
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
	std::sort(mImageArray, mImageArray + mImageCount, sSortByNonIncreasingHeight);  // 将所有图集图片按高度降序排序
	theAtlasWidth = PickAtlasWidth();
	theAtlasHeight = 0;

	for (int i = 0; i < mImageCount; i++)
	{
		ReanimAtlasImage* aImage = &mImageArray[i];
		PlaceAtlasImage(aImage, i, theAtlasWidth);

		/* 
			此处原为“theAtlasHeight = max(GetClosestPowerOf2Above(aImage->mY + aImage->mHeight), theAtlasHeight);”
			这样在 max 宏展开时，会导致 GetClosestPowerOf2Above(aImage->mY + aImage->mHeight) 被重复计算，故稍作修改
		*/ 

		int aImageHeight = GetClosestPowerOf2Above(aImage->mY + aImage->mHeight);
		theAtlasHeight = std::max(aImageHeight, theAtlasHeight);
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
	for (int aTrackIndex = 0; aTrackIndex < theReanimDef->mTracks.count; aTrackIndex++)
	{
		ReanimatorTrack* aTrack = &theReanimDef->mTracks.tracks[aTrackIndex];
		for (int aKeyIndex = 0; aKeyIndex < aTrack->mTransforms.count; aKeyIndex++)  // 遍历每一帧上的贴图
		{
			Image* aImage = aTrack->mTransforms.mTransforms[aKeyIndex].mImage;
			// 如果存在贴图，且贴图的宽、高均不大于 254 像素，且相同的贴图未加入至图集图片数组中
			if (aImage != nullptr && aImage->mWidth <= 254 && aImage->mHeight <= 254 && FindImage(aImage) < 0)
				AddImage(aImage);  // 先将其加入数组中，后续再确定其位于图集中的位置
		}
	}

	int aAtlasWidth, aAtlasHeight;
	ArrangeImages(aAtlasWidth, aAtlasHeight);

	for (int aTrackIndex = 0; aTrackIndex < theReanimDef->mTracks.count; aTrackIndex++)
	{
		ReanimatorTrack* aTrack = &theReanimDef->mTracks.tracks[aTrackIndex];
		for (int aKeyIndex = 0; aKeyIndex < aTrack->mTransforms.count; aKeyIndex++)  // 遍历每一帧上的贴图
		{
			Image*& aImage = aTrack->mTransforms.mTransforms[aKeyIndex].mImage;
			if (aImage != nullptr && aImage->mWidth <= 254 && aImage->mHeight <= 254)
			{
				intptr_t aImageIndex = FindImage(aImage);
				TOD_ASSERT(aImageIndex >= 0);
				aImage = (Image*)(aImageIndex + 1);  // ★ 将图片在数组中的序号作为 Image* 修改动画定义
			}
		}
	}

	//mMemoryImage = ReanimAtlasMakeBlankMemoryImage(aAtlasWidth, aAtlasHeight);
	if (aAtlasWidth <= 0 || aAtlasHeight <= 0) {
		return; // Can't make images of zero size.
	}

	mMemoryImage = std::make_unique<Vk::VkImage>(aAtlasWidth, aAtlasHeight);
	Graphics aMemoryGraphis(mMemoryImage.get());
	for (int aImageIndex = 0; aImageIndex < mImageCount; aImageIndex++)
	{
		ReanimAtlasImage* aImage = &mImageArray[aImageIndex];
		if (!aImage->mOriginalImage->mWidth || !aImage->mOriginalImage->mHeight) continue;
		aMemoryGraphis.DrawImage(aImage->mOriginalImage, aImage->mX, aImage->mY);  // 将原贴图绘制在图集上
	}
	FixPixelsOnAlphaEdgeForBlending(mMemoryImage.get());  // 将所有透明像素的颜色修正为其周围像素颜色的平均值
}
