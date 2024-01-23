#include "ImageFont.h"
#include "Common.h"
#include "Graphics.h"
#include "Image.h"
#include "SexyAppBase.h"
#include <queue>
#include <vector>
//#include "MemoryImage.h"
//#include "graphics/DDImage.h"

using namespace Sexy;

////

DataElement::DataElement() :
	mIsList(false)
{
}

DataElement::~DataElement()
{
}

SingleDataElement::SingleDataElement()
{
	mIsList = false;
}

SingleDataElement::SingleDataElement(const std::string theString) :
	mString(theString)
{
	mIsList = false;
}

SingleDataElement::~SingleDataElement()
{
}

DataElement* SingleDataElement::Duplicate()
{
	SingleDataElement* aSingleDataElement = new SingleDataElement(*this);
	return aSingleDataElement;
}

ListDataElement::ListDataElement()
{
	mIsList = true;
}

ListDataElement::~ListDataElement()
{
	for (ulong i = 0; i < mElementVector.size(); i++)
		delete mElementVector[i];
}

ListDataElement::ListDataElement(const ListDataElement& theListDataElement)
{
	mIsList = true;
	for (ulong i = 0; i < theListDataElement.mElementVector.size(); i++)
		mElementVector.push_back(theListDataElement.mElementVector[i]->Duplicate());
}

ListDataElement& ListDataElement::operator=(const ListDataElement& theListDataElement)
{
	ulong i;

	for (i = 0; i < mElementVector.size(); i++)
		delete mElementVector[i];
	mElementVector.clear();

	for (i = 0; i < theListDataElement.mElementVector.size(); i++)
		mElementVector.push_back(theListDataElement.mElementVector[i]->Duplicate());

	return *this;
}

DataElement* ListDataElement::Duplicate()
{
	ListDataElement* aListDataElement = new ListDataElement(*this);
	return aListDataElement;
}

///

CharData::CharData()
{
	mWidth = 0;
	mOrder = 0;

	//for (ulong i = 0; i < 256; i++)
	//	mKerningOffsets[i] = 0;
}

FontLayer::FontLayer(FontData* theFontData)
{
	mFontData = theFontData;
	mDrawMode = -1;
	mSpacing = 0;
	mPointSize = 0;
	mAscent = 0;
	mAscentPadding = 0;
	mMinPointSize = -1;
	mMaxPointSize = -1;
	mHeight = 0;
	mDefaultHeight = 0;
	mColorMult = Color::White;
	mColorAdd = Color(0, 0, 0, 0);
	mLineSpacingOffset = 0;
	mBaseOrder = 0;
}

inline CharData* FontLayer::GetCharData(SexyChar theChar)
{
	bool array_lookup = theChar >= 0 && (char32_t)theChar < mCharDataTable.size();
	if (array_lookup) {
		auto &aData = mCharDataTable[theChar];
		if (aData.has_value()) return aData.value().get();
	} else {
		auto anItr = mCharDataMap.find(theChar);
		if (anItr != mCharDataMap.end()) return anItr->second.get();
	}
	
	auto pChar = mCharDataMap.insert(CharDataMap::value_type(theChar, std::make_unique<CharData>())).first->second;

	if (array_lookup) {
		mCharDataTable[theChar] = pChar;
	}

	return pChar.get();
}

FontLayer::FontLayer(const FontLayer& theFontLayer) :
	mFontData(theFontLayer.mFontData),
	mRequiredTags(theFontLayer.mRequiredTags),
	mExcludedTags(theFontLayer.mExcludedTags),
	mCharDataMap(theFontLayer.mCharDataMap),
	mCharDataTable(theFontLayer.mCharDataTable),
	mColorMult(theFontLayer.mColorMult),
	mColorAdd(theFontLayer.mColorAdd),
	mImage(theFontLayer.mImage),
	mDrawMode(theFontLayer.mDrawMode),
	mOffset(theFontLayer.mOffset),
	mSpacing(theFontLayer.mSpacing),
	mMinPointSize(theFontLayer.mMinPointSize),
	mMaxPointSize(theFontLayer.mMaxPointSize),
	mPointSize(theFontLayer.mPointSize),
	mAscent(theFontLayer.mAscent),
	mAscentPadding(theFontLayer.mAscentPadding),
	mHeight(theFontLayer.mHeight),
	mDefaultHeight(theFontLayer.mDefaultHeight),
	mLineSpacingOffset(theFontLayer.mLineSpacingOffset),
	mBaseOrder(theFontLayer.mBaseOrder),
	// @Minerscale mUseAlphaCorrection wasn't initialised in the copy constructor causing UB
	mUseAlphaCorrection(theFontLayer.mUseAlphaCorrection)
{
	//ulong i;
	//
	//for (i = 0; i < 256; i++)
	//	mCharData[i] = theFontLayer.mCharData[i];	

	/*
	for (auto &anItr : theFontLayer.mCharDataMap)
	{
		FontLayer::SetCharData(anItr.first, anItr.second);
		//mCharDataMap.insert(CharDataMap::value_type(anItr.first, anItr.second));
	}*/
}

FontData::FontData()
{
	mInitialized = false;

	mApp = NULL;
	mRefCount = 0;
	mDefaultPointSize = 0;

	//for (ulong i = 0; i < 256; i++)
	//	mCharMap[i] = (uchar) i;
}

FontData::~FontData()
{
	DataElementMap::iterator anItr = mDefineMap.begin();
	while (anItr != mDefineMap.end())
	{
		std::string aDefineName = anItr->first;
		DataElement* aDataElement = anItr->second;

		delete aDataElement;
		++anItr;
	}
}

void FontData::Ref()
{
	mRefCount++;
}

void FontData::DeRef()
{
	if (--mRefCount == 0)
	{
		delete this;
	}
}

bool FontData::Error(const std::string& theError)
{
	if (mApp != NULL)
	{
		std::string anErrorString = mFontErrorHeader + theError;

		if (mCurrentLine.length() > 0)
		{
			anErrorString += " on Line " + StrFormat("%d:\r\n\r\n", mCurrentLineNum) + mCurrentLine;
		}

		mApp->Popup(anErrorString);
	}

	return false;
}

bool FontData::DataToLayer(DataElement* theSource, FontLayer** theFontLayer)
{
	*theFontLayer = NULL;

	if (theSource->mIsList)
		return false;

	std::string aLayerName = StringToUpper(((SingleDataElement*)theSource)->mString);

	FontLayerMap::iterator anItr = mFontLayerMap.find(aLayerName);
	if (anItr == mFontLayerMap.end())
	{
		Error("Undefined Layer");
		return false;
	}

	*theFontLayer = anItr->second;

	return true;
}

bool FontData::GetColorFromDataElement(DataElement* theElement, Color& theColor)
{
	if (theElement->mIsList)
	{
		DoubleVector aFactorVector;
		if (!DataToDoubleVector(theElement, &aFactorVector) && (aFactorVector.size() == 4))
			return false;

		theColor = Color(
			(int)(aFactorVector[0] * 255),
			(int)(aFactorVector[1] * 255),
			(int)(aFactorVector[2] * 255),
			(int)(aFactorVector[3] * 255));

		return true;
	}

	int aColor = 0;
	if (!StringToInt(((SingleDataElement*)theElement)->mString, &aColor))
		return false;

	theColor = aColor;
	return true;
}

char32_t UTF8CharToUTF32Char(const std::string theString) {
	char32_t ret = 0;
	memcpy(&ret, theString.data(), theString.length());
	return ret;
}

bool FontData::HandleCommand(const ListDataElement& theParams)
{
	std::string aCmd = ((SingleDataElement*)theParams.mElementVector[0])->mString;

	bool invalidNumParams = false;
	bool invalidParamFormat = false;
	bool literalError = false;
	bool sizeMismatch = false;

	if (strcasecmp(aCmd.c_str(), "Define") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			if (!theParams.mElementVector[1]->mIsList)
			{
				std::string aDefineName = StringToUpper(((SingleDataElement*)theParams.mElementVector[1])->mString);

				if (!IsImmediate(aDefineName))
				{
					DataElementMap::iterator anItr = mDefineMap.find(aDefineName);
					if (anItr != mDefineMap.end())
					{
						delete anItr->second;
						mDefineMap.erase(anItr);
					}

					if (theParams.mElementVector[2]->mIsList)
					{
						ListDataElement* aValues = new ListDataElement();
						if (!GetValues(((ListDataElement*)theParams.mElementVector[2]), aValues))
						{
							delete aValues;
							return false;
						}

						mDefineMap.insert(DataElementMap::value_type(aDefineName, aValues));
					}
					else
					{
						SingleDataElement* aDefParam = (SingleDataElement*)theParams.mElementVector[2];

						DataElement* aDerefVal = Dereference(aDefParam->mString);

						if (aDerefVal)
							mDefineMap.insert(DataElementMap::value_type(aDefineName, aDerefVal->Duplicate()));
						else
							mDefineMap.insert(DataElementMap::value_type(aDefineName, aDefParam->Duplicate()));
					}
				}
				else
					invalidParamFormat = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "CreateHorzSpanRectList") == 0)
	{
		if (theParams.mElementVector.size() == 4)
		{
			IntVector aRectIntVector;
			IntVector aWidthsVector;

			if ((!theParams.mElementVector[1]->mIsList) &&
				(DataToIntVector(theParams.mElementVector[2], &aRectIntVector)) &&
				(aRectIntVector.size() == 4) &&
				(DataToIntVector(theParams.mElementVector[3], &aWidthsVector)))
			{
				std::string aDefineName = StringToUpper(((SingleDataElement*)theParams.mElementVector[1])->mString);

				int aXPos = 0;

				ListDataElement* aRectList = new ListDataElement();

				for (ulong aWidthNum = 0; aWidthNum < aWidthsVector.size(); aWidthNum++)
				{
					ListDataElement* aRectElement = new ListDataElement();
					aRectList->mElementVector.push_back(aRectElement);

					char aStr[256];

					sprintf(aStr, "%d", aRectIntVector[0] + aXPos);
					aRectElement->mElementVector.push_back(new SingleDataElement(aStr));

					sprintf(aStr, "%d", aRectIntVector[1]);
					aRectElement->mElementVector.push_back(new SingleDataElement(aStr));

					sprintf(aStr, "%d", aWidthsVector[aWidthNum]);
					aRectElement->mElementVector.push_back(new SingleDataElement(aStr));

					sprintf(aStr, "%d", aRectIntVector[3]);
					aRectElement->mElementVector.push_back(new SingleDataElement(aStr));

					aXPos += aWidthsVector[aWidthNum];
				}

				DataElementMap::iterator anItr = mDefineMap.find(aDefineName);
				if (anItr != mDefineMap.end())
				{
					delete anItr->second;
					mDefineMap.erase(anItr);
				}

				mDefineMap.insert(DataElementMap::value_type(aDefineName, aRectList));
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "SetDefaultPointSize") == 0)
	{
		if (theParams.mElementVector.size() == 2)
		{
			int aPointSize;

			if ((!theParams.mElementVector[1]->mIsList) &&
				(StringToInt(((SingleDataElement*)theParams.mElementVector[1])->mString, &aPointSize)))
			{
				mDefaultPointSize = aPointSize;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "SetCharMap") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			StringVector aFromVector;
			StringVector aToVector;

			if ((DataToStringVector(theParams.mElementVector[1], &aFromVector)) &&
				(DataToStringVector(theParams.mElementVector[2], &aToVector)))
			{
				if (aFromVector.size() == aToVector.size())
				{
					for (ulong aMapIdx = 0; aMapIdx < aFromVector.size(); aMapIdx++)
					{
						if ((aFromVector[aMapIdx].length() == 1) && (aToVector[aMapIdx].length() == 1))
						{
							//mCharMap[(uchar) aFromVector[aMapIdx][0]] = (uchar) aToVector[aMapIdx][0];
							SexyChar fromChar = aFromVector[aMapIdx][0];
							SexyChar toChar = aToVector[aMapIdx][0];

							SetMappedChar(fromChar, toChar);
						}
						else
							invalidParamFormat = true;
					}
				}
				else
					sizeMismatch = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "CreateLayer") == 0)
	{
		if (theParams.mElementVector.size() == 2)
		{
			if (!theParams.mElementVector[1]->mIsList)
			{
				std::string aLayerName = StringToUpper(((SingleDataElement*)theParams.mElementVector[1])->mString);

				mFontLayerList.push_back(FontLayer(this));
				FontLayer* aFontLayer = &mFontLayerList.back();

				if (!mFontLayerMap.insert(FontLayerMap::value_type(aLayerName, aFontLayer)).second)
				{
					Error("Layer Already Exists");
				}
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "CreateLayerFrom") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aSourceLayer;

			if ((!theParams.mElementVector[1]->mIsList) && (DataToLayer(theParams.mElementVector[2], &aSourceLayer)))
			{
				std::string aLayerName = StringToUpper(((SingleDataElement*)theParams.mElementVector[1])->mString);

				mFontLayerList.push_back(FontLayer(*aSourceLayer));
				FontLayer* aFontLayer = &mFontLayerList.back();

				if (!mFontLayerMap.insert(FontLayerMap::value_type(aLayerName, aFontLayer)).second)
				{
					Error("Layer Already Exists");
				}
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerRequireTags") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			StringVector aStringVector;

			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(DataToStringVector(theParams.mElementVector[2], &aStringVector)))
			{
				for (ulong i = 0; i < aStringVector.size(); i++)
					aLayer->mRequiredTags.push_back(StringToUpper(aStringVector[i]));
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerExcludeTags") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			StringVector aStringVector;

			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(DataToStringVector(theParams.mElementVector[2], &aStringVector)))
			{
				for (ulong i = 0; i < aStringVector.size(); i++)
					aLayer->mExcludedTags.push_back(StringToUpper(aStringVector[i]));
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerPointRange") == 0)
	{
		if (theParams.mElementVector.size() == 4)
		{
			FontLayer* aLayer;
			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(!theParams.mElementVector[2]->mIsList) &&
				(!theParams.mElementVector[3]->mIsList))
			{
				int aMinPointSize;
				int aMaxPointSize;

				if ((StringToInt(((SingleDataElement*)theParams.mElementVector[2])->mString, &aMinPointSize)) &&
					(StringToInt(((SingleDataElement*)theParams.mElementVector[3])->mString, &aMaxPointSize)))
				{
					aLayer->mMinPointSize = aMinPointSize;
					aLayer->mMaxPointSize = aMaxPointSize;
				}
				else
					invalidParamFormat = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetPointSize") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(!theParams.mElementVector[2]->mIsList))
			{
				int aPointSize;
				if (StringToInt(((SingleDataElement*)theParams.mElementVector[2])->mString, &aPointSize))
				{
					aLayer->mPointSize = aPointSize;
				}
				else
					invalidParamFormat = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetHeight") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(!theParams.mElementVector[2]->mIsList))
			{
				int aHeight;
				if (StringToInt(((SingleDataElement*)theParams.mElementVector[2])->mString, &aHeight))
				{
					aLayer->mHeight = aHeight;
				}
				else
					invalidParamFormat = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetImage") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			std::string aFileNameString;

			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(DataToString(theParams.mElementVector[2], &aFileNameString)))
			{
				std::string aFileName = GetPathFrom(aFileNameString, GetFileDir(mSourceFile));

				Image *anImage = mApp->GetSharedImage(aFileName, "");

				if ((Image*)anImage != NULL)
				{
					aLayer->mImage = anImage;
				}
				else
				{
					Error("Failed to Load Image");
					return false;
				}
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetDrawMode") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) && (!theParams.mElementVector[2]->mIsList))
			{
				int anDrawMode;
				if ((StringToInt(((SingleDataElement*)theParams.mElementVector[2])->mString, &anDrawMode)) &&
					(anDrawMode >= 0) && (anDrawMode <= 1))
				{
					aLayer->mDrawMode = anDrawMode;
				}
				else
					invalidParamFormat = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetColorMult") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			if (DataToLayer(theParams.mElementVector[1], &aLayer))
			{
				if (!GetColorFromDataElement(theParams.mElementVector[2], aLayer->mColorMult))
					invalidParamFormat = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetColorAdd") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			if (DataToLayer(theParams.mElementVector[1], &aLayer))
			{
				if (!GetColorFromDataElement(theParams.mElementVector[2], aLayer->mColorAdd))
					invalidParamFormat = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetAscent") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(!theParams.mElementVector[2]->mIsList))
			{
				int anAscent;
				if (StringToInt(((SingleDataElement*)theParams.mElementVector[2])->mString, &anAscent))
				{
					aLayer->mAscent = anAscent;
				}
				else
					invalidParamFormat = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetAscentPadding") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(!theParams.mElementVector[2]->mIsList))
			{
				int anAscent;
				if (StringToInt(((SingleDataElement*)theParams.mElementVector[2])->mString, &anAscent))
				{
					aLayer->mAscentPadding = anAscent;
				}
				else
					invalidParamFormat = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetLineSpacingOffset") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(!theParams.mElementVector[2]->mIsList))
			{
				int anAscent;
				if (StringToInt(((SingleDataElement*)theParams.mElementVector[2])->mString, &anAscent))
				{
					aLayer->mLineSpacingOffset = anAscent;
				}
				else
					invalidParamFormat = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetOffset") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			IntVector anOffset;

			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) && (DataToIntVector(theParams.mElementVector[2], &anOffset)) && (anOffset.size() == 2))
			{
				aLayer->mOffset.mX = anOffset[0];
				aLayer->mOffset.mY = anOffset[1];
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetCharWidths") == 0)
	{
		if (theParams.mElementVector.size() == 4)
		{
			FontLayer* aLayer;
			StringVector aCharsVector;
			IntVector aCharWidthsVector;

			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(DataToStringVector(theParams.mElementVector[2], &aCharsVector)) &&
				(DataToIntVector(theParams.mElementVector[3], &aCharWidthsVector)))
			{
				if (aCharsVector.size() == aCharWidthsVector.size())
				{
					for (ulong i = 0; i < aCharsVector.size(); i++)
					{
						//std::wstring aWString = UTF8StringToWString(aCharsVector[i]);
						char32_t first_char = UTF8CharToUTF32Char(aCharsVector[i]);
						aLayer->GetCharData(first_char)->mWidth = aCharWidthsVector[i];
						//aLayer->mCharData[(uchar) aCharsVector[i][0]].mWidth = aCharWidthsVector[i];
					}
				}
				else
					sizeMismatch = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetSpacing") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			IntVector anOffset;

			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(!theParams.mElementVector[2]->mIsList))
			{
				int aSpacing;

				if (StringToInt(((SingleDataElement*)theParams.mElementVector[2])->mString, &aSpacing))
				{
					aLayer->mSpacing = aSpacing;
				}
				else
					invalidParamFormat = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetImageMap") == 0)
	{
		if (theParams.mElementVector.size() == 4)
		{
			FontLayer* aLayer;
			StringVector aCharsVector;
			ListDataElement aRectList;

			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(DataToStringVector(theParams.mElementVector[2], &aCharsVector)) &&
				(DataToList(theParams.mElementVector[3], &aRectList)))
			{
				if (aCharsVector.size() == aRectList.mElementVector.size())
				{
					if ((Image*)aLayer->mImage != NULL)
					{
						int anImageWidth = aLayer->mImage->GetWidth();
						int anImageHeight = aLayer->mImage->GetHeight();

						for (ulong i = 0; i < aCharsVector.size(); i++)
						{
							IntVector aRectElement;

							char32_t first_char = UTF8CharToUTF32Char(aCharsVector[i]);
							//std::wstring aWString = UTF8StringToWString(aCharsVector[i]);

							if ((DataToIntVector(aRectList.mElementVector[i], &aRectElement)) &&
								(aRectElement.size() == 4))

							{
								Rect aRect = Rect(aRectElement[0], aRectElement[1], aRectElement[2], aRectElement[3]);

								if ((aRect.mX < 0) || (aRect.mY < 0) ||
									(aRect.mX + aRect.mWidth > anImageWidth) || (aRect.mY + aRect.mHeight > anImageHeight))
								{
									Error("Image rectangle out of bounds");
									return false;
								}

								//aLayer->mCharData[(uchar) aCharsVector[i][0]].mImageRect = aRect;		
								aLayer->GetCharData(first_char)->mImageRect = aRect;
							}
							else
								invalidParamFormat = true;
						}

						aLayer->mDefaultHeight = 0;
						//for (int aCharNum = 0; aCharNum < 256; aCharNum++)
						//	if (aLayer->mCharData[aCharNum].mImageRect.mHeight + aLayer->mCharData[aCharNum].mOffset.mY > aLayer->mDefaultHeight)
						//		aLayer->mDefaultHeight = aLayer->mCharData[aCharNum].mImageRect.mHeight + aLayer->mCharData[aCharNum].mOffset.mY;
						for (auto &anItr : aLayer->mCharDataMap)
						{
							CharData *aCharData = anItr.second.get();
							if (aCharData->mImageRect.mHeight + aCharData->mOffset.mY > aLayer->mDefaultHeight)
							{
								aLayer->mDefaultHeight = aCharData->mImageRect.mHeight + aCharData->mOffset.mY;
							}
						}
					}
					else
					{
						Error("Layer image not set");
						return false;
					}
				}
				else
					sizeMismatch = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetCharOffsets") == 0)
	{
		if (theParams.mElementVector.size() == 4)
		{
			FontLayer* aLayer;
			StringVector aCharsVector = StringVector();
			ListDataElement aRectList = ListDataElement();

			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(DataToStringVector(theParams.mElementVector[2], &aCharsVector)) &&
				(DataToList(theParams.mElementVector[3], &aRectList)))
			{
				if (aCharsVector.size() == aRectList.mElementVector.size())
				{
					for (ulong i = 0; i < aCharsVector.size(); i++)
					{
						IntVector aRectElement = IntVector();

						char32_t first_char = UTF8CharToUTF32Char(aCharsVector[i]);
						//std::wstring aWString = UTF8StringToWString(aCharsVector[i]);

						if ((DataToIntVector(aRectList.mElementVector[i], &aRectElement)) &&
							(aRectElement.size() == 2))
						{
							//aLayer->mCharData[(uchar) aCharsVector[i][0]].mOffset = Point(aRectElement[0], aRectElement[1]);
							aLayer->GetCharData(first_char)->mOffset = Point(aRectElement[0], aRectElement[1]);
						}
						else {
							invalidParamFormat = true;
				 		}
					}
				}
				else
					sizeMismatch = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetKerningPairs") == 0)
	{
		if (theParams.mElementVector.size() == 4)
		{
			FontLayer* aLayer;
			StringVector aPairsVector;
			IntVector anOffsetsVector;

			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(DataToStringVector(theParams.mElementVector[2], &aPairsVector)) &&
				(DataToIntVector(theParams.mElementVector[3], &anOffsetsVector)))
			{
				if (aPairsVector.size() == anOffsetsVector.size())
				{
					for (ulong i = 0; i < aPairsVector.size(); i++)
					{
						// Convert two utf8 chars to utf32
						char32_t aChars[2]{};
						int stringIdx = 0;
						for (int j = 0; j < 2; ++j) {
							char *a = (char *)&aChars[j];
							for (size_t k = 0; k < aPairsVector[i].length(); ++k) {
								a[k] = aPairsVector[i][stringIdx++];
								if (!(a[k] & 0x80)) break;
							}
						}

						//std::wstring aWString = UTF8StringToWString(aPairsVector[i]);
						//if (aWString.length() == 2)
						//{
							//aLayer->mCharData[(uchar) aPairsVector[i][0]].mKerningOffsets[(uchar) aPairsVector[i][1]] = anOffsetsVector[i];
						aLayer->GetCharData(aChars[0])->SetKernOffset(aChars[1], anOffsetsVector[i]);
						//}
					}
				}
				else
					sizeMismatch = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetBaseOrder") == 0)
	{
		if (theParams.mElementVector.size() == 3)
		{
			FontLayer* aLayer;
			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(!theParams.mElementVector[2]->mIsList))
			{
				int aBaseOrder;
				if (StringToInt(((SingleDataElement*)theParams.mElementVector[2])->mString, &aBaseOrder))
				{
					aLayer->mBaseOrder = aBaseOrder;
				}
				else
					invalidParamFormat = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetCharOrders") == 0)
	{
		if (theParams.mElementVector.size() == 4)
		{
			FontLayer* aLayer;
			StringVector aCharsVector;
			IntVector aCharOrdersVector;

			if ((DataToLayer(theParams.mElementVector[1], &aLayer)) &&
				(DataToStringVector(theParams.mElementVector[2], &aCharsVector)) &&
				(DataToIntVector(theParams.mElementVector[3], &aCharOrdersVector)))
			{
				if (aCharsVector.size() == aCharOrdersVector.size())
				{
					for (ulong i = 0; i < aCharsVector.size(); i++)
					{
						if (aCharsVector[i].length() == 1)
						{
							//aLayer->mCharData[(uchar) aCharsVector[i][0]].mOrder = aCharOrdersVector[i];
							aLayer->GetCharData(aCharsVector[i][0])->mOrder = aCharOrdersVector[i];
						}
						else
							invalidParamFormat = true;
					}
				}
				else
					sizeMismatch = true;
			}
			else
				invalidParamFormat = true;
		}
		else
			invalidNumParams = true;
	}
	else if (strcasecmp(aCmd.c_str(), "LayerSetExInfo") == 0) {}
	else
	{
		Error("Unknown Command");
		return false;
	}

	if (invalidNumParams)
	{
		Error("Invalid Number of Parameters");
		return false;
	}

	if (invalidParamFormat)
	{
		Error("Invalid Paramater Type");
		return false;
	}

	if (literalError)
	{
		Error("Undefined Value");
		return false;
	}

	if (sizeMismatch)
	{
		Error("List Size Mismatch");
		return false;
	}

	return true;
}

bool FontData::Load(SexyAppBase* theSexyApp, const std::string& theFontDescFileName)
{
	if (mInitialized)
		return false;

	bool hasErrors = false;

	mApp = theSexyApp;
	mCurrentLine = "";

	mFontErrorHeader = "Font Descriptor Error in " + theFontDescFileName + "\r\n";

	mSourceFile = theFontDescFileName;

	mInitialized = LoadDescriptor(theFontDescFileName); ;

	return !hasErrors;
}

bool FontData::LoadLegacy(Image* /*theFontImage*/, const std::string& /*theFontDescFileName*/)
{
	if (mInitialized)
		return false;

	mFontLayerList.push_back(FontLayer(this));
	FontLayer* aFontLayer = &mFontLayerList.back();

	FontLayerMap::iterator anItr = mFontLayerMap.insert(FontLayerMap::value_type("", aFontLayer)).first;
	if (anItr == mFontLayerMap.end())
		return false;

	unreachable();
	/*
	aFontLayer->mImage = (MemoryImage*)theFontImage;
	aFontLayer->mDefaultHeight = aFontLayer->mImage->GetHeight();
	aFontLayer->mAscent = aFontLayer->mImage->GetHeight();

	int aCharPos = 0;
	FILE* aStream = fopen(theFontDescFileName.c_str(), "r");

	if (aStream == NULL)
		return false;

	mSourceFile = theFontDescFileName;

	// int aSpaceWidth = 0; // unused
	//fscanf(aStream,"%d%d",&aFontLayer->mCharData[' '].mWidth,&aFontLayer->mAscent);
	fscanf(aStream, "%d%d", &aFontLayer->GetCharData(' ')->mWidth, &aFontLayer->mAscent);

	while (!feof(aStream))
	{
		char aBuf[2] = { 0, 0 }; // needed because fscanf will null terminate the string it reads
		char aChar = 0;
		int aWidth = 0;

		fscanf(aStream, "%1s%d", aBuf, &aWidth);
		aChar = aBuf[0];


		if (aChar == 0)
			break;

		//aFontLayer->mCharData[(uchar) aChar].mImageRect = Rect(aCharPos, 0, aWidth, aFontLayer->mImage->GetHeight());
		//aFontLayer->mCharData[(uchar) aChar].mWidth = aWidth;
		aFontLayer->GetCharData(aChar)->mImageRect = Rect(aCharPos, 0, aWidth, aFontLayer->mImage->GetHeight());
		aFontLayer->GetCharData(aChar)->mWidth = aWidth;

		aCharPos += aWidth;
	}

	char c;

	for (c = 'A'; c <= 'Z'; c++)
		if ((aFontLayer->mCharDataMap[c].mWidth == 0) && (aFontLayer->mCharDataMap[c - 'A' + 'a'].mWidth != 0))
			//mCharMap[c] = c - 'A' + 'a';
			mCharMap.insert(CharMap::value_type(c, c - 'A' + 'a'));

	for (c = 'a'; c <= 'z'; c++)
		if ((aFontLayer->mCharDataMap[c].mWidth == 0) && (aFontLayer->mCharDataMap[c - 'a' + 'A'].mWidth != 0))
			//mCharMap[c] = c - 'a' + 'A';
			mCharMap.insert(CharMap::value_type(c, c - 'a' + 'A'));

	mInitialized = true;
	fclose(aStream);

	return true;*/
}

////

ActiveFontLayer::ActiveFontLayer()
{
	mScaledImage = NULL;
	mOwnsImage = false;
}

ActiveFontLayer::ActiveFontLayer(const ActiveFontLayer& theActiveFontLayer) :
	mBaseFontLayer(theActiveFontLayer.mBaseFontLayer),
	mScaledImage(theActiveFontLayer.mScaledImage),
	mOwnsImage(theActiveFontLayer.mOwnsImage)
{
	if (mOwnsImage) {
		unreachable();
		/* FIXME
		mScaledImage = mBaseFontLayer->mFontData->mApp->CopyImage(mScaledImage);*/
	}

	//for (int aCharNum = 0; aCharNum < 256; aCharNum++)
	//	mScaledCharImageRects[aCharNum] = theActiveFontLayer.mScaledCharImageRects[aCharNum];
	
	for (auto anItr = theActiveFontLayer.mScaledCharImageRects.begin(); anItr != theActiveFontLayer.mScaledCharImageRects.end(); anItr++)
	{
		mScaledCharImageRects.insert(CharRectMap::value_type(anItr->first, anItr->second));
	}
}

ActiveFontLayer::~ActiveFontLayer()
{
	if (mOwnsImage)
		delete mScaledImage;
}

////

ImageFont::ImageFont(SexyAppBase* theSexyApp, const std::string& theFontDescFileName)
{
	mScale = 1.0;
	mFontData = new FontData();
	mFontData->Ref();
	mFontData->Load(theSexyApp, theFontDescFileName);
	mPointSize = mFontData->mDefaultPointSize;
	GenerateActiveFontLayers();
	mActiveListValid = true;
	mForceScaledImagesWhite = false;
}

ImageFont::ImageFont(Image* /*theFontImage*/)
{
	mScale = 1.0;
	mFontData = new FontData();
	mFontData->Ref();
	mFontData->mInitialized = true;
	mPointSize = mFontData->mDefaultPointSize;
	mActiveListValid = false;
	mForceScaledImagesWhite = false;

	mFontData->mFontLayerList.push_back(FontLayer(mFontData));
	FontLayer* aFontLayer = &mFontData->mFontLayerList.back();

	// mFontData->mFontLayerMap.insert(FontLayerMap::value_type("", aFontLayer)).first;
	// Weird stray .first
	(void)mFontData->mFontLayerMap.insert(FontLayerMap::value_type("", aFontLayer)).first;

	unreachable();
	/* TODO
	aFontLayer->mImage = (MemoryImage*)theFontImage;
	aFontLayer->mDefaultHeight = aFontLayer->mImage->GetHeight();
	aFontLayer->mAscent = aFontLayer->mImage->GetHeight();*/
}

ImageFont::ImageFont(const ImageFont& theImageFont) :
	_Font(theImageFont),
	mFontData(theImageFont.mFontData),
	mPointSize(theImageFont.mPointSize),
	mTagVector(theImageFont.mTagVector),
	mActiveListValid(theImageFont.mActiveListValid),
	mScale(theImageFont.mScale),
	mForceScaledImagesWhite(theImageFont.mForceScaledImagesWhite)
{
	mFontData->Ref();

	if (mActiveListValid)
		mActiveLayerList = theImageFont.mActiveLayerList;
}

ImageFont::ImageFont(Image* theFontImage, const std::string& theFontDescFileName)
{

	mScale = 1.0;
	mFontData = new FontData();
	mFontData->Ref();
	mFontData->LoadLegacy(theFontImage, theFontDescFileName);
	mPointSize = mFontData->mDefaultPointSize;
	GenerateActiveFontLayers();
	mActiveListValid = true;
}

ImageFont::~ImageFont()
{
	mFontData->DeRef();
}

/*ImageFont::ImageFont(const ImageFont& theImageFont, Image* theImage) :
	_Font(theImageFont),
	mImage(theImage)
{
	for (int i = 0; i < 256; i++)
	{
		mCharPos[i] = theImageFont.mCharPos[i];
		mCharWidth[i] = theImageFont.mCharWidth[i];
	}
}*/

void ImageFont::GenerateActiveFontLayers()
{
	if (!mFontData->mInitialized)
		return;

	mActiveLayerList.clear();

	ulong i;

	mAscent = 0;
	mAscentPadding = 0;
	mHeight = 0;
	mLineSpacingOffset = 0;

	FontLayerList::iterator anItr = mFontData->mFontLayerList.begin();

	bool firstLayer = true;

	while (anItr != mFontData->mFontLayerList.end())
	{
		FontLayer* aFontLayer = &*anItr;

		if ((mPointSize >= aFontLayer->mMinPointSize) &&
			((mPointSize <= aFontLayer->mMaxPointSize) || (aFontLayer->mMaxPointSize == -1)))
		{
			bool active = true;

			// Make sure all required tags are included
			for (i = 0; i < aFontLayer->mRequiredTags.size(); i++)
				if (std::find(mTagVector.begin(), mTagVector.end(), aFontLayer->mRequiredTags[i]) == mTagVector.end())
					active = false;

			// Make sure no excluded tags are included
			for (i = 0; i < mTagVector.size(); i++)
				if (std::find(aFontLayer->mExcludedTags.begin(), aFontLayer->mExcludedTags.end(),
					mTagVector[i]) != aFontLayer->mExcludedTags.end())
					active = false;

			if (active)
			{
				mActiveLayerList.push_back(ActiveFontLayer());

				ActiveFontLayer* anActiveFontLayer = &mActiveLayerList.back();

				anActiveFontLayer->mBaseFontLayer = aFontLayer;

				double aLayerPointSize = 1;
				double aPointSize = mScale;

				if ((mScale == 1.0) && ((aFontLayer->mPointSize == 0) || (mPointSize == aFontLayer->mPointSize)))
				{
					anActiveFontLayer->mScaledImage = aFontLayer->mImage;
					anActiveFontLayer->mOwnsImage = false;

					// Use the specified point size

					//for (int aCharNum = 0; aCharNum < 256; aCharNum++)
					//	anActiveFontLayer->mScaledCharImageRects[aCharNum] = aFontLayer->GetCharData(aCharNum)->mImageRect;aFontLayer->mCharData[aCharNum].mImageRect;
					for (auto &anItr : aFontLayer->mCharDataMap)
					{
						anActiveFontLayer->mScaledCharImageRects.insert(CharRectMap::value_type(anItr.first, anItr.second->mImageRect));
					}
				}
				else
				{
					if (aFontLayer->mPointSize != 0)
					{
						aLayerPointSize = aFontLayer->mPointSize;
						aPointSize = mPointSize * mScale;
					}

					// Resize font elements
					int aCharNum = 0;

					unreachable();
					/* TODO
					MemoryImage* aMemoryImage = new MemoryImage(mFontData->mApp);
					*/

					int aCurX = 0;
					int aMaxHeight = 0;

					//for (aCharNum = 0; aCharNum < 256; aCharNum++)
					//{
					//	Rect* anOrigRect = &aFontLayer->mCharData[aCharNum].mImageRect;
					//
					//	Rect aScaledRect(aCurX, 0,  
					//		(int) ((anOrigRect->mWidth * aPointSize) / aLayerPointSize),
					//		(int) ((anOrigRect->mHeight * aPointSize) / aLayerPointSize));
					//
					//	anActiveFontLayer->mScaledCharImageRects[aCharNum] = aScaledRect;
					//
					//	if (aScaledRect.mHeight > aMaxHeight)
					//		aMaxHeight = aScaledRect.mHeight;
					//
					//	aCurX += aScaledRect.mWidth;
					//}
					for (auto &anItr : aFontLayer->mCharDataMap)
					{
						aCharNum++;
						Rect* anOrigRect = &anItr.second->mImageRect;

						Rect aScaledRect(aCurX, 0,
							(int)((anOrigRect->mWidth * aPointSize) / aLayerPointSize),
							(int)((anOrigRect->mHeight * aPointSize) / aLayerPointSize));

						anActiveFontLayer->mScaledCharImageRects[aCharNum] = aScaledRect;

						if (aScaledRect.mHeight > aMaxHeight)
							aMaxHeight = aScaledRect.mHeight;

						aCurX += aScaledRect.mWidth;
					}

					unreachable();
					/* TODO
					anActiveFontLayer->mScaledImage = aMemoryImage;
					anActiveFontLayer->mOwnsImage = true;

					// Create the image now

					aMemoryImage->Create(aCurX, aMaxHeight);

					Graphics g(aMemoryImage);

					//for (aCharNum = 0; aCharNum < 256; aCharNum++)
					//{
					//	if ((Image*) aFontLayer->mImage != NULL)
					//		g.DrawImage(aFontLayer->mImage, anActiveFontLayer->mScaledCharImageRects[aCharNum], aFontLayer->mCharData[aCharNum].mImageRect);						
					//}
					for (auto anItr = aFontLayer->mCharDataMap.begin(); anItr != aFontLayer->mCharDataMap.end(); anItr++)
					{
						if ((Image*)aFontLayer->mImage != NULL)
						{
							g.DrawImage(aFontLayer->mImage, anActiveFontLayer->mScaledCharImageRects[aCharNum], anItr->second.mImageRect);
						}
					}


					if (mForceScaledImagesWhite)
					{
						int aCount = aMemoryImage->mWidth * aMemoryImage->mHeight;
						uint32_t* aBits = aMemoryImage->GetBits();

						for (int i = 0; i < aCount; i++) {
							// *(aBits++) = *aBits | 0x00FFFFFF; ambiguous
							*(aBits) = *aBits | 0x00FFFFFF;
							aBits++;
						}
					}

					aMemoryImage->Palletize();*/
				}

				int aLayerAscent = (aFontLayer->mAscent * aPointSize) / aLayerPointSize;
				if (aLayerAscent > mAscent)
					mAscent = aLayerAscent;

				if (aFontLayer->mHeight != 0)
				{
					int aLayerHeight = (aFontLayer->mHeight * aPointSize) / aLayerPointSize;
					if (aLayerHeight > mHeight)
						mHeight = aLayerHeight;
				}
				else
				{
					int aLayerHeight = (aFontLayer->mDefaultHeight * aPointSize) / aLayerPointSize;
					if (aLayerHeight > mHeight)
						mHeight = aLayerHeight;
				}

				int anAscentPadding = (aFontLayer->mAscentPadding * aPointSize) / aLayerPointSize;
				if ((firstLayer) || (anAscentPadding < mAscentPadding))
					mAscentPadding = anAscentPadding;

				int aLineSpacingOffset = (aFontLayer->mLineSpacingOffset * aPointSize) / aLayerPointSize;
				if ((firstLayer) || (aLineSpacingOffset > mLineSpacingOffset))
					mLineSpacingOffset = aLineSpacingOffset;

				firstLayer = false;
			}
		}

		++anItr;
	}
}

int ImageFont::StringWidth(const SexyString& theString)
{
	int aWidth = 0;
	SexyChar aPrevChar = 0;
	for (int i = 0; i < (int)theString.length(); i++)
	{
		SexyChar aChar = theString[i];
		aWidth += CharWidthKern(aChar, aPrevChar);
		aPrevChar = aChar;
	}

	return aWidth;
}

int ImageFont::CharWidthKern(SexyChar theChar, SexyChar thePrevChar)
{
	Prepare();

	auto cached = mKernTable.findKern(theChar, thePrevChar);
	if (cached.has_value()) return cached.value();

	int aMaxXPos = 0;
	double aPointSize = mPointSize * mScale;

	theChar = GetMappedChar(theChar);
	if (thePrevChar != 0)
		thePrevChar = GetMappedChar(thePrevChar);

	for (auto &anActiveFontLayer : mActiveLayerList)
	{
		int aLayerXPos = 0;

		int aLayerPointSize = anActiveFontLayer.mBaseFontLayer->mPointSize;

		double aScale = aLayerPointSize ? aPointSize / aLayerPointSize : mScale;
		int aCharWidth = anActiveFontLayer.mBaseFontLayer->GetCharData(theChar)->mWidth * aScale;

		int aSpacing = thePrevChar ? (anActiveFontLayer.mBaseFontLayer->mSpacing +
					anActiveFontLayer.mBaseFontLayer->GetCharData(thePrevChar)->GetKernOffset(theChar)) * aScale : 0;

		aLayerXPos += aCharWidth + aSpacing;

		if (aLayerXPos > aMaxXPos)
			aMaxXPos = aLayerXPos;
	}

	mKernTable.cacheKern(theChar, thePrevChar, aMaxXPos);

	return aMaxXPos;
}

int ImageFont::CharWidth(SexyChar theChar)
{
	return CharWidthKern(theChar, 0);
}

//CritSect gRenderCritSec;
/*
static const int POOL_SIZE = 4096;
static RenderCommand gRenderCommandPool[POOL_SIZE];
static RenderCommand* gRenderTail[256];
static RenderCommand* gRenderHead[256];
*/
void ImageFont::DrawStringEx(Graphics* g, int theX, int theY, const SexyString& theString, const Color& theColor, RectList* theDrawnAreas, int* theWidth)
{
	//std::multimap<int, RenderCommand>aRenderCommandPool;
	static std::array<std::vector<RenderCommand>, 256> aRenderCommandPool{};
	if (theDrawnAreas != NULL)
		theDrawnAreas->clear();

	if (!mFontData->mInitialized)
	{
		if (theWidth != NULL)
			*theWidth = 0;
		return;
	}

	Prepare();

	bool colorizeImages = g->GetColorizeImages();
	g->SetColorizeImages(true);

	int aCurXPos = theX;
	for (auto it(theString.begin()), nextit(std::next(it)); it != theString.end(); it = nextit++) {
		SexyChar aChar = GetMappedChar(*it);
		SexyChar aNextChar = 0;
		if (nextit != theString.end()) aNextChar = GetMappedChar(*nextit);

		int aMaxXPos = aCurXPos;

		int layerOrderOffset = 0;
		for (auto &fontLayer : mActiveLayerList) {
			int aLayerXPos = aCurXPos;

			int anImageX;
			int anImageY;
			int aCharWidth;
			int aSpacing;

			double aScale = mScale;

			FontLayer &aBaseFontLayer = *fontLayer.mBaseFontLayer;

			int aLayerPointSize = aBaseFontLayer.mPointSize;
			CharData &aBaseCharData = *aBaseFontLayer.GetCharData(aChar);
			if (aLayerPointSize != 0)
				aScale *= (float)mPointSize / (float)aLayerPointSize;

			if (aScale == 1.0)
			{
				anImageX = aLayerXPos + aBaseFontLayer.mOffset.mX + aBaseCharData.mOffset.mX;
				anImageY = theY - (aBaseFontLayer.mAscent - aBaseFontLayer.mOffset.mY - aBaseCharData.mOffset.mY);
				aCharWidth = aBaseCharData.mWidth;

				if (aNextChar != 0) aSpacing = aBaseFontLayer.mSpacing + aBaseCharData.GetKernOffset(aNextChar);
				else aSpacing = 0;
			}
			else
			{
				anImageX = aLayerXPos + (int)((aBaseFontLayer.mOffset.mX + aBaseCharData.mOffset.mX) * aScale);
				anImageY = theY - (int)((aBaseFontLayer.mAscent - aBaseFontLayer.mOffset.mY - aBaseCharData.mOffset.mY) * aScale);
				aCharWidth = (aBaseCharData.mWidth * aScale);

				if (aNextChar != 0) aSpacing = (int)((aBaseFontLayer.mSpacing + aBaseCharData.GetKernOffset(aNextChar)) * aScale);
				else aSpacing = 0;
			}

			Color aColor;
			aColor.mRed = std::min((theColor.mRed * aBaseFontLayer.mColorMult.mRed / 255) + aBaseFontLayer.mColorAdd.mRed, 255);
			aColor.mGreen = std::min((theColor.mGreen * aBaseFontLayer.mColorMult.mGreen / 255) + aBaseFontLayer.mColorAdd.mGreen, 255);
			aColor.mBlue = std::min((theColor.mBlue * aBaseFontLayer.mColorMult.mBlue / 255) + aBaseFontLayer.mColorAdd.mBlue, 255);
			aColor.mAlpha = std::min((theColor.mAlpha * aBaseFontLayer.mColorMult.mAlpha / 255) + aBaseFontLayer.mColorAdd.mAlpha, 255);

			RenderCommand aRenderCommand = {
				.mImage = fontLayer.mScaledImage,
				.mDest = {anImageX, anImageY},
				.mSrc = fontLayer.mScaledCharImageRects[aChar],
				.mMode = aBaseFontLayer.mDrawMode,
				.mColor = aColor,
				.mNext = nullptr,
			};

			int anOrderIdx = std::min(std::max(layerOrderOffset + aBaseFontLayer.mBaseOrder + aBaseCharData.mOrder + 128, 0), 255);

			aRenderCommandPool[anOrderIdx].push_back(aRenderCommand);
			//aRenderCommandPool.insert(std::pair<int, RenderCommand>(anOrderIdx, aRenderCommand));

			if (theDrawnAreas != NULL)
			{
				Rect aDestRect(anImageX, anImageY, fontLayer.mScaledCharImageRects[aChar].mWidth, fontLayer.mScaledCharImageRects[aChar].mHeight);

				theDrawnAreas->push_back(aDestRect);
			}

			aLayerXPos += aCharWidth + aSpacing;

			if (aLayerXPos > aMaxXPos)
				aMaxXPos = aLayerXPos;

			++layerOrderOffset;
		}

		aCurXPos = aMaxXPos;
	}

	if (theWidth != NULL)
		*theWidth = aCurXPos - theX;

	Color anOrigColor = g->GetColor();

	int anOrigMode = g->GetDrawMode();
	for (auto &aRenderVec : aRenderCommandPool) {
		for (auto &cmd : aRenderVec) {
			if (cmd.mMode != -1) g->SetDrawMode(cmd.mMode);
			g->SetColor(Color(cmd.mColor));
			if (cmd.mImage != nullptr) g->DrawImage(cmd.mImage, cmd.mDest[0], cmd.mDest[1], cmd.mSrc);
			if (cmd.mMode != -1) g->SetDrawMode(anOrigMode);
		}

		aRenderVec.clear();
	}

	g->SetColor(anOrigColor);
	g->SetColorizeImages(colorizeImages);
}

void ImageFont::DrawString(Graphics* g, int theX, int theY, const SexyString& theString, const Color& theColor, const Rect& theClipRect)
{
	(void)theClipRect;
	DrawStringEx(g, theX, theY, theString, theColor, NULL, NULL);
}

_Font* ImageFont::Duplicate()
{
	return new ImageFont(*this);
}

void ImageFont::SetPointSize(int thePointSize)
{
	mPointSize = thePointSize;
	mActiveListValid = false;
}

void ImageFont::SetScale(double theScale)
{
	mScale = theScale;
	mActiveListValid = false;
}

int	ImageFont::GetPointSize()
{
	return mPointSize;
}

int	ImageFont::GetDefaultPointSize()
{
	return mFontData->mDefaultPointSize;
}

bool ImageFont::AddTag(const std::string& theTagName)
{
	if (HasTag(theTagName))
		return false;

	std::string aTagName = StringToUpper(theTagName);
	mTagVector.push_back(aTagName);
	mActiveListValid = false;
	return true;
}

bool ImageFont::RemoveTag(const std::string& theTagName)
{
	std::string aTagName = StringToUpper(theTagName);

	StringVector::iterator anItr = std::find(mTagVector.begin(), mTagVector.end(), aTagName);
	if (anItr == mTagVector.end())
		return false;

	mTagVector.erase(anItr);
	mActiveListValid = false;
	return true;
}

bool ImageFont::HasTag(const std::string& theTagName)
{
	StringVector::iterator anItr = std::find(mTagVector.begin(), mTagVector.end(), theTagName);
	return anItr != mTagVector.end();
}

std::string ImageFont::GetDefine(const std::string& theName)
{
	DataElement* aDataElement = mFontData->Dereference(theName);

	if (aDataElement == NULL)
		return "";

	return mFontData->DataElementToString(aDataElement);
}

void ImageFont::Prepare()
{
	if (!mActiveListValid)
	{
		GenerateActiveFontLayers();
		mActiveListValid = true;
	}
}

inline void FontData::SetMappedChar(SexyChar fromChar, SexyChar toChar)
{
	mCharMap[fromChar] = toChar;
	if (fromChar >= 0 && (uint32_t)fromChar < mCharTable.size()) {
		if (!mCharTable[fromChar].has_value()) {
			mCharTable[fromChar] = toChar;
		}
	}
}


SexyChar ImageFont::GetMappedChar(SexyChar theChar)
{
	if (theChar >= 0 && (uint32_t)theChar < mFontData->mCharTable.size()) {
		auto aChar = mFontData->mCharTable[theChar];
		if (aChar.has_value()) return aChar.value();
		else                   return theChar;
	}
	auto anItr = mFontData->mCharMap.find(theChar);
	if (anItr != mFontData->mCharMap.end())
	{
		return anItr->second;
	}
	return theChar;
}
