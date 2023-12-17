#define INITGUID

#include "DDInterface.h"
#include "DDImage.h"
#include "D3DInterface.h"
#include "SexyAppBase.h"
#include "misc/AutoCrit.h"
#include "Graphics.h"
#include "misc/PerfTimer.h"
#include "misc/Debug.h"
#include "misc/DirectXErrorString.h"
#include "MemoryImage.h"

using namespace Sexy;

typedef HRESULT (WINAPI *DirectDrawCreateFunc)(GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);
typedef HRESULT (WINAPI *DirectDrawCreateExFunc)(GUID FAR *lpGUID, LPVOID *lplpDD, REFIID iid, IUnknown FAR *pUnkOuter);

//extern HMODULE gDDrawDLL;
static DirectDrawCreateFunc gDirectDrawCreateFunc = NULL;
static DirectDrawCreateExFunc gDirectDrawCreateExFunc = NULL;

DDInterface::DDInterface(SexyAppBase* theApp)
{
	mApp = theApp;
	mPrimarySurface = NULL;
	mDrawSurface = NULL;
	mSecondarySurface = NULL;
	mScreenImage = NULL;
	mDD = NULL;
	mDD7 = NULL;
	mRedAddTable = NULL;
	mGreenAddTable = NULL;
	mBlueAddTable = NULL;
	mInitialized = false;
	mVideoOnlyDraw = false;	
	mScanLineFailCount = 0;

	//TODO: Standards, anyone?
	mNextCursorX = 0;
	mNextCursorY = 0;
	mCursorX = 0;
	mCursorY = 0;
	mInRedraw = false;
	//mCursorWidth = 54;
	//mCursorHeight = 54;
	mCursorWidth = 64;
	mCursorHeight = 64;
	mCursorImage = NULL;
	mOldCursorArea = NULL;
	mNewCursorArea = NULL;
	mHasOldCursorArea = false;
	mOldCursorArea = NULL;
	mNewCursorArea = NULL;
	mNewCursorAreaImage = NULL;
	mOldCursorAreaImage = NULL;
	mInitCount = 0;
	mRefreshRate = 60;
	mMillisecondsPerFrame = 1000/mRefreshRate;

	mD3DInterface = new D3DInterface;

	gDirectDrawCreateFunc = (DirectDrawCreateFunc)GetProcAddress(gDDrawDLL,"DirectDrawCreate");
	gDirectDrawCreateExFunc = (DirectDrawCreateExFunc)GetProcAddress(gDDrawDLL,"DirectDrawCreateEx");
}

DDInterface::~DDInterface()
{
	delete [] mRedAddTable;
	delete [] mGreenAddTable;
	delete [] mBlueAddTable;

	Cleanup();

	delete mD3DInterface;
}

std::string DDInterface::ResultToString(int theResult)
{
	switch (theResult)
	{
	case RESULT_OK:
		return "RESULT_OK";
	case RESULT_FAIL:
		return "RESULT_FAIL";
	case RESULT_DD_CREATE_FAIL:
		return "RESULT_DD_CREATE_FAIL";
	case RESULT_SURFACE_FAIL:
		return "RESULT_SURFACE_FAIL";
	case RESULT_EXCLUSIVE_FAIL:
		return "RESULT_EXCLUSIVE_FAIL";
	case RESULT_DISPCHANGE_FAIL:
		return "RESULT_DISPCHANGE_FAIL";		
	case RESULT_INVALID_COLORDEPTH:
		return "RESULT_INVALID_COLORDEPTH";
	default:
		return "RESULT_UNKNOWN";
	}
}

bool DDInterface::GotDXError(HRESULT theResult, const char *theContext)
{ 
	if (!SUCCEEDED(theResult))
	{
		std::string anError = GetDirectXErrorString(theResult);
		mErrorString = StrFormat("%s: %s",theContext, anError.c_str());

		return true;
	}
	else
		return false;
}


DDImage* DDInterface::GetScreenImage()
{
	return mScreenImage;
}

HRESULT	DDInterface::CreateSurface(DDSURFACEDESC2 *theDesc, LPDIRECTDRAWSURFACE *theSurface, void*)
{
	AutoCrit aCrit(mCritSect);
	HRESULT aResult;
	if (mDD7 != NULL)
	{
		LPDIRECTDRAWSURFACE7 aSurface;
		aResult = mDD7->CreateSurface(theDesc, &aSurface, NULL);
		if (!SUCCEEDED(aResult))
			return aResult;

		aResult = aSurface->QueryInterface(IID_IDirectDrawSurface, (LPVOID*)theSurface);
		aSurface->Release();

		if (!SUCCEEDED(aResult))
			return aResult;
	}
	else
	{
		DDSURFACEDESC aDesc;
		ZeroMemory(&aDesc,sizeof(aDesc));
		aDesc.dwSize = sizeof(aDesc);
		aDesc.dwFlags = theDesc->dwFlags;
		aDesc.dwHeight = theDesc->dwHeight;
		aDesc.dwWidth = theDesc->dwWidth;
		aDesc.lPitch = theDesc->lPitch;
		aDesc.dwBackBufferCount = theDesc->dwBackBufferCount;
		aDesc.dwRefreshRate = theDesc->dwRefreshRate;
		aDesc.dwAlphaBitDepth = theDesc->dwAlphaBitDepth;
		aDesc.dwReserved = theDesc->dwReserved;
		aDesc.lpSurface = theDesc->lpSurface;
	    aDesc.ddpfPixelFormat = theDesc->ddpfPixelFormat;        
		aDesc.ddsCaps.dwCaps = theDesc->ddsCaps.dwCaps;                

		aResult = mDD->CreateSurface(&aDesc, theSurface, NULL);
		if (!SUCCEEDED(aResult))
			return aResult;
	}

	// Make sure it's 32-bit or 16-bit
	DDSURFACEDESC aDesc;
	ZeroMemory(&aDesc, sizeof(aDesc));
	aDesc.dwSize = sizeof(aDesc);
	aDesc.dwFlags = DDSD_PIXELFORMAT;
	mPrimarySurface->GetSurfaceDesc(&aDesc);
#ifdef OPTIMIZE_SOFTWARE_DRAWING
	// If things are stored blue low, green middle, red high, we can optimize a lot of our software rendering based on bit patterns.
	// This of course does not matter for native data which is already in the correct order (and can be optimized similarly).
	gOptimizeSoftwareDrawing = aDesc.ddpfPixelFormat.dwBBitMask < aDesc.ddpfPixelFormat.dwGBitMask && aDesc.ddpfPixelFormat.dwGBitMask < aDesc.ddpfPixelFormat.dwRBitMask;
#endif
	int aNumBits = aDesc.ddpfPixelFormat.dwRGBBitCount;
	if (aNumBits!=16 && aNumBits!=32)
	{
		(*theSurface)->Release();
		*theSurface = NULL;
		return DDERR_INVALIDPIXELFORMAT;
	}

	return aResult;
}

void DDInterface::ClearSurface(LPDIRECTDRAWSURFACE theSurface)
{
	if (theSurface)
	{
		DDSURFACEDESC desc;
		memset(&desc, 0, sizeof desc);
		desc.dwSize = sizeof desc;
		HRESULT hr = theSurface->Lock(NULL, &desc, DDLOCK_WAIT | DDLOCK_WRITEONLY, NULL);
		if ( DD_OK == hr )
		{
			DWORD pixelSize = desc.ddpfPixelFormat.dwRGBBitCount / 8;
			unsigned char* p = (unsigned char*)desc.lpSurface;
			for ( DWORD row = 0; row < desc.dwHeight; ++row )
			{
				memset(p, 0, pixelSize*desc.dwWidth);
				p += desc.lPitch;
			}
			theSurface->Unlock(NULL);
		}
	}
}

int DDInterface::Init(HWND theWindow, bool IsWindowed)
{
	AutoCrit anAutoCrit(mCritSect);

	mInitialized = false;

	/*DDImageSet aReleasedImageSet;

	DDImageSet::iterator anItr = mDDImageSet.begin();
	while (anItr != mDDImageSet.end())
	{
		DDImage* aDDImage = *anItr;

		if ((aDDImage->mSurface != NULL) || aDDImage->mWantDDSurface || !aDDImage->mKeepBits)
		{
			aDDImage->DeleteDDSurface();
			aReleasedImageSet.insert(aDDImage);
		}

		++anItr;
	}*/

	Cleanup();

	HRESULT aResult;
	DDSURFACEDESC2 aDesc;

	if (gDirectDrawCreateExFunc != NULL)
	{
		aResult = gDirectDrawCreateExFunc(NULL, (LPVOID*)&mDD7, IID_IDirectDraw7, NULL); 
		if (GotDXError(aResult, "DirectDrawCreateEx"))
			return RESULT_DD_CREATE_FAIL;

		aResult = mDD7->QueryInterface(IID_IDirectDraw, (LPVOID*)&mDD);
		if (GotDXError(aResult, "QueryrInterface IID_IDirectDraw"))
			return RESULT_DD_CREATE_FAIL;		
	}
	else 
	{
		aResult = gDirectDrawCreateFunc(NULL, &mDD, NULL); 		
	}

	RECT aRect;
	GetClientRect(theWindow, &aRect);

	mHWnd = theWindow;
	mWidth = mApp->mWidth;
	mHeight = mApp->mHeight;
	mAspect.Set(mWidth, mHeight);
	mDesktopWidth = GetSystemMetrics(SM_CXSCREEN);
	mDesktopHeight = GetSystemMetrics(SM_CYSCREEN);
	mDesktopAspect.Set(mDesktopWidth, mDesktopHeight);
	mDisplayWidth = mWidth;
	mDisplayHeight = mHeight;
	mDisplayAspect = mAspect;
	mPresentationRect = Rect( 0, 0, mWidth, mHeight );
	mApp->mScreenBounds = mPresentationRect;
	mFullscreenBits = mApp->mFullscreenBits;
	mIsWindowed = IsWindowed;
	mHasOldCursorArea = false;

	OutputDebug(_S("Application requests %4lu x %4lu [%2d:%2d]\n"), mWidth, mHeight, mAspect.mNumerator, mAspect.mDenominator);

	if (GotDXError(aResult, "DirectDrawCreate"))
		return RESULT_DD_CREATE_FAIL;	
	
	if (IsWindowed)
	{
		OutputDebug(_S("Hack aspect is                   [%2d:%2d]\n"), mApp->mWindowAspect.mNumerator, mApp->mWindowAspect.mDenominator);

		if ( mApp->mEnableWindowAspect && mAspect < mApp->mWindowAspect )
		{
			mIsWidescreen = true;

			// Setup the window at the hack aspect,
			// but with the height requested by the application.
			mDisplayWidth = mHeight * mApp->mWindowAspect;
			mDisplayHeight = mHeight;
			mDisplayAspect = mApp->mWindowAspect;

			// resize the window.
			RECT rc;
			POINT pt;
			WINDOWINFO info;
			::GetWindowInfo(theWindow, &info);
			::GetClientRect(theWindow, &rc);
			pt.x = rc.left;
			pt.y = rc.top;
			::ClientToScreen(theWindow, &pt);
			rc.left = pt.x - (mDisplayWidth - mWidth) / 2;
			rc.top = pt.y - (mDisplayHeight - mHeight) / 2;
			rc.right = rc.left + mDisplayWidth;
			rc.bottom = rc.top + mDisplayHeight;
			::AdjustWindowRectEx(&rc, info.dwStyle, false, info.dwExStyle);
			::MoveWindow(theWindow, std::max((LONG)0, rc.left), std::max((LONG)0, rc.top), rc.right-rc.left, rc.bottom-rc.top, false);

			if ( mApp->mWidescreenAware )
			{
				mWidth = mDisplayWidth;
				mHeight = mDisplayHeight;
				mAspect = mDisplayAspect;

				mPresentationRect.mWidth = mDisplayWidth;
				mPresentationRect.mHeight = mDisplayHeight;
				mPresentationRect.mX = 0;
				mPresentationRect.mY = 0;
			}
			else
			{
				// Set the dest rect for drawing the back buffer to the center of
				// the wide display.
				mPresentationRect.mWidth = mWidth;
				mPresentationRect.mHeight = mHeight;
				mPresentationRect.mX = ( mDisplayWidth - mPresentationRect.mWidth ) / 2;
				mPresentationRect.mY = 0;
			}
		}

		OutputDebug(_S("Window is            %4lu x %4lu [%2d:%2d]\n"), mDisplayWidth, mDisplayHeight, mDisplayAspect.mNumerator, mDisplayAspect.mDenominator);

		aResult = mDD->SetCooperativeLevel(theWindow, DDSCL_NORMAL);

		ZeroMemory(&aDesc, sizeof(aDesc));
		aDesc.dwSize = sizeof(aDesc);
		aDesc.dwFlags = DDSD_CAPS;
    
		aDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		aDesc.ddsCaps.dwCaps |= DDSCAPS_3DDEVICE;
		
		aDesc.dwBackBufferCount = 1;
		aResult = CreateSurface(&aDesc, &mPrimarySurface, NULL);
		if (aResult==DDERR_INVALIDPIXELFORMAT) // check for non 16 or 32 bit primary surface
			return RESULT_INVALID_COLORDEPTH;
		else if (GotDXError(aResult, "CreateSurface Windowed Primary"))
			return RESULT_SURFACE_FAIL;				

		ZeroMemory(&aDesc, sizeof(aDesc));
		aDesc.dwSize = sizeof(aDesc);
		aDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		aDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		aDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | DDSCAPS_3DDEVICE;

		aDesc.dwWidth = mWidth;
		aDesc.dwHeight = mHeight;

		aResult = CreateSurface(&aDesc, &mDrawSurface, NULL);
		if (GotDXError(aResult,"CreateSurface Windowed DrawSurface"))
			return RESULT_SURFACE_FAIL;

		IDirectDrawClipper* aClipper;
		aResult = mDD->CreateClipper(0, &aClipper, NULL);
		if (GotDXError(aResult,"CreateClipper Windowed")) return RESULT_FAIL;

		// Associate the clipper with the window
		aResult = aClipper->SetHWnd(0, theWindow);
		if (SUCCEEDED(aResult)) 
			aResult = mPrimarySurface->SetClipper(aClipper);

		aClipper->Release();
	}
	else
	{
		OutputDebug(_S("Desktop is           %4lu x %4lu [%2d:%2d]\n"), mDesktopWidth, mDesktopHeight, mDesktopAspect.mNumerator, mDesktopAspect.mDenominator);

		if ( mAspect < mDesktopAspect )
		{
			mIsWidescreen = true;

			// Set the display mode to the size of the desktop.
			mDisplayWidth = mDesktopWidth;
			mDisplayHeight = mDesktopHeight;
			mDisplayAspect = mDesktopAspect;

			if ( mApp->mWidescreenAware )
			{
				// Setup the draw buffer(s) at the same aspect ratio as the desktop,
				// but with the height requested by the application.
				mAspect = mDisplayAspect;
				mWidth = mHeight * mAspect;

				mPresentationRect.mWidth = mDisplayWidth;
				mPresentationRect.mHeight = mDisplayHeight;
				mPresentationRect.mX = 0;
				mPresentationRect.mY = 0;
			}
			else
			{
				// Set the dest rect for drawing the back buffer to the center of
				// the wide display.
				mPresentationRect.mWidth = mWidth * mDisplayHeight / mHeight;
				mPresentationRect.mHeight = mDisplayHeight;
				mPresentationRect.mX = ( mDisplayWidth - mPresentationRect.mWidth ) / 2;
				mPresentationRect.mY = 0;
			}
		}

		OutputDebug(_S("Display is           %4lu x %4lu [%2d:%2d]\n"), mDisplayWidth, mDisplayHeight, mDisplayAspect.mNumerator, mDisplayAspect.mDenominator);

		aResult = mDD->SetCooperativeLevel(theWindow, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
		if (GotDXError(aResult,"SetCooperativeLevel FullScreen"))
			return RESULT_EXCLUSIVE_FAIL;

		aResult = mDD->SetDisplayMode(mDisplayWidth, mDisplayHeight, mFullscreenBits);

		if (GotDXError(aResult,"SetDisplayMode FullScreen"))
			return RESULT_DISPCHANGE_FAIL;

		ZeroMemory(&aDesc, sizeof(aDesc));
		aDesc.dwSize = sizeof(aDesc);
		aDesc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		aDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
							  DDSCAPS_FLIP |
							  DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE/* |
							  ,DDSCAPS_FRONTBUFFER*/;
		aDesc.dwBackBufferCount = 1;

		aResult = CreateSurface(&aDesc, &mPrimarySurface, NULL);
		if (GotDXError(aResult,"CreateSurface FullScreen Primary"))
			return RESULT_SURFACE_FAIL;

		DDSCAPS aCaps;		
		ZeroMemory(&aCaps,sizeof(aCaps));
		aCaps.dwCaps = DDSCAPS_BACKBUFFER;
		aResult = mPrimarySurface->GetAttachedSurface(&aCaps, &mSecondarySurface);

		if (GotDXError(aResult,"GetAttachedSurface"))
			return RESULT_SURFACE_FAIL;

		if (mIsWidescreen)
		{
			ClearSurface(mPrimarySurface);
			ClearSurface(mSecondarySurface);
		}

		ZeroMemory(&aDesc, sizeof(aDesc));
		aDesc.dwSize = sizeof(aDesc);
		aDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		aDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY | DDSCAPS_3DDEVICE;

		aDesc.dwWidth = mWidth;
		aDesc.dwHeight = mHeight;		

		aResult = CreateSurface(&aDesc, &mDrawSurface, NULL);
		if (GotDXError(aResult, "CreateSurface FullScreen DrawSurface"))
			return RESULT_SURFACE_FAIL;
		
		IDirectDrawClipper* aClipper;
		aResult = mDD->CreateClipper(0, &aClipper, NULL);
		if (GotDXError(aResult,"CreateClipper FullScreen")) return RESULT_FAIL;

		// Associate the clipper with the window
		aResult = aClipper->SetHWnd(0, theWindow);
		if (SUCCEEDED(aResult)) 
			aResult = mPrimarySurface->SetClipper(aClipper);

		aClipper->Release();

		if (!mApp->mFullScreenPageFlip)
			mDD->FlipToGDISurface();
	}
	
	OutputDebug(_S("Draw buffer is       %4lu x %4lu [%2d:%2d]\n"), mWidth, mHeight, mAspect.mNumerator, mAspect.mDenominator);

	if (FAILED(mDD->GetMonitorFrequency(&mRefreshRate)) || mRefreshRate<60)
	{
		mApp->mVSyncBroken = true;
		mRefreshRate = 60;
	}

	if (mRefreshRate < 60)
	{
		mApp->mVSyncBroken = true;
		mRefreshRate = 60;		
	}
	else if (mRefreshRate > 100) // We must have at least 1 Update per UpdateF for demo compatibility
	{
		mApp->mVSyncBroken = true;
		mRefreshRate = 100; 
	}

	mMillisecondsPerFrame = 1000/mRefreshRate;

	// Create the images needed for cursor stuff
	ZeroMemory(&aDesc, sizeof(aDesc));
	aDesc.dwSize = sizeof(aDesc);
	aDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	aDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	aDesc.dwWidth = mCursorWidth;
	aDesc.dwHeight = mCursorHeight;

	aResult = CreateSurface(&aDesc, &mOldCursorArea, NULL);
	if (GotDXError(aResult, "CreateSurface OldCursorArea"))
		return RESULT_SURFACE_FAIL;
	
	aResult = CreateSurface(&aDesc, &mNewCursorArea, NULL);
	if (GotDXError(aResult, "CreateSurface NewCursorArea"))
		return RESULT_SURFACE_FAIL;

	mOldCursorAreaImage = new DDImage(this);
	mOldCursorAreaImage->SetSurface(mOldCursorArea);
	mOldCursorAreaImage->SetImageMode(false, false);

	mNewCursorAreaImage = new DDImage(this);
	mNewCursorAreaImage->SetSurface(mNewCursorArea);	
	mNewCursorAreaImage->SetImageMode(false, false);

	// Get data from the primary surface
	if (mPrimarySurface != NULL)
	{
		DDSURFACEDESC aDesc;

		ZeroMemory(&aDesc, sizeof(aDesc));
		aDesc.dwSize = sizeof(aDesc);
		aDesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		HRESULT aResult = mPrimarySurface->GetSurfaceDesc(&aDesc); (void)aResult;

		if ((aDesc.ddpfPixelFormat.dwRGBBitCount != 16) &&
			(aDesc.ddpfPixelFormat.dwRGBBitCount != 32))
			return RESULT_INVALID_COLORDEPTH;

		mRGBBits = aDesc.ddpfPixelFormat.dwRGBBitCount;
		mRedMask = aDesc.ddpfPixelFormat.dwRBitMask;
		mGreenMask = aDesc.ddpfPixelFormat.dwGBitMask;
		mBlueMask = aDesc.ddpfPixelFormat.dwBBitMask;

		int i;
		for (i = 31; i >= 0; i--) // @ Minerscale Fix UB shift by 32 bits
		{
			if (((mRedMask >> i) & 1) != 0)
				mRedShift = i;
			if (((mGreenMask >> i) & 1) != 0)
				mGreenShift = i;
			if (((mBlueMask >> i) & 1) != 0)
				mBlueShift = i;						
		}

		for (i = 0; i < 32; i++)
		{	
			if ((i+mRedShift < 32) && ((mRedMask >> (i+mRedShift)) != 0))
				mRedBits = i+1;
			if ((i+mGreenShift < 32) && ((mGreenMask >> (i+mGreenShift)) != 0))
				mGreenBits = i+1;
			if ((i+mBlueShift < 32) && ((mBlueMask >> (i+mBlueShift)) != 0))
				mBlueBits = i+1;
		}

		delete [] mRedAddTable;
		delete [] mGreenAddTable;
		delete [] mBlueAddTable;		

		int aMaxR = (1<<mRedBits) - 1;
		int aMaxG = (1<<mGreenBits) - 1;
		int aMaxB = (1<<mBlueBits) - 1;

		mRedAddTable = new int[aMaxR*2+1];
		mGreenAddTable = new int[aMaxG*2+1];
		mBlueAddTable = new int[aMaxB*2+1];

		for (i = 0; i < aMaxR*2+1; i++)
			mRedAddTable[i] = std::min(i, aMaxR);
		for (i = 0; i < aMaxG*2+1; i++)
			mGreenAddTable[i] = std::min(i, aMaxG);
		for (i = 0; i < aMaxB*2+1; i++)
			mBlueAddTable[i] = std::min(i, aMaxB);

		// Create the tables that we will use to convert from 
		// internal color representation to surface representation
		for (i = 0; i < 256; i++)
		{
			mRedConvTable[i] = ((i * mRedMask) / 255) & mRedMask;
			mGreenConvTable[i] = ((i * mGreenMask) / 255) & mGreenMask;
			mBlueConvTable[i] = ((i * mBlueMask) / 255) & mBlueMask;
		}
	}

	// Generate DDSurfaces for all our... image things
	/*anItr = aReleasedImageSet.begin();
	while (anItr != aReleasedImageSet.end())
	{
		DDImage* aDDImage = *anItr;

		DDImageSet::iterator aFindItr = mDDImageSet.find(aDDImage);
		if (aFindItr != mDDImageSet.end())
		{
			if (mIs3D)
			{
				if (aDDImage->mWantPal)
					aDDImage->mWantPal = aDDImage->Palletize();
			}
			else
			{
				//if (aDDImage->mOnlyKeepDDSurface)
				//	aDDImage->OnlyKeepDDSurface();
				//else if (aDDImage->mWantDDSurface)
				//	aDDImage->GenerateDDSurface();
			}

			aDDImage->GenerateExtraBuffers();
			
		}

		++anItr;
	}*/

	SetVideoOnlyDraw(mVideoOnlyDraw);
	if(!mD3DInterface->InitFromDDInterface(this))
	{
		mErrorString = "3D init error: ";
		mErrorString += D3DInterface::mErrorString;
		return RESULT_3D_FAIL;
	}

	mInitCount++;
	mInitialized = true;

	return RESULT_OK;
}

void DDInterface::SetVideoOnlyDraw(bool videoOnlyDraw)
{
	AutoCrit anAutoCrit(mCritSect);

	mVideoOnlyDraw = videoOnlyDraw;

	if (mSecondarySurface == NULL)
	{
		DDSURFACEDESC2 aDesc;

		ZeroMemory(&aDesc, sizeof(aDesc));
		aDesc.dwSize = sizeof(aDesc);
		aDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		aDesc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY;
		aDesc.dwWidth = mWidth;
		aDesc.dwHeight = mHeight;

		HRESULT aResult = CreateSurface(&aDesc, &mSecondarySurface, NULL);

		if (FAILED(aResult))
			mVideoOnlyDraw = false;
	}

	bool useSecondary = mVideoOnlyDraw;
	delete mScreenImage;
	mScreenImage = new DDImage(this);
	mScreenImage->SetSurface(useSecondary ? mSecondarySurface : mDrawSurface);		
	mScreenImage->mNoLock = mVideoOnlyDraw;
	mScreenImage->mVideoMemory = mVideoOnlyDraw;
	mScreenImage->SetImageMode(false, false);
}

void DDInterface::RemapMouse(int& theX, int& theY)
{
	if (mInitialized)
	{
		theX = ( theX - mPresentationRect.mX ) * mWidth / mPresentationRect.mWidth;
		theY = ( theY - mPresentationRect.mY ) * mHeight / mPresentationRect.mHeight;
	}
}

ulong DDInterface::GetColorRef(ulong theRGB)
{
	DDSURFACEDESC aDesc;

	ZeroMemory(&aDesc, sizeof(aDesc));
	aDesc.dwSize = sizeof(aDesc);
    aDesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
    HRESULT aResult = mPrimarySurface->GetSurfaceDesc(&aDesc); (void)aResult;

	BYTE bRed   = (BYTE) ((theRGB >> 16) & 0xFF);
    BYTE bGreen = (BYTE) ((theRGB >>  8) & 0xFF);
    BYTE bBlue  = (BYTE) ((theRGB      ) & 0xFF);

	ulong aColor;
	aColor = ((DWORD(((LONGLONG)bRed * (LONGLONG)aDesc.ddpfPixelFormat.dwRBitMask) / 255) & aDesc.ddpfPixelFormat.dwRBitMask) |
			(DWORD(((LONGLONG)bGreen * (LONGLONG)aDesc.ddpfPixelFormat.dwGBitMask) / 255) & aDesc.ddpfPixelFormat.dwGBitMask) |
			(DWORD(((LONGLONG)bBlue * (LONGLONG)aDesc.ddpfPixelFormat.dwBBitMask) / 255) & aDesc.ddpfPixelFormat.dwBBitMask));

	return aColor;
}

void DDInterface::AddDDImage(DDImage* theDDImage)
{
	AutoCrit anAutoCrit(mCritSect);

	mDDImageSet.insert(theDDImage);
}

void DDInterface::RemoveDDImage(DDImage* theDDImage)
{
	AutoCrit anAutoCrit(mCritSect);

	DDImageSet::iterator anItr = mDDImageSet.find(theDDImage);
	if (anItr != mDDImageSet.end())
		mDDImageSet.erase(anItr);
}

void DDInterface::Remove3DData(MemoryImage* theImage) // for 3d texture cleanup
{
	mD3DInterface->RemoveMemoryImage(theImage);
}

void DDInterface::Cleanup()
{
	AutoCrit anAutoCrit(mCritSect);

	mInitialized = false;	
	mD3DInterface->Cleanup();

	if (mOldCursorAreaImage != NULL)
	{
		delete mOldCursorAreaImage;
		mOldCursorAreaImage = NULL;
	}

	if (mNewCursorAreaImage != NULL)
	{
		delete mNewCursorAreaImage;
		mNewCursorAreaImage = NULL;
	}

	if (mOldCursorArea != NULL)
	{
		mOldCursorArea->Release();
		mOldCursorArea = NULL;
	}

	if (mNewCursorArea != NULL)
	{
		mNewCursorArea->Release();
		mNewCursorArea = NULL;
	}	

	if (mScreenImage != NULL)	
	{
		delete mScreenImage;
		mScreenImage = NULL;
	}

	if (mDrawSurface != NULL)
	{
		mDrawSurface->Release();
		mDrawSurface = NULL;
	}

	if (mSecondarySurface != NULL)
	{
		mSecondarySurface->Release();
		mSecondarySurface = NULL;
	}

	if (mPrimarySurface != NULL)	
	{
		mPrimarySurface->Release();	
		mPrimarySurface = NULL;
	}
	
	if (mDD != NULL)
	{
		mDD->SetCooperativeLevel(mHWnd, DDSCL_NORMAL);		
		mDD->Release();
		mDD = NULL;
	}

	if (mDD7 != NULL)
	{
		mDD7->Release();
		mDD7 = NULL;
	}
}

bool DDInterface::CopyBitmap(LPDIRECTDRAWSURFACE theSurface, HBITMAP theBitmap, int theX, int theY, unsigned int theWidth, unsigned int theHeight)
{
	AutoCrit anAutoCrit(mCritSect);

    HRESULT hr;

    if (theBitmap == NULL || theSurface == NULL) return false;

    // Make sure this surface is restored.
    theSurface->Restore();

    // Get size of the bitmap
    BITMAP bmBitmap;

    GetObject(theBitmap, sizeof(bmBitmap), &bmBitmap);
    theWidth  = (theWidth  == 0) ? bmBitmap.bmWidth : theWidth;
    theHeight = (theHeight == 0) ? bmBitmap.bmHeight : theHeight;

    DDSURFACEDESC aDesc;
    ZeroMemory(&aDesc, sizeof(aDesc));
	aDesc.dwSize = sizeof(aDesc);
    aDesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    hr = theSurface->GetSurfaceDesc(&aDesc);
    if (FAILED(hr)) return false;

    // Create memory DC
    HDC hdcImage = CreateCompatibleDC(NULL);
    if (hdcImage != NULL)
    {
        // Select bitmap into memory DC
        HBITMAP anOldBitmap = (HBITMAP)SelectObject(hdcImage, theBitmap);

        // Get surface DC
        HDC hdc;
        hr = theSurface->GetDC(&hdc);
        if (SUCCEEDED(hr))
        {
            // Copy the bitmap. Use BitBlt, if possible, otherwise use
            // StretchBlt
            if (theWidth == aDesc.dwWidth && theHeight == aDesc.dwHeight)
            {
                BitBlt(hdc, 0, 0, theWidth, theHeight, hdcImage, theX, theY, SRCCOPY);
            }
            else
            {
                StretchBlt(hdc, 0, 0, aDesc.dwWidth, aDesc.dwHeight, hdcImage,
                           theX, theY, theWidth, theHeight, SRCCOPY);
            }

            // Release surface DC
            theSurface->ReleaseDC(hdc);
        }

        // Select old bitmap into the memory DC and delete the DC
        SelectObject(hdcImage, anOldBitmap);
        DeleteDC(hdcImage);
    }
    else return false;

    return true;
}

extern std::fstream gStreamThing;

bool DDInterface::Redraw(Rect* theClipRect)
{
	AutoCrit anAutoCrit(mCritSect);
		
	if (!mInitialized)
		return false;	

	DDBLTFX aBltFX;
	ZeroMemory(&aBltFX, sizeof(aBltFX));
	aBltFX.dwSize = sizeof(aBltFX);    	

	mInRedraw = true;

	if (!mD3DInterface->mErrorString.empty())
	{
		mInRedraw = false;
		return false;
	}

	mD3DInterface->Flush();

	RECT aDestRect;
	RECT aSrcRect;
	if (NULL == theClipRect || mIsWidescreen)
	{
		// ClipRect cannot be supported when the draw surface and
		// primary surface are not the same size in widescreen mode.
		aDestRect = mPresentationRect.ToRECT();
		aSrcRect = Rect(0, 0, mWidth, mHeight).ToRECT();
	}
	else
	{
		aDestRect = theClipRect->ToRECT();
		aSrcRect = theClipRect->ToRECT();
	}
	
	if ( mIsWidescreen )
	{
		aBltFX.dwDDFX = DDBLTFX_ARITHSTRETCHY;
	}

	POINT aPoint = {0, 0};		

	ClientToScreen(mHWnd, &aPoint);
	OffsetRect(&aDestRect, aPoint.x, aPoint.y);
	
	DDSURFACEDESC aDesc;
	ZeroMemory(&aDesc,sizeof(aDesc));
	aDesc.dwSize=sizeof(aDesc);
	mDrawSurface->Lock(NULL,&aDesc,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT|DDLOCK_READONLY ,0);
	mDrawSurface->Unlock(NULL);

	if (mIsWindowed)
	{
		HRESULT aResult;
		
		//DWORD aScanLine;
		//mDD->GetScanLine(&aScanLine);

		int aScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		if (!mVideoOnlyDraw)
		{
			mCursorX = mNextCursorX;
			mCursorY = mNextCursorY;

			DrawCursorTo(mDrawSurface, false);
			
			if ((mApp->mWaitForVSync) && (!mApp->mSoftVSyncWait))
			{
				bool scanLineFail = false;

				if (mScanLineFailCount >= 3)
					scanLineFail = true;

				if (!scanLineFail)
				{
					int aHalfMarkClient = mApp->mHeight / 2;
					int aHalfMarkScreen = aDestRect.top + aHalfMarkClient;					
					int aBotMarkScreen = aDestRect.top + mHeight;

					DWORD aScanLine = 0x7FFFFFFF;
					
					bool wasLess = false;

					DWORD aTopStartTick = GetTickCount();
					
					for (;;)
					{					
						aResult = mDD->GetScanLine(&aScanLine);

						if (aResult == DD_OK)
						{
							// Wait until we scan below half way on the window
							int aHalfMarkDist = aHalfMarkScreen - aScanLine;
							
							if ((aHalfMarkDist <= 0) || ((int) aScanLine >= aScreenHeight))
							{
								if (wasLess)
									break;
							}
							else
							{
								wasLess = true;
							}
						}
						else
						{
							if (aResult == DDERR_VERTICALBLANKINPROGRESS )
							{
								if (wasLess)
									break;
							}
							else
							{
								scanLineFail = true;
								break;
							}
						}

						DWORD aTickNow = GetTickCount();
						if (aTickNow - aTopStartTick >= 200)
						{
							// It shouldn't take this long
							scanLineFail = true;
							break;
						}
					}

					if (!scanLineFail)
					{
						mScanLineFailCount = 0;											

						RECT aTopDestRect = {aDestRect.left, aDestRect.top, aDestRect.right, aHalfMarkScreen};
						RECT aTopSrcRect = {aSrcRect.left, aSrcRect.top, aSrcRect.right, aHalfMarkClient};
						aResult = mPrimarySurface->Blt(&aTopDestRect, mDrawSurface, &aTopSrcRect, DDBLT_WAIT, &aBltFX);

						DWORD aLastScanLine = aScanLine;						

						for (;;)
						{			
							if (SUCCEEDED(mDD->GetScanLine(&aScanLine)))
							{	
								// Wait until we scan below the bottom of the window
								int aHalfMarkDist = aBotMarkScreen - aScanLine;
								if ((aScanLine < aLastScanLine) || (aHalfMarkDist <= 0))
									break;
							}
						}

						RECT aBotDestRect = {aDestRect.left, aHalfMarkScreen, aDestRect.right, aDestRect.bottom};
						RECT aBotSrcRect = {aSrcRect.left, aHalfMarkClient, aSrcRect.right, aSrcRect.bottom};
						aResult = mPrimarySurface->Blt(&aBotDestRect, mDrawSurface, &aBotSrcRect, DDBLT_WAIT, &aBltFX);
					}
					else
					{
						mScanLineFailCount++;
					}										
				}
				
				if (scanLineFail)
				{
					mDD->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
					aResult = mPrimarySurface->Blt(&aDestRect, mDrawSurface, &aSrcRect, DDBLT_WAIT, &aBltFX);
				}												
			}
			else
			{
				aResult = mPrimarySurface->Blt(&aDestRect, mDrawSurface, &aSrcRect, DDBLT_WAIT, &aBltFX);
			}

			if (mHasOldCursorArea)
			{
				// Restore from the drawn surface, incase we don't do a redraw
				//  of the drawn surface by next Redraw
				RestoreOldCursorAreaFrom(mDrawSurface, false);
				
				// Set it back to true so it gets removed from the primary 
				//  surface when we move the mouse
				mHasOldCursorArea = true;
			}
		}
		else
		{
			if ((mApp->mWaitForVSync) && (!mApp->mSoftVSyncWait))
				mDD->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);

			mCursorX = mNextCursorX;
			mCursorY = mNextCursorY;

			DrawCursorTo(mSecondarySurface, true);
			aResult = mPrimarySurface->Blt(&aDestRect, mSecondarySurface, &aSrcRect, DDBLT_WAIT, &aBltFX);
		}

		mInRedraw = false;
		return !GotDXError(aResult,"Redraw Windowed");
	}
	else
	{
		// Don't flip away from the GDI surface during the TryMedia purchasing process
		if (!mApp->mNoDefer && mApp->mFullScreenPageFlip)
		{
			if (!mVideoOnlyDraw) {
				HRESULT aResult = mSecondarySurface->Blt(&aDestRect, mDrawSurface, &aSrcRect, DDBLT_WAIT, &aBltFX);
				(void)aResult; // Unused
			}

			mCursorX = mNextCursorX;
			mCursorY = mNextCursorY;

			DrawCursorTo(mSecondarySurface, true);
						
			HRESULT aResult = mPrimarySurface->Flip(NULL, DDFLIP_WAIT);			

			mInRedraw = false;
			return !GotDXError(aResult,"Redraw FullScreen Flip");
		}
		else
		{
			HRESULT aResult = mPrimarySurface->Blt(&aDestRect, mDrawSurface, &aSrcRect, DDBLT_WAIT, &aBltFX);

			mInRedraw = false;
			return !GotDXError(aResult,"Redraw FullScreen Blt");
		}
	}
}

void DDInterface::RestoreOldCursorAreaFrom(LPDIRECTDRAWSURFACE theSurface, bool adjust)
{
	if ((mHasOldCursorArea) && (mPrimarySurface != NULL))
	{
		Rect aSexyScreenRect(
			mCursorX - (mCursorWidth / 2),
			mCursorY - (mCursorHeight / 2),
			mCursorWidth,
			mCursorHeight);

		Rect aClippedScreenRect = aSexyScreenRect.Intersection(Rect(0, 0, mWidth, mHeight));						
		
		Rect aSexyLocalRect(
			aClippedScreenRect.mX - aSexyScreenRect.mX, 
			aClippedScreenRect.mY - aSexyScreenRect.mY, 
			aClippedScreenRect.mWidth, 
			aClippedScreenRect.mHeight);
		
		if (adjust)
		{
			POINT aPoint = {0, 0};		
			ClientToScreen(mHWnd, &aPoint);
			aClippedScreenRect.Offset(aPoint.x, aPoint.y);
		}

		RECT aLocalRect = aSexyLocalRect.ToRECT();
		RECT aScreenRect = aClippedScreenRect.ToRECT();

		DDBLTFX aBltFX;
		ZeroMemory(&aBltFX, sizeof(aBltFX));
		aBltFX.dwSize = sizeof(aBltFX);

		// From mNewCursorArea to theSurface
		HRESULT aResult = theSurface->Blt(&aScreenRect, mOldCursorArea, &aLocalRect, DDBLT_WAIT, &aBltFX);
		(void)aResult; // Unused

		mHasOldCursorArea = false;
	}
}

void DDInterface::DrawCursorTo(LPDIRECTDRAWSURFACE theSurface, bool adjust)
{
	if ((mCursorImage != NULL) && (mPrimarySurface != NULL))
	{
		DDSURFACEDESC aDesc;
		ZeroMemory(&aDesc, sizeof(aDesc));
		aDesc.dwSize = sizeof(aDesc);
		aDesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		HRESULT aResult = mPrimarySurface->GetSurfaceDesc(&aDesc);
		Rect aDestSurfaceRect(0, 0, aDesc.dwWidth, aDesc.dwHeight);

		Rect aSexyScreenRect(
			mCursorX - (mCursorWidth / 2),
			mCursorY - (mCursorHeight / 2),
			mCursorWidth,
			mCursorHeight);

		Rect aClippedScreenRect = aSexyScreenRect.Intersection(mPresentationRect);
						
		Rect aSexyLocalRect(
			aClippedScreenRect.mX - aSexyScreenRect.mX, 
			aClippedScreenRect.mY - aSexyScreenRect.mY, 
			aClippedScreenRect.mWidth, 
			aClippedScreenRect.mHeight);

		if (adjust)
		{
			POINT aPoint = {0, 0};		
			ClientToScreen(mHWnd, &aPoint);
			aClippedScreenRect.Offset(aPoint.x, aPoint.y);
		}

		RECT aLocalRect = aSexyLocalRect.ToRECT();
		RECT aScreenRect = aClippedScreenRect.ToRECT();

		DDBLTFX aBltFX;
		ZeroMemory(&aBltFX, sizeof(aBltFX));
		aBltFX.dwSize = sizeof(aBltFX);
		
		// From theSurface to mOldCursorArea
		aResult = mOldCursorArea->Blt(&aLocalRect, theSurface, &aScreenRect, DDBLT_WAIT, &aBltFX);
		if (aResult != DD_OK)			
		{
			// Try to clip it now.  We don't ALWAYS want to clip it, though
			Rect aPrevRect = aClippedScreenRect;
			aClippedScreenRect = aClippedScreenRect.Intersection(aDestSurfaceRect);
			
			aSexyLocalRect.mX += (aClippedScreenRect.mX - aPrevRect.mX);
			aSexyLocalRect.mY += (aClippedScreenRect.mY - aPrevRect.mY);
			aSexyLocalRect.mWidth = aClippedScreenRect.mWidth;
			aSexyLocalRect.mHeight = aClippedScreenRect.mHeight;

			aLocalRect = aSexyLocalRect.ToRECT();
			aScreenRect = aClippedScreenRect.ToRECT();			

			aResult = mOldCursorArea->Blt(&aLocalRect, theSurface, &aScreenRect, DDBLT_WAIT, &aBltFX);			
			//DBG_ASSERT(aResult == DD_OK);
		}
		
		mHasOldCursorArea = aResult == DD_OK;

		// From mOldCursorArea to mNewCursorArea
		aResult = mNewCursorArea->Blt(&aLocalRect, mOldCursorArea, &aLocalRect, DDBLT_WAIT, &aBltFX);

		// Draw image to mNewCursorAreaImage
		Graphics aNewCursorAreaG(mNewCursorAreaImage);
		aNewCursorAreaG.DrawImage(mCursorImage,
			(mCursorWidth - mCursorImage->mWidth) / 2,
			(mCursorHeight - mCursorImage->mHeight) / 2);

		// From mNewCursorArea to theSurface
		aResult = theSurface->Blt(&aScreenRect, mNewCursorArea, &aLocalRect, DDBLT_WAIT, &aBltFX);
	}
	else
		mHasOldCursorArea = false;
}

void DDInterface::MoveCursorTo(LPDIRECTDRAWSURFACE theSurface, bool adjust, int theNewCursorX, int theNewCursorY)
{
	DBG_ASSERT(mHasOldCursorArea);	

	if ((mCursorImage != NULL) && (mPrimarySurface != NULL))
	{
		DDSURFACEDESC aDesc;
		ZeroMemory(&aDesc, sizeof(aDesc));
		aDesc.dwSize = sizeof(aDesc);
		aDesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		HRESULT aResult = mPrimarySurface->GetSurfaceDesc(&aDesc);
		Rect aDestSurfaceRect(0, 0, aDesc.dwWidth, aDesc.dwHeight);

		// OLD RECTANGLES
		Rect aSexyScreenRect(
			mCursorX - (mCursorWidth / 2),
			mCursorY - (mCursorHeight / 2),
			mCursorWidth,
			mCursorHeight);

		Rect aClippedScreenRect = aSexyScreenRect.Intersection(mPresentationRect);
		
		Rect aSexyLocalRect(
			aClippedScreenRect.mX - aSexyScreenRect.mX, 
			aClippedScreenRect.mY - aSexyScreenRect.mY, 
			aClippedScreenRect.mWidth, 
			aClippedScreenRect.mHeight);
		if (adjust)
		{
			POINT aPoint = {0, 0};		
			ClientToScreen(mHWnd, &aPoint);
			aClippedScreenRect.Offset(aPoint.x, aPoint.y);
			//aClippedScreenRect = aClippedScreenRect.Intersection(aDestSurfaceRect);	
		}

		aSexyLocalRect.mWidth = aClippedScreenRect.mWidth;
		aSexyLocalRect.mHeight = aClippedScreenRect.mHeight;
		
		RECT aLocalRect = aSexyLocalRect.ToRECT();
		RECT aScreenRect = aClippedScreenRect.ToRECT();

		// NEW RECTANGLES
		Rect aNewSexyScreenRect(
			theNewCursorX - (mCursorWidth / 2),
			theNewCursorY - (mCursorHeight / 2),
			mCursorWidth,
			mCursorHeight);
				
		Rect aNewClippedScreenRect = aNewSexyScreenRect.Intersection(mPresentationRect);
		Rect aNewSexyLocalRect(
			aNewClippedScreenRect.mX - aNewSexyScreenRect.mX, 
			aNewClippedScreenRect.mY - aNewSexyScreenRect.mY, 
			aNewClippedScreenRect.mWidth, 
			aNewClippedScreenRect.mHeight);
		if (adjust)
		{
			POINT aPoint = {0, 0};
			ClientToScreen(mHWnd, &aPoint);
			aNewClippedScreenRect.Offset(aPoint.x, aPoint.y);
			//aNewClippedScreenRect = aNewClippedScreenRect.Intersection(aDestSurfaceRect);
		}

		aNewSexyLocalRect.mWidth = aNewClippedScreenRect.mWidth;
		aNewSexyLocalRect.mHeight = aNewClippedScreenRect.mHeight;

		RECT aNewLocalRect = aNewSexyLocalRect.ToRECT();
		RECT aNewScreenRect = aNewClippedScreenRect.ToRECT();

		// Do drawing stuff now

		DDBLTFX aBltFX;
		ZeroMemory(&aBltFX, sizeof(aBltFX));
		aBltFX.dwSize = sizeof(aBltFX);

		// From theSurface to mNewCursorArea
		//  It may still have some of the old cursor on it though, since we haven't restored
		//  that area yet
		
		aResult = mNewCursorArea->Blt(&aNewLocalRect, theSurface, &aNewScreenRect, DDBLT_WAIT, &aBltFX);
		if (aResult != DD_OK)			
		{
			// Try to clip it now.  We don't ALWAYS want to clip it, though
			Rect aPrevRect = aNewClippedScreenRect;
			aNewClippedScreenRect = aNewClippedScreenRect.Intersection(aDestSurfaceRect);
			
			aNewSexyLocalRect.mX += (aNewClippedScreenRect.mX - aPrevRect.mX);
			aNewSexyLocalRect.mY += (aNewClippedScreenRect.mY - aPrevRect.mY);
			aNewSexyLocalRect.mWidth = aNewClippedScreenRect.mWidth;
			aNewSexyLocalRect.mHeight = aNewClippedScreenRect.mHeight;

			aNewLocalRect = aNewSexyLocalRect.ToRECT();
			aNewScreenRect = aNewClippedScreenRect.ToRECT();					

			aResult = mNewCursorArea->Blt(&aNewLocalRect, theSurface, &aNewScreenRect, DDBLT_WAIT, &aBltFX);			
			//DBG_ASSERT(aResult == DD_OK);
		}
		//DBG_ASSERT(aResult == DD_OK);

		// Figure out the overlapping area from the source
		Rect aCursorAreaRect(0, 0, mCursorWidth, mCursorHeight);
		Rect aSexyOrigSrcAreaRect(aCursorAreaRect);
		aSexyOrigSrcAreaRect.Offset(theNewCursorX - mCursorX, theNewCursorY - mCursorY);
		Rect aSexySrcAreaRect = aSexyOrigSrcAreaRect.Intersection(aCursorAreaRect);

		// Does the new cursor area overlap with the old one?
		if ((aSexySrcAreaRect.mWidth > 0) || (aSexySrcAreaRect.mHeight > 0))
		{
			Rect aSexyDestAreaRect(
				aSexySrcAreaRect.mX - aSexyOrigSrcAreaRect.mX,
				aSexySrcAreaRect.mY - aSexyOrigSrcAreaRect.mY, 
				aSexySrcAreaRect.mWidth, aSexySrcAreaRect.mHeight);
			
			RECT aSrcAreaRect = aSexySrcAreaRect.ToRECT();
			RECT aDestAreaRect = aSexyDestAreaRect.ToRECT();
			
			// Restore old area from new area
			//  This will give us our new pure OLD buffer
			mNewCursorArea->Blt(&aDestAreaRect, mOldCursorArea, &aSrcAreaRect, DDBLT_WAIT, &aBltFX);
			//DBG_ASSERT(aResult == DD_OK);
			
			// Draw offset image to mOldCursorAreaImage.  This is to avoid flicker
			Graphics aOldCursorAreaG(mOldCursorAreaImage);
			aOldCursorAreaG.DrawImage(mCursorImage, 
				theNewCursorX - mCursorX + (mCursorWidth - mCursorImage->mWidth)/2, 
				theNewCursorY - mCursorY + (mCursorHeight - mCursorImage->mHeight)/2);
		}
		
		// Restore the old cursor area
		aResult = theSurface->Blt(&aScreenRect, mOldCursorArea, &aLocalRect, DDBLT_WAIT, &aBltFX);
		//DBG_ASSERT(aResult == DD_OK);
		
		// The screen is now PURE and restored

		// Move the new cursor area to the old one, since this is what we will have to
		//  use to redraw the old area
		RECT aFullAreaRect = {0, 0, mCursorWidth, mCursorHeight};
		aResult = mOldCursorArea->Blt(&aFullAreaRect, mNewCursorArea, &aFullAreaRect, DDBLT_WAIT, &aBltFX);
		//DBG_ASSERT(aResult == DD_OK);

		// Draw image to mNewCursorAreaImage, preparing to draw it to the screen
		Graphics aNewCursorAreaG(mNewCursorAreaImage);
		aNewCursorAreaG.DrawImage(mCursorImage, 
			(mCursorWidth - mCursorImage->mWidth)/2, 
			(mCursorHeight - mCursorImage->mHeight)/2);

		// From mNewCursorArea to theSurface
		aResult = theSurface->Blt(&aNewScreenRect, mNewCursorArea, &aNewLocalRect, DDBLT_WAIT, &aBltFX);
		//DBG_ASSERT(aResult == DD_OK);

		// The cursor is now fully moved
		mCursorX = theNewCursorX;
		mCursorY = theNewCursorY;
	}
	else
		mHasOldCursorArea = false;
}

bool DDInterface::SetCursorImage(Image* theImage)
{
	AutoCrit anAutoCrit(mCritSect);
	
	if (mCursorImage != theImage)
	{
		// Wait until next Redraw or cursor move to draw new cursor
		mCursorImage = theImage;		
		return true;
	}
	else
		return false;
}

void DDInterface::SetCursorPos(int theCursorX, int theCursorY)
{
	mNextCursorX = theCursorX;
	mNextCursorY = theCursorY;

	if (mInRedraw)
		return;

	AutoCrit anAutoCrit(mCritSect);	
	
	if (mHasOldCursorArea)
	{
		MoveCursorTo(mPrimarySurface, true, theCursorX, theCursorY);
	}
	else
	{
		mCursorX = theCursorX;
		mCursorY = theCursorY;
		DrawCursorTo(mPrimarySurface, true);		
	}
}
