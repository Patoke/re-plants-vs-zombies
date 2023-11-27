#include "KeyCodes.h"

using namespace Sexy;

#define MAX_KEYNAME_LEN 12

typedef struct
{
	char mKeyName[MAX_KEYNAME_LEN];
	KeyCode mKeyCode;
} KeyNameEntry;

KeyNameEntry aKeyCodeArray[] =
{
	{"UNKNOWN", KEYCODE_UNKNOWN},
	{"LBUTTON", KEYCODE_LBUTTON},	
	{"RBUTTON", KEYCODE_RBUTTON},
	{"CANCEL", KEYCODE_CANCEL},
	{"MBUTTON", KEYCODE_MBUTTON},
	{"BACK", KEYCODE_BACK},
	{"TAB", KEYCODE_TAB},
	{"CLEAR", KEYCODE_CLEAR},
	{"RETURN", KEYCODE_RETURN},
	{"SHIFT", KEYCODE_SHIFT},
	{"CONTROL", KEYCODE_CONTROL},
	{"MENU", KEYCODE_MENU},
	{"PAUSE", KEYCODE_PAUSE},
	{"CAPITAL", KEYCODE_CAPITAL},
	{"KANA", KEYCODE_KANA},
	{"HANGEUL", KEYCODE_HANGEUL},
	{"HANGUL", KEYCODE_HANGUL},
	{"JUNJA", KEYCODE_JUNJA},
	{"FINAL", KEYCODE_FINAL},
	{"HANJA", KEYCODE_HANJA},
	{"KANJI", KEYCODE_KANJI},
	{"ESCAPE", KEYCODE_ESCAPE},
	{"CONVERT", KEYCODE_CONVERT},
	{"NONCONVERT", KEYCODE_NONCONVERT},
	{"ACCEPT", KEYCODE_ACCEPT},
	{"MODECHANGE", KEYCODE_MODECHANGE},
	{"SPACE", KEYCODE_SPACE},
	{"PRIOR", KEYCODE_PRIOR},
	{"NEXT", KEYCODE_NEXT},
	{"END", KEYCODE_END},
	{"HOME", KEYCODE_HOME},
	{"LEFT", KEYCODE_LEFT},
	{"UP", KEYCODE_UP},
	{"RIGHT", KEYCODE_RIGHT},
	{"DOWN", KEYCODE_DOWN},
	{"SELECT", KEYCODE_SELECT},
	{"PRINT", KEYCODE_PRINT},
	{"EXECUTE", KEYCODE_EXECUTE},
	{"SNAPSHOT", KEYCODE_SNAPSHOT},
	{"INSERT", KEYCODE_INSERT},
	{"DELETE", KEYCODE_DELETE},
	{"HELP", KEYCODE_HELP},
	{"LWIN", KEYCODE_LWIN},
	{"RWIN", KEYCODE_RWIN},
	{"APPS", KEYCODE_APPS},
	{"NUMPAD0", KEYCODE_NUMPAD0},
	{"NUMPAD1", KEYCODE_NUMPAD1},
	{"NUMPAD2", KEYCODE_NUMPAD2},
	{"NUMPAD3", KEYCODE_NUMPAD3},
	{"NUMPAD4", KEYCODE_NUMPAD4},
	{"NUMPAD5", KEYCODE_NUMPAD5},
	{"NUMPAD6", KEYCODE_NUMPAD6},
	{"NUMPAD7", KEYCODE_NUMPAD7},
	{"NUMPAD8", KEYCODE_NUMPAD8},
	{"NUMPAD9", KEYCODE_NUMPAD9},
	{"MULTIPLY", KEYCODE_MULTIPLY},
	{"ADD", KEYCODE_ADD},
	{"SEPARATOR", KEYCODE_SEPARATOR},
	{"SUBTRACT", KEYCODE_SUBTRACT},
	{"DECIMAL", KEYCODE_DECIMAL},
	{"DIVIDE", KEYCODE_DIVIDE},
	{"F1", KEYCODE_F1},
	{"F2", KEYCODE_F2},
	{"F3", KEYCODE_F3},
	{"F4", KEYCODE_F4},
	{"F5", KEYCODE_F5},
	{"F6", KEYCODE_F6},
	{"F7", KEYCODE_F7},
	{"F8", KEYCODE_F8},
	{"F9", KEYCODE_F9},
	{"F10", KEYCODE_F10},
	{"F11", KEYCODE_F11},
	{"F12", KEYCODE_F12},
	{"F13", KEYCODE_F13},
	{"F14", KEYCODE_F14},
	{"F15", KEYCODE_F15},
	{"F16", KEYCODE_F16},
	{"F17", KEYCODE_F17},
	{"F18", KEYCODE_F18},
	{"F19", KEYCODE_F19},
	{"F20", KEYCODE_F20},
	{"F21", KEYCODE_F21},
	{"F22", KEYCODE_F22},
	{"F23", KEYCODE_F23},
	{"F24", KEYCODE_F24},
	{"NUMLOCK", KEYCODE_NUMLOCK},
	{"SCROLL", KEYCODE_SCROLL}	
};

KeyCode	Sexy::GetKeyCodeFromName(const std::string& theKeyName)
{
	char aKeyName[MAX_KEYNAME_LEN];

	if (theKeyName.length() >= MAX_KEYNAME_LEN-1)
		return KEYCODE_UNKNOWN;

	strcpy(aKeyName, theKeyName.c_str());
	strupr(aKeyName);

	if (theKeyName.length() == 1)
	{
		unsigned char aKeyNameChar = aKeyName[0];

		if ((aKeyNameChar >= (unsigned char) KEYCODE_ASCIIBEGIN) && (aKeyNameChar <= (unsigned char) KEYCODE_ASCIIEND))
			return (KeyCode) aKeyNameChar;

		if ((aKeyNameChar >= ((unsigned char) KEYCODE_ASCIIBEGIN2) - 0x80) && (aKeyNameChar <= ((unsigned char) KEYCODE_ASCIIEND2) - 0x80))
			return (KeyCode) (aKeyNameChar + 0x80);
	}	

	for (size_t i = 0; i < sizeof(aKeyCodeArray)/sizeof(aKeyCodeArray[0]); i++)	
		if (strcmp(aKeyName, aKeyCodeArray[i].mKeyName) == 0)
			return aKeyCodeArray[i].mKeyCode;	

	return KEYCODE_UNKNOWN;
}

const std::string Sexy::GetKeyNameFromCode(const KeyCode& theKeyCode)
{
	if ((theKeyCode >= KEYCODE_ASCIIBEGIN) && (theKeyCode <= KEYCODE_ASCIIEND))
	{
		char aStr[2] = {(char) theKeyCode, 0};
		return aStr;
	}

	if ((theKeyCode >= KEYCODE_ASCIIBEGIN2) && (theKeyCode <= KEYCODE_ASCIIEND2))
	{
		char aStr[2] = {(char)(((unsigned char) theKeyCode) - 0x80), 0};
		return aStr;
	}

	for (size_t i = 0; i < sizeof(aKeyCodeArray)/sizeof(aKeyCodeArray[0]); i++)	
		if (theKeyCode == aKeyCodeArray[i].mKeyCode)
			return aKeyCodeArray[i].mKeyName;	

	return "UNKNOWN";
}
