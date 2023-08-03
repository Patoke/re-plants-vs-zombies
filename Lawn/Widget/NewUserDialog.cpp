#include "NewUserDialog.h"
#include "../../LawnApp.h"
#include "../../Resources.h"
#include "../../SexyAppFramework/WidgetManager.h"

//0x45D610
// GOTY @Patoke: 0x460F20
NewUserDialog::NewUserDialog(LawnApp* theApp, bool isRename) : LawnDialog(
	theApp, 
	isRename ? Dialogs::DIALOG_RENAMEUSER : Dialogs::DIALOG_CREATEUSER, 
	true, 
	// @Patoke: these locals don't exist
	isRename ? _S("RENAME USER") : _S("NEW USER"), 
	_S("Please enter your name:"), 
	_S("[DIALOG_BUTTON_OK]"), 
	Dialog::BUTTONS_OK_CANCEL)
{
	mApp = theApp;
	mVerticalCenterText = false;
	mNameEditWidget = CreateEditWidget(0, this, this);
	mNameEditWidget->mMaxChars = 12;
	mNameEditWidget->AddWidthCheckFont(FONT_BRIANNETOD16, 220);
	CalcSize(110, 40);
}

//0x45D7E0¡¢0x45D800
NewUserDialog::~NewUserDialog()
{
	delete mNameEditWidget;
}

//0x45D870
void NewUserDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	LawnDialog::AddedToManager(theWidgetManager);
	AddWidget(mNameEditWidget);
	theWidgetManager->SetFocus(mNameEditWidget);
}

//0x45D8E0
void NewUserDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	LawnDialog::RemovedFromManager(theWidgetManager);
	RemoveWidget(mNameEditWidget);
}

int NewUserDialog::GetPreferredHeight(int theWidth)
{
	return LawnDialog::GetPreferredHeight(theWidth) + 40;
}

//0x45D910
void NewUserDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	LawnDialog::Resize(theX, theY, theWidth, theHeight);
	mNameEditWidget->Resize(mContentInsets.mLeft + 12, mHeight - 155, mWidth - mContentInsets.mLeft - mContentInsets.mRight - 24, 28);
}

//0x45D970
void NewUserDialog::Draw(Graphics* g)
{
	LawnDialog::Draw(g);
	DrawEditBox(g, mNameEditWidget);
}

//0x45D9D0
void NewUserDialog::EditWidgetText(int theId, const SexyString& theString)
{
	mApp->ButtonDepress(mId + 2000);
}

//0x45D9F0
bool NewUserDialog::AllowChar(int, SexyChar theChar)
{
	return sexyisalnum(theChar) || theChar == _S(' ');
}

//0x45DA20
SexyString NewUserDialog::GetName()
{
	SexyString aString;
	SexyChar aLastChar = _S(' ');

	for (int i = 0; i < mNameEditWidget->mString.size(); i++)
	{
		SexyChar aChar = mNameEditWidget->mString[i];
		if (aChar != _S(' '))
		{
			aString.append(1, aChar);
		}
		else if (aChar != aLastChar)
		{
			aString.append(1, _S(' '));
		}

		aLastChar = aChar;
	}

	if (aString.size() && aString[aString.size() - 1] == _S(' '))
	{
		aString.resize(aString.size() - 1);
	}

	return aString;
}

void NewUserDialog::SetName(const SexyString& theName)
{
	mNameEditWidget->SetText(theName, true);
	mNameEditWidget->mCursorPos = theName.size();
	mNameEditWidget->mHilitePos = 0;
}
