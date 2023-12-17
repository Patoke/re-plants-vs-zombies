#ifndef __GLINTERFACE_H__
#define __GLINTERFACE_H__

#include "Common.h"
#include "misc/CritSect.h"
#include "NativeDisplay.h"
#include "misc/Rect.h"
#include "misc/Ratio.h"

namespace Sexy
{

	class SexyAppBase;
	class DDImage;
	class Image;
	class MemoryImage;
	class D3DInterface;

	typedef std::set<DDImage*> DDImageSet;

	class GLInterface : public NativeDisplay
	{
	public:
		SexyAppBase* mApp;

		CritSect				mCritSect;
		bool					mInRedraw;
		int						mWidth;
		int						mHeight;
		Ratio					mAspect;
		int						mDesktopWidth;
		int						mDesktopHeight;
		Ratio					mDesktopAspect;
		bool					mIsWidescreen;
		int						mDisplayWidth;
		int						mDisplayHeight;
		Ratio					mDisplayAspect;

		Rect					mPresentationRect;
		int						mFullscreenBits;
		DWORD					mRefreshRate;
		DWORD					mMillisecondsPerFrame;
		int						mScanLineFailCount;

		int* mRedAddTable;
		int* mGreenAddTable;
		int* mBlueAddTable;

		ulong					mRedConvTable[256];
		ulong					mGreenConvTable[256];
		ulong					mBlueConvTable[256];

		bool					mInitialized;
		HWND					mHWnd;
		bool					mIsWindowed;
		DDImage*				mScreenImage;
		DDImageSet				mDDImageSet;
		bool					mVideoOnlyDraw;
		ulong					mInitCount;

		int						mCursorWidth;
		int						mCursorHeight;
		int						mNextCursorX;
		int						mNextCursorY;
		int						mCursorX;
		int						mCursorY;
		Image* mCursorImage;

		std::string				mErrorString;

	public:
		bool					CopyBitmap(LPDIRECTDRAWSURFACE theSurface, HBITMAP TheBitmap, int theX, int theY, unsigned int theWidth, unsigned int theHeight);
		ulong					GetColorRef(ulong theRGB);
		void					AddDDImage(DDImage* theDDImage);
		void					RemoveDDImage(DDImage* theDDImage);
		void					Remove3DData(MemoryImage* theImage); // for 3d texture cleanup

		void					Cleanup();

		void					RestoreOldCursorAreaFrom(LPDIRECTDRAWSURFACE theSurface, bool adjust);
		void					DrawCursorTo(LPDIRECTDRAWSURFACE theSurface, bool adjust);
		void					MoveCursorTo(LPDIRECTDRAWSURFACE theSurface, bool adjust, int theNewCursorX, int theNewCursorY);

		HRESULT					CreateSurface(DDSURFACEDESC2* theDesc, LPDIRECTDRAWSURFACE* theSurface, void*);
		void					ClearSurface(LPDIRECTDRAWSURFACE theSurface);

	public:
		GLInterface(SexyAppBase* theApp);
		virtual ~GLInterface();

		int						Init(HWND theWindow, bool IsWindowed);
	};

}

#endif //__GLINTERFACE_H__