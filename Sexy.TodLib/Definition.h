#ifndef __TODDEFINITION_H__
#define __TODDEFINITION_H__

#include <string>
#include "TodList.h"
#include "Reanimator.h"
#include "TodParticle.h"

enum class DefFieldType : int
{
    DT_INVALID,
    DT_INT,
    DT_FLOAT,
    DT_STRING,
    DT_ENUM,
    DT_VECTOR2,
    DT_ARRAY,
    DT_TRACK_FLOAT,
    DT_FLAGS,
    DT_IMAGE,
    DT_FONT
};

/*
    [Ϊͨ������������ݣ��ڴ˹涨]
        �����ڴ洢������Ķ������ݡ����࣬��Ϊ���������࣬���� _DefClass����Ӧ�أ����� _DefClass ���������� _Class��
        ���磬ReanimatorDefinition ��Ϊ Reanimation �ࣨ�����ࣩ�Ķ��������࣬TodParticleDefinition ��Ϊ TodParticleSystem �ࣨ����ϵͳ�ࣩ�Ķ���������ȡ�
*/

// ====================================================================================================
// �� �������־��
// ----------------------------------------------------------------------------------------------------
// ���ڰ���־λ�жϵģ�/ö�����͵ģ����ݣ�һ�� DefSymbol ��¼��һ����־λ�ϵģ�/һ��ö����ģ�ֵ��
// ====================================================================================================
class DefSymbol
{
public:
    int                 mSymbolValue;                   //+0x0����־λ�ϵ�ֵ��ö�����Ӧ����ֵ����Ϊ -1 ���ʾ�����ڸ���
    const char*         mSymbolName;                    //+0x4����־λ��ö��������ƣ�Ϊ��ָ��ʱ��ʾ�����ڸ���ʱ���Ϊ��ȡ�����ı�־
};
//extern DefSymbol gParticleFlagSymbols[];  //0x69E290
//extern DefSymbol gEmitterTypeSymbols[];  //0x69E260
//extern DefSymbol gParticleTypeSymbols[];  //0x69E200

// ====================================================================================================
// �� ���ṹ�ֶΡ�
// ----------------------------------------------------------------------------------------------------
// �ṹ�ֶμ�¼��һ�����е�һ���ĳ�Ա������_MemVar�������ݺ������������еĽṹ��
// ====================================================================================================
class DefField
{
public:
    const char*         mFieldName;                     //+0x0��ָ�� _MemVar �����ơ�ָ����ַ�����ʱ��ʾ�޴˱������ʱ���Ϊ��ȡ�����ı�־
    int                 mFieldOffset;                   //+0x4��_MemVar ���������е�ƫ��������ϻ����⣩
    DefFieldType        mFieldType;                     //+0x8��*_MemVar �����ݴ洢���ͣ���ͬ���͵����ݵĶ�ȡ��ʽҲ������ͬ
    void*               mExtraData;                     //+0xC���������ݡ����ڶ� *_MemVar �а�����ָ��������������
    // �� _MemVar Ϊָ�������������ݵ�ָ���ͱ������� mExtraData Ϊָ�� _MemVar ���������Ķ���ṹͼ��ָ�룻
    // �� _MemVar Ϊ��־��ö�����͵����ݣ��� mExtraData Ϊָ�������־���ݵ� DefSymbol �����ָ�룻����mExtraData Ϊ��ָ�롣
    // ��Ȼ����һ�� _DefClass ��Ķ���ṹͼ���Ѿ�����ͨ����غ�����ȡ�� _DefClass ��ȫ�����ݣ�������ǳ��������
    // ���� _DefClass �еĲ���ָ�����ָ���������Ȼ��Ҫ��һ��������Ӧ���͵Ķ���ṹͼ���еݹ��ȡ���������������
    // ����ͨ�����Ƕ�׵Ķ���ṹͼ����ԭ�����в㼶��ϵ�ĸ����ͱ���ָ�롰չ������ֱ����ǰ�������������Ѿ������ڿ��ԡ�չ������ָ��Ϊֹ���ݹ��ȡ������
};

// ====================================================================================================
// �� ������ṹͼ��
// ----------------------------------------------------------------------------------------------------
// ����ṹͼ������һ�ֶ��������ࣨ_DefClass���ж������ݵĴ洢��ʽ�Ͷ�ȡ��ʽ�������ڡ�_DefDefClass����
// ====================================================================================================
class DefMap
{
public:
    DefField*           mMapFields;                     //+0x0���ṹ�ֶε����飬��¼ _DefClass ���еĸ���Ա������ _DefClass �еĽṹ��ÿ���¼һ�ֽṹ��
    int                 mDefSize;                       //+0x4��һ�� _DefClass ʵ����ռ�õ��ڴ��С��Ҳ���������ζ�ȡʱ�Ķ�ȡ���ȣ�һ��Ϊ sizeof(_DefClass)
    void*               (*mConstructorFunc)(void*);     //+0x8��_DefClass ����ʵ���Ĺ��캯����ָ��
};

void* __cdecl           TodParticleDefinitionConstructor(void* thePointer); //0x5155A0
void* __cdecl           TodEmitterDefinitionConstructor(void* thePointer);  //0x5155C0
void* __cdecl           ParticleFieldConstructor(void* thePointer);         //0x515620
void* __cdecl           TrailDefinitionConstructor(void* thePointer);       //0x51B7F0
void* __cdecl           ReanimatorTransformConstructor(void* thePointer);   //0x471570
void* __cdecl           ReanimatorTrackConstructor(void* thePointer);       //0x4715B0
void* __cdecl           ReanimatorDefinitionConstructor(void* thePointer);  //0x4715D0

//extern DefField gParticleFieldDefFields[];  //0x69E2F8
extern DefMap gParticleFieldDefMap;  //0x69E338
//
//extern DefField gEmitterDefFields[];  //0x69E350
extern DefMap gEmitterDefMap;  //0x69E344
//
//extern DefField gParticleDefFields[];  //0x69E670
extern DefMap gParticleDefMap;  //0x69E690
//
extern DefMap gTrailDefMap;  //0x69D98C
//
//extern DefField gReanimatorTransformDefFields[];  //0x69F088
extern DefMap gReanimatorTransformDefMap;  //0x69F07C
//
//extern DefField gReanimatorTrackDefFields[];  //0x69F148
extern DefMap gReanimatorTrackDefMap;  //0x69F178
//
//extern DefField gReanimatorDefFields[];  //0x69F184
extern DefMap gReanimatorDefMap;  //0x69F1B4

// ====================================================================================================
// �� ���������顿
// ----------------------------------------------------------------------------------------------------
// һ�����������Ӧһ��ָ�����ͱ���ָ������͡�
// ====================================================================================================
class DefinitionArrayDef
{
public:
    void*               mArrayData;                     //+0x0�������ɸ��ض������������͵�ʵ�����ɵ����飬���綯�������еġ����������
    int                 mArrayCount;                    //+0x4������Ĵ�С�����綯�������еġ����������������ϵͳ�����еġ�������������
    // �����������е�һ�������飨ָ�룩 + ����������ϣ��ڶ�ȡʱ���� DefField ����һ�� DefinitionArrayDef �ṹ
    // ���� TodParticleDefinition �µ� *mEmitterDefs �� mEmitterDefCount���Լ� TodEmitterDefinition �µ� *mParticleFields �� mParticleFieldCount �ȡ�
    // �ڶ�ȡʱ����Ϊ mArrayCount ��һ�������������ڳ��ζ�ȡʱ�ͱ���ȷ��ȡ����Ϊ���������ͣ�������Ҳ���ں��� mArrayData ���޸������г�ΪУ��ο�
};

// ====================================================================================================
// �� ��ѹ����������ͷ��
// ----------------------------------------------------------------------------------------------------
// ��ѹ������ǰ���һ��ѹ����������ͷ����¼У�黺��ֵ��ԭʼ���ݳ��ȣ������ڽ�ѹʱ������������ԡ�
// ====================================================================================================
class CompressedDefinitionHeader
{
public:
    unsigned int        mCookie;                        //+0x0������ѹ��У��Ļ���ֵ
    unsigned long       mUncompressedSize;              //+0x4��δѹ�����ݵĳ���
};

// ====================================================================================================
// �� ������·����
// ----------------------------------------------------------------------------------------------------
// ����·����һ����ͼǰ׺���ǰ׺����ͼ��ŵ��ļ���·��֮�佨��������
// ====================================================================================================
class DefLoadResPath
{
public:
    const char*         mPrefix;                        //+0x0����ͼ��ǰ׺���硰IMAGE_"
    const char*         mDirectory;                     //+0x4��ǰ׺��Ӧ����ͼ�����ļ��У��硰images\��
};

SexyString /*__cdecl*/  DefinitionGetCompiledFilePathFromXMLFilePath(const SexyString& theXMLFilePath);
bool                    IsFileInPakFile(const SexyString& theFilePath);
bool                    DefinitionIsCompiled(const SexyString& theXMLFilePath);
bool                    DefinitionReadCompiledFile(const SexyString& theCompiledFilePath, DefMap* theDefMap, void* theDefinition);
void                    DefinitionFillWithDefaults(DefMap* theDefMap, void* theDefinition);
void                    DefinitionXmlError(XMLParser* theXmlParser, char* theFormat, ...);
bool                    DefSymbolValueFromString(DefSymbol* theSymbolMap, const char* theName, int* theResultValue);
bool                    DefinitionReadXMLString(XMLParser* theXmlParser, SexyString& theValue);
bool                    DefinitionReadIntField(XMLParser* theXmlParser, int* theValue);
bool                    DefinitionReadFloatField(XMLParser* theXmlParser, float* theValue);
bool                    DefinitionReadStringField(XMLParser* theXmlParser, const char** theValue);
bool                    DefinitionReadEnumField(XMLParser* theXmlParser, int* theValue, DefSymbol* theSymbolMap);
bool                    DefinitionReadVector2Field(XMLParser* theXmlParser, SexyVector2* theValue);
bool                    DefinitionReadArrayField(XMLParser* theXmlParser, DefinitionArrayDef* theArray, DefField* theField);
bool                    DefinitionReadFloatTrackField(XMLParser* theXmlParser, FloatParameterTrack* theTrack);
bool                    DefinitionReadFlagField(XMLParser* theXmlParser, const SexyString& theElementName, uint* theResultValue, DefSymbol* theSymbolMap);
bool                    DefinitionReadImageField(XMLParser* theXmlParser, Image** theImage);
bool                    DefinitionReadFontField(XMLParser* theXmlParser, Font** theFont);
bool                    DefinitionReadField(XMLParser* theXmlParser, DefMap* theDefMap, void* theDefinition, bool* theDone);
bool                    DefinitionWriteCompiledFile(const SexyString& theCompiledFilePath, DefMap* theDefMap, void* theDefinition);
bool                    DefinitionCompileFile(const SexyString theXMLFilePath, const SexyString& theCompiledFilePath, DefMap* theDefMap, void* theDefinition);
/*inline*/ void*        DefinitionAlloc(int theSize);
void*                   DefinitionUncompressCompiledBuffer(void* theCompressedBuffer, size_t theCompressedBufferSize, size_t& theUncompressedSize, const SexyString& theCompiledFilePath);
uint /*__cdecl*/        DefinitionCalcHashSymbolMap(int aSchemaHash, DefSymbol* theSymbolMap);
uint /*__cdecl*/        DefinitionCalcHashDefMap(int aSchemaHash, DefMap* theDefMap, TodList<DefMap*>& theProgressMaps);
uint /*__cdecl*/        DefinitionCalcHash(DefMap* theDefMap);
bool                    DefReadFromCacheString(void*& theReadPtr, char** theString);
bool                    DefReadFromCacheArray(void*& theReadPtr, DefinitionArrayDef* theArray, DefMap* theDefMap);
bool                    DefReadFromCacheImage(void*& theReadPtr, Image** theImage);
bool                    DefReadFromCacheFont(void*& theReadPtr, Font** theFont);
bool                    DefReadFromCacheFloatTrack(void*& theReadPtr, FloatParameterTrack* theTrack);
bool                    DefMapReadFromCache(void*& theReadPtr, DefMap* theDefMap, void* theDefinition);
bool                    DefinitionCompileAndLoad(const SexyString& theXMLFilePath, DefMap* theDefMap, void* theDefinition);
bool                    DefinitionLoadMap(XMLParser* theXmlParser, DefMap* theDefMap, void* theDefinition);
bool                    DefinitionLoadImage(Image** theImage, const SexyString& theName);
bool                    DefinitionLoadFont(Font** theFont, const SexyString& theName);
bool                    DefinitionLoadXML(const SexyString& theFilename, DefMap* theDefMap, void* theDefinition);
void                    DefinitionFreeArrayField(DefinitionArrayDef* theArray, DefMap* theDefMap);
void                    DefinitionFreeMap(DefMap* theDefMap, void* theDefinition);

/*inline*/ bool         FloatTrackIsSet(const FloatParameterTrack& theTrack);
/*inline*/ void         FloatTrackSetDefault(FloatParameterTrack& theTrack, float theValue);
float                   FloatTrackEvaluate(FloatParameterTrack& theTrack, float theTimeValue, float theInterp);
float                   FloatTrackEvaluateFromLastTime(FloatParameterTrack& theTrack, float theTimeValue, float theInterp);
/*inline*/ bool         FloatTrackIsConstantZero(FloatParameterTrack& theTrack);

#endif