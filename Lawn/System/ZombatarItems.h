#ifndef __ZOMBATARITEMS_H__
#define __ZOMBATARITEMS_H__

class ZombatarItem
{
public:
	struct Image* mImage;
	struct Image* mColorMask;
	int unk1;
	int unk2;
	int unk3;
	int unk4;
	int unk5;
	int unk6;
	const char* mTrackName;
	int mRenderGroup;
};

#endif