#include "D3DInterface.h"
#include "DDInterface.h"
#include "Graphics.h"
#include "DirectXErrorString.h"
#include "SexyMatrix.h"
#include "SexyAppBase.h"
#include "TriVertex.h"
#include <assert.h>
#include <algorithm>

#pragma warning(disable:4244)

using namespace Sexy;

static int gMinTextureWidth;
static int gMinTextureHeight;
static int gMaxTextureWidth;
static int gMaxTextureHeight;
static int gMaxTextureAspectRatio;
static DWORD gSupportedPixelFormats;
static bool gTextureSizeMustBePow2;
static const int MAX_TEXTURE_SIZE = 1024;
static bool gLinearFilter = false;
std::string D3DInterface::mErrorString;
static const int gVertexType = D3DFVF_TLVERTEX;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
[[maybe_unused]]
static void DisplayError(HRESULT theError, const char *theMsg)
{
	static bool hadError = false;
	if (!hadError)
	{
		std::string aMsg;
		std::string anError = GetDirectXErrorString(theError);
		aMsg = theMsg;
		aMsg += ": ";
		aMsg += anError;

		hadError = true;
		int aResult = MessageBoxA(NULL,aMsg.c_str(),"Error",MB_ABORTRETRYIGNORE);
		if (aResult==IDABORT)
			exit(0);
		else if (aResult==IDRETRY)
			__debugbreak();
	}
}
*/

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DInterface::CheckDXError(HRESULT theError, const char *theMsg)
{
	if(FAILED(theError))
	{
		std::string aMsg;
		std::string anError = GetDirectXErrorString(theError);
		aMsg = theMsg;
		aMsg += ": ";
		aMsg += anError;
		mErrorString = aMsg;
		gSexyAppBase->RegistryWriteString("Test3D\\RuntimeError",aMsg);

	//	DisplayError(theError,theMsg);
		return true;
	}
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
D3DInterface::D3DInterface()
{
	mHWnd = NULL;
	mWidth = 640;
	mHeight = 480;
	mDD = NULL;
	mDDSDrawSurface = NULL;
	mZBuffer = NULL;

	mD3D = NULL;
	mD3DDevice = NULL;
	//mD3DViewport = NULL;
	mSceneBegun = false;
	mIsWindowed = true;

	gMinTextureWidth = 64;
	gMinTextureHeight = 64;
	gMaxTextureWidth = 64;
	gMaxTextureHeight = 64;
	gMaxTextureAspectRatio = 1;

/*
	//Test Transform
	SexyTransform2D aTrans;
	aTrans.Translate(-320,-240);
	aTrans.Rotate(45*3.14159f/180);
//	aTrans.Scale(1.3f,1.3f);
	aTrans.Translate(320,240);
	PushTransform(aTrans);
*/
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
D3DInterface::~D3DInterface()
{
	Cleanup();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::MakeDDPixelFormat(PixelFormat theFormatType, DDPIXELFORMAT* theFormat)
{
	ZeroMemory(theFormat,sizeof(DDPIXELFORMAT));
	theFormat->dwSize = sizeof(DDPIXELFORMAT);

	switch(theFormatType)
	{
		case PixelFormat_A8R8G8B8:
			theFormat->dwFlags = DDPF_ALPHAPIXELS | DDPF_RGB;
			theFormat->dwRGBBitCount = 32;
			theFormat->dwRGBAlphaBitMask	= 0xFF000000;
			theFormat->dwRBitMask			= 0x00FF0000;
			theFormat->dwGBitMask			= 0x0000FF00;
			theFormat->dwBBitMask			= 0x000000FF;
			break;


		case PixelFormat_A4R4G4B4:
			theFormat->dwFlags = DDPF_ALPHAPIXELS | DDPF_RGB;
			theFormat->dwRGBBitCount = 16;
			theFormat->dwRGBAlphaBitMask	= 0xF000;
			theFormat->dwRBitMask			= 0x0F00;
			theFormat->dwGBitMask			= 0x00F0;
			theFormat->dwBBitMask			= 0x000F;
			break;

		case PixelFormat_R5G6B5:
			theFormat->dwFlags = DDPF_RGB;
			theFormat->dwRGBBitCount = 16;
			theFormat->dwRBitMask			= 0xF800;
			theFormat->dwGBitMask			= 0x07E0;
			theFormat->dwBBitMask			= 0x001F;
			break;

		case PixelFormat_Palette8:
			theFormat->dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
			theFormat->dwRGBBitCount = 8;
			break;

		case PixelFormat_Unknown:
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
PixelFormat D3DInterface::GetDDPixelFormat(LPDDPIXELFORMAT theFormat) 
{
	if (theFormat->dwFlags						==	(DDPF_ALPHAPIXELS | DDPF_RGB)	&& 
		theFormat->dwRGBBitCount				==	32								&& 
		theFormat->dwRGBAlphaBitMask			==	0xFF000000						&&
		theFormat->dwRBitMask					==	0x00FF0000						&&
		theFormat->dwGBitMask					==	0x0000FF00						&&
		theFormat->dwBBitMask					==	0x000000FF)
	{
		return PixelFormat_A8R8G8B8;
	}

	if (theFormat->dwFlags						==	(DDPF_ALPHAPIXELS | DDPF_RGB)	&& 
		theFormat->dwRGBBitCount				==	16								&& 
		theFormat->dwRGBAlphaBitMask			==	0xF000							&&
		theFormat->dwRBitMask					==	0x0F00							&&
		theFormat->dwGBitMask					==	0x00F0							&&
		theFormat->dwBBitMask					==	0x000F)
	{
		return PixelFormat_A4R4G4B4;
	}

	if (theFormat->dwFlags						==	DDPF_RGB						&& 
		theFormat->dwRGBBitCount				==	16								&& 
		theFormat->dwRGBAlphaBitMask			==	0x0000							&&
		theFormat->dwRBitMask					==	0xF800							&&
		theFormat->dwGBitMask					==	0x07E0							&&
		theFormat->dwBBitMask					==	0x001F)
	{
		return PixelFormat_R5G6B5;
	}

	if (theFormat->dwFlags						== (DDPF_RGB | DDPF_PALETTEINDEXED8) &&
		theFormat->dwRGBBitCount				== 8)
	{
		return PixelFormat_Palette8;
	}

	return PixelFormat_Unknown;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
HRESULT CALLBACK D3DInterface::PixelFormatsCallback(LPDDPIXELFORMAT theFormat, LPVOID lpContext)
{
	(void)lpContext;
	gSupportedPixelFormats |= D3DInterface::GetDDPixelFormat(theFormat);
	
	return D3DENUMRET_OK; 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::UpdateViewport()
{
	HRESULT hr;
	RECT aRect;
	GetClientRect(mHWnd, &aRect);

	POINT aTopLeft = {aRect.left, aRect.top};
	POINT aBotRight = {aRect.right, aRect.bottom};
	::ClientToScreen(mHWnd, &aTopLeft);
	::ClientToScreen(mHWnd, &aBotRight);

	RECT aScreenRect = {aTopLeft.x, aTopLeft.y, aBotRight.x, aBotRight.y};

	D3DVIEWPORT7 &aD3DViewport = mD3DViewport;
	aD3DViewport.dwX = 0;
	aD3DViewport.dwY = 0;
	aD3DViewport.dwWidth = aScreenRect.right - aScreenRect.left;
	aD3DViewport.dwHeight = aScreenRect.bottom - aScreenRect.top;
	aD3DViewport.dvMinZ = 0; //-2048.0f;
	aD3DViewport.dvMaxZ = 1; //2048.0f;

	hr = mD3DDevice->SetViewport(&mD3DViewport);
	(void)hr; // unused
}


//-----------------------------------------------------------------------------
// Name: EnumZBufferCallback()
// Desc: Enumeration function to report valid pixel formats for z-buffers.
//-----------------------------------------------------------------------------
static HRESULT WINAPI EnumZBufferCallback( DDPIXELFORMAT* pddpf, VOID* pddpfDesired )
{
    if( pddpf->dwFlags == DDPF_ZBUFFER )
    {
        memcpy( pddpfDesired, pddpf, sizeof(DDPIXELFORMAT) );
		return D3DENUMRET_CANCEL;
    }

    return D3DENUMRET_OK;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DInterface::InitD3D()
{	
	if(CheckDXError(mDD->QueryInterface(IID_IDirect3D7, (LPVOID*) &mD3D),"QueryInterface IID_IDirect3D7"))
		return false;

	if(CheckDXError(mD3D->CreateDevice(IID_IDirect3DHALDevice, mDDSDrawSurface, &mD3DDevice),"CreateDevice IID_IDirect3DHALDevice"))
		return false;

	D3DDEVICEDESC7 aCaps;
	ZeroMemory(&aCaps,sizeof(aCaps));
	if(CheckDXError(mD3DDevice->GetCaps(&aCaps)))
		return false;

	gTextureSizeMustBePow2 = aCaps.dpcTriCaps.dwTextureCaps&D3DPTEXTURECAPS_NONPOW2CONDITIONAL?false:true;
	gMinTextureWidth = aCaps.dwMinTextureWidth;
	gMinTextureHeight = aCaps.dwMinTextureHeight;
	gMaxTextureWidth = aCaps.dwMaxTextureWidth;
	gMaxTextureHeight = aCaps.dwMaxTextureHeight;
	gMaxTextureAspectRatio = aCaps.dwMaxTextureAspectRatio;
	gLinearFilter = false;

	if (gMaxTextureWidth==0) // the card is not filling in these values so default them to something that will work
	{
		gMaxTextureWidth = 64;
		gMaxTextureHeight = 64;
		gMinTextureWidth = 64;
		gMinTextureHeight = 64;
		gMaxTextureAspectRatio = 1;
	}

	if (gMaxTextureWidth > MAX_TEXTURE_SIZE)
		gMaxTextureWidth = MAX_TEXTURE_SIZE;
	if (gMaxTextureHeight > MAX_TEXTURE_SIZE)
		gMaxTextureHeight = MAX_TEXTURE_SIZE;

	if (gMinTextureWidth < 1)
		gMinTextureWidth = 1;
	if (gMinTextureHeight < 1)
		gMinTextureWidth = 1;

	if (gMaxTextureAspectRatio==0)
		gMaxTextureAspectRatio = 65536;

	gSupportedPixelFormats = 0;
	mD3DDevice->EnumTextureFormats(PixelFormatsCallback,NULL);
/*	if (!(gSupportedPixelFormats & PixelFormat_A8R8G8B8))
	{
		mErrorString = "A8R8G8B8 texture format not supported.";
		return false;
	}*/

	if (!(aCaps.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE)) // need alpha in palettes
		gSupportedPixelFormats &= ~PixelFormat_Palette8;

	UpdateViewport();

	// Create ZBuffer
	DDPIXELFORMAT ddpfZBuffer;
	mD3D->EnumZBufferFormats( IID_IDirect3DHALDevice, EnumZBufferCallback, (VOID*)&ddpfZBuffer );

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize         = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags        = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT|DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | DDSCAPS_VIDEOMEMORY;
	ddsd.dwWidth        = mD3DViewport.dwWidth;
	ddsd.dwHeight       = mD3DViewport.dwHeight;
    memcpy( &ddsd.ddpfPixelFormat, &ddpfZBuffer, sizeof(DDPIXELFORMAT) );

	/*
	// Create z-buffer
    if( CheckDXError(mDD->CreateSurface( &ddsd, &mZBuffer, NULL ) ) )
		return false;

	// Attach the z-buffer to the back buffer.
    if( CheckDXError( mDDSDrawSurface->AddAttachedSurface( mZBuffer ) ) )
		return false;*/

	mD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET ,0xff000000, 1.0f, 0L);
	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DInterface::InitFromDDInterface(DDInterface *theInterface)
{
	mErrorString.erase();

	mDD = theInterface->mDD7;
	mHWnd = theInterface->mHWnd;
	mWidth = theInterface->mWidth;
	mHeight = theInterface->mHeight;

	if (CheckDXError(theInterface->mDrawSurface->QueryInterface(IID_IDirectDrawSurface7,(LPVOID*)&mDDSDrawSurface), "Query DrawSurface"))
		return false;


	mIsWindowed = theInterface->mIsWindowed;
	return InitD3D();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool gD3DInterfacePreDrawError = false;
bool D3DInterface::PreDraw()
{
	if (gSexyAppBase->mPhysMinimized)
		return false;

	if (!mSceneBegun)
	{
		HRESULT hr;


		if (!SUCCEEDED(mD3DDevice->SetRenderTarget(mDDSDrawSurface, 0))) // this happens when there's been a mode switch (this caused the nvidia screensaver bluescreen)
		{
			gD3DInterfacePreDrawError = true;
			return false;
		}
		else
			gD3DInterfacePreDrawError = false;

//		mD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET ,0xff000000, 1.0f, 0L);

		hr = mD3DDevice->BeginScene();
		
		// alphablend states 
		mD3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
		mD3DDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA); 
		mD3DDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA); 
		mD3DDevice->SetRenderState(D3DRENDERSTATE_LIGHTING , FALSE);

		// filter states 
		mD3DDevice->SetTextureStageState(0,D3DTSS_MINFILTER, D3DTFG_POINT); 
		mD3DDevice->SetTextureStageState(0,D3DTSS_MAGFILTER, D3DTFG_POINT); 
		mD3DDevice->SetTextureStageState(0,D3DTSS_MIPFILTER, D3DTFG_POINT); 
//		mD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2, D3DTA_CURRENT );
//		mD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_CURRENT );
//		mD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TEXTURE );
//		mD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
//		mD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP , D3DTOP_MODULATE  );
		mD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE  );
		mD3DDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP); 
		hr = mD3DDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP); 

		// Setup non-texture render states 				
		mD3DDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, FALSE); 
		mD3DDevice->SetRenderState(D3DRENDERSTATE_SPECULARENABLE, FALSE); 
		mD3DDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE); 
		mD3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE); 
		hr = mD3DDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
		(void)hr; // unused		

		mSceneBegun = true;
		gLinearFilter = false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static LPDIRECTDRAWSURFACE7 CreateTextureSurface(LPDIRECT3DDEVICE7 theDevice, LPDIRECTDRAW7 theDraw, int theWidth, int theHeight, PixelFormat theFormat)
{
	if (D3DInterface::CheckDXError(theDevice->SetTexture(0, NULL),"SetTexture NULL"))
		return NULL;

	DDSURFACEDESC2 aDesc;
	LPDIRECTDRAWSURFACE7 aSurface;

	ZeroMemory(&aDesc, sizeof(aDesc));
	aDesc.dwSize = sizeof(aDesc);
	
	aDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	
	aDesc.ddsCaps.dwCaps = DDSCAPS_TEXTURE;	
//	aDesc.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	aDesc.ddsCaps.dwCaps2 = DDSCAPS2_D3DTEXTUREMANAGE;

	aDesc.dwWidth = theWidth;
	aDesc.dwHeight = theHeight;	

	aDesc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	D3DInterface::MakeDDPixelFormat(theFormat, &aDesc.ddpfPixelFormat);
//	D3DXMakeDDPixelFormat(theFormat, &aDesc.ddpfPixelFormat);

	HRESULT hr = theDraw->CreateSurface(&aDesc, &aSurface, NULL);

	if (FAILED(hr))
	{
		std::string anError = GetDirectXErrorString(hr);
		return NULL;	
	}

	return aSurface;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CopyImageToTexture8888(void *theDest, DWORD theDestPitch, MemoryImage *theImage, int offx, int offy, int theWidth, int theHeight, bool rightPad)
{

	if (theImage->mColorTable == NULL)
	{
		DWORD *srcRow = theImage->GetBits() + offy * theImage->GetWidth() + offx;
		char *dstRow = (char*)theDest;

		for(int y=0; y<theHeight; y++)
		{
			DWORD *src = srcRow;
			DWORD *dst = (DWORD*)dstRow;
			for(int x=0; x<theWidth; x++)
			{
				*dst++ = *src++;
			}

			if (rightPad) 
				*dst = *(dst-1);

			srcRow += theImage->GetWidth();
			dstRow += theDestPitch;
		}
	}
	else // palette
	{
		uchar *srcRow = (uchar*)theImage->mColorIndices + offy * theImage->GetWidth() + offx;
		uchar *dstRow = (uchar*)theDest;
		DWORD *palette = theImage->mColorTable;

		for(int y=0; y<theHeight; y++)
		{
			uchar *src = srcRow;
			DWORD *dst = (DWORD*)dstRow;
			for(int x=0; x<theWidth; x++)
				*dst++ = palette[*src++];

			if (rightPad) 
				*dst = *(dst-1);

			srcRow += theImage->GetWidth();
			dstRow += theDestPitch;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CopyTexture8888ToImage(void *theDest, DWORD theDestPitch, MemoryImage *theImage, int offx, int offy, int theWidth, int theHeight)
{		
	char *srcRow = (char*)theDest;
	DWORD *dstRow = theImage->GetBits() + offy * theImage->GetWidth() + offx;

	for(int y=0; y<theHeight; y++)
	{
		DWORD *src = (DWORD*)srcRow;
		DWORD *dst = dstRow;
		
		for(int x=0; x<theWidth; x++)
			*dst++ = *src++;		

		dstRow += theImage->GetWidth();
		srcRow += theDestPitch;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CopyImageToTexture4444(void *theDest, DWORD theDestPitch, MemoryImage *theImage, int offx, int offy, int theWidth, int theHeight, bool rightPad)
{
	if (theImage->mColorTable == NULL)
	{
		DWORD *srcRow = theImage->GetBits() + offy * theImage->GetWidth() + offx;
		char *dstRow = (char*)theDest;

		for(int y=0; y<theHeight; y++)
		{
			DWORD *src = srcRow;
			ushort *dst = (ushort*)dstRow;
			for(int x=0; x<theWidth; x++)
			{
				DWORD aPixel = *src++;
				*dst++ = ((aPixel>>16)&0xF000) | ((aPixel>>12)&0x0F00) | ((aPixel>>8)&0x00F0) | ((aPixel>>4)&0x000F);
			}

			if (rightPad) 
				*dst = *(dst-1);

			srcRow += theImage->GetWidth();
			dstRow += theDestPitch;
		}
	}
	else // palette
	{
		uchar *srcRow = (uchar*)theImage->mColorIndices + offy * theImage->GetWidth() + offx;
		uchar *dstRow = (uchar*)theDest;
		DWORD *palette = theImage->mColorTable;

		for(int y=0; y<theHeight; y++)
		{
			uchar *src = srcRow;
			ushort *dst = (ushort*)dstRow;
			for(int x=0; x<theWidth; x++)
			{
				DWORD aPixel = palette[*src++];
				*dst++ = ((aPixel>>16)&0xF000) | ((aPixel>>12)&0x0F00) | ((aPixel>>8)&0x00F0) | ((aPixel>>4)&0x000F);
			}

			if (rightPad) 
				*dst = *(dst-1);

			srcRow += theImage->GetWidth();
			dstRow += theDestPitch;
		}

	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CopyTexture4444ToImage(void *theDest, DWORD theDestPitch, MemoryImage *theImage, int offx, int offy, int theWidth, int theHeight)
{		
	char *srcRow = (char*)theDest;
	DWORD *dstRow = theImage->GetBits() + offy * theImage->GetWidth() + offx;

	for(int y=0; y<theHeight; y++)
	{
		ushort *src = (ushort*)srcRow;
		DWORD *dst = dstRow;
		
		for(int x=0; x<theWidth; x++)
		{
			ushort aPixel = *src++;			
			*dst++ = 0xFF000000 | ((aPixel & 0xF000) << 16) | ((aPixel & 0x0F00) << 12) | ((aPixel & 0x00F0) << 8) | ((aPixel & 0x000F) << 4);
		}

		dstRow += theImage->GetWidth();
		srcRow += theDestPitch;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CopyImageToTexture565(void *theDest, DWORD theDestPitch, MemoryImage *theImage, int offx, int offy, int theWidth, int theHeight, bool rightPad)
{
	if (theImage->mColorTable == NULL)
	{
		DWORD *srcRow = theImage->GetBits() + offy * theImage->GetWidth() + offx;
		char *dstRow = (char*)theDest;

		for(int y=0; y<theHeight; y++)
		{
			DWORD *src = srcRow;
			ushort *dst = (ushort*)dstRow;
			for(int x=0; x<theWidth; x++)
			{
				DWORD aPixel = *src++;
				*dst++ = ((aPixel>>8)&0xF800) | ((aPixel>>5)&0x07E0) | ((aPixel>>3)&0x001F);
			}

			if (rightPad) 
				*dst = *(dst-1);

			srcRow += theImage->GetWidth();
			dstRow += theDestPitch;
		}
	}
	else // palette
	{
		uchar *srcRow = (uchar*)theImage->mColorIndices + offy * theImage->GetWidth() + offx;
		uchar *dstRow = (uchar*)theDest;
		DWORD *palette = theImage->mColorTable;

		for(int y=0; y<theHeight; y++)
		{
			uchar *src = srcRow;
			ushort *dst = (ushort*)dstRow;
			for(int x=0; x<theWidth; x++)
			{
				DWORD aPixel = palette[*src++];
				*dst++ = ((aPixel>>8)&0xF800) | ((aPixel>>5)&0x07E0) | ((aPixel>>3)&0x001F);
			}

			if (rightPad) 
				*dst = *(dst-1);

			srcRow += theImage->GetWidth();
			dstRow += theDestPitch;
		}

	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CopyTexture565ToImage(void *theDest, DWORD theDestPitch, MemoryImage *theImage, int offx, int offy, int theWidth, int theHeight)
{		
	char *srcRow = (char*)theDest;
	DWORD *dstRow = theImage->GetBits() + offy * theImage->GetWidth() + offx;

	for(int y=0; y<theHeight; y++)
	{
		ushort *src = (ushort*)srcRow;
		DWORD *dst = dstRow;
		
		for(int x=0; x<theWidth; x++)
		{
			ushort aPixel = *src++;			
			*dst++ = 0xFF000000 | ((aPixel & 0xF800) << 8) | ((aPixel & 0x07E0) << 5) | ((aPixel & 0x001F) << 3);
		}

		dstRow += theImage->GetWidth();
		srcRow += theDestPitch;
	}
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CopyImageToTexturePalette8(void *theDest, DWORD theDestPitch, MemoryImage *theImage, int offx, int offy, int theWidth, int theHeight, bool rightPad)
{
	uchar *srcRow = (uchar*)theImage->mColorIndices + offy * theImage->GetWidth() + offx;
	uchar *dstRow = (uchar*)theDest;

	for(int y=0; y<theHeight; y++)
	{
		uchar *src = srcRow;
		uchar *dst = dstRow;
		for(int x=0; x<theWidth; x++)
			*dst++ = *src++;

		if (rightPad) 
			*dst = *(dst-1);

		srcRow += theImage->GetWidth();
		dstRow += theDestPitch;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CopyTexturePalette8ToImage(void *theDest, DWORD theDestPitch, MemoryImage *theImage, int offx, int offy, int theWidth, int theHeight, LPDIRECTDRAWPALETTE thePalette)
{
	char *srcRow = (char*)theDest;
	DWORD *dstRow = theImage->GetBits() + offy * theImage->GetWidth() + offx;

	PALETTEENTRY aPaletteEntries[256];
	thePalette->GetEntries(0, 0, 256, aPaletteEntries); 

	for(int y=0; y<theHeight; y++)
	{
		uchar *src = (uchar*) srcRow;
		DWORD *dst = dstRow;
		
		for(int x=0; x<theWidth; x++)
		{
			DWORD aPixel = *((DWORD*)(aPaletteEntries+*src++));
			*dst++ = (aPixel&0xFF00FF00) | ((aPixel>>16)&0xFF) | ((aPixel<<16)&0xFF0000);
		}

		dstRow += theImage->GetWidth();
		srcRow += theDestPitch;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void CopyImageToTexture(LPDIRECTDRAWSURFACE7 theTexture, MemoryImage *theImage, int offx, int offy, int texWidth, int texHeight, PixelFormat theFormat)
{
	if (theTexture==NULL)
		return;

	DDSURFACEDESC2 aDesc;
	aDesc.dwSize = sizeof(aDesc);
	if (D3DInterface::CheckDXError(theTexture->Lock(NULL,&aDesc,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_WRITEONLY,NULL),"Lock Texture"))
		return;

	int aWidth = std::min(texWidth,(theImage->GetWidth()-offx));
	int aHeight = std::min(texHeight,(theImage->GetHeight()-offy));

	bool rightPad = aWidth<texWidth;
	bool bottomPad = aHeight<texHeight;
//	if(aWidth < texWidth || aHeight < texHeight)
//		memset(aDesc.lpSurface, 0, aDesc.lPitch*aDesc.dwHeight);

	if(aWidth>0 && aHeight>0)
	{
		switch (theFormat)
		{
			case PixelFormat_A8R8G8B8:	CopyImageToTexture8888(aDesc.lpSurface, aDesc.lPitch, theImage, offx, offy, aWidth, aHeight, rightPad); break;
			case PixelFormat_A4R4G4B4:	CopyImageToTexture4444(aDesc.lpSurface, aDesc.lPitch, theImage, offx, offy, aWidth, aHeight, rightPad); break;
			case PixelFormat_R5G6B5:	CopyImageToTexture565(aDesc.lpSurface, aDesc.lPitch, theImage, offx, offy, aWidth, aHeight, rightPad); break;
			case PixelFormat_Palette8:	CopyImageToTexturePalette8(aDesc.lpSurface, aDesc.lPitch, theImage, offx, offy, aWidth, aHeight, rightPad); break;
			case PixelFormat_Unknown: break;
		}

		if (bottomPad)
		{
			uchar *dstrow = ((uchar*)aDesc.lpSurface)+aDesc.lPitch*aHeight;
			memcpy(dstrow,dstrow-aDesc.lPitch,aDesc.lPitch);
		}
	}

	D3DInterface::CheckDXError(theTexture->Unlock(NULL),"Texture Unlock");
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static int GetClosestPowerOf2Above(int theNum)
{
	int aPower2 = 1;
	while (aPower2 < theNum)
		aPower2<<=1;

	return aPower2;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static bool IsPowerOf2(int theNum)
{
	int aNumBits = 0;
	while (theNum>0)
	{
		aNumBits += theNum&1;
		theNum >>= 1;
	}

	return aNumBits==1;
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void GetBestTextureDimensions(int &theWidth, int &theHeight, bool isEdge, bool usePow2, DWORD theImageFlags)
{
//	theImageFlags = D3DImageFlag_MinimizeNumSubdivisions;
	if (theImageFlags & D3DImageFlag_Use64By64Subdivisions)
	{
		theWidth = theHeight = 64;
		return;
	}

	static int aGoodTextureSize[MAX_TEXTURE_SIZE];
	static bool haveInited = false;
	if (!haveInited)
	{
		haveInited = true;
		int i;
		int aPow2 = 1;
		for (i=0; i<MAX_TEXTURE_SIZE; i++)
		{
			if (i > aPow2)
				aPow2 <<= 1;

			int aGoodValue = aPow2;
			if ((aGoodValue - i ) > 64)
			{
				aGoodValue >>= 1;
				while (true)
				{
					int aLeftOver = i % aGoodValue;
					if (aLeftOver<64 || IsPowerOf2(aLeftOver))
						break;

					aGoodValue >>= 1;
				}
			}
			aGoodTextureSize[i] = aGoodValue;
		}
	}

	int aWidth = theWidth;
	int aHeight = theHeight;

	if (usePow2)
	{
		if (isEdge || (theImageFlags & D3DImageFlag_MinimizeNumSubdivisions))
		{
			aWidth = aWidth >= gMaxTextureWidth ? gMaxTextureWidth : GetClosestPowerOf2Above(aWidth);
			aHeight = aHeight >= gMaxTextureHeight ? gMaxTextureHeight : GetClosestPowerOf2Above(aHeight);
		}
		else
		{
			aWidth = aWidth >= gMaxTextureWidth ? gMaxTextureWidth : aGoodTextureSize[aWidth];
			aHeight = aHeight >= gMaxTextureHeight ? gMaxTextureHeight : aGoodTextureSize[aHeight];
		}
	}

	if (aWidth < gMinTextureWidth)
		aWidth = gMinTextureWidth;

	if (aHeight < gMinTextureHeight)
		aHeight = gMinTextureHeight;

	if (aWidth > aHeight)
	{
		while (aWidth > gMaxTextureAspectRatio*aHeight)
			aHeight <<= 1;
	}
	else if (aHeight > aWidth)
	{
		while (aHeight > gMaxTextureAspectRatio*aWidth)
			aWidth <<= 1;
	}

	theWidth = aWidth;
	theHeight = aHeight;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
TextureData::TextureData()
{
	mWidth = 0;
	mHeight = 0;
	mTexVecWidth = 0;
	mTexVecHeight = 0;
	mBitsChangedCount = 0;
	mTexMemSize = 0;
	mTexPieceWidth = 64;
	mTexPieceHeight = 64;

	mPalette = NULL;
	mPixelFormat = PixelFormat_Unknown;
	mImageFlags = 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
TextureData::~TextureData()
{
	ReleaseTextures();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TextureData::ReleaseTextures()
{
	for(int i=0; i<(int)mTextures.size(); i++)
	{
		LPDIRECTDRAWSURFACE7 aSurface = mTextures[i].mTexture;
		if (aSurface!=NULL)
			aSurface->Release();
	}

	mTextures.clear();

	mTexMemSize = 0;

	if (mPalette!=NULL)
	{
		mPalette->Release();
		mPalette = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TextureData::CreateTextureDimensions(MemoryImage *theImage)
{
	int aWidth = theImage->GetWidth();
	int aHeight = theImage->GetHeight();
	int i;
/**/
	// Calculate inner piece sizes
	mTexPieceWidth = aWidth;
	mTexPieceHeight = aHeight;
	bool usePow2 = true; //gTextureSizeMustBePow2 || mPixelFormat==PixelFormat_Palette8;
	GetBestTextureDimensions(mTexPieceWidth, mTexPieceHeight,false,usePow2,mImageFlags);

	// Calculate right boundary piece sizes
	int aRightWidth = aWidth%mTexPieceWidth;
	int aRightHeight = mTexPieceHeight;
	if (aRightWidth > 0)
		GetBestTextureDimensions(aRightWidth, aRightHeight,true,usePow2,mImageFlags);
	else
		aRightWidth = mTexPieceWidth;

	// Calculate bottom boundary piece sizes
	int aBottomWidth = mTexPieceWidth;
	int aBottomHeight = aHeight%mTexPieceHeight;
	if (aBottomHeight > 0)
		GetBestTextureDimensions(aBottomWidth, aBottomHeight,true,usePow2,mImageFlags);
	else
		aBottomHeight = mTexPieceHeight;

	// Calculate corner piece size
	int aCornerWidth = aRightWidth;
	int aCornerHeight = aBottomHeight;
	GetBestTextureDimensions(aCornerWidth, aCornerHeight,true,usePow2,mImageFlags);
/**/

//	mTexPieceWidth = 64;
//	mTexPieceHeight = 64;


	// Allocate texture array
	mTexVecWidth = (aWidth + mTexPieceWidth - 1)/mTexPieceWidth;
	mTexVecHeight = (aHeight + mTexPieceHeight - 1)/mTexPieceHeight;
	mTextures.resize(mTexVecWidth*mTexVecHeight);
	
	// Assign inner pieces
	for(i=0; i<(int)mTextures.size(); i++)
	{
		TextureDataPiece &aPiece = mTextures[i];
		aPiece.mTexture = NULL;
		aPiece.mWidth = mTexPieceWidth;
		aPiece.mHeight = mTexPieceHeight;
	}

	// Assign right pieces
/**/
	for(i=mTexVecWidth-1; i<(int)mTextures.size(); i+=mTexVecWidth)
	{
		TextureDataPiece &aPiece = mTextures[i];
		aPiece.mWidth = aRightWidth;
		aPiece.mHeight = aRightHeight;
	}

	// Assign bottom pieces
	for(i=mTexVecWidth*(mTexVecHeight-1); i<(int)mTextures.size(); i++)
	{
		TextureDataPiece &aPiece = mTextures[i];
		aPiece.mWidth = aBottomWidth;
		aPiece.mHeight = aBottomHeight;
	}

	// Assign corner piece
	mTextures.back().mWidth = aCornerWidth;
	mTextures.back().mHeight = aCornerHeight;
/**/

	mMaxTotalU = aWidth/(float)mTexPieceWidth;
	mMaxTotalV = aHeight/(float)mTexPieceHeight;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TextureData::CreateTextures(MemoryImage *theImage, LPDIRECT3DDEVICE7 theDevice, LPDIRECTDRAW7 theDraw)
{
	theImage->DeleteSWBuffers(); // don't need these buffers for 3d drawing

	// Choose appropriate pixel format
	PixelFormat aFormat = PixelFormat_A8R8G8B8;
	//theImage->mD3DFlags = D3DImageFlag_UseA4R4G4B4;

	theImage->CommitBits();
	if (!theImage->mHasAlpha && !theImage->mHasTrans && (gSupportedPixelFormats & PixelFormat_R5G6B5))
	{
		if (!(theImage->mD3DFlags & D3DImageFlag_UseA8R8G8B8))
			aFormat = PixelFormat_R5G6B5;
	}

	LPDIRECTDRAWPALETTE aDDPalette = NULL;
	if (theImage->mColorIndices != NULL && (gSupportedPixelFormats & PixelFormat_Palette8))
	{
		PALETTEENTRY aPalette[256];
		for (int i=0; i<256; i++)
		{
			DWORD aPixel = theImage->mColorTable[i];
			*(DWORD*)(aPalette+i) = (aPixel&0xFF00FF00) | ((aPixel>>16)&0xFF) | ((aPixel<<16)&0xFF0000);
		}
		HRESULT aResult = theDraw->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALPHA | DDPCAPS_ALLOW256,aPalette, &aDDPalette, NULL);
		if (SUCCEEDED(aResult))
			aFormat = PixelFormat_Palette8;
		else
		{
			std::string anError = GetDirectXErrorString(aResult);
			gSupportedPixelFormats &= ~PixelFormat_Palette8;
		}
	}

	if ((theImage->mD3DFlags & D3DImageFlag_UseA4R4G4B4) && aFormat==PixelFormat_A8R8G8B8 && (gSupportedPixelFormats & PixelFormat_A4R4G4B4))
		aFormat = PixelFormat_A4R4G4B4;

	if (aFormat==PixelFormat_A8R8G8B8 && !(gSupportedPixelFormats & PixelFormat_A8R8G8B8))
		aFormat = PixelFormat_A4R4G4B4;


	// Release texture if image size has changed
	bool createTextures = false;
	if (mWidth!=theImage->mWidth || mHeight!=theImage->mHeight || aFormat!=mPixelFormat || theImage->mD3DFlags!=mImageFlags)
	{
		ReleaseTextures();

		mPixelFormat = aFormat;
		mImageFlags = theImage->mD3DFlags;
		CreateTextureDimensions(theImage);
		createTextures = true;
	}

	mPalette =  aDDPalette;

	int i,x,y;

	int aHeight = theImage->GetHeight();
	int aWidth = theImage->GetWidth();

	if (mPalette!=NULL)
		mTexMemSize += 256*4;

	int aFormatSize = 4;
	if (aFormat==PixelFormat_Palette8)
		aFormatSize = 1;
	else if (aFormat==PixelFormat_R5G6B5)
		aFormatSize = 2;
	else if (aFormat==PixelFormat_A4R4G4B4)
		aFormatSize = 2;

	i=0;
	for(y=0; y<aHeight; y+=mTexPieceHeight)
	{
		for(x=0; x<aWidth; x+=mTexPieceWidth, i++)
		{
			TextureDataPiece &aPiece = mTextures[i];
			if (createTextures)
			{
				aPiece.mTexture = CreateTextureSurface(theDevice, theDraw, aPiece.mWidth, aPiece.mHeight, aFormat);
				if (aPiece.mTexture==NULL) // create texture failure
				{
					mPixelFormat = PixelFormat_Unknown;
					return;
				}

				if (mPalette!=NULL)
					aPiece.mTexture->SetPalette(mPalette);
					
				mTexMemSize += aPiece.mWidth*aPiece.mHeight*aFormatSize;
			}

			CopyImageToTexture(aPiece.mTexture,theImage,x,y,aPiece.mWidth,aPiece.mHeight,aFormat);
		}
	}

	mWidth = theImage->mWidth;
	mHeight = theImage->mHeight;
	mBitsChangedCount = theImage->mBitsChangedCount;
	mPixelFormat = aFormat;
}
	
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TextureData::CheckCreateTextures(MemoryImage *theImage, LPDIRECT3DDEVICE7 theDevice, LPDIRECTDRAW7 theDraw)
{
	if(mPixelFormat==PixelFormat_Unknown || theImage->mWidth != mWidth || theImage->mHeight != mHeight || theImage->mBitsChangedCount != mBitsChangedCount || theImage->mD3DFlags != mImageFlags)
		CreateTextures(theImage, theDevice, theDraw);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
LPDIRECTDRAWSURFACE7 TextureData::GetTexture(int x, int y, int &width, int &height, float &u1, float &v1, float &u2, float &v2)
{
	int tx = x/mTexPieceWidth;
	int ty = y/mTexPieceHeight;

	TextureDataPiece &aPiece = mTextures[ty*mTexVecWidth + tx];

	int left = x%mTexPieceWidth;
	int top = y%mTexPieceHeight;
	int right = left+width;
	int bottom = top+height;

	if(right > aPiece.mWidth)
		right = aPiece.mWidth;

	if(bottom > aPiece.mHeight)
		bottom = aPiece.mHeight;

	width = right-left;
	height = bottom-top;

	u1 = (float)left/aPiece.mWidth;
	v1 = (float)top/aPiece.mHeight;
	u2 = (float)right/aPiece.mWidth;
	v2 = (float)bottom/aPiece.mHeight;

	return aPiece.mTexture;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
LPDIRECTDRAWSURFACE7 TextureData::GetTextureF(float x, float y, float &width, float &height, float &u1, float &v1, float &u2, float &v2)
{
	int tx = x/mTexPieceWidth;
	int ty = y/mTexPieceHeight;

	TextureDataPiece &aPiece = mTextures[ty*mTexVecWidth + tx];

	float left = x - tx*mTexPieceWidth;
	float top = y - ty*mTexPieceHeight;
	float right = left+width;
	float bottom = top+height;

	if(right > aPiece.mWidth)
		right = aPiece.mWidth;

	if(bottom > aPiece.mHeight)
		bottom = aPiece.mHeight;

	width = right-left;
	height = bottom-top;

	u1 = (float)left/aPiece.mWidth;
	v1 = (float)top/aPiece.mHeight;
	u2 = (float)right/aPiece.mWidth;
	v2 = (float)bottom/aPiece.mHeight;

	return aPiece.mTexture;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void SetLinearFilter(LPDIRECT3DDEVICE7 theDevice, bool linear)
{
	if (gLinearFilter != linear)
	{
		D3DTEXTUREMAGFILTER aFilter = linear ? D3DTFG_LINEAR : D3DTFG_POINT;		

		const char *aDebugContext = linear ? "SetTextureStageState LINEAR" : "SetTextureStageState Point";

		D3DInterface::CheckDXError(theDevice->SetTextureStageState(0,D3DTSS_MINFILTER, aFilter),aDebugContext);
		D3DInterface::CheckDXError(theDevice->SetTextureStageState(0,D3DTSS_MAGFILTER, aFilter),aDebugContext);
		D3DInterface::CheckDXError(theDevice->SetTextureStageState(0,D3DTSS_MIPFILTER, aFilter),aDebugContext);

		gLinearFilter = linear;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TextureData::Blt(LPDIRECT3DDEVICE7 theDevice, float theX, float theY, const Rect& theSrcRect, const Color& theColor)
{
	int srcLeft = theSrcRect.mX;
	int srcTop = theSrcRect.mY;
	int srcRight = srcLeft + theSrcRect.mWidth;
	int srcBottom = srcTop + theSrcRect.mHeight;
	int srcX, srcY;
	float dstX, dstY;
	int aWidth,aHeight;
	float u1,v1,u2,v2;

	srcY = srcTop;
	dstY = theY;

	DWORD aColor = RGBA_MAKE(theColor.mRed, theColor.mGreen, theColor.mBlue, theColor.mAlpha);			

	if ((srcLeft >= srcRight) || (srcTop >= srcBottom))
		return;

	while(srcY < srcBottom)
	{
		srcX = srcLeft;
		dstX = theX;
		while(srcX < srcRight)
		{
			aWidth = srcRight-srcX;
			aHeight = srcBottom-srcY;
			LPDIRECTDRAWSURFACE7 aTexture = GetTexture(srcX, srcY, aWidth, aHeight, u1, v1, u2, v2);

			float x = dstX - 0.5f;
			float y = dstY - 0.5f;

			D3DTLVERTEX aVertex[4] = 
			{
				{ {x},				{y},			{0},{1},{aColor},{0},{u1},{v1} },
				{ {x},				{y+aHeight},	{0},{1},{aColor},{0},{u1},{v2} },
				{ {x+aWidth},		{y},			{0},{1},{aColor},{0},{u2},{v1} },
				{ {x+aWidth},		{y+aHeight},	{0},{1},{aColor},{0},{u2},{v2} }
			};

			
			D3DInterface::CheckDXError(theDevice->SetTexture(0, aTexture),"SetTexture gTexture");			
			D3DInterface::CheckDXError(theDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, gVertexType, aVertex, 4, 0),"DrawPrimitive (Tri) 1");

			srcX += aWidth;
			dstX += aWidth;
		}

		srcY += aHeight;
		dstY += aHeight;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//typedef std::vector<D3DTLVERTEX> VertexList;
struct VertexList
{
	enum { MAX_STACK_VERTS = 100 };
	D3DTLVERTEX mStackVerts[MAX_STACK_VERTS];
	D3DTLVERTEX *mVerts;
	int mSize;
	int mCapacity;

	typedef int size_type;

	VertexList() : mVerts(mStackVerts), mSize(0), mCapacity(MAX_STACK_VERTS) { }
	VertexList(const VertexList &theList) : mVerts(mStackVerts), mSize(theList.mSize), mCapacity(MAX_STACK_VERTS)
	{ 
		reserve(mSize);
		memcpy(mVerts,theList.mVerts,mSize*sizeof(mVerts[0]));
	}
	
	~VertexList() 
	{ 
		if (mVerts != mStackVerts)
			delete mVerts; 
	}

	void reserve(int theCapacity)
	{
		if (mCapacity < theCapacity)
		{
			mCapacity = theCapacity;
			D3DTLVERTEX *aNewList = new D3DTLVERTEX[theCapacity];
			memcpy(aNewList,mVerts,mSize*sizeof(mVerts[0]));
			if (mVerts != mStackVerts)
				delete mVerts;

			mVerts = aNewList;
		}
	}

	void push_back(const D3DTLVERTEX &theVert) 
	{ 
		if (mSize==mCapacity)
			reserve(mCapacity*2);
			
		mVerts[mSize++] = theVert; 
	}
	
	void operator=(const VertexList &theList) 
	{ 
		reserve(theList.mSize);
		mSize = theList.mSize; 
		memcpy(mVerts,theList.mVerts,mSize*sizeof(mVerts[0]));
	}
	

	D3DTLVERTEX& operator[](int thePos) 
	{ 
		return mVerts[thePos]; 
	}

	int size() { return mSize; }
	void clear() { mSize = 0; }
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static inline float GetCoord(const D3DTLVERTEX &theVertex, int theCoord)
{
	switch (theCoord)
	{
		case 0: return theVertex.sx;
		case 1: return theVertex.sy;
		case 2: return theVertex.sz;
		case 3: return theVertex.tu;
		case 4: return theVertex.tv;
		default: return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static inline D3DTLVERTEX Interpolate(const D3DTLVERTEX &v1, const D3DTLVERTEX &v2, float t)
{
	D3DTLVERTEX aVertex = v1;
	aVertex.sx = v1.sx + t*(v2.sx-v1.sx);
	aVertex.sy = v1.sy + t*(v2.sy-v1.sy);
	aVertex.tu = v1.tu + t*(v2.tu-v1.tu);
	aVertex.tv = v1.tv + t*(v2.tv-v1.tv);
	if (v1.color!=v2.color)
	{
		int r = RGBA_GETRED(v1.color) + t*(RGBA_GETRED(v2.color) - RGBA_GETRED(v1.color));
		int g = RGBA_GETGREEN(v1.color) + t*(RGBA_GETGREEN(v2.color) - RGBA_GETGREEN(v1.color));
		int b = RGBA_GETBLUE(v1.color) + t*(RGBA_GETBLUE(v2.color) - RGBA_GETBLUE(v1.color));
		int a = RGBA_GETALPHA(v1.color) + t*(RGBA_GETALPHA(v2.color) - RGBA_GETALPHA(v1.color));
		aVertex.color = RGBA_MAKE(r,g,b,a);
	}
	
	return aVertex;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
template<class Pred>
struct PointClipper
{
	Pred mPred;

	void ClipPoint(int n, float clipVal, const D3DTLVERTEX &v1, const D3DTLVERTEX &v2, VertexList &out);
	void ClipPoints(int n, float clipVal, VertexList &in, VertexList &out);
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
template<class Pred>
void PointClipper<Pred>::ClipPoint(int n, float clipVal, const D3DTLVERTEX &v1, const D3DTLVERTEX &v2, VertexList &out)
{
	if (!mPred(GetCoord(v1,n), clipVal))
	{
		if (!mPred(GetCoord(v2,n), clipVal)) // both inside
			out.push_back(v2);
		else // inside -> outside
		{
			float t = (clipVal - GetCoord(v1,n))/(GetCoord(v2,n)-GetCoord(v1,n));
			out.push_back(Interpolate(v1,v2,t));
		}
	}
	else
	{
		if (!mPred(GetCoord(v2,n), clipVal)) // outside -> inside
		{
			float t = (clipVal - GetCoord(v1, n))/(GetCoord(v2,n)-GetCoord(v1,n));
			out.push_back(Interpolate(v1,v2,t));
			out.push_back(v2);
		}
//			else // outside -> outside
	}
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
template<class Pred>
void PointClipper<Pred>::ClipPoints(int n, float clipVal, VertexList &in, VertexList &out)
{
	if(in.size()<2)
		return;

	ClipPoint(n,clipVal,in[in.size()-1],in[0],out);
	for(VertexList::size_type i=0; i<in.size()-1; i++)
		ClipPoint(n,clipVal,in[i],in[i+1],out);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void DrawPolyClipped(LPDIRECT3DDEVICE7 theDevice, const Rect *theClipRect, const VertexList &theList)
{
	VertexList l1, l2;
	l1 = theList;

	int left = theClipRect->mX;
	int right = left + theClipRect->mWidth;
	int top = theClipRect->mY;
	int bottom = top + theClipRect->mHeight;

	VertexList *in = &l1, *out = &l2;
	PointClipper<std::less<float> > aLessClipper;
	PointClipper<std::greater_equal<float> > aGreaterClipper;

	aLessClipper.ClipPoints(0,left,*in,*out); std::swap(in,out); out->clear();
	aLessClipper.ClipPoints(1,top,*in,*out); std::swap(in,out); out->clear();
	aGreaterClipper.ClipPoints(0,right,*in,*out); std::swap(in,out); out->clear();
	aGreaterClipper.ClipPoints(1,bottom,*in,*out); 

	VertexList &aList = *out;
	
	if (aList.size() >= 3)
		D3DInterface::CheckDXError(theDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, gVertexType, &aList[0], aList.size(), 0),"DrawPrimitive (Tri) 2");
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void DoPolyTextureClip(VertexList &theList)
{
	VertexList l2;

	float left = 0;
	float right = 1;
	float top = 0;
	float bottom = 1;

	VertexList *in = &theList, *out = &l2;
	PointClipper<std::less<float> > aLessClipper;
	PointClipper<std::greater_equal<float> > aGreaterClipper;

	aLessClipper.ClipPoints(3,left,*in,*out); std::swap(in,out); out->clear();
	aLessClipper.ClipPoints(4,top,*in,*out); std::swap(in,out); out->clear();
	aGreaterClipper.ClipPoints(3,right,*in,*out); std::swap(in,out); out->clear();
	aGreaterClipper.ClipPoints(4,bottom,*in,*out); 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TextureData::BltTransformed(LPDIRECT3DDEVICE7 theDevice, const SexyMatrix3 &theTrans, const Rect& theSrcRect, const Color& theColor, const Rect *theClipRect, float theX, float theY, bool center)
{
	int srcLeft = theSrcRect.mX;
	int srcTop = theSrcRect.mY;
	int srcRight = srcLeft + theSrcRect.mWidth;
	int srcBottom = srcTop + theSrcRect.mHeight;
	int srcX, srcY;
	float dstX, dstY;
	int aWidth;
	int aHeight;
	float u1,v1,u2,v2;
	float startx = 0, starty = 0;
	float pixelcorrect = 0.5f;

	if (center)
	{
		startx = -theSrcRect.mWidth/2.0f;
		starty = -theSrcRect.mHeight/2.0f;
		pixelcorrect = 0.0f;
	}			

	srcY = srcTop;
	dstY = starty;

	DWORD aColor = RGBA_MAKE(theColor.mRed, theColor.mGreen, theColor.mBlue, theColor.mAlpha);			

	if ((srcLeft >= srcRight) || (srcTop >= srcBottom))
		return;

	while(srcY < srcBottom)
	{
		srcX = srcLeft;
		dstX = startx;
		while(srcX < srcRight)
		{
			aWidth = srcRight-srcX;
			aHeight = srcBottom-srcY;
			LPDIRECTDRAWSURFACE7 aTexture = GetTexture(srcX, srcY, aWidth, aHeight, u1, v1, u2, v2);

			float x = dstX; // - 0.5f;
			float y = dstY; // - 0.5f;

			SexyVector2 p[4] = { SexyVector2(x, y), SexyVector2(x,y+aHeight), SexyVector2(x+aWidth, y) , SexyVector2(x+aWidth, y+aHeight) };
			SexyVector2 tp[4];

			int i;
			for (i=0; i<4; i++)
			{
				tp[i] = theTrans*p[i];
				tp[i].x -= pixelcorrect - theX;
				tp[i].y -= pixelcorrect - theY;
			}

			bool clipped = false;
			if (theClipRect != NULL)
			{
				int left = theClipRect->mX;
				int right = left + theClipRect->mWidth;
				int top = theClipRect->mY;
				int bottom = top + theClipRect->mHeight;
				for (i=0; i<4; i++)
				{
					if (tp[i].x<left || tp[i].x>=right || tp[i].y<top || tp[i].y>=bottom)
					{
						clipped = true;
						break;
					}
				}
			}

			D3DTLVERTEX aVertex[4] = 
			{
				{ {tp[0].x},{tp[0].y},{0},{1},{aColor},{0},{u1},{v1} },
				{ {tp[1].x},{tp[1].y},{0},{1},{aColor},{0},{u1},{v2} },
				{ {tp[2].x},{tp[2].y},{0},{1},{aColor},{0},{u2},{v1} },
				{ {tp[3].x},{tp[3].y},{0},{1},{aColor},{0},{u2},{v2} }
			};

			D3DInterface::CheckDXError(theDevice->SetTexture(0, aTexture),"SetTexture gTexture");

			if (!clipped)
				D3DInterface::CheckDXError(theDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, gVertexType, aVertex, 4, 0),"DrawPrimitive (Tri) 3");
			else
			{
				VertexList aList;
				aList.push_back(aVertex[0]);
				aList.push_back(aVertex[1]);
				aList.push_back(aVertex[3]);
				aList.push_back(aVertex[2]);

				DrawPolyClipped(theDevice, theClipRect, aList);
//				DrawPolyClipped(theDevice, theClipRect, aVertex+1, 3);
			}

//			D3DInterface::CheckDXError(theDevice->SetTexture(0, NULL),"SetTexture NULL");

			srcX += aWidth;
			dstX += aWidth;
		}

		srcY += aHeight;
		dstY += aHeight;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define GetColorFromTriVertex(theVertex, theColor) (theVertex.color?theVertex.color:theColor)

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TextureData::BltTriangles(LPDIRECT3DDEVICE7 theDevice, const TriVertex theVertices[][3], int theNumTriangles, DWORD theColor, float tx, float ty)
{	
	if ((mMaxTotalU <= 1.0) && (mMaxTotalV <= 1.0))
	{
		D3DInterface::CheckDXError(theDevice->SetTexture(0, mTextures[0].mTexture),"SetTexture gTexture");

		D3DTLVERTEX aVertexCache[100][3];
		int aVertexCacheNum = 0;

		for (int aTriangleNum = 0; aTriangleNum < theNumTriangles; aTriangleNum++)
		{
			TriVertex* aTriVerts = (TriVertex*) theVertices[aTriangleNum];
			D3DTLVERTEX* aD3DVertex = (D3DTLVERTEX*) aVertexCache[aVertexCacheNum++];

			aD3DVertex[0].sx = aTriVerts[0].x + tx;
			aD3DVertex[0].sy = aTriVerts[0].y + ty;
			aD3DVertex[0].sz = 0;
			aD3DVertex[0].rhw = 1;
			aD3DVertex[0].color = GetColorFromTriVertex(aTriVerts[0],theColor);
			aD3DVertex[0].specular = 0;
			aD3DVertex[0].tu = aTriVerts[0].u * mMaxTotalU;
			aD3DVertex[0].tv = aTriVerts[0].v * mMaxTotalV;

			aD3DVertex[1].sx = aTriVerts[1].x + tx;
			aD3DVertex[1].sy = aTriVerts[1].y + ty;
			aD3DVertex[1].sz = 0;
			aD3DVertex[1].rhw = 1;
			aD3DVertex[1].color = GetColorFromTriVertex(aTriVerts[1],theColor);
			aD3DVertex[1].specular = 0;
			aD3DVertex[1].tu = aTriVerts[1].u * mMaxTotalU;
			aD3DVertex[1].tv = aTriVerts[1].v * mMaxTotalV;

			aD3DVertex[2].sx = aTriVerts[2].x + tx;
			aD3DVertex[2].sy = aTriVerts[2].y + ty;
			aD3DVertex[2].sz = 0;
			aD3DVertex[2].rhw = 1;
			aD3DVertex[2].color = GetColorFromTriVertex(aTriVerts[2],theColor);
			aD3DVertex[2].specular = 0;
			aD3DVertex[2].tu = aTriVerts[2].u * mMaxTotalU;
			aD3DVertex[2].tv = aTriVerts[2].v * mMaxTotalV;
			
			if ((aVertexCacheNum == 100) || (aTriangleNum == theNumTriangles - 1))
			{
				// Flush the triangles now
				D3DInterface::CheckDXError(theDevice->DrawPrimitive(D3DPT_TRIANGLELIST, gVertexType, aVertexCache, aVertexCacheNum*3, 0),"DrawPrimitive (TriList)");				
				aVertexCacheNum = 0;
			}
		}
	}
	else
	{
		for (int aTriangleNum = 0; aTriangleNum < theNumTriangles; aTriangleNum++)
		{
			TriVertex* aTriVerts = (TriVertex*) theVertices[aTriangleNum];

			D3DTLVERTEX aVertex[3] = 
			{
				{ {aTriVerts[0].x + tx},{aTriVerts[0].y + ty},	{0},{1},{GetColorFromTriVertex(aTriVerts[0],theColor)},	{0},{aTriVerts[0].u*mMaxTotalU},{aTriVerts[0].v*mMaxTotalV} },
				{ {aTriVerts[1].x + tx},{aTriVerts[1].y + ty},	{0},{1},{GetColorFromTriVertex(aTriVerts[1],theColor)},	{0},{aTriVerts[1].u*mMaxTotalU},{aTriVerts[1].v*mMaxTotalV} },
				{ {aTriVerts[2].x + tx},{aTriVerts[2].y + ty},	{0},{1},{GetColorFromTriVertex(aTriVerts[2],theColor)},	{0},{aTriVerts[2].u*mMaxTotalU},{aTriVerts[2].v*mMaxTotalV} }
			};

			float aMinU = mMaxTotalU, aMinV = mMaxTotalV;
			float aMaxU = 0, aMaxV = 0;

			int i,j,k;
			for (i=0; i<3; i++)
			{
				if(aVertex[i].tu < aMinU)
					aMinU = aVertex[i].tu;

				if(aVertex[i].tv < aMinV)
					aMinV = aVertex[i].tv;

				if(aVertex[i].tu > aMaxU)
					aMaxU = aVertex[i].tu;

				if(aVertex[i].tv > aMaxV)
					aMaxV = aVertex[i].tv;
			}
			
			VertexList aMasterList;
			aMasterList.push_back(aVertex[0]);
			aMasterList.push_back(aVertex[1]);
			aMasterList.push_back(aVertex[2]);


			VertexList aList;

			int aLeft = floorf(aMinU);
			int aTop = floorf(aMinV);
			int aRight = ceilf(aMaxU);
			int aBottom = ceilf(aMaxV);
			if (aLeft < 0)
				aLeft = 0;
			if (aTop < 0)
				aTop = 0;
			if (aRight > mTexVecWidth)
				aRight = mTexVecWidth;
			if (aBottom > mTexVecHeight)
				aBottom = mTexVecHeight;

			TextureDataPiece &aStandardPiece = mTextures[0];
			for (i=aTop; i<aBottom; i++)
			{
				for (j=aLeft; j<aRight; j++)
				{
					TextureDataPiece &aPiece = mTextures[i*mTexVecWidth + j];


					VertexList aList = aMasterList;
					for(k=0; k<3; k++)
					{
						aList[k].tu -= j;
						aList[k].tv -= i;
						if (i==mTexVecHeight-1)
							aList[k].tv *= (float)aStandardPiece.mHeight / aPiece.mHeight;
						if (j==mTexVecWidth-1)
							aList[k].tu *= (float)aStandardPiece.mWidth / aPiece.mWidth;
					}
			
					DoPolyTextureClip(aList);
					if (aList.size() >= 3)
					{
						D3DInterface::CheckDXError(theDevice->SetTexture(0, aPiece.mTexture),"SetTexture gTexture");
						D3DInterface::CheckDXError(theDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, gVertexType, &aList[0], aList.size(), 0),"DrawPrimitive (Tri) 4");

		/*				CheckDXError(theDevice->SetTexture(0, NULL),"SetTexture NULL");
						theDevice->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME);
						CheckDXError(theDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, gVertexType, &aList[0], aList.size(), D3DDP_WAIT),"DrawPrimitive (Tri)");
						theDevice->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID);*/
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DInterface::CreateImageTexture(MemoryImage *theImage)
{
	bool wantPurge = false;

	if(theImage->mD3DData==NULL)
	{
		theImage->mD3DData = new TextureData();
		
		// The actual purging was deferred
		wantPurge = theImage->mPurgeBits;

		AutoCrit aCrit(gSexyAppBase->mDDInterface->mCritSect); // Make images thread safe
		mImageSet.insert(theImage);
	}

	TextureData *aData = (TextureData*)theImage->mD3DData;
	aData->CheckCreateTextures(theImage, mD3DDevice, mDD);
	
	if (wantPurge)
		theImage->PurgeBits();

	return aData->mPixelFormat != PixelFormat_Unknown;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DInterface::RecoverBits(MemoryImage* theImage)
{
	if (theImage->mD3DData == NULL)
		return false;

	TextureData* aData = (TextureData*) theImage->mD3DData;
	if (aData->mBitsChangedCount != theImage->mBitsChangedCount) // bits have changed since texture was created
		return false;
	
	for (int aPieceRow = 0; aPieceRow < aData->mTexVecHeight; aPieceRow++)
	{
		for (int aPieceCol = 0; aPieceCol < aData->mTexVecWidth; aPieceCol++)
		{
			TextureDataPiece* aPiece = &aData->mTextures[aPieceRow*aData->mTexVecWidth + aPieceCol];
		
			DDSURFACEDESC2 aDesc;
			aDesc.dwSize = sizeof(aDesc);
			if (D3DInterface::CheckDXError(aPiece->mTexture->Lock(NULL,&aDesc,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_READONLY,NULL),"Lock Texture"))
				return false;

			int offx = aPieceCol*aData->mTexPieceWidth;
			int offy = aPieceRow*aData->mTexPieceHeight;
			int aWidth = std::min(theImage->mWidth-offx, aPiece->mWidth);
			int aHeight = std::min(theImage->mHeight-offy, aPiece->mHeight);

			switch (aData->mPixelFormat)
			{
			case PixelFormat_A8R8G8B8:	CopyTexture8888ToImage(aDesc.lpSurface, aDesc.lPitch, theImage, offx, offy, aWidth, aHeight); break;
			case PixelFormat_A4R4G4B4:	CopyTexture4444ToImage(aDesc.lpSurface, aDesc.lPitch, theImage, offx, offy, aWidth, aHeight); break;
			case PixelFormat_R5G6B5: CopyTexture565ToImage(aDesc.lpSurface, aDesc.lPitch, theImage, offx, offy, aWidth, aHeight); break;
			case PixelFormat_Palette8:	CopyTexturePalette8ToImage(aDesc.lpSurface, aDesc.lPitch, theImage, offx, offy, aWidth, aHeight, aData->mPalette); break;
			case PixelFormat_Unknown: break;
			}

			D3DInterface::CheckDXError(aPiece->mTexture->Unlock(NULL),"Texture Unlock");
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::SetCurTexture(MemoryImage *theImage)
{
	if (theImage==NULL)
	{
		mD3DDevice->SetTexture(0,NULL);
		return;
		}

	if (!CreateImageTexture(theImage))
		return;

	TextureData *aData = (TextureData*)theImage->mD3DData;
	mD3DDevice->SetTexture(0,aData->mTextures[0].mTexture);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::PushTransform(const SexyMatrix3 &theTransform, bool concatenate)
{
	if (mTransformStack.empty() || !concatenate)
		mTransformStack.push_back(theTransform);
	else
	{
		SexyMatrix3 &aTrans = mTransformStack.back();
		mTransformStack.push_back(theTransform*aTrans);
	}
}
	
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::PopTransform()
{
	if (!mTransformStack.empty())
		mTransformStack.pop_back();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::RemoveMemoryImage(MemoryImage *theImage)
{
	if (theImage->mD3DData != NULL)
	{
		delete (TextureData*)theImage->mD3DData;
		theImage->mD3DData = NULL;

		AutoCrit aCrit(gSexyAppBase->mDDInterface->mCritSect); // Make images thread safe
		mImageSet.erase(theImage);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::Cleanup()
{
	Flush();

	ImageSet::iterator anItr;
	for(anItr = mImageSet.begin(); anItr != mImageSet.end(); ++anItr)
	{
		MemoryImage *anImage = *anItr;
		delete (TextureData*)anImage->mD3DData;
		anImage->mD3DData = NULL;
	}

	mImageSet.clear();

	if (mD3DDevice != NULL)
	{
		mD3DDevice->Release();
		mD3DDevice = NULL;
	}

	if (mD3D != NULL)
	{
		mD3D->Release();
		mD3D = NULL;
	}

	if (mDDSDrawSurface != NULL)
	{
		mDDSDrawSurface->Release();
		mDDSDrawSurface = NULL;
	}

	if (mZBuffer != NULL)
	{
		mZBuffer->Release();
		mZBuffer = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::SetupDrawMode(int theDrawMode, const Color &theColor, Image *theImage)
{
	(void)theColor;(void)theImage;
	if (theDrawMode == Graphics::DRAWMODE_NORMAL)
	{
/*		if (theImage != NULL)
		{
			MemoryImage *anImage = (MemoryImage*)theImage;
			mD3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, (anImage->mHasAlpha || anImage->mHasTrans || theColor.mAlpha<255)?TRUE:FALSE);
		}
		else
			mD3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, theColor.mAlpha<255?TRUE:FALSE);
*/

		mD3DDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA); 
		mD3DDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA); 
	}
	else // Additive
	{
//		mD3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
//		theColor.mRed = theColor.mRed * theColor.mAlpha / 255;
//		theColor.mGreen = theColor.mGreen * theColor.mAlpha / 255;
//		theColor.mBlue = theColor.mBlue * theColor.mAlpha / 255;

//		mD3DDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ONE);
		mD3DDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA);
		mD3DDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
	}			
//	mD3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::Blt(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode, bool linearFilter)
{
	if (!mTransformStack.empty())
	{
		BltClipF(theImage,theX,theY,theSrcRect,NULL,theColor,theDrawMode);
		return;
	}

	if (!PreDraw())
		return;

	MemoryImage* aSrcMemoryImage = (MemoryImage*) theImage;

	if (!CreateImageTexture(aSrcMemoryImage))
		return;

	SetupDrawMode(theDrawMode, theColor, theImage);	

	TextureData *aData = (TextureData*)aSrcMemoryImage->mD3DData;

	SetLinearFilter(mD3DDevice, linearFilter);
	aData->Blt(mD3DDevice,theX,theY,theSrcRect,theColor);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::BltMirror(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Color& theColor, int theDrawMode, bool linearFilter)
{
	SexyTransform2D aTransform;		

	aTransform.Translate(-theSrcRect.mWidth,0);
	aTransform.Scale(-1, 1);
	aTransform.Translate(theX, theY);

	BltTransformed(theImage,NULL,theColor,theDrawMode,theSrcRect,aTransform,linearFilter);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::BltClipF(Image* theImage, float theX, float theY, const Rect& theSrcRect, const Rect *theClipRect, const Color& theColor, int theDrawMode)
{
	SexyTransform2D aTransform;
	aTransform.Translate(theX, theY);

	BltTransformed(theImage,theClipRect,theColor,theDrawMode,theSrcRect,aTransform,true);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::StretchBlt(Image* theImage,  const Rect& theDestRect, const Rect& theSrcRect, const Rect* theClipRect, const Color &theColor, int theDrawMode, bool fastStretch, bool mirror)
{
	float xScale = (float)theDestRect.mWidth / theSrcRect.mWidth;
	float yScale = (float)theDestRect.mHeight / theSrcRect.mHeight;

	SexyTransform2D aTransform;
	if (mirror)
	{
		aTransform.Translate(-theSrcRect.mWidth,0);
		aTransform.Scale(-xScale, yScale);
	}
	else
		aTransform.Scale(xScale, yScale);

	aTransform.Translate(theDestRect.mX, theDestRect.mY);
	BltTransformed(theImage,theClipRect,theColor,theDrawMode,theSrcRect,aTransform,!fastStretch);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::BltRotated(Image* theImage, float theX, float theY, const Rect* theClipRect, const Color& theColor, int theDrawMode, double theRot, float theRotCenterX, float theRotCenterY, const Rect &theSrcRect)
{
	SexyTransform2D aTransform;

	aTransform.Translate(-theRotCenterX, -theRotCenterY);
	aTransform.RotateRad(theRot);
	aTransform.Translate(theX+theRotCenterX,theY+theRotCenterY);

	BltTransformed(theImage,theClipRect,theColor,theDrawMode,theSrcRect,aTransform,true);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::BltTransformed(Image* theImage, const Rect* theClipRect, const Color& theColor, int theDrawMode, const Rect &theSrcRect, const SexyMatrix3 &theTransform, bool linearFilter, float theX, float theY, bool center)
{
	if (!PreDraw())
		return;

	MemoryImage* aSrcMemoryImage = (MemoryImage*) theImage;

	if (!CreateImageTexture(aSrcMemoryImage))
		return;

	SetupDrawMode(theDrawMode, theColor, theImage);	

	TextureData *aData = (TextureData*)aSrcMemoryImage->mD3DData;

	if (!mTransformStack.empty())
	{
		SetLinearFilter(mD3DDevice, true); // force linear filtering in the case of a global transform
		if (theX!=0 || theY!=0)
		{
			SexyTransform2D aTransform;
			if (center)
				aTransform.Translate(-theSrcRect.mWidth/2.0f,-theSrcRect.mHeight/2.0f);

			aTransform = theTransform * aTransform;
			aTransform.Translate(theX,theY);
			aTransform = mTransformStack.back() * aTransform;

			aData->BltTransformed(mD3DDevice, aTransform, theSrcRect, theColor, theClipRect);
		}
		else
		{
			SexyTransform2D aTransform = mTransformStack.back()*theTransform;
			aData->BltTransformed(mD3DDevice, aTransform, theSrcRect, theColor, theClipRect, theX, theY, center);
		}
	}
	else
	{
		SetLinearFilter(mD3DDevice, linearFilter);
		aData->BltTransformed(mD3DDevice, theTransform, theSrcRect, theColor, theClipRect, theX, theY, center);
	}
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::DrawLine(double theStartX, double theStartY, double theEndX, double theEndY, const Color& theColor, int theDrawMode)
{
	if (!PreDraw())
		return;

	SetupDrawMode(theDrawMode, theColor, NULL);

	float x1, y1, x2, y2;
	DWORD aColor = RGBA_MAKE(theColor.mRed, theColor.mGreen, theColor.mBlue, theColor.mAlpha);			

	if (!mTransformStack.empty())
	{
		SexyVector2 p1(theStartX,theStartY);
		SexyVector2 p2(theEndX,theEndY);
		p1 = mTransformStack.back()*p1;
		p2 = mTransformStack.back()*p2;

		x1 = p1.x;
		y1 = p1.y;
		x2 = p2.x;
		y2 = p2.y;
	}
	else
	{
		x1 = theStartX;
		y1 = theStartY;
		x2 = theEndX;
		y2 = theEndY;
	}

	D3DTLVERTEX aVertex[3] = 
	{
		{ {x1},		{y1},		{0},{1},{aColor},{0},{0},{0} },
		{ {x2},		{y2},		{0},{1},{aColor},{0},{0},{0} },
		{ {x2+0.5f},{y2+0.5f},	{0},{1},{aColor},{0},{0},{0} }
	};

	D3DInterface::CheckDXError(mD3DDevice->SetTexture(0, NULL),"SetTexture NULL");
	D3DInterface::CheckDXError(mD3DDevice->DrawPrimitive(D3DPT_LINESTRIP, gVertexType, aVertex, 3, 0),"DrawPrimitive (Line)");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::FillRect(const Rect& theRect, const Color& theColor, int theDrawMode)
{
	if (!PreDraw())
		return;

	SetupDrawMode(theDrawMode, theColor, NULL);

	DWORD aColor = RGBA_MAKE(theColor.mRed, theColor.mGreen, theColor.mBlue, theColor.mAlpha);			
	float x = theRect.mX - 0.5f;
	float y = theRect.mY - 0.5f;
	float aWidth = theRect.mWidth;
	float aHeight = theRect.mHeight;

	D3DTLVERTEX aVertex[4] = 
	{
		{ {x},			{y},		{0},{1},{aColor},{0},{0},{0} },
		{ {x},			{y+aHeight},{0},{1},{aColor},{0},{0},{0} },
		{ {x+aWidth},	{y},		{0},{1},{aColor},{0},{0},{0} },
		{ {x+aWidth},	{y+aHeight},{0},{1},{aColor},{0},{0},{0} }
	};


	if (!mTransformStack.empty())
	{
		SexyVector2 p[4] = { SexyVector2(x, y), SexyVector2(x,y+aHeight), SexyVector2(x+aWidth, y) , SexyVector2(x+aWidth, y+aHeight) };

		int i;
		for (i=0; i<4; i++)
		{
			p[i] = mTransformStack.back()*p[i];
			p[i].x -= 0.5f;
			p[i].y -= 0.5f;
			aVertex[i].sx = p[i].x;
			aVertex[i].sy = p[i].y;
		}
	}

	CheckDXError(mD3DDevice->SetTexture(0, NULL),"SetTexture NULL");
	CheckDXError(mD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, gVertexType, aVertex, 4, 0),"DrawPrimitive (Rect)");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::DrawTriangle(const TriVertex &p1, const TriVertex &p2, const TriVertex &p3, const Color &theColor, int theDrawMode)
{
	if (!PreDraw())
		return;

	SetupDrawMode(theDrawMode, theColor, NULL);

	DWORD aColor = RGBA_MAKE(theColor.mRed, theColor.mGreen, theColor.mBlue, theColor.mAlpha);			
	D3DTLVERTEX aVertex[3] = 
	{
		{ {p1.x},{p1.y},{0},{1},{GetColorFromTriVertex(p1, aColor)},{0},{0},{0} },
		{ {p2.x},{p2.y},{0},{1},{GetColorFromTriVertex(p2, aColor)},{0},{0},{0} },
		{ {p3.x},{p3.y},{0},{1},{GetColorFromTriVertex(p3, aColor)},{0},{0},{0} }
	};


	CheckDXError(mD3DDevice->SetTexture(0, NULL),"SetTexture NULL");
	CheckDXError(mD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, gVertexType, aVertex, 3, 0),"DrawPrimitive (Tri) 5");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::FillPoly(const Point theVertices[], int theNumVertices, const Rect *theClipRect, const Color &theColor, int theDrawMode, int tx, int ty)
{
	if (theNumVertices<3)
		return;

	if (!PreDraw())
		return;

	SetupDrawMode(theDrawMode, theColor, NULL);
	DWORD aColor = RGBA_MAKE(theColor.mRed, theColor.mGreen, theColor.mBlue, theColor.mAlpha);			

	CheckDXError(mD3DDevice->SetTexture(0, NULL),"SetTexture NULL");

	VertexList aList;
	for (int i=0; i<theNumVertices; i++)
	{
		D3DTLVERTEX vert = 	{ {theVertices[i].mX + (D3DVALUE)tx}, {theVertices[i].mY + (D3DVALUE)ty},	{0},	{1},	{aColor},	{0},	{0},		{0} };
		if (!mTransformStack.empty())
		{
			SexyVector2 v(vert.sx,vert.sy);
			v = mTransformStack.back()*v;
			vert.sx = v.x;
			vert.sy = v.y;
		}

		aList.push_back(vert);
	}

	if (theClipRect != NULL)
		DrawPolyClipped(mD3DDevice,theClipRect,aList);
	else
		D3DInterface::CheckDXError(mD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, gVertexType, &aList[0], aList.size(), 0),"DrawPrimitive (FillPoly)");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::DrawTriangleTex(const TriVertex &p1, const TriVertex &p2, const TriVertex &p3, const Color &theColor, int theDrawMode, Image *theTexture, bool blend)
{
	TriVertex aVertices[1][3] = {{p1, p2, p3}};
	DrawTrianglesTex(aVertices,1,theColor,theDrawMode,theTexture,blend);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::DrawTrianglesTex(const TriVertex theVertices[][3], int theNumTriangles, const Color &theColor, int theDrawMode, Image *theTexture, float tx, float ty, bool blend)
{
	if (!PreDraw())
		return;

	MemoryImage* aSrcMemoryImage = (MemoryImage*)theTexture;

	if (!CreateImageTexture(aSrcMemoryImage))
		return;

	SetupDrawMode(theDrawMode, theColor, theTexture);
	DWORD aColor = RGBA_MAKE(theColor.mRed, theColor.mGreen, theColor.mBlue, theColor.mAlpha);			
	
	TextureData *aData = (TextureData*)aSrcMemoryImage->mD3DData;

	SetLinearFilter(mD3DDevice, blend);	

	aData->BltTriangles(mD3DDevice, theVertices, theNumTriangles, aColor, tx, ty);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::DrawTrianglesTexStrip(const TriVertex theVertices[], int theNumTriangles, const Color &theColor, int theDrawMode, Image *theTexture, float tx, float ty, bool blend)
{
	TriVertex aList[100][3];
	int aTriNum = 0;
	while (aTriNum < theNumTriangles)
	{
		int aMaxTriangles = std::min(100,theNumTriangles - aTriNum);
		for (int i=0; i<aMaxTriangles; i++)
		{
			aList[i][0] = theVertices[aTriNum];
			aList[i][1] = theVertices[aTriNum+1];
			aList[i][2] = theVertices[aTriNum+2];
			aTriNum++;
		}
		DrawTrianglesTex(aList,aMaxTriangles,theColor,theDrawMode,theTexture, tx, ty, blend);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DInterface::Flush()
{
	if (mSceneBegun)
	{
		mD3DDevice->EndScene();

		mSceneBegun = false;
		mErrorString.erase();
	}
}

