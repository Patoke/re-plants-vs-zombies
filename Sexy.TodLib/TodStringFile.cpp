#include "TodDebug.h"
#include "TodCommon.h"
#include "TodStringFile.h"
#include "../PakLib/PakInterface.h"
#include "../SexyAppFramework/Font.h"

int gTodStringFormatCount;               //[0x69DE4C]
TodStringListFormat* gTodStringFormats;  //[0x69DA34]

int gLawnStringFormatCount = 12;
TodStringListFormat gLawnStringFormats[14] = {    //0x6A5010
	{ "NORMAL",           nullptr,    Color(40,   50,     90,     255),       0,      0U },
	{ "FLAVOR",           nullptr,    Color(143,  67,     27,     255),       0,      1U },
	{ "KEYWORD",          nullptr,    Color(143,  67,     27,     255),       0,      0U },
	{ "NOCTURNAL",        nullptr,    Color(136,  50,     170,    255),       0,      0U },
	{ "AQUATIC",          nullptr,    Color(11,   161,    219,    255),       0,      0U },
	{ "STAT",             nullptr,    Color(204,  36,     29,     255),       0,      0U },
	{ "METAL",            nullptr,    Color(204,  36,     29,     255),       0,      2U },
	{ "KEYMETAL",         nullptr,    Color(143,  67,     27,     255),       0,      2U },
	{ "SHORTLINE",        nullptr,    Color(0,    0,      0,      0),         -9,     0U },
	{ "EXTRASHORTLINE",   nullptr,    Color(0,    0,      0,      0),         -14,    0U },
	{ "CREDITS1",         nullptr,    Color(0,    0,      0,      0),         3,      0U },
	{ "CREDITS2",         nullptr,    Color(0,    0,      0,      0),         2,      0U },
	{ "NORMAL",           nullptr,    Color(40,   50,     90,     255),       0,      0U },
	{ "KEYWORD",          nullptr,    Color(143,  67,     27,     255),       0,      0U }
};

TodStringListFormat::TodStringListFormat()
{
	mFormatName = "";
	mNewFont = nullptr;
	mLineSpacingOffset = 0;
	mFormatFlags = 0U;
}

TodStringListFormat::TodStringListFormat(const char* theFormatName, Font** theFont, const Color& theColor, int theLineSpacingOffset, unsigned int theFormatFlags) : 
	mFormatName(theFormatName), mNewFont(theFont), mNewColor(theColor), mLineSpacingOffset(theLineSpacingOffset), mFormatFlags(theFormatFlags)
{ 
}

void TodStringListSetColors(TodStringListFormat* theFormats, int theCount)
{
	gTodStringFormats = theFormats;
	gTodStringFormatCount = theCount;
}

//0x518E40
bool TodStringListReadName(const char*& thePtr, std::string& theName)
{
	const char* aNameStart = strchr(thePtr, '[');
	if (aNameStart == nullptr)  // 如果文本中不存在“[”
	{
		if (strspn(thePtr, " \n\r\t") != strlen(thePtr))  // 如果文本不全是空白字符
		{
			TodTrace("Failed to find string name");
			return false;
		}

		theName = "";
		return true;
	}
	else
	{
		const char* aNameEnd = strchr(aNameStart + 1, ']');
		if (aNameEnd == nullptr)  // 如果“[”后不存在“]”
		{
			TodTrace("Failed to find ']'");
			return false;
		}

		int aCount = aNameEnd - aNameStart - 1;
		theName = Sexy::Trim(string(aNameStart + 1, aCount));  // 取得中括号之间的部分并去除字符串前后的空白字符
		if (theName.size() == 0)
		{
			TodTrace("Name Too Short");
			return false;
		}

		thePtr += aCount + 2;  // 移动读取指针至“]”后
		return true;
	}
}

//0x518F60
void TodStringRemoveReturnChars(std::string& theString)
{
	for (int i = 0; i < theString.size(); )
	{
		if (theString[i] == '\r')
			theString.replace(i, 1, "", 0);  // 原版中此处的“1”和“""”已内联至函数内部
		else
			i++;
	}
}

//0x518FB0
bool TodStringListReadValue(const char*& thePtr, std::string& theValue)
{
	const char* aValueEnd = strchr(thePtr, '[');
	int aLen = aValueEnd ? aValueEnd - thePtr : strlen(thePtr);
	theValue = Sexy::Trim(string(thePtr, aLen));  // 如果存在下一个“[”，则取到“[”前为止；否则，取剩下的全部
	TodStringRemoveReturnChars(theValue);  // 移除所有的换行符
	thePtr += aLen;  // 移动读取指针至“[”处（或结尾处）
	return true;
}

//0x519080
bool TodStringListReadItems(const char* theFileText)
{
	const char* aPtr = theFileText;
	std::string aName;
	std::string aValue;

	for (;;)
	{
		if (!TodStringListReadName(aPtr, aName))  // 读取一个标签
			return false;
		if (aName.size() == 0)  // 读取成功但没有读取到标签，表明读取完成
			return true;
		if (!TodStringListReadValue(aPtr, aValue))  // 读取对应的内容
			return false;

		std::string aNameUpper = Sexy::StringToUpper(aName);
		gSexyAppBase->SetString(aNameUpper, Sexy::StringToWString(aValue));
	}
}

//0x519240
bool TodStringListReadFile(const char* theFileName)
{
	PFILE* pFile = p_fopen(theFileName, "rb");
	if (pFile == nullptr)
	{
		TodTrace("Failed to open '%s'", theFileName);
		return false;
	}

	p_fseek(pFile, 0, SEEK_END);  // 指针调整至文件末尾
	int aSize = p_ftell(pFile);  // 当前位置即为文件长度
	p_fseek(pFile, 0, SEEK_SET);  // 指针调回文件开头
	char* aFileText = new char[aSize + 1];
	bool aSuccess = true;
	if (p_fread(aFileText, sizeof(char), aSize, pFile) <= 0)  // 按字节读取数据
	{
		TodTrace("Failed to read '%s'", theFileName);
		aSuccess = false;
	}
	aFileText[aSize] = '\0';
	if (aSuccess)
	{
		aSuccess = TodStringListReadItems(aFileText);
	}
	p_fclose(pFile);  // 关闭文件流
	delete[] aFileText;

	return aSuccess;
}

//0x519390
void TodStringListLoad(const char* theFileName)
{
	if (!TodStringListReadFile(theFileName))
		TodErrorMessageBox(Sexy::StrFormat("Failed to load string list file '%s'", theFileName).c_str(), "Error");
}

//0x519410
SexyString TodStringListFind(const SexyString& theName)
{
	std::string aNameString = Sexy::SexyStringToString(theName);
	StringWStringMap::iterator anItr = gSexyAppBase->mStringProperties.find(aNameString);
	if (anItr != gSexyAppBase->mStringProperties.end())
	{
		return Sexy::WStringToSexyString(anItr->second);
	}
	else
	{
		return Sexy::StrFormat(_S("<Missing %s>"), theName.c_str());
	}
}

//0x519520
SexyString TodStringTranslate(const SexyString& theString)
{
	if (theString.size() >= 3 && theString[0] == '[')
	{
		SexyString aName = theString.substr(1, theString.size() - 2);  // 取“[”与“]”中间的部分
		return TodStringListFind(aName);
	}
	return theString;
}

//0x5195D0
SexyString TodStringTranslate(const SexyChar* theString)
{
	if (theString != nullptr)
	{
		int aLen = strlen(theString);
		if (aLen >= 3 && theString[0] == '[')
		{
			SexyString aName(theString, 1, aLen - 2);  // 取“[”与“]”中间的部分
			return TodStringListFind(aName);
		}
		else
			return theString;
	}
	else
		return "";
}

//0x5196C0
bool TodStringListExists(const SexyString& theString)
{
	if (theString.size() >= 3 && theString[0] == '[')
	{
		SexyString aName = theString.substr(1, theString.size() - 2);  // 取“[”与“]”中间的部分
		return gSexyAppBase->mStringProperties.find(aName) != gSexyAppBase->mStringProperties.end();
	}
	return false;
}

//0x5197B0
void TodWriteStringSetFormat(const char* theFormat, TodStringListFormat& theCurrentFormat)
{
	for (int i = 0; i < gTodStringFormatCount; i++)
	{
		const TodStringListFormat& aFormat = gTodStringFormats[i];
		if (strncmp(theFormat, aFormat.mFormatName, strlen(aFormat.mFormatName)) == 0)
		{
			if (aFormat.mNewFont != nullptr)
				theCurrentFormat.mNewFont = aFormat.mNewFont;
			if (aFormat.mNewColor != Color(0, 0, 0, 0))
				theCurrentFormat.mNewColor = aFormat.mNewColor;
			theCurrentFormat.mLineSpacingOffset = aFormat.mLineSpacingOffset;
			theCurrentFormat.mFormatFlags = aFormat.mFormatFlags;
			return;
		}
	}
}

bool CharIsSpaceInFormat(char theChar, const TodStringListFormat& theCurrentFormat)
{
	return theChar == ' ' || (TestBit(theCurrentFormat.mFormatFlags, TodStringFormatFlag::TOD_FORMAT_IGNORE_NEWLINES) && theChar == '\n');
}

//0x519870
int TodWriteString(Graphics* g, const SexyString& theString, int theX, int theY, TodStringListFormat& theCurrentFormat, int theWidth, DrawStringJustification theJustification, bool drawString, int theOffset, int theLength)
{
	Font* aFont = *theCurrentFormat.mNewFont;
	if (drawString)  // 如果需要实际绘制
	{
		int aSpareX = theWidth - TodWriteString(g, theString, theX, theY, theCurrentFormat, theWidth, DrawStringJustification::DS_ALIGN_LEFT, false, theOffset, theLength);
		switch (theJustification)  // 根据对齐方式调整实际绘制的横坐标
		{
		case DrawStringJustification::DS_ALIGN_RIGHT:
		case DrawStringJustification::DS_ALIGN_RIGHT_VERTICAL_MIDDLE:
			theX += aSpareX;
			break;
		case DrawStringJustification::DS_ALIGN_CENTER:
		case DrawStringJustification::DS_ALIGN_CENTER_VERTICAL_MIDDLE:
			theX += aSpareX / 2;
			break;
		}
	}

	if (theLength < 0 || theOffset + theLength > theString.size())
		theLength = theString.size();
	else
		theLength = theOffset + theLength;  // 将 theLength 更改为子串结束位置

	SexyString aString;
	int aXOffset = 0;
	bool aPrevCharWasSpace = false;
	for (int i = theOffset; i < theLength; i++)
	{
		if (theString[i] == '{')
		{
			const char* aFormatStart = theString.c_str() + i;
			const char* aFormatEnd = strchr(aFormatStart + 1, '}');
			if (aFormatEnd != nullptr)  // 如果存在完整的“{FORMAT}”控制字符
			{
				i += aFormatEnd - aFormatStart;  // i 移动至 "}" 处
				if (drawString)  // 如果需要实际绘制
					aFont->DrawString(g, theX + aXOffset, theY, aString, theCurrentFormat.mNewColor, g->mClipRect);  // 将已经积攒的字符进行绘制
				
				aXOffset += aFont->StringWidth(aString);  // 横向偏移值加上绘制的字符串的宽度
				aString.assign("");  // 清空字符串
				TodWriteStringSetFormat(aFormatStart + 1, theCurrentFormat);  // 根据当前控制字符调整格式
				Font* aFont = *theCurrentFormat.mNewFont;
			}
		}
		else
		{
			if (TestBit(theCurrentFormat.mFormatFlags, TodStringFormatFlag::TOD_FORMAT_IGNORE_NEWLINES))  // 如果将换行符视作空格
			{
				if (CharIsSpaceInFormat(theString[i], theCurrentFormat))  // 如果当前字符是空格
				{
					if (!aPrevCharWasSpace)  // 如果前一个字符不是空格
						aString.append(1, ' ');  // 积攒一个空格
					continue;
				}
				else
					aPrevCharWasSpace = false;  // 确保字符串中至多只能连续出现 1 个空格字符
			}

			aString.append(1, theString[i]);
		}
	}

	if (drawString)  // 如果需要实际绘制
		aFont->DrawString(g, theX + aXOffset, theY, aString, theCurrentFormat.mNewColor, g->mClipRect);  // 将已经积攒的字符进行绘制
	return aXOffset + aFont->StringWidth(aString);
}

int TodWriteWordWrappedHelper(Graphics* g, const SexyString& theString, int theX, int theY, TodStringListFormat& theCurrentFormat, int theWidth, DrawStringJustification theJustification, bool drawString, int theOffset, int theLength, int theMaxChars)
{
	if (theOffset + theLength > theMaxChars)  // 如果指定子串超出了字符串的最大长度
	{
		theLength = theMaxChars - theOffset;  // 修正子串长度
		if (theLength <= 0)
			return -1;
	}
	return TodWriteString(g, theString, theX, theY, theCurrentFormat, theWidth, theJustification, drawString, theOffset, theLength);
}

//0x519B50
int TodDrawStringWrappedHelper(Graphics* g, const SexyString& theText, const Rect& theRect, Font* theFont, const Color& theColor, DrawStringJustification theJustification, bool drawString)
{
	int theMaxChars = theText.size();
	TodStringListFormat aCurrentFormat;
	aCurrentFormat.mNewFont = &theFont;
	aCurrentFormat.mNewColor = theColor;
	aCurrentFormat.mFormatName = "";
	aCurrentFormat.mLineSpacingOffset = 0;
	aCurrentFormat.mFormatFlags = 0U;

	int aYOffset = theFont->GetAscent() - theFont->GetAscentPadding();
	int aLineSpacing = theFont->GetLineSpacing() + aCurrentFormat.mLineSpacingOffset;
	SexyString aCurString;
	int aLineFeedPos = 0;
	int aCurPos = 0;
	int aCurWidth = 0;
	SexyChar aCurChar = '\0';
	SexyChar aPrevChar = '\0';
	int aSpacePos = -1;
	int aMaxWidth = 0;
	while (aCurPos < theText.size())
	{
		aCurChar = theText[aCurPos];
		if (aCurChar == '{')  // 如果当前字符是特殊格式控制字符的起始标志（即“{”）
		{
			const char* aFmtStart = theText.c_str() + aCurPos;
			const char* aFormat = aFmtStart + 1;
			const char* aFmtEnd = strchr(aFormat, '}');
			if (aFmtEnd != nullptr)  // 如果存在与“{”对应的“}”，即存在完整的控制字符
			{
				aCurPos += aFmtEnd - aFmtStart + 1;  // aCurPos 移至“}”的下一个字符处
				int aOldAscentOffset = theFont->GetAscent() - theFont->GetAscentPadding();
				Color aExistingColor = aCurrentFormat.mNewColor;  // 备份当前格式的颜色
				TodWriteStringSetFormat(aFormat, aCurrentFormat);  // 根据当前控制字符设置新的格式
				aCurrentFormat.mNewColor = aExistingColor;  // 还原为原有格式的颜色
				int aNewAscentOffset = (*aCurrentFormat.mNewFont)->GetAscent() - (*aCurrentFormat.mNewFont)->GetAscentPadding();
				aLineSpacing = (*aCurrentFormat.mNewFont)->GetLineSpacing() + aCurrentFormat.mLineSpacingOffset;
				aYOffset += aNewAscentOffset - aOldAscentOffset;
				continue;
			}
		}
		else if (CharIsSpaceInFormat(aCurChar, aCurrentFormat))
		{
			aSpacePos = aCurPos;
			aCurChar = ' ';
		}
		else if (aCurChar == '\n')
		{
			aSpacePos = aCurPos;
			aCurWidth = theRect.mWidth + 1;
			aCurPos++;
		}

		aCurWidth += (*aCurrentFormat.mNewFont)->CharWidthKern(aCurChar, aPrevChar);  // 当前宽度加上当前字符的宽度
		aPrevChar = aCurChar;
		if (aCurWidth > theRect.mWidth)  // 如果当前宽度超出了限制区域的宽度，则进行换行的处理
		{
			int aLineWidth;
			if (aSpacePos != -1)  // 如果本行前面的字符中存在空格字符
			{
				int aCurY = (int)g->mTransY + theRect.mY + aYOffset;
				if (aCurY >= g->mClipRect.mY && aCurY <= g->mClipRect.mY + g->mClipRect.mHeight + aLineSpacing)  // 确保当前绘制位置纵坐标在裁剪范围内
				{
					TodWriteWordWrappedHelper(
						g, 
						theText, 
						theRect.mX, 
						theRect.mY + aYOffset,
						aCurrentFormat, 
						theRect.mWidth, 
						theJustification, 
						drawString, 
						aLineFeedPos, // 上次换行的位置即为新行开始的位置
						aSpacePos - aLineFeedPos, // 绘制部分为从上次换行的位置开始至本行空格字符之前的文本
						theMaxChars
					);  // 绘制新一行的文本（若需要）
				}

				aLineWidth = aCurWidth;
				if (aLineWidth < 0)  // 如果本行字符总宽度小于 0
					break;

				aCurPos = aSpacePos + 1;  // 将 aCurPos 移至下一行的开始处
				if (aCurChar != '\n')
					while (aCurPos < theText.size() && CharIsSpaceInFormat(theText[aCurPos], aCurrentFormat))
						aCurPos++;  // aCurPos 跳过所有连续的空白字符
			}
			else
			{
				if (aCurPos < aLineFeedPos + 1)
					aCurPos++;  // 确保每行至少有 1 个字符

				aLineWidth = TodWriteWordWrappedHelper(
					g,
					theText,
					theRect.mX,
					theRect.mY + aYOffset,
					aCurrentFormat, 
					theRect.mWidth, 
					theJustification, 
					drawString, 
					aLineFeedPos, // 上次换行的位置即为新行开始的位置
					aCurPos - aLineFeedPos, // 绘制部分为从上次换行的位置开始至当前位置的文本
					theMaxChars
				);  // 绘制新一行的文本（若需要）
				if (aLineWidth < 0)  // 如果本行字符总宽度小于 0
					break;
			}

			if (aLineWidth > aMaxWidth)
				aMaxWidth = aLineWidth;  // 更新最大行宽度
			aYOffset += aLineSpacing;
			aLineFeedPos = aCurPos;  // 记录当前位置为“上次换行的位置”
			aSpacePos = -1;
			aCurWidth = 0;
			aPrevChar = '\0';
		}
		else  // 当前宽度未超过限制区域宽度时
		{
			aCurPos++;  // 继续下一个字符
		}
	}

	if (aLineFeedPos < theText.size())
	{
		int aLastLineLength = TodWriteWordWrappedHelper(
			g,
			theText,
			theRect.mX,
			theRect.mY + aYOffset,
			aCurrentFormat,
			theRect.mWidth,
			theJustification,
			drawString,
			aLineFeedPos, // 上次换行的位置即为最后一行开始的位置
			theText.size() - aLineFeedPos, // 绘制部分为从上次换行的位置开始的所有剩余文本
			theMaxChars
		);  // 绘制最后一行的文本
		if (aLastLineLength >= 0)
			aYOffset += aLineSpacing;
	}
	else
		aYOffset += aLineSpacing;

	return (*aCurrentFormat.mNewFont)->GetDescent() + aYOffset - aLineSpacing;
}

//0x51A040
void TodDrawStringWrapped(Graphics* g, const SexyString& theText, const Rect& theRect, Font* theFont, const Color& theColor, DrawStringJustification theJustification)
{
	SexyString aTextFinal = TodStringTranslate(theText);
	Rect aRectTodUse = theRect;
	if (theJustification == DrawStringJustification::DS_ALIGN_LEFT_VERTICAL_MIDDLE ||
		theJustification == DrawStringJustification::DS_ALIGN_RIGHT_VERTICAL_MIDDLE ||
		theJustification == DrawStringJustification::DS_ALIGN_CENTER_VERTICAL_MIDDLE)  // 如果纵向需要居中
	{
		aRectTodUse.mY += (aRectTodUse.mHeight - TodDrawStringWrappedHelper(g, aTextFinal, aRectTodUse, theFont, theColor, theJustification, false)) / 2;
	}
	TodDrawStringWrappedHelper(g, aTextFinal, aRectTodUse, theFont, theColor, theJustification, true);
}
