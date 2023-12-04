#include "DialogButton.h"
#include "graphics/SysFont.h"
#include "WidgetManager.h"

using namespace Sexy;

static int gDialogButtonColors[][3] = {
	{255, 255, 255},
	{255, 255, 255},
	{0, 0, 0},
	{255, 255, 255},
	{132, 132, 132},
	{212, 212, 212}};

DialogButton::DialogButton(Image* theComponentImage, int theId, ButtonListener* theListener) :
	ButtonWidget(theId, theListener)
{
	mComponentImage = theComponentImage;

	mTextOffsetX = mTextOffsetY = 0;
	mTranslateX = mTranslateY = 1;
	mDoFinger = true;

	SetColors(gDialogButtonColors, NUM_COLORS);
}

void DialogButton::Draw(Graphics* g)
{
	if (mBtnNoDraw)
		return;

	if (mComponentImage==NULL)
	{
		ButtonWidget::Draw(g);
		return;
	}

	if ((mFont == NULL) && (mLabel.length() > 0))
		mFont = new SysFont(mWidgetManager->mApp, "Arial Unicode MS", 12, true);

	bool doTranslate = IsButtonDown();

	if (mNormalRect.mWidth==0)
	{
		if (doTranslate)
			g->Translate(mTranslateX, mTranslateY);

		g->DrawImageBox(Rect(0, 0, mWidth, mHeight), mComponentImage);
	}
	else
	{
		if (mDisabled && (mDisabledRect.mWidth > 0) && (mDisabledRect.mHeight > 0))
			g->DrawImageBox(mDisabledRect, Rect(0, 0, mWidth, mHeight), mComponentImage);
		else if (IsButtonDown())	
			g->DrawImageBox(mDownRect, Rect(0, 0, mWidth, mHeight), mComponentImage);
		else if ((mOverAlpha > 0))
		{
			if (mOverAlpha<1)
				g->DrawImageBox(mNormalRect, Rect(0, 0, mWidth, mHeight), mComponentImage);

			g->SetColorizeImages(true);
			g->SetColor(Color(255,255,255,(int)(mOverAlpha * 255)));
			g->DrawImageBox(mOverRect, Rect(0, 0, mWidth, mHeight), mComponentImage);
			g->SetColorizeImages(false);
		}
		else if(mIsOver)
			g->DrawImageBox(mOverRect, Rect(0, 0, mWidth, mHeight), mComponentImage);
		else
			g->DrawImageBox(mNormalRect, Rect(0, 0, mWidth, mHeight), mComponentImage);
		
		if (doTranslate)
			g->Translate(mTranslateX, mTranslateY);
	}

	if (mFont != NULL)
	{
		g->SetFont(mFont);		

		if (mIsOver)
			g->SetColor(mColors[COLOR_LABEL_HILITE]);
		else
			g->SetColor(mColors[COLOR_LABEL]);
		
		int aFontX = (mWidth - mFont->StringWidth(mLabel))/2;
		int aFontY = (mHeight + mFont->GetAscent() - mFont->GetAscentPadding() - mFont->GetAscent()/6 - 1)/2;

		g->DrawString(mLabel, aFontX + mTextOffsetX, aFontY + mTextOffsetY);
	}

	if (doTranslate)
		g->Translate(-mTranslateX, -mTranslateY);
}
