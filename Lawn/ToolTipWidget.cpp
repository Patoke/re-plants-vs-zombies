#include "../Resources.h"
#include "ToolTipWidget.h"
#include "../GameConstants.h"
#include "../Sexy.TodLib/TodCommon.h"
#include "graphics/Font.h"
#include "../Sexy.TodLib/TodStringFile.h"

using namespace Sexy;

ToolTipWidget::ToolTipWidget()
{
	mX = 0;
	mY = 0;
	mWidth = 0;
	mHeight = 0;
	mVisible = 1;
	mCenter = 0;
	mMinLeft = 0;
	mMaxBottom = BOARD_HEIGHT;
	mGetsLinesWidth = 0;
	mWarningFlashCounter = 0;
}

//0x51A530
void ToolTipWidget::GetLines(std::vector<SexyString>& theLines)
{
	int aLineWidth = 0;
	unsigned int aIndexStart = 0;
	unsigned int aIndexInLine = 0;

	while (aIndexInLine != mLabel.size())
	{
		while (aIndexInLine < mLabel.size() && mLabel[aIndexInLine] != ' ' && mLabel[aIndexInLine] != '\n')
		{
			aLineWidth += FONT_PICO129->CharWidth(mLabel[aIndexInLine]);
			aIndexInLine++;
		}

		if (aIndexInLine != mLabel.size() && aLineWidth < mGetsLinesWidth && mLabel[aIndexInLine] != '\n')
		{
			aLineWidth += FONT_PICO129->CharWidth(mLabel[aIndexInLine]);
			aIndexInLine++;
		}
		else
		{
			SexyString aLine = mLabel.substr(aIndexStart, aIndexInLine - aIndexStart);
			aLineWidth = 0;
			theLines.push_back(aLine);

			if (aIndexInLine < mLabel.size() && mLabel[aIndexInLine] == '\n')
			{
				aIndexInLine++;
			}
			while (aIndexInLine < mLabel.size() && mLabel[aIndexInLine] == ' ')
			{
				aIndexInLine++;
			}

			aIndexStart = aIndexInLine;
		}
	}
}

//0x51A710
void ToolTipWidget::CalculateSize()
{
	std::vector<SexyString> aLines;

	int aTitleWidth = FONT_TINYBOLD->StringWidth(mTitle);
	int aWarningWidth = FONT_PICO129->StringWidth(mWarningText);
	int aMaxWidth = std::max(aTitleWidth, aWarningWidth);

	mGetsLinesWidth = std::max(aMaxWidth - 30, 100);
	GetLines(aLines);

	for (size_t i = 0; i < aLines.size(); i++)
	{
		int aLineWidth = FONT_PICO129->StringWidth(aLines[i]);
		aMaxWidth = std::max(aMaxWidth, aLineWidth);
	}

	int aHeight = 6;
	if (!mTitle.empty())
	{
		aHeight = FONT_TINYBOLD->GetAscent() + 8;
	}
	if (!mWarningText.empty())
	{
		aHeight += FONT_TINYBOLD->GetAscent() + 2;
	}
	aHeight += aLines.size() * FONT_PICO129->GetAscent();

	mWidth = aMaxWidth + 10;
	mHeight = aHeight + aLines.size() * 2 - 2;
}

//0x51A8D0
void ToolTipWidget::SetLabel(const SexyString& theLabel)
{
	mLabel = TodStringTranslate(theLabel);
	CalculateSize();
}

//0x51A950
void ToolTipWidget::SetTitle(const SexyString& theTitle)
{
	mTitle = TodStringTranslate(theTitle);
	CalculateSize();
}

//0x51A9D0
void ToolTipWidget::SetWarningText(const SexyString& theWarningText)
{
	mWarningText = TodStringTranslate(theWarningText);
	CalculateSize();
}

//0x51AA50
void ToolTipWidget::Draw(Graphics* g)
{
	if (!mVisible)
		return;

	int aPosX = mX;
	if (mCenter)
	{
		aPosX -= mWidth / 2;
	}
	if (mMinLeft - g->mTransX > aPosX)  // aPosX + g->mTransX < mMinLeft
	{
		aPosX = mMinLeft - (int)g->mTransX;
	}
	else if (aPosX + mWidth + g->mTransX > BOARD_WIDTH)
	{
		aPosX = BOARD_WIDTH - g->mTransX - mWidth;
	}

	int aPosY = mY;
	if (-g->mTransY > aPosY)  // aPosY + g->mTransY > 0
	{
		aPosY = (int)-g->mTransY;
	}
	else if (mMaxBottom < mY + mHeight + g->mTransY)
	{
		aPosY = mMaxBottom - (int)g->mTransY - mHeight;
	}

	g->SetColor(Color(255, 255, 200, 255));
	g->FillRect(aPosX, aPosY, mWidth, mHeight);
	g->SetColor(Color::Black);
	g->DrawRect(aPosX, aPosY, mWidth - 1, mHeight - 1);
	aPosY++;

	if (!mTitle.empty())
	{
		g->SetFont(FONT_TINYBOLD);
		g->DrawString(mTitle, aPosX + (mWidth - FONT_TINYBOLD->StringWidth(mTitle)) / 2, aPosY + FONT_TINYBOLD->GetAscent());
		aPosY += FONT_TINYBOLD->GetAscent() + 2;
	}

	if (!mWarningText.empty())
	{
		g->SetFont(FONT_PICO129);
		int x = aPosX + (mWidth - FONT_PICO129->StringWidth(mWarningText)) / 2;
		int y = aPosY + FONT_PICO129->GetAscent();

		Color aWarningColor(255, 0, 0);
		if (mWarningFlashCounter > 0 && mWarningFlashCounter % 20 < 10)
		{
			aWarningColor = Color(0, 0, 0);
		}

		g->SetColor(aWarningColor);
		g->DrawString(mWarningText, x, y);
		g->SetColor(Color::Black);

		aPosY += FONT_PICO129->GetAscent() + 2;
	}

	std::vector<SexyString> aLines;
	GetLines(aLines);

	g->SetFont(FONT_PICO129);
	for (size_t i = 0; i < aLines.size(); i++)
	{
		SexyString aLine = aLines[i];
		g->DrawString(aLine, aPosX + (mWidth - FONT_PICO129->StringWidth(aLine)) / 2, aPosY + FONT_PICO129->GetAscent());
		aPosY += FONT_PICO129->GetAscent() + 2;
	}
}
