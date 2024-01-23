#include "XMLParser.h"
#include "Debug.h"
#include "paklib/PakInterface.h"

using namespace Sexy;

XMLParser::XMLParser()
{
	mFile = NULL;
	mLineNum = 0;
	mAllowComments = false;
	mGetCharFunc = &XMLParser::GetUTF8Char;
	mForcedEncodingType = false;
}

XMLParser::~XMLParser()
{
	if (mFile != NULL)
		p_fclose(mFile);
}

void XMLParser::SetEncodingType(XMLEncodingType theEncoding)
{
	switch (theEncoding)
	{
		case ASCII:		mGetCharFunc = &XMLParser::GetAsciiChar;	mForcedEncodingType = true; break;
		case UTF_8:		mGetCharFunc = &XMLParser::GetUTF8Char;		mForcedEncodingType = true; break;
		case UTF_16:	mGetCharFunc = &XMLParser::GetUTF16Char;	mForcedEncodingType = true; break;
		case UTF_16_LE:	mGetCharFunc = &XMLParser::GetUTF16LEChar;	mForcedEncodingType = true; break;
		case UTF_16_BE:	mGetCharFunc = &XMLParser::GetUTF16BEChar;	mForcedEncodingType = true; break;
	}
}

void XMLParser::Fail(const SexyString& theErrorText)
{
	mHasFailed = true;
	mErrorText = theErrorText;
}

void XMLParser::Init()
{
	mSection = _S("");
	mLineNum = 1;
	mHasFailed = false;
	mErrorText = _S("");
	mFirstChar = true;
	mByteSwap = false;
}

bool XMLParser::AddAttribute(XMLElement* theElement, const SexyString& theAttributeKey, const SexyString& theAttributeValue)
{
	std::pair<XMLParamMap::iterator,bool> aRet;

	aRet = theElement->mAttributes.insert(XMLParamMap::value_type(theAttributeKey, theAttributeValue));
	if (!aRet.second)
		aRet.first->second = theAttributeValue;

	if (theAttributeKey != _S("/"))
		theElement->mAttributeIteratorList.push_back(aRet.first);

	return aRet.second;
}

bool XMLParser::GetAsciiChar(wchar_t* theChar, bool* error)
{
	(void)error;
	wchar_t aChar = 0;
	if (p_fread(&aChar, 1, 1, mFile) != 1)
		return false;

	*theChar = aChar;
	return true;
}

bool XMLParser::GetUTF8Char(wchar_t* theChar, bool* error)
{
	static const unsigned short aMaskData[] = {
		0xC0,		// 1 extra byte
		0xE0,		// 2 extra bytes
		0xF0,		// 3 extra bytes
		0xF8,		// 4 extra bytes
		0xFC		// 5 extra bytes
	};
	*error = true;

	int aTempChar = 0;
	if (p_fread(&aTempChar, 1, 1, mFile) == 1)
	{
		if ((aTempChar & 0x80) != 0)
		{
			if ((aTempChar & 0xC0) != 0xC0) return false; // sanity check: high bit should not be set without the next highest bit being set too.

			int aBytesRead[6];
			int* aBytesReadPtr = &aBytesRead[0];

			*aBytesReadPtr++ = aTempChar;

			int aLen;
			for (aLen = 0; aLen < (int)(sizeof(aMaskData)/sizeof(*aMaskData)); ++aLen)
			{
				if ( (aTempChar & aMaskData[aLen]) == ((aMaskData[aLen] << 1) & aMaskData[aLen]) ) break;
			}
			if (aLen >= (int)(sizeof(aMaskData)/sizeof(*aMaskData))) return false;

			aTempChar &= ~aMaskData[aLen];
			int aTotalLen = aLen+1;

			DBG_ASSERTE(aTotalLen >= 2 && aTotalLen <= 6);

			int anExtraChar = 0;
			while (aLen > 0)
			{
				if (p_fread(&anExtraChar, 1, 1, mFile) != 1) return false;
				if ((anExtraChar & 0xC0) != 0x80) return false; // sanity check: high bit set, and next highest bit NOT set.

				*aBytesReadPtr++ = anExtraChar;

				aTempChar = (aTempChar << 6) | (anExtraChar & 0x3F);
				--aLen;
			}

			// validate substrings
			bool valid = true;
			switch (aTotalLen)
			{
				case 2:
					valid = !((aBytesRead[0] & 0x3E) == 0);
					break;
				case 3:
					valid = !((aBytesRead[0] & 0x1F) == 0 && (aBytesRead[1] & 0x20) == 0);
					break;
				case 4:
					valid = !((aBytesRead[0] & 0x0F) == 0 && (aBytesRead[1] & 0x30) == 0);
					break;
				case 5:
					valid = !((aBytesRead[0] & 0x07) == 0 && (aBytesRead[1] & 0x38) == 0);
					break;
				case 6:
					valid = !((aBytesRead[0] & 0x03) == 0 && (aBytesRead[1] & 0x3C) == 0);
					break;
			}
			if (!valid) return false;
		}

		if ( (aTempChar >= 0xD800 && aTempChar <= 0xDFFF) || (aTempChar >= 0xFFFE && aTempChar <= 0xFFFF) ) return false;

		if (aTempChar == 0xFEFF && mFirstChar) // zero-width non breaking space as the first char is a byte order marker.
		{
			mFirstChar = false;
			return GetUTF8Char(theChar, error);
		}

		*theChar = (wchar_t)aTempChar;
		*error = false;
		return true;		
	}

	*error = false;
	return false;
}

bool XMLParser::GetUTF16Char(wchar_t* theChar, bool* error)
{
	wchar_t aTempChar = 0;
	if (p_fread(&aTempChar, 2, 1, mFile) != 1)
		return false;

	if (mFirstChar)
	{
		mFirstChar = false;
		if (aTempChar == 0xFEFF)
		{
			mByteSwap = false;
			return GetUTF16Char(theChar, error);
		}
		else if (aTempChar == 0xFFFE)
		{
			mByteSwap = true;
			return GetUTF16Char(theChar, error);
		}
	}
	if (mByteSwap) aTempChar = (wchar_t)((aTempChar << 8) | (aTempChar >> 8));

	if ((aTempChar & 0xD800) == 0xD800)
	{
		wchar_t aNextChar = 0;
		if (p_fread(&aNextChar, 2, 1, mFile) != 1)
			return false;

		if (mByteSwap) aNextChar = (wchar_t)((aNextChar << 8) | (aNextChar >> 8));
		if ((aNextChar & 0xDC00) == 0xDC00)
		{
			*theChar = (wchar_t)((((aTempChar & ~0xD800) << 10) | (aNextChar & ~0xDC00)) + 0x10000);
		}
		else return false;
	}
	else *theChar = aTempChar;

	return true;
}

bool XMLParser::GetUTF16LEChar(wchar_t* theChar, bool* error)
{
	(void)error;
	wchar_t aTempChar = 0;
	if (p_fread(&aTempChar, 2, 1, mFile) != 1)
		return false;

	aTempChar = WORD_LITTLEE_TO_NATIVE(aTempChar);

	if ((aTempChar & 0xD800) == 0xD800)
	{
		wchar_t aNextChar = 0;
		if (p_fread(&aNextChar, 2, 1, mFile) != 1)
			return false;

		aNextChar = WORD_LITTLEE_TO_NATIVE(aTempChar);
		if ((aNextChar & 0xDC00) == 0xDC00)
		{
			*theChar = (wchar_t)((((aTempChar & ~0xD800) << 10) | (aNextChar & ~0xDC00)) + 0x10000);
		}
		else return false;
	}

	return true;
}

bool XMLParser::GetUTF16BEChar(wchar_t* theChar, bool* error)
{
	(void)error;
	wchar_t aTempChar = 0;
	if (p_fread(&aTempChar, 2, 1, mFile) != 1)
		return false;

	aTempChar = WORD_BIGE_TO_NATIVE(aTempChar);

	if ((aTempChar & 0xD800) == 0xD800)
	{
		wchar_t aNextChar = 0;
		if (p_fread(&aNextChar, 2, 1, mFile) != 1)
			return false;

		aNextChar = WORD_BIGE_TO_NATIVE(aTempChar);
		if ((aNextChar & 0xDC00) == 0xDC00)
		{
			*theChar = (wchar_t)((((aTempChar & ~0xD800) << 10) | (aNextChar & ~0xDC00)) + 0x10000);
		}
		else return false;
	}

	return true;
}

bool XMLParser::OpenFile(const std::string& theFileName)
{		
	mFile = p_fopen(theFileName.c_str(), "r");

	if (mFile == NULL)
	{
		mLineNum = 0;
		Fail(StringToSexyString("Unable to open file " + theFileName));
		return false;
	}
	else if (!mForcedEncodingType)
	{
		p_fseek(mFile, 0, SEEK_END);
		long aFileLen = p_ftell(mFile);
		p_fseek(mFile, 0, SEEK_SET);

		mGetCharFunc = &XMLParser::GetAsciiChar;
		if (aFileLen >= 2) // UTF-16?
		{
			int aChar1 = p_fgetc(mFile);
			int aChar2 = p_fgetc(mFile);

			if ( (aChar1 == 0xFF && aChar2 == 0xFE) || (aChar1 == 0xFE && aChar2 == 0xFF) )
				mGetCharFunc = &XMLParser::GetUTF16Char;

			p_ungetc(aChar2, mFile);
			p_ungetc(aChar1, mFile);			
		}
		if ((mGetCharFunc = &XMLParser::GetAsciiChar))
		{
			if (aFileLen >= 3) // UTF-8?
			{
				int aChar1 = p_fgetc(mFile);
				int aChar2 = p_fgetc(mFile);
				int aChar3 = p_fgetc(mFile);

				if (aChar1 == 0xEF && aChar2 == 0xBB && aChar3 == 0xBF)
					mGetCharFunc = &XMLParser::GetUTF8Char;

				p_ungetc(aChar3, mFile);
				p_ungetc(aChar2, mFile);
				p_ungetc(aChar1, mFile);			
			}
		}
	}

	mFileName = theFileName.c_str();
	Init();
	return true;
}

void XMLParser::SetStringSource(const std::wstring& theString)
{
	Init();

	int aSize = theString.size();

	mBufferedText.resize(aSize);	
	for (int i = 0; i < aSize; i++)
		mBufferedText[i] = theString[aSize - i - 1];	
}

void XMLParser::SetStringSource(const std::string& theString)
{
	SetStringSource(StringToWString(theString));
}

bool XMLParser::NextElement(XMLElement* theElement)
{
	for (;;)
	{		
		theElement->mType = XMLElement::TYPE_NONE;
		theElement->mSection = mSection;
		theElement->mValue = _S("");
		theElement->mAttributes.clear();			
		theElement->mInstruction.erase();

		bool hasSpace = false;	
		bool inQuote = false;
		bool gotEndQuote = false;

		bool doingAttribute = false;
		bool AttributeVal = false;
		std::wstring aAttributeKey;
		std::wstring aAttributeValue;

		std::wstring aLastAttributeKey;
		
		for (;;)
		{
			// Process character by character

			wchar_t c;
			int aVal;
			
			if (mBufferedText.size() > 0)
			{								
				c = mBufferedText[mBufferedText.size()-1];
				mBufferedText.pop_back();				

				aVal = 1;
			}
			else
			{
				if (mFile != NULL)
				{
					bool error = false;
					if ((this->*mGetCharFunc)(&c, &error))
					{
						aVal = 1;
					}
					else
					{
						if (error) Fail(_S("Illegal Character"));
						aVal = 0;
					}
				}
				else
				{
					aVal = 0;
				}
			}
			
			if (aVal == 1)
			{
				bool processChar = false;

				if (c == L'\n')
				{
					mLineNum++;
				}

				if (theElement->mType == XMLElement::TYPE_COMMENT)
				{
					// Just add text to theElement->mInstruction until we find -->

					SexyString* aStrPtr = &theElement->mInstruction;
					
					*aStrPtr += (SexyChar)c;					

					int aLen = aStrPtr->length();

					if ((c == L'>') && (aLen >= 3) && ((*aStrPtr)[aLen - 2] == L'-') && ((*aStrPtr)[aLen - 3] == L'-'))
					{
						*aStrPtr = aStrPtr->substr(0, aLen - 3);
						break;
					}
				}
				else if (theElement->mType == XMLElement::TYPE_INSTRUCTION)
				{
					// Just add text to theElement->mInstruction until we find ?>

					SexyString* aStrPtr = &theElement->mValue;

					if ((theElement->mInstruction.length() != 0) || (::iswspace(c)))
						aStrPtr = &theElement->mInstruction;
					
					*aStrPtr += (SexyChar)c;					

					int aLen = aStrPtr->length();

					if ((c == L'>') && (aLen >= 2) && ((*aStrPtr)[aLen - 2] == L'?'))
					{
						*aStrPtr = aStrPtr->substr(0, aLen - 2);
						break;
					}
				}
				else
				{
					if (c == L'"')
					{
						inQuote = !inQuote;
						if (theElement->mType==XMLElement::TYPE_NONE || theElement->mType==XMLElement::TYPE_ELEMENT)
							processChar = true;

						if (!inQuote)
							gotEndQuote = true;
					}
					else if (!inQuote)
					{
						if (c == L'<')
						{
							if (theElement->mType == XMLElement::TYPE_ELEMENT)
							{
								//TODO: Fix buffered text.  Not sure what I meant by that.

								//OLD: mBufferedText = c + mBufferedText;

								mBufferedText.push_back(c);								
								break;
							}

							if (theElement->mType == XMLElement::TYPE_NONE)
							{
								theElement->mType = XMLElement::TYPE_START;
							}
							else
							{
								Fail(_S("Unexpected '<'"));
								return false;
							}
						}
						else if (c == L'>')
						{
							if (theElement->mType == XMLElement::TYPE_START)
							{	
								bool insertEnd = false;

								if (aAttributeKey == L"/")
								{
									// We will get this if we have a space before the />, so we can ignore it
									//  and go about our business now
									insertEnd = true;
								}
								else
								{
									// Probably isn't committed yet
									if (aAttributeKey.length() > 0)
									{										
//										theElement->mAttributes[aLastAttributeKey] = aAttributeValue;

										aAttributeKey = XMLDecodeString(aAttributeKey);
										aAttributeValue = XMLDecodeString(aAttributeValue);

										aLastAttributeKey = aAttributeKey;
										AddAttribute(theElement, WStringToSexyString(aLastAttributeKey), WStringToSexyString(aAttributeValue));

										aAttributeKey = L"";
										aAttributeValue = L"";
									}

									if (aLastAttributeKey.length() > 0)
									{
										SexyString aVal = theElement->mAttributes[WStringToSexyString(aLastAttributeKey)];

										int aLen = aVal.length();

										if ((aLen > 0) && (aVal[aLen-1] == '/'))
										{
											// Its an empty element, fake start and end segments
//											theElement->mAttributes[aLastAttributeKey] = aVal.substr(0, aLen - 1);
											
											AddAttribute(theElement, WStringToSexyString(aLastAttributeKey), XMLDecodeString(aVal.substr(0, aLen - 1)));

											insertEnd = true;
										}
									}
									else
									{
										int aLen = theElement->mValue.length();

										if ((aLen > 0) && (theElement->mValue[aLen-1] == '/'))
										{
											// Its an empty element, fake start and end segments
											theElement->mValue = theElement->mValue.substr(0, aLen - 1);
											insertEnd = true;
										}
									}
								}

								// Do we want to fake an ending section?
								if (insertEnd)
								{									
									SexyString anAddString = _S("</") + theElement->mValue + _S(">");

									int anOldSize = mBufferedText.size();
									int anAddLength = anAddString.length();

									mBufferedText.resize(anOldSize + anAddLength);

									for (int i = 0; i < anAddLength; i++)
										mBufferedText[anOldSize + i] = (wchar_t)(anAddString[anAddLength - i - 1]);

									// clear out aAttributeKey, since it contains "/" as its value and will insert
									// it into the element's attribute map.
									aAttributeKey = L"";

									//OLD: mBufferedText = "</" + theElement->mValue + ">" + mBufferedText;
								}

								if (mSection.length() != 0)
									mSection += _S("/");

								mSection += theElement->mValue;								

								break;
							}
							else if (theElement->mType == XMLElement::TYPE_END)
							{
								int aLastSlash = mSection.rfind(_S('/'));
								if ((aLastSlash == -1) && (mSection.length() == 0))
								{
									Fail(_S("Unexpected End"));
									return false;
								}

								SexyString aLastSectionName = mSection.substr(aLastSlash + 1);
								
								if (aLastSectionName != theElement->mValue)
								{
									Fail(_S("End '") + theElement->mValue + _S("' Doesn't Match Start '") + aLastSectionName + _S("'"));
									return false;
								}

								if (aLastSlash == -1)
									mSection.erase(mSection.begin(), mSection.end());
								else
									mSection.erase(mSection.begin() + aLastSlash, mSection.end());

								break;
							}
							else
							{
								Fail(_S("Unexpected '>'"));
								return false;
							}
						}
						else if ((c == L'/') && (theElement->mType == XMLElement::TYPE_START) && (theElement->mValue == _S("")))
						{					
							theElement->mType = XMLElement::TYPE_END;					
						}				
						else if ((c == L'?') && (theElement->mType == XMLElement::TYPE_START) && (theElement->mValue == _S("")))
						{
							theElement->mType = XMLElement::TYPE_INSTRUCTION;
						}
						else if (::isspace((uchar) c))
						{
							if (theElement->mValue != _S(""))
								hasSpace = true;

							// It's a comment!
							if ((theElement->mType == XMLElement::TYPE_START) && (theElement->mValue == _S("!--")))
								theElement->mType = XMLElement::TYPE_COMMENT;
						}
						else if (c > 32)
						{
							processChar = true;
						}
						else
						{
							Fail(_S("Illegal Character"));
							return false;
						}
					} 
					else
					{
						processChar = true;
					}

					if (processChar)
					{
						if (theElement->mType == XMLElement::TYPE_NONE)
							theElement->mType = XMLElement::TYPE_ELEMENT;

						if (theElement->mType == XMLElement::TYPE_START)
						{
							if (hasSpace)
							{
								if ((!doingAttribute) || ((!AttributeVal) && (c != _S('='))) ||
									((AttributeVal) && ((aAttributeValue.length() > 0) || gotEndQuote)))
								{
									if (doingAttribute)
									{
										aAttributeKey = XMLDecodeString(aAttributeKey);
										aAttributeValue = XMLDecodeString(aAttributeValue);

//										theElement->mAttributes[aAttributeKey] = aAttributeValue;

										AddAttribute(theElement, WStringToSexyString(aAttributeKey), WStringToSexyString(aAttributeValue));

										aAttributeKey = L"";
										aAttributeValue = L"";

										aLastAttributeKey = aAttributeKey;
									}
									else
									{
										doingAttribute = true;
									}
																
									AttributeVal = false;
								}

								hasSpace = false;
							}

							std::wstring* aStrPtr = NULL;

							if (!doingAttribute)
							{
								theElement->mValue += (SexyChar)c;
							}
							else
							{
								if (c == L'=')
								{
									AttributeVal = true;
									gotEndQuote = false;
								}
								else
								{
									if (!AttributeVal)
										aStrPtr = &aAttributeKey;
									else
										aStrPtr = &aAttributeValue;
								}
							}

							if (aStrPtr != NULL)
							{								
								*aStrPtr += c;						
							}
						}
						else
						{
							if (hasSpace)
							{
								theElement->mValue += _S(" ");
								hasSpace = false;
							}
							
							theElement->mValue += (SexyChar)c;
						}
					}
				}
			}
			else
			{
				if (theElement->mType != XMLElement::TYPE_NONE)
					Fail(_S("Unexpected End of File"));
					
				return false;
			}			
		}		

		if (aAttributeKey.length() > 0)
		{
			aAttributeKey = XMLDecodeString(aAttributeKey);
			aAttributeValue = XMLDecodeString(aAttributeValue);
//			theElement->mAttributes[aAttributeKey] = aAttributeValue;

			AddAttribute(theElement, WStringToSexyString(aAttributeKey), WStringToSexyString(aAttributeValue));
		}

		theElement->mValue = XMLDecodeString(theElement->mValue);				

		// Ignore comments
		if ((theElement->mType != XMLElement::TYPE_COMMENT) || mAllowComments)
			return true;
	}
}

bool XMLParser::HasFailed()
{
	return mHasFailed;
}

SexyString XMLParser::GetErrorText()
{
	return mErrorText;
}

int XMLParser::GetCurrentLineNum()
{
	return mLineNum;
}

std::string XMLParser::GetFileName()
{
	return mFileName;
}
