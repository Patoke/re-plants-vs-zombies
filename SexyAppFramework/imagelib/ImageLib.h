#ifndef __IMAGELIB_H__
#define __IMAGELIB_H__

#include <string>

namespace ImageLib
{

class Image
{
public:
	int						mWidth;
	int						mHeight;
	unsigned long*			mBits;

public:
	Image();
	virtual ~Image();

	int						GetWidth();
	int						GetHeight();
	unsigned long*			GetBits();
};

bool WriteJPEGImage(const std::string& theFileName, Image* theImage);
bool WritePNGImage(const std::string& theFileName, Image* theImage);
bool WriteTGAImage(const std::string& theFileName, Image* theImage);
bool WriteBMPImage(const std::string& theFileName, Image* theImage);
extern int gAlphaComposeColor;
extern bool gAutoLoadAlpha;
extern bool gIgnoreJPEG2000Alpha;  // I've noticed alpha in jpeg2000's that shouldn't have alpha so this defaults to true


Image* GetImage(const std::string& theFileName, bool lookForAlphaImage = true);

//void InitJPEG2000();
//void CloseJPEG2000();
//void SetJ2KCodecKey(const std::string& theKey);

}

#endif //__IMAGELIB_H__