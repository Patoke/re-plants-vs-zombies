#include "Widget.h"
#include "WidgetManager.h"
#include "Graphics.h"
#include "Font.h"
#include "Image.h"
#include "SexyAppBase.h"
#include "debug.h"

using namespace Sexy;

bool Widget::mWriteColoredString = true;

Widget::Widget()
{
	mWidgetManager = NULL;	
	mVisible = true;
	mDisabled = false;
	mIsDown = false;
	mIsOver = false;
	mDoFinger = false;	
	mMouseVisible = true;		
	mHasFocus = false;
	mHasTransparencies = false;	
	mWantsFocus = false;
	mTabPrev = NULL;
	mTabNext = NULL;
}

Widget::~Widget()
{	
	mColors.clear();
}

void Widget::WidgetRemovedHelper()
{
	if (mWidgetManager==NULL)
		return;

	// Call RemovedFromManager on all child widgets and disable them and stuff like that
	for (WidgetList::iterator aWidgetItr = mWidgets.begin(); aWidgetItr != mWidgets.end(); ++aWidgetItr)
	{
		Widget *aWidget = *aWidgetItr;
		aWidget->WidgetRemovedHelper();
	}	

	mWidgetManager->DisableWidget(this);

	PreModalInfoList::iterator anItr = mWidgetManager->mPreModalInfoList.begin();
	while (anItr != mWidgetManager->mPreModalInfoList.end())
	{
		PreModalInfo* aPreModalInfo = &(*anItr);
		if (aPreModalInfo->mPrevBaseModalWidget == this)
			aPreModalInfo->mPrevBaseModalWidget = NULL;
		if (aPreModalInfo->mPrevFocusWidget == this)
			aPreModalInfo->mPrevFocusWidget = NULL;
		++anItr;
	}
	
	RemovedFromManager(mWidgetManager);
	MarkDirtyFull(this);

	mWidgetManager = NULL;
}

void Widget::OrderInManagerChanged()
{
}

bool Widget::IsPointVisible(int x, int y)
{
	return true;
}

void Widget::SetVisible(bool isVisible)
{
	if (mVisible == isVisible)
		return;
	
	mVisible = isVisible;
	
	if (mVisible)
		MarkDirty();
	else
		MarkDirtyFull();

	if (mWidgetManager != NULL)
		mWidgetManager->RehupMouse();
}

void Widget::Draw(Graphics* g) // Already translated
{
}

void Widget::DrawOverlay(Graphics* g)
{
}

void Widget::DrawOverlay(Graphics* g, int thePriority)
{
	DrawOverlay(g);
}

void Widget::SetColors(int theColors[][3], int theNumColors)
{
	mColors.clear();

	for (int i = 0; i < theNumColors; i++)
		SetColor(i, Color(theColors[i][0], theColors[i][1], theColors[i][2]));
	MarkDirty();
}

void Widget::SetColors(int theColors[][4], int theNumColors)
{	
	mColors.clear();

	for (int i = 0; i < theNumColors; i++)
		SetColor(i, Color(theColors[i][0], theColors[i][1], theColors[i][2], theColors[i][3]));		

	MarkDirty();
}

void Widget::SetColor(int theIdx, const Color& theColor)
{
	if (theIdx >= (int)mColors.size())
		mColors.resize(theIdx + 1);

	mColors[theIdx] = theColor;
	MarkDirty();
}

const Color& Widget::GetColor(int theIdx)
{
	static Color aColor;
	if (theIdx < (int) mColors.size())
		return mColors[theIdx];
	return aColor;
}

Color Widget::GetColor(int theIdx, const Color& theDefaultColor)
{
	if (theIdx < (int) mColors.size())
		return mColors[theIdx];
	return theDefaultColor;
}

void Widget::Resize(int theX, int theY, int theWidth, int theHeight)
{
	if ((mX == theX) && (mY == theY) && (mWidth == theWidth) && (mHeight == theHeight))
		return;

	// Mark everything dirty that is over or under the old position
	MarkDirtyFull();
	
	mX = theX;
	mY = theY;
	mWidth = theWidth;
	mHeight = theHeight;
		
	// Mark things dirty that are over the new position
	MarkDirty();

	if (mWidgetManager != NULL)
		mWidgetManager->RehupMouse();
}

void Widget::Resize(const Rect& theRect)
{
	Resize(theRect.mX, theRect.mY, theRect.mWidth, theRect.mHeight);
}

void Widget::Move(int theNewX, int theNewY)
{
	Resize(theNewX, theNewY, mWidth, mHeight);
}

bool Widget::WantsFocus()
{
	return mWantsFocus;
}

void Widget::SetDisabled(bool isDisabled)
{
	if (mDisabled == isDisabled)
		return;

	mDisabled = isDisabled;

	if ((isDisabled) && (mWidgetManager != NULL))
		mWidgetManager->DisableWidget(this);
		
	MarkDirty();
	
	// Incase a widget is enabled right under our cursor
	if ((!isDisabled) && (mWidgetManager != NULL) && (Contains(mWidgetManager->mLastMouseX, mWidgetManager->mLastMouseY)))
		mWidgetManager->MousePosition(mWidgetManager->mLastMouseX, mWidgetManager->mLastMouseY);
}

void Widget::GotFocus()
{
	mHasFocus = true;		
}

void Widget::LostFocus()
{
	mHasFocus = false;		
}

void Widget::Update()
{
	WidgetContainer::Update();
}

void Widget::UpdateF(float theFrac)
{
}

void Widget::KeyChar(SexyChar theChar)
{
}

void Widget::KeyDown(KeyCode theKey)
{
	if (theKey == KEYCODE_TAB)
	{
		if (mWidgetManager->mKeyDown[KEYCODE_SHIFT])
		{
			if (mTabPrev != NULL)
				mWidgetManager->SetFocus(mTabPrev);
		}
		else
		{
			if (mTabNext != NULL)
				mWidgetManager->SetFocus(mTabNext);
		}
	}
}

void Widget::KeyUp(KeyCode theKey)
{		
}

void Widget::ShowFinger(bool on)
{
	if (mWidgetManager == NULL)
		return;

	if (on)
		mWidgetManager->mApp->SetCursor(CURSOR_HAND);
	else
		mWidgetManager->mApp->SetCursor(CURSOR_POINTER);

	/*if (on)
		mWidgetManager->mApplet.setCursor(new Cursor(Cursor.HAND_CURSOR));
	else
		mWidgetManager->mApplet.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));*/
}

void Widget::MouseEnter()
{
	
}

void Widget::MouseLeave()
{
	
}

void Widget::MouseMove(int x, int y)
{
}

void Widget::MouseDown(int x, int y, int theClickCount)
{
	if (theClickCount == 3)
		MouseDown(x, y, 2, 1);
	else if (theClickCount >= 0)
		MouseDown(x, y, 0, theClickCount);
	else
		MouseDown(x, y, 1, -theClickCount);
}

void Widget::MouseDown(int x, int y, int theBtnNum, int theClickCount)
{
}

void Widget::MouseUp(int x, int y)
{
}

void Widget::MouseUp(int x, int y, int theLastDownButtonId)
{
	MouseUp(x, y);

	if (theLastDownButtonId == 3)
		MouseUp(x, y, 2, 1);
	else if (theLastDownButtonId >= 0)
		MouseUp(x, y, 0, theLastDownButtonId);
	else
		MouseUp(x, y, 1, -theLastDownButtonId);
}

void Widget::MouseUp(int x, int y, int theBtnNum, int theClickCount)
{
}

void Widget::MouseDrag(int x, int y)
{
}

void Widget::MouseWheel(int theDelta)
{
}

//////// Helper functions

Rect Widget::WriteCenteredLine(Graphics* g, int anOffset, const SexyString& theLine)
{
	Font* aFont = g->GetFont();
	int aWidth = aFont->StringWidth(theLine);
	int aX = (mWidth - aWidth) / 2;

	g->DrawString(theLine, aX, anOffset);

	return Rect(aX, anOffset - aFont->GetAscent(), aWidth, aFont->GetHeight());
}

Rect Widget::WriteCenteredLine(Graphics* g, int anOffset, const SexyString& theLine, Color theColor1, Color theColor2, const Point& theShadowOffset)
{
	Font* aFont = g->GetFont();
	int aWidth = aFont->StringWidth(theLine);
	int aX = (mWidth - aWidth) / 2;
	
	g->SetColor(theColor2);
	g->DrawString(theLine, (mWidth - aWidth)/2 + theShadowOffset.mX, anOffset + theShadowOffset.mY);
	
	g->SetColor(theColor1);
	g->DrawString(theLine, (mWidth - aWidth)/2, anOffset);

	// account for shadow in position and size
	// TODO: this may not be necessary.
	return Rect(
		aX + min(0,theShadowOffset.mX),
		anOffset - aFont->GetAscent() + min(0,theShadowOffset.mY), 
		aWidth + abs(theShadowOffset.mX), 
		aFont->GetHeight() + abs(theShadowOffset.mY));
}

int Widget::WriteString(Graphics* g, const SexyString& theString, int theX, int theY, int theWidth, int theJustification, bool drawString, int theOffset, int theLength)
{
	bool oldColored = g->mWriteColoredString;
	g->mWriteColoredString = mWriteColoredString;
	int aXOffset = g->WriteString(theString,theX,theY,theWidth,theJustification,drawString,theOffset,theLength);
	g->mWriteColoredString = oldColored;

	return aXOffset;
}

int	Widget::WriteWordWrapped(Graphics* g, const Rect& theRect, const SexyString& theLine, int theLineSpacing, int theJustification)
{
	bool oldColored = g->mWriteColoredString;
	g->mWriteColoredString = mWriteColoredString;
	int aReturn = g->WriteWordWrapped(theRect,theLine,theLineSpacing,theJustification);
	g->mWriteColoredString = oldColored;

	return aReturn;
}

int Widget::GetWordWrappedHeight(Graphics* g, int theWidth, const SexyString& theLine, int aLineSpacing)
{
	return g->GetWordWrappedHeight(theWidth,theLine,aLineSpacing);
}

int Widget::GetNumDigits(int theNumber)
{		
	int aDivisor = 10;
	int aNumDigits = 1;
	while (theNumber >= aDivisor)
	{
		aNumDigits++;
		aDivisor *= 10;
	}			
		
	return aNumDigits;
}

void Widget::WriteNumberFromStrip(Graphics* g, int theNumber, int theX, int theY, Image* theNumberStrip, int aSpacing)
{
	int aDivisor = 10;
	int aNumDigits = 1;
	while (theNumber >= aDivisor)
	{
		aNumDigits++;
		aDivisor *= 10;
	}
	if (theNumber == 0)
		aDivisor = 10;

	int aDigitLen = theNumberStrip->GetWidth() / 10;
	
	for (int aDigitIdx = 0; aDigitIdx < aNumDigits; aDigitIdx++)
	{				
		aDivisor /= 10;
		int aDigit = (theNumber / aDivisor) % 10;				
			
		Graphics* aClipG = g->Create();
		aClipG->ClipRect(theX + aDigitIdx*(aDigitLen + aSpacing), theY, aDigitLen, theNumberStrip->GetHeight());
		aClipG->DrawImage(theNumberStrip, theX + aDigitIdx*(aDigitLen + aSpacing) - aDigit*aDigitLen, theY);		
		delete aClipG;
	}
}										 
								 
bool Widget::Contains(int theX, int theY)
{
	return ((theX >= mX) && (theX < mX + mWidth) &&
			(theY >= mY) && (theY < mY + mHeight));
}

Rect Widget::GetInsetRect()
{
	return Rect(mX + mMouseInsets.mLeft, mY + mMouseInsets.mTop, 
						 mWidth - mMouseInsets.mLeft - mMouseInsets.mRight,
						 mHeight - mMouseInsets.mTop - mMouseInsets.mBottom);
}

void Widget::DeferOverlay(int thePriority)
{
	mWidgetManager->DeferOverlay(this, thePriority);
}

void Widget::Layout(int theLayoutFlags, Widget *theRelativeWidget, int theLeftPad, int theTopPad, int theWidthPad, int theHeightPad)
{
	int aRelLeft = theRelativeWidget->Left();
	int aRelTop = theRelativeWidget->Top();
	if (theRelativeWidget==mParent)
	{
		aRelLeft = 0;
		aRelTop = 0;
	}

	int aRelWidth = theRelativeWidget->Width();
	int aRelHeight = theRelativeWidget->Height();
	int aRelRight = aRelLeft + aRelWidth;
	int aRelBottom = aRelTop + aRelHeight;

	int aLeft = Left();
	int aTop = Top();
	int aWidth = Width();
	int aHeight = Height();

	int aType = 1;
	while(aType<LAY_Max)
	{
		if(theLayoutFlags&aType)
		{
			switch(aType)
			{
				case LAY_SameWidth: aWidth = aRelWidth+theWidthPad; break;
				case LAY_SameHeight: aHeight = aRelHeight+theHeightPad; break;
	
				case LAY_Above: aTop = aRelTop-aHeight+theTopPad; break;
				case LAY_Below: aTop = aRelBottom+theTopPad; break;
				case LAY_Right: aLeft = aRelRight+theLeftPad; break;
				case LAY_Left:  aLeft = aRelLeft-aWidth+theLeftPad; break;
			
				case LAY_SameLeft: aLeft = aRelLeft+theLeftPad; break;
				case LAY_SameRight: aLeft = aRelRight-aWidth+theLeftPad; break;
				case LAY_SameTop: aTop = aRelTop+theTopPad; break;
				case LAY_SameBottom: aTop = aRelBottom-aHeight+theTopPad; break;

				case LAY_GrowToRight: aWidth = aRelRight-aLeft+theWidthPad; break;
				case LAY_GrowToLeft: aWidth = aRelLeft-aLeft+theWidthPad; break;
				case LAY_GrowToTop: aHeight = aRelTop-aTop+theHeightPad; break;
				case LAY_GrowToBottom: aHeight = aRelBottom-aTop+theHeightPad; break;

				case LAY_SetLeft: aLeft = theLeftPad; break;
				case LAY_SetTop: aTop = theTopPad; break;
				case LAY_SetWidth: aWidth = theWidthPad; break;
				case LAY_SetHeight: aHeight = theHeightPad; break;

				case LAY_HCenter: aLeft = aRelLeft+(aRelWidth-aWidth)/2 + theLeftPad; break;
				case LAY_VCenter: aTop = aRelTop+(aRelHeight-aHeight)/2 + theTopPad; break;
			}
		}

		aType<<=1;
	}

	Resize(aLeft,aTop,aWidth,aHeight);
}
