#ifndef __IMITATERDIALOG_H__
#define __IMITATERDIALOG_H__

#include "LawnDialog.h"

class ToolTipWidget;
class ImitaterDialog : public LawnDialog
{
public:
    ToolTipWidget*          mToolTip;
    SeedType                mToolTipSeed;

public:
    ImitaterDialog();
    virtual ~ImitaterDialog();

    SeedType                SeedHitTest(int x, int y);
    void                    UpdateCursor();
    virtual void            Update();
    /*inline*/ void         GetSeedPosition(int theIndex, int& x, int& y);
    virtual void            Draw(Graphics* g);
    void                    ShowToolTip();
    /*inline*/ void         RemoveToolTip();
    virtual void            MouseDown(int x, int y, int theClickCount);
    virtual void            MouseUp(int, int, int){}
};

#endif
