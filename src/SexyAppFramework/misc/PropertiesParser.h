#ifndef __SEXYAPPFRAMEWORK_NO_PROPERTIES_PARSER__
#ifndef __PROPERTIESPARSER_H__
#define __PROPERTIESPARSER_H__

#include "SexyAppBase.h"

namespace Sexy
{

class XMLParser;

class PropertiesParser
{
public:
	SexyAppBase*			mApp;
	XMLParser*				mXMLParser;
	SexyString				mError;
	bool					mHasFailed;

protected:
	void					Fail(const SexyString& theErrorText);

	bool					ParseSingleElement(SexyString* theString);
	bool					ParseStringArray(StringVector* theStringVector);
	bool					ParseProperties();
	bool					DoParseProperties();

public:
	PropertiesParser(SexyAppBase* theApp);
	virtual ~PropertiesParser();

	bool					ParsePropertiesFile(const std::string& theFilename);
	bool					ParsePropertiesBuffer(const Buffer& theBuffer);
	SexyString				GetErrorText();
};

}

#endif //__PROPERTIESPARSER_H__

#endif // __SEXYAPPFRAMEWORK_NO_PROPERTIES_PARSER__