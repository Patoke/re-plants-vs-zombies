#include "UserDialog.h"
#include "GameButton.h"
#include "LawnApp.h"
#include "Resources.h"
#include "lawn/system/ProfileMgr.h"
#include "lawn/system/PlayerInfo.h"
#include "todlib/TodStringFile.h"
#include "widget/ListWidget.h"

static int gUserListWidgetColors[][3] = {  //0x69F274
    {  23,  24,  35 },
    {   0,   0,   0 },
    { 235, 225, 180 },
    { 255, 255, 255 },
    {  20, 180,  15 }
};

//0x51C760
// @Patoke: these dialogs don't have localizations
UserDialog::UserDialog(LawnApp* theApp) : LawnDialog(theApp, Dialogs::DIALOG_USERDIALOG, true, _S("WHO ARE YOU?"), _S(""), _S(""), Dialog::BUTTONS_OK_CANCEL)
{
	mVerticalCenterText = false;
	mUserList = new ListWidget(0, FONT_BRIANNETOD16, this);
    mUserList->SetColors(gUserListWidgetColors, LENGTH(gUserListWidgetColors));
    mUserList->mDrawOutline = true;
    mUserList->mJustify = ListWidget::JUSTIFY_CENTER;
    mUserList->mItemHeight = 24;
    
    mRenameButton = MakeButton(UserDialog::UserDialog_RenameUser, this, _S("Rename"));
    mDeleteButton = MakeButton(UserDialog::UserDialog_DeleteUser, this, _S("Delete"));

    mNumUsers = 0;
    if (theApp->mPlayerInfo)
    {
        mUserList->SetSelect(mUserList->AddLine(theApp->mPlayerInfo->mName, false));
        mNumUsers++;
    }

    const ProfileMap& aMap = theApp->mProfileMgr->GetProfileMap();
    for (ProfileMap::const_iterator anItr = aMap.begin(); anItr != aMap.end(); anItr++)
    {
        if (theApp->mPlayerInfo && anItr->second.mName == theApp->mPlayerInfo->mName)
        {
            continue;
        }

        mUserList->AddLine(anItr->second.mName, false);
        mNumUsers++;
    }

    if (mNumUsers < 8)
    {
        mUserList->AddLine(TodStringTranslate(_S("(Create a New User)")), false);
    }

    mTallBottom = true;
    CalcSize(210, 270);
}

//0x51CBC0¡¢0x51CBE0
UserDialog::~UserDialog()
{
    delete mUserList;
    delete mRenameButton;
    delete mDeleteButton;
}

//0x51CC80
void UserDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
    LawnDialog::Resize(theX, theY, theWidth, theHeight);
    mUserList->Resize(GetLeft() + 30, GetTop() + 4, GetWidth() - 60, 200);
    mRenameButton->Layout(LayoutFlags::LAY_SameLeft | LayoutFlags::LAY_Above | LayoutFlags::LAY_SameHeight | LayoutFlags::LAY_SameWidth, mLawnYesButton, 0, 0, 0, 0);
    mDeleteButton->Layout(LayoutFlags::LAY_SameLeft | LayoutFlags::LAY_Above | LayoutFlags::LAY_SameHeight | LayoutFlags::LAY_SameWidth, mLawnNoButton, 0, 0, 0, 0);
}

//0x51CD20
int UserDialog::GetPreferredHeight(int theWidth)
{
    return LawnDialog::GetPreferredHeight(theWidth) + 190;
}

//0x51CD40
void UserDialog::AddedToManager(WidgetManager* theWidgetManager)
{
    LawnDialog::AddedToManager(theWidgetManager);
    AddWidget(mUserList);
    AddWidget(mDeleteButton);
    AddWidget(mRenameButton);
}

//0x51CDC0
void UserDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
    LawnDialog::RemovedFromManager(theWidgetManager);
    RemoveWidget(mUserList);
    RemoveWidget(mDeleteButton);
    RemoveWidget(mRenameButton);
}

//0x51CE10
SexyString UserDialog::GetSelName()
{
    if (mUserList->mSelectIdx < 0 || mUserList->mSelectIdx >= mNumUsers)
    {
        return _S("");
    }
    return mUserList->GetStringAt(mUserList->mSelectIdx);
}

//0x51CE70
void UserDialog::FinishDeleteUser()
{
    int aSelIdx = mUserList->mSelectIdx;
    mUserList->RemoveLine(mUserList->mSelectIdx);

    aSelIdx--;
    if (aSelIdx < 0)
    {
        aSelIdx = 0;
    }
    if (mUserList->GetLineCount() > 0)
    {
        mUserList->SetSelect(aSelIdx);
    }

    mNumUsers--;
    if (mNumUsers == 7)
    {
        mUserList->AddLine(TodStringTranslate(_S("[CREATE_NEW_USER]")), false);
    }
}

void UserDialog::FinishRenameUser(const SexyString& theNewName)
{
    if (mUserList->mSelectIdx < mNumUsers)
    {
        mUserList->SetLine(mUserList->mSelectIdx, theNewName);
    }
}

//0x51CF50
void UserDialog::Draw(Graphics* g)
{
    LawnDialog::Draw(g);
}

//0x51CF60
void UserDialog::ListClicked(int theId, int theIdx, int theClickCount)
{
    (void)theId;
    if (theIdx == mNumUsers)
    {
        mApp->DoCreateUserDialog();
    }
    else
    {
        mUserList->SetSelect(theIdx);
        if (theClickCount == 2)  // ×ó¼üË«»÷
        {
            mApp->FinishUserDialog(true);
        }
    }
}

//0x51CFA0
// GOTY @Patoke: 0x5278C0
void UserDialog::ButtonDepress(int theId)
{
    LawnDialog::ButtonDepress(theId);
    SexyString aSelName = GetSelName();
    if (!aSelName.empty())
    {
        switch (theId)
        {
        case UserDialog::UserDialog_RenameUser:
            mApp->DoRenameUserDialog(aSelName);
            break;

        case UserDialog::UserDialog_DeleteUser:
            mApp->DoConfirmDeleteUserDialog(aSelName);
            break;
        }
    }
}

//0x51D060
void UserDialog::EditWidgetText(int theId, const SexyString& theString)
{
    (void)theId;(void)theString;
    mApp->ButtonDepress(mId + 2000);
}

//0x51D080
bool UserDialog::AllowChar(int theId, SexyChar theChar)
{
    (void)theId;
    return sexyisdigit(theChar);
}
