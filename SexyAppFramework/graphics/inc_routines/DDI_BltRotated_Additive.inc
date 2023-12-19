{
	double aCos = cos(theRot);
	double aSin = sin(theRot);

	int aCosLong = (int) (aCos * 0x10000);
	int aSinLong = (int) (aSin * 0x10000);	

/* unused
#ifdef OPTIMIZE_SOFTWARE_DRAWING
	int aRotCenterXLong = (int)(theRotCenterX * 0x10000);
	int aRotCenterYLong = (int)(theRotCenterY * 0x10000);
#endif
*/

	if (!LockSurface())
		return;

	int* aMaxRedTable = mGLInterface->mRedAddTable;
	int* aMaxGreenTable = mGLInterface->mGreenAddTable;
	int* aMaxBlueTable = mGLInterface->mBlueAddTable;

	int aRedShift = mGLInterface->mRedShift;
	int aGreenShift = mGLInterface->mGreenShift;
	int aBlueShift = mGLInterface->mBlueShift;

	int aRedRightShift = 32 - mGLInterface->mRedBits;
	int aGreenRightShift = 24 - mGLInterface->mGreenBits;
	int aBlueRightShift = 16 - mGLInterface->mBlueBits;

	ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
	ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
	ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;

	// unused
	// ulong aRRoundAdd = aRMask >> 1;
	// ulong aGRoundAdd = aGMask >> 1;
	// ulong aBRoundAdd = aBMask >> 1;
	
	if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
	{			
		ulong* aDestPixelsRow = ((ulong*) mLockedSurfaceDesc.lpSurface) + ((int)aDestRect.mY * mLockedSurfaceDesc.lPitch/4) + (int)aDestRect.mX;		
		int aDestPixelsPitch = mLockedSurfaceDesc.lPitch/4;

		//if (theColor == Color::White)				
		{
			int ca = theColor.mAlpha;
			int cr = (theColor.mRed * ca) / 255;
			int cg = (theColor.mGreen * ca) / 255;
			int cb = (theColor.mBlue * ca) / 255;
		
				
			#define DEST_PIXEL_TYPE ulong
			#define WRITE_PIXEL\
			{\
				ulong dest = *aDestPixels;\
				\
				int r = aMaxRedTable[((dest & aRMask)>>aRedShift) + \
					((((((src1 & 0xFF0000) * a1) + \
						((src2 & 0xFF0000) * a2) + \
						((src3 & 0xFF0000) * a3) + \
						((src4 & 0xFF0000) * a4)) >> aRedRightShift) * cr) >> 8)];\
				int g = aMaxGreenTable[((dest & aGMask)>>aGreenShift) + \
					(((((src1 & 0x00FF00) * a1) + \
						((src2 & 0x00FF00) * a2) + \
						((src3 & 0x00FF00) * a3) + \
						((src4 & 0x00FF00) * a4)) * cg) >> (aGreenRightShift + 8))];\
				int b = aMaxBlueTable[((dest & aBMask)>>aBlueShift) + \
					(((((src1 & 0x0000FF) * a1) + \
						((src2 & 0x0000FF) * a2) + \
						((src3 & 0x0000FF) * a3) + \
						((src4 & 0x0000FF) * a4)) * cb) >> (aBlueRightShift + 8))];\
				\
				*(aDestPixels++) = \
					(r << aRedShift) | \
					(g << aGreenShift) | \
					(b << aBlueShift);\
			}

			#include "BltRotatedHelper.inc"		
			
			#undef WRITE_PIXEL
			#undef DEST_PIXEL_TYPE
															
			
		}
		/*else
		{
			
		}*/
	}

	UnlockSurface();
}