#ifndef __SEXY_RESOURCEMANAGER_H__
#define __SEXY_RESOURCEMANAGER_H__

#include "Common.h"
#include "graphics/Image.h"
#include "SexyAppBase.h"
#include <string>
#include <map>

namespace ImageLib
{
class Image;
};

namespace Sexy
{

class XMLParser;
class XMLElement;
class Image;
class SoundInstance;
class SexyAppBase;
class _Font;

typedef std::map<std::string, std::string>	StringToStringMap;
typedef std::map<SexyString, SexyString>	XMLParamMap;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class ResourceManager
{
public: // @Patoke todo: revert to protected
	enum ResType
	{
		ResType_Image,
		ResType_Sound,
		ResType_Font
	};


	struct BaseRes
	{
		ResType mType;
		std::string mId;
		std::string mResGroup;
		std::string mPath;
		XMLParamMap mXMLAttributes;
		bool mFromProgram;

		virtual ~BaseRes() {}
		virtual void DeleteResource() { }
	};

	struct ImageRes : public BaseRes
	{
		SharedImageRef mImage;
		std::string mAlphaImage;
		std::string mAlphaGridImage;
		std::string mVariant;
		bool mAutoFindAlpha;
		bool mPalletize;
		bool mA4R4G4B4;
		bool mA8R8G8B8;
		bool mDDSurface;
		bool mPurgeBits;
		bool mMinimizeSubdivisions;
		int mRows;
		int mCols;
		DWORD mAlphaColor;
		AnimInfo mAnimInfo;

		ImageRes() { mType = ResType_Image; }
		virtual void DeleteResource();
	};

	struct SoundRes : public BaseRes
	{
		int mSoundId;
		double mVolume;
		int mPanning;

		SoundRes() { mType = ResType_Sound; }
		virtual void DeleteResource();
	};

	struct FontRes : public BaseRes
	{
		_Font *mFont;
		Image *mImage;
		std::string mImagePath;
		std::string mTags;

		// For SysFonts
		bool mSysFont;
		bool mBold;
		bool mItalic;
		bool mUnderline;
		bool mShadow;
		int mSize;


		FontRes() { mType = ResType_Font; }
		virtual void DeleteResource();
	};

	typedef std::map<std::string,BaseRes*> ResMap;
	typedef std::list<BaseRes*> ResList;
	typedef std::map<std::string,ResList,StringLessNoCase> ResGroupMap;

	std::set<std::string,StringLessNoCase> mLoadedGroups;

	ResMap					mImageMap;
	ResMap					mSoundMap;
	ResMap					mFontMap;

	XMLParser*				mXMLParser;
	std::string				mError;
	bool					mHasFailed;
	SexyAppBase*			mApp;
	std::string				mCurResGroup;
	std::string				mDefaultPath;
	std::string				mDefaultIdPrefix;
	bool					mAllowMissingProgramResources;
	bool					mAllowAlreadyDefinedResources; // for reparsing file while running
	bool					mHadAlreadyDefinedError;

	ResGroupMap				mResGroupMap;
	ResList*				mCurResGroupList;
	ResList::iterator		mCurResGroupListItr;


	bool					Fail(const std::string& theErrorText);

	virtual bool			ParseCommonResource(XMLElement &theElement, BaseRes *theRes, ResMap &theMap);
	virtual bool			ParseSoundResource(XMLElement &theElement);
	virtual bool			ParseImageResource(XMLElement &theElement);
	virtual bool			ParseFontResource(XMLElement &theElement);
	virtual bool			ParseSetDefaults(XMLElement &theElement);
	virtual bool			ParseResources();

	bool					DoParseResources();
	void					DeleteMap(ResMap &theMap);
	virtual void			DeleteResources(ResMap &theMap, const std::string &theGroup);

//	bool					LoadAlphaGridImage(ImageRes *theRes, DDImage *theImage);
//	bool					LoadAlphaImage(ImageRes *theRes, DDImage *theImage);
	virtual bool			DoLoadImage(ImageRes *theRes);
	virtual bool			DoLoadFont(FontRes* theRes);
	virtual bool			DoLoadSound(SoundRes* theRes);

	int						GetNumResources(const std::string &theGroup, ResMap &theMap);

public:
	ResourceManager(SexyAppBase *theApp);
	virtual ~ResourceManager();

	bool					ParseResourcesFile(const std::string& theFilename);
	bool					ReparseResourcesFile(const std::string& theFilename);

	std::string				GetErrorText();
	bool					HadError();
	bool					IsGroupLoaded(const std::string &theGroup);

	int						GetNumImages(const std::string &theGroup);
	int						GetNumSounds(const std::string &theGroup);
	int						GetNumFonts(const std::string &theGroup);
	int						GetNumResources(const std::string &theGroup);

	virtual bool			LoadNextResource();
	virtual void			ResourceLoadedHook(BaseRes *theRes);

	virtual void			StartLoadResources(const std::string &theGroup);
	virtual bool			LoadResources(const std::string &theGroup);

	bool					ReplaceImage(const std::string &theId, Image *theImage);
	bool					ReplaceSound(const std::string &theId, int theSound);
	bool					ReplaceFont(const std::string &theId, _Font *theFont);

	void					DeleteImage(const std::string &theName);
	SharedImageRef			LoadImage(const std::string &theName);

	void					DeleteFont(const std::string &theName);
	_Font*					LoadFont(const std::string &theName);

	SharedImageRef			GetImage(const std::string &theId);
	int						GetSound(const std::string &theId);
	_Font*					GetFont(const std::string &theId);
	
	// Returns all the XML attributes associated with the image
	const XMLParamMap&		GetImageAttributes(const std::string &theId);

	// These throw a ResourceManagerException if the resource is not found
	virtual SharedImageRef	GetImageThrow(const std::string &theId);
	virtual int				GetSoundThrow(const std::string &theId);
	virtual _Font*			GetFontThrow(const std::string &theId);

	void					SetAllowMissingProgramImages(bool allow);

	virtual void			DeleteResources(const std::string &theGroup);
	void					DeleteExtraImageBuffers(const std::string &theGroup);

	const ResList*			GetCurResGroupList()	{return mCurResGroupList;}
	std::string				GetCurResGroup()		{return mCurResGroup;}
	void					DumpCurResGroup(std::string& theDestStr);
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct ResourceManagerException : public std::exception
{
	std::string what;
	ResourceManagerException(const std::string &theWhat) : what(theWhat) { }
};

}

#endif //__PROPERTIESPARSER_H__


