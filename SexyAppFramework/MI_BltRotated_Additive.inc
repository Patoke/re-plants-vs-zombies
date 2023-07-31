{
	double aCos = cos(theRot);
	double aSin = sin(theRot);

	int aCosLong = (int) (aCos * 0x10000);
	int aSinLong = (int) (aSin * 0x10000);	

#ifdef OPTIMIZE_SOFTWARE_DRAWING
	int aRotCenterXLong = (int)(theRotCenterX * 0x10000);
	int aRotCenterYLong = (int)(theRotCenterY * 0x10000);
#endif

	ulong* aDestPixelsRow = GetBits() + ((int)aDestRect.mY * mWidth) + (int)aDestRect.mX;		
	int aDestPixelsPitch = mWidth;

	if (theColor == Color::White)
	{
		#define DEST_PIXEL_TYPE ulong
		#define WRITE_PIXEL\
		{\
			ulong dest = *aDestPixels;\
			int a = a1+a2+a3+a4; \
			if(a==0) /* transparent */ \
				aDestPixels++;\
			else \
			{ \
				ulong r = aMaxTable[((((src1&0xFF0000)*a1) + ((src2&0xFF0000)*a2) + ((src3&0xFF0000)*a3) + ((src4&0xFF0000)*a4))>>24) + ((dest&0xFF0000)>>16)]; \
				ulong g = aMaxTable[((((src1&0x00FF00)*a1) + ((src2&0x00FF00)*a2) + ((src3&0x00FF00)*a3) + ((src4&0x00FF00)*a4))>>16) + ((dest&0x00FF00)>>8)]; \
				ulong b = aMaxTable[((((src1&0x0000FF)*a1) + ((src2&0x0000FF)*a2) + ((src3&0x0000FF)*a3) + ((src4&0x0000FF)*a4))>>8) + (dest&0x0000FF)]; \
				\
				*aDestPixels++ = (dest & 0xFF000000) | (r<<16) | (g<<8) | b; \
			} \
		}
		
		#include "BltRotatedHelper.inc"		
		
		#undef WRITE_PIXEL
		#undef DEST_PIXEL_TYPE		
	}
	else	
	{
		int ca = theColor.mAlpha;
		int cr = (theColor.mRed * ca) / 255;
		int cg = (theColor.mGreen * ca) / 255;
		int cb = (theColor.mBlue * ca) / 255;
		
		#define DEST_PIXEL_TYPE ulong
		#define WRITE_PIXEL\
		{\
			ulong dest = *aDestPixels;\
			int a = a1+a2+a3+a4; \
			if(a==0) /* transparent */ \
				aDestPixels++; \
			else \
			{ \
				ulong destPixel = *aDestPixels; \
			\
				ulong r = aMaxTable[((((((src1&0xFF0000)*a1) + ((src2&0xFF0000)*a2) + ((src3&0xFF0000)*a3) + ((src4&0xFF0000)*a4))>>24)*cr)>>8) + ((dest&0xFF0000)>>16)]; \
				ulong g = aMaxTable[(((((src1&0x00FF00)*a1) + ((src2&0x00FF00)*a2) + ((src3&0x00FF00)*a3) + ((src4&0x00FF00)*a4))*cg)>>24) + ((dest&0x00FF00)>>8)]; \
				ulong b = aMaxTable[(((((src1&0x0000FF)*a1) + ((src2&0x0000FF)*a2) + ((src3&0x0000FF)*a3) + ((src4&0x0000FF)*a4))*cb)>>16) + (dest&0x0000FF)]; \
				\
				*aDestPixels++ = (dest & 0xFF000000) | (r<<16) | (g<<8) | b; \
			}\
		}
		
		#include "BltRotatedHelper.inc"		
		
		#undef WRITE_PIXEL
		#undef DEST_PIXEL_TYPE		
	}
}