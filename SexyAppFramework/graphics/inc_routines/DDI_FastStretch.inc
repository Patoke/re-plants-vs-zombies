{
	if (!LockSurface())
		return;	
	
	int aFPSrcY = theSrcRect.mY * 0x10000;

	int anAddX = (theSrcRect.mWidth * 0x10000) / theDestRect.mWidth;
	int anAddY = (theSrcRect.mHeight * 0x10000) / theDestRect.mHeight;

	if (theColor == Color::White)
	{
		if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 16)
		{
			ushort* aDestPixelsRow = ((ushort*) mLockedSurfaceDesc.lpSurface) + (theDestRect.mY * mLockedSurfaceDesc.lPitch/2) + theDestRect.mX;

			ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
			ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
			ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;

			// unused
			// ulong aRRoundAdd = aRMask >> 1;
			// ulong aGRoundAdd = aGMask >> 1;
			// ulong aBRoundAdd = aBMask >> 1;					
	
			for (int y = 0; y < theDestRect.mHeight; y++)
			{
				int aFPSrcX = theSrcRect.mX * 0x10000;

				ushort* aDestPixels = aDestPixelsRow;
				SRC_TYPE* aSrcPixelsRow = aSrcBits + (aSrcMemoryImage->mWidth * (aFPSrcY >> 16));

				for (int x = 0; x < theDestRect.mWidth; x++, aFPSrcX += anAddX)
				{
					SRC_TYPE* aSrcPixels = aSrcPixelsRow + (aFPSrcX >> 16);
					
					ulong src = READ_COLOR(aSrcPixels);
					ulong dest = *aDestPixels;
					
					int a = src >> 24;	
					
					if (a != 0)
					{
						int oma = 256 - a;
						
						*(aDestPixels _PLUSPLUS) = 
							src +
							(((((dest & aRMask) * oma)) >> 8) & aRMask) +
							(((((dest & aGMask) * oma)) >> 8) & aGMask) +
							(((((dest & aBMask) * oma)) >> 8) & aBMask);
					}
					else
						aDestPixels _PLUSPLUS;
				}

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/2;
				aFPSrcY += anAddY;
			}		
		}
		else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
		{
			ulong* aDestPixelsRow = ((ulong*) mLockedSurfaceDesc.lpSurface) + (theDestRect.mY * mLockedSurfaceDesc.lPitch/4) + theDestRect.mX;

			ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
			ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
			ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;

			// unused
			// ulong aRRoundAdd = aRMask >> 1;
			// ulong aGRoundAdd = aGMask >> 1;
			// ulong aBRoundAdd = aBMask >> 1;					
	
			for (int y = 0; y < theDestRect.mHeight; y++)
			{
				int aFPSrcX = theSrcRect.mX * 0x10000;

				ulong* aDestPixels = aDestPixelsRow;
				SRC_TYPE* aSrcPixelsRow = aSrcBits + (aSrcMemoryImage->mWidth * (aFPSrcY >> 16));

				for (int x = 0; x < theDestRect.mWidth; x++, aFPSrcX += anAddX)
				{
					SRC_TYPE* aSrcPixels = aSrcPixelsRow + (aFPSrcX >> 16);
					
					ulong src = READ_COLOR(aSrcPixels);
					ulong dest = *aDestPixels;
					
					int a = src >> 24;	
					
					if (a != 0)
					{
						int oma = 256 - a;
						
						*(aDestPixels _PLUSPLUS) = 
							src +
							(((((dest & aRMask) * oma)) >> 8) & aRMask) +
							(((((dest & aGMask) * oma)) >> 8) & aGMask) +
							(((((dest & aBMask) * oma)) >> 8) & aBMask);
					}
					else
						aDestPixels _PLUSPLUS;
				}

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/4;
				aFPSrcY += anAddY;
			}
		}
	}
	else
	{
		int ca = theColor.mAlpha;
		int cr = (theColor.mRed * ca) / 255;
		int cg = (theColor.mGreen * ca) / 255;
		int cb = (theColor.mBlue * ca) / 255;
	
		if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 16)
		{
			ushort* aDestPixelsRow = ((ushort*) mLockedSurfaceDesc.lpSurface) + (theDestRect.mY * mLockedSurfaceDesc.lPitch/2) + theDestRect.mX;

			ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
			ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
			ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;

			// unused
			// ulong aRRoundAdd = aRMask >> 1;
			// ulong aGRoundAdd = aGMask >> 1;
			// ulong aBRoundAdd = aBMask >> 1;					
	
			for (int y = 0; y < theDestRect.mHeight; y++)
			{
				int aFPSrcX = theSrcRect.mX * 0x10000;

				ushort* aDestPixels = aDestPixelsRow;
				SRC_TYPE* aSrcPixelsRow = aSrcBits + (aSrcMemoryImage->mWidth * (aFPSrcY >> 16));

				for (int x = 0; x < theDestRect.mWidth; x++, aFPSrcX += anAddX)
				{
					SRC_TYPE* aSrcPixels = aSrcPixelsRow + (aFPSrcX >> 16);
					
					ulong src = READ_COLOR(aSrcPixels);
					ulong dest = *aDestPixels;
					
					int a = ((src >> 24) * ca) / 255;
					
					if (a != 0)
					{
						int oma = 256 - a;
						
						*(aDestPixels _PLUSPLUS) = 
							(((((dest & aRMask) * oma) + ((src & aRMask) * cr)) >> 8) & aRMask) +
							(((((dest & aGMask) * oma) + ((src & aGMask) * cg)) >> 8) & aGMask) +
							(((((dest & aBMask) * oma) + ((src & aBMask) * cb)) >> 8) & aBMask);
					}
					else
						aDestPixels _PLUSPLUS;
				}

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/2;
				aFPSrcY += anAddY;
			}		
		}
		else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
		{
			ulong* aDestPixelsRow = ((ulong*) mLockedSurfaceDesc.lpSurface) + (theDestRect.mY * mLockedSurfaceDesc.lPitch/4) + theDestRect.mX;

			ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
			ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
			ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;

			// unused
			// ulong aRRoundAdd = aRMask >> 1;
			// ulong aGRoundAdd = aGMask >> 1;
			// ulong aBRoundAdd = aBMask >> 1;					
	
			for (int y = 0; y < theDestRect.mHeight; y++)
			{
				int aFPSrcX = theSrcRect.mX * 0x10000;

				ulong* aDestPixels = aDestPixelsRow;
				SRC_TYPE* aSrcPixelsRow = aSrcBits + (aSrcMemoryImage->mWidth * (aFPSrcY >> 16));

				for (int x = 0; x < theDestRect.mWidth; x++, aFPSrcX += anAddX)
				{
					SRC_TYPE* aSrcPixels = aSrcPixelsRow + (aFPSrcX >> 16);
					
					ulong src = READ_COLOR(aSrcPixels);
					ulong dest = *aDestPixels;
					
					int a = ((src >> 24) * ca) / 255;
					int oma = 256 - a;		
					
					if (a != 0)
					{
						// redundant line
						// int oma = 256 - a;
						
						*(aDestPixels _PLUSPLUS) = 
							(((((dest & aRMask) * oma) + ((src & aRMask) * cr)) >> 8) & aRMask) +
							(((((dest & aGMask) * oma) + ((src & aGMask) * cg)) >> 8) & aGMask) +
							(((((dest & aBMask) * oma) + ((src & aBMask) * cb)) >> 8) & aBMask);
					}
					else
						aDestPixels _PLUSPLUS;
				}

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/4;
				aFPSrcY += anAddY;
			}
		}
	}
	
	UnlockSurface();
}