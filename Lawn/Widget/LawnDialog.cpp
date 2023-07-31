#include "../Board.h"
#include "LawnDialog.h"
#include "GameButton.h"
#include "../../LawnApp.h"
#include "../LawnCommon.h"
#include "../../Resources.h"
#include "ChallengeScreen.h"
#include "../../Sexy.TodLib/TodDebug.h"
#include "../../Sexy.TodLib/Reanimator.h"
#include "../../Sexy.TodLib/EffectSystem.h"
#include "../../Sexy.TodLib/TodStringFile.h"
#include "../../SexyAppFramework/ImageFont.h"


//0x456A80
LawnDialog::LawnDialog(LawnApp* theApp, int theId, bool isModal, const SexyString& theDialogHeader, const SexyString& theDialogLines, const SexyString& theDialogFooter, int theButtonMode) :
	Dialog(nullptr, nullptr, theId, isModal, theDialogHeader, theDialogLines, _S(""), BUTTONS_NONE)
{
    mApp = theApp;
    mButtonDelay = -1;
    mReanimation = new ReanimationWidget();
    mReanimation->mLawnDialog = this;
    mDrawStandardBack = true;
    mTallBottom = false;
    mVerticalCenterText = true;
    mDialogHeader = TodStringTranslate(theDialogHeader);
    mDialogLines = TodStringTranslate(theDialogLines);
    SetColor(0, { 0xE0,0xBB,0x62 });
    SetColor(1, { 0xE0,0xBB,0x62 });
    SetHeaderFont(Sexy::FONT_DWARVENTODCRAFT24);
    SetLinesFont(Sexy::FONT_DWARVENTODCRAFT15);
    mContentInsets = Insets(36, 35, 46, 36);

    // @Patoke: these dialogs had the wrong local name
    if (theButtonMode == 1)
    {
        mLawnYesButton = MakeButton(1000, this, _S("[DIALOG_BUTTON_YES]"));
        mLawnNoButton = MakeButton(1001, this, _S("[DIALOG_BUTTON_NO]"));
    }
    else if (theButtonMode == 2)
    {
        mLawnYesButton = MakeButton(1000, this, _S("[DIALOG_BUTTON_OK]"));
        mLawnNoButton = MakeButton(1001, this, _S("[DIALOG_BUTTON_CANCEL]"));
    }
    else if (theButtonMode == 3)
    {
        mLawnYesButton = MakeButton(1000, this, theDialogFooter);
        mLawnNoButton = nullptr;
    }
    else
    {
        mLawnYesButton = nullptr;
        mLawnNoButton = nullptr;
    }

    mApp->SetCursor(CURSOR_POINTER);
    CalcSize(0, 0);
}

//0x456E80 & 0x456EA0
LawnDialog::~LawnDialog()
{
    if (mReanimation) delete mReanimation;
    if (mLawnYesButton) delete mLawnYesButton;
    if (mLawnNoButton) delete mLawnNoButton;
}

int LawnDialog::GetLeft()
{
    return mContentInsets.mLeft + mBackgroundInsets.mLeft;
}

int LawnDialog::GetWidth()
{
    return mWidth - mContentInsets.mLeft - mContentInsets.mRight - mBackgroundInsets.mLeft - mBackgroundInsets.mRight;
}

int LawnDialog::GetTop()
{
    return mContentInsets.mTop + mBackgroundInsets.mTop + 99;
}

//0x456F30
void LawnDialog::CalcSize(int theExtraX, int theExtraY)
{
    // ���ݱ߾����һ����С���
    int aWidth = mBackgroundInsets.mLeft + mBackgroundInsets.mRight + mContentInsets.mLeft + mContentInsets.mRight + theExtraX;
    // ��С�����Ҫ���⿼�ǵ������ַ����Ŀ��
    if (mDialogHeader.size() > 0) aWidth += mHeaderFont->StringWidth(mDialogHeader);
    // �Ի�����ͼ����С���
    int aTopMidWidth = Sexy::IMAGE_DIALOG_TOPMIDDLE->mWidth;
    int aImageWidth = Sexy::IMAGE_DIALOG_TOPLEFT->mWidth + Sexy::IMAGE_DIALOG_TOPRIGHT->mWidth + aTopMidWidth;
    // �������յĿ��
    if (aWidth <= aImageWidth)
    {
        aWidth = aImageWidth;
    }
    else if (aTopMidWidth > 0)
    {
        int anExtraWidth = (aWidth - aImageWidth) % aTopMidWidth;
        if (anExtraWidth)
        {
            aWidth += aTopMidWidth - anExtraWidth;
        }
    }

    // ���ݱ߾����һ����С�߶�
    int aHeight = mBackgroundInsets.mTop + mBackgroundInsets.mBottom + mContentInsets.mTop + mContentInsets.mBottom + theExtraY + DIALOG_HEADER_OFFSET;
    // ���⿼�ǵ������ַ����ĸ߶�
    if (mDialogHeader.size() > 0)
    {
        aHeight += -mHeaderFont->GetAscentPadding() + mHeaderFont->GetHeight() + mSpaceAfterHeader;
    }
    // �Ի����Ⱥ͸߶ȶ��⿼�������ı�
    if (mDialogLines.size() > 0)
    {
        aWidth += aTopMidWidth;
        Graphics g;
        g.SetFont(mLinesFont);
        int aBasicWidth = aWidth - mBackgroundInsets.mLeft - mBackgroundInsets.mRight - mContentInsets.mLeft - mContentInsets.mRight - 4;
        aHeight += GetWordWrappedHeight(&g, aBasicWidth, mDialogLines, mLinesFont->GetLineSpacing() + mLineSpacingOffset) + 30;
    }
    aHeight += mButtonHeight;
    // �Ի�����ͼ����С�߶�
    int aBottomHeight = (mTallBottom ? Sexy::IMAGE_DIALOG_BIGBOTTOMLEFT : Sexy::IMAGE_DIALOG_BOTTOMLEFT)->mHeight;
    int aImageHeight = Sexy::IMAGE_DIALOG_TOPLEFT->mHeight + aBottomHeight + DIALOG_HEADER_OFFSET;
    // �������յĸ߶�
    if (aHeight < aImageHeight)
    {
        aHeight = aImageHeight;
    }
    else
    {
        int aCenterHeight = Sexy::IMAGE_DIALOG_CENTERLEFT->mHeight;
        int anExtraHeight = (aHeight - aImageHeight) % aCenterHeight;
        if (anExtraHeight) aHeight += aCenterHeight - anExtraHeight;
    }

    Resize(mX, mY, aWidth, aHeight);
}

//0x457160
void LawnDialog::AddedToManager(WidgetManager* theWidgetManager)
{
    Dialog::AddedToManager(theWidgetManager);
    AddWidget(mReanimation);
    if (mLawnYesButton) AddWidget(mLawnYesButton);
    if (mLawnNoButton) AddWidget(mLawnNoButton);
}

//0x4571B0
void LawnDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
    Dialog::RemovedFromManager(theWidgetManager);
    if (mLawnYesButton) RemoveWidget(mLawnYesButton);
    if (mLawnNoButton) RemoveWidget(mLawnNoButton);
    RemoveWidget(mReanimation);

    if (mReanimation->mReanim)
    {
        mApp->mEffectSystem->mReanimationHolder->mReanimations.DataArrayFree(mReanimation->mReanim);
        mReanimation->mReanim = nullptr;
    }
}

//0x457230
void LawnDialog::SetButtonDelay(int theDelay)
{
    mButtonDelay = theDelay;
    if (mLawnYesButton) mLawnYesButton->SetDisabled(true);
    if (mLawnNoButton) mLawnNoButton->SetDisabled(true);
}

//0x457280
void LawnDialog::Update()
{
    Dialog::Update();
    if (!mButtonDelay)
    {
        if (mLawnYesButton) mLawnYesButton->SetDisabled(false);
        if (mLawnNoButton) mLawnNoButton->SetDisabled(false);
    }
    MarkDirty();
}

//0x4572E0
void LawnDialog::ButtonPress(int theId)
{
    mApp->PlaySample(Sexy::SOUND_GRAVEBUTTON);
}

//0x457300
void LawnDialog::ButtonDepress(int theId)
{
    if (mUpdateCnt > mButtonDelay)
    {
        Dialog::ButtonDepress(theId);
    }
}

//0x457320
void LawnDialog::CheckboxChecked(int theId, bool theChecked)
{
    mApp->PlaySample(Sexy::SOUND_BUTTONCLICK);
}

//0x457340
void LawnDialog::KeyDown(KeyCode theKey)
{
    if (mId == DIALOG_PAUSED && mApp->mBoard)
    {
        mApp->mBoard->DoTypingCheck(theKey);
    }

    if (mId != DIALOG_ALMANAC)
    {
        if (theKey == KEYCODE_SPACE || theKey == KEYCODE_RETURN || theKey == 'y' || theKey == 'Y')
        {
            Dialog::ButtonDepress(Dialog::ID_YES);
        }
        else if ((theKey == KEYCODE_ESCAPE || theKey == 'n' || theKey == 'N') && mLawnNoButton)
        {
            Dialog::ButtonDepress(Dialog::ID_NO);
        }
    }
}

//0x4573D0
void LawnDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
    Dialog::Resize(theX, theY, theWidth, theHeight);

    int aButtonAreaX = mContentInsets.mLeft + mBackgroundInsets.mLeft - 5;
    int aButtonAreaY = mHeight - mContentInsets.mBottom - mBackgroundInsets.mBottom - IMAGE_BUTTON_LEFT->mHeight + 2;
    int aButtonAreaWidth = mWidth - mContentInsets.mRight - mBackgroundInsets.mRight - mBackgroundInsets.mLeft - mContentInsets.mLeft + 8;
    // ��ť��ͼ�����£���ť����С���
    int aButtonMinWidth = IMAGE_BUTTON_LEFT->mWidth + IMAGE_BUTTON_RIGHT->mWidth;
    // ��ť����������Ŀ��
    int aBtnMidWidth = IMAGE_BUTTON_MIDDLE->mWidth;
    int aButtonExtraWidth = (aButtonAreaWidth - 10) / 2 - aBtnMidWidth - aButtonMinWidth + 1;
    // ���㰴ť������չ�Ķ�����
    if (aButtonExtraWidth <= 0)
    {
        aButtonExtraWidth = 0;
    }
    else if (aBtnMidWidth > 0)
    {
        int anExtraWidth = aButtonExtraWidth % aBtnMidWidth;
        if (anExtraWidth)
        {
            // �����в���ͼ��ȵĲ��ֲ������в���ͼ���
            aButtonExtraWidth += aBtnMidWidth - anExtraWidth;
        }
    }
    // ���տ��
    int aButtonWidth = aButtonMinWidth + aButtonExtraWidth;

    if (mTallBottom)
    {
        aButtonAreaY += 5;
    }

    if (mLawnYesButton && mLawnNoButton)
    {
        mLawnYesButton->Resize(aButtonAreaX, aButtonAreaY, aButtonWidth, IMAGE_BUTTON_LEFT->mHeight);
        mLawnNoButton->Resize(aButtonAreaWidth - aButtonWidth + aButtonAreaX, aButtonAreaY, aButtonWidth, IMAGE_BUTTON_LEFT->mHeight);
    }
    else if (mLawnYesButton)
    {
        aButtonExtraWidth = aButtonAreaWidth - aBtnMidWidth - aButtonMinWidth + 1;
        if (aButtonExtraWidth <= 0)
        {
            aButtonExtraWidth = 0;
        }
        else if (aBtnMidWidth > 0)
        {
            int anExtraWidth = aButtonExtraWidth % aBtnMidWidth;
            if (anExtraWidth)
            {
                aButtonExtraWidth += aBtnMidWidth - anExtraWidth;
            }
        }
        int aButtonWidth = aButtonMinWidth + aButtonExtraWidth;

        mLawnYesButton->Resize(aButtonAreaX + (aButtonAreaWidth - aButtonWidth) / 2, aButtonAreaY, aButtonWidth, IMAGE_BUTTON_LEFT->mHeight);
    }

    if (mReanimation->mReanim)
    {
        mReanimation->Resize(mReanimation->mPosX, mReanimation->mPosY + DIALOG_HEADER_OFFSET, mReanimation->mWidth, mReanimation->mHeight);
    }
}

//0x4575A0
void LawnDialog::Draw(Graphics* g)
{
    if (!mDrawStandardBack)
        return;

    Image* aBottomLeftImage = IMAGE_DIALOG_BOTTOMLEFT;
    Image* aBottomMiddleImage = IMAGE_DIALOG_BOTTOMMIDDLE;
    Image* aBottomRightImage = IMAGE_DIALOG_BOTTOMRIGHT;
    if (mTallBottom)
    {
        aBottomLeftImage = IMAGE_DIALOG_BIGBOTTOMLEFT;
        aBottomMiddleImage = IMAGE_DIALOG_BIGBOTTOMMIDDLE;
        aBottomRightImage = IMAGE_DIALOG_BIGBOTTOMRIGHT;
    }

    int aRepeatX = (mWidth - IMAGE_DIALOG_TOPRIGHT->mWidth - IMAGE_DIALOG_TOPLEFT->mWidth) / IMAGE_DIALOG_TOPMIDDLE->mWidth;
    int aRepeatY = (mHeight - IMAGE_DIALOG_TOPLEFT->mHeight - aBottomLeftImage->mHeight - DIALOG_HEADER_OFFSET) / IMAGE_DIALOG_CENTERLEFT->mHeight;

    int aPosX = 0;
    int aPosY = DIALOG_HEADER_OFFSET;
    g->DrawImage(IMAGE_DIALOG_TOPLEFT, aPosX, aPosY);
    aPosX += IMAGE_DIALOG_TOPLEFT->mWidth;
    for (int i = 0; i < aRepeatX; i++)
    {
        g->DrawImage(IMAGE_DIALOG_TOPMIDDLE, aPosX, aPosY);
        aPosX += IMAGE_DIALOG_TOPMIDDLE->mWidth;
    }
    g->DrawImage(IMAGE_DIALOG_TOPRIGHT, aPosX, aPosY);

    aPosY += IMAGE_DIALOG_TOPRIGHT->mHeight;
    for (int y = 0; y < aRepeatY; y++)
    {
        aPosX = 0;

        g->DrawImage(IMAGE_DIALOG_CENTERLEFT, aPosX, aPosY);
        aPosX += IMAGE_DIALOG_CENTERLEFT->mWidth;

        for (int x = 0; x < aRepeatX; x++)
        {
            g->DrawImage(IMAGE_DIALOG_CENTERMIDDLE, aPosX, aPosY);
            aPosX += IMAGE_DIALOG_CENTERMIDDLE->mWidth;
        }

        g->DrawImage(IMAGE_DIALOG_CENTERRIGHT, aPosX, aPosY);
        aPosY += IMAGE_DIALOG_CENTERLEFT->mHeight;
    }

    aPosX = 0;
    g->DrawImage(aBottomLeftImage, aPosX, aPosY);
    aPosX += aBottomLeftImage->mWidth;
    for (int i = 0; i < aRepeatX; i++)
    {
        g->DrawImage(aBottomMiddleImage, aPosX, aPosY);
        aPosX += aBottomMiddleImage->mWidth;
    }
    g->DrawImage(aBottomRightImage, aPosX, aPosY);
    g->DrawImage(IMAGE_DIALOG_HEADER, (mWidth - IMAGE_DIALOG_HEADER->mWidth) / 2 - 5, 0);

    int aFontY = mContentInsets.mTop + mBackgroundInsets.mTop + DIALOG_HEADER_OFFSET;
    if (mDialogHeader.size() > 0)
    {
        int aOffsetY = aFontY - mHeaderFont->GetAscentPadding() + mHeaderFont->GetAscent();
        g->SetFont(mHeaderFont);
        g->SetColor(mColors[Dialog::COLOR_HEADER]);
        WriteCenteredLine(g, aOffsetY, mDialogHeader);
        aFontY = aOffsetY - mHeaderFont->GetAscent() + mHeaderFont->GetHeight() + mSpaceAfterHeader;
    }

    g->SetFont(mLinesFont);
    g->SetColor(mColors[Dialog::COLOR_LINES]);
    int aLinesAreaWidth = mWidth - mContentInsets.mLeft - mContentInsets.mRight - mBackgroundInsets.mLeft - mBackgroundInsets.mRight - 4;
    Rect aRect(mBackgroundInsets.mLeft + mContentInsets.mLeft + 2, aFontY, aLinesAreaWidth, 0);
    if (mVerticalCenterText)
    {
        int aLinesHeight = GetWordWrappedHeight(g, aLinesAreaWidth, mDialogLines, mLinesFont->GetLineSpacing() + mLineSpacingOffset);
        int aLinesAreaHeight = mHeight - mContentInsets.mBottom - mBackgroundInsets.mBottom - mButtonHeight - aFontY - 55;
        if (mTallBottom)
        {
            aLinesAreaHeight -= 36;
        }
        aRect.mY += (aLinesAreaHeight - aLinesHeight) / 2;
    }
    WriteWordWrapped(g, aRect, mDialogLines, mLinesFont->GetLineSpacing() + mLineSpacingOffset, mTextAlign);
}

//0x4579A0
ReanimationWidget::ReanimationWidget()
{
    mApp = (LawnApp*)gSexyAppBase;
    mPosX = 0.0f;
    mPosY = 0.0f;
    mReanim = nullptr;
    mMouseVisible = false;
    mClip = false;
    mLawnDialog = nullptr;
    mHasAlpha = true;
}

//0x4579F0��0x457A10
ReanimationWidget::~ReanimationWidget()
{
    Dispose();
}

void ReanimationWidget::Dispose()
{
    if (mReanim)
    {
        mApp->mEffectSystem->mReanimationHolder->mReanimations.DataArrayFree(mReanim);
        mReanim = nullptr;
    }
}

//0x457A80
void ReanimationWidget::AddReanimation(float x, float y, ReanimationType theReanimationType)
{
    TOD_ASSERT(mReanim == nullptr);
    
    mPosX = x;
    mPosY = y;
    mReanim = mApp->mEffectSystem->mReanimationHolder->AllocReanimation(x, y, 0, theReanimationType);
    mReanim->mLoopType = ReanimLoopType::REANIM_LOOP;
    mReanim->mIsAttachment = true;
    if (mReanim->TrackExists("anim_idle"))
        mReanim->SetFramesForLayer("anim_idle");

    Resize(x, y, 10, 10);
}

//0x457B80
void ReanimationWidget::Draw(Graphics* g)
{
    if (mReanim)
        mReanim->Draw(g);
}

//0x457BA0
void ReanimationWidget::Update()
{
    if (mReanim)
    {
        mReanim->Update();
        MarkDirty();
    }
}

//0x457BC0
GameOverDialog::GameOverDialog(const SexyString& theMessage, bool theShowChallengeName) : LawnDialog(
    gLawnApp, 
    Dialogs::DIALOG_GAME_OVER, 
    true, 
    _S("[GAME_OVER]"), 
    theMessage,
    _S(""), 
    Dialog::BUTTONS_FOOTER)
{
    mMenuButton = nullptr;
    mLawnYesButton->SetLabel(_S("[TRY_AGAIN]"));
    if (theShowChallengeName)
    {
        mDialogHeader = TodStringTranslate(mApp->GetCurrentChallengeDef().mChallengeName);
    }

    if (theMessage.size() == 0)
    {
        mContentInsets.mTop += 15;
    }
    CalcSize(0, 0);
    mApp->CenterDialog(this, mWidth, mHeight);
    mClip = false;

    mMenuButton = MakeButton(1, this, _S("[MAIN_MENU_BUTTON]"));
    mMenuButton->Resize(635 - mX, -10 - mY, 163, 46);

    gLawnApp->mBoard->mShowShovel = false;
    gLawnApp->mBoard->mMenuButton->mBtnNoDraw = true;
}

GameOverDialog::~GameOverDialog()
{
    delete mMenuButton;
}

//0x457E50
void GameOverDialog::ButtonDepress(int theId)
{
    if (theId == 1)
    {
        mApp->KillDialog(Dialogs::DIALOG_GAME_OVER);
        mApp->KillBoard();
        if (mApp->IsSurvivalMode())
        {
            mApp->ShowChallengeScreen(ChallengePage::CHALLENGE_PAGE_SURVIVAL);
        }
        else if (mApp->IsPuzzleMode())
        {
            mApp->ShowChallengeScreen(ChallengePage::CHALLENGE_PAGE_PUZZLE);
        }
        else if (mApp->IsAdventureMode())
        {
            mApp->ShowGameSelector();
        }
        else
        {
            mApp->ShowChallengeScreen(ChallengePage::CHALLENGE_PAGE_CHALLENGE);
        }
    }
    else if (theId == Dialog::ID_FOOTER)
    {
        mApp->KillDialog(Dialogs::DIALOG_GAME_OVER);
        mApp->EndLevel();
    }
}

//0x457F20
void GameOverDialog::AddedToManager(WidgetManager* theWidgetManager)
{
    LawnDialog::AddedToManager(theWidgetManager);
    if (mMenuButton)
    {
        AddWidget(mMenuButton);
    }
}

//0x457F80
void GameOverDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
    LawnDialog::RemovedFromManager(theWidgetManager);
    if (mMenuButton)
    {
        RemoveWidget(mMenuButton);
    }
}

//0x457FB0
void GameOverDialog::MouseDrag(int x, int y)
{
    LawnDialog::MouseDrag(x, y);
    if (mMenuButton)
    {
        mMenuButton->Resize(635 - mX, -10 - mY, 163, 46);
    }
}
