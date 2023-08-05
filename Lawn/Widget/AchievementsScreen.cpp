#include "AchievementsScreen.h"
#include "../Board.h"
#include "GameButton.h"
#include "GameSelector.h"
#include "../../LawnApp.h"
#include "AlmanacDialog.h"
#include "../../Resources.h"
#include "../System/Music.h"
#include "../../GameConstants.h"
#include "../System/PlayerInfo.h"
#include "../System/ProfileMgr.h"
#include "../../Sexy.TodLib/TodFoley.h"
#include "../../Sexy.TodLib/TodDebug.h"
#include "../../SexyAppFramework/Font.h"
#include "../../Sexy.TodLib/Reanimator.h"
#include "../../Sexy.TodLib/TodParticle.h"
#include "../../SexyAppFramework/Dialog.h"
#include "../../SexyAppFramework/WidgetManager.h"

Rect aBackButtonRect = { 120, 35, 130, 80 };

// GOTY @Patoke: 0x401000
AchievementsWidget::AchievementsWidget(LawnApp* theApp) {
	mApp = theApp;
	mWidth = 800;
	mHeight = IMAGE_ACHEESEMENTS_CHINA->mHeight + IMAGE_SELECTORSCREEN_ACHIEVEMENTS_BG->mHeight + 15700;
	mScrollDirection = -1;
	mScrollValue = 0;
	mDefaultScrollValue = 30;
	mScrollDecay = 1;
	mDidPressMoreButton = false;
	mMoreRockRect = Rect(710, 470, IMAGE_ACHEESEMENTS_MORE_ROCK->mWidth - 25, IMAGE_ACHEESEMENTS_MORE_ROCK->mHeight - 50);
}

// GOTY @Patoke: 0x4010E0
AchievementsWidget::~AchievementsWidget() {

}

// GOTY @Patoke: 0x401A10
void AchievementsWidget::Update() {
	if (mScrollValue <= 0)
		return;

	if (mScrollValue > mDefaultScrollValue)
		mScrollValue = mDefaultScrollValue;

	mScrollValue -= mScrollDecay;

	int aNewY = mY + mScrollValue * mScrollDirection;
	if (aNewY >= -1)
		aNewY = -1;
	//if (aNewY >= mApp->mHeight)
	//	aNewY = mApp->mHeight;

	int aMaxScroll = 2 * mApp->mHeight + 50 - mHeight;
	if (aNewY <= aMaxScroll)
		aNewY = aMaxScroll;

	mY = aNewY;

	int aDelta = aNewY - mY;
	mMoreRockRect.mY += aDelta;
	aBackButtonRect.mY += aDelta;

	if (mScrollValue <= 0)
		mScrollValue = 0;
}

// GOTY @Patoke: 0x401160
void AchievementsWidget::Draw(Graphics* g) {
	g->DrawImage(IMAGE_SELECTORSCREEN_ACHIEVEMENTS_BG, 0, 0);

	int aHeight = IMAGE_SELECTORSCREEN_ACHIEVEMENTS_BG->mHeight;
	int i = 70;
	do {
		g->DrawImage(IMAGE_ACHEESEMENTS_HOLE_TILE, 0, aHeight);
		aHeight += 225;
		--i;
	} while (i);

	g->DrawImage(IMAGE_ACHEESEMENTS_BOOKWORM, 0, 1125);
	g->DrawImage(IMAGE_ACHEESEMENTS_BEJEWELED, 0, 2250);
	g->DrawImage(IMAGE_ACHEESEMENTS_CHUZZLE, 0, 4500);
	g->DrawImage(IMAGE_ACHEESEMENTS_PEGGLE, 0, 6750);
	g->DrawImage(IMAGE_ACHEESEMENTS_PIPE, 0, 9000);
	g->DrawImage(IMAGE_ACHEESEMENTS_ZUMA, 0, 11250);

	g->DrawImage(IMAGE_ACHEESEMENTS_CHINA, 0, mHeight - IMAGE_ACHEESEMENTS_CHINA->mHeight - /*50*/ 650);
	
	if (aBackButtonRect.Contains(mWidgetManager->mLastMouseX - mX, mWidgetManager->mLastMouseY - mY))
		g->DrawImage(IMAGE_ACHEESEMENTS_BACK_HIGHLIGHT, 128, 55);

	// todo @Patoke: finish rebuilding this

	g->DrawImage(IMAGE_ACHEESEMENTS_MORE_ROCK, 700, 450);

	bool aIsHighlight = mMoreRockRect.Contains(mWidgetManager->mLastMouseX - mX, mWidgetManager->mLastMouseY - mY);
	if (mDidPressMoreButton) {
		g->DrawImage(aIsHighlight ? IMAGE_ACHEESEMENTS_TOP_BUTTON_HIGHLIGHT : IMAGE_ACHEESEMENTS_TOP_BUTTON, 700, 450);
	}
	else {
		g->DrawImage(aIsHighlight ? IMAGE_ACHEESEMENTS_MORE_BUTTON_HIGHLIGHT : IMAGE_ACHEESEMENTS_MORE_BUTTON, 700, 450);
	}
}

// GOTY @Patoke: 0x4019D0
void AchievementsWidget::KeyDown(KeyCode theKey) {
	if (theKey == KEYCODE_UP) {
		mScrollValue = mDefaultScrollValue;
		mScrollDirection = 1;
	}
	else if (theKey == KEYCODE_DOWN) {
		mScrollValue = mDefaultScrollValue;
		mScrollDirection = -1;
	}
}

// GOTY @Patoke: 0x4017F0
void AchievementsWidget::MouseDown(int x, int y, int theClickCount) {
	if (aBackButtonRect.Contains(x, y))
		mApp->PlaySample(SOUND_GRAVEBUTTON);

	if (mMoreRockRect.Contains(x, y))
		mApp->PlaySample(SOUND_GRAVEBUTTON);
}

// GOTY @Patoke: 0x401890
void AchievementsWidget::MouseUp(int x, int y, int theClickCount) {
	Point aPos = Point(x, y);
	if (aBackButtonRect.Contains(aPos)) {
		mApp->mGameSelector->SlideTo(0, 0);
		mApp->mGameSelector->mWidgetManager->SetFocus(mApp->mGameSelector);
	}

	if (mMoreRockRect.Contains(aPos)) {
		mDidPressMoreButton = !mDidPressMoreButton;
		mScrollDirection = mDidPressMoreButton ? -1 : 1;
		mScrollValue = 20;
	}
}

// GOTY @Patoke: 0x4019A0
void AchievementsWidget::MouseWheel(int theDelta) {
	mScrollValue = mDefaultScrollValue;

	if (theDelta > 0)
		mScrollDirection = 1;
	else if (theDelta < 0)
		mScrollDirection = -1;
}