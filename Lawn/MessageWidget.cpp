#include "Board.h"
#include "Challenge.h"
#include "../LawnApp.h"
#include "../Resources.h"
#include "MessageWidget.h"
#include "../SexyAppFramework/Font.h"
#include "../Sexy.TodLib/TodCommon.h"
#include "../Sexy.TodLib/Reanimator.h"
#include "../Sexy.TodLib/TodStringFile.h"

//(0x4081F1)
MessageWidget::MessageWidget(LawnApp* theApp)
{
	mApp = theApp;
	mDuration = 0;
	mLabel[0] = _S('\0');
	mMessageStyle = MessageStyle::MESSAGE_STYLE_OFF;
	mLabelNext[0] = _S('\0');
	mMessageStyleNext = MessageStyle::MESSAGE_STYLE_OFF;
	mSlideOffTime = 100;
	memset(mTextReanimID, (int)ReanimationID::REANIMATIONID_NULL, MAX_MESSAGE_LENGTH);
}

//0x458FC0
void MessageWidget::ClearReanim()
{
	for (int i = 0; i < MAX_MESSAGE_LENGTH; i++)
	{
		Reanimation* aReanim = mApp->ReanimationTryToGet(mTextReanimID[i]);
		if (aReanim)
		{
			aReanim->ReanimationDie();
			mTextReanimID[i] = ReanimationID::REANIMATIONID_NULL;
		}
	}
}

void MessageWidget::ClearLabel()
{
	if (mReanimType != ReanimationType::REANIM_NONE)
	{
		mDuration = min(mDuration, 100 + mSlideOffTime + 1);
	}
	else
	{
		mDuration = 0;
	}
}

//0x459010
void MessageWidget::SetLabel(const SexyString& theNewLabel, MessageStyle theMessageStyle)
{
	SexyString aLabel = TodStringTranslate(theNewLabel);
	TOD_ASSERT(aLabel.length() < MAX_MESSAGE_LENGTH - 1);

	if (mReanimType != ReanimationType::REANIM_NONE && mDuration > 0)
	{
		mMessageStyleNext = theMessageStyle;
		strcpy(mLabelNext, aLabel.c_str());
		ClearLabel();
	}
	else
	{
		ClearReanim();
		strcpy(mLabel, aLabel.c_str());
		mMessageStyle = theMessageStyle;
		mReanimType = ReanimationType::REANIM_NONE;

		switch (theMessageStyle)
		{
		case MessageStyle::MESSAGE_STYLE_HINT_LONG:
		case MessageStyle::MESSAGE_STYLE_BIG_MIDDLE:
		case MessageStyle::MESSAGE_STYLE_ZEN_GARDEN_LONG:
		case MessageStyle::MESSAGE_STYLE_HINT_TALL_LONG:
			mDuration = 1500;
			break;

		case MessageStyle::MESSAGE_STYLE_HINT_TALL_UNLOCKMESSAGE:
			mDuration = 500;
			break;

		case MessageStyle::MESSAGE_STYLE_HINT_FAST:
		case MessageStyle::MESSAGE_STYLE_HINT_TALL_FAST:
		case MessageStyle::MESSAGE_STYLE_BIG_MIDDLE_FAST:
		case MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1:
		case MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL2:
		case MessageStyle::MESSAGE_STYLE_TUTORIAL_LATER:
			mDuration = 500;
			break;

		case MessageStyle::MESSAGE_STYLE_HINT_STAY:
		case MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1_STAY:
		case MessageStyle::MESSAGE_STYLE_TUTORIAL_LATER_STAY:
			mDuration = 10000;
			break;

		case MessageStyle::MESSAGE_STYLE_HOUSE_NAME:
			mDuration = 250;
			break;

		case MessageStyle::MESSAGE_STYLE_HUGE_WAVE:
			mDuration = 750;
			mReanimType = ReanimationType::REANIM_TEXT_FADE_ON;
			break;

		case MessageStyle::MESSAGE_STYLE_SLOT_MACHINE:
			mDuration = 750;
			break;

		default:
			TOD_ASSERT();
			break;
		}
		
		if (mReanimType != ReanimationType::REANIM_NONE)
		{
			LayoutReanimText();
		}
		mDisplayTime = mDuration;
	}
}

//0x4591E0
void MessageWidget::LayoutReanimText()
{
	float aMaxWidth = 0;
	int aCurLine = 0, aCurPos = 0;
	Font* aFont = GetFont();
	int aLabelLen = sexystrlen(mLabel);
	mSlideOffTime = aLabelLen + 100;

	float aLineWidth[MAX_REANIM_LINES];
	for (int aPos = 0; aPos <= aLabelLen; aPos++)
	{
		if (aPos == aLabelLen || mLabel[aPos] == _S('\n'))
		{
			TOD_ASSERT(aCurLine < MAX_REANIM_LINES);

			int aLen = aPos - aCurPos;
			int aOff = aCurPos;
			aCurPos = aPos + 1;
			SexyString aLine(&mLabel[aOff], aLen);
			
			aLineWidth[aCurLine] = aFont->StringWidth(aLine);
			aMaxWidth = max(aMaxWidth, aLineWidth[aCurLine]);
			aCurLine++;
		}
	}

	aCurLine = 0;
	float aCurPosY = 0.0f;
	float aCurPosX = -aLineWidth[0] * 0.5f;
	// ���±�����Ļ�е������ı����ֱ����ʵ���λ�ô���ÿһ�����ֵĶ���
	for (int aPos = 0; aPos < aLabelLen; aPos++)
	{
		// �������ֵĶ���
		Reanimation* aReanimText = mApp->AddReanimation(aCurPosX, aCurPosY, 0, mReanimType);
		aReanimText->mIsAttachment = true;
		aReanimText->PlayReanim("anim_enter", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0.0f, 0.0f);
		mTextReanimID[aPos] = mApp->ReanimationGetID(aReanimText);

		aCurPosX += aFont->CharWidth(mLabel[aPos]);  // �����������һ�����ֵ�λ��
		if (mLabel[aPos] == _S('\n'))  // ���д���
		{
			aCurLine++;
			TOD_ASSERT(aCurLine < MAX_REANIM_LINES);
			aCurPosX = -aLineWidth[aCurLine] * 0.5f;
			aCurPosY += aFont->GetLineSpacing();
		}
	}
}

//0x4594B0
void MessageWidget::Update()
{
	if (!mApp->mBoard || mApp->mBoard->mPaused)
		return;

	// ������Ļ��ʣ��ʱ�䵹��ʱ����һ����Ļ���л�
	if (mDuration < 10000 && mDuration > 0)
	{
		mDuration--;
		if (mDuration == 0)
		{
			mMessageStyle = MessageStyle::MESSAGE_STYLE_OFF;
			if (mMessageStyleNext != MessageStyle::MESSAGE_STYLE_OFF)
			{
				SetLabel(mLabelNext, mMessageStyleNext);
				mMessageStyleNext = MessageStyle::MESSAGE_STYLE_OFF;
			}
		}
	}

	int aLabelLen = sexystrlen(mLabel);
	// ���±���ÿ�����ֵĶ����������䶯�����ʲ������䶯��
	for (int aPos = 0; aPos < aLabelLen; aPos++)
	{
		Reanimation* aTextReanim = mApp->ReanimationTryToGet(mTextReanimID[aPos]);
		if (aTextReanim == nullptr)
		{
			break;  // ���������ı�����ʱ������ѭ����ֱ�ӷ���
		}

		// ���ö�������
		int aTextSpeed = mReanimType == ReanimationType::REANIM_TEXT_FADE_ON ? 100 : 1;
		if (mDuration > mSlideOffTime)
		{
			if (mReanimType == ReanimationType::REANIM_TEXT_FADE_ON)
			{
				aTextReanim->mAnimRate = 60.0f;
			}
			else
			{
				aTextReanim->mAnimRate = TodAnimateCurveFloat(0, 50, (mDisplayTime - mDuration) * aTextSpeed - aPos, 0.0f, 40.0f, TodCurves::CURVE_LINEAR);
			}
		}
		else
		{
			if (mDuration == mSlideOffTime)
			{
				aTextReanim->PlayReanim("anim_leave", ReanimLoopType::REANIM_PLAY_ONCE_AND_HOLD, 0, 0.0f);
			}
			aTextReanim->mAnimRate = TodAnimateCurveFloat(0, 50, (mSlideOffTime - mDuration) * aTextSpeed - aPos, 0.0f, 40.0f, TodCurves::CURVE_LINEAR);
		}

		aTextReanim->Update();  //���¶���
	}
}

//0x459710
void MessageWidget::DrawReanimatedText(Graphics* g, Font* theFont, const Color& theColor, float thePosY)
{
	int aLabelLen = sexystrlen(mLabel);
	for (int aPos = 0; aPos < aLabelLen; aPos++)
	{
		Reanimation* aTextReanim = mApp->ReanimationTryToGet(mTextReanimID[aPos]);
		if (aTextReanim == nullptr)
		{
			break;  // ���������ı�����ʱ������ѭ����ֱ�ӷ���
		}

		ReanimatorTransform aTransform;
		aTextReanim->GetCurrentTransform(2, &aTransform);

		int anAlpha = ClampInt(FloatRoundToInt(theColor.mAlpha * aTransform.mAlpha), 0, 255);
		if (anAlpha <= 0)
		{
			break;  // �ı�������ȫ͸��ʱ��ֱ�ӷ���
		}
		Color aFinalColor(theColor);
		aFinalColor.mAlpha = anAlpha;

		aTransform.mTransX += aTextReanim->mOverlayMatrix.m02;
		aTransform.mTransY += aTextReanim->mOverlayMatrix.m12 + thePosY - BOARD_HEIGHT / 2;
		if (mReanimType == ReanimationType::REANIM_TEXT_FADE_ON && mDisplayTime - mDuration < mSlideOffTime)
		{
			float aStretch = 1.0f - aTextReanim->mAnimTime;
			aTransform.mTransX += aStretch * aTextReanim->mOverlayMatrix.m02;
		}

		SexyMatrix3 aMatrix;
		Reanimation::MatrixFromTransform(aTransform, aMatrix);
		SexyString aLetter;
		aLetter.append(1, mLabel[aPos]);
		TodDrawStringMatrix(g, theFont, aMatrix, aLetter, aFinalColor);
	}
}

//0x459990
Font* MessageWidget::GetFont()
{
	switch (mMessageStyle)
	{
	case MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1:
	case MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1_STAY:
	case MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL2:
	case MessageStyle::MESSAGE_STYLE_TUTORIAL_LATER:
	case MessageStyle::MESSAGE_STYLE_TUTORIAL_LATER_STAY:
	case MessageStyle::MESSAGE_STYLE_HINT_LONG:
	case MessageStyle::MESSAGE_STYLE_HINT_FAST:
	case MessageStyle::MESSAGE_STYLE_HINT_STAY:
	case MessageStyle::MESSAGE_STYLE_HINT_TALL_FAST:
	case MessageStyle::MESSAGE_STYLE_HINT_TALL_UNLOCKMESSAGE:
	case MessageStyle::MESSAGE_STYLE_HINT_TALL_LONG:
	case MessageStyle::MESSAGE_STYLE_BIG_MIDDLE:
	case MessageStyle::MESSAGE_STYLE_BIG_MIDDLE_FAST:
	case MessageStyle::MESSAGE_STYLE_HOUSE_NAME:
	case MessageStyle::MESSAGE_STYLE_HUGE_WAVE:
	case MessageStyle::MESSAGE_STYLE_ZEN_GARDEN_LONG:
		return Sexy::FONT_HOUSEOFTERROR28;

	case MessageStyle::MESSAGE_STYLE_SLOT_MACHINE:
		return Sexy::FONT_HOUSEOFTERROR16;
	}

	TOD_ASSERT();
}

//0x4599E0
void MessageWidget::Draw(Graphics* g)
{
	if (mDuration <= 0)
		return;
	
	Font* aFont = GetFont();
	Font* aOutlineFont = nullptr;
	int aPosX = BOARD_WIDTH / 2;
	int aPosY = 596;
	int aTextOffsetY = 0;
	int aRectHeight = 0;
	int aMinAlpha = 255;
	Color aColor(250, 250, 0, 255);
	Color aOutlineColor(0, 0, 0, 255);
	bool aFadeOut = false;
	if (aFont == Sexy::FONT_CONTINUUMBOLD14)
	{
		aOutlineFont = Sexy::FONT_CONTINUUMBOLD14OUTLINE;
	}

	switch (mMessageStyle)
	{
	case MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1:
	case MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL1_STAY:
		aPosY = 400;
		aRectHeight = 110;
		aTextOffsetY = -4;
		aColor = Color(253, 245, 173);
		aMinAlpha = 192;
		break;

	case MessageStyle::MESSAGE_STYLE_TUTORIAL_LEVEL2:
	case MessageStyle::MESSAGE_STYLE_TUTORIAL_LATER:
	case MessageStyle::MESSAGE_STYLE_TUTORIAL_LATER_STAY:
	case MessageStyle::MESSAGE_STYLE_HINT_TALL_FAST:
	case MessageStyle::MESSAGE_STYLE_HINT_TALL_UNLOCKMESSAGE:
	case MessageStyle::MESSAGE_STYLE_HINT_TALL_LONG:
		aPosY = 476;
		aRectHeight = 100;
		aTextOffsetY = -4;
		aColor = Color(253, 245, 173);
		aMinAlpha = 192;
		break;

	case MessageStyle::MESSAGE_STYLE_HINT_LONG:
	case MessageStyle::MESSAGE_STYLE_HINT_FAST:
	case MessageStyle::MESSAGE_STYLE_HINT_STAY:
		aPosY = 527;
		aRectHeight = 55;
		aTextOffsetY = -4;
		aColor = Color(253, 245, 173);
		aMinAlpha = 192;
		break;

	case MessageStyle::MESSAGE_STYLE_BIG_MIDDLE:
	case MessageStyle::MESSAGE_STYLE_BIG_MIDDLE_FAST:
		aPosY = 300;
		aRectHeight = 110;
		aColor = Color(253, 245, 173);
		aMinAlpha = 192;
		break;

	case MessageStyle::MESSAGE_STYLE_HOUSE_NAME:
		aPosY = 550;
		aColor = Color(255, 255, 255, 255);
		aFadeOut = true;
		break;

	case MessageStyle::MESSAGE_STYLE_HUGE_WAVE:
		aPosY = 330;
		aColor = Color(255, 0, 0);
		break;

	case MessageStyle::MESSAGE_STYLE_SLOT_MACHINE:
		aPosY = 93;
		aPosX = 340;
		aMinAlpha = 64;
		break;

	case MessageStyle::MESSAGE_STYLE_ZEN_GARDEN_LONG:
		aPosY = 514;
		aRectHeight = 55;
		aTextOffsetY = -4;
		aColor = Color(253, 245, 173);
		aMinAlpha = 192;
		break;

	default:
		TOD_ASSERT();
		break;
	}

	if (mReanimType != ReanimationType::REANIM_NONE)
	{
		if (aFont == Sexy::FONT_CONTINUUMBOLD14)
		{
			DrawReanimatedText(g, Sexy::FONT_CONTINUUMBOLD14OUTLINE, Color::Black, aPosY);
		}
		DrawReanimatedText(g, aFont, aColor, aPosY);
	}
	else
	{
		if (aMinAlpha != 255)
		{
			aColor.mAlpha = TodAnimateCurve(75, 0, mApp->mBoard->mMainCounter % 75, aMinAlpha, 255, TodCurves::CURVE_BOUNCE_SLOW_MIDDLE);
			aOutlineColor.mAlpha = aColor.mAlpha;
		}
		if (aFadeOut)
		{
			aColor.mAlpha = ClampInt(mDuration * 15, 0, 255);
			aOutlineColor.mAlpha = aColor.mAlpha;
		}

		if (aRectHeight > 0)
		{
			aOutlineColor = Color(0, 0, 0, 128);
			Rect aRect(0, aPosY, BOARD_WIDTH, aRectHeight);
			g->SetColor(aOutlineColor);
			g->FillRect(aRect);

			aRect.mY += aTextOffsetY;
			TodDrawStringWrapped(g, mLabel, aRect, aFont, aColor, DrawStringJustification::DS_ALIGN_CENTER_VERTICAL_MIDDLE);
		}
		else
		{
			Rect aRect(aPosX - mApp->mBoard->mX - BOARD_WIDTH / 2, aPosY - aFont->mAscent, BOARD_WIDTH, BOARD_HEIGHT);
			if (aOutlineFont)
			{
				TodDrawStringWrapped(g, mLabel, aRect, aOutlineFont, aOutlineColor, DrawStringJustification::DS_ALIGN_CENTER);
			}
			TodDrawStringWrapped(g, mLabel, aRect, aFont, aColor, DrawStringJustification::DS_ALIGN_CENTER);
		}

		if (mMessageStyle == MessageStyle::MESSAGE_STYLE_HOUSE_NAME)
		{
			SexyString aSubStr;
			if (mApp->IsSurvivalMode() && mApp->mBoard->mChallenge->mSurvivalStage > 0)
			{
				int aFlags = mApp->mBoard->GetNumWavesPerSurvivalStage() * mApp->mBoard->mChallenge->mSurvivalStage / mApp->mBoard->GetNumWavesPerFlag();
				SexyString aFlagStr = mApp->Pluralize(aFlags, _S("[ONE_FLAG]"), _S("[COUNT_FLAGS]"));
				SexyString aSubStr = TodReplaceString(_S("[FLAGS_COMPLETED]"), _S("{FLAGS}"), aFlagStr);
			}

			if (aSubStr.size() > 0)
			{
				TodDrawString(
					g, 
					aSubStr, 
					BOARD_WIDTH / 2 - mApp->mBoard->mX, 
					aPosY + 26, 
					Sexy::FONT_HOUSEOFTERROR16, 
					Color(224, 187, 62, aColor.mAlpha), 
					DrawStringJustification::DS_ALIGN_CENTER
				);
			}
		}
	}
}
