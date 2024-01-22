#ifndef __TYPINGCHECK_H__
#define __TYPINGCHECK_H__

#include <string>
#include "framework/misc/KeyCodes.h"

class TypingCheck
{
protected:
	std::string		mPhrase;
	std::string		mRecentTyping;

public:
	TypingCheck() : mPhrase() { }
	TypingCheck(const std::string& thePhrase);

	void			SetPhrase(const std::string& thePhrase);
	/*inline*/ void	AddKeyCode(Sexy::KeyCode theKeyCode);
	void			AddChar(char theChar);
	/*inline*/ bool	Check();
	bool			Check(Sexy::KeyCode theKeyCode);
};

#endif
