#include "DataSync.h"

DataReader::DataReader()
{
	mFile = nullptr;
	mData = nullptr;
	mDataLen = 0;
	mDataPos = 0;
	mOwnData = false;
}

//0x441B20、0x441B80
DataReader::~DataReader()
{
	if (mFile)
	{
		fclose(mFile);
		mFile = nullptr;
	}

	if (mOwnData)
	{
		delete[] mData;
	}

	mData = nullptr;
	mDataLen = 0;
	mDataPos = 0;
	mOwnData = false;
}

bool DataReader::OpenFile(const std::string& theFileName)
{
	mFile = fopen(theFileName.c_str(), "rb");
	return mFile;
}

void DataReader::OpenMemory(const void* theData, unsigned long theDataLen, bool takeOwnership)
{
	if (mFile)
	{
		fclose(mFile);
		mFile = nullptr;
	}
	if (mOwnData)
	{
		delete[] mData;
	}

	mData = (char*)theData;
	mDataLen = theDataLen;
	mOwnData = takeOwnership;
}

void DataReader::Close()
{
	if (mFile)
	{
		fclose(mFile);
		mFile = nullptr;
	}
}

//0x441BE0
void DataReader::ReadBytes(void* theMem, unsigned long theNumBytes)
{
	if (mData)
	{
		mDataPos += theNumBytes;
		if (mDataPos > mDataLen)
		{
			throw DataReaderException();
		}

		memcpy(theMem, mData, theNumBytes);
		mData += theNumBytes;
	}
	else if (!mFile || fread(theMem, sizeof(char), theNumBytes, mFile) != theNumBytes)
	{
		throw DataReaderException();
	}
}

void DataReader::Rewind(unsigned long theNumBytes)
{
	theNumBytes = std::min(theNumBytes, mDataPos);
	mDataPos -= theNumBytes;
	mData -= theNumBytes;
}

unsigned short DataReader::ReadShort()
{
	unsigned short aShort;
	ReadBytes(&aShort, sizeof(aShort));
	return aShort;
}

unsigned long DataReader::ReadLong()
{
	unsigned int aLong;
	ReadBytes(&aLong, sizeof(aLong));
	return aLong;
}

unsigned char DataReader::ReadByte()
{
	unsigned char aChar;
	ReadBytes(&aChar, sizeof(aChar));
	return aChar;
}

bool DataReader::ReadBool()
{
	bool aBool;
	ReadBytes(&aBool, sizeof(aBool));
	return aBool;
}

float DataReader::ReadFloat()
{
	float aFloat;
	ReadBytes(&aFloat, sizeof(aFloat));
	return aFloat;
}

double DataReader::ReadDouble()
{
	double aDouble;
	ReadBytes(&aDouble, sizeof(aDouble));
	return aDouble;
}

void DataReader::ReadString(SexyString& theStr)
{
	unsigned int aStrLen = ReadShort();
	theStr.resize(aStrLen);
	ReadBytes((void*)theStr.c_str(), aStrLen);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

//0x441E70
DataSync::DataSync(DataReader& theReader)
{
	Reset();
	mReader = &theReader;
}

//0x441F10
DataSync::DataSync(DataWriter& theWriter)
{
	Reset();
	mWriter = &theWriter;
}

//0x441FB0
DataSync::~DataSync()
{
}

//0x442020
void DataSync::ResetPointerTable()
{
	mIntToPointerMap.clear();
	mPointerToIntMap.clear();
	mPointerSyncList.clear();
	mCurPointerIndex = 1;
	mPointerToIntMap[nullptr] = 0;
	mIntToPointerMap[0] = nullptr;
}

void DataSync::Reset()
{
	mReader = nullptr;
	mWriter = nullptr;
	ResetPointerTable();
}

void DataSync::SyncBytes(void* theData, unsigned long theDataLen)
{
	if (mReader)
	{
		mReader->ReadBytes(theData, theDataLen);
	}
	else
	{
		mWriter->WriteBytes(theData, theDataLen);
	}
}

void DataSync::SyncLong(unsigned long& theNum)
{
	if (mReader)
	{
		theNum = mReader->ReadLong();
	}
	else
	{
		mWriter->WriteLong(theNum);
	}
}

void DataSync::SyncLong(char& theNum)
{
	SyncLong((unsigned long&)theNum);
}

void DataSync::SyncLong(short& theNum)
{
	SyncLong((unsigned long&)theNum);
}

void DataSync::SyncLong(long& theNum)
{
	SyncLong((unsigned long&)theNum);
}

void DataSync::SyncLong(unsigned char& theNum)
{
	SyncLong((unsigned long&)theNum);
}

void DataSync::SyncLong(unsigned short& theNum)
{
	SyncLong((unsigned long&)theNum);
}

void DataSync::SyncLong(int& theNum)
{
	SyncLong((unsigned long&)theNum);
}

void DataSync::SyncSLong(long& theNum)
{
	if (mReader)
	{
		theNum = (long)mReader->ReadLong();
	}
	else
	{
		mWriter->WriteLong((unsigned long)theNum);
	}
}

void DataSync::SyncSLong(char& theNum)
{
	SyncSLong((long&)theNum);
}

void DataSync::SyncSLong(short& theNum)
{
	SyncSLong((long&)theNum);
}

void DataSync::SyncSLong(int& theNum)
{
	SyncSLong((long&)theNum);
}

void DataSync::SyncSLong(unsigned char& theNum)
{
	SyncSLong((long&)theNum);
}

void DataSync::SyncSLong(unsigned short& theNum)
{
	SyncSLong((long&)theNum);
}

void DataSync::SyncSLong(unsigned long& theNum)
{
	SyncSLong((long&)theNum);
}

void DataSync::SyncShort(unsigned short& theNum)
{
	if (mReader)
	{
		theNum = mReader->ReadShort();
	}
	else
	{
		mWriter->WriteShort(theNum);
	}
}

void DataSync::SyncShort(char& theNum)
{
	SyncShort((unsigned short&)theNum);
}

void DataSync::SyncShort(short& theNum)
{
	SyncShort((unsigned short&)theNum);
}

void DataSync::SyncShort(long& theNum)
{
	SyncShort((unsigned short&)theNum);
}

void DataSync::SyncShort(unsigned char& theNum)
{
	SyncShort((unsigned short&)theNum);
}

void DataSync::SyncShort(unsigned long& theNum)
{
	SyncShort((unsigned short&)theNum);
}

void DataSync::SyncShort(int& theNum)
{
	SyncShort((unsigned short&)theNum);
}

void DataSync::SyncSShort(short& theNum)
{
	if (mReader)
	{
		theNum = (short)mReader->ReadShort();
	}
	else
	{
		mWriter->WriteShort((unsigned short)theNum);
	}
}

void DataSync::SyncSShort(char& theNum)
{
	SyncSShort((short&)theNum);
}

void DataSync::SyncSShort(long& theNum)
{
	SyncSShort((short&)theNum);
}

void DataSync::SyncSShort(unsigned char& theNum)
{
	SyncSShort((short&)theNum);
}

void DataSync::SyncSShort(unsigned short& theNum)
{
	SyncSShort((short&)theNum);
}

void DataSync::SyncSShort(unsigned long& theNum)
{
	SyncSShort((short&)theNum);
}

void DataSync::SyncSShort(int& theNum)
{
	SyncSShort((short&)theNum);
}

void DataSync::SyncByte(unsigned char& theChar)
{
	if (mReader)
	{
		theChar = mReader->ReadByte();
	}
	else
	{
		mWriter->WriteByte(theChar);
	}
}

void DataSync::SyncByte(char& theChar)
{
	SyncByte((unsigned char&)theChar);
}

void DataSync::SyncByte(short& theChar)
{
	SyncByte((unsigned char&)theChar);
}

void DataSync::SyncByte(long& theChar)
{
	SyncByte((unsigned char&)theChar);
}

void DataSync::SyncByte(unsigned short& theChar)
{
	SyncByte((unsigned char&)theChar);
}

void DataSync::SyncByte(unsigned long& theChar)
{
	SyncByte((unsigned char&)theChar);
}

void DataSync::SyncByte(int& theChar)
{
	SyncByte((unsigned char&)theChar);
}

void DataSync::SyncSByte(char& theChar)
{
	if (mReader)
	{
		theChar = (char)mReader->ReadByte();
	}
	else
	{
		mWriter->WriteByte((unsigned char)theChar);
	}
}

void DataSync::SyncSByte(short& theChar)
{
	SyncByte((char&)theChar);
}

void DataSync::SyncSByte(long& theChar)
{
	SyncByte((char&)theChar);
}

void DataSync::SyncSByte(unsigned char& theChar)
{
	SyncByte((char&)theChar);
}

void DataSync::SyncSByte(unsigned short& theChar)
{
	SyncByte((char&)theChar);
}

void DataSync::SyncSByte(unsigned long& theChar)
{
	SyncByte((char&)theChar);
}

void DataSync::SyncSByte(int& theChar)
{
	SyncByte((char&)theChar);
}

void DataSync::SyncBool(bool& theBool)
{
	if (mReader)
	{
		theBool = mReader->ReadBool();
	}
	else
	{
		mWriter->WriteBool(theBool);
	}
}

void DataSync::SyncFloat(float& theFloat)
{
	if (mReader)
	{
		theFloat = mReader->ReadFloat();
	}
	else
	{
		mWriter->WriteFloat(theFloat);
	}
}

void DataSync::SyncDouble(double& theDouble)
{
	if (mReader)
	{
		theDouble = mReader->ReadDouble();
	}
	else
	{
		mWriter->WriteDouble(theDouble);
	}
}

void DataSync::SyncString(SexyString& theStr)
{
	if (mReader)
	{
		mReader->ReadString(theStr);
	}
	else
	{
		mWriter->WriteString(theStr);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

DataWriter::DataWriter()
{
	mFile = nullptr;
	mData = nullptr;
	mDataLen = 0;
	mCapacity = 0;
}

//0x4436A0、0x4436F0
DataWriter::~DataWriter()
{
	if (mFile)
	{
		fclose(mFile);
		mFile = nullptr;
	}

	delete[] mData;
	mData = nullptr;
	mDataLen = 0;
	mCapacity = 0;
}

bool DataWriter::OpenFile(const std::string& theFileName)
{
	mFile = fopen(theFileName.c_str(), "wb");
	return mFile;
}

void DataWriter::Close()
{
	if (mFile)
	{
		fclose(mFile);
		mFile = nullptr;
	}
}

//0x443730
void DataWriter::EnsureCapacity(unsigned long theNumBytes)
{
	if (mCapacity < theNumBytes)
	{
		// 每次将容量乘 2 直到容量达到 theNumBytes 或更多
		do { mCapacity <<= 1; } while (mCapacity < theNumBytes);

		// 申请新内存
		char* aData = new char[mCapacity];
		// 将原数据迁移至新内存区域中
		memcpy(aData, mData, mDataLen);
		// 释放旧有内存区域
		delete[] mData;
		mData = aData;
	}
}

void DataWriter::OpenMemory(unsigned long theReserveAmount)
{
	if (mFile)
	{
		fclose(mFile);
		mFile = nullptr;
	}
	delete[] mData;
	mData = 0;
	mDataLen = 0;
	mCapacity = 0;

	if (theReserveAmount < 32)
		theReserveAmount = 32;
	mData = new char[theReserveAmount];
	mCapacity = theReserveAmount;
}

void DataWriter::WriteBytes(const void* theData, unsigned long theDataLen)
{
	if (mData)
	{
		EnsureCapacity(mDataLen + theDataLen);
		memcpy(mData + mDataLen, theData, theDataLen);
		mDataLen += theDataLen;
	}
	else if (mFile)
	{
		fwrite(theData, sizeof(unsigned char), theDataLen, mFile);
	}
}

//0x443770
void DataWriter::WriteLong(unsigned long theLong)
{
	//if (mData)
	//{
	//	EnsureCapacity(mDataLen + sizeof(unsigned long));
	//	*(unsigned long*)(mData + mDataLen) = theLong;
	//	mDataLen += sizeof(unsigned long);
	//}
	//else if (mFile)
	//{
	//	fwrite(&theLong, sizeof(char), sizeof(unsigned long) / sizeof(char), mFile);
	//}
	WriteBytes(&theLong, sizeof(unsigned long));
}

//0x4437C0
void DataWriter::WriteShort(unsigned short theShort)
{
	//if (mData)
	//{
	//	EnsureCapacity(mDataLen + sizeof(short));
	//	*(short*)(mData + mDataLen) = theShort;
	//	mDataLen += sizeof(short);
	//}
	//else if (mFile)
	//{
	//	fwrite(&theShort, sizeof(char), sizeof(short) / sizeof(char), mFile);
	//}
	WriteBytes(&theShort, sizeof(unsigned short));
}

void DataWriter::WriteByte(unsigned char theChar)
{
	WriteBytes(&theChar, sizeof(unsigned char));
}

void DataWriter::WriteBool(bool theBool)
{
	WriteBytes(&theBool, sizeof(bool));
}

void DataWriter::WriteFloat(float theFloat)
{
	WriteBytes(&theFloat, sizeof(float));
}

void DataWriter::WriteDouble(double theDouble)
{
	WriteBytes(&theDouble, sizeof(double));
}

//0x443810
void DataWriter::WriteString(const SexyString& theStr)
{
	unsigned short aStrLen = (unsigned short)theStr.length();
	WriteShort(aStrLen);
	WriteBytes(theStr.c_str(), (unsigned long)aStrLen);
}
