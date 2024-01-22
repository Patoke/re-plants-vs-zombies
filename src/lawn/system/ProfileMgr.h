#ifndef __PROFILEMGR_H__
#define __PROFILEMGR_H__

#include <map>
#include <string>
#include "framework/Common.h"

class DataSync;
class PlayerInfo;
typedef std::pair<SexyString, PlayerInfo> ProfilePair;
typedef std::map<SexyString, PlayerInfo, Sexy::StringLessNoCase> ProfileMap;

class ProfileMgr
{
protected:
	ProfileMap			mProfileMap;			//+0x4
	unsigned long		mNextProfileId;			//+0x10
	unsigned long		mNextProfileUseSeq;		//+0x14

protected:
	void				SyncState(DataSync& theSync);
	void				DeleteOldestProfile();
	inline void			DeleteOldProfiles() { while(mProfileMap.size() > 200) DeleteOldestProfile(); }

public:
	bool				DeleteProfile(const SexyString& theName);

protected:
	/*inline*/ void		DeleteProfile(ProfileMap::iterator theProfile);

public:
	ProfileMgr() { Clear(); }  //0x46A6E0
	virtual ~ProfileMgr() { ; }  //0x46A780

	/*inline*/ void		Clear();
	void				Load();
	void				Save();
	inline int			GetNumProfiles() const { return mProfileMap.size(); }
	PlayerInfo*			GetProfile(const SexyString& theName);
	PlayerInfo*			AddProfile(const SexyString& theName);
	PlayerInfo*			GetAnyProfile();
	bool				RenameProfile(const SexyString& theOldName, const SexyString& theNewName);
	inline ProfileMap&	GetProfileMap() { return mProfileMap; }
};

#endif
