#ifndef WINDOW_INTERFACE_H
#define WINDOW_INTERFACE_H

#include "graphics/Image.h"
using namespace Sexy;

enum PointerType
{
    CURSOR_POINTER,
    CURSOR_HAND,
    CURSOR_DRAGGING,
    CURSOR_TEXT,
    CURSOR_CIRCLE_SLASH,
    CURSOR_SIZEALL,
    CURSOR_SIZENESW,
    CURSOR_SIZENS,
    CURSOR_SIZENWSE,
    CURSOR_SIZEWE,  
    CURSOR_WAIT,
    CURSOR_NONE,
    CURSOR_DEFAULT,
    CURSOR_CUSTOM,
    NUM_POINTER_TYPES,
};

/*
 * This looks  cursed but what it actually  is  is an attempt  to avoid using
 * runtime  polymorphism to implement a simple  interface.  This  is mostly a
 * programming jihadism thing because  such a powerful hammer  shouldn't need
 * to be used on such a small nail, but it's also a little bit faster. Anyway
 * this  is  an  example  of   'F-bounded polymorphism' implemented using the
 * adorably named curiously recurring template pattern. (CRTP)
 */

template <class T>
class WindowInterface {
public:
    Image* GetScreenImage()    { return static_cast<T*>(this)->GetScreenImage();      }
    void ShowWindow()          {        static_cast<T*>(this)->ShowWindow();          }
    void PollEvents()          {        static_cast<T*>(this)->PollEvents();          }
    void RehupFocus()          {        static_cast<T*>(this)->RehupFocus();          }
    bool ShouldClose()         { return static_cast<T*>(this)->ShouldClose();         }
    void ReleaseMouseCapture() {        static_cast<T*>(this)->ReleaseMouseCapture(); }
    void Draw()                {        static_cast<T*>(this)->Draw();                }
    void EnforceCursor()       {        static_cast<T*>(this)->EnforceCursor();       }
    int CreateCursor(int xHotSpot, int yHotSpot, int nWidth, int nHeight, const void* pvANDPlane, const void* pvXORPlane)
    { return static_cast<T*>(this)->CreateCursor(xHotSpot, yHotSpot, nWidth, nHeight, pvANDPlane, pvXORPlane); }

};

#endif // WINDOW_INTERFACE_H
