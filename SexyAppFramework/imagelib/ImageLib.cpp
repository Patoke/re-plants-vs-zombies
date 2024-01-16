#define XMD_H

#define NOMINMAX 1
#include <windows.h>
#include "ImageLib.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <math.h>
#include <tchar.h>
#include "paklib/PakInterface.h"
#include "Common.h"


using namespace ImageLib;

Image::Image()
{
	mWidth = 0;
	mHeight = 0;
	mBits = NULL;
}

Image::~Image()
{
	delete mBits;
}

int	Image::GetWidth()
{
	return mWidth;
}

int	Image::GetHeight()
{
	return mHeight;
}

unsigned long* Image::GetBits()
{
	return mBits;
}


Image* GetImageWithSDL(const std::string& theFileName)
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

	Image* anImage = new Image();
	anImage->mWidth = aSurface32->w;
	anImage->mHeight = aSurface32->h;

	auto bufferSize = aSurface32->w * aSurface32->h;
	anImage->mBits = new unsigned long[bufferSize];

	// Copy the pixels
	SDL_memcpy((char*)anImage->mBits, (char*)aSurface32->pixels, bufferSize * sizeof(unsigned long));

	SDL_FreeSurface(aSurface32);

	return anImage;
}



bool ImageLib::WriteJPEGImage(const std::string& theFileName, Image* theImage)
{
	auto aSurface = SDL_CreateRGBSurfaceFrom(theImage->mBits, theImage->mWidth, theImage->mHeight, 32, theImage->mWidth * 4, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (aSurface == NULL)
		return false;
	auto aResult = IMG_SaveJPG(aSurface, theFileName.c_str(), 80);
	if (aResult != 0)
		return false;
	return true;
}

bool ImageLib::WritePNGImage(const std::string& theFileName, Image* theImage)
{
	auto aSurface = SDL_CreateRGBSurfaceFrom(theImage->mBits, theImage->mWidth, theImage->mHeight, 32, theImage->mWidth * 4, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
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

	BYTE aHeaderIDLen = 0;
	fwrite(&aHeaderIDLen, sizeof(BYTE), 1, aTGAFile);

	BYTE aColorMapType = 0;
	fwrite(&aColorMapType, sizeof(BYTE), 1, aTGAFile);
	
	BYTE anImageType = 2;
	fwrite(&anImageType, sizeof(BYTE), 1, aTGAFile);

	WORD aFirstEntryIdx = 0;
	fwrite(&aFirstEntryIdx, sizeof(WORD), 1, aTGAFile);

	WORD aColorMapLen = 0;
	fwrite(&aColorMapLen, sizeof(WORD), 1, aTGAFile);

	BYTE aColorMapEntrySize = 0;
	fwrite(&aColorMapEntrySize, sizeof(BYTE), 1, aTGAFile);	

	WORD anXOrigin = 0;
	fwrite(&anXOrigin, sizeof(WORD), 1, aTGAFile);

	WORD aYOrigin = 0;
	fwrite(&aYOrigin, sizeof(WORD), 1, aTGAFile);

	WORD anImageWidth = theImage->mWidth;
	fwrite(&anImageWidth, sizeof(WORD), 1, aTGAFile);	

	WORD anImageHeight = theImage->mHeight;
	fwrite(&anImageHeight, sizeof(WORD), 1, aTGAFile);	

	BYTE aBitCount = 32;
	fwrite(&aBitCount, sizeof(BYTE), 1, aTGAFile);	

	BYTE anImageDescriptor = 8 | (1<<5);
	fwrite(&anImageDescriptor, sizeof(BYTE), 1, aTGAFile);

	fwrite(theImage->mBits, 4, theImage->mWidth*theImage->mHeight, aTGAFile);

	fclose(aTGAFile);

	return true;
}

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
	DWORD *aRow = theImage->mBits + (theImage->mHeight-1)*theImage->mWidth;
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

Image* ImageLib::GetImage(const std::string& theFilename, bool lookForAlphaImage)
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

	Image* anImage = NULL;

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
	Image* anAlphaImage = NULL;
	if(lookForAlphaImage)
	{
		// Check _ImageName
		anAlphaImage = GetImage(theFilename.substr(0, aLastSlashPos+1) + "_" +
			theFilename.substr(aLastSlashPos+1, theFilename.length() - aLastSlashPos - 1), false);

		// Check ImageName_
		if(anAlphaImage==NULL)
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
				unsigned long* aBits1 = anImage->mBits;
				unsigned long* aBits2 = anAlphaImage->mBits;
				int aSize = anImage->mWidth*anImage->mHeight;

				for (int i = 0; i < aSize; i++)
				{
					*aBits1 = (*aBits1 & 0x00FFFFFF) | ((*aBits2 & 0xFF) << 24);
					++aBits1;
					++aBits2;
				}
			}

			delete anAlphaImage;
		}
		else if (gAlphaComposeColor==0xFFFFFF)
		{
			anImage = anAlphaImage;

			unsigned long* aBits1 = anImage->mBits;

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
			anImage = anAlphaImage;

			unsigned long* aBits1 = anImage->mBits;

			int aSize = anImage->mWidth*anImage->mHeight;
			for (int i = 0; i < aSize; i++)
			{
				*aBits1 = aColor | ((*aBits1 & 0xFF) << 24);
				++aBits1;
			}
		}
	}

	return anImage;
}
