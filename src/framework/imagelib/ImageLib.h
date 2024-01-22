#ifndef __IMAGELIB_H__
#define __IMAGELIB_H__

#include <cstdint>
#include <memory>
#include <string>
#include <array>

namespace ImageLib
{

class Image
{
public:
	int							mWidth = 0;
	int							mHeight = 0;
	std::unique_ptr<uint32_t[]>	mBits = nullptr;

	Image(int width, int height) :
		mWidth(width),
		mHeight(height),
		mBits(std::make_unique<uint32_t[]>(mWidth*mHeight)) {
			for (int i = 0; i < mHeight; ++i) {
				for (int j = 0; j < mWidth; ++j) {
					//mBits[i*mWidth + j] = (((i/16)&1) ^ ((j/16)&1))?0xFFFF00FF:0x00000000;
					mBits[i*mWidth + j] = 0x00000000;
				}
			}
		}

	Image(int width, int height, std::unique_ptr<uint32_t[]> bits) :
		mWidth(width),
		mHeight(height),
		mBits(std::move(bits)) {}
};

bool WriteJPEGImage(const std::string& theFileName, Image* theImage);
bool WritePNGImage(const std::string& theFileName, Image* theImage);
bool WriteTGAImage(const std::string& theFileName, Image* theImage);
bool WriteBMPImage(const std::string& theFileName, Image* theImage);
extern int gAlphaComposeColor;
extern bool gAutoLoadAlpha;
extern bool gIgnoreJPEG2000Alpha;  // I've noticed alpha in jpeg2000's that shouldn't have alpha so this defaults to true

std::unique_ptr<Image> GetImage(const std::string& theFileName, bool lookForAlphaImage = true, bool theDoImageSanding = false);

//void InitJPEG2000();
//void CloseJPEG2000();
//void SetJ2KCodecKey(const std::string& theKey);

}

#endif //__IMAGELIB_H__
