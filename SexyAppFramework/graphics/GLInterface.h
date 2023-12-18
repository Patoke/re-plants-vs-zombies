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

	public:
		GLInterface(SexyAppBase* theApp);
		virtual ~GLInterface();

		int						Init(HWND theWindow, bool IsWindowed);
	};

}

#endif //__GLINTERFACE_H__