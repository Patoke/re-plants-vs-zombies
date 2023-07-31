{	
	ulong* aDestBits = GetBits();

	int aSrcRowWidth = aSrcMemoryImage->GetWidth();

	int aSrcXI = (int) floor(theSrcRect.mX);
	int aSrcYI = (int) floor(theSrcRect.mY);
	int aSrcWidthI = (int) ceil(theSrcRect.mWidth + (theSrcRect.mX - aSrcXI));
	int aSrcHeightI = (int) ceil(theSrcRect.mHeight + (theSrcRect.mY - aSrcYI));

	if (aSrcXI<0) aSrcXI = 0;
	if (aSrcYI<0) aSrcYI = 0;
	if (aSrcXI + aSrcWidthI > theImage->mWidth) aSrcWidthI = theImage->mWidth - aSrcXI;
	if (aSrcYI + aSrcHeightI > theImage->mHeight) aSrcHeightI = theImage->mHeight - aSrcYI;
	if (aSrcWidthI<=0 || aSrcHeightI<=0)
		return;

	int aTempDestWidth = theDestRect.mWidth+4;
	int aTempDestHeight = theDestRect.mHeight+4;

	// For holding horizontally resized pixels not vertically (yet)
	ulong* aNewHorzPixels = new ulong[aTempDestWidth*aSrcHeightI*4];

	ulong* aNewHorzPixelsEnd = aNewHorzPixels + (aTempDestWidth*aSrcHeightI*4);

	ZeroMemory(aNewHorzPixels, aTempDestWidth*aSrcHeightI*4*4);

	int aSrcImageWidth = theImage->GetWidth();

	if (theSrcRect.mWidth >= theDestRect.mWidth)
	{
		double aDestXFactor = theDestRect.mWidth / theSrcRect.mWidth;
		double aDestXOffset = 1.0 + (aSrcXI - theSrcRect.mX) * aDestXFactor;

		// Shrinking			

		for (int aSrcX = 0; aSrcX < aSrcWidthI; aSrcX++)			
		{
			double aDestX1 = aDestXFactor * aSrcX + aDestXOffset;
			double aDestX2 = aDestX1 + aDestXFactor;
			
			int aDestXI1 = (int) aDestX1;
			int aDestXI2 = (int) aDestX2;

			SRC_TYPE* s1 = &aSrcBits[aSrcYI*aSrcRowWidth + aSrcXI+aSrcX];

			if (aDestXI1 == aDestXI2)
			{
				ulong* d = &aNewHorzPixels[aDestXI1*4];					
				int aFactor = (int) (257 * aDestXFactor);
				
				for (int aSrcY = 0; aSrcY < aSrcHeightI; aSrcY++)
				{
					ulong pixel = READ_COLOR(s1);
					
					*d++ += aFactor * ((pixel      ) & 0xFF);
					*d++ += aFactor * ((pixel >>  8) & 0xFF);
					*d++ += aFactor * ((pixel >> 16) & 0xFF);
					*d++ += aFactor * ((pixel >> 24) & 0xFF);

					DBG_ASSERTE(d <= aNewHorzPixelsEnd);

					d += aTempDestWidth*4 - 4;
					s1 += aSrcRowWidth;
				}					
			}
			else
			{								
				int aFactor1 = (int) (257 * (aDestXI2 - aDestX1));
				int aFactor2 = (int) (257 * (aDestX2 - aDestXI2));
				
				ulong* d = &aNewHorzPixels[aDestXI1*4];					
				
				for (int aSrcY = 0; aSrcY < aSrcHeightI; aSrcY++)
				{
					ulong pixel = READ_COLOR(s1);
					
					*d++ += aFactor1 * ((pixel      ) & 0xFF);
					*d++ += aFactor1 * ((pixel >>  8) & 0xFF);
					*d++ += aFactor1 * ((pixel >> 16) & 0xFF);
					*d++ += aFactor1 * ((pixel >> 24) & 0xFF);
					
					*d++ += aFactor2 * ((pixel      ) & 0xFF);
					*d++ += aFactor2 * ((pixel >>  8) & 0xFF);
					*d++ += aFactor2 * ((pixel >> 16) & 0xFF);
					*d++ += aFactor2 * ((pixel >> 24) & 0xFF);

					DBG_ASSERTE(d <= aNewHorzPixelsEnd);

					d += aTempDestWidth*4 - 8;
					s1 += aSrcRowWidth;
				}					
			}
		}
	}
	else
	{
		double aSrcXFactor;
		if (theDestRect.mWidth != 1)
			aSrcXFactor = (theSrcRect.mWidth - 1) / (theDestRect.mWidth - 1);
		else
			aSrcXFactor = (theSrcRect.mWidth) / (theDestRect.mWidth);

		for (int aDestX = 1; aDestX < aTempDestWidth-1; aDestX++)
		{
			ulong* d = &aNewHorzPixels[aDestX*4];

			double aSrcX = (aDestX - 1)*aSrcXFactor + theSrcRect.mX;
			int aSrcXI = (int) aSrcX;

			int aFactor1 = (int) (257 * (1.0 - (aSrcX - aSrcXI)));
			int aFactor2 = (int) (257 - aFactor1);
			
			SRC_TYPE* s = &aSrcBits[aSrcYI*aSrcRowWidth+aSrcXI];				

			for (int aDestY = 0; aDestY < aSrcHeightI; aDestY++)
			{
				ulong pixel1 = READ_COLOR(s++);
				ulong pixel2 = READ_COLOR(s);
				
				*d++ = (aFactor1 * ((pixel1      ) & 0xFF)) + (aFactor2 * ((pixel2      ) & 0xFF));
				*d++ = (aFactor1 * ((pixel1 >>  8) & 0xFF)) + (aFactor2 * ((pixel2 >>  8) & 0xFF));
				*d++ = (aFactor1 * ((pixel1 >> 16) & 0xFF)) + (aFactor2 * ((pixel2 >> 16) & 0xFF));
				*d++ = (aFactor1 * ((pixel1 >> 24) & 0xFF)) + (aFactor2 * ((pixel2 >> 24) & 0xFF));

				DBG_ASSERTE(d <= aNewHorzPixelsEnd);

				d += aTempDestWidth*4 - 4;
				s += aSrcRowWidth - 1;
			}				
		}
	}

	ulong* aNewPixels = new ulong[aTempDestWidth*aTempDestHeight*4];

	ulong* aNewPixelsEnd = aNewPixels + (aTempDestWidth*aTempDestHeight*4);

	ZeroMemory(aNewPixels, aTempDestWidth*aTempDestHeight*4*4);

	// Now resize vertically
	if (theSrcRect.mHeight >= theDestRect.mHeight)
	{			
		double aDestYFactor = theDestRect.mHeight / theSrcRect.mHeight;
		
		double aDestYOffset = 1.0 + (aSrcYI - theSrcRect.mY) * aDestYFactor;

		for (int aSrcY = 0; aSrcY < aSrcHeightI; aSrcY++)
		{
			double aDestY1 = aDestYFactor * aSrcY + aDestYOffset;
			double aDestY2 = aDestY1 + aDestYFactor;
			
			int aDestYI1 = (int) floor(aDestY1);
			int aDestYI2 = (int) floor(aDestY2);

			ulong* s = &aNewHorzPixels[aSrcY*aTempDestWidth*4];

			if (aDestYI1 == aDestYI2)
			{
				ulong* d = &aNewPixels[aDestYI1*aTempDestWidth*4];
				int aFactor = (int) (256 * aDestYFactor);
				
				for (int aSrcX = 0; aSrcX < aTempDestWidth; aSrcX++)
				{												
					*d++ += aFactor * *s++;
					*d++ += aFactor * *s++;
					*d++ += aFactor * *s++;
					*d++ += aFactor * *s++;						
				}

				DBG_ASSERTE(d <= aNewPixelsEnd);
			}
			else
			{
				int aFactor1 = (int) (256 * (aDestYI2 - aDestY1));
				int aFactor2 = (int) (256 * (aDestY2 - aDestYI2));					
				
				ulong* d1 = &aNewPixels[aDestYI1*aTempDestWidth*4];
				ulong* d2 = &aNewPixels[aDestYI2*aTempDestWidth*4];					
				
				for (int aSrcX = 0; aSrcX < aTempDestWidth; aSrcX++)
				{
					*d1++ += aFactor1 * *s;
					*d2++ += aFactor2 * *s++;

					*d1++ += aFactor1 * *s;
					*d2++ += aFactor2 * *s++;

					*d1++ += aFactor1 * *s;
					*d2++ += aFactor2 * *s++;

					*d1++ += aFactor1 * *s;
					*d2++ += aFactor2 * *s++;
				}

				DBG_ASSERTE(d1 <= aNewPixelsEnd);
				DBG_ASSERTE(d2 <= aNewPixelsEnd);
			}
		}
	}
	else
	{
		double aSrcYFactor; 
		if (theDestRect.mHeight != 1)
			aSrcYFactor = (theSrcRect.mHeight - 1) / (theDestRect.mHeight - 1);
		else
			aSrcYFactor = (theSrcRect.mHeight) / (theDestRect.mHeight);

		for (int aDestY = 1; aDestY < theDestRect.mHeight + 1; aDestY++)			
		{
			ulong* d = &aNewPixels[(aDestY*aTempDestWidth+1)*4];

			double aSrcY = (aDestY-1)*aSrcYFactor + (theSrcRect.mY - ((int) theSrcRect.mY));
			int aSrcYI = (int) aSrcY;

			int aFactor1 = (int) (256 * (1.0 - (aSrcY - aSrcYI)));
			int aFactor2 = 256 - aFactor1;
			
			ulong* s1 = &aNewHorzPixels[(aSrcYI*aTempDestWidth+1)*4];
			ulong* s2;
			
			if (aSrcYI == aSrcHeightI - 1)
				s2 = s1;
			else
				s2 = &aNewHorzPixels[((aSrcYI+1)*aTempDestWidth+1)*4];

			for (int aDestX = 1; aDestX < aTempDestWidth-1; aDestX++)	
			{
				*d++ = (aFactor1 * *s1++) + (aFactor2 * *s2++);
				*d++ = (aFactor1 * *s1++) + (aFactor2 * *s2++);
				*d++ = (aFactor1 * *s1++) + (aFactor2 * *s2++);
				*d++ = (aFactor1 * *s1++) + (aFactor2 * *s2++);					
			}				
		}
	}

	
	for (int y = 0; y < theDestRect.mHeight; y++)
	{
		ulong* aDestPixels = &aDestBits[(theDestRect.mY+y)*mWidth+theDestRect.mX];

		for (int x = 0; x < theDestRect.mWidth; x++)				
		{
			ulong *p = &aNewPixels[((y+1)*aTempDestWidth+x+1)*4];
			
			int b = (*p++) >> 16;
			int g = (*p++) >> 16;
			int r = (*p++) >> 16;
			int a = (*p++) >> 16;
			
			/*aDestBits[(theDestRect.mY+y)*mWidth+theDestRect.mX+x] =
				(r) | (g << 8) | (b << 16) | (a << 24);*/

			if (a != 0)
			{		
				ulong dest = *aDestPixels;
				int aDestAlpha = dest >> 24;
				int aNewDestAlpha = aDestAlpha + ((255 - aDestAlpha) * a) / 255;

				//a = 255 * a / (a + aDestAlpha);					
				int oma = 256 - a;
				
				DBG_ASSERTE(aDestPixels < aDestEnd);

				*(aDestPixels++) = (aNewDestAlpha << 24) |
					(((((dest & 0x0000FF) * oma) >> 8) & 0x0000FF) + (((b * a) >> 8))) |
					(((((dest & 0x00FF00) * oma) >> 8) & 0x00FF00) + (((g * a) >> 8) << 8)) |
					(((((dest & 0xFF0000) * oma) >> 8) & 0xFF0000) + (((r * a) >> 8) << 16));				
			}
			else
				aDestPixels++;
		}
	}

	delete[] aNewPixels;
	delete[] aNewHorzPixels;
}