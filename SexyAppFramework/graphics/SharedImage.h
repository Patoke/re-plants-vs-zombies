#ifndef __SHARED_IMAGE_H__
#define __SHARED_IMAGE_H__

#include "Common.h"

namespace Sexy
{

class Image;
class GLImage;
class MemoryImage;

class SharedImage
{
public:
	GLImage*				mImage;
	int						mRefCount;		

	SharedImage();
};

typedef std::map<std::pair<std::string, std::string>, SharedImage> SharedImageMap;

class SharedImageRef
{
public:
	SharedImage*			mSharedImage;
	MemoryImage*			mUnsharedImage;
	bool					mOwnsUnshared;

public:
	SharedImageRef();
	SharedImageRef(const SharedImageRef& theSharedImageRef);
	SharedImageRef(SharedImage* theSharedImage);
	~SharedImageRef();

	void					Release();

	SharedImageRef&			operator=(const SharedImageRef& theSharedImageRef);
	SharedImageRef&			operator=(SharedImage* theSharedImage);
	SharedImageRef&			operator=(MemoryImage* theUnsharedImage);
	MemoryImage*			operator->();
	operator Image*();
	operator MemoryImage*();
	operator GLImage*();
};

}

#endif //__SHARED_IMAGE_H__