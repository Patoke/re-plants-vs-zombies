#ifndef __D3DTESTER_H__
#define __D3DTESTER_H__
#include <d3d.h>
#include <exception>
#include <string>

namespace Sexy
{

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class D3DTestImage
{
protected:
	DWORD *mBits;
	int mWidth;
	int mHeight;

	void FreeImage();

public:
	D3DTestImage();
	D3DTestImage(int theWidth, int theHeight);
	virtual ~D3DTestImage();

	void Create(int theWidth, int theHeight);

	const D3DTestImage& operator=(const D3DTestImage &theImage);

	void MakeVerticalBands();
	void FillRect(int x, int y, int theWidth, int theHeight, DWORD theColor);
	void CopyToTexture8888(LPDIRECTDRAWSURFACE7 theTexture, int offx, int offy, int texWidth, int texHeight);
	void CopyToTexture4444(LPDIRECTDRAWSURFACE7 theTexture, int offx, int offy, int texWidth, int texHeight);
	void DrawPieceToDevice(LPDIRECT3DDEVICE7 theDevice, LPDIRECTDRAWSURFACE7 theTexture, float x, float y, int offx, int offy, int texWidth, int texHeight, DWORD theColor);
	void DrawToDevice(LPDIRECT3DDEVICE7 theDevice, LPDIRECTDRAWSURFACE7 theTexture, int x, int y, DWORD theColor = 0xffffffff);
	bool CompareEqual(const D3DTestImage &theImage) const;

	DWORD* GetBits() { return mBits; }
	const DWORD* GetBits() const { return mBits; }

	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }

	enum { COLOR_TOLERANCE = 16*16*3 };
	static int ColorDistance(DWORD c1, DWORD c2);
	bool IsUniformColor(DWORD theColor, int &theNumMistakes, int testWidth, int testHeight) const;
	int CheckUniformBands(int testWidth, int testHeight, int xoff = 0, int yoff = 0);
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class D3DTester
{
protected:
	LPDIRECTDRAW7			mDD7;
	LPDIRECTDRAWSURFACE7	mPrimarySurface;
	LPDIRECTDRAWSURFACE7	mTextureSurface;
	LPDIRECTDRAWSURFACE7	mTextureSurface2;
	LPDIRECT3D7				mD3D7;
	LPDIRECT3DDEVICE7		mD3DDevice7;
	HKEY					mRegKey;
	GUID					mDisplayGUID;
	std::string				mDisplayDriver;
	std::string				mDisplayDescription;

	enum { TEST_VERSION = 2 };

	D3DTestImage mTestImage;

	struct TestException : public std::exception
	{
		std::string mMsg;
		TestException(const std::string &theMsg) : mMsg(theMsg) { }
	};

	static HRESULT CALLBACK PixelFormatsCallback(LPDDPIXELFORMAT theFormat, LPVOID lpContext);

	void CopyPrimaryToTestImage();
	bool TestAlphaBlend();
	bool TestAdditiveBlend();
	bool TestAlphaAddBlend();
	bool TestAlphaModulate();
	bool TestClipProblem();

	std::string mFailureReason;
	std::string mWarning;

	DWORD mMinVidMemory;
	DWORD mRecommendedVidMemory;
	bool mCheckRegistry;
	bool mShouldWriteToRegistry;
	bool mResultsChanged;
	int mDriverYear;

	bool Fail(const std::string &theStr);
	bool Warn(const std::string &theStr);

	bool Init(HWND theHWND, LPDIRECTDRAW7 theDDraw);
	bool FileContains(FILE* theFile, const char* theString);
	bool IsSupportedCard(const char *theDisplayDesc);
	void Cleanup();
	bool DoTest();

	bool CheckRegistry();
	void WriteToRegistry();

public:
	D3DTester();
	virtual ~D3DTester();

	static void CheckDXError(HRESULT theResult, const char *theMsg = "");

	bool HadWarning() { return !mWarning.empty(); }
	bool HadError() { return !mFailureReason.empty(); }
	bool Is3DSupported() { return !HadError(); }
	bool Is3DRecommended() { return !HadError() && !HadWarning(); }
	std::string GetWarning() { return mWarning; }
	std::string GetError() { return mFailureReason; }
	bool ResultsChanged() { return mResultsChanged; }

	void SetVidMemoryConstraints(DWORD theMin, DWORD theRecommended);

	void TestD3D(HWND theHWND, LPDIRECTDRAW7 theDDraw);
};

} // namespace Sexy


#endif