#ifndef __TODSTRINGFILE_H__
#define __TODSTRINGFILE_H__

#include "graphics/Graphics.h"
#include "../ConstEnums.h"
using namespace Sexy;

//enum DrawStringJustification;
enum TodStringFormatFlag
{
    TOD_FORMAT_IGNORE_NEWLINES,
    TOD_FORMAT_HIDE_UNTIL_MAGNETSHROOM
};

class TodStringListFormat
{
public:
    const char*     mFormatName;
    _Font**          mNewFont;
    Color           mNewColor;
    int             mLineSpacingOffset;
    unsigned int    mFormatFlags;

public:
    TodStringListFormat();
    TodStringListFormat(const char* theFormatName, _Font** theFont, const Color& theColor, int theLineSpacingOffset, unsigned int theFormatFlags);
};
extern int gTodStringFormatCount;               //[0x69DE4C]
extern TodStringListFormat* gTodStringFormats;  //[0x69DA34]

extern const int gLawnStringFormatCount;
extern TodStringListFormat gLawnStringFormats[12];  //0x6A5010

void                TodStringListSetColors(TodStringListFormat* theFormats, int theCount);
void                TodWriteStringSetFormat(const char* theFormat, TodStringListFormat& theCurrentFormat);
bool                TodStringListReadName(const char*& thePtr, std::string& theName);
bool                TodStringListReadValue(const char*& thePtr, std::string& theValue);
bool                TodStringListReadItems(const char* theFileText);
bool                TodStringListReadFile(const char* theFileName);
void                TodStringListLoad(const char* theFileName);
SexyString          TodStringListFind(const SexyString& theName);
SexyString			TodStringTranslate(const SexyString& theString);
SexyString			TodStringTranslate(const SexyChar* theString);
bool                TodStringListExists(const SexyString& theString);
void                TodStringRemoveReturnChars(std::string& theString);
bool                CharIsSpaceInFormat(char theChar, const TodStringListFormat& theCurrentFormat);
int                 TodWriteString(Graphics* g, const SexyString& theString, int theX, int theY, TodStringListFormat& theCurrentFormat, int theWidth, DrawStringJustification theJustification, bool drawString, int theOffset, int theLength);
/*inline*/ int      TodWriteWordWrappedHelper(Graphics* g, const SexyString& theString, int theX, int theY, TodStringListFormat& theCurrentFormat, int theWidth, DrawStringJustification theJustification, bool drawString, int theOffset, int theLength, int theMaxChars);
int                 TodDrawStringWrappedHelper(Graphics* g, const SexyString& theText, const Rect& theRect, _Font* theFont, const Color& theColor, DrawStringJustification theJustification, bool drawString);
/*inline*/ void		TodDrawStringWrapped(Graphics* g, const SexyString& theText, const Rect& theRect, _Font* theFont, const Color& theColor, DrawStringJustification theJustification);

#endif  //__TODSTRINGFILE_H__