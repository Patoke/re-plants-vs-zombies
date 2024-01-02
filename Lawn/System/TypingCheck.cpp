#include "TypingCheck.h"
using namespace Sexy;

//0x51C470
TypingCheck::TypingCheck(const std::string& thePhrase)
{
	SetPhrase(thePhrase);
}

//0x51C4D0
void TypingCheck::SetPhrase(const std::string& thePhrase)
{
	for (size_t i = 0; i < thePhrase.size(); i++)
		AddChar(thePhrase[i]);
}

void TypingCheck::AddKeyCode(Sexy::KeyCode theKeyCode)
{
	mPhrase.append(1, (char)theKeyCode);
}

//0x51C510
void TypingCheck::AddChar(char theChar)
{
	theChar = (char)tolower(theChar);
	std::string aCharString{theChar};
	AddKeyCode(GetKeyCodeFromName(aCharString));
}

bool TypingCheck::Check()
{
	if (mRecentTyping.compare(mPhrase) == 0)
	{
		mRecentTyping.clear();
		return true;
	}
	return false;
}

//0x51C5A0
bool TypingCheck::Check(Sexy::KeyCode theKeyCode)
{
	mRecentTyping.append(1, (char)theKeyCode);
	size_t aLength = mPhrase.size();
	if (aLength == 0)
		return false;

	if (mRecentTyping.size() > aLength)
		mRecentTyping = mRecentTyping.substr(1, aLength);
	
	return Check();
}
