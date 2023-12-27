#include "SharedImage.h"
//#include "graphics/DDImage.h"
#include "SexyAppBase.h"

using namespace Sexy;

SharedImage::SharedImage()
{
	//mImage = NULL;
	mRefCount = 0;
}

SharedImageRef::SharedImageRef(const SharedImageRef& theSharedImageRef)
{
	mSharedImage = theSharedImageRef.mSharedImage;
	if (mSharedImage != NULL)
		mSharedImage->mRefCount++;
	mUnsharedImage = theSharedImageRef.mUnsharedImage;	
	mOwnsUnshared = false;
}

SharedImageRef::SharedImageRef()
{
	mSharedImage = NULL;
	mUnsharedImage = NULL;
	mOwnsUnshared = false;
}

SharedImageRef::SharedImageRef(SharedImage* theSharedImage)
{
	mSharedImage = theSharedImage;
	if (theSharedImage != NULL)
		mSharedImage->mRefCount++;

	mUnsharedImage = NULL;
	mOwnsUnshared = false;
}

SharedImageRef::~SharedImageRef()
{
	Release();
}

void SharedImageRef::Release()
{	
	if (mOwnsUnshared)
		delete mUnsharedImage;
	mUnsharedImage = NULL;
	if (mSharedImage != NULL)
	{
		if (--mSharedImage->mRefCount == 0)
			gSexyAppBase->mCleanupSharedImages = true;
	}
	mSharedImage = NULL;
}

SharedImageRef& SharedImageRef::operator=(const SharedImageRef& theSharedImageRef)
{
	Release();
	mSharedImage = theSharedImageRef.mSharedImage;
	if (mSharedImage != NULL)
		mSharedImage->mRefCount++;
	return *this;
}

SharedImageRef&	SharedImageRef::operator=(SharedImage* theSharedImage)
{
	Release();
	mSharedImage = theSharedImage;
	mSharedImage->mRefCount++;
	return *this;
}

SharedImageRef& SharedImageRef::operator=(MemoryImage* theUnsharedImage)
{
	Release();
	mUnsharedImage = theUnsharedImage;	
	return *this;
}

MemoryImage* SharedImageRef::operator->()
{
	return (MemoryImage*) *this;
}


SharedImageRef::operator Image*()
{
	return (MemoryImage*) *this;
}

SharedImageRef::operator MemoryImage*()
{
	if (mUnsharedImage != NULL)
		return mUnsharedImage;
	else
		unreachable();
		/* FIXME
		return (DDImage*) *this;
		*/
}

/*
SharedImageRef::operator DDImage*()
{
	if (mSharedImage != NULL)
		return mSharedImage->mImage;
	else
		return NULL;
}*/
