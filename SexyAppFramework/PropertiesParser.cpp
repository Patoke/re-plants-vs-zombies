#include "PropertiesParser.h"
#include "XMLParser.h"
#include <stdlib.h>

using namespace Sexy;

PropertiesParser::PropertiesParser(SexyAppBase* theApp)
{
	mApp = theApp;
	mHasFailed = false;
	mXMLParser = NULL;
}

void PropertiesParser::Fail(const SexyString& theErrorText)
{
	if (!mHasFailed)
	{
		mHasFailed = true;
		int aLineNum = mXMLParser->GetCurrentLineNum();

		mError = theErrorText;
		if (aLineNum > 0) mError += StrFormat(_S(" on Line %d"), aLineNum);
		if (!mXMLParser->GetFileName().empty()) mError += StrFormat(_S(" in File '%s'"), mXMLParser->GetFileName().c_str());
	}
}


PropertiesParser::~PropertiesParser()
{
}


bool PropertiesParser::ParseSingleElement(SexyString* aString)
{
	*aString = _S("");

	for (;;)
	{
		XMLElement aXMLElement;
		if (!mXMLParser->NextElement(&aXMLElement))
			return false;
		
		if (aXMLElement.mType == XMLElement::TYPE_START)
		{
			Fail(_S("Unexpected Section: '") + aXMLElement.mValue + _S("'"));
			return false;			
		}
		else if (aXMLElement.mType == XMLElement::TYPE_ELEMENT)
		{
			*aString = aXMLElement.mValue;
		}		
		else if (aXMLElement.mType == XMLElement::TYPE_END)
		{
			return true;
		}
	}
}

bool PropertiesParser::ParseStringArray(StringVector* theStringVector)
{
	theStringVector->clear();

	for (;;)
	{
		XMLElement aXMLElement;
		if (!mXMLParser->NextElement(&aXMLElement))
			return false;
		
		if (aXMLElement.mType == XMLElement::TYPE_START)
		{
			if (aXMLElement.mValue == _S("String"))
			{
				SexyString aString;

				if (!ParseSingleElement(&aString))
					return false;

				theStringVector->push_back(SexyStringToStringFast(aString));
			}
			else
			{
				Fail(_S("Invalid Section '") + aXMLElement.mValue + _S("'"));
				return false;
			}
		}
		else if (aXMLElement.mType == XMLElement::TYPE_ELEMENT)
		{
			Fail(_S("Element Not Expected '") + aXMLElement.mValue + _S("'"));
			return false;
		}		
		else if (aXMLElement.mType == XMLElement::TYPE_END)
		{
			return true;
		}
	}
}


bool PropertiesParser::ParseProperties()
{
	for (;;)
	{
		XMLElement aXMLElement;
		if (!mXMLParser->NextElement(&aXMLElement))
			return false;
		
		if (aXMLElement.mType == XMLElement::TYPE_START)
		{
			if (aXMLElement.mValue == _S("String"))
			{				
				SexyString aDef;
				if (!ParseSingleElement(&aDef))
					return false;

				std::string anId = SexyStringToStringFast(aXMLElement.mAttributes[_S("id")]);
				mApp->SetString(anId, SexyStringToWStringFast(aDef));
			}
			else if (aXMLElement.mValue == _S("StringArray"))
			{
				StringVector aDef;

				if (!ParseStringArray(&aDef))
					return false;

				std::string anId = SexyStringToStringFast(aXMLElement.mAttributes[_S("id")]);

				mApp->mStringVectorProperties.insert(StringStringVectorMap::value_type(anId, aDef));
			}
			else if (aXMLElement.mValue == _S("Boolean"))
			{
				SexyString aVal;

				if (!ParseSingleElement(&aVal))
					return false;

				aVal = Upper(aVal);

				bool boolVal;
				if ((aVal == _S("1")) || (aVal == _S("YES")) || (aVal == _S("ON")) || (aVal == _S("TRUE")))
					boolVal = true;
				else if ((aVal == _S("0")) || (aVal == _S("NO")) || (aVal == _S("OFF")) || (aVal == _S("FALSE")))
					boolVal = false;
				else
				{
					Fail(_S("Invalid Boolean Value: '") + aVal + _S("'"));
					return false;
				}

				std::string anId = SexyStringToStringFast(aXMLElement.mAttributes[_S("id")]);

				mApp->SetBoolean(anId, boolVal);
			}
			else if (aXMLElement.mValue == _S("Integer"))
			{
				SexyString aVal;

				if (!ParseSingleElement(&aVal))
					return false;

				int anInt;
				if (!StringToInt(aVal, &anInt))
				{
					Fail(_S("Invalid Integer Value: '") + aVal + _S("'"));
					return false;
				}

				std::string anId = SexyStringToStringFast(aXMLElement.mAttributes[_S("id")]);

				mApp->SetInteger(anId, anInt);
			}
			else if (aXMLElement.mValue == _S("Double"))
			{
				SexyString aVal;

				if (!ParseSingleElement(&aVal))
					return false;

				double aDouble;
				if (!StringToDouble(aVal, &aDouble))
				{
					Fail(_S("Invalid Double Value: '") + aVal + _S("'"));
					return false;
				}

				std::string anId = SexyStringToStringFast(aXMLElement.mAttributes[_S("id")]);

				mApp->SetDouble(anId, aDouble);
			}
			else
			{
				Fail(_S("Invalid Section '") + aXMLElement.mValue + _S("'"));
				return false;
			}
		}
		else if (aXMLElement.mType == XMLElement::TYPE_ELEMENT)
		{
			Fail(_S("Element Not Expected '") + aXMLElement.mValue + _S("'"));
			return false;
		}		
		else if (aXMLElement.mType == XMLElement::TYPE_END)
		{
			return true;
		}
	}
}

bool PropertiesParser::DoParseProperties()
{
	if (!mXMLParser->HasFailed())
	{
		for (;;)
		{
			XMLElement aXMLElement;
			if (!mXMLParser->NextElement(&aXMLElement))
				break;

			if (aXMLElement.mType == XMLElement::TYPE_START)
			{
				if (aXMLElement.mValue == _S("Properties"))
				{
					if (!ParseProperties())
						break;
				}
				else 
				{
					Fail(_S("Invalid Section '") + aXMLElement.mValue + _S("'"));
					break;
				}
			}
			else if (aXMLElement.mType == XMLElement::TYPE_ELEMENT)
			{
				Fail(_S("Element Not Expected '") + aXMLElement.mValue + _S("'"));
				break;
			}
		}
	}

	if (mXMLParser->HasFailed())
		Fail(mXMLParser->GetErrorText());	

	delete mXMLParser;
	mXMLParser = NULL;

	return !mHasFailed;
}

bool PropertiesParser::ParsePropertiesBuffer(const Buffer& theBuffer)
{
	mXMLParser = new XMLParser();

	mXMLParser->SetStringSource(theBuffer.UTF8ToWideString());
	return DoParseProperties();
}

bool PropertiesParser::ParsePropertiesFile(const std::string& theFilename)
{
	mXMLParser = new XMLParser();
	mXMLParser->OpenFile(theFilename);
	return DoParseProperties();	
}

SexyString PropertiesParser::GetErrorText()
{
	return mError;
}