#ifndef WINDOW_INTERFACE_H
#define WINDOW_INTERFACE_H

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
	bool IsFocused() { return static_cast<T*>(this)->IsFocused(); }
	bool ShouldClose() { return static_cast<T*>(this)->ShouldClose(); }
	void ReleaseMouseCapture() { return static_cast<T*>(this)->ReleaseMouseCapture(); }
	void Draw() { static_cast<T*>(this)->Draw(); }
};

#endif // WINDOW_INTERFACE_H
