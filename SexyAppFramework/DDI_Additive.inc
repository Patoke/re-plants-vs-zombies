{
	aMemoryImage->CommitBits();

	RECT aDestRect = {theX, theY, theX + theSrcRect.mWidth, theY + theSrcRect.mHeight};
	RECT aSrcRect = {theSrcRect.mX, theSrcRect.mY, theSrcRect.mX + theSrcRect.mWidth, theSrcRect.mY + theSrcRect.mHeight};	

	if (!LockSurface())
		return;

	uchar* aSrcRLAdditiveData = aMemoryImage->GetRLAdditiveData(mDDInterface);

	if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 16)
	{
		ushort* aDestPixelsRow = ((ushort*) mLockedSurfaceDesc.lpSurface) + (theY * mLockedSurfaceDesc.lPitch/2) + theX;		

		uchar* aRLAdditiveDataRow = aSrcRLAdditiveData + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;

		ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
		ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
		ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;
		
		int aMaxR = (1<<mDDInterface->mRedBits) - 1;
		int aMaxG = (1<<mDDInterface->mGreenBits) - 1;
		int aMaxB = (1<<mDDInterface->mBlueBits) - 1;

		int aRedShift = mDDInterface->mRedShift;
		int aGreenShift = mDDInterface->mGreenShift;
		int aBlueShift = mDDInterface->mBlueShift;

		int* aMaxRedTable = mDDInterface->mRedAddTable;
		int* aMaxGreenTable = mDDInterface->mGreenAddTable;
		int* aMaxBlueTable = mDDInterface->mBlueAddTable;

		if (theColor == Color::White)
		{
			for (int y = 0; y < theSrcRect.mHeight; y++)
			{
				ushort* aDestPixels = aDestPixelsRow;				
				uchar* aRLAdditiveData = aRLAdditiveDataRow;
				
				aSrcPixels = aSrcPixelsRow;

				for (int aSpanLeft = theSrcRect.mWidth; aSpanLeft > 0; )
				{
					ulong src = PEEK_SRC_COLOR;
					uchar rl = *aRLAdditiveData;

					if (rl > aSpanLeft)
						rl = aSpanLeft;					

					if ((src & 0xFFFFFF) == 0) // A run of black
					{				
						aSrcPixels += rl;
						aDestPixels _PLUSEQUALS rl;
					}
					else
					{
						for (int i = 0; i < rl; i++)
						{
							src = NEXT_SRC_COLOR;
							ulong dest = *aDestPixels;					
							
							int r = aMaxRedTable[((dest & aRMask) + (src & aRMask)) >> aRedShift];
							int g = aMaxGreenTable[((dest & aGMask) + (src & aGMask)) >> aGreenShift];
							int b = aMaxBlueTable[((dest & aBMask) + (src & aBMask)) >> aBlueShift];							

							*(aDestPixels _PLUSPLUS) = 
								(r << aRedShift) |
								(g << aGreenShift) |
								(b << aBlueShift);
						}
					}

					aRLAdditiveData += rl;
					aSpanLeft -= rl;
				}

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/2;
				aSrcPixelsRow += theImage->mWidth;
				aRLAdditiveDataRow += theImage->mWidth;
			}
		}
		else
		{
			int ca = theColor.mAlpha;
			int cr = (theColor.mRed * ca) / 255;
			int cg = (theColor.mGreen * ca) / 255;
			int cb = (theColor.mBlue * ca) / 255;

			for (int y = 0; y < theSrcRect.mHeight; y++)
			{
				ushort* aDestPixels = aDestPixelsRow;				
				uchar* aRLAdditiveData = aRLAdditiveDataRow;
				
				aSrcPixels = aSrcPixelsRow;

				for (int aSpanLeft = theSrcRect.mWidth; aSpanLeft > 0; )
				{
					ulong src = PEEK_SRC_COLOR;
					uchar rl = *aRLAdditiveData;

					if (rl > aSpanLeft)
						rl = aSpanLeft;					

					if ((src & 0xFFFFFF) == 0) // A run of black
					{				
						aSrcPixels += rl;
						aDestPixels _PLUSEQUALS rl;
					}
					else
					{
						for (int i = 0; i < rl; i++)
						{
							src = NEXT_SRC_COLOR;
							ulong dest = *aDestPixels;					
							
							int r = aMaxRedTable[((dest & aRMask) >> aRedShift) + ((((src & aRMask) >> aRedShift) * cr) >> 8)];
							int g = aMaxGreenTable[((dest & aGMask) >> aGreenShift) + ((((src & aGMask) >> aGreenShift) * cg) >> 8)];
							int b = aMaxBlueTable[((dest & aBMask) >> aBlueShift) + ((((src & aBMask) >> aBlueShift) * cb) >> 8)];

							*(aDestPixels _PLUSPLUS) = 
								(r << aRedShift) |
								(g << aGreenShift) |
								(b << aBlueShift);
						}
					}

					aRLAdditiveData += rl;
					aSpanLeft -= rl;
				}

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/2;
				aSrcPixelsRow += theImage->mWidth;
				aRLAdditiveDataRow += theImage->mWidth;
			}
		}
	}
	else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
	{
		ulong* aDestPixelsRow = ((ulong*) mLockedSurfaceDesc.lpSurface) + (theY * mLockedSurfaceDesc.lPitch/4) + theX;		
		uchar* aRLAdditiveDataRow = aSrcRLAdditiveData + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;

		ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
		ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
		ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;
		
		int aMaxR = (1<<mDDInterface->mRedBits) - 1;
		int aMaxG = (1<<mDDInterface->mGreenBits) - 1;
		int aMaxB = (1<<mDDInterface->mBlueBits) - 1;

		int aRedShift = mDDInterface->mRedShift;
		int aGreenShift = mDDInterface->mGreenShift;
		int aBlueShift = mDDInterface->mBlueShift;

		int* aMaxRedTable = mDDInterface->mRedAddTable;
		int* aMaxGreenTable = mDDInterface->mGreenAddTable;
		int* aMaxBlueTable = mDDInterface->mBlueAddTable;

		if (theColor == Color::White)
		{
			for (int y = 0; y < theSrcRect.mHeight; y++)
			{
				ulong* aDestPixels = aDestPixelsRow;				
				uchar* aRLAdditiveData = aRLAdditiveDataRow;
				
				aSrcPixels = aSrcPixelsRow;

				for (int aSpanLeft = theSrcRect.mWidth; aSpanLeft > 0; )
				{
					ulong src = PEEK_SRC_COLOR;
					uchar rl = *aRLAdditiveData;

					if (rl > aSpanLeft)
						rl = aSpanLeft;					

					if ((src & 0xFFFFFF) == 0) // A run of black
					{				
						aSrcPixels += rl;
						aDestPixels _PLUSEQUALS rl;
					}
					else
					{
						for (int i = 0; i < rl; i++)
						{
							src = NEXT_SRC_COLOR;
							ulong dest = *aDestPixels;					
							
							int r = aMaxRedTable[((dest & aRMask) + (src & aRMask)) >> aRedShift];
							int g = aMaxGreenTable[((dest & aGMask) + (src & aGMask)) >> aGreenShift];
							int b = aMaxBlueTable[((dest & aBMask) + (src & aBMask)) >> aBlueShift];

							*(aDestPixels _PLUSPLUS) = 
								(r << aRedShift) |
								(g << aGreenShift) |
								(b << aBlueShift);
						}
					}

					aRLAdditiveData += rl;
					aSpanLeft -= rl;
				}

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/4;
				aSrcPixelsRow += theImage->mWidth;
				aRLAdditiveDataRow += theImage->mWidth;
			}
		}
		else
		{
			int ca = theColor.mAlpha;
			int cr = (theColor.mRed * ca) / 255;
			int cg = (theColor.mGreen * ca) / 255;
			int cb = (theColor.mBlue * ca) / 255;

			for (int y = 0; y < theSrcRect.mHeight; y++)
			{
				ulong* aDestPixels = aDestPixelsRow;				
				uchar* aRLAdditiveData = aRLAdditiveDataRow;
				
				aSrcPixels = aSrcPixelsRow;

				for (int aSpanLeft = theSrcRect.mWidth; aSpanLeft > 0; )
				{
					ulong src = PEEK_SRC_COLOR;
					uchar rl = *aRLAdditiveData;

					if (rl > aSpanLeft)
						rl = aSpanLeft;					

					if ((src & 0xFFFFFF) == 0) // A run of black
					{				
						aSrcPixels += rl;
						aDestPixels _PLUSEQUALS rl;
					}
					else
					{
						for (int i = 0; i < rl; i++)
						{
							src = NEXT_SRC_COLOR;
							ulong dest = *aDestPixels;					
							
							int r = aMaxRedTable[((dest & aRMask) >> aRedShift) + ((((src & aRMask) >> aRedShift) * cr) >> 8)];
							int g = aMaxGreenTable[((dest & aGMask) >> aGreenShift) + ((((src & aGMask) >> aGreenShift) * cg) >> 8)];
							int b = aMaxBlueTable[((dest & aBMask) >> aBlueShift) + ((((src & aBMask) >> aBlueShift) * cb) >> 8)];

							*(aDestPixels _PLUSPLUS) = 
								(r << aRedShift) |
								(g << aGreenShift) |
								(b << aBlueShift);
						}
					}

					aRLAdditiveData += rl;
					aSpanLeft -= rl;
				}

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/4;
				aSrcPixelsRow += theImage->mWidth;
				aRLAdditiveDataRow += theImage->mWidth;
			}
		}
	}

	UnlockSurface();
}