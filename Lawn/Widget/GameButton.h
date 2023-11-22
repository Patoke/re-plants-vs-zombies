#ifndef __GAMEBUTTON_H__
#define __GAMEBUTTON_H__

#include "../../SexyAppFramework/SexyVector.h"
#include "../../SexyAppFramework/DialogButton.h"

using namespace std;
using namespace Sexy;

class LawnApp;
class GameButton
{
public:
	enum
	{
		BUTTON_LABEL_LEFT = -1,
		BUTTON_LABEL_CENTER = 0,
		BUTTON_LABEL_RIGHT = 1
	};
	enum
	{
		COLOR_LABEL = 0,
		COLOR_LABEL_HILITE = 1,
		COLOR_DARK_OUTLINE = 2,
		COLOR_LIGHT_OUTLINE = 3,
		COLOR_MEDIUM_OUTLINE = 4,
		COLOR_BKG = 5,
		NUM_COLORS = 6
	};

public:
	LawnApp*				mApp;							//+0x0
	Widget*					mParentWidget;					//+0x4
	int						mX;								//+0x8
	int						mY;								//+0xC
	int						mWidth;							//+0x10
	int						mHeight;						//+0x14
	bool					mIsOver;						//+0x18
	bool					mIsDown;						//+0x19
	bool					mDisabled;						//+0x1A
	Color					mColors[6];						//+0x1C
	int						mId;							//+0x7C
	SexyString				mLabel;							//+0x80
	int						mLabelJustify;					//+0x9C
	_Font*					mFont;							//+0xA0
	Image*					mButtonImage;					//+0xA4
	Image*					mOverImage;						//+0xA8
	Image*					mDownImage;						//+0xAC
	Image*					mDisabledImage;					//+0xB0
	Image*					mOverOverlayImage;				//+0xB4
	Rect					mNormalRect;					//+0xB8
	Rect					mOverRect;						//+0xC8
	Rect					mDownRect;						//+0xD8
	Rect					mDisabledRect;					//+0xE8
	bool					mInverted;						//+0xF8
	bool					mBtnNoDraw;						//+0xF9
	bool					mFrameNoDraw;					//+0xFA
	double					mOverAlpha;						//+0x100
	double					mOverAlphaSpeed;				//+0x108
	double					mOverAlphaFadeInSpeed;			//+0x110			
	bool					mDrawStoneButton;				//+0x118
	int						mTextOffsetX;					//+0x11C
	int						mTextOffsetY;					//+0x120
	int						mButtonOffsetX;					//+0x124
	int						mButtonOffsetY;					//+0x128

public:
	GameButton(int theId);
	~GameButton();

	static /*inline*/ bool	HaveButtonImage(Image* theImage, Rect& theRect);
	void					DrawButtonImage(Graphics* g, Image* theImage, Rect& theRect, int theX, int theY);
	/*inline*/ void			SetFont(_Font* theFont);
	/*inline*/ bool			IsButtonDown();
	void					Draw(Graphics* g);
	/*inline*/ void			SetDisabled(bool theDisabled);
	/*inline*/ bool			IsMouseOver();
	void					Update();
	/*inline*/ void			Resize(int theX, int theY, int theWidth, int theHeight);
	/*inline*/ void			SetLabel(const SexyString& theLabel);
};

class LawnStoneButton : public DialogButton
{
public:
	LawnStoneButton(Image* theComponentImage, int theId, ButtonListener* theListener) : DialogButton(theComponentImage, theId, theListener) { }

	virtual void			Draw(Graphics* g);
	/*inline*/ void			SetLabel(const SexyString& theLabel);
};

class NewLawnButton : public DialogButton
{
public:
    _Font*					mHiliteFont;            //+0x138
    int						mTextDownOffsetX;       //+0x13C
    int						mTextDownOffsetY;       //+0x140
	int						mButtonOffsetX;			//+0x144
	int						mButtonOffsetY;			//+0x148
	bool					mUsePolygonShape;		//+0x14C
	SexyVector2				mPolygonShape[4];		//+0x150

public:
    NewLawnButton(Image* theComponentImage, int theId, ButtonListener* theListener);
	virtual ~NewLawnButton();
	
    virtual void			Draw(Graphics* g);
	virtual bool			IsPointVisible(int x, int y);
    void					SetLabel(const SexyString& theLabel);
	// @Patoke: user defined
	void					SetOffset(int theX, int theY);
};

LawnStoneButton*			MakeButton(int theId, ButtonListener* theListener, const SexyString& theText);
NewLawnButton*				MakeNewButton(int theId, ButtonListener* theListener, const SexyString& theText, _Font* theFont, Image* theImageNormal, Image* theImageOver, Image* theImageDown);
void						DrawStoneButton(Graphics* g, int x, int y, int theWidth, int theHeight, bool isDown, bool isHighLighted, const SexyString& theLabel);

#endif
