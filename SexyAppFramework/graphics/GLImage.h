#ifndef __GLIMAGE_H__
#define __GLIMAGE_H__

#include "MemoryImage.h"
#include "GLInterface.h"
#include <ddraw.h>

namespace Sexy
{

	class GLInterface;
	class SysFont;

	class GLImage : public MemoryImage
	{
	protected:
		friend class			SysFont;

	public:
		GLInterface* mGLInterface;

	public:
		GLImage();
		GLImage(GLInterface* theGLInterface);
		virtual ~GLImage();
	};

}

#endif //__GLIMAGE_H__