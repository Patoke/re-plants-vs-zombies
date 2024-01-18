#include <memory>
#define XMD_H

#include "ImageLib.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <math.h>
#include "paklib/PakInterface.h"
#include "Common.h"


using namespace ImageLib;


std::unique_ptr<Image> GetImageWithSDL(const std::string& theFileName)
{
	auto fp = fopen(theFileName.c_str(), "rb");
	if (fp == NULL)
		return NULL;

	SDL_Surface* aSurface = IMG_Load(theFileName.c_str());
	fclose(fp);
	
	if (aSurface == NULL)
		return NULL;

	auto aSurface32 = SDL_ConvertSurfaceFormat(aSurface, SDL_PIXELFORMAT_ARGB8888, 0);
	SDL_FreeSurface(aSurface);

	if (aSurface32 == NULL)
		return NULL;

	auto anImage = std::make_unique<Image>(aSurface32->w, aSurface32->h);

	auto bufferSize = aSurface32->w * aSurface32->h;

	// Copy the pixels
	SDL_memcpy(anImage->mBits.get(), aSurface32->pixels, bufferSize * sizeof(uint32_t));

	SDL_FreeSurface(aSurface32);

	return std::move(anImage);
}



bool ImageLib::WriteJPEGImage(const std::string& theFileName, Image* theImage)
{
	auto aSurface = SDL_CreateRGBSurfaceFrom(theImage->mBits.get(), theImage->mWidth, theImage->mHeight, 32, theImage->mWidth * 4, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (aSurface == NULL)
		return false;
	auto aResult = IMG_SaveJPG(aSurface, theFileName.c_str(), 80);
	if (aResult != 0)
		return false;
	return true;
}

bool ImageLib::WritePNGImage(const std::string& theFileName, Image* theImage)
{
	auto aSurface = SDL_CreateRGBSurfaceFrom(theImage->mBits.get(), theImage->mWidth, theImage->mHeight, 32, theImage->mWidth * 4, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (aSurface == NULL)
		return false;
	auto aResult = IMG_SavePNG(aSurface, theFileName.c_str());
	if (aResult != 0)
		return false;
	return true;
}

bool ImageLib::WriteTGAImage(const std::string& theFileName, Image* theImage)
{
	FILE* aTGAFile = fopen(theFileName.c_str(), "wb");
	if (aTGAFile == NULL)
		return false;

	unsigned char aHeaderIDLen = 0;
	fwrite(&aHeaderIDLen, sizeof(unsigned char), 1, aTGAFile);

	unsigned char aColorMapType = 0;
	fwrite(&aColorMapType, sizeof(unsigned char), 1, aTGAFile);
	
	unsigned char anImageType = 2;
	fwrite(&anImageType, sizeof(unsigned char), 1, aTGAFile);

	uint16_t aFirstEntryIdx = 0;
	fwrite(&aFirstEntryIdx, sizeof(uint16_t), 1, aTGAFile);

	uint16_t aColorMapLen = 0;
	fwrite(&aColorMapLen, sizeof(uint16_t), 1, aTGAFile);

	unsigned char aColorMapEntrySize = 0;
	fwrite(&aColorMapEntrySize, sizeof(unsigned char), 1, aTGAFile); 

	uint16_t anXOrigin = 0;
	fwrite(&anXOrigin, sizeof(uint16_t), 1, aTGAFile);

	uint16_t aYOrigin = 0;
	fwrite(&aYOrigin, sizeof(uint16_t), 1, aTGAFile);

	uint16_t anImageWidth = theImage->mWidth;
	fwrite(&anImageWidth, sizeof(uint16_t), 1, aTGAFile); 

	uint16_t anImageHeight = theImage->mHeight;
	fwrite(&anImageHeight, sizeof(uint16_t), 1, aTGAFile);  

	unsigned char aBitCount = 32;
	fwrite(&aBitCount, sizeof(unsigned char), 1, aTGAFile);  

	unsigned char anImageDescriptor = 8 | (1<<5);
	fwrite(&anImageDescriptor, sizeof(unsigned char), 1, aTGAFile);

	fwrite(theImage->mBits.get(), 4, theImage->mWidth*theImage->mHeight, aTGAFile);

	fclose(aTGAFile);

	return true;
}

typedef struct tagBITMAPFILEHEADER {
	uint16_t  bfType;
	unsigned int bfSize;
	uint16_t  bfReserved1;
	uint16_t  bfReserved2;
	unsigned int bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	unsigned int biSize;
	int  biWidth;
	int  biHeight;
	uint16_t  biPlanes;
	uint16_t  biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	int  biXPelsPerMeter;
	int  biYPelsPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef  enum {
	 BI_RGB = 0x0000,
	 BI_RLE8 = 0x0001,
	 BI_RLE4 = 0x0002,
	 BI_BITFIELDS = 0x0003,
	 BI_JPEG = 0x0004,
	 BI_PNG = 0x0005,
	 BI_CMYK = 0x000B,
	 BI_CMYKRLE8 = 0x000C,
	 BI_CMYKRLE4 = 0x000D
} Compression;

bool ImageLib::WriteBMPImage(const std::string& theFileName, Image* theImage)
{
	FILE* aFile = fopen(theFileName.c_str(), "wb");
	if (aFile == NULL)
		return false;

	BITMAPFILEHEADER aFileHeader;
	BITMAPINFOHEADER aHeader;

	memset(&aFileHeader,0,sizeof(aFileHeader));
	memset(&aHeader,0,sizeof(aHeader));

	int aNumBytes = theImage->mWidth*theImage->mHeight*4;

	aFileHeader.bfType = ('M'<<8) | 'B';
	aFileHeader.bfSize = sizeof(aFileHeader) + sizeof(aHeader) + aNumBytes;
	aFileHeader.bfOffBits = sizeof(aHeader); 

	aHeader.biSize = sizeof(aHeader);
	aHeader.biWidth = theImage->mWidth;
	aHeader.biHeight = theImage->mHeight;
	aHeader.biPlanes = 1;
	aHeader.biBitCount = 32;
	aHeader.biCompression = BI_RGB;

	fwrite(&aFileHeader,sizeof(aFileHeader),1,aFile);
	fwrite(&aHeader,sizeof(aHeader),1,aFile);
	unsigned int *aRow = theImage->mBits.get() + (theImage->mHeight-1)*theImage->mWidth;
	int aRowSize = theImage->mWidth*4;
	(void)aRowSize; // Unused
	for (int i=0; i<theImage->mHeight; i++, aRow-=theImage->mWidth)
		fwrite(aRow,4,theImage->mWidth,aFile);

	fclose(aFile);
	return true;
}


int ImageLib::gAlphaComposeColor = 0xFFFFFF;
bool ImageLib::gAutoLoadAlpha = true;
bool ImageLib::gIgnoreJPEG2000Alpha = true;

uint32_t AverageNearByPixels(ImageLib::Image* theImage, uint32_t* thePixel, int x, int y)
{
    int aRed = 0;
    int aGreen = 0;
    int aBlue = 0;
    int aBitsCount = 0;

    for (int i = -1; i <= 1; i++)  // 依次循环上方、当前、下方的一行
    {
        if (i == 0)  // 排除当前行
        {
            continue;
        }

        for (int j = -1; j <= 1; j++)  // 依次循环左方、当前、右方的一列
        {
            if ((x != 0 || j != -1) && (x != theImage->mWidth - 1 || j != 1) && (y != 0 || i != -1) && (y != theImage->mHeight - 1 || i != 1))
            {
                unsigned int aPixel = *(thePixel + i * theImage->mWidth + j);
                if (aPixel & 0xFF000000UL)  // 如果不是透明像素
                {
                    aRed += (aPixel >> 16) & 0x000000FFUL;
                    aGreen += (aPixel >> 8) & 0x000000FFUL;
                    aBlue += aPixel & 0x000000FFUL;
                    aBitsCount++;
                }
            }
        }
    }

    if (aBitsCount == 0)
        return 0;

    aRed /= aBitsCount;
    aRed = std::min(aRed, 255);
    aGreen /= aBitsCount;
    aGreen = std::min(aGreen, 255);
    aBlue /= aBitsCount;
    aBlue = std::min(aBlue, 255);
    return (aRed << 16) | (aGreen << 8) | (aBlue);
}

std::unique_ptr<Image> ImageLib::GetImage(const std::string& theFilename, bool lookForAlphaImage, bool theDoImageSanding)
{
	if (!gAutoLoadAlpha)
		lookForAlphaImage = false;

	int aLastDotPos = theFilename.rfind('.');
	int aLastSlashPos = std::max((int)theFilename.rfind('\\'), (int)theFilename.rfind('/'));

	std::string anExt;
	std::string aFilename;

	if (aLastDotPos > aLastSlashPos)
	{
		anExt = theFilename.substr(aLastDotPos, theFilename.length() - aLastDotPos);
		aFilename = theFilename.substr(0, aLastDotPos);
	}
	else
		aFilename = theFilename;

	std::unique_ptr<Image> anImage = nullptr;

	if ((anImage == NULL) && ((stricmp(anExt.c_str(), ".tga") == 0) || (anExt.length() == 0)))
		anImage = GetImageWithSDL(aFilename + ".tga");

	if ((anImage == NULL) && ((stricmp(anExt.c_str(), ".jpg") == 0) || (anExt.length() == 0)))
		anImage = GetImageWithSDL(aFilename + ".jpg");

	if ((anImage == NULL) && ((stricmp(anExt.c_str(), ".png") == 0) || (anExt.length() == 0)))
		anImage = GetImageWithSDL(aFilename + ".png");

	if ((anImage == NULL) && ((stricmp(anExt.c_str(), ".gif") == 0) || (anExt.length() == 0)))
		anImage = GetImageWithSDL(aFilename + ".gif");

	if ((anImage == NULL) && (stricmp(anExt.c_str(), ".j2k") == 0))
		anImage = GetImageWithSDL(aFilename + ".j2k");
	if ((anImage == NULL) && (stricmp(anExt.c_str(), ".jp2") == 0))
		anImage = GetImageWithSDL(aFilename + ".jp2");


	// Check for alpha images
	std::unique_ptr<Image> anAlphaImage = NULL;
	if(lookForAlphaImage)
	{
		// Check _ImageName
		anAlphaImage = GetImage(theFilename.substr(0, aLastSlashPos+1) + "_" +
			theFilename.substr(aLastSlashPos+1, theFilename.length() - aLastSlashPos - 1), false);

		// Check ImageName_
		if(anAlphaImage == NULL)
			anAlphaImage = GetImage(theFilename + "_", false);
	}

	// Compose alpha channel with image
	if (anAlphaImage != NULL) 
	{
		if (anImage != NULL)
		{
			if ((anImage->mWidth == anAlphaImage->mWidth) &&
				(anImage->mHeight == anAlphaImage->mHeight))
			{
				uint32_t* aBits1 = anImage->mBits.get();
				uint32_t* aBits2 = anAlphaImage->mBits.get();
				int aSize = anImage->mWidth*anImage->mHeight;

				for (int i = 0; i < aSize; i++)
				{
					*aBits1 = (*aBits1 & 0x00FFFFFF) | ((*aBits2 & 0xFF) << 24);
					++aBits1;
					++aBits2;
				}
			}
		}
		else if (gAlphaComposeColor==0xFFFFFF)
		{
			anImage = std::move(anAlphaImage);

			uint32_t* aBits1 = anImage->mBits.get();

			int aSize = anImage->mWidth*anImage->mHeight;
			for (int i = 0; i < aSize; i++)
			{
				*aBits1 = (0x00FFFFFF) | ((*aBits1 & 0xFF) << 24);
				++aBits1;
			}
		}
		else
		{
			const int aColor = gAlphaComposeColor;
			anImage = std::move(anAlphaImage);

			uint32_t* aBits1 = anImage->mBits.get();

			int aSize = anImage->mWidth*anImage->mHeight;
			for (int i = 0; i < aSize; i++)
			{
				*aBits1 = aColor | ((*aBits1 & 0xFF) << 24);
				++aBits1;
			}
		}
	}

    // Image Sanding. We have to move it here to avoid moving the image
    // between the CPU and GPU constantly. Sand once before uploading to
    // the GPU and everyone is happy.
	if (theDoImageSanding) {
		uint32_t* aBitsPtr = anImage->mBits.get();
		for (int y = 0; y < anImage->mHeight; y++)
		{
			for (int x = 0; x < anImage->mWidth; x++)
			{
				if ((*aBitsPtr & 0xFF000000UL) == 0)  // 如果像素的不透明度为 0
				{
					*aBitsPtr = AverageNearByPixels(anImage.get(), aBitsPtr, x, y);  // 计算该点周围非透明像素的平均颜色
				}
	
				aBitsPtr++;
			}
		}
	}

	return anImage;
}
