#ifndef __TEXTWIDGET_H__
#define __TEXTWIDGET_H__

#include "Widget.h"
#include "ScrollListener.h"

namespace Sexy
{

class ScrollbarWidget;
class Font;

typedef std::vector<SexyString> SexyStringVector;
typedef std::vector<int> IntVector;

class TextWidget : public Widget, public ScrollListener
{
public:
	Font*				mFont;
	ScrollbarWidget*	mScrollbar;		
	
	SexyStringVector	mLogicalLines;
	SexyStringVector	mPhysicalLines;	
	IntVector			mLineMap;
	double				mPosition;
	double				mPageSize;	
	bool				mStickToBottom;	
	int					mHiliteArea[2][2];
	int					mMaxLines;
	
public:
	TextWidget();

	virtual SexyStringVector GetLines();
	virtual void SetLines(SexyStringVector theNewLines);	
	virtual void Clear();		
	virtual void DrawColorString(Graphics* g, const SexyString& theString, int x, int y, bool useColors);			
	virtual void DrawColorStringHilited(Graphics* g, const SexyString& theString, int x, int y, int theStartPos, int theEndPos);		
	virtual int GetStringIndex(const SexyString& theString, int thePixel);
	
	virtual int GetColorStringWidth(const SexyString& theString);
	virtual void Resize(int theX, int theY, int theWidth, int theHeight);		
	virtual Color GetLastColor(const SexyString& theString);		
	virtual void AddToPhysicalLines(int theIdx, const SexyString& theLine);
	
	virtual void AddLine(const SexyString& theString);
	virtual bool SelectionReversed();		
	virtual void GetSelectedIndices(int theLineIdx, int* theIndices);		
	virtual void Draw(Graphics* g);
	virtual void ScrollPosition(int theId, double thePosition);		
	virtual void GetTextIndexAt(int x, int y, int* thePosArray);
	virtual SexyString GetSelection();		

	virtual void MouseDown(int x, int y, int theClickCount);		
	virtual void MouseDrag(int x, int y);	
	
	virtual void KeyDown(KeyCode theKey);
};

}

#endif //__TEXTWIDGET_H__