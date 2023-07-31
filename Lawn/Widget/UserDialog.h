#ifndef __USERDIALOG_H__
#define __USERDIALOG_H__

#include "LawnDialog.h"
#include "../../SexyAppFramework/ListListener.h"
#include "../../SexyAppFramework/EditListener.h"

namespace Sexy
{
	class ListWidget;
};

class UserDialog : public LawnDialog, public ListListener, public EditListener
{
protected:
	enum
	{
		UserDialog_RenameUser,
		UserDialog_DeleteUser
	};

public:
	ListWidget*			mUserList;				//+0x174
	DialogButton*		mRenameButton;			//+0x178
	DialogButton*		mDeleteButton;			//+0x17C
	int					mNumUsers;				//+0x180

public:
	UserDialog(LawnApp* theApp);
	virtual ~UserDialog();

	virtual void		Resize(int theX, int theY, int theWidth, int theHeight);
	virtual int			GetPreferredHeight(int theWidth);
	virtual void		AddedToManager(WidgetManager* theWidgetManager);
	virtual void		RemovedFromManager(WidgetManager* theWidgetManager);
	virtual void		ListClicked(int theId, int theIdx, int theClickCount);
	virtual void		ButtonDepress(int theId);
	virtual void		EditWidgetText(int theId, const SexyString& theString);
	virtual bool		AllowChar(int theId, SexyChar theChar);
	virtual void		Draw(Graphics* g);
	void				FinishDeleteUser();
	void				FinishRenameUser(const SexyString& theNewName);
	SexyString			GetSelName();
};

#endif
