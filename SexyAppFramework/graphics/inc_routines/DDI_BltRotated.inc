{
	double aCos = cos(theRot);
	double aSin = sin(theRot);

	int aCosLong = (int) (aCos * 0x10000);
	int aSinLong = (int) (aSin * 0x10000);	

	if (!LockSurface())
		return;

	int aRedShift = mGLInterface->mRedShift;
	int aGreenShift = mGLInterface->mGreenShift;
	int aBlueShift = mGLInterface->mBlueShift;

	int aRedRightShift = 32 - mGLInterface->mRedBits;
	int aGreenRightShift = 24 - mGLInterface->mGreenBits;
	int aBlueRightShift = 16 - mGLInterface->mBlueBits;

	int aNumBits = mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount;

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

		if (theColor == Color::White)
		{
			#define DEST_PIXEL_TYPE ulong
			#define WRITE_PIXEL\
			{\
				ulong dest = *aDestPixels;\
				int oma = 256 - (a1 + a2 + a3 + a4);\
				*(aDestPixels++) = \
					(((((dest & aBMask) * oma) >> 8) & aBMask) + \
						(((((src1 & 0x0000FF) * a1)  + \
							((src2 & 0x0000FF) * a2)  + \
							((src3 & 0x0000FF) * a3)  + \
							((src4 & 0x0000FF) * a4)) >> aBlueRightShift) << aBlueShift)) | \
					(((((dest & aGMask) * oma) >> 8) & aGMask) + \
						(((((src1 & 0x00FF00) * a1)  + \
							((src2 & 0x00FF00) * a2)  + \
							((src3 & 0x00FF00) * a3)  + \
							((src4 & 0x00FF00) * a4)) >> aGreenRightShift) << aGreenShift)) | \
					(((((dest & aRMask) * oma) >> 8) & aRMask) + \
						(((((src1 & 0xFF0000) * a1)  + \
							((src2 & 0xFF0000) * a2)  + \
							((src3 & 0xFF0000) * a3)  + \
							((src4 & 0xFF0000) * a4)) >> aRedRightShift) << aRedShift));\
			}
			#define WRITE_PIXEL_OPTIMIZED\
			{\
				ulong dest = *aDestPixels;\
				int oma = 256 - (a1 + a2 + a3 + a4);\
				ulong src_b = ((src1 & 0xFF00FF) * a1 + (src2 & 0xFF00FF) * a2 + (src3 & 0xFF00FF) * a3 + (src4 & 0xFF00FF) * a4) & 0xFF00FF00;\
				ulong src_b2 = ((src1 & 0xFF00) * a1 + (src2 & 0xFF00) * a2 + (src3 & 0xFF00) * a3 + (src4 & 0xFF00) * a4) & 0xFF0000;\
				*(aDestPixels++) =\
						((((dest & 0xFF00FF) * oma + src_b) >> 8) & 0xFF00FF) |\
						((((dest & 0x00FF00) * oma + src_b2) >> 8) & 0x00FF00);\
			}



			#include "BltRotatedHelper.inc"		
						
			#undef WRITE_PIXEL	
			#undef WRITE_PIXEL_OPTIMIZED
			#undef DEST_PIXEL_TYPE 											
		}
		else		
		{
			// Colorized version
			
			int ca = theColor.mAlpha;
			int cr = theColor.mRed + 1;
			int cg = theColor.mGreen + 1;
			int cb = theColor.mBlue + 1;
		
			#define DEST_PIXEL_TYPE ulong
			#define WRITE_PIXEL\
			{\
				ulong dest = *aDestPixels;\
				a1 = (a1*ca)>>8; a2 = (a2*ca)>>8; a3 = (a3*ca)>>8; a4 = (a4*ca)>>8; \
				int oma = 256 - (a1 + a2 + a3 + a4);\
				*(aDestPixels++) = \
					( ((((dest & aBMask) * oma) >> 8) & aBMask) + \
						(((( (((src1 & 0x0000FF) * a1)  + \
								((src2 & 0x0000FF) * a2)  + \
								((src3 & 0x0000FF) * a3)  + \
								((src4 & 0x0000FF) * a4)) >> 8) * cb) >> aBlueRightShift) << aBlueShift) ) | \
					( ((((dest & aGMask) * oma) >> 8) & aGMask) + \
						(((( (((src1 & 0x00FF00) * a1)  + \
							    ((src2 & 0x00FF00) * a2)  + \
								((src3 & 0x00FF00) * a3)  + \
								((src4 & 0x00FF00) * a4)) >> 8) * cg) >> aGreenRightShift) << aGreenShift) ) | \
					( ((((dest & aRMask) * oma) >> 8) & aRMask) + \
						(((( (((src1 & 0xFF0000) * a1)  + \
								((src2 & 0xFF0000) * a2)  + \
								((src3 & 0xFF0000) * a3)  + \
								((src4 & 0xFF0000) * a4)) >> 8) * cr) >> aRedRightShift) << aRedShift) );\
			}					
								
			#include "BltRotatedHelper.inc"		
						
			#undef WRITE_PIXEL	
			#undef DEST_PIXEL_TYPE 			
								
		}
	}

	UnlockSurface();
}