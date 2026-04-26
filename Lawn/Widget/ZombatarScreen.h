#ifndef __ZOMBATARSCREEN_H__
#define __ZOMBATARSCREEN_H__
// @Patoke: implement file

#include "../../ConstEnums.h"
#include "widget/Widget.h"
#include "../../LawnApp.h"
#include "ZombatarItems.h"
#include "LawnDialog.h"

class LawnApp;

class ZombatarScreen : public LawnDialog {
public:
	LawnApp* mApp;							//+GOTY @Patoke: 0x184
	DWORD dword188;							//+GOTY @Patoke: 0x188
	DWORD dword18C;							//+GOTY @Patoke: 0x18C
	DWORD dword190;							//+GOTY @Patoke: 0x190
	ZombatarItems* mZombatarItems;			//+GOTY @Patoke: 0x194

	ZombatarScreen(LawnApp* theApp);
	virtual ~ZombatarScreen();

};

#endif