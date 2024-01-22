#ifndef __CURSOROBJECT_H__
#define __CURSOROBJECT_H__

#include "GameObject.h"

class CursorObject : public GameObject
{
public:
	int						mSeedBankIndex;			//+0x24
	SeedType				mType;					//+0x28
	SeedType				mImitaterType;			//+0x2C
	CursorType				mCursorType;			//+0x30
	CoinID					mCoinID;				//+0x34
	PlantID					mGlovePlantID;			//+0x38
	PlantID					mDuplicatorPlantID;		//+0x3C
	PlantID					mCobCannonPlantID;		//+0x40
	int						mHammerDownCounter;		//+0x44
	ReanimationID			mReanimCursorID;		//+0x48

public:
	CursorObject();

	void					Update();
	void					Draw(Graphics* g);
	void					Die();
};

class CursorPreview : public GameObject
{
public:
	int						mGridX;
	int						mGridY;

public:
	CursorPreview();

	void					Update();
	void					Draw(Graphics* g);
};

#endif
