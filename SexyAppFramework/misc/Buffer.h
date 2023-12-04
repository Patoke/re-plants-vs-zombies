#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <string>
#include "Common.h"

namespace Sexy
{

typedef std::vector<uchar> ByteVector;

class Buffer
{
public:
	ByteVector				mData;
	int						mDataBitSize;
	mutable int				mReadBitPos;
	mutable int				mWriteBitPos;	

public:
	Buffer();
	virtual ~Buffer();
			
	void					SeekFront() const;
	void					Clear();

	void					FromWebString(const std::string& theString);
	void					WriteByte(uchar theByte);
	void					WriteNumBits(int theNum, int theBits);
	static int				GetBitsRequired(int theNum, bool isSigned);
	void					WriteBoolean(bool theBool);
	void					WriteShort(short theShort);
	void					WriteLong(long theLong);
	void					WriteString(const std::string& theString);
	void					WriteUTF8String(const std::wstring& theString);
	void					WriteLine(const std::string& theString);	
	void					WriteBuffer(const ByteVector& theBuffer);
	void					WriteBytes(const uchar* theByte, int theCount);
	void					SetData(const ByteVector& theBuffer);
	void					SetData(uchar* thePtr, int theCount);

	std::string				ToWebString() const;
	std::wstring			UTF8ToWideString() const;
	uchar					ReadByte() const;
	int						ReadNumBits(int theBits, bool isSigned) const;
	bool					ReadBoolean() const;
	short					ReadShort() const;
	long					ReadLong() const;
	std::string				ReadString() const;	
	std::wstring			ReadUTF8String() const;
	std::string				ReadLine() const;
	void					ReadBytes(uchar* theData, int theLen) const;
	void					ReadBuffer(ByteVector* theByteVector) const;

	const uchar*			GetDataPtr() const;
	int						GetDataLen() const;	
	int						GetDataLenBits() const;
	ulong					GetCRC32(ulong theSeed = 0) const;

	bool					AtEnd() const;
	bool					PastEnd() const;
};

}

#endif //__BUFFER_H__