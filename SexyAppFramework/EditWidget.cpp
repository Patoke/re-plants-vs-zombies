#include "EditWidget.h"
#include "SysFont.h"
#include "WidgetManager.h"
#include "SexyAppBase.h"
#include "EditListener.h"

using namespace Sexy;

static int gEditWidgetColors[][3] = 
{{255, 255, 255},
{0, 0, 0},
{0, 0, 0},
{0, 0, 0},
{255, 255, 255}};

EditWidget::EditWidget(int theId, EditListener* theEditListener)
{		
	mId = theId;
	mEditListener = theEditListener;	
	mFont = NULL;

	mHadDoubleClick = false;
	mHilitePos = -1;
	mLastModifyIdx = -1;
	mLeftPos = 0;
	mUndoCursor = 0;
	mUndoHilitePos = 0;
	mLastModifyIdx = 0;
	mBlinkAcc = 0;
	mCursorPos = 0;
	mShowingCursor = false;
	mDrawSelOverride = false;
	mMaxChars = -1;
	mMaxPixels = -1;
	mPasswordChar = 0;
	mBlinkDelay = 40;

	SetColors(gEditWidgetColors, NUM_COLORS);
}

EditWidget::~EditWidget()
{
	delete mFont;
	ClearWidthCheckFonts();

}

void EditWidget::ClearWidthCheckFonts()
{
	for (WidthCheckList::iterator anItr = mWidthCheckList.begin(); anItr != mWidthCheckList.end(); ++anItr)
		delete anItr->mFont;

	mWidthCheckList.clear();
}

void EditWidget::AddWidthCheckFont(Font *theFont, int theMaxPixels)
{
	mWidthCheckList.push_back(WidthCheck());
	WidthCheck &aCheck = mWidthCheckList.back();
	aCheck.mWidth = theMaxPixels;
	aCheck.mFont = theFont->Duplicate();
}

void EditWidget::SetText(const SexyString& theText, bool leftPosToZero)
{
	mString = theText;
	mCursorPos = mString.length();
	mHilitePos = 0;
	if (leftPosToZero)
		mLeftPos = 0;
	else
		FocusCursor(true);
	
	MarkDirty();
}

SexyString& EditWidget::GetDisplayString()
{
	if (mPasswordChar==0)
		return mString;

	if (mPasswordDisplayString.size()!=mString.size())
	{
		mPasswordDisplayString = SexyString(mString.size(), mPasswordChar);
		//mPasswordDisplayString.resize(mString.size());
		//for (int i=0; i<(int)mPasswordDisplayString.length(); i++)
		//	mPasswordDisplayString[i] = mPasswordChar; 
	}

	return mPasswordDisplayString;
}

bool EditWidget::WantsFocus()
{
	return true;
}

void EditWidget::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Widget::Resize(theX, theY, theWidth, theHeight);

	FocusCursor(false);		
}

void EditWidget::SetFont(Font* theFont, Font* theWidthCheckFont)
{
	delete mFont;
	mFont = theFont->Duplicate();

	ClearWidthCheckFonts();
	if (theWidthCheckFont != NULL)
		AddWidthCheckFont(theWidthCheckFont);
}

void EditWidget::Draw(Graphics* g) // Already translated
{	
	if (mFont == NULL)
		mFont = new SysFont(mWidgetManager->mApp, "Arial Unicode MS", 10, false);

	SexyString &aString = GetDisplayString();

	g->SetColor(mColors[COLOR_BKG]);			
	g->FillRect(0, 0, mWidth, mHeight);
	
	for (int i = 0; i < 2; i++)
	{
		Graphics* aClipG = g->Create();		
		aClipG->SetFont(mFont);
				
		if (i == 1)
		{
			int aCursorX = mFont->StringWidth(aString.substr(0, mCursorPos)) - mFont->StringWidth(aString.substr(0, mLeftPos));
			int aHiliteX = aCursorX+2;
			if ((mHilitePos != -1) && (mCursorPos != mHilitePos))
				aHiliteX = mFont->StringWidth(aString.substr(0, mHilitePos)) - mFont->StringWidth(aString.substr(0, mLeftPos));
			
			if (!mShowingCursor)
				aCursorX += 2;								
			
			aCursorX = min(max(0, aCursorX), mWidth-8);
			aHiliteX = min(max(0, aHiliteX), mWidth-8);
			
			aClipG->ClipRect(4 + min(aCursorX, aHiliteX), (mHeight - mFont->GetHeight())/2, abs(aHiliteX - aCursorX), mFont->GetHeight());
		}
		else
			aClipG->ClipRect(4, 0, mWidth-8, mHeight);			
		
		bool hasfocus = mHasFocus || mDrawSelOverride;
		if (i == 1 && hasfocus)
		{
			aClipG->SetColor(mColors[COLOR_HILITE]);
			aClipG->FillRect(0, 0, mWidth, mHeight);
		}
	
		if (i == 0 || !hasfocus)
			aClipG->SetColor(mColors[COLOR_TEXT]);
		else
			aClipG->SetColor(mColors[COLOR_HILITE_TEXT]);			
		aClipG->DrawString(aString.substr(mLeftPos), 4, (mHeight - mFont->GetHeight())/2 + mFont->GetAscent());
		
		delete aClipG;
	}		
			
	g->SetColor(mColors[COLOR_OUTLINE]);
	g->DrawRect(0, 0, mWidth-1, mHeight-1);				
}

void EditWidget::UpdateCaretPos()
{
	SexyAppBase *anApp = mWidgetManager->mApp;

	Point aPoint = GetAbsPos();

	if (aPoint.mX<10) aPoint.mX = 10;
	else if (aPoint.mX>anApp->mWidth-10) aPoint.mX = anApp->mWidth-10;
	if (aPoint.mY<10) aPoint.mY = 10;
	else if (aPoint.mY>anApp->mHeight-10) aPoint.mY = anApp->mHeight-10;

	SetCaretPos(aPoint.mX,aPoint.mY);
}

void EditWidget::GotFocus()
{
	Widget::GotFocus();
	if (mWidgetManager && mWidgetManager->mApp->mTabletPC)
	{
		SexyAppBase *anApp = mWidgetManager->mApp;

		CreateCaret(anApp->mHWnd,NULL,0,0);
		UpdateCaretPos();
		ShowCaret(anApp->mHWnd);
	}
	
	mShowingCursor = true;
	mBlinkAcc = 0;
	MarkDirty();	
}

void EditWidget::LostFocus()
{
	Widget::LostFocus();

	if (mWidgetManager && mWidgetManager->mApp->mTabletPC)
	{
		HideCaret(mWidgetManager->mApp->mHWnd);
		DestroyCaret();
	}

	mShowingCursor = false;	
	MarkDirty();
}

void EditWidget::Update()
{
	Widget::Update();

	if (mHasFocus)
	{
		if (mWidgetManager->mApp->mTabletPC)
		{
			UpdateCaretPos();
		}

		if (++mBlinkAcc > mBlinkDelay)
		{
			MarkDirty();
			mBlinkAcc = 0;
			mShowingCursor = !mShowingCursor;			
		}		
	}	
}

void EditWidget::EnforceMaxPixels()
{
	if (mMaxPixels<=0 && mWidthCheckList.empty()) // no width checking in effect
		return;

	if (mWidthCheckList.empty())
	{
		while (mFont->StringWidth(mString) > mMaxPixels)
			mString = mString.substr(0, mString.length()-1);

		return;
	}
		
	for (WidthCheckList::iterator anItr = mWidthCheckList.begin(); anItr != mWidthCheckList.end(); ++anItr)
	{
		int aWidth = anItr->mWidth;
		if (aWidth<=0)
		{
			aWidth = mMaxPixels;
			if (aWidth<=0)
				continue;
		}

		while (anItr->mFont->StringWidth(mString) > aWidth)
			mString = mString.substr(0,mString.length()-1);
	} 
}

bool EditWidget::IsPartOfWord(SexyChar theChar)
{
	return (((theChar >= _S('A')) && (theChar <= _S('Z'))) ||
			((theChar >= _S('a')) && (theChar <= _S('z'))) ||
			((theChar >= _S('0')) && (theChar <= _S('9'))) ||
			(((unsigned int)theChar >= (unsigned int)(L'?')) && ((unsigned int)theChar <= (unsigned int)(L'ÿ'))) ||
			(theChar == _S('_')));
}

void EditWidget::ProcessKey(KeyCode theKey, SexyChar theChar)
{
	bool shiftDown = mWidgetManager->mKeyDown[KEYCODE_SHIFT];
	bool controlDown = mWidgetManager->mKeyDown[KEYCODE_CONTROL];

	if ((theKey == KEYCODE_SHIFT) || (theKey == KEYCODE_CONTROL))
		return;

	bool bigChange = false;
	bool removeHilite = !shiftDown;
	
	if (shiftDown && (mHilitePos == -1))
		mHilitePos = mCursorPos;
	
	SexyString anOldString = mString;
	int anOldCursorPos = mCursorPos;
	int anOldHilitePos = mHilitePos;
	if ((theChar == 3) || (theChar == 24))
	{
		// Copy	selection
		
		if ((mHilitePos != -1) && (mHilitePos != mCursorPos))
		{
			if (mCursorPos < mHilitePos)
				mWidgetManager->mApp->CopyToClipboard(SexyStringToString(GetDisplayString().substr(mCursorPos, mHilitePos)));
			else
				mWidgetManager->mApp->CopyToClipboard(SexyStringToString(GetDisplayString().substr(mHilitePos, mCursorPos)));
		
			if (theChar == 3)
			{				
				removeHilite = false;
			}
			else
			{
				mString = mString.substr(0, min(mCursorPos, mHilitePos)) + mString.substr(max(mCursorPos, mHilitePos));
				mCursorPos = min(mCursorPos, mHilitePos);
				mHilitePos = -1;
				bigChange = true;
			}
		}				
	}
	else if (theChar == 22)
	{
		// Paste selection
		
		SexyString aBaseString = StringToSexyString(mWidgetManager->mApp->GetClipboard());
		
		if (aBaseString.length() > 0)
		{	
			SexyString aString;

			for (ulong i = 0; i < aBaseString.length(); i++)
			{
				if ((aBaseString[i] == '\r') || (aBaseString[i] == '\n'))
					break;

				if (mFont->CharWidth(aBaseString[i]) != 0 && mEditListener->AllowChar(mId, aBaseString[i]))
					aString += aBaseString[i];					
			}			

			if (mHilitePos == -1)
			{
				// Insert string where cursor is
				mString = mString.substr(0, mCursorPos) + aString + mString.substr(mCursorPos);
			}
			else
			{
				// Replace selection with new string
				mString = mString.substr(0, min(mCursorPos, mHilitePos)) + aString + mString.substr(max(mCursorPos, mHilitePos));
				mCursorPos = min(mCursorPos, mHilitePos);
				mHilitePos = -1;
			}
		
			mCursorPos += aString.length();
		
			bigChange = true;
		}
	}
	else if (theChar == 26)
	{
		// Undo
		
		mLastModifyIdx = -1;
		
		SexyString aSwapString = mString;
		int aSwapCursorPos = mCursorPos;
		int aSwapHilitePos = mHilitePos;			
		
		mString = mUndoString;
		mCursorPos = mUndoCursor;
		mHilitePos = mUndoHilitePos;
					
		mUndoString = aSwapString;
		mUndoCursor = aSwapCursorPos;
		mUndoHilitePos = aSwapHilitePos;			
		
		removeHilite = false;						
	}
	else if (theKey == KEYCODE_LEFT)
	{
		if (controlDown)
		{
			// Get to a word
			while ((mCursorPos > 0) && (!IsPartOfWord(mString[mCursorPos-1])))
				   mCursorPos--;
			
			// Go beyond the word
			while ((mCursorPos > 0) && (IsPartOfWord(mString[mCursorPos-1])))
				   mCursorPos--;
		}
		else if (shiftDown || (mHilitePos == -1))
			mCursorPos--;
		else
			mCursorPos = min(mCursorPos, mHilitePos);
	}
	else if (theKey == KEYCODE_RIGHT)
	{
		if (controlDown)
		{
			// Get to whitespace
			while ((mCursorPos < (int) mString.length()-1) && (IsPartOfWord(mString[mCursorPos+1])))
				   mCursorPos++;
			
			// Go beyond the whitespace
			while ((mCursorPos < (int) mString.length()-1) && (!IsPartOfWord(mString[mCursorPos+1])))
				   mCursorPos++;
		}
		if (shiftDown || (mHilitePos == -1))
			mCursorPos++;
		else
			mCursorPos = max(mCursorPos, mHilitePos);
	}
	else if (theKey == KEYCODE_BACK)
	{
		if (mString.length() > 0)
		{
			if ((mHilitePos != -1) && (mHilitePos != mCursorPos))
			{
				// Delete selection
				mString = mString.substr(0, min(mCursorPos, mHilitePos)) + mString.substr(max(mCursorPos, mHilitePos));
				mCursorPos = min(mCursorPos, mHilitePos);
				mHilitePos = -1;
				
				bigChange = true;
			}
			else
			{
				// Delete char behind cursor
				if (mCursorPos > 0)
					mString = mString.substr(0, mCursorPos-1) + mString.substr(mCursorPos);
				else
					mString = mString.substr(mCursorPos);
				mCursorPos--;
				mHilitePos = -1;
				
				if (mCursorPos != mLastModifyIdx)
					bigChange = true;
				mLastModifyIdx = mCursorPos-1;
			}
		}
	}
	else if (theKey == KEYCODE_DELETE)
	{
		if (mString.length() > 0)
		{
			if ((mHilitePos != -1) && (mHilitePos != mCursorPos))
			{
				// Delete selection
				mString = mString.substr(0, min(mCursorPos, mHilitePos)) + mString.substr(max(mCursorPos, mHilitePos));
				mCursorPos = min(mCursorPos, mHilitePos);
				mHilitePos = -1;
				
				bigChange = true;
			}
			else
			{
				// Delete char in front of cursor
				if (mCursorPos < (int) mString.length())
					mString = mString.substr(0, mCursorPos) + mString.substr(mCursorPos+1);
				
				if (mCursorPos != mLastModifyIdx)
					bigChange = true;
				mLastModifyIdx = mCursorPos;
			}
		}	
	}
	else if (theKey == KEYCODE_HOME)
	{
		mCursorPos = 0;	
	}
	else if (theKey == KEYCODE_END)
	{
		mCursorPos = mString.length();	
	}
	else if (theKey == KEYCODE_RETURN)
	{
		mEditListener->EditWidgetText(mId, mString);		
	}
	else
	{
		SexyString aString = SexyString(1, theChar);
		unsigned int uTheChar = (unsigned int)theChar;
		unsigned int range = 127;
		if (gSexyAppBase->mbAllowExtendedChars)
		{
			range = 255;
		}

		if ((uTheChar >= 32) && (uTheChar <= range) && (mFont->StringWidth(aString) > 0) && mEditListener->AllowChar(mId, theChar))
		{				
			if ((mHilitePos != -1) && (mHilitePos != mCursorPos))
			{
				// Replace selection with new character
				mString = mString.substr(0, min(mCursorPos, mHilitePos)) + SexyString(1, theChar) + mString.substr(max(mCursorPos, mHilitePos));
				mCursorPos = min(mCursorPos, mHilitePos);
				mHilitePos = -1;
				
				bigChange = true;
			}
			else
			{
				// Insert character where cursor is
				mString = mString.substr(0, mCursorPos) + SexyString(1, theChar) + mString.substr(mCursorPos);
				
				if (mCursorPos != mLastModifyIdx+1)
					bigChange = true;						
				mLastModifyIdx = mCursorPos;
				mHilitePos = -1;
			}
											
			mCursorPos++;				
			FocusCursor(false);
		}
		else
			removeHilite = false;
	}
	
	if ((mMaxChars != -1) && ((int) mString.length() > mMaxChars))
		mString = mString.substr(0, mMaxChars);

	EnforceMaxPixels();

	if (mCursorPos < 0)
		mCursorPos = 0;
	else if (mCursorPos > (int) mString.length())
		mCursorPos = mString.length();
	
	if (anOldCursorPos != mCursorPos)
	{
		mBlinkAcc = 0;
		mShowingCursor = true;
	}
	
	FocusCursor(true);
	
	if (removeHilite || mHilitePos==mCursorPos)
		mHilitePos = -1;
	
	if (!mEditListener->AllowText(mId, mString))
	{
		mString = anOldString;
		mCursorPos = anOldCursorPos;
		mHilitePos = anOldHilitePos;
	}
	else if (bigChange)
	{
		mUndoString = anOldString;
		mUndoCursor = anOldCursorPos;
		mUndoHilitePos = anOldHilitePos;
	}
	
	MarkDirty();
}

void EditWidget::KeyDown(KeyCode theKey)
{
	if (((theKey < 'A') || (theKey >= 'Z')) && mEditListener->AllowKey(mId, theKey))
		ProcessKey(theKey, 0);

	Widget::KeyDown(theKey);
}

void EditWidget::KeyChar(SexyChar theChar)
{
//	if (mEditListener->AllowChar(mId, theChar))
		ProcessKey(KEYCODE_UNKNOWN, theChar);

	Widget::KeyChar(theChar);
}

int EditWidget::GetCharAt(int x, int y)
{
	int aPos = 0;

	SexyString &aString = GetDisplayString();
					
	for (int i = mLeftPos; i < (int) aString.length(); i++)
	{
		SexyString aLoSubStr = aString.substr(mLeftPos, i-mLeftPos);
		SexyString aHiSubStr = aString.substr(mLeftPos, i-mLeftPos+1);
			
		int aLoLen = mFont->StringWidth(aLoSubStr);
		int aHiLen = mFont->StringWidth(aHiSubStr);
		if (x >= (aLoLen+aHiLen)/2 + 5)				
			aPos = i+1;	
	}					
	
	return aPos;
}

void EditWidget::FocusCursor(bool bigJump)
{
	while (mCursorPos < mLeftPos)
	{
		if (bigJump)
			mLeftPos = max(0, mLeftPos-10);
		else
			mLeftPos = max(0, mLeftPos-1);
		MarkDirty();
	}				
					
	if (mFont != NULL)
	{
		SexyString &aString = GetDisplayString();
		while ((mWidth-8 > 0) && (mFont->StringWidth(aString.substr(0, mCursorPos)) - mFont->StringWidth(aString.substr(0, mLeftPos)) >= mWidth-8))
		{
			if (bigJump)
				mLeftPos = min(mLeftPos + 10, (int) mString.length()-1);
			else
				mLeftPos = min(mLeftPos + 1, (int) mString.length()-1);

			MarkDirty();
		}
	}
}

void EditWidget::MouseDown(int x, int y, int theBtnNum, int theClickCount)
{
	Widget::MouseDown(x, y, theBtnNum, theClickCount);

	mHilitePos = -1;
	mCursorPos = GetCharAt(x, y);
	
	if (theClickCount > 1)
	{
		mHadDoubleClick = true;
		HiliteWord();
	}
	
	MarkDirty();
	
	FocusCursor(false);
}

void EditWidget::MouseUp(int x, int y, int theBtnNum, int theClickCount)
{
	Widget::MouseUp(x,y,theBtnNum,theClickCount);
	if (mHilitePos==mCursorPos)
		mHilitePos = -1;
	
	if (mHadDoubleClick)
	{		
		mHilitePos = -1;
		mCursorPos = GetCharAt(x, y);		

		mHadDoubleClick = false;
		HiliteWord();
	}

	MarkDirty();
}

void EditWidget::HiliteWord()
{
	SexyString &aString = GetDisplayString();

	if (mCursorPos < (int) aString.length())
	{
		// Find first space before word
		mHilitePos = mCursorPos;
		while ((mHilitePos > 0) && (IsPartOfWord(aString[mHilitePos-1])))
			mHilitePos--;

		// Find first space after word
		while ((mCursorPos < (int) aString.length()-1) && (IsPartOfWord(aString[mCursorPos+1])))
			mCursorPos++;
		if (mCursorPos < (int) aString.length())
			mCursorPos++;
	}
}

void EditWidget::MouseDrag(int x, int y)
{
	Widget::MouseDrag(x, y);

	if (mHilitePos == -1)
		mHilitePos = mCursorPos;
	
	mCursorPos = GetCharAt(x, y);
	MarkDirty();
	
	FocusCursor(false);
}

void EditWidget::MouseEnter()
{
	Widget::MouseEnter();

	mWidgetManager->mApp->SetCursor(CURSOR_TEXT);
}

void EditWidget::MouseLeave()
{	
	Widget::MouseLeave();

	mWidgetManager->mApp->SetCursor(CURSOR_POINTER);
}

void EditWidget::MarkDirty()
{
	if (mColors[COLOR_BKG].mAlpha != 255)
		Widget::MarkDirtyFull();
	else
		Widget::MarkDirty();
}
