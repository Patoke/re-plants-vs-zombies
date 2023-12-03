#include "D3D8Helper.h"
#include "D3DTester.h"
#include "DirectXErrorString.h"
#include "Common.h"
#include "D3DInterface.h"
#include "SexyAppBase.h"
#include <cstdint>



using namespace Sexy;
static const int gD3DTestTextureWidth = 64;
static const int gD3DTestTextureHeight = 64;
static bool gD3DTestHas32BitTexture = false;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
D3DTestImage::D3DTestImage()
{
	mBits = NULL;
	mWidth = 0;
	mHeight = 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
D3DTestImage::D3DTestImage(int theWidth, int theHeight)
{
	mBits = NULL;
	mWidth = 0;
	mHeight = 0;	

	Create(theWidth,theHeight);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
D3DTestImage::~D3DTestImage()
{
	FreeImage();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTestImage::Create(int theWidth, int theHeight)
{
	FreeImage();
	if(theWidth>0 && theHeight>0)
	{
		mBits = new DWORD[theWidth*theHeight];
		mWidth = theWidth;
		mHeight = theHeight;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTestImage::FreeImage()
{
	delete [] mBits;
	mWidth = 0;
	mHeight = 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
const D3DTestImage& D3DTestImage::operator=(const D3DTestImage &theImage)
{
	if (&theImage==this)
		return *this;

	Create(theImage.GetWidth(), theImage.GetHeight());
	memcpy(mBits, theImage.GetBits(), mWidth*mHeight*4);

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTestImage::CompareEqual(const D3DTestImage &theImage) const
{
	if (theImage.GetWidth() != GetWidth())
		return false;

	if (theImage.GetHeight() != GetHeight())
		return false;

	return memcmp(theImage.GetBits(),GetBits(),mWidth*mHeight*4)==0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTestImage::FillRect(int x, int y, int theWidth, int theHeight, DWORD theColor)
{
	DWORD *aRow = mBits + y*mWidth + x;
	for(int j=0; j<theHeight; j++)
	{
		DWORD *aPixel = aRow;
		for(int i=0; i<theWidth; i++)
			*aPixel++ = theColor;

		aRow += mWidth;
	}

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTestImage::MakeVerticalBands()
{
	for(int i=0; i<mWidth; i++)
		FillRect(i,0,1,mHeight,i&1?0xFFFFFFFF:0xFF000000);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTestImage::CopyToTexture8888(LPDIRECTDRAWSURFACE7 theTexture, int offx, int offy, int texWidth, int texHeight)
{
	DDSURFACEDESC2 aDesc;
	aDesc.dwSize = sizeof(aDesc);
	D3DTester::CheckDXError(theTexture->Lock(NULL,&aDesc,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_WRITEONLY,NULL),"Lock Texture");

	int aWidth = std::min(texWidth,(GetWidth()-offx));
	int aHeight = std::min(texHeight,(GetHeight()-offy));

	if(aWidth < texWidth || aHeight < texHeight)
		memset(aDesc.lpSurface, 0, aDesc.lPitch*aDesc.dwHeight);

	if(aWidth>0 && aHeight>0)
	{
		DWORD *srcRow = GetBits() + offy * GetWidth() + offx;
		char *dstRow = (char*)aDesc.lpSurface;

		for(int y=0; y<aHeight; y++)
		{
			DWORD *src = srcRow;
			DWORD *dst = (DWORD*)dstRow;
			for(int x=0; x<aWidth; x++)
				*dst++ = *src++;

			srcRow += GetWidth();
			dstRow += aDesc.lPitch;
		}
	}

	D3DTester::CheckDXError(theTexture->Unlock(NULL),"Texture Unlock");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTestImage::CopyToTexture4444(LPDIRECTDRAWSURFACE7 theTexture, int offx, int offy, int texWidth, int texHeight)
{

	DDSURFACEDESC2 aDesc;
	aDesc.dwSize = sizeof(aDesc);
	D3DTester::CheckDXError(theTexture->Lock(NULL,&aDesc,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT | DDLOCK_WRITEONLY,NULL),"Lock Texture");

	int aWidth = std::min(texWidth,(GetWidth()-offx));
	int aHeight = std::min(texHeight,(GetHeight()-offy));

	if(aWidth < texWidth || aHeight < texHeight)
		memset(aDesc.lpSurface, 0, aDesc.lPitch*aDesc.dwHeight);

	if(aWidth>0 && aHeight>0)
	{
		DWORD *srcRow = GetBits() + offy * GetWidth() + offx;
		char *dstRow = (char*)aDesc.lpSurface;

		for(int y=0; y<aHeight; y++)
		{
			DWORD *src = srcRow;
			ushort *dst = (ushort*)dstRow;
			for(int x=0; x<aWidth; x++)
			{
				DWORD aPixel = *src++;
				*dst++ = ((aPixel>>16)&0xF000) | ((aPixel>>12)&0x0F00) | ((aPixel>>8)&0x00F0) | ((aPixel>>4)&0x000F);
			}

			srcRow += GetWidth();
			dstRow += aDesc.lPitch;
		}
	}

	D3DTester::CheckDXError(theTexture->Unlock(NULL),"Texture Unlock");
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTestImage::DrawPieceToDevice(LPDIRECT3DDEVICE7 theDevice, LPDIRECTDRAWSURFACE7 theTexture, float x, float y, int offx, int offy, int texWidth, int texHeight, DWORD theColor)
{
	float maxU = (float)texWidth/gD3DTestTextureWidth;
	float maxV = (float)texHeight/gD3DTestTextureHeight;

	if (gD3DTestHas32BitTexture)
		CopyToTexture8888(theTexture, offx, offy, texWidth, texHeight);
	else
		CopyToTexture4444(theTexture, offx, offy, texWidth, texHeight);

	x -= 0.5f;
	y -= 0.5f;

	D3DTLVERTEX aVertex[4] = 
	{
		{ {x},			{y},			{0},{1},{theColor},{0},{0},	{0} },
		{ {x},			{y+texHeight},	{0},{1},{theColor},{0},{0},	{maxV} },
		{ {x+texWidth},	{y},			{0},{1},{theColor},{0},{maxU},	{0} },
		{ {x+texWidth},	{y+texHeight},	{0},{1},{theColor},{0},{maxU},	{maxV} }
	};
		
	D3DTester::CheckDXError(theDevice->SetTexture(0, theTexture),"SetTexture theTexture");
	D3DTester::CheckDXError(theDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, aVertex, 4, D3DDP_WAIT),"DrawPrimitive");
	D3DTester::CheckDXError(theDevice->SetTexture(0, NULL),"SetTexture NULL");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTestImage::DrawToDevice(LPDIRECT3DDEVICE7 theDevice, LPDIRECTDRAWSURFACE7 theTexture, int x, int y, DWORD theColor)
{
	int aWidth = GetWidth();
	int aHeight = GetHeight();

	int aTexWidth = std::min(64,gD3DTestTextureWidth);
	int aTexHeight = std::min(64,gD3DTestTextureHeight);

	for(int j=0; j<aHeight; j+=aTexHeight)
	{
		for(int i=0; i<aWidth; i+=aTexWidth)
		{
			DrawPieceToDevice(theDevice, theTexture, (float)x+i,(float)y+j,i,j,aTexWidth,aTexHeight,theColor);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int D3DTestImage::ColorDistance(DWORD c1, DWORD c2)
{
	int r1 = (c1&0xff0000)>>16;
	int g1 = (c1&0x00ff00)>>8;
	int b1 = (c1&0xff);

	int r2 = (c2&0xff0000)>>16;
	int g2 = (c2&0x00ff00)>>8;
	int b2 = (c2&0xff);

	return (r1-r2)*(r1-r2) + (g1-g2)*(g1-g2) + (b1-b2)*(b1-b2);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTestImage::IsUniformColor(DWORD theColor, int &theNumMistakes, int testWidth, int testHeight) const
{
	theNumMistakes = 0;
	const DWORD *aRow = GetBits();
	DWORD aLastPixel = *aRow;
	bool isUniform = true;
	for(int i=0; i<testHeight; i++)
	{
		const DWORD *aSrc = aRow;
		for(int j=0; j<testWidth; j++)
		{
			DWORD aPixel = *aSrc++;
			if(aLastPixel!=aPixel)
				isUniform = false;

			if(ColorDistance(aPixel,theColor)>COLOR_TOLERANCE)
				theNumMistakes++;
		}

		aRow += GetWidth();
	}

	return isUniform;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int D3DTestImage::CheckUniformBands(int testWidth, int testHeight, int xoff, int yoff)
{
	int aNumMistakes = 0;
	const DWORD *aRow = GetBits() + yoff*GetWidth() + xoff;
	DWORD aLastPixel = *aRow;
	//bool isUniform = true; // unused
	for(int i=0; i<testHeight; i++)
	{
		const DWORD *aSrc = aRow;
		for(int j=0; j<testWidth; j++)
		{
			DWORD aPixel = *aSrc++;
			if(aLastPixel!=aPixel)
				//isUniform = false; // unused

			if(ColorDistance(aPixel,(j&1)?0xFFFFFF:0x000000)>COLOR_TOLERANCE)
				aNumMistakes++;
		}

		aRow += GetWidth();
	}

	return aNumMistakes;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static int D3DTestHighBit(DWORD theMask)
{
	int aBit = 31;
	while(aBit>0)
	{
		if((1<<aBit) & theMask)
			break;

		aBit--;
	}

	return aBit;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
template <class PixelType> 
static void D3DTestPixelConvert(D3DTestImage &theImage, DDSURFACEDESC2 &theDesc, PixelType)
{
	int rMask = theDesc.ddpfPixelFormat.dwRBitMask;
	int gMask = theDesc.ddpfPixelFormat.dwGBitMask;
	int bMask = theDesc.ddpfPixelFormat.dwBBitMask;

	int redShift = 23-D3DTestHighBit(rMask);
	int greenShift = 15-D3DTestHighBit(gMask);
	int blueShift = 7-D3DTestHighBit(bMask);

	char *srcRow = (char*)theDesc.lpSurface;
	DWORD *dstRow = theImage.GetBits();
	for(int j=0; j<theImage.GetHeight(); j++)
	{
		PixelType *src = (PixelType*)srcRow;
		DWORD *dst = dstRow;
		for(int i=0; i<theImage.GetWidth(); i++)
		{
			
			PixelType aPixel = *src++;
			int r = aPixel & rMask;
			int g = aPixel & gMask;
			int b = aPixel & bMask;

			if(redShift>0) r<<=redShift; else r>>=-redShift;
			if(greenShift>0) g<<=greenShift; else g>>=-greenShift;
			if(blueShift>0) b<<=blueShift; else b>>=-blueShift;
			*dst++ = 0xFF000000 | r | g | b;
		}

		srcRow += theDesc.lPitch;
		dstRow += theImage.GetWidth();
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
[[maybe_unused]]
static void D3DTestPixelConvert24(D3DTestImage &theImage, DDSURFACEDESC2 &theDesc)
{
	int rMask = theDesc.ddpfPixelFormat.dwRBitMask;
	int gMask = theDesc.ddpfPixelFormat.dwGBitMask;
	int bMask = theDesc.ddpfPixelFormat.dwBBitMask;

	int redShift = 23-D3DTestHighBit(rMask);
	int greenShift = 15-D3DTestHighBit(gMask);
	int blueShift = 7-D3DTestHighBit(bMask);

	char *srcRow = (char*)theDesc.lpSurface;
	DWORD *dstRow = theImage.GetBits();
	for(int j=0; j<theImage.GetHeight(); j++)
	{
		char *src = srcRow;
		DWORD *dst = dstRow;
		for(int i=0; i<theImage.GetWidth(); i++)
		{
			
			DWORD aPixel = *((DWORD*)src)&0xFFFFFF;
			src += 3;

			int r = aPixel & rMask;
			int g = aPixel & gMask;
			int b = aPixel & bMask;

			if(redShift>0) r<<=redShift; else r>>=-redShift;
			if(greenShift>0) g<<=greenShift; else g>>=-greenShift;
			if(blueShift>0) b<<=blueShift; else b>>=-blueShift;
			*dst++ = 0xFF000000 | r | g | b;
		}

		srcRow += theDesc.lPitch;
		dstRow += theImage.GetWidth();
	}
}
*/

#define SafeSetRenderState(x,y)\
	CheckDXError(mD3DDevice7->SetRenderState(x,y),#x ", " #y)

#define SafeSetTextureStageState(i,x,y)\
	CheckDXError(mD3DDevice7->SetTextureStageState(i,x,y),#x ", " #y)

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
D3DTester::D3DTester()
{
	mDD7 = NULL;
	mPrimarySurface = NULL;
	mTextureSurface = NULL;
	mTextureSurface2 = NULL;
	mD3D7 = NULL;
	mD3DDevice7 = NULL;
	mRegKey = NULL;
	memset(&mDisplayGUID, 0, sizeof(GUID));

	mMinVidMemory = 0;
	mRecommendedVidMemory = 0;
	mDriverYear = 0;

	mCheckRegistry = true;
	mResultsChanged = false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
D3DTester::~D3DTester()
{
	Cleanup();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTester::CheckRegistry()
{
	mResultsChanged = true;

	DWORD aSize, aType;

	if (mRegKey==NULL)
		return false;

	// Check Test Version
	DWORD aVersion = 0;
	aSize = sizeof(aVersion);
	aType = REG_DWORD;
	if (RegQueryValueExA(mRegKey, "Version", 0, &aType, (uchar*) &aVersion, &aSize) != ERROR_SUCCESS)
		return false;

	if (aVersion != TEST_VERSION)
		return false;

	// Check Min Vid Memory
	DWORD aMinVidMemory = 0;
	aSize = sizeof(aMinVidMemory);
	aType = REG_DWORD;
	if (RegQueryValueExA(mRegKey, "MinVidMemory", 0, &aType, (uchar*) &aMinVidMemory, &aSize) != ERROR_SUCCESS)
		return false;

	if (aMinVidMemory != mMinVidMemory)
		return false;

	// Check Recommended Vid Memory
	DWORD aRecVidMemory = 0;
	aSize = sizeof(aRecVidMemory);
	aType = REG_DWORD;
	if (RegQueryValueExA(mRegKey, "RecVidMemory", 0, &aType, (uchar*) &aRecVidMemory, &aSize) != ERROR_SUCCESS)
		return false;

	if (aRecVidMemory != mRecommendedVidMemory)
		return false;

	// Check GUID
	UUID aGUID;
	aSize = sizeof(aGUID);
	aType = REG_BINARY;
	if (RegQueryValueExA(mRegKey, "DisplayGUID", 0, &aType, (uchar*) &aGUID, &aSize) != ERROR_SUCCESS)
		return false;

	if (aSize != sizeof(aGUID))
		return false;

	if (memcmp(&aGUID,&mDisplayGUID,sizeof(aGUID)) != 0) // different video card or driver
		return false;

	// Get failure reason
	char aBuf[4096];
	aType = REG_SZ;
	aSize = 4096;
	if (RegQueryValueExA(mRegKey, "FailureReason", 0, &aType, (uchar*) aBuf, &aSize) != ERROR_SUCCESS)
		return false;

	mFailureReason = aBuf;

	// Get warining
	aType = REG_SZ;
	aSize = 4096;
	if (RegQueryValueExA(mRegKey, "Warning", 0, &aType, (uchar*) aBuf, &aSize) != ERROR_SUCCESS)
		return false;

	mWarning = aBuf;

	mResultsChanged = false;

	if (!mFailureReason.empty())
		return false;
	else
		return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTester::WriteToRegistry()
{
	DWORD aSize, aType;

	if (mRegKey==NULL)
		return;

	// Write Test Version
	DWORD aVersion = TEST_VERSION;
	aSize = sizeof(aVersion);
	aType = REG_DWORD;
	RegSetValueExA(mRegKey, "Version", 0, aType, (uchar*) &aVersion, aSize);

	// Write Min Vid Memory
	DWORD aMinVidMemory = mMinVidMemory;
	aSize = sizeof(aMinVidMemory);
	aType = REG_DWORD;
	RegSetValueExA(mRegKey, "MinVidMemory", 0, aType, (uchar*) &aMinVidMemory, aSize);

	// Write Recommended Vid Memory
	DWORD aRecVidMemory = mRecommendedVidMemory;
	aSize = sizeof(aRecVidMemory);
	aType = REG_DWORD;
	RegSetValueExA(mRegKey, "RecVidMemory", 0, aType, (uchar*) &aRecVidMemory, aSize);

	// Write GUID
	aSize = sizeof(mDisplayGUID);
	aType = REG_BINARY;
	RegSetValueExA(mRegKey, "DisplayGUID", 0, aType, (uchar*) &mDisplayGUID, aSize);

	// Write failure reason
	aType = REG_SZ;
	aSize = mFailureReason.length()+1;
	RegSetValueExA(mRegKey, "FailureReason", 0, aType, (uchar*) mFailureReason.c_str(), aSize);

	// Write warining
	aType = REG_SZ;
	aSize = mWarning.length()+1;
	RegSetValueExA(mRegKey, "Warning", 0, aType, (uchar*) mWarning.c_str(), aSize);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTester::FileContains(FILE* theFile, const char* theString)
{
	bool found = false;
	char aBuf[4096];
	while (!feof(theFile))
	{
		if (fgets(aBuf,4000,theFile)==NULL)
			break;

		std::string aStr = Trim(aBuf);
		if (!aStr.empty() && StrFindNoCase(theString,aStr.c_str()) >= 0)
		{
			found = true;
			break;
		}
	}
	
	return found;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTester::IsSupportedCard(const char *theDisplayDesc)
{
	// Look for 'bad' exception list
	FILE* aFile = fopen("vhwb.dat","r");
	if (aFile != NULL)
	{
		bool found = FileContains(aFile, theDisplayDesc);
		fclose(aFile);
		if (found)
			return false;
	}

	// Look for 'good' supported list
	aFile = fopen("vhw.dat","r");
	if (aFile==NULL) // default checks
	{
		if (mDriverYear>=2002)
			return true;

		if (StrFindNoCase(theDisplayDesc,"nvidia") >= 0)
			return true;

		if (StrFindNoCase(theDisplayDesc,"radeon") >= 0)
			return true;

		if (StrFindNoCase(theDisplayDesc,"ati ") >= 0)
			return true;
	
		return false;
	}

	bool found = FileContains(aFile, theDisplayDesc);
	fclose(aFile);
	return found;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTester::Init(HWND theHWND, LPDIRECTDRAW7 theDDraw)
{
	Cleanup();
	mDriverYear = 0;

	if (mCheckRegistry)
	{
		std::string aKey = RemoveTrailingSlash("SOFTWARE\\" + gSexyAppBase->mRegKey) + "\\Test3D";
		RegCreateKeyExA(HKEY_CURRENT_USER, aKey.c_str(),0,(char*)"",REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&mRegKey,NULL);
	}

	try
	{

		if (theDDraw==NULL)
		{
			//extern HMODULE gDDrawDLL;

			typedef HRESULT (WINAPI *DirectDrawCreateExFunc)(GUID FAR *lpGUID, LPVOID *lplpDD, REFIID iid, IUnknown FAR *pUnkOuter);
			DirectDrawCreateExFunc aDirectDrawCreateExFunc = (DirectDrawCreateExFunc)GetProcAddress(gDDrawDLL,"DirectDrawCreateEx");
			if (aDirectDrawCreateExFunc == NULL)							
				return Fail("No DirectDrawCreateEx"); 			

			CheckDXError(aDirectDrawCreateExFunc(NULL, (LPVOID*)&mDD7, IID_IDirectDraw7, NULL),"DirectDrawCreateEx");
		}
		else
		{
			theDDraw->AddRef();
			mDD7 = theDDraw;
		}

		if (!GetD3D8AdapterInfo(mDisplayGUID,mDisplayDriver,mDisplayDescription))
		{
			// Get Device GUID
			DDDEVICEIDENTIFIER2 aDeviceInfo;
			CheckDXError(mDD7->GetDeviceIdentifier(&aDeviceInfo,0), "GetDeviceIdentifier");
			mDisplayGUID = aDeviceInfo.guidDeviceIdentifier;
			mDisplayDriver = aDeviceInfo.szDriver;
			mDisplayDescription = aDeviceInfo.szDescription;
		}

		// Test Video Memory
		DWORD dwTotal, dwFree; 
		DDSCAPS2 ddsCaps;
		ZeroMemory(&ddsCaps,sizeof(ddsCaps));
		ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY;
	 
		HDC aDC = GetDC(NULL);
		int aWidth = GetDeviceCaps(aDC, HORZRES);
		int aHeight = GetDeviceCaps(aDC, VERTRES);
		int aBPP = GetDeviceCaps( aDC, BITSPIXEL );
		ReleaseDC(NULL, aDC);

		HRESULT aResult =  mDD7->GetAvailableVidMem(&ddsCaps, &dwTotal, &dwFree);
		if (!SUCCEEDED(aResult))
		{
			Warn(StrFormat("GetAvailableVidMem failed: %s",GetDirectXErrorString(aResult).c_str()));
		}
		else
		{
			// fix @Patoke: fix integer overflow
			int64_t total64bit = dwTotal; // dwTotal will overflow but as long as the vram isn't INT_MAX + 1 it won't heck up
			total64bit += (aBPP / 8) * aWidth * aHeight;
			total64bit /= (1024 * 1024);
			//dwTotal += (aBPP/8)*aWidth*aHeight;
			//dwTotal /= (1024*1024);
			if (total64bit < mMinVidMemory)
				return Fail("Not enough video memory.");
			else if (total64bit < mRecommendedVidMemory)
				Warn("Low video memory.");
		}

		// Check registry to see if we've already done the test
		if (mCheckRegistry)
		{
			if (CheckRegistry())
				return false;

			mShouldWriteToRegistry = true;
		}

		mFailureReason = "";
		mWarning = "";

		// Get date on driver dll
		std::string aPath = mDisplayDriver;
		if (aPath.find_first_of("/\\")==std::string::npos)
		{
			char aBuf[_MAX_PATH+1];
			if (GetSystemDirectoryA(aBuf,sizeof(aBuf)-1))
				aPath = AddTrailingSlash(aBuf,true) + aPath;
		}

		FILETIME aFileTime;
		memset(&aFileTime, 0, sizeof(aFileTime));
		HANDLE aFileHandle = CreateFileA(aPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (aFileHandle != INVALID_HANDLE_VALUE)
		{				
			SYSTEMTIME aSystemTime;
			if (GetFileTime(aFileHandle, NULL, NULL, &aFileTime) && FileTimeToSystemTime(&aFileTime,&aSystemTime))
				mDriverYear = aSystemTime.wYear;

			CloseHandle(aFileHandle);
		}

		// Check supported cards
		if (!IsSupportedCard(mDisplayDescription.c_str()))
			Warn(StrFormat("Unsupported video card: %s",mDisplayDescription.c_str()));

		// Get Direct3D7 to test 3d capabilities
		CheckDXError(mDD7->QueryInterface(IID_IDirect3D7, (LPVOID*)&mD3D7),"QueryInterface IID_IDirect3D7"); 

		CheckDXError(mDD7->SetCooperativeLevel(theHWND, DDSCL_NORMAL),"SetCooperativeLevel");

		// Create Primary Surface for test rendering
		DDSURFACEDESC2 ddsd;
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags  = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
		ddsd.dwWidth = 100;
		ddsd.dwHeight = 100;
		CheckDXError(mDD7->CreateSurface(&ddsd, &mPrimarySurface, NULL),"CreateSurface (Primary)");
		mTestImage.Create(ddsd.dwWidth,ddsd.dwHeight);


		CheckDXError(mD3D7->CreateDevice(IID_IDirect3DHALDevice, mPrimarySurface, &mD3DDevice7),"CreateDevice");

		DWORD aFormat = 0;
		CheckDXError(mD3DDevice7->EnumTextureFormats(PixelFormatsCallback,&aFormat),"EnumTextureFormats");
		if (!(aFormat & PixelFormat_A8R8G8B8))
		{
			Warn("A8R8G8B8 texture format not supported.");
			if (!(aFormat & PixelFormat_A4R4G4B4))
				return Fail("A4R4G4B4 and A8R8G8B8 texture formats not supported.");

			gD3DTestHas32BitTexture = false;
		}
		else
			gD3DTestHas32BitTexture = true;

		// Create Texture Surface
		DDSURFACEDESC2 aDesc;
		ZeroMemory(&aDesc, sizeof(aDesc));
		aDesc.dwSize = sizeof(aDesc);	
		aDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
		aDesc.ddsCaps.dwCaps = DDSCAPS_TEXTURE;	
		aDesc.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;

		aDesc.dwWidth = 64;
		aDesc.dwHeight = 64;	

		aDesc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
		aDesc.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;

		if (gD3DTestHas32BitTexture)
		{
			aDesc.ddpfPixelFormat.dwRGBBitCount = 32;
			aDesc.ddpfPixelFormat.dwRGBAlphaBitMask = 0xFF000000;
			aDesc.ddpfPixelFormat.dwRBitMask		= 0x00FF0000;
			aDesc.ddpfPixelFormat.dwGBitMask		= 0x0000FF00;
			aDesc.ddpfPixelFormat.dwBBitMask		= 0x000000FF;
		}
		else
		{
			aDesc.ddpfPixelFormat.dwRGBBitCount = 16;
			aDesc.ddpfPixelFormat.dwRGBAlphaBitMask = 0xF000;
			aDesc.ddpfPixelFormat.dwRBitMask		= 0x0F00;
			aDesc.ddpfPixelFormat.dwGBitMask		= 0x00F0;
			aDesc.ddpfPixelFormat.dwBBitMask		= 0x000F;
		}

		CheckDXError(mDD7->CreateSurface(&aDesc, &mTextureSurface, NULL), "CreateSurface (TextureSurface1)");
		CheckDXError(mDD7->CreateSurface(&aDesc, &mTextureSurface2, NULL), "CreateSurface (TextureSurfacd2)");
	}
	catch(TestException &ex)
	{
		return Fail(ex.mMsg);
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
HRESULT CALLBACK D3DTester::PixelFormatsCallback(LPDDPIXELFORMAT theFormat, LPVOID lpContext)
{
	*((DWORD*)lpContext) |= D3DInterface::GetDDPixelFormat(theFormat);
	
	return D3DENUMRET_OK; 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTester::Cleanup()
{	
	if (mDD7)
	{
		mDD7->Release();
		mDD7 = NULL;
	}

	if (mD3D7)
	{
		mD3D7->Release();
		mD3D7 = NULL;
	}

	if (mD3DDevice7)
	{
		mD3DDevice7->Release();
		mD3DDevice7 = NULL;
	}

	if (mPrimarySurface)
	{
		mPrimarySurface->Release();
		mPrimarySurface = NULL;
	}

	if (mTextureSurface)
	{
		mTextureSurface->Release();
		mTextureSurface = NULL;
	}

	if (mTextureSurface2)
	{
		mTextureSurface2->Release();
		mTextureSurface2 = NULL;
	}

	if (mRegKey)
	{
		RegCloseKey(mRegKey);
		mRegKey = NULL;
	}

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTester::Fail(const std::string &theStr)
{
	mFailureReason = theStr;
	return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTester::Warn(const std::string &theStr)
{
	mWarning = theStr;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTester::CheckDXError(HRESULT theResult, const char *theMsg)
{
	if (FAILED(theResult))
	{
		std::string aMsg = "DXError - ";
		aMsg += theMsg;
		aMsg += ": ";
		aMsg += GetDirectXErrorString(theResult);

		throw TestException(aMsg);
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTester::CopyPrimaryToTestImage()
{
	DDBLTFX aBltFX;
	ZeroMemory(&aBltFX, sizeof(aBltFX));
	aBltFX.dwSize = sizeof(aBltFX);    	

	DDSURFACEDESC2 aDesc;
	memset(&aDesc, 0, sizeof(aDesc));
	aDesc.dwSize = sizeof(aDesc);

	D3DTester::CheckDXError(mPrimarySurface->Lock(NULL,&aDesc,DDLOCK_WAIT,NULL),"CopyPrimary Lock");

	if(aDesc.ddpfPixelFormat.dwRGBBitCount==32)
		D3DTestPixelConvert<DWORD>(mTestImage,aDesc,0);
	else if(aDesc.ddpfPixelFormat.dwRGBBitCount==16)
		D3DTestPixelConvert<unsigned short>(mTestImage,aDesc,0);
	else if(aDesc.ddpfPixelFormat.dwRGBBitCount==24)
		throw TestException("Can't test 24-bit mode.");
//		D3DTestPixelConvert24(mTestImage,aDesc);
	else
	{
		mPrimarySurface->Unlock(NULL);
		throw TestException("Invalid Color Depth");
	}

	mPrimarySurface->Unlock(NULL);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTester::TestAlphaBlend()
{
	try
	{
		CheckDXError(mD3DDevice7->BeginScene());
		CheckDXError(mD3DDevice7->Clear(0, NULL, D3DCLEAR_TARGET ,0xff000000, 1.0f, 0L),"Clear");

		SafeSetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,TRUE);
		SafeSetRenderState(D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA);
		SafeSetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		SafeSetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

		D3DTestImage anImage(64,64);

		anImage.FillRect(0,0,64,64,0xFF0000FF);
		anImage.DrawToDevice(mD3DDevice7, mTextureSurface, 0, 0); 

		anImage.FillRect(0,0,64,64,0x80FF0000);
		anImage.DrawToDevice(mD3DDevice7, mTextureSurface, 0, 0); 

		mD3DDevice7->EndScene();

		CopyPrimaryToTestImage();
	}
	catch(TestException &ex)
	{
		mD3DDevice7->EndScene();
		return Fail(ex.mMsg);
	}

	int aNumErrors = 0;
	bool isUniform = mTestImage.IsUniformColor(0x7f007f,aNumErrors,10,10);
	if (aNumErrors==0)
	{
		if (isUniform)
			return true;
		else
			return Warn("Alpha blend test not uniform.");
	}
	else
		return Fail("Alpha blend Not Supported");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTester::TestAdditiveBlend()
{
	try
	{
		CheckDXError(mD3DDevice7->BeginScene());
		CheckDXError(mD3DDevice7->Clear(0, NULL, D3DCLEAR_TARGET ,0xff000000, 1.0f, 0L),"Clear");

		SafeSetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,TRUE);
		SafeSetRenderState(D3DRENDERSTATE_SRCBLEND,  D3DBLEND_ONE);
		SafeSetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		SafeSetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

		D3DTestImage anImage(64,64);

		anImage.FillRect(0,0,64,64,0xFF404040);
		anImage.DrawToDevice(mD3DDevice7, mTextureSurface, 0, 0); 

		anImage.FillRect(0,0,64,64,0xFF404040);
		anImage.DrawToDevice(mD3DDevice7, mTextureSurface, 0, 0); 

		mD3DDevice7->EndScene();

		CopyPrimaryToTestImage();
	}
	catch(TestException &ex)
	{
		mD3DDevice7->EndScene();
		return Fail(ex.mMsg);
	}

	int aNumErrors = 0;
	bool isUniform = mTestImage.IsUniformColor(0x808080,aNumErrors,10,10);
	if(aNumErrors==0)
	{
		if (isUniform)
			return true;
		else
			return Warn("Additive blend test not uniform.");
	}
	else
		return Fail("Additive blend not supported.");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTester::TestAlphaAddBlend()
{
	try
	{
		CheckDXError(mD3DDevice7->BeginScene());
		CheckDXError(mD3DDevice7->Clear(0, NULL, D3DCLEAR_TARGET ,0xff000000, 1.0f, 0L),"Clear");

		SafeSetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,TRUE);
		SafeSetRenderState(D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA);
		SafeSetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
		SafeSetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

		D3DTestImage anImage(64,64);

		anImage.FillRect(0,0,64,64,0xFF404040);
		anImage.DrawToDevice(mD3DDevice7, mTextureSurface, 0, 0); 

		anImage.FillRect(0,0,64,64,0x80808080);
		anImage.DrawToDevice(mD3DDevice7, mTextureSurface, 0, 0); 

		mD3DDevice7->EndScene();

		CopyPrimaryToTestImage();
	}
	catch(TestException &ex)
	{
		mD3DDevice7->EndScene();
		return Fail(ex.mMsg);
	}

	int aNumErrors = 0;
	bool isUniform = mTestImage.IsUniformColor(0x808080,aNumErrors,10,10);
	if(aNumErrors==0)
	{
		if (isUniform)
			return true;
		else
			return Warn("AlphaAdd blend test not uniform.");
	}
	else
		return Fail("AlphaAdd blend not supported.");
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTester::TestAlphaModulate()
{
	try
	{
		CheckDXError(mD3DDevice7->BeginScene());
		CheckDXError(mD3DDevice7->Clear(0, NULL, D3DCLEAR_TARGET ,0xff000000, 1.0f, 0L),"Clear");

		SafeSetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,TRUE);
		SafeSetRenderState(D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA);
		SafeSetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		SafeSetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

		SafeSetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE);

		D3DTestImage anImage(64,64);

		anImage.FillRect(0,0,64,64,0xFF0000FF);
		anImage.DrawToDevice(mD3DDevice7, mTextureSurface, 0, 0); 

		anImage.FillRect(0,0,64,64,0xFFFF0000);
		anImage.DrawToDevice(mD3DDevice7, mTextureSurface, 0, 0, 0x80FFFFFF); 

		mD3DDevice7->EndScene();

		CopyPrimaryToTestImage();
	}
	catch(TestException &ex)
	{
		mD3DDevice7->EndScene();
		return Fail(ex.mMsg);
	}

	int aNumErrors = 0;
	bool isUniform = mTestImage.IsUniformColor(0x7f007f,aNumErrors,10,10);
	if(aNumErrors==0)
	{
		if (isUniform)
			return true;
		else
			return Warn("AlphaModulated blend test not uniform.");
	}
	else
		return Fail("AlphaModulated blend not supported.");
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTester::TestClipProblem()
{
	try
	{
		CheckDXError(mD3DDevice7->BeginScene());
		CheckDXError(mD3DDevice7->Clear(0, NULL, D3DCLEAR_TARGET ,0xff000000, 1.0f, 0L),"Clear");

		SafeSetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE,TRUE);
		SafeSetRenderState(D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA);
		SafeSetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
		SafeSetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);

		D3DTestImage anImage(64,64);
		anImage.MakeVerticalBands();
		anImage.DrawToDevice(mD3DDevice7, mTextureSurface, -2, -2); 

		mD3DDevice7->EndScene();

		CopyPrimaryToTestImage();
	}
	catch(TestException &ex)
	{
		mD3DDevice7->EndScene();
		return Fail(ex.mMsg);
	}

	int aNumErrors = mTestImage.CheckUniformBands(62,62);
	if (aNumErrors==0)
		return true;
	else 
		return Warn("Clip problem detected.");
}

#undef SafeSetRenderState
#undef SafeSetTextureStageState

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
bool D3DTester::DoTest()
{
	if (!TestAlphaBlend())
		return false;

	if (!TestAdditiveBlend())
		return false;

	if (!TestAlphaAddBlend())
		return false;

	if (!TestAlphaModulate())
		return false;

	if (!TestClipProblem())
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTester::SetVidMemoryConstraints(DWORD theMin, DWORD theRecommended)
{
	mMinVidMemory = theMin;
	mRecommendedVidMemory = theRecommended;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void D3DTester::TestD3D(HWND theHWND, LPDIRECTDRAW7 theDDraw)
{
	if (!gSexyAppBase->mPlayingDemoBuffer)
	{
		mShouldWriteToRegistry = false;
		if (Init(theHWND, theDDraw))
		{
			DoTest();
		}
		else
		{
			if ((mCheckRegistry) && (!mShouldWriteToRegistry))
			{
				mResultsChanged = RegQueryValueExA(mRegKey, "FailureReason", 0, NULL, NULL, NULL) != ERROR_SUCCESS;
				mShouldWriteToRegistry = true;
			}
		}

		if (mShouldWriteToRegistry)
			WriteToRegistry();

		Cleanup();
	}

	gSexyAppBase->DemoSyncString(&mFailureReason);
	gSexyAppBase->DemoSyncString(&mWarning);
	gSexyAppBase->DemoSyncBool(&mResultsChanged);
}


