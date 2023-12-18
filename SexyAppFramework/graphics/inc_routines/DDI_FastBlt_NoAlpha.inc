{	
	if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
	{
		ulong* aDestPixelsRow = ((ulong*) mLockedSurfaceDesc.lpSurface) + (theY * mLockedSurfaceDesc.lPitch/4) + theX;		

		if (aMemoryImage->mHasTrans)
		{
			for (int y = 0; y < theSrcRect.mHeight; y++)
			{
				ulong* aDestPixels = aDestPixelsRow;
				
				aSrcPixels = aSrcPixelsRow;

				for (int x = 0; x < theSrcRect.mWidth; x++)
				{
					ulong aSrc = NEXT_SRC_COLOR;

					if (aSrc>>24 != 0)
						*(aDestPixels++) = aSrc;
					else
						aDestPixels++;
				}

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/4;
				aSrcPixelsRow += theImage->mWidth;
			}
		}
		else
		{
			for (int y = 0; y < theSrcRect.mHeight; y++)
			{
				ulong* aDestPixels = aDestPixelsRow;
				
				aSrcPixels = aSrcPixelsRow;

				for (int x = 0; x < theSrcRect.mWidth; x++)
				{
					*(aDestPixels++) = NEXT_SRC_COLOR;
				}

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/4;
				aSrcPixelsRow += theImage->mWidth;
			}
		}
	}

	UnlockSurface();
}