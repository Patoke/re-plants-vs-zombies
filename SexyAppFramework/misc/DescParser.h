#ifndef __DESCPARSER_H__
#define __DESCPARSER_H__

#include "Common.h"

namespace Sexy
{

class DataElement 
{
public:	
	bool					mIsList;

public:
	DataElement();
	virtual ~DataElement();

	virtual DataElement*	Duplicate() = 0;
};

class SingleDataElement : public DataElement
{
public:
	std::string				mString;	

public:
	SingleDataElement();
	SingleDataElement(const std::string theString);
	virtual ~SingleDataElement();

	virtual DataElement*	Duplicate();
};

typedef std::vector<DataElement*> ElementVector;

class ListDataElement : public DataElement
{
public:
	ElementVector			mElementVector;

public:
	ListDataElement();
	ListDataElement(const ListDataElement& theListDataElement);
	virtual ~ListDataElement();
	
	ListDataElement&		operator=(const ListDataElement& theListDataElement);

	virtual DataElement*	Duplicate();
};

typedef std::map<std::string, DataElement*> DataElementMap;
typedef std::vector<std::string> StringVector;
typedef std::vector<int> IntVector;
typedef std::vector<double> DoubleVector;

class DescParser
{
public:
	enum
	{
		CMDSEP_SEMICOLON = 1,
		CMDSEP_NO_INDENT = 2
	};

public:
	int						mCmdSep;

	std::string				mError;
	int						mCurrentLineNum;
	std::string				mCurrentLine;
	DataElementMap			mDefineMap;

public:
	virtual bool			Error(const std::string& theError);
	virtual DataElement*	Dereference(const std::string& theString);
	bool					IsImmediate(const std::string& theString);
	std::string				Unquote(const std::string& theQuotedString);
	bool					GetValues(ListDataElement* theSource, ListDataElement* theValues);
	std::string				DataElementToString(DataElement* theDataElement);
	bool					DataToString(DataElement* theSource, std::string* theString);
	bool					DataToInt(DataElement* theSource, int* theInt);
	bool					DataToStringVector(DataElement* theSource, StringVector* theStringVector);
	bool					DataToList(DataElement* theSource, ListDataElement* theValues);
	bool					DataToIntVector(DataElement* theSource, IntVector* theIntVector);
	bool					DataToDoubleVector(DataElement* theSource, DoubleVector* theDoubleVector);
	bool					ParseToList(const std::string& theString, ListDataElement* theList, bool expectListEnd, int* theStringPos);
	bool					ParseDescriptorLine(const std::string& theDescriptorLine);

	// You must implement this one
	virtual bool			HandleCommand(const ListDataElement& theParams) = 0;
	
public:
	DescParser();
	virtual ~DescParser();	

	bool					LoadDescriptor(const std::string& theFileName);	
};

}

#endif //__DESCPARSER_H__