#include "DDImage.h"
#include <math.h>
#include "DDInterface.h"
#include "D3DInterface.h"
#include "Rect.h"
#include "Graphics.h"
#include "SexyAppBase.h"
#include "Debug.h"
#include "PerfTimer.h"

#pragma warning(disable:4005) // macro redefinition
#pragma warning(disable:4244) // conversion possible loss of data

using namespace Sexy;

extern bool gOptimizeSoftwareDrawing;

DDImage::DDImage(DDInterface* theDDInterface) :
	MemoryImage(theDDInterface->mApp)
{
	mDDInterface = theDDInterface;	
	Init();
}

DDImage::DDImage() :
	MemoryImage(gSexyAppBase)
{
	mDDInterface = gSexyAppBase->mDDInterface;
	Init();
}

DDImage::~DDImage()
{
	if (mSurface != NULL)
		mSurface->Release();
	mDDInterface->RemoveDDImage(this);

	DBG_ASSERTE(mLockCount == 0);
}

void DDImage::Init()
{
	mSurface = NULL;
	mDDInterface->AddDDImage(this);
	
	mNoLock = false;
	mVideoMemory = false;
	mFirstPixelTrans = false;
	mWantDDSurface = false;			
	mDrawToBits = false;
	mSurfaceSet = false;

	mLockCount = 0;
}

bool DDImage::Check3D(Image *theImage)
{
	DDImage *anImage = dynamic_cast<DDImage*>(theImage);
	if (anImage!=NULL)
		return Check3D(anImage);
	else
		return false;
}

bool DDImage::Check3D(DDImage *theImage)
{
	return theImage->mDDInterface->mIs3D && theImage->mSurface==theImage->mDDInterface->mDrawSurface;
}

bool DDImage::LockSurface()
{
	if (Check3D(this))
		return false;

	if (mLockCount == 0)
	{
		memset(&mLockedSurfaceDesc, 0, sizeof(mLockedSurfaceDesc));
		mLockedSurfaceDesc.dwSize = sizeof(mLockedSurfaceDesc);
		int aResult = GetSurface()->Lock(NULL, &mLockedSurfaceDesc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);

		if (aResult != DD_OK)
			return false;
	}

	mLockCount++;

	DBG_ASSERTE(mLockCount < 8);

	return true;
}

bool DDImage::UnlockSurface()
{
	if (Check3D(this))
		return false;

	--mLockCount;

	if (mLockCount == 0)
	{
		mSurface->Unlock(NULL);
	}

	DBG_ASSERTE(mLockCount >= 0);

	return true;
}

void DDImage::SetSurface(LPDIRECTDRAWSURFACE theSurface)
{
	mSurfaceSet = true;
	mSurface = theSurface;
	mSurface->AddRef();

//	TDDSurfaceDesc aDesc;
	DDSURFACEDESC aDesc;
	ZeroMemory(&aDesc, sizeof(aDesc));
	aDesc.dwSize = sizeof(aDesc);
    aDesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    
    HRESULT aResult = mSurface->GetSurfaceDesc(&aDesc);	(void)aResult;

	mWidth = aDesc.dwWidth;
	mHeight = aDesc.dwHeight;

	mNoLock = false;
}

bool DDImage::GenerateDDSurface()
{
	if (mSurface != NULL)
		return true;	

	CommitBits();

	if (mHasAlpha)
		return false;

	mWantDDSurface = true;

	// Force into non-palletized mode for this
	if (mColorTable != NULL)
		GetBits();

	HRESULT aResult;
//	TDDSurfaceDesc aDesc;
	DDSURFACEDESC2 aDesc;

	ZeroMemory(&aDesc, sizeof(aDesc));
	aDesc.dwSize = sizeof(aDesc);
	aDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	aDesc.ddsCaps.dwCaps = mVideoMemory ? DDSCAPS_VIDEOMEMORY : DDSCAPS_SYSTEMMEMORY;
	aDesc.dwWidth = mWidth;
	aDesc.dwHeight = mHeight;
		
	AutoCrit aCrit(mDDInterface->mCritSect); // prevent mSurface from being released while we're in this code

	aResult = mDDInterface->CreateSurface(&aDesc, &mSurface, NULL);
	if (aResult != DD_OK)
		return false;

	if (!LockSurface())
		return false;

	const int rRightShift = 16 + (8-mDDInterface->mRedBits);
	const int gRightShift = 8 + (8-mDDInterface->mGreenBits);
	const int bRightShift = 0 + (8-mDDInterface->mBlueBits);

	const int rLeftShift = mDDInterface->mRedShift;
	const int gLeftShift = mDDInterface->mGreenShift;
	const int bLeftShift = mDDInterface->mBlueShift;

	const int rMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
	const int gMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
	const int bMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;

	int aNumBits = mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount;

	if (aNumBits == 16)
	{
		ushort* mSurfaceBits = (ushort*) mLockedSurfaceDesc.lpSurface;

		if (mSurfaceBits != NULL)
		{
			int i;
			bool firstTrans = true;			
						
			ushort* a16Bits = NULL;
			ushort aTransColor = 0;
			
			if (mBits != NULL)
			{
				a16Bits = new ushort[mWidth*mHeight];
				ulong* aSrcPtr = mBits;	
				ushort* a16SrcPtr = a16Bits;

				for (i = 0; i < mWidth*mHeight; i++)
				{			
					ulong val = *(aSrcPtr++);

					*a16SrcPtr =	(((val>>rRightShift)<<rLeftShift)&rMask) | 
									(((val>>gRightShift)<<gLeftShift)&gMask) |
									(((val>>bRightShift)<<bLeftShift)&bMask);

					int anAlpha = val >> 24;
					if ((firstTrans) && (anAlpha < 255))
					{
						firstTrans = false;
						aTransColor = *a16SrcPtr;
					}

					++a16SrcPtr;
				}
			}
			
			if ((mHasTrans) && (mBits != NULL))
			{
				if (mFirstPixelTrans)
				{
					aTransColor = *a16Bits;

					if (a16Bits != NULL)
					{
						ushort* aDestPtr = mSurfaceBits;						
						ushort* a16SrcPtr = a16Bits;
						for (int aRow = 0; aRow < mHeight; aRow++)
						{
							for (int aCol = 0; aCol < mWidth; aCol++)
							{																
								*(aDestPtr++) = *a16SrcPtr;
								++a16SrcPtr;
							}

							aDestPtr += mLockedSurfaceDesc.lPitch/2 - mWidth;
						}
					}
				}
				else
				{
					bool needNewColor = false;
					ulong* aSrcPtr = mBits;
					ushort* a16SrcPtr = a16Bits;
					for (i = 0; i < mWidth*mHeight; i++)
					{
						ulong val = *(aSrcPtr++);
						ushort a16Val = *(a16SrcPtr++);

						int anAlpha = val >> 24;
						if ((anAlpha == 255) && (aTransColor == a16Val))
						{
							needNewColor = true;
							break;
						}
					}

					if (needNewColor)
					{
						int* aUsedColorArray = new int[2048];

						ZeroMemory(aUsedColorArray, 2048*sizeof(int));					

						aSrcPtr = mBits;
						a16SrcPtr = a16Bits;
						for (i = 0; i < mWidth*mHeight; i++)
						{
							ulong val = *(aSrcPtr++);
							ushort a16Val = *(a16SrcPtr++);

							int anAlpha = val >> 24;
							if (anAlpha > 0)
								aUsedColorArray[a16Val/32] |= (1<<(a16Val%32));						
						}

						bool done = false;
						for (int i = 0; i < 2048; i++)
						{
							if (aUsedColorArray[i] != 0xFFFF)
							{
								for (int aBit = 0; aBit < 32; aBit++)														
								{
									if ((aUsedColorArray[i] & (1<<aBit)) == 0)
									{
										aTransColor = i*32+aBit;
										break;
									}
								}
							}

							if (done)
								break;
						}

						delete[] aUsedColorArray;
					}

					if (mBits != NULL)
					{
						ushort* aDestPtr = mSurfaceBits;
						ulong* aSrcPtr = mBits;
						ushort* a16SrcPtr = a16Bits;
						for (int aRow = 0; aRow < mHeight; aRow++)
						{
							for (int aCol = 0; aCol < mWidth; aCol++)
							{
								ulong val = *(aSrcPtr++);

								//*(aDestPtr++) = 0xFF000000 |  ((val >> 24) & 0xFF);

								int anAlpha = val >> 24;
								if (anAlpha < 255)
									*(aDestPtr++) = aTransColor;
								else
									*(aDestPtr++) = *a16SrcPtr;

								++a16SrcPtr;
							}

							aDestPtr += mLockedSurfaceDesc.lPitch/2 - mWidth;
						}
					}
				}
			}
			else
			{
				if (a16Bits != NULL)
				{
					ushort* aDestPtr = mSurfaceBits;						
					ushort* a16SrcPtr = a16Bits;
					for (int aRow = 0; aRow < mHeight; aRow++)
					{
						for (int aCol = 0; aCol < mWidth; aCol++)
						{																
							*(aDestPtr++) = *a16SrcPtr;
							++a16SrcPtr;
						}

						aDestPtr += mLockedSurfaceDesc.lPitch/2 - mWidth;
					}
				}
			}

			delete a16Bits;	

			if (mHasTrans)
			{
				DDCOLORKEY aColorKey = {aTransColor, aTransColor};
				mSurface->SetColorKey(DDCKEY_SRCBLT, &aColorKey);				
			}		
		}
	}
	else if (aNumBits == 32)
	{
		ulong* mSurfaceBits = (ulong*) mLockedSurfaceDesc.lpSurface;		

		if (mSurfaceBits != NULL)
		{
			int i;
			bool firstTrans = true;						
			
			ulong aTransColor = 0;

			if ((mHasTrans) && (mBits != NULL))
			{
				if (mFirstPixelTrans)
				{
					ulong val = *mBits;

					aTransColor = (((val>>rRightShift)<<rLeftShift)&rMask) | 
								  (((val>>gRightShift)<<gLeftShift)&gMask) |
								  (((val>>bRightShift)<<bLeftShift)&bMask);

					if (mBits != NULL)
					{
						ulong* aDestPtr = mSurfaceBits;
						ulong* aSrcPtr = mBits;
						for (int aRow = 0; aRow < mHeight; aRow++)
						{
							for (int aCol = 0; aCol < mWidth; aCol++)
							{				
								ulong val = *(aSrcPtr++);								
								
								*(aDestPtr++) = 
										(((val>>rRightShift)<<rLeftShift)&rMask) | 
										(((val>>gRightShift)<<gLeftShift)&gMask) |
										(((val>>bRightShift)<<bLeftShift)&bMask);
							}

							aDestPtr += mLockedSurfaceDesc.lPitch/4 - mWidth;
						}
					}
				}
				else
				{
					ulong* aSrcPtr = mBits;	
					for (i = 0; i < mWidth*mHeight; i++)
					{			
						ulong val = *(aSrcPtr++);				

						int anAlpha = val >> 24;
						if ((firstTrans) && (anAlpha < 255))
						{
							firstTrans = false;
							aTransColor = val;					
						}
					}

					bool needNewColor = false;
					aSrcPtr = mBits;
					for (i = 0; i < mWidth*mHeight; i++)
					{
						ulong val = *(aSrcPtr++);

						int anAlpha = val >> 24;
						if ((anAlpha == 255) && (aTransColor == (val & 0x00FFFFFF)))
						{
							needNewColor = true;
							break;
						}
					}

					if (needNewColor)
					{
						int* aUsedColorArray = new int[0x80000];

						ZeroMemory(aUsedColorArray, 0x80000*sizeof(int));					

						aSrcPtr = mBits;	
						for (i = 0; i < mWidth*mHeight; i++)
						{
							ulong val = *(aSrcPtr++);						

							int anAlpha = val >> 24;						

							if (anAlpha > 0)
								aUsedColorArray[(val & 0xFFFFFF)/32] |= (1<<(val%32));
						}

						bool done = false;
						for (int i = 0; i < 0x80000; i++)
						{
							if (aUsedColorArray[i] != 0xFFFF)						
							{
								for (int aBit = 0; aBit < 32; aBit++)														
								{
									if ((aUsedColorArray[i] & (1<<aBit)) == 0)								
									{
										aTransColor = i*32+aBit;							
										done = true;
										break;
									}
								}
							}

							if (done)
								break;
						}						

						delete[] aUsedColorArray;
					}

					if (mBits != NULL)
					{
						ulong* aDestPtr = mSurfaceBits;
						ulong* aSrcPtr = mBits;
						for (int aRow = 0; aRow < mHeight; aRow++)
						{
							for (int aCol = 0; aCol < mWidth; aCol++)
							{				
								ulong val = *(aSrcPtr++);

								//*(aDestPtr++) = 0xFF000000 |  ((val >> 24) & 0xFF);

								int anAlpha = val >> 24;
								if (anAlpha < 255)
									*(aDestPtr++) = aTransColor;
								else
								{
									*(aDestPtr++) = 
												(((val>>rRightShift)<<rLeftShift)&rMask) | 
												(((val>>gRightShift)<<gLeftShift)&gMask) |
												(((val>>bRightShift)<<bLeftShift)&bMask);
								}
							}

							aDestPtr += mLockedSurfaceDesc.lPitch/4 - mWidth;
						}
					}
				}

				DDCOLORKEY aColorKey = {aTransColor, aTransColor};
				mSurface->SetColorKey(DDCKEY_SRCBLT, &aColorKey);
			}
			else
			{
				if (mBits != NULL)
				{
					ulong* aDestPtr = mSurfaceBits;
					ulong* aSrcPtr = mBits;
					for (int aRow = 0; aRow < mHeight; aRow++)
					{
						for (int aCol = 0; aCol < mWidth; aCol++)
						{				
							ulong val = *(aSrcPtr++);					

							*(aDestPtr++) = 
										(((val>>rRightShift)<<rLeftShift)&rMask) | 
										(((val>>gRightShift)<<gLeftShift)&gMask) |
										(((val>>bRightShift)<<bLeftShift)&bMask);
						}

						aDestPtr += mLockedSurfaceDesc.lPitch/4 - mWidth;
					}
				}
			}
		}
	}
	else
	{
		return false;
	}

	UnlockSurface();
	
	return true;
}

void DDImage::DeleteDDSurface()
{
	if (mSurface != NULL)
	{
		if ((mColorTable == NULL) && (mBits == NULL) && (mD3DData == NULL))
			GetBits();

		mSurface->Release();
		mSurface = NULL;
	}
}

void DDImage::ReInit()
{
	MemoryImage::ReInit();

	if (mWantDDSurface)
		GenerateDDSurface();
}

void DDImage::PurgeBits()
{
	if (mSurfaceSet)
		return;

	mPurgeBits = true;

	CommitBits();

	if (!mApp->Is3DAccelerated())
	{
		if ((mWantDDSurface) && (GenerateDDSurface()))
		{                        
			delete [] mBits;
			mBits = NULL;

			delete [] mColorIndices;
			mColorIndices = NULL;

			delete [] mColorTable;
			mColorTable = NULL;                        

			return;
		}	
	}
	else // Accelerated
	{
		if (mSurface != NULL)
		{
			GetBits();
			DeleteDDSurface();
		}
	}
	
	MemoryImage::PurgeBits();
}

void DDImage::DeleteAllNonSurfaceData()
{
	delete [] mBits;
	mBits = NULL;

	delete [] mNativeAlphaData;
	mNativeAlphaData = NULL;

	delete [] mRLAdditiveData;
	mRLAdditiveData = NULL;

	delete [] mRLAlphaData;
	mRLAlphaData = NULL;

	delete [] mColorTable;
	mColorTable = NULL;

	delete [] mColorIndices;
	mColorIndices = NULL;
}

void DDImage::DeleteNativeData()
{
	if (mSurfaceSet)
		return;

	MemoryImage::DeleteNativeData();
	DeleteDDSurface();
}

void DDImage::DeleteExtraBuffers()
{
	if (mSurfaceSet)
		return;

	MemoryImage::DeleteExtraBuffers();
	DeleteDDSurface();
}

void DDImage::SetVideoMemory(bool wantVideoMemory)
{
	if (wantVideoMemory != mVideoMemory)
	{		
		mVideoMemory = wantVideoMemory;

		// Make sure that we have the bits
		GetBits();

		DeleteDDSurface();
	}
}

void DDImage::RehupFirstPixelTrans()
{
	if (!GenerateDDSurface())
		return;

	if ((mNoLock) || (!mHasTrans) || (!mFirstPixelTrans))
		return;

//	TDDSurfaceDesc aDesc;
	DDSURFACEDESC aDesc;

	ZeroMemory(&aDesc, sizeof(aDesc));
	aDesc.dwSize = sizeof(aDesc);
    aDesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    HRESULT aResult = mDDInterface->mPrimarySurface->GetSurfaceDesc(&aDesc);
	if (FAILED(aResult))
		return;

	int aNumBits = aDesc.ddpfPixelFormat.dwRGBBitCount;

	if (aNumBits == 16)
	{
		if (!LockSurface())
			return;

		ushort* aSurfaceBits = (ushort*) mLockedSurfaceDesc.lpSurface;	

		ushort aTransColor = *aSurfaceBits;

		DDCOLORKEY aColorKey = {aTransColor, aTransColor};
		mSurface->SetColorKey(DDCKEY_SRCBLT, &aColorKey);	

		UnlockSurface();
	}
	else if ((aNumBits == 24) || (aNumBits == 32))
	{
		if (!LockSurface())
			return;

		ulong* aSurfaceBits = (ulong*) mLockedSurfaceDesc.lpSurface;		

		ulong aTransColor = *aSurfaceBits;

		DDCOLORKEY aColorKey = {aTransColor, aTransColor};
		mSurface->SetColorKey(DDCKEY_SRCBLT, &aColorKey);		

		UnlockSurface();
	}
}

LPDIRECTDRAWSURFACE DDImage::GetSurface()
{
	//TODO: Log if generate surface fails

	if (mSurface == NULL)
		GenerateDDSurface();

	return mSurface;
}

bool DDImage::PolyFill3D(const Point theVertices[], int theNumVertices, const Rect *theClipRect, const Color &theColor, int theDrawMode, int tx, int ty, bool convex)
{
	if (Check3D(this))
	{
		mDDInterface->mD3DInterface->FillPoly(theVertices,theNumVertices,theClipRect,theColor,theDrawMode,tx,ty);
		return true;
	}
	else
		return false;
}

void DDImage::FillRect(const Rect& theRect, const Color& theColor, int theDrawMode)
{
	if (Check3D(this))
	{
		mDDInterface->mD3DInterface->FillRect(theRect,theColor,theDrawMode);
		return;
	}

	CommitBits();
	if ((mDrawToBits) || (mHasAlpha) || ((mHasTrans) && (!mFirstPixelTrans)) || (mDDInterface->mIs3D))
	{
		MemoryImage::FillRect(theRect, theColor, theDrawMode);
		return;
	}	

	switch (theDrawMode)
	{
	case Graphics::DRAWMODE_NORMAL:
		NormalFillRect(theRect, theColor);
		break;
	case Graphics::DRAWMODE_ADDITIVE:
		AdditiveFillRect(theRect, theColor);
		break;
	}

	DeleteAllNonSurfaceData();
}

void DDImage::NormalDrawLine(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor)
{
	if (mNoLock)
		return;

	double aMinX = std::min(theStartX, theEndX);
	double aMinY = std::min(theStartY, theEndY);
	double aMaxX = std::max(theStartX, theEndX);
	double aMaxY = std::max(theStartY, theEndY);

	LPDIRECTDRAWSURFACE aSurface = GetSurface();
	(void)aSurface; // Unused

	if (!LockSurface())
		return;	

	ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
	ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
	ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;

	ulong aRRoundAdd = aRMask >> 1;
	ulong aGRoundAdd = aGMask >> 1;
	ulong aBRoundAdd = aBMask >> 1;
	
	if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 16)
	{
		if (theColor.mAlpha == 255)
		{
			ushort aColor = (ushort)
				(((((theColor.mRed * aRMask) + aRRoundAdd) >> 8) & aRMask) |
				((((theColor.mGreen * aGMask) + aGRoundAdd) >> 8) & aGMask) |
				((((theColor.mBlue * aBMask) + aBRoundAdd) >> 8) & aBMask));

			double dv = theEndY - theStartY;
			double dh = theEndX - theStartX;
			// int minG, maxG unused
			int G, DeltaG1, DeltaG2;

			double swap;
			int inc = 1;
			int aCurX;
			int aCurY;
			int aRowWidth = mLockedSurfaceDesc.lPitch/2;
			int aRowAdd = aRowWidth;

			if (fabs(dv) < fabs(dh))
			{
				// Mostly horizontal
				if (dh < 0)
				{
					dh = -dh;
					dv = -dv;
					swap = theEndY;
					theEndY = theStartY;
					theStartY = swap;
					swap = theEndX;
					theEndX = theStartX;
					theStartX = swap;
				}
				if (dv < 0)
				{
					dv = -dv;
					inc = -1;
					aRowAdd = -aRowAdd;
				}

				ushort* aDestPixels = ((ushort*) mLockedSurfaceDesc.lpSurface) + ((int) theStartY * aRowWidth) + (int) theStartX;
				*aDestPixels = aColor;
				aDestPixels++;

				aCurY = (int) theStartY;
				aCurX = (int) theStartX + 1;

				G = 2 * dv - dh;
				DeltaG1 = 2 * (dv - dh);
				DeltaG2 = 2 * dv;
				
				G += DeltaG2 * (theStartY - (int) theStartY);

				while (aCurX <= theEndX)
				{
					if (G > 0)
					{
						G += DeltaG1;
						aCurY += inc;
						aDestPixels += aRowAdd;

						if (aCurX<aMinX || aCurY<aMinY || aCurX>aMaxX || aCurY>aMaxY)
							break;
					}
					else
						G += DeltaG2;
					
					*aDestPixels = aColor;

					aCurX++;
					aDestPixels++;
				}
			}
			else
			{
				// Mostly vertical
				if ( dv < 0 )
				{
					dh = -dh;
					dv = -dv;
					swap = theEndY;
					theEndY = theStartY;
					theStartY = swap;
					swap = theEndX;
					theEndX = theStartX;
					theStartX = swap;
				}

				if (dh < 0)
				{
					dh = -dh;
					inc = -1;
				}

				ushort* aDestPixels = ((ushort*) mLockedSurfaceDesc.lpSurface) + ((int) theStartY * mLockedSurfaceDesc.lPitch/2) + (int) theStartX;
				*aDestPixels = aColor;
				aDestPixels += aRowAdd;

				aCurX = theStartX;
				aCurY = theStartY + 1;

				G = 2 * dh - dv;
				// minG = maxG = G; // unused
				DeltaG1 = 2 * ( dh - dv );
				DeltaG2 = 2 * dh;

				G += DeltaG2 * (theStartX - (int) theStartX);

				while (aCurY <= theEndY)
				{
					if ( G > 0 )
					{						
						G += DeltaG1;
						aCurX += inc;
						aDestPixels += inc;

						if (aCurX<aMinX || aCurY<aMinY || aCurX>aMaxX || aCurY>aMaxY)
							break;
					}
					else
						G += DeltaG2;
					
					*aDestPixels = aColor;

					aCurY++;
					aDestPixels += aRowAdd;
				}
			}
		}
		else
		{
			ushort src = 
				((((((theColor.mRed * theColor.mAlpha + 0x80) >> 8) * aRMask) + aRRoundAdd) >> 8) & aRMask) +
				((((((theColor.mGreen * theColor.mAlpha + 0x80) >> 8) * aGMask) + aGRoundAdd) >> 8) & aGMask) +
				((((((theColor.mBlue * theColor.mAlpha + 0x80) >> 8) * aBMask) + aBRoundAdd) >> 8) & aBMask);
			int oma = 256 - theColor.mAlpha;

			double dv = theEndY - theStartY;
			double dh = theEndX - theStartX;
			// int minG, maxG unused
			int G, DeltaG1, DeltaG2;
			double swap;
			int inc = 1;
			int aCurX;
			int aCurY;
			int aRowWidth = mLockedSurfaceDesc.lPitch/2;
			int aRowAdd = aRowWidth;

			if (abs(dv) < abs(dh))
			{
				// Mostly horizontal
				if (dh < 0)
				{
					dh = -dh;
					dv = -dv;
					swap = theEndY;
					theEndY = theStartY;
					theStartY = swap;
					swap = theEndX;
					theEndX = theStartX;
					theStartX = swap;
				}
				if (dv < 0)
				{
					dv = -dv;
					inc = -1;
					aRowAdd = -aRowAdd;
				}

				ushort* aDestPixels = ((ushort*) mLockedSurfaceDesc.lpSurface) + ((int) theStartY * aRowWidth) + (int) theStartX;
				ushort dest = *aDestPixels;
				*(aDestPixels++) = src + 
					(((((dest & aRMask) * oma) + aRRoundAdd) >> 8) & aRMask) +
					(((((dest & aGMask) * oma) + aGRoundAdd) >> 8) & aGMask) +
					(((((dest & aBMask) * oma) + aBRoundAdd) >> 8) & aBMask);				

				aCurY = theStartY;
				aCurX = theStartX + 1;

				G = 2 * dv - dh;
				DeltaG1 = 2 * (dv - dh);
				DeltaG2 = 2 * dv;

				G += DeltaG2 * (theStartY - (int) theStartY);

				while (aCurX <= theEndX)
				{
					if (G > 0)
					{
						G += DeltaG1;
						aCurY += inc;
						aDestPixels += aRowAdd;

						if (aCurX<aMinX || aCurY<aMinY || aCurX>aMaxX || aCurY>aMaxY)
							break;
					}
					else
						G += DeltaG2;
					
					dest = *aDestPixels;
					*(aDestPixels++) = src + 
						(((((dest & aRMask) * oma) + aRRoundAdd) >> 8) & aRMask) +
						(((((dest & aGMask) * oma) + aGRoundAdd) >> 8) & aGMask) +
						(((((dest & aBMask) * oma) + aBRoundAdd) >> 8) & aBMask);					

					aCurX++;					
				}
			}
			else
			{
				// Mostly vertical
				if ( dv < 0 )
				{
					dh = -dh;
					dv = -dv;
					swap = theEndY;
					theEndY = theStartY;
					theStartY = swap;
					swap = theEndX;
					theEndX = theStartX;
					theStartX = swap;
				}

				if (dh < 0)
				{
					dh = -dh;
					inc = -1;
				}

				ushort* aDestPixels = ((ushort*) mLockedSurfaceDesc.lpSurface) + ((int) theStartY * mLockedSurfaceDesc.lPitch/2) + (int) theStartX;
				ushort dest = *aDestPixels;
				*aDestPixels = src + 
					(((((dest & aRMask) * oma) + aRRoundAdd) >> 8) & aRMask) +
					(((((dest & aGMask) * oma) + aGRoundAdd) >> 8) & aGMask) +
					(((((dest & aBMask) * oma) + aBRoundAdd) >> 8) & aBMask);
				aDestPixels += aRowAdd;

				aCurX = theStartX;
				aCurY = theStartY + 1;

				G = 2 * dh - dv;
				// minG = maxG = G; // unused
				DeltaG1 = 2 * ( dh - dv );
				DeltaG2 = 2 * dh;

				G += DeltaG2 * (theStartX - (int) theStartX);

				while (aCurY <= theEndY)
				{
					if ( G > 0 )
					{
						G += DeltaG1;
						aCurX += inc;
						aDestPixels += inc;

						if (aCurX<aMinX || aCurY<aMinY || aCurX>aMaxX || aCurY>aMaxY)
							break;
					}
					else
						G += DeltaG2;
					
					dest = *aDestPixels;
					*aDestPixels = src + 
						(((((dest & aRMask) * oma) + aRRoundAdd) >> 8) & aRMask) +
						(((((dest & aGMask) * oma) + aGRoundAdd) >> 8) & aGMask) +
						(((((dest & aBMask) * oma) + aBRoundAdd) >> 8) & aBMask);

					aCurY++;
					aDestPixels += aRowAdd;
				}
			}
		}
	}
	else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
	{
		if (theColor.mAlpha == 255)
		{
			ulong aColor = 
				((((theColor.mRed * aRMask) + aRRoundAdd) >> 8) & aRMask) |
				((((theColor.mGreen * aGMask) + aGRoundAdd) >> 8) & aGMask) |
				((((theColor.mBlue * aBMask) + aBRoundAdd) >> 8) & aBMask);

			double dv = theEndY - theStartY;
			double dh = theEndX - theStartX;
			// int minG, maxG // unused
			int G, DeltaG1, DeltaG2;
			double swap;
			int inc = 1;
			int aCurX;
			int aCurY;
			int aRowWidth = mLockedSurfaceDesc.lPitch/4;
			int aRowAdd = aRowWidth;;

			if (abs(dv) < abs(dh))
			{
				// Mostly horizontal
				if (dh < 0)
				{
					dh = -dh;
					dv = -dv;
					swap = theEndY;
					theEndY = theStartY;
					theStartY = swap;
					swap = theEndX;
					theEndX = theStartX;
					theStartX = swap;
				}
				if (dv < 0)
				{
					dv = -dv;
					inc = -1;
					aRowAdd = -aRowAdd;
				}

				ulong* aDestPixels = ((ulong*) mLockedSurfaceDesc.lpSurface) + ((int) theStartY * aRowWidth) + (int) theStartX;
				*aDestPixels = aColor;
				aDestPixels++;

				aCurY = theStartY;
				aCurX = theStartX + 1;

				G = 2 * dv - dh;
				DeltaG1 = 2 * (dv - dh);
				DeltaG2 = 2 * dv;

				G += DeltaG2 * (theStartY - (int) theStartY);

				while (aCurX <= theEndX)
				{
					if (G > 0)
					{
						G += DeltaG1;
						aCurY += inc;
						aDestPixels += aRowAdd;

						if (aCurX<aMinX || aCurY<aMinY || aCurX>aMaxX || aCurY>aMaxY)
							break;
					}
					else
						G += DeltaG2;
					
					*aDestPixels = aColor;

					aCurX++;
					aDestPixels++;
				}
			}
			else
			{
				// Mostly vertical
				if ( dv < 0 )
				{
					dh = -dh;
					dv = -dv;
					swap = theEndY;
					theEndY = theStartY;
					theStartY = swap;
					swap = theEndX;
					theEndX = theStartX;
					theStartX = swap;
				}

				if (dh < 0)
				{
					dh = -dh;
					inc = -1;
				}

				ulong* aDestPixels = ((ulong*) mLockedSurfaceDesc.lpSurface) + ((int) theStartY * aRowWidth) + (int) theStartX;
				*aDestPixels = aColor;
				aDestPixels += aRowAdd;

				aCurX = theStartX;
				aCurY = theStartY + 1;

				G = 2 * dh - dv;
				// minG = maxG = G; // unused
				DeltaG1 = 2 * ( dh - dv );
				DeltaG2 = 2 * dh;

				G += DeltaG2 * (theStartX - (int) theStartX);

				while (aCurY <= theEndY)
				{
					if ( G > 0 )
					{
						G += DeltaG1;
						aCurX += inc;
						aDestPixels += inc;

						if (aCurX<aMinX || aCurY<aMinY || aCurX>aMaxX || aCurY>aMaxY)
							break;
					}
					else
						G += DeltaG2;
					
					*aDestPixels = aColor;

					aCurY++;
					aDestPixels += aRowAdd;
				}
			}
		}
		else
		{
			ulong src = 
				((((((theColor.mRed * theColor.mAlpha + 0x80) >> 8) * aRMask) + aRRoundAdd) >> 8) & aRMask) +
				((((((theColor.mGreen * theColor.mAlpha + 0x80) >> 8) * aGMask) + aGRoundAdd) >> 8) & aGMask) +
				((((((theColor.mBlue * theColor.mAlpha + 0x80) >> 8) * aBMask) + aBRoundAdd) >> 8) & aBMask);
			int oma = 256 - theColor.mAlpha;

			double dv = theEndY - theStartY;
			double dh = theEndX - theStartX;
			// int minG, maxG // unused
			int G, DeltaG1, DeltaG2;
			double swap;
			int inc = 1;
			int aCurX;
			int aCurY;
			int aRowWidth = mLockedSurfaceDesc.lPitch/4;
			int aRowAdd = aRowWidth;

			if (abs(dv) < abs(dh))
			{
				// Mostly horizontal
				if (dh < 0)
				{
					dh = -dh;
					dv = -dv;
					swap = theEndY;
					theEndY = theStartY;
					theStartY = swap;
					swap = theEndX;
					theEndX = theStartX;
					theStartX = swap;
				}
				if (dv < 0)
				{
					dv = -dv;
					inc = -1;
					aRowAdd = -aRowAdd;
				}

				ulong* aDestPixels = ((ulong*) mLockedSurfaceDesc.lpSurface) + ((int) theStartY * aRowWidth) + (int) theStartX;
				ulong dest = *aDestPixels;
				*(aDestPixels++) = src + 
					(((((dest & aRMask) * oma) + aRRoundAdd) >> 8) & aRMask) +
					(((((dest & aGMask) * oma) + aGRoundAdd) >> 8) & aGMask) +
					(((((dest & aBMask) * oma) + aBRoundAdd) >> 8) & aBMask);				

				aCurY = theStartY;
				aCurX = theStartX + 1;

				G = 2 * dv - dh;
				DeltaG1 = 2 * (dv - dh);
				DeltaG2 = 2 * dv;

				G += DeltaG2 * (theStartX - (int) theStartX);

				while (aCurX <= theEndX)
				{
					if (G > 0)
					{
						G += DeltaG1;
						aCurY += inc;
						aDestPixels += aRowAdd;

						if (aCurX<aMinX || aCurY<aMinY || aCurX>aMaxX || aCurY>aMaxY)
							break;
					}
					else
						G += DeltaG2;
					
					dest = *aDestPixels;
					*(aDestPixels++) = src + 
						(((((dest & aRMask) * oma) + aRRoundAdd) >> 8) & aRMask) +
						(((((dest & aGMask) * oma) + aGRoundAdd) >> 8) & aGMask) +
						(((((dest & aBMask) * oma) + aBRoundAdd) >> 8) & aBMask);					

					aCurX++;					
				}
			}
			else
			{
				// Mostly vertical
				if ( dv < 0 )
				{
					dh = -dh;
					dv = -dv;
					swap = theEndY;
					theEndY = theStartY;
					theStartY = swap;
					swap = theEndX;
					theEndX = theStartX;
					theStartX = swap;
				}

				if (dh < 0)
				{
					dh = -dh;
					inc = -1;
				}

				ulong* aDestPixels = ((ulong*) mLockedSurfaceDesc.lpSurface) + ((int) theStartY * aRowWidth) + (int) theStartX;
				ulong dest = *aDestPixels;
				*aDestPixels = src + 
					(((((dest & aRMask) * oma) + aRRoundAdd) >> 8) & aRMask) +
					(((((dest & aGMask) * oma) + aGRoundAdd) >> 8) & aGMask) +
					(((((dest & aBMask) * oma) + aBRoundAdd) >> 8) & aBMask);
				aDestPixels += aRowAdd;

				aCurX = theStartX;
				aCurY = theStartY + 1;

				G = 2 * dh - dv;
				// minG = maxG = G; // unused
				DeltaG1 = 2 * ( dh - dv );
				DeltaG2 = 2 * dh;

				G += DeltaG2 * (theStartX - (int) theStartX);

				while (aCurY <= theEndY)
				{
					if ( G > 0 )
					{
						G += DeltaG1;
						aCurX += inc;
						aDestPixels += inc;

						if (aCurX<aMinX || aCurY<aMinY || aCurX>aMaxX || aCurY>aMaxY)
							break;
					}
					else
						G += DeltaG2;
					
					dest = *aDestPixels;
					*aDestPixels = src + 
						(((((dest & aRMask) * oma) + aRRoundAdd) >> 8) & aRMask) +
						(((((dest & aGMask) * oma) + aGRoundAdd) >> 8) & aGMask) +
						(((((dest & aBMask) * oma) + aBRoundAdd) >> 8) & aBMask);

					aCurY++;
					aDestPixels += aRowAdd;
				}
			}
		}
	}

	UnlockSurface();
}

void DDImage::AdditiveDrawLine(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor)
{
	if (mNoLock)
		return;

	double aMinX = std::min(theStartX, theEndX);
	double aMinY = std::min(theStartY, theEndY);
	double aMaxX = std::max(theStartX, theEndX);
	double aMaxY = std::max(theStartY, theEndY);

	LPDIRECTDRAWSURFACE aSurface = GetSurface();
	(void)aSurface; // unused

	if (!LockSurface())
		return;

	ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
	ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
	ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;

	// unused
	// ulong aRRoundAdd = aRMask >> 1;
	// ulong aGRoundAdd = aGMask >> 1;
	// ulong aBRoundAdd = aBMask >> 1;

	int aRedShift = mDDInterface->mRedShift;
	int aGreenShift = mDDInterface->mGreenShift;
	int aBlueShift = mDDInterface->mBlueShift;

	int* aMaxRedTable = mDDInterface->mRedAddTable;
	int* aMaxGreenTable = mDDInterface->mGreenAddTable;
	int* aMaxBlueTable = mDDInterface->mBlueAddTable;
	
	if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 16)
	{
		ushort rc = ((theColor.mRed * theColor.mAlpha) / 255) >> (8-mDDInterface->mRedBits);
		ushort gc = ((theColor.mGreen * theColor.mAlpha) / 255) >> (8-mDDInterface->mGreenBits);
		ushort bc = ((theColor.mBlue * theColor.mAlpha) / 255) >> (8-mDDInterface->mBlueBits);

		double dv = theEndY - theStartY;
		double dh = theEndX - theStartX;
		// int minG, maxG // unused
		int G, DeltaG1, DeltaG2;
		double swap;
		int inc = 1;
		int aCurX;
		int aCurY;
		int aRowWidth = mLockedSurfaceDesc.lPitch/2;
		int aRowAdd = aRowWidth;

		if (abs(dv) < abs(dh))
		{
			// Mostly horizontal
			if (dh < 0)
			{
				dh = -dh;
				dv = -dv;
				swap = theEndY;
				theEndY = theStartY;
				theStartY = swap;
				swap = theEndX;
				theEndX = theStartX;
				theStartX = swap;
			}
			if (dv < 0)
			{
				dv = -dv;
				inc = -1;
				aRowAdd = -aRowAdd;
			}

			ushort* aDestPixels = ((ushort*) mLockedSurfaceDesc.lpSurface) + ((int) theStartY * aRowWidth) + (int) theStartX;
			ushort dest = *aDestPixels;

			int r = aMaxRedTable[((dest & aRMask) >> aRedShift) + rc];
			int g = aMaxGreenTable[((dest & aGMask) >> aGreenShift) + gc];
			int b = aMaxBlueTable[((dest & aBMask) >> aBlueShift) + bc];

			*(aDestPixels++) = 
				(r << aRedShift) |
				(g << aGreenShift) |
				(b << aBlueShift);

			aCurY = theStartY;
			aCurX = theStartX + 1;

			G = 2 * dv - dh;
			DeltaG1 = 2 * (dv - dh);
			DeltaG2 = 2 * dv;

			G += DeltaG2 * (theStartY - (int) theStartY);

			while (aCurX <= theEndX)
			{
				if (G > 0)
				{					
					G += DeltaG1;
					aCurY += inc;
					aDestPixels += aRowAdd;

					if (aCurX<aMinX || aCurY<aMinY || aCurX>aMaxX || aCurY>aMaxY)
						break;
				}
				else
					G += DeltaG2;
				
				dest = *aDestPixels;

				r = aMaxRedTable[((dest & aRMask) >> aRedShift) + rc];
				g = aMaxGreenTable[((dest & aGMask) >> aGreenShift) + gc];
				b = aMaxBlueTable[((dest & aBMask) >> aBlueShift) + bc];

				*(aDestPixels++) = 
					(r << aRedShift) |
					(g << aGreenShift) |
					(b << aBlueShift);

				aCurX++;				
			}
		}
		else
		{
			// Mostly vertical
			if ( dv < 0 )
			{
				dh = -dh;
				dv = -dv;
				swap = theEndY;
				theEndY = theStartY;
				theStartY = swap;
				swap = theEndX;
				theEndX = theStartX;
				theStartX = swap;
			}

			if (dh < 0)
			{
				dh = -dh;
				inc = -1;
			}

			ushort* aDestPixels = ((ushort*) mLockedSurfaceDesc.lpSurface) + ((int) theStartY * mLockedSurfaceDesc.lPitch/2) + (int) theStartX;
			
			ushort dest = *aDestPixels;

			int r = aMaxRedTable[((dest & aRMask) >> aRedShift) + rc];
			int g = aMaxGreenTable[((dest & aGMask) >> aGreenShift) + gc];
			int b = aMaxBlueTable[((dest & aBMask) >> aBlueShift) + bc];

			*aDestPixels = 
				(r << aRedShift) |
				(g << aGreenShift) |
				(b << aBlueShift);

			aDestPixels += aRowAdd;

			aCurX = theStartX;
			aCurY = theStartY + 1;

			G = 2 * dh - dv;
			// minG = maxG = G; // unused
			DeltaG1 = 2 * ( dh - dv );
			DeltaG2 = 2 * dh;	
			
			G += DeltaG2 * (theStartX - (int) theStartX);

			while (aCurY <= theEndY)
			{
				if ( G > 0 )
				{
					G += DeltaG1;
					aCurX += inc;
					aDestPixels += inc;

					if (aCurX<aMinX || aCurY<aMinY || aCurX>aMaxX || aCurY>aMaxY)
						break;
				}
				else
					G += DeltaG2;
				
				dest = *aDestPixels;

				r = aMaxRedTable[((dest & aRMask) >> aRedShift) + rc];
				g = aMaxGreenTable[((dest & aGMask) >> aGreenShift) + gc];
				b = aMaxBlueTable[((dest & aBMask) >> aBlueShift) + bc];

				*aDestPixels = 
					(r << aRedShift) |
					(g << aGreenShift) |
					(b << aBlueShift);

				aCurY++;
				aDestPixels += aRowAdd;
			}
		}
	}
	else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
	{
		ulong rc = ((theColor.mRed * theColor.mAlpha) / 255) >> (8-mDDInterface->mRedBits);
		ulong gc = ((theColor.mGreen * theColor.mAlpha) / 255) >> (8-mDDInterface->mGreenBits);
		ulong bc = ((theColor.mBlue * theColor.mAlpha) / 255) >> (8-mDDInterface->mBlueBits);

		double dv = theEndY - theStartY;
		double dh = theEndX - theStartX;
		// int minG, maxG // unused
		int G, DeltaG1, DeltaG2;
		double swap;
		int inc = 1;
		int aCurX;
		int aCurY;
		int aRowWidth = mLockedSurfaceDesc.lPitch/4;
		int aRowAdd = aRowWidth;

		if (abs(dv) < abs(dh))
		{
			// Mostly horizontal
			if (dh < 0)
			{
				dh = -dh;
				dv = -dv;
				swap = theEndY;
				theEndY = theStartY;
				theStartY = swap;
				swap = theEndX;
				theEndX = theStartX;
				theStartX = swap;
			}

			if (dv < 0)
			{
				dv = -dv;
				inc = -1;
				aRowAdd = -aRowAdd;
			}

			ulong* aDestPixels = ((ulong*) mLockedSurfaceDesc.lpSurface) + ((int) theStartY * aRowWidth) + (int) theStartX;
			ulong dest = *aDestPixels;

			int r = aMaxRedTable[((dest & aRMask) >> aRedShift) + rc];
			int g = aMaxGreenTable[((dest & aGMask) >> aGreenShift) + gc];
			int b = aMaxBlueTable[((dest & aBMask) >> aBlueShift) + bc];

			*(aDestPixels++) = 
				(r << aRedShift) |
				(g << aGreenShift) |
				(b << aBlueShift);

			aCurY = theStartY;
			aCurX = theStartX + 1;

			G = 2 * dv - dh;
			DeltaG1 = 2 * (dv - dh);
			DeltaG2 = 2 * dv;			

			while (aCurX <= theEndX)
			{
				if (G > 0)
				{
					G += DeltaG1;
					aCurY += inc;
					aDestPixels += aRowAdd;

					if (aCurX<aMinX || aCurY<aMinY || aCurX>aMaxX || aCurY>aMaxY)
						break;
				}
				else
					G += DeltaG2;
				
				dest = *aDestPixels;

				r = aMaxRedTable[((dest & aRMask) >> aRedShift) + rc];
				g = aMaxGreenTable[((dest & aGMask) >> aGreenShift) + gc];
				b = aMaxBlueTable[((dest & aBMask) >> aBlueShift) + bc];

				*(aDestPixels++) = 
					(r << aRedShift) |
					(g << aGreenShift) |
					(b << aBlueShift);

				aCurX++;				
			}
		}
		else
		{
			// Mostly vertical
			if ( dv < 0 )
			{
				dh = -dh;
				dv = -dv;
				swap = theEndY;
				theEndY = theStartY;
				theStartY = swap;
				swap = theEndX;
				theEndX = theStartX;
				theStartX = swap;
			}

			if (dh < 0)
			{
				dh = -dh;
				inc = -1;
			}

			ulong* aDestPixels = ((ulong*) mLockedSurfaceDesc.lpSurface) + ((int) theStartY * mLockedSurfaceDesc.lPitch/4) + (int) theStartX;
			
			ulong dest = *aDestPixels;

			int r = aMaxRedTable[((dest & aRMask) >> aRedShift) + rc];
			int g = aMaxGreenTable[((dest & aGMask) >> aGreenShift) + gc];
			int b = aMaxBlueTable[((dest & aBMask) >> aBlueShift) + bc];

			*aDestPixels = 
				(r << aRedShift) |
				(g << aGreenShift) |
				(b << aBlueShift);

			aDestPixels += aRowAdd;

			aCurX = theStartX;
			aCurY = theStartY + 1;

			G = 2 * dh - dv;
			// minG = maxG = G; // unused
			DeltaG1 = 2 * ( dh - dv );
			DeltaG2 = 2 * dh;
			while (aCurY <= theEndY)
			{
				if ( G > 0 )
				{
					G += DeltaG1;
					aCurX += inc;
					aDestPixels += inc;

					if (aCurX<aMinX || aCurY<aMinY || aCurX>aMaxX || aCurY>aMaxY)
						break;
				}
				else
					G += DeltaG2;
				
				dest = *aDestPixels;

				r = aMaxRedTable[((dest & aRMask) >> aRedShift) + rc];
				g = aMaxGreenTable[((dest & aGMask) >> aGreenShift) + gc];
				b = aMaxBlueTable[((dest & aBMask) >> aBlueShift) + bc];

				*aDestPixels = 
					(r << aRedShift) |
					(g << aGreenShift) |
					(b << aBlueShift);

				aCurY++;
				aDestPixels += aRowAdd;
			}
		}
	}

	UnlockSurface();
}

void DDImage::DrawLine(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor, int theDrawMode)
{	
	if (Check3D(this))
	{
		mDDInterface->mD3DInterface->DrawLine(theStartX,theStartY,theEndX,theEndY,theColor,theDrawMode);
		return;
	}

	if ((mDrawToBits) || (mHasAlpha) || (mHasTrans) || (mDDInterface->mIs3D))
	{
		MemoryImage::DrawLine(theStartX, theStartY, theEndX, theEndY, theColor, theDrawMode);
		return;
	}

	if (theStartY == theEndY)
	{
		int aStartX = std::min(theStartX, theEndX);
		int aEndX = std::max(theStartX, theEndX);

		FillRect(Rect(aStartX, theStartY, aEndX-aStartX+1, theEndY-theStartY+1), theColor, theDrawMode);
		return;
	}
	else if (theStartX == theEndX)
	{
		int aStartY = std::min(theStartY, theEndY);
		int aEndY = std::max(theStartY, theEndY);

		FillRect(Rect(theStartX, aStartY, theEndX-theStartX+1, aEndY-aStartY+1), theColor, theDrawMode);
		return;
	}

	CommitBits();	

	switch (theDrawMode)
	{
	case Graphics::DRAWMODE_NORMAL:
		NormalDrawLine(theStartX, theStartY, theEndX, theEndY, theColor);
		break;
	case Graphics::DRAWMODE_ADDITIVE:
		AdditiveDrawLine(theStartX, theStartY, theEndX, theEndY, theColor);
		break;
	}

	DeleteAllNonSurfaceData();
}

void DDImage::NormalDrawLineAA(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor)
{
	LPDIRECTDRAWSURFACE aSurface = GetSurface();
	(void)aSurface; // unused

	if (!LockSurface())
		return;

	ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
	ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
	ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;
	ulong color = (((theColor.mRed * aRMask) >> 8) & aRMask) |
					(((theColor.mGreen * aGMask) >> 8) & aGMask) |
					(((theColor.mBlue * aBMask) >> 8) & aBMask);

	int aX0 = (int)theStartX, aX1 = (int)theEndX;
	int aY0 = (int)theStartY, aY1 = (int)theEndY;
	int aXinc = 1;
	if (aY0 > aY1)
	{
		int aTempX = aX0, aTempY = aY0;
		aX0 = aX1; aY0 = aY1;
		aX1 = aTempX; aY1 = aTempY;
		double aTempXd = theStartX, aTempYd = theStartY;
		theStartX = theEndX; theStartY = theEndY;
		theEndX = aTempXd; theEndY = aTempYd;
	}

	int dx = aX1 - aX0;
	int dy = aY1 - aY0;
	double dxd = theEndX - theStartX;
	double dyd = theEndY - theStartY;
	if (dx < 0)
	{
		dx = -dx;
		aXinc = -1;
		dxd = -dxd;
	}

	if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
	{
		ulong* aBits = (ulong*)mLockedSurfaceDesc.lpSurface;
#ifdef OPTIMIZE_SOFTWARE_DRAWING
		if (theColor.mAlpha != 255)
		{
			#define PIXEL_TYPE			ulong
			#define CALC_WEIGHT_A(w)	(((w) * (theColor.mAlpha+1)) >> 8)
			#define BLEND_PIXEL(p)		\
						*(p) =			\
								((((color & 0xFF00FF) * a + (dest & 0xFF00FF) * oma) >> 8) & 0xFF00FF) |\
								((((color & 0x00FF00) * a + (dest & 0x00FF00) * oma) >> 8) & 0x00FF00);
			const int STRIDE = mLockedSurfaceDesc.lPitch / sizeof(PIXEL_TYPE);

			#include "GENERIC_DrawLineAA.inc"

			#undef PIXEL_TYPE
			#undef CALC_WEIGHT_A
			#undef BLEND_PIXEL
		}
		else
		{
			#define PIXEL_TYPE			ulong
			#define CALC_WEIGHT_A(w)	(w)
			#define BLEND_PIXEL(p)		\
						*(p) =			\
								((((color & 0xFF00FF) * a + (dest & 0xFF00FF) * oma) >> 8) & 0xFF00FF) |\
								((((color & 0x00FF00) * a + (dest & 0x00FF00) * oma) >> 8) & 0x00FF00);
			const int STRIDE = mLockedSurfaceDesc.lPitch / sizeof(PIXEL_TYPE);

			#include "GENERIC_DrawLineAA.inc"

			#undef PIXEL_TYPE
			#undef CALC_WEIGHT_A
			#undef BLEND_PIXEL
		}
#else
		if (theColor.mAlpha != 255)
		{
			#define PIXEL_TYPE			ulong
			#define CALC_WEIGHT_A(w)	(((w) * (theColor.mAlpha+1)) >> 8)
			#define BLEND_PIXEL(p)		\
						*(p) =			\
								((((color & aRMask) * a + (dest & aRMask) * oma) >> 8) & aRMask) |\
								((((color & aGMask) * a + (dest & aGMask) * oma) >> 8) & aGMask) |\
								((((color & aBMask) * a + (dest & aBMask) * oma) >> 8) & aBMask);
			const int STRIDE = mLockedSurfaceDesc.lPitch / sizeof(PIXEL_TYPE);

			#include "GENERIC_DrawLineAA.inc"

			#undef PIXEL_TYPE
			#undef CALC_WEIGHT_A
			#undef BLEND_PIXEL
		}
		else
		{
			#define PIXEL_TYPE			ulong
			#define CALC_WEIGHT_A(w)	(w)
			#define BLEND_PIXEL(p)		\
						*(p) =			\
								((((color & aRMask) * a + (dest & aRMask) * oma) >> 8) & aRMask) |\
								((((color & aGMask) * a + (dest & aGMask) * oma) >> 8) & aGMask) |\
								((((color & aBMask) * a + (dest & aBMask) * oma) >> 8) & aBMask);
			const int STRIDE = mLockedSurfaceDesc.lPitch / sizeof(PIXEL_TYPE);

			#include "GENERIC_DrawLineAA.inc"

			#undef PIXEL_TYPE
			#undef CALC_WEIGHT_A
			#undef BLEND_PIXEL
		}
#endif
	}
	else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 16)
	{
		ushort* aBits = (ushort*)mLockedSurfaceDesc.lpSurface;
#ifdef OPTIMIZE_SOFTWARE_DRAWING
		if (aGMask == 0x3E0) // 5-5-5
		{
			#define PIXEL_TYPE			ushort
			#define BLEND_PIXEL(p)		\
			{\
				a >>= 3;\
				oma >>= 3;\
				ulong _src = (((color | (color << 16)) & 0x3E07C1F) * a >> 5) & 0x3E07C1F;\
				ulong _dest = (((dest | (dest << 16)) & 0x3E07C1F) * oma >> 5) & 0x3E07C1F;\
				*(p) = (_src | (_src>>16)) + (_dest | (_dest>>16));\
			}
			const int STRIDE = mLockedSurfaceDesc.lPitch / sizeof(PIXEL_TYPE);
			if (theColor.mAlpha != 255)
			{
				#define CALC_WEIGHT_A(w)	(((w) * (theColor.mAlpha+1)) >> 8)
				#include "GENERIC_DrawLineAA.inc"
				#undef CALC_WEIGHT_A
			}
			else
			{
				#define CALC_WEIGHT_A(w)	(w)
				#include "GENERIC_DrawLineAA.inc"
				#undef CALC_WEIGHT_A
			}
			#undef PIXEL_TYPE
			#undef BLEND_PIXEL
		}
		else if (aGMask == 0x7E0) // 5-6-5
		{
			#define PIXEL_TYPE			ushort
			#define BLEND_PIXEL(p)		\
			{\
				a >>= 3;\
				oma >>= 3;\
				ulong _src = (((color | (color << 16)) & 0x7E0F81F) * a >> 5) & 0x7E0F81F;\
				ulong _dest = (((dest | (dest << 16)) & 0x7E0F81F) * oma >> 5) & 0x7E0F81F;\
				*(p) = (_src | (_src>>16)) + (_dest | (_dest>>16));\
			}
			const int STRIDE = mLockedSurfaceDesc.lPitch / sizeof(PIXEL_TYPE);
			if (theColor.mAlpha != 255)
			{
				#define CALC_WEIGHT_A(w)	(((w) * (theColor.mAlpha+1)) >> 8)
				#include "GENERIC_DrawLineAA.inc"
				#undef CALC_WEIGHT_A
			}
			else
			{
				#define CALC_WEIGHT_A(w)	(w)
				#include "GENERIC_DrawLineAA.inc"
				#undef CALC_WEIGHT_A
			}
			#undef PIXEL_TYPE
			#undef BLEND_PIXEL
		}
		else
		{
#endif
			if (theColor.mAlpha != 255)
			{
				#define PIXEL_TYPE			ushort
				#define CALC_WEIGHT_A(w)	(((w) * (theColor.mAlpha+1)) >> 8)
				#define BLEND_PIXEL(p)		\
							*(p) =			\
									((((color & aRMask) * a + (dest & aRMask) * oma) >> 8) & aRMask) |\
									((((color & aGMask) * a + (dest & aGMask) * oma) >> 8) & aGMask) |\
									((((color & aBMask) * a + (dest & aBMask) * oma) >> 8) & aBMask);
				const int STRIDE = mLockedSurfaceDesc.lPitch / sizeof(PIXEL_TYPE);

				#include "GENERIC_DrawLineAA.inc"

				#undef PIXEL_TYPE
				#undef CALC_WEIGHT_A
				#undef BLEND_PIXEL
			}
			else
			{
				#define PIXEL_TYPE			ushort
				#define CALC_WEIGHT_A(w)	(w)
				#define BLEND_PIXEL(p)		\
							*(p) =			\
									((((color & aRMask) * a + (dest & aRMask) * oma) >> 8) & aRMask) |\
									((((color & aGMask) * a + (dest & aGMask) * oma) >> 8) & aGMask) |\
									((((color & aBMask) * a + (dest & aBMask) * oma) >> 8) & aBMask);
				const int STRIDE = mLockedSurfaceDesc.lPitch / sizeof(PIXEL_TYPE);

				#include "GENERIC_DrawLineAA.inc"

				#undef PIXEL_TYPE
				#undef CALC_WEIGHT_A
				#undef BLEND_PIXEL
			}
#ifdef OPTIMIZE_SOFTWARE_DRAWING
		}
#endif
	}

	UnlockSurface();
}

void DDImage::AdditiveDrawLineAA(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor)
{
}

void DDImage::DrawLineAA(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor, int theDrawMode)
{
	if (Check3D(this))
	{
		mDDInterface->mD3DInterface->DrawLine(theStartX,theStartY,theEndX,theEndY,theColor,theDrawMode);
		return;
	}

	if ((mDrawToBits) || (mHasAlpha) || (mHasTrans) || (mDDInterface->mIs3D))
	{
		MemoryImage::DrawLine(theStartX, theStartY, theEndX, theEndY, theColor, theDrawMode);
		return;
	}

	if (theStartY == theEndY)
	{
		int aStartX = std::min(theStartX, theEndX);
		int aEndX = std::max(theStartX, theEndX);

		FillRect(Rect(aStartX, theStartY, aEndX-aStartX+1, theEndY-theStartY+1), theColor, theDrawMode);
		return;
	}
	else if (theStartX == theEndX)
	{
		int aStartY = std::min(theStartY, theEndY);
		int aEndY = std::max(theStartY, theEndY);

		FillRect(Rect(theStartX, aStartY, theEndX-theStartX+1, aEndY-aStartY+1), theColor, theDrawMode);
		return;
	}

	CommitBits();	

	switch (theDrawMode)
	{
	case Graphics::DRAWMODE_NORMAL:
		NormalDrawLineAA(theStartX, theStartY, theEndX, theEndY, theColor);
		break;
	case Graphics::DRAWMODE_ADDITIVE:
		AdditiveDrawLineAA(theStartX, theStartY, theEndX, theEndY, theColor);
		break;
	}

	DeleteAllNonSurfaceData();
}

void DDImage::CommitBits()
{
	if (mSurface == NULL)
	{
		MemoryImage::CommitBits();
		return;
	}
}

void DDImage::Create(int theWidth, int theHeight)
{
	delete [] mBits;

	mWidth = theWidth;
	mHeight = theHeight;

	mBits = NULL;

	BitsChanged();	
}

void DDImage::BitsChanged()
{
	MemoryImage::BitsChanged();

	if (mSurface != NULL)
		mSurface->Release();
	mSurface = NULL;
}

ulong* DDImage::GetBits()
{
	if (mBits == NULL)
	{
		if (mSurface == NULL)
			return MemoryImage::GetBits();

		if (mNoLock)
			return NULL;

		LPDIRECTDRAWSURFACE aSurface = mSurface;
		(void)aSurface; // unused

		if (!LockSurface())
			return NULL;

		mBits = new ulong[mWidth*mHeight + 1];
		mBits[mWidth*mHeight] = MEMORYCHECK_ID;

		// unused
		//int aRRound = (1 << (7 - mDDInterface->mRedBits));
		//int aGRound = (1 << (7 - mDDInterface->mGreenBits));
		//int aBRound = (1 << (7 - mDDInterface->mBlueBits));

		if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 16)
		{
			ushort* aSrcPixelsRow = (ushort*) mLockedSurfaceDesc.lpSurface;
			ulong* aDest = mBits;

			for (int y = 0; y < mHeight; y++)
			{				
				ushort* aSrcPixels = aSrcPixelsRow;

				for (int x = 0; x < mWidth; x++)
				{
					ulong src = *(aSrcPixels++);

					int r = ((src >> mDDInterface->mRedShift << (8 - mDDInterface->mRedBits)) & 0xFF);
					int g = ((src >> mDDInterface->mGreenShift << (8 - mDDInterface->mGreenBits)) & 0xFF);
					int b = ((src >> mDDInterface->mBlueShift << (8 - mDDInterface->mBlueBits)) & 0xFF);
					
					*aDest++ = 0xFF000000 | (r << 16) | (g << 8) | (b);
				}
				
				aSrcPixelsRow += mLockedSurfaceDesc.lPitch/2;
			}
		}
		else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
		{
			ulong* aSrcPixelsRow = (ulong*) mLockedSurfaceDesc.lpSurface;
			ulong* aDest = mBits;

			for (int y = 0; y < mHeight; y++)
			{				
				ulong* aSrcPixels = aSrcPixelsRow;

				for (int x = 0; x < mWidth; x++)
				{
					ulong src = *(aSrcPixels++);

					int r = (src >> mDDInterface->mRedShift << (8 - mDDInterface->mRedBits)) & 0xFF;
					int g = (src >> mDDInterface->mGreenShift << (8 - mDDInterface->mGreenBits)) & 0xFF;
					int b = (src >> mDDInterface->mBlueShift << (8 - mDDInterface->mBlueBits)) & 0xFF;
					
					*aDest++ = 0xFF000000 | (r << 16) | (g << 8) | (b);
				}
				
				aSrcPixelsRow += mLockedSurfaceDesc.lPitch/4;
			}
		}		

		UnlockSurface();
	}

	return mBits;
}

void DDImage::NormalFillRect(const Rect& theRect, const Color& theColor)
{
	if (mNoLock)
		return;

	LPDIRECTDRAWSURFACE aSurface = GetSurface();
	(void)aSurface; // unused

	if (!LockSurface())
		return;	

	ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
	ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
	ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;

	ulong aRRoundAdd = aRMask;
	ulong aGRoundAdd = aGMask;
	ulong aBRoundAdd = aBMask;
	
	if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 16)
	{
		if (theColor.mAlpha == 255)
		{
			ushort aColor = 
				((((theColor.mRed * aRMask) + aRRoundAdd) >> 8) & aRMask) |
				((((theColor.mGreen * aGMask) + aGRoundAdd) >> 8) & aGMask) |
				((((theColor.mBlue * aBMask) + aBRoundAdd) >> 8) & aBMask);

			ushort* aDestPixelsRow = ((ushort*) mLockedSurfaceDesc.lpSurface) + (theRect.mY * mLockedSurfaceDesc.lPitch/2) + theRect.mX;
			
			for (int y = 0; y < theRect.mHeight; y++)
			{
				ushort* aDestPixels = aDestPixelsRow;			

				for (int x = 0; x < theRect.mWidth; x++)			
					*(aDestPixels++) = aColor;

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/2;			
			}
		}
		else
		{
			ushort src = 
				((((((theColor.mRed * theColor.mAlpha + 0x80) >> 8) * aRMask) + aRRoundAdd) >> 8) & aRMask) +
				((((((theColor.mGreen * theColor.mAlpha + 0x80) >> 8) * aGMask) + aGRoundAdd) >> 8) & aGMask) +
				((((((theColor.mBlue * theColor.mAlpha + 0x80) >> 8) * aBMask) + aBRoundAdd) >> 8) & aBMask);
			int oma = 256 - theColor.mAlpha;

			ushort* aDestPixelsRow = ((ushort*) mLockedSurfaceDesc.lpSurface) + (theRect.mY * mLockedSurfaceDesc.lPitch/2) + theRect.mX;
					
			for (int y = 0; y < theRect.mHeight; y++)
			{
				ushort* aDestPixels = aDestPixelsRow;			

				for (int x = 0; x < theRect.mWidth; x++)			
				{
					ushort dest = *aDestPixels;

					*(aDestPixels++) = src + 
						(((((dest & aRMask) * oma) + aRRoundAdd) >> 8) & aRMask) +
						(((((dest & aGMask) * oma) + aGRoundAdd) >> 8) & aGMask) +
						(((((dest & aBMask) * oma) + aBRoundAdd) >> 8) & aBMask);
				}

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/2;			
			}
		}
	}
	else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
	{
		if (theColor.mAlpha == 255)
		{
			ulong aColor = 
				((((theColor.mRed * aRMask)) >> 8) & aRMask) |
				((((theColor.mGreen * aGMask)) >> 8) & aGMask) |
				((((theColor.mBlue * aBMask)) >> 8) & aBMask);

			ulong* aDestPixelsRow = ((ulong*) mLockedSurfaceDesc.lpSurface) + (theRect.mY * mLockedSurfaceDesc.lPitch/4) + theRect.mX;
			
			for (int y = 0; y < theRect.mHeight; y++)
			{
				ulong* aDestPixels = aDestPixelsRow;			

				for (int x = 0; x < theRect.mWidth; x++)			
					*(aDestPixels++) = aColor;

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/4;
			}
		}
		else
		{
			ulong src = 
				((((((theColor.mRed * theColor.mAlpha + 0x7F) >> 8) * aRMask) + aRRoundAdd) >> 8) & aRMask) +
				((((((theColor.mGreen * theColor.mAlpha + 0x7F) >> 8) * aGMask) + aGRoundAdd) >> 8) & aGMask) +
				((((((theColor.mBlue * theColor.mAlpha + 0x7F) >> 8) * aBMask) + aBRoundAdd) >> 8) & aBMask);
			int oma = 256 - theColor.mAlpha;

			ulong* aDestPixelsRow = ((ulong*) mLockedSurfaceDesc.lpSurface) + (theRect.mY * mLockedSurfaceDesc.lPitch/4) + theRect.mX;
					
			for (int y = 0; y < theRect.mHeight; y++)
			{
				ulong* aDestPixels = aDestPixelsRow;			

				for (int x = 0; x < theRect.mWidth; x++)			
				{
					ulong dest = *aDestPixels;

					*(aDestPixels++) = src + 
						(((((dest & aRMask) * oma)) >> 8) & aRMask) +
						(((((dest & aGMask) * oma)) >> 8) & aGMask) +
						(((((dest & aBMask) * oma)) >> 8) & aBMask);
				}

				aDestPixelsRow += mLockedSurfaceDesc.lPitch/4;
			}
		}
	}

	UnlockSurface();
}

void DDImage::AdditiveFillRect(const Rect& theRect, const Color& theColor)
{
	if (mNoLock)
		return;

	LPDIRECTDRAWSURFACE aSurface = GetSurface();
	(void)aSurface; // unused

	if (!LockSurface())
		return;	

	ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
	ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
	ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;

	// ulong aRRoundAdd = aRMask >> 1;
	// ulong aGRoundAdd = aGMask >> 1;
	// ulong aBRoundAdd = aBMask >> 1;
	
	int aRedShift = mDDInterface->mRedShift;
	int aGreenShift = mDDInterface->mGreenShift;
	int aBlueShift = mDDInterface->mBlueShift;

	int* aMaxRedTable = mDDInterface->mRedAddTable;
	int* aMaxGreenTable = mDDInterface->mGreenAddTable;
	int* aMaxBlueTable = mDDInterface->mBlueAddTable;	

	if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 16)
	{
		ushort rc = ((theColor.mRed * theColor.mAlpha) / 255) >> (8-mDDInterface->mRedBits);
		ushort gc = ((theColor.mGreen * theColor.mAlpha) / 255) >> (8-mDDInterface->mGreenBits);
		ushort bc = ((theColor.mBlue * theColor.mAlpha) / 255) >> (8-mDDInterface->mBlueBits);

		ushort* aDestPixelsRow = ((ushort*) mLockedSurfaceDesc.lpSurface) + (theRect.mY * mLockedSurfaceDesc.lPitch/2) + theRect.mX;
				
		for (int y = 0; y < theRect.mHeight; y++)
		{
			ushort* aDestPixels = aDestPixelsRow;			

			for (int x = 0; x < theRect.mWidth; x++)			
			{
				ushort dest = *aDestPixels;

				int r = aMaxRedTable[((dest & aRMask) >> aRedShift) + rc];
				int g = aMaxGreenTable[((dest & aGMask) >> aGreenShift) + gc];
				int b = aMaxBlueTable[((dest & aBMask) >> aBlueShift) + bc];				

				*(aDestPixels++) = 
					(r << aRedShift) |
					(g << aGreenShift) |
					(b << aBlueShift);
			}				

			aDestPixelsRow += mLockedSurfaceDesc.lPitch/2;			
		}
	}
	else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
	{
		ulong rc = ((theColor.mRed * theColor.mAlpha) / 255) >> (8-mDDInterface->mRedBits);
		ulong gc = ((theColor.mGreen * theColor.mAlpha) / 255) >> (8-mDDInterface->mGreenBits);
		ulong bc = ((theColor.mBlue * theColor.mAlpha) / 255) >> (8-mDDInterface->mBlueBits);

		ulong* aDestPixelsRow = ((ulong*) mLockedSurfaceDesc.lpSurface) + (theRect.mY * mLockedSurfaceDesc.lPitch/4) + theRect.mX;
		
		for (int y = 0; y < theRect.mHeight; y++)
		{
			ulong* aDestPixels = aDestPixelsRow;			

			for (int x = 0; x < theRect.mWidth; x++)			
			{
				ulong dest = *aDestPixels;

				int r = aMaxRedTable[((dest & aRMask) >> aRedShift) + rc];
				int g = aMaxGreenTable[((dest & aGMask) >> aGreenShift) + gc];
				int b = aMaxBlueTable[((dest & aBMask) >> aBlueShift) + bc];				

				*(aDestPixels++) = 
					(r << aRedShift) |
					(g << aGreenShift) |
					(b << aBlueShift);
			}				

			aDestPixelsRow += mLockedSurfaceDesc.lPitch/4;
		}
	}

	UnlockSurface();
}

void DDImage::NormalBlt(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor)
{
	theImage->mDrawn = true;

	MemoryImage* aMemoryImage = dynamic_cast<MemoryImage*>(theImage);
	DDImage* aDDImage = dynamic_cast<DDImage*>(theImage);	

	if (aMemoryImage != NULL)
	{
		aMemoryImage->CommitBits();		

		RECT aDestRect = {theX, theY, theX + theSrcRect.mWidth, theY + theSrcRect.mHeight};
		RECT aSrcRect = {theSrcRect.mX, theSrcRect.mY, theSrcRect.mX + theSrcRect.mWidth, theSrcRect.mY + theSrcRect.mHeight};	

		//TODO:
		if ((aMemoryImage->mIsVolatile) && ((aDDImage == NULL) || (aDDImage->mSurface == NULL)) && 
			(!mNoLock) && (theColor == Color::White))
		{
			if (aMemoryImage->mColorTable == NULL)
			{
				ulong* aSrcBits = aMemoryImage->GetBits();			

				#define SRC_TYPE ulong
				#define NEXT_SRC_COLOR (*(aSrcPixels++))
				#include "DDI_NormalBlt_Volatile.inc"
				#undef	SRC_TYPE
				#undef NEXT_SRC_COLOR
			}
			else
			{			
				ulong* aColorTable = aMemoryImage->mColorTable;
				uchar* aSrcBits = aMemoryImage->mColorIndices;

				#define SRC_TYPE uchar
				#define NEXT_SRC_COLOR (aColorTable[*(aSrcPixels++)])
				
				#include "DDI_NormalBlt_Volatile.inc"

				#undef SRC_TYPE
				#undef NEXT_SRC_COLOR
			}
		}
		else if ((aMemoryImage->mHasAlpha) || (theColor != Color::White))
		{
			if (mNoLock)
				return;			

			LPDIRECTDRAWSURFACE aSurface = GetSurface();
			(void)aSurface; // unused

 			if (!LockSurface())
				return;

			// Ensure NativeAlphaData is calculated
			void *aNativeData = aMemoryImage->GetNativeAlphaData(mDDInterface);

			// Ensure RunLength data is calculated
			uchar* aSrcRLAlphaData = aMemoryImage->GetRLAlphaData();

			#define _PLUSPLUS ++
			#define _PLUSEQUALS +=
			if (aMemoryImage->mColorTable == NULL)
			{
				ulong* aSrcPixelsRow = ((ulong*) aNativeData) + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;
				ulong* aSrcPixels;
				
				#define NEXT_SRC_COLOR (*(aSrcPixels++))
				#define PEEK_SRC_COLOR (*aSrcPixels)

				#include "DDI_AlphaBlt.inc"

				#undef NEXT_SRC_COLOR
				#undef PEEK_SRC_COLOR
			}
			else
			{
				ulong* aNativeColorTable = (ulong*) aNativeData;

				uchar* aSrcPixelsRow = aMemoryImage->mColorIndices + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;
				uchar* aSrcPixels;

				#define NEXT_SRC_COLOR (aNativeColorTable[*(aSrcPixels++)])
				#define PEEK_SRC_COLOR (aNativeColorTable[*aSrcPixels])

				#include "DDI_AlphaBlt.inc"

				#undef NEXT_SRC_COLOR
				#undef PEEK_SRC_COLOR
			}		

			#undef _PLUSPLUS
			#undef _PLUSEQUALS 
			UnlockSurface();
		}		
		else if ((aDDImage == NULL) || (aDDImage->mSurface == NULL) || ((!mVideoMemory) && (aDDImage->mVideoMemory)))
		{
			if (mNoLock)
				return;

			//TODO: Have some sort of cool thing here
			LPDIRECTDRAWSURFACE aSurface = GetSurface();
			(void)aSurface; // unused

			if (!LockSurface())
				return;

			void* aNativeAlphaData = aMemoryImage->GetNativeAlphaData(mDDInterface);

			if (aMemoryImage->mColorTable == NULL)
			{
				ulong* aSrcPixelsRow = ((ulong*) aNativeAlphaData) + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;
				ulong* aSrcPixels;
				
				#define NEXT_SRC_COLOR (*(aSrcPixels++))
				#define PEEK_SRC_COLOR (*aSrcPixels)

				#include "DDI_FastBlt_NoAlpha.inc"

				#undef NEXT_SRC_COLOR
				#undef PEEK_SRC_COLOR
			}
			else
			{
				ulong* aNativeAlphaColorTable = (ulong*) aNativeAlphaData;

				uchar* aSrcPixelsRow = aMemoryImage->mColorIndices + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;
				uchar* aSrcPixels;

				#define NEXT_SRC_COLOR (aNativeAlphaColorTable[*(aSrcPixels++)])
				#define PEEK_SRC_COLOR (aNativeAlphaColorTable[*aSrcPixels])

				#include "DDI_FastBlt_NoAlpha.inc"

				#undef NEXT_SRC_COLOR
				#undef PEEK_SRC_COLOR
			}		
		}
		else
		{
			if (mLockCount > 0)
				mSurface->Unlock(NULL);

			DDBLTFX aBltFX;
			ZeroMemory(&aBltFX, sizeof(aBltFX));
			aBltFX.dwSize = sizeof(aBltFX);
    
			DWORD aFlags = DDBLT_WAIT;

			if (aDDImage->mHasTrans)
				aFlags |= DDBLT_KEYSRC;

			HRESULT aResult = GetSurface()->Blt(&aDestRect, aDDImage->GetSurface(), &aSrcRect, aFlags, &aBltFX);
			(void)aResult; // unused
		
			if (mLockCount > 0)
			{
				if (mSurface->Lock(NULL, &mLockedSurfaceDesc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL) != DD_OK)
					return;
			}
		}
	}
}

void DDImage::NormalBltMirror(Image* theImage, int theX, int theY, const Rect& theSrcRectOrig, const Color& theColor)
{
	theImage->mDrawn = true;

	Rect theSrcRect = theSrcRectOrig;
//	theSrcRect.mX = (theSrcRect.mX+theSrcRect.mWidth)*-1 + theImage->mWidth;
	theX += theSrcRect.mWidth-1;

	MemoryImage* aMemoryImage = dynamic_cast<MemoryImage*>(theImage);
	// DDImage* aDDImage = dynamic_cast<DDImage*>(theImage); // unused

	if (aMemoryImage != NULL)
	{
		aMemoryImage->CommitBits();		

		if (mNoLock)
			return;

		LPDIRECTDRAWSURFACE aSurface = GetSurface();
		(void)aSurface; // unused

 		if (!LockSurface())
			return;

		// Ensure NativeAlphaData is calculated
		void *aNativeData = aMemoryImage->GetNativeAlphaData(mDDInterface);

		// Ensure RunLength data is calculated
		uchar* aSrcRLAlphaData = aMemoryImage->GetRLAlphaData();

		#define _PLUSPLUS --
		#define _PLUSEQUALS -=
		if (aMemoryImage->mColorTable == NULL)
		{
			ulong* aSrcPixelsRow = ((ulong*) aNativeData) + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;
			ulong* aSrcPixels;
			
			#define NEXT_SRC_COLOR (*(aSrcPixels++))
			#define PEEK_SRC_COLOR (*aSrcPixels)

			#include "DDI_AlphaBlt.inc"

			#undef NEXT_SRC_COLOR
			#undef PEEK_SRC_COLOR
		}
		else
		{
			ulong* aNativeColorTable = (ulong*) aNativeData;

			uchar* aSrcPixelsRow = aMemoryImage->mColorIndices + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;
			uchar* aSrcPixels;

			#define NEXT_SRC_COLOR (aNativeColorTable[*(aSrcPixels++)])
			#define PEEK_SRC_COLOR (aNativeColorTable[*aSrcPixels])

			#include "DDI_AlphaBlt.inc"

			#undef NEXT_SRC_COLOR
			#undef PEEK_SRC_COLOR
		}		

		#undef _PLUSPLUS
		#undef _PLUSEQUALS 
		UnlockSurface();
	}
}


void DDImage::AdditiveBlt(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor)
{
	theImage->mDrawn = true;

	if (mNoLock)
		return;

	MemoryImage* aMemoryImage = dynamic_cast<MemoryImage*>(theImage);
	// DDImage* aDDImage = dynamic_cast<DDImage*>(theImage); // unused

	if (aMemoryImage != NULL)
	{
		if (!LockSurface())
			return;

		// Ensure NativeAlphaData is calculated
		void* aNativeAlphaData = aMemoryImage->GetNativeAlphaData(mDDInterface);

		#define _PLUSPLUS ++
		#define _PLUSEQUALS +=
		if (aMemoryImage->mColorTable == NULL)
		{
			ulong* aSrcPixelsRow = ((ulong*) aNativeAlphaData) + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;
			ulong* aSrcPixels;
			
			#define NEXT_SRC_COLOR (*(aSrcPixels++))
			#define PEEK_SRC_COLOR (*aSrcPixels)

			#include "DDI_Additive.inc"

			#undef NEXT_SRC_COLOR
			#undef PEEK_SRC_COLOR
		}
		else
		{
			ulong* aNativeAlphaColorTable = (ulong*) aNativeAlphaData;

			uchar* aSrcPixelsRow = aMemoryImage->mColorIndices + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;
			uchar* aSrcPixels;

			#define NEXT_SRC_COLOR (aNativeAlphaColorTable[*(aSrcPixels++)])
			#define PEEK_SRC_COLOR (aNativeAlphaColorTable[*aSrcPixels])

			#include "DDI_Additive.inc"

			#undef NEXT_SRC_COLOR
			#undef PEEK_SRC_COLOR
		}
		#undef _PLUSPLUS
		#undef _PLUSEQUALS

		UnlockSurface();
	}
}

void DDImage::AdditiveBltMirror(Image* theImage, int theX, int theY, const Rect& theSrcRectOrig, const Color& theColor)
{
	theImage->mDrawn = true;

	if (mNoLock)
		return;

	Rect theSrcRect = theSrcRectOrig;
//	theSrcRect.mX = (theSrcRect.mX+theSrcRect.mWidth)*-1 + theImage->mWidth;
	theX += theSrcRect.mWidth-1;

	MemoryImage* aMemoryImage = dynamic_cast<MemoryImage*>(theImage);
	// DDImage* aDDImage = dynamic_cast<DDImage*>(theImage); // unused

	if (aMemoryImage != NULL)
	{
		if (!LockSurface())
			return;

		// Ensure NativeAlphaData is calculated
		void* aNativeAlphaData = aMemoryImage->GetNativeAlphaData(mDDInterface);

		#define _PLUSPLUS --
		#define _PLUSEQUALS -=
		if (aMemoryImage->mColorTable == NULL)
		{
			ulong* aSrcPixelsRow = ((ulong*) aNativeAlphaData) + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;
			ulong* aSrcPixels;
			
			#define NEXT_SRC_COLOR (*(aSrcPixels++))
			#define PEEK_SRC_COLOR (*aSrcPixels)

			#include "DDI_Additive.inc"

			#undef NEXT_SRC_COLOR
			#undef PEEK_SRC_COLOR
		}
		else
		{
			ulong* aNativeAlphaColorTable = (ulong*) aNativeAlphaData;

			uchar* aSrcPixelsRow = aMemoryImage->mColorIndices + (theSrcRect.mY * theImage->mWidth) + theSrcRect.mX;
			uchar* aSrcPixels;

			#define NEXT_SRC_COLOR (aNativeAlphaColorTable[*(aSrcPixels++)])
			#define PEEK_SRC_COLOR (aNativeAlphaColorTable[*aSrcPixels])

			#include "DDI_Additive.inc"

			#undef NEXT_SRC_COLOR
			#undef PEEK_SRC_COLOR
		}
		#undef _PLUSPLUS
		#undef _PLUSEQUALS

		UnlockSurface();
	}
}

void DDImage::Blt(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode)
{
	theImage->mDrawn = true;

	//if (gDebug)
	//	mApp->CopyToClipboard("+DDImage::Blt");	

	DBG_ASSERTE((theColor.mRed >= 0) && (theColor.mRed <= 255));
	DBG_ASSERTE((theColor.mGreen >= 0) && (theColor.mGreen <= 255));
	DBG_ASSERTE((theColor.mBlue >= 0) && (theColor.mBlue <= 255));
	DBG_ASSERTE((theColor.mAlpha >= 0) && (theColor.mAlpha <= 255));

	CommitBits();	
	
	if (Check3D(this))
	{
		DDImage* aDDImage = dynamic_cast<DDImage*>(theImage);	

		// Special short-circuit 
		if ((aDDImage != NULL) && (aDDImage->mSurface != NULL) &&
			(mDDInterface->mD3DInterface->mTransformStack.empty()) && 
			(theDrawMode == Graphics::DRAWMODE_NORMAL) && 
			(theColor == Color::White) && (!aDDImage->mHasAlpha))
		{
			if (mLockCount > 0)
				mSurface->Unlock(NULL);

			DDBLTFX aBltFX;
			ZeroMemory(&aBltFX, sizeof(aBltFX));
			aBltFX.dwSize = sizeof(aBltFX);

			DWORD aFlags = DDBLT_WAIT;

			if (aDDImage->mHasTrans)
				aFlags |= DDBLT_KEYSRC;

			RECT aDestRect = {theX, theY, theX + theSrcRect.mWidth, theY + theSrcRect.mHeight};
			RECT aSrcRect = {theSrcRect.mX, theSrcRect.mY, theSrcRect.mX + theSrcRect.mWidth, theSrcRect.mY + theSrcRect.mHeight};	

			HRESULT aResult = GetSurface()->Blt(&aDestRect, aDDImage->GetSurface(), &aSrcRect, aFlags, &aBltFX);
			(void)aResult; // unused
		
			if (mLockCount > 0)
			{
				if (mSurface->Lock(NULL, &mLockedSurfaceDesc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL) != DD_OK)
					return;
			}

			return;
		}

		mDDInterface->mD3DInterface->Blt(theImage,theX,theY,theSrcRect,theColor,theDrawMode);
		return;
	}

	if ((mDrawToBits) || (mHasAlpha) || ((mHasTrans) && (!mFirstPixelTrans)) || (mDDInterface->mIs3D && this!=mDDInterface->mNewCursorAreaImage && this!=mDDInterface->mOldCursorAreaImage))
	{
		MemoryImage::Blt(theImage, theX, theY, theSrcRect, theColor, theDrawMode);
		return;
	}	

	switch (theDrawMode)
	{
	case Graphics::DRAWMODE_NORMAL:		
		NormalBlt(theImage, theX, theY, theSrcRect, theColor);
		break;
	case Graphics::DRAWMODE_ADDITIVE:
		AdditiveBlt(theImage, theX, theY, theSrcRect, theColor);
		break;
	}

	DeleteAllNonSurfaceData();
}

void DDImage::BltMirror(Image* theImage, int theX, int theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode)
{
	DBG_ASSERTE((theColor.mRed >= 0) && (theColor.mRed <= 255));
	DBG_ASSERTE((theColor.mGreen >= 0) && (theColor.mGreen <= 255));
	DBG_ASSERTE((theColor.mBlue >= 0) && (theColor.mBlue <= 255));
	DBG_ASSERTE((theColor.mAlpha >= 0) && (theColor.mAlpha <= 255));

	CommitBits();	

	if (Check3D(this))
	{
		mDDInterface->mD3DInterface->BltMirror(theImage,theX,theY,theSrcRect,theColor,theDrawMode);
		return;
	}

	switch (theDrawMode)
	{
	case Graphics::DRAWMODE_NORMAL:		
		NormalBltMirror(theImage, theX, theY, theSrcRect, theColor);
		break;
	case Graphics::DRAWMODE_ADDITIVE:
		AdditiveBltMirror(theImage, theX, theY, theSrcRect, theColor);
		break;
	}

	DeleteAllNonSurfaceData();
}

void DDImage::BltF(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Rect &theClipRect, const Color& theColor, int theDrawMode)
{
	theImage->mDrawn = true;

	if (Check3D(this))
	{
		FRect aClipRect(theClipRect.mX,theClipRect.mY,theClipRect.mWidth,theClipRect.mHeight);
		FRect aDestRect(theX,theY,theSrcRect.mWidth,theSrcRect.mHeight);

		FRect anIntersect = aDestRect.Intersection(aClipRect);
		if (anIntersect.mWidth!=aDestRect.mWidth || anIntersect.mHeight!=aDestRect.mHeight)
		{
			if (anIntersect.mWidth!=0 && anIntersect.mHeight!=0)
				mDDInterface->mD3DInterface->BltClipF(theImage,theX,theY,theSrcRect,&theClipRect,theColor,theDrawMode);
		}
		else
			mDDInterface->mD3DInterface->Blt(theImage,theX,theY,theSrcRect,theColor,theDrawMode,true);

		return;
	}
	else
		BltRotated(theImage,theX,theY,theSrcRect,theClipRect,theColor,theDrawMode,0,0,0);
}

//0x57A610
void DDImage::BltRotated(Image* theImage, float theX, float theY, const Rect &theSrcRect, const Rect& theClipRect, const Color& theColor, int theDrawMode, double theRot, float theRotCenterX, float theRotCenterY)
{
	theImage->mDrawn = true;

	if (mNoLock)
		return;	

	CommitBits();

	if (Check3D(this))
	{
		mDDInterface->mD3DInterface->BltRotated(theImage,theX,theY,&theClipRect,theColor,theDrawMode,theRot,theRotCenterX,theRotCenterY,theSrcRect);
		return;
	}

	if ((mDrawToBits) || (mHasAlpha) || ((mHasTrans) && (!mFirstPixelTrans)) || (mDDInterface->mIs3D))
	{
		MemoryImage::BltRotated(theImage, theX, theY, theSrcRect, theClipRect, theColor, theDrawMode, theRot, theRotCenterX, theRotCenterY);
		return;
	}	

	// This BltRotatedClipHelper clipping used to happen in Graphics::DrawImageRotated
	FRect aDestRect;
	if (!BltRotatedClipHelper(theX, theY, theSrcRect, theClipRect, theRot, aDestRect,theRotCenterX,theRotCenterY))
		return;

	MemoryImage* aMemoryImage = dynamic_cast<MemoryImage*>(theImage);
	// DDImage* aDDImage = dynamic_cast<DDImage*>(theImage); // unused

	if (aMemoryImage != NULL)
	{
		aMemoryImage->CommitBits();

		if (theDrawMode == Graphics::DRAWMODE_NORMAL)
		{
			if (aMemoryImage->mColorTable == NULL)
			{
				ulong* aSrcBits = aMemoryImage->GetBits() + theSrcRect.mX + theSrcRect.mY*aMemoryImage->GetWidth();			

				#define SRC_TYPE ulong
				#define READ_COLOR(ptr) (*(ptr))

				#include "DDI_BltRotated.inc"

				#undef SRC_TYPE
				#undef READ_COLOR

			}
			else
			{			
				ulong* aColorTable = aMemoryImage->mColorTable;
				uchar* aSrcBits = aMemoryImage->mColorIndices + theSrcRect.mX + theSrcRect.mY*aMemoryImage->GetWidth();

				#define SRC_TYPE uchar
				#define READ_COLOR(ptr) (aColorTable[*(ptr)])

				#include "DDI_BltRotated.inc"

				#undef SRC_TYPE
				#undef READ_COLOR
			}
		}
		else
		{
			if (aMemoryImage->mColorTable == NULL)
			{
				ulong* aSrcBits = aMemoryImage->GetBits() + theSrcRect.mX + theSrcRect.mY*aMemoryImage->GetWidth();			

				#define SRC_TYPE ulong
				#define READ_COLOR(ptr) (*(ptr))

				#include "DDI_BltRotated_Additive.inc"

				#undef SRC_TYPE
				#undef READ_COLOR

			}
			else
			{			
				ulong* aColorTable = aMemoryImage->mColorTable;
				uchar* aSrcBits = aMemoryImage->mColorIndices + theSrcRect.mX + theSrcRect.mY*aMemoryImage->GetWidth();

				#define SRC_TYPE uchar
				#define READ_COLOR(ptr) (aColorTable[*(ptr)])

				#include "DDI_BltRotated_Additive.inc"

				#undef SRC_TYPE
				#undef READ_COLOR
			}
		}
	}

	DeleteAllNonSurfaceData();
}

void DDImage::StretchBlt(Image* theImage, const Rect& theDestRectOrig, const Rect& theSrcRectOrig, const Rect& theClipRect, const Color& theColor, int theDrawMode, bool fastStretch)
{
	theImage->mDrawn = true;

	DDImage* aSrcDDImage = dynamic_cast<DDImage*>(theImage);
	MemoryImage* aSrcMemoryImage = dynamic_cast<MemoryImage*>(theImage);

	CommitBits();

	if (Check3D(this))
	{
		mDDInterface->mD3DInterface->StretchBlt(theImage,theDestRectOrig,theSrcRectOrig,&theClipRect,theColor,theDrawMode,fastStretch);
		return;
	}

	Rect theDestRect;
	FRect theSrcRect;
	if (!StretchBltClipHelper(theSrcRectOrig,theClipRect,theDestRectOrig,theSrcRect,theDestRect))
		return;

	if (fastStretch)
	{
		if ((aSrcDDImage != NULL) && (theColor == Color::White) && (theDrawMode == Graphics::DRAWMODE_NORMAL) && 
			(!aSrcDDImage->mHasAlpha) && (aSrcDDImage->GetSurface() != NULL))
		{
			LPDIRECTDRAWSURFACE aSrcSurface = aSrcDDImage->GetSurface();
			(void)aSrcSurface; // unused
			LPDIRECTDRAWSURFACE aDestSurface = GetSurface();
			(void)aDestSurface; // unused

			DDBLTFX aBltFX;
			ZeroMemory(&aBltFX, sizeof(aBltFX));
			aBltFX.dwSize = sizeof(aBltFX);
    
			DWORD aFlags = DDBLT_WAIT;

			if (aSrcDDImage->mHasTrans)
				aFlags |= DDBLT_KEYSRC;

			RECT aDestRect = {theDestRect.mX, theDestRect.mY, theDestRect.mX + theDestRect.mWidth, theDestRect.mY + theDestRect.mHeight};
			RECT aSrcRect = {(LONG)theSrcRect.mX, (LONG)theSrcRect.mY, (LONG)(theSrcRect.mX + theSrcRect.mWidth), (LONG)(theSrcRect.mY + theSrcRect.mHeight)};	
			
			if (mLockCount > 0)
			{
				mSurface->Unlock(NULL);
			}
			
			HRESULT aResult = GetSurface()->Blt(&aDestRect, aSrcDDImage->GetSurface(), &aSrcRect, aFlags, &aBltFX);
			(void)aResult; // unused
		
			if (mLockCount > 0)
			{
				if (mSurface->Lock(NULL, &mLockedSurfaceDesc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL) != DD_OK)
					return;
			}			
		}
		else
		{
			if (aSrcMemoryImage != NULL)
			{
				aSrcMemoryImage->CommitBits();

			// Ensure NativeAlphaData is calculated
				void *aNativeAlphaData = aSrcMemoryImage->GetNativeAlphaData(mDDInterface);

				#define _PLUSPLUS ++
				if (theDrawMode == Graphics::DRAWMODE_NORMAL)
				{
					if (aSrcMemoryImage->mColorTable == NULL)
					{
						//ulong* aSrcBits = aSrcMemoryImage->GetBits();		
						ulong* aSrcBits = ((ulong*) aNativeAlphaData);	

						#define SRC_TYPE ulong
						#define READ_COLOR(ptr) (*(ptr))

						#include "DDI_FastStretch.inc"

						#undef SRC_TYPE
						#undef READ_COLOR
					}
					else
					{
						ulong* aColorTable = (ulong*) aNativeAlphaData;
						uchar* aSrcBits = aSrcMemoryImage->mColorIndices;

						#define SRC_TYPE uchar
						#define READ_COLOR(ptr) (aColorTable[*(ptr)])

						#include "DDI_FastStretch.inc"

						#undef SRC_TYPE
						#undef READ_COLOR
					}
				}
				else
				{
					if (aSrcMemoryImage->mColorTable == NULL)
					{
						//ulong* aSrcBits = aSrcMemoryImage->GetBits();		
						ulong* aSrcBits = ((ulong*) aNativeAlphaData);	

						#define SRC_TYPE ulong
						#define READ_COLOR(ptr) (*(ptr))

						#include "DDI_FastStretch_Additive.inc"

						#undef SRC_TYPE
						#undef READ_COLOR
					}
					else
					{
						ulong* aColorTable = (ulong*) aNativeAlphaData;
						uchar* aSrcBits = aSrcMemoryImage->mColorIndices;

						#define SRC_TYPE uchar
						#define READ_COLOR(ptr) (aColorTable[*(ptr)])

						#include "DDI_FastStretch_Additive.inc"

						#undef SRC_TYPE
						#undef READ_COLOR
					}
				}
				#undef _PLUSPLUS
			}
		}
	}
	else
	{
		if ((mDrawToBits) || (mHasAlpha) || (mHasTrans) || (mDDInterface->mIs3D))
		{
			MemoryImage::StretchBlt(theImage, theDestRectOrig, theSrcRectOrig, theClipRect, theColor, theDrawMode, fastStretch);
			return;
		}

		// Stretch it to a temporary image
		MemoryImage aTempImage(mApp);
		Rect aTempRect(0, 0, theDestRect.mWidth, theDestRect.mHeight);

		aTempImage.Create(theDestRect.mWidth, theDestRect.mHeight);			
		if (fastStretch)
			aTempImage.FastStretchBlt(theImage, aTempRect, theSrcRect, theColor, 0);
		else
			aTempImage.SlowStretchBlt(theImage, aTempRect, theSrcRect, theColor, 0);

		Blt(&aTempImage, theDestRect.mX, theDestRect.mY, aTempRect, theColor, theDrawMode);
	}

	DeleteAllNonSurfaceData();
}

//0x583AF0
void DDImage::StretchBltMirror(Image* theImage, const Rect& theDestRectOrig, const Rect& theSrcRectOrig, const Rect& theClipRect, const Color& theColor, int theDrawMode, bool fastStretch)
{
	theImage->mDrawn = true;

	// DDImage* aSrcDDImage = dynamic_cast<DDImage*>(theImage); // unused
	MemoryImage* aSrcMemoryImage = dynamic_cast<MemoryImage*>(theImage);

	CommitBits();

	if (Check3D(this))
	{
		mDDInterface->mD3DInterface->StretchBlt(theImage,theDestRectOrig,theSrcRectOrig,&theClipRect,theColor,theDrawMode,fastStretch,true);
		return;
	}

	FRect theSrcRect;
	Rect theDestRect;

	if (!StretchBltMirrorClipHelper(theSrcRectOrig,theClipRect,theDestRectOrig,theSrcRect,theDestRect))
		return;

	theDestRect.mX += theDestRect.mWidth-1;


	if (aSrcMemoryImage != NULL)
	{
		aSrcMemoryImage->CommitBits();

	// Ensure NativeAlphaData is calculated
		void *aNativeAlphaData = aSrcMemoryImage->GetNativeAlphaData(mDDInterface);

		#define _PLUSPLUS --
		if (theDrawMode == Graphics::DRAWMODE_NORMAL)
		{
			if (aSrcMemoryImage->mColorTable == NULL)
			{
				//ulong* aSrcBits = aSrcMemoryImage->GetBits();		
				ulong* aSrcBits = ((ulong*) aNativeAlphaData);	

				#define SRC_TYPE ulong
				#define READ_COLOR(ptr) (*(ptr))

				#include "DDI_FastStretch.inc"

				#undef SRC_TYPE
				#undef READ_COLOR
			}
			else
			{
				ulong* aColorTable = (ulong*) aNativeAlphaData;
				uchar* aSrcBits = aSrcMemoryImage->mColorIndices;

				#define SRC_TYPE uchar
				#define READ_COLOR(ptr) (aColorTable[*(ptr)])

				#include "DDI_FastStretch.inc"

				#undef SRC_TYPE
				#undef READ_COLOR
			}
		}
		else
		{
			if (aSrcMemoryImage->mColorTable == NULL)
			{
				//ulong* aSrcBits = aSrcMemoryImage->GetBits();		
				ulong* aSrcBits = ((ulong*) aNativeAlphaData);	

				#define SRC_TYPE ulong
				#define READ_COLOR(ptr) (*(ptr))

				#include "DDI_FastStretch_Additive.inc"

				#undef SRC_TYPE
				#undef READ_COLOR
			}
			else
			{
				ulong* aColorTable = (ulong*) aNativeAlphaData;
				uchar* aSrcBits = aSrcMemoryImage->mColorIndices;

				#define SRC_TYPE uchar
				#define READ_COLOR(ptr) (aColorTable[*(ptr)])

				#include "DDI_FastStretch_Additive.inc"

				#undef SRC_TYPE
				#undef READ_COLOR
			}
		}
		#undef _PLUSPLUS
	}

	DeleteAllNonSurfaceData();  //0x56C560
}

void DDImage::BltMatrix(Image* theImage, float x, float y, const SexyMatrix3 &theMatrix, const Rect& theClipRect, const Color& theColor, int theDrawMode, const Rect &theSrcRect, bool blend)
{
	theImage->mDrawn = true;

	if (Check3D(this))
	{
		mDDInterface->mD3DInterface->BltTransformed(theImage,&theClipRect,theColor,theDrawMode,theSrcRect,theMatrix,blend,x,y,true);
		return;
	}

	LPDIRECTDRAWSURFACE aSurface = GetSurface();
	(void)aSurface; // unused
 	if (!LockSurface())
		return;

	int aPixelFormat = 0;
	if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
		aPixelFormat = 0x888;
	else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask == 0xf800 && mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask == 0x07e0 && mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask == 0x001f)
		aPixelFormat = 0x565;
	else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask == 0x7c00 && mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask == 0x03e0 && mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask == 0x001f)
		aPixelFormat = 0x555;
	else
		DBG_ASSERTE(FALSE);

	BltMatrixHelper(theImage,x,y,theMatrix,theClipRect,theColor,theDrawMode,theSrcRect,mLockedSurfaceDesc.lpSurface,mLockedSurfaceDesc.lPitch,aPixelFormat,blend);

	UnlockSurface();

	DeleteAllNonSurfaceData();

}

void DDImage::BltTrianglesTex(Image *theTexture, const TriVertex theVertices[][3], int theNumTriangles, const Rect& theClipRect, const Color &theColor, int theDrawMode, float tx, float ty, bool blend)
{
	theTexture->mDrawn = true;

	if (Check3D(this))
	{
		mDDInterface->mD3DInterface->DrawTrianglesTex(theVertices,theNumTriangles,theColor,theDrawMode,theTexture,tx,ty,blend);
		return;
	}

	LPDIRECTDRAWSURFACE aSurface = GetSurface();
	(void)aSurface; // unused
 	if (!LockSurface())
		return;

	int aPixelFormat = 0;
	if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
		aPixelFormat = 0x888;
	else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask == 0xf800 && mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask == 0x07e0 && mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask == 0x001f)
		aPixelFormat = 0x565;
	else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask == 0x7c00 && mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask == 0x03e0 && mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask == 0x001f)
		aPixelFormat = 0x555;
	else
		DBG_ASSERTE(FALSE);

	BltTrianglesTexHelper(theTexture,theVertices,theNumTriangles,theClipRect,theColor,theDrawMode,mLockedSurfaceDesc.lpSurface,mLockedSurfaceDesc.lPitch,aPixelFormat,tx,ty,blend);
	UnlockSurface();

	DeleteAllNonSurfaceData();
}

bool DDImage::Palletize()
{
	if (MemoryImage::Palletize())
	{
		// Don't keep around the DDSurface if we palletize the image, that
		// would be a waste of memory		
		DeleteDDSurface();
		return true;
	}
	else
	{		
		return false;
	}
}

void DDImage::FillScanLinesWithCoverage(Span* theSpans, int theSpanCount, const Color& theColor, int theDrawMode, const BYTE* theCoverage, int theCoverX, int theCoverY, int theCoverWidth, int theCoverHeight)
{
	if (theSpanCount == 0) return;

	if (Check3D(this))
	{ // ugh!#@$
		int l = theSpans[0].mX, t = theSpans[0].mY;
		int r = l + theSpans[0].mWidth, b = t;
		for (int i = 1; i < theSpanCount; ++i)  //此循环结束后，Rect(l, t, r - l + 1, b - t + 1) 即为包含所有 Span 的最小矩形区域
		{
			l = std::min(theSpans[i].mX, l);
			r = std::max(theSpans[i].mX + theSpans[i].mWidth - 1, r);
			t = std::min(theSpans[i].mY, t);
			b = std::max(theSpans[i].mY, b);
		}
		for (int i = 0; i < theSpanCount; ++i)  //此循环将所有 Span 的绝对坐标更改为在上述矩形区域内的相对坐标
		{
			theSpans[i].mX -= l;
			theSpans[i].mY -= t;
		}

		MemoryImage aTempImage;
		aTempImage.Create(r-l+1, b-t+1);  //创建一个与最小矩形区域相同大小的 MemoryImage
		//theCoverX - l 和 theCoverY - t 分别将绝对坐标转化为 MemoryImage 上的相对坐标
		aTempImage.FillScanLinesWithCoverage(theSpans, theSpanCount, theColor, theDrawMode, theCoverage, theCoverX - l, theCoverY - t, theCoverWidth, theCoverHeight);
		Blt(&aTempImage, l, t, Rect(0, 0, r-l+1, b-t+1), Color::White, theDrawMode);
		return;
	}

	LPDIRECTDRAWSURFACE aSurface = GetSurface();
	(void)aSurface; // unused
	
	if (!LockSurface())
		return;

	ulong aRMask = mLockedSurfaceDesc.ddpfPixelFormat.dwRBitMask;
	ulong aGMask = mLockedSurfaceDesc.ddpfPixelFormat.dwGBitMask;
	ulong aBMask = mLockedSurfaceDesc.ddpfPixelFormat.dwBBitMask;

	// unused
	// ulong aRRoundAdd = aRMask >> 1;
	// ulong aGRoundAdd = aGMask >> 1;
	// ulong aBRoundAdd = aBMask >> 1;
	
	if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 16)
	{
		//ushort src_red		= (((theColor.mRed * (theColor.mAlpha+1)) >> 8) * aRMask) & aRMask;
		//ushort src_green	= (((theColor.mGreen * (theColor.mAlpha+1)) >> 8) * aGMask) & aGMask;
		//ushort src_blue		= (((theColor.mBlue * (theColor.mAlpha+1)) >> 8) * aBMask) & aBMask;
		ushort src = 
			(((theColor.mRed * aRMask) >> 8) & aRMask) |
			(((theColor.mGreen * aGMask) >> 8) & aGMask) |
			(((theColor.mBlue * aBMask) >> 8) & aBMask);
		ushort* theBits = (ushort*)mLockedSurfaceDesc.lpSurface;
		
		for (int i = 0; i < theSpanCount; ++i)
		{
			Span* aSpan = &theSpans[i];
			int x = aSpan->mX - theCoverX;
			int y = aSpan->mY - theCoverY;

			ushort* aDestPixels = &theBits[aSpan->mY*mWidth + aSpan->mX];
			const BYTE* aCoverBits = &theCoverage[y*theCoverWidth+x];
			for (int w = 0; w < aSpan->mWidth; ++w)
			{
				int cover = *aCoverBits++;
				int a = ((cover+1) * theColor.mAlpha) >> 8;
				int oma = 256 - a;
				ushort dest = *aDestPixels;
				
				*(aDestPixels++) = 
					((((dest & aRMask) * oma + (src & aRMask) * a) >> 8) & aRMask) |
					((((dest & aGMask) * oma + (src & aGMask) * a) >> 8) & aGMask) |
					((((dest & aBMask) * oma + (src & aBMask) * a) >> 8) & aBMask);
			}
		}
	}
	else if (mLockedSurfaceDesc.ddpfPixelFormat.dwRGBBitCount == 32)
	{
		//ulong src_red		= (((theColor.mRed * (theColor.mAlpha+1)) >> 8) * aRMask) & aRMask;
		//ulong src_green		= (((theColor.mGreen * (theColor.mAlpha+1)) >> 8) * aGMask) & aGMask;
		//ulong src_blue		= (((theColor.mBlue * (theColor.mAlpha+1)) >> 8) * aBMask) & aBMask;
		ulong src = 
			(((theColor.mRed * aRMask) >> 8) & aRMask) |
			(((theColor.mGreen * aGMask) >> 8) & aGMask) |
			(((theColor.mBlue * aBMask) >> 8) & aBMask);
		ulong* theBits = (ulong*)mLockedSurfaceDesc.lpSurface;
		
		for (int i = 0; i < theSpanCount; ++i)
		{
			Span* aSpan = &theSpans[i];
			int x = aSpan->mX - theCoverX;
			int y = aSpan->mY - theCoverY;

			ulong* aDestPixels = &theBits[aSpan->mY*mWidth + aSpan->mX];
			const BYTE* aCoverBits = &theCoverage[y*theCoverWidth+x];
			for (int w = 0; w < aSpan->mWidth; ++w)
			{
				int cover = *aCoverBits++;
				int a = ((cover+1) * theColor.mAlpha) >> 8;
				int oma = 256 - a;
				ulong dest = *aDestPixels;
				
				*(aDestPixels++) = 
					((((dest & aRMask) * oma + (src & aRMask) * a) >> 8) & aRMask) |
					((((dest & aGMask) * oma + (src & aGMask) * a) >> 8) & aGMask) |
					((((dest & aBMask) * oma + (src & aBMask) * a) >> 8) & aBMask);
			}
		}
	}

	UnlockSurface();
	DeleteAllNonSurfaceData();
}
