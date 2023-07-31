#ifndef __DATASYNC_H__
#define __DATASYNC_H__

#include "../../SexyAppFramework/Common.h"

class DataReader
{
protected:
	FILE*					mFile;			//+0x4
	char*					mData;			//+0x8
	unsigned long			mDataLen;		//+0xC
	unsigned long			mDataPos;		//+0x10
	bool					mOwnData;		//+0x14

public:
	DataReader();
	virtual ~DataReader();

	bool					OpenFile(const std::string& theFileName);
	void					OpenMemory(const void* theData, unsigned long theDataLen, bool takeOwnership);
	void					Close();
	void					ReadBytes(void* theMem, unsigned long theNumBytes);
	void					Rewind(unsigned long theNumBytes);
	unsigned long			ReadLong();
	unsigned short			ReadShort();
	unsigned char			ReadByte();
	bool					ReadBool();
	float					ReadFloat();
	double					ReadDouble();
	void					ReadString(SexyString& theStr);
};
class DataReaderException : public std::exception
{
};

class DataWriter
{
protected:
	FILE*					mFile;			//+0x4
	char*					mData;			//+0x8
	unsigned long			mDataLen;		//+0xC
	unsigned long			mCapacity;		//+0x10

protected:
	void					EnsureCapacity(unsigned long theNumBytes);

public:
	DataWriter();
	virtual ~DataWriter();

	bool					OpenFile(const std::string& theFileName);
	void					OpenMemory(unsigned long theReserveAmount = 0x20);
	void					Close();
	inline bool				WriteToFile(const std::string& theFileName) { /* Œ¥’“µΩ */ }
	void					WriteBytes(const void* theData, unsigned long theDataLen);
	void					WriteLong(unsigned long theLong);
	void					WriteShort(unsigned short theShort);
	void					WriteByte(unsigned char theChar);
	void					WriteBool(bool theBool);
	void					WriteFloat(float theFloat);
	void					WriteDouble(double theDouble);
	void					WriteString(const SexyString& theStr);
	inline unsigned long	GetPos() { /* Œ¥’“µΩ */ }
	inline void				SetLong(unsigned long, unsigned long) { /* Œ¥’“µΩ */ }
	inline void				SetShort(unsigned int, unsigned long) { /* Œ¥’“µΩ */ }
	inline void				SetByte(unsigned int, unsigned long) { /* Œ¥’“µΩ */ }
	inline void*			GetDataPtr() { return mData; }
	inline int				GetDataLen() { return mDataLen; }
};

typedef std::map<void*, int> PointerToIntMap;
typedef std::map<int, void*> IntToPointerMap;

class DataSync
{
protected:
	DataReader*				mReader;
	DataWriter*				mWriter;
	int						mVersion;
	PointerToIntMap			mPointerToIntMap;
	IntToPointerMap			mIntToPointerMap;
	std::vector<void**>		mPointerSyncList;
	int						mCurPointerIndex;

protected:
	void ResetPointerTable();
	void Reset();

public:
	DataSync(DataReader& theReader);
	DataSync(DataWriter& theWriter);
	virtual ~DataSync();

	inline void				SyncPointers() { /* Œ¥’“µΩ */ }
	inline void				SetReader(DataReader* theReader) { mReader = theReader; }
	inline void				SetWriter(DataWriter* theWriter) { mWriter = theWriter; }
	inline DataReader*		GetReader() { return mReader; }
	inline DataWriter*		GetWriter() { return mWriter; }
	void					SyncBytes(void* theData, unsigned long theDataLen);
	void					SyncLong(char& theNum);
	void					SyncLong(short& theNum);
	void					SyncLong(long& theNum);
	void					SyncLong(unsigned char& theNum);
	void					SyncLong(unsigned short& theNum);
	void					SyncLong(unsigned long& theNum);
	void					SyncLong(int& theNum);
	void					SyncSLong(char& theNum);
	void					SyncSLong(short& theNum);
	void					SyncSLong(long& theNum);
	void					SyncSLong(unsigned char& theNum);
	void					SyncSLong(unsigned short& theNum);
	void					SyncSLong(unsigned long& theNum);
	void					SyncSLong(int& theNum);
	void					SyncShort(char& theNum);
	void					SyncShort(short& theNum);
	void					SyncShort(long& theNum);
	void					SyncShort(unsigned char& theNum);
	void					SyncShort(unsigned short& theNum);
	void					SyncShort(unsigned long& theNum);
	void					SyncShort(int& theNum);
	void					SyncSShort(char& theNum);
	void					SyncSShort(short& theNum);
	void					SyncSShort(long& theNum);
	void					SyncSShort(unsigned char& theNum);
	void					SyncSShort(unsigned short& theNum);
	void					SyncSShort(unsigned long& theNum);
	void					SyncSShort(int& theNum);
	void					SyncByte(char& theChar);
	void					SyncByte(short& theChar);
	void					SyncByte(long& theChar);
	void					SyncByte(unsigned char& theChar);
	void					SyncByte(unsigned short& theChar);
	void					SyncByte(unsigned long& theChar);
	void					SyncByte(int& theChar);
	void					SyncSByte(char& theChar);
	void					SyncSByte(short& theChar);
	void					SyncSByte(long& theChar);
	void					SyncSByte(unsigned char& theChar);
	void					SyncSByte(unsigned short& theChar);
	void					SyncSByte(unsigned long& theChar);
	void					SyncSByte(int& theChar);
	void					SyncBool(bool& theBool);
	void					SyncFloat(float& theFloat);
	void					SyncDouble(double& theDouble);
	void					SyncString(SexyString& theStr);
	inline void				SyncPointer(void**) { /* Œ¥’“µΩ */ }
	inline void				RegisterPointer(void*) { /* Œ¥’“µΩ */ }
	inline void				SetVersion(int theVersion) { mVersion = theVersion; }
	inline int				GetVersion() const { return mVersion; }
};

#endif