#include "Trail.h"
#include <assert.h>
#include "TodDebug.h"
#include "Definition.h"
#include "../ImageLib/zlib/zlib.h"
#include "../PakLib/PakInterface.h"
#include "../SexyAppFramework/PerfTimer.h"
#include "../SexyAppFramework/XMLParser.h"

DefSymbol gTrailFlagDefSymbols[] = {  //0x69E150
    { 0, "Loops" },                 { -1, nullptr }
};
DefField gTrailDefFields[] = {  //0x69E160
    { "Image",              0x0,        DefFieldType::DT_IMAGE,         nullptr },
    { "MaxPoints",          0x4,        DefFieldType::DT_INT,           nullptr },
    { "MinPointDistance",   0x8,        DefFieldType::DT_FLOAT,         nullptr },
    { "TrailFlags",         0xC,        DefFieldType::DT_FLAGS,         gTrailFlagDefSymbols },
    { "WidthOverLength",    0x18,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "WidthOverTime",      0x20,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "AlphaOverLength",    0x28,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "AlphaOverTime",      0x30,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "TrailDuration",      0x10,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "",                   0x0,        DefFieldType::DT_INVALID,       nullptr }
};
DefMap gTrailDefMap = { gTrailDefFields, sizeof(TrailDefinition), TrailDefinitionConstructor };  //0x69D98C

DefSymbol gParticleFlagSymbols[] = {  //0x69E290
    {  0, "RandomLaunchSpin" },     {  1, "AlignLaunchSpin" },  {  2, "AlignToPixel" },     {  4, "ParticleLoops" },    {  3, "SystemLoops" },
    {  5, "ParticlesDontFollow" },  {  6, "RandomStartTime" },  {  7, "DieIfOverloaded" },  {  8, "Additive" },         {  9, "FullScreen" },
    { 10, "SoftwareOnly" },         { 11, "HardwareOnly" },     { -1, nullptr }
};
DefSymbol gEmitterTypeSymbols[] = {  //0x69E260
    {  0, "Circle" },               {  1, "Box" },              {  2, "BoxPath" },          {  3, "CirclePath" },       {  4, "CircleEvenSpacing" },
    { -1, nullptr }
};
DefSymbol gParticleTypeSymbols[] = {  //0x69E200
    {  1, "Friction" },             {  2, "Acceleration" },     {  3, "Attractor" },        {  4, "MaxVelocity" },      {  5, "Velocity" },
    {  6, "Position" },             {  7, "SystemPosition" },   {  8, "GroundConstraint" }, {  9, "Shake" },            { 10, "Circle" },
    { 11, "Away" },                 { -1, nullptr }
};

DefField gParticleFieldDefFields[] = {  //0x69E2F8
    { "FieldType",          0x0,        DefFieldType::DT_ENUM,          gParticleTypeSymbols },
    { "x",                  0x4,        DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "y",                  0xC,        DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "",                   0x0,        DefFieldType::DT_INVALID,       nullptr },
};
DefMap gParticleFieldDefMap = { gParticleFieldDefFields, sizeof(ParticleField), ParticleFieldConstructor };  //0x69E338

DefField gEmitterDefFields[] = {  //0x69E350
    { "Image",              0x0,        DefFieldType::DT_IMAGE,         nullptr },
    { "ImageRow",           0x8,        DefFieldType::DT_INT,           nullptr },
    { "ImageCol",           0x4,        DefFieldType::DT_INT,           nullptr },
    { "ImageFrames",        0xC,        DefFieldType::DT_INT,           nullptr },
    { "Animated",           0x10,       DefFieldType::DT_INT,           nullptr },
    { "ParticleFlags",      0x14,       DefFieldType::DT_FLAGS,         gParticleFlagSymbols },
    { "EmitterType",        0x18,       DefFieldType::DT_ENUM,          gEmitterTypeSymbols },
    { "Name",               0x1C,       DefFieldType::DT_STRING,        nullptr },
    { "SystemDuration",     0x24,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "OnDuration",         0x20,       DefFieldType::DT_STRING,        nullptr },
    { "CrossFadeDuration",  0x2C,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "SpawnRate",          0x34,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "SpawnMinActive",     0x3C,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "SpawnMaxActive",     0x44,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "SpawnMaxLaunched",   0x4C,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "EmitterRadius",      0x54,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "EmitterOffsetX",     0x5C,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "EmitterOffsetY",     0x64,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "EmitterBoxX",        0x6C,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "EmitterBoxY",        0x74,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "EmitterPath",        0x8C,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "EmitterSkewX",       0x7C,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "EmitterSkewY",       0x84,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ParticleDuration",   0x94,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "SystemRed",          0xAC,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "SystemGreen",        0xB4,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "SystemBlue",         0xBC,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "SystemAlpha",        0xC4,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "SystemBrightness",   0xCC,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "LaunchSpeed",        0x9C,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "LaunchAngle",        0xA4,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "Field",              0xD4,       DefFieldType::DT_ARRAY,         &gParticleFieldDefMap },
    { "SystemField",        0xDC,       DefFieldType::DT_ARRAY,         &gParticleFieldDefMap },
    { "ParticleRed",        0xE4,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ParticleGreen",      0xEC,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ParticleBlue",       0xF4,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ParticleAlpha",      0xFC,       DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ParticleBrightness", 0x104,      DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ParticleSpinAngle",  0x10C,      DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ParticleSpinSpeed",  0x114,      DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ParticleScale",      0x11C,      DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ParticleStretch",    0x124,      DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "CollisionReflect",   0x12C,      DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "CollisionSpin",      0x134,      DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ClipTop",            0x13C,      DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ClipBottom",         0x144,      DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ClipLeft",           0x14C,      DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "ClipRight",          0x154,      DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "AnimationRate",      0x15C,      DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "",                   0x0,        DefFieldType::DT_INVALID,       nullptr },
};
DefMap gEmitterDefMap = { gEmitterDefFields, sizeof(TodEmitterDefinition), TodEmitterDefinitionConstructor };  //0x69E344

DefField gParticleDefFields[] = {  //0x69E670
    { "Emitter",            0x0,        DefFieldType::DT_ARRAY,         &gEmitterDefMap },
    { "",                   0x0,        DefFieldType::DT_INVALID,       nullptr }
};
DefMap gParticleDefMap = { gParticleDefFields, sizeof(TodParticleDefinition), TodParticleDefinitionConstructor };  //0x69E690

DefField gReanimatorTransformDefFields[] = {  //0x69F088
    { "x",                  0x0,        DefFieldType::DT_FLOAT,         nullptr },
    { "y",                  0x4,        DefFieldType::DT_FLOAT,         nullptr },
    { "kx",                 0x8,        DefFieldType::DT_FLOAT,         nullptr },
    { "kx",                 0xC,        DefFieldType::DT_FLOAT,         nullptr },
    { "sx",                 0x10,       DefFieldType::DT_FLOAT,         nullptr },
    { "sx",                 0x14,       DefFieldType::DT_FLOAT,         nullptr },
    { "f",                  0x18,       DefFieldType::DT_FLOAT,         nullptr },
    { "a",                  0x1C,       DefFieldType::DT_FLOAT,         nullptr },
    { "i",                  0x20,       DefFieldType::DT_IMAGE,         nullptr },
    { "font",               0x24,       DefFieldType::DT_FONT,          nullptr },
    { "text",               0x28,       DefFieldType::DT_STRING,        nullptr },
    { "",                   0x0,        DefFieldType::DT_INVALID,       nullptr }
};
DefMap gReanimatorTransformDefMap = { gReanimatorTransformDefFields, sizeof(ReanimatorTransform), };  //0x69F07C

DefField gReanimatorTrackDefFields[] = {  //0x69F148
    { "name",               0x0,        DefFieldType::DT_STRING,        nullptr },
    { "t",                  0x4,        DefFieldType::DT_ARRAY,         &gReanimatorTransformDefMap },
    { "",                   0x0,        DefFieldType::DT_INVALID,       nullptr }
};
DefMap gReanimatorTrackDefMap = { gReanimatorTrackDefFields, sizeof(ReanimatorTrack), ReanimatorTrackConstructor };  //0x69F178

DefField gReanimatorDefFields[] = {
    { "track",              0x0,        DefFieldType::DT_ARRAY,         &gReanimatorTrackDefMap },
    { "fps",                0x8,        DefFieldType::DT_FLOAT,         nullptr },
    { "",                   0x0,        DefFieldType::DT_INVALID,       nullptr }
};  //0x69F184
DefMap gReanimatorDefMap = { gReanimatorDefFields, sizeof(ReanimatorDefinition), ReanimatorDefinitionConstructor };  //0x69F1B4

static DefLoadResPath gDefLoadResPaths[4] = { {"IMAGE_", ""}, {"IMAGE_", "particles\\"}, {"IMAGE_REANIM_", "reanim\\"}, {"IMAGE_REANIM_", "images\\"} };  //0x6A1A48

//0x5155A0
void* ParticleFieldConstructor(void* thePointer)
{
    if (thePointer)
    {
        ((ParticleField*)thePointer)->mX.mNodes = nullptr;
        ((ParticleField*)thePointer)->mX.mCountNodes = 0;
        ((ParticleField*)thePointer)->mY.mNodes = nullptr;
        ((ParticleField*)thePointer)->mY.mCountNodes = 0;
        ((ParticleField*)thePointer)->mFieldType = ParticleFieldType::FIELD_INVALID;
    }
    return thePointer;
}

//0x5155C0
void* TodEmitterDefinitionConstructor(void* thePointer)
{
    if (thePointer)
    {
        memset(thePointer, NULL, sizeof(TodEmitterDefinition));
        ((TodEmitterDefinition*)thePointer)->mImageFrames = 1;
        ((TodEmitterDefinition*)thePointer)->mEmitterType = EmitterType::EMITTER_BOX;
        ((TodEmitterDefinition*)thePointer)->mName = "";
        ((TodEmitterDefinition*)thePointer)->mOnDuration = "";
        ((TodEmitterDefinition*)thePointer)->mImageRow = 0;
        ((TodEmitterDefinition*)thePointer)->mImageCol = 0;
        ((TodEmitterDefinition*)thePointer)->mAnimated = 0;
        ((TodEmitterDefinition*)thePointer)->mImage = nullptr;
    }
    return thePointer;
}

//0x515620
void* TodParticleDefinitionConstructor(void* thePointer)
{
    if (thePointer)
    {
        ((TodParticleDefinition*)thePointer)->mEmitterDefs = nullptr;
        ((TodParticleDefinition*)thePointer)->mEmitterDefCount = 0;
    }
    return thePointer;
}

void* TrailDefinitionConstructor(void* thePointer)
{
    if (thePointer)
    {
        memset(thePointer, 0, sizeof(TrailDefinition));
        ((TrailDefinition*)thePointer)->mMinPointDistance = 1.0f;
        ((TrailDefinition*)thePointer)->mMaxPoints = 2;
        ((TrailDefinition*)thePointer)->mTrailFlags = 0U;
        ((TrailDefinition*)thePointer)->mImage = nullptr;
    }
    return thePointer;
}

//0x471570
void* ReanimatorTransformConstructor(void* thePointer)
{
    if (thePointer)
    {
        ((ReanimatorTransform*)thePointer)->mTransX = DEFAULT_FIELD_PLACEHOLDER;
        ((ReanimatorTransform*)thePointer)->mTransY = DEFAULT_FIELD_PLACEHOLDER;
        ((ReanimatorTransform*)thePointer)->mSkewX = DEFAULT_FIELD_PLACEHOLDER;
        ((ReanimatorTransform*)thePointer)->mSkewY = DEFAULT_FIELD_PLACEHOLDER;
        ((ReanimatorTransform*)thePointer)->mScaleX = DEFAULT_FIELD_PLACEHOLDER;
        ((ReanimatorTransform*)thePointer)->mScaleY = DEFAULT_FIELD_PLACEHOLDER;
        ((ReanimatorTransform*)thePointer)->mFrame = DEFAULT_FIELD_PLACEHOLDER;
        ((ReanimatorTransform*)thePointer)->mAlpha = DEFAULT_FIELD_PLACEHOLDER;
        ((ReanimatorTransform*)thePointer)->mImage = nullptr;
        ((ReanimatorTransform*)thePointer)->mFont = nullptr;
        ((ReanimatorTransform*)thePointer)->mText = "";
    }
    return thePointer;
}

//0x4715B0
void* ReanimatorTrackConstructor(void* thePointer)
{
    if (thePointer)
    {
        ((ReanimatorTrack*)thePointer)->mName = "";
        ((ReanimatorTrack*)thePointer)->mTransforms = nullptr;
        ((ReanimatorTrack*)thePointer)->mTransformCount = 0;
    }
    return thePointer;
}

//0x4715D0
void* ReanimatorDefinitionConstructor(void* thePointer)
{
    if (thePointer)
    {
        ((ReanimatorDefinition*)thePointer)->mTracks = nullptr;
        ((ReanimatorDefinition*)thePointer)->mTrackCount = 0;
        ((ReanimatorDefinition*)thePointer)->mFPS = 12.0f;
        ((ReanimatorDefinition*)thePointer)->mReanimAtlas = nullptr;
    }
    return thePointer;
}

void* DefinitionAlloc(int theSize)
{
    void* aPtr = operator new[](theSize);
    TOD_ASSERT(aPtr);
    memset(aPtr, 0, theSize);
    return aPtr;
}

//0x443BE0
bool DefinitionLoadImage(Image** theImage, const SexyString& theName)
{
    // 当贴图文件路径不存在时，无须获取贴图
    if (theName.size() == 0)
    {
        *theImage = nullptr;
        return true;
    }

    // 尝试借助资源管理器，从 XML 中加载贴图
    Image* anImage = (Image*)gSexyAppBase->mResourceManager->LoadImage(theName);
    if (anImage)
    {
        *theImage = anImage;
        return true;
    }

    // 从可能的贴图路径中手动加载贴图
    for (const DefLoadResPath& aLoadResPath : gDefLoadResPaths)
    {
        int aNameLen = theName.size();
        int aPrefixLen = strlen(aLoadResPath.mPrefix);
        if (aPrefixLen < aNameLen)
        {
            SexyString aPathToTry = aLoadResPath.mDirectory + theName.substr(aPrefixLen, aNameLen);
            SharedImageRef aImageRef = gSexyAppBase->GetSharedImage(aPathToTry);
            if ((Image*)aImageRef != nullptr)
            {
                TodHesitationTrace("Load Image '%s'", theName.c_str());
                TodAddImageToMap(&aImageRef, theName);
                TodMarkImageForSanding((Image*)aImageRef);
                *theImage = (Image*)aImageRef;
                return true;
            }
        }
    }
    return false;
}

//0x443F60
bool DefinitionLoadFont(_Font** theFont, const SexyString& theName)
{
    _Font* aFont = gSexyAppBase->mResourceManager->LoadFont(SexyStringToString(theName));
    *theFont = aFont;
    return aFont != nullptr;
}

bool DefinitionLoadXML(const SexyString& theFileName, DefMap* theDefMap, void* theDefinition)
{
    return DefinitionCompileAndLoad(theFileName, theDefMap, theDefinition);
}

//0x444020
bool DefReadFromCacheArray(void*& theReadPtr, DefinitionArrayDef* theArray, DefMap* theDefMap)
{
    int aDefSize;
    SMemR(theReadPtr, &aDefSize, sizeof(int));  // 先读取一个整数表示 theDefMap 描述的定义结构类的大小
    if (aDefSize != theDefMap->mDefSize)  // 比较其与当前给出的定义结构图声明的大小是否一致
    {
        TodTrace("cache has old def: array size");
        return false;
    }
    if (theArray->mArrayCount == 0)  // 如果类中没有实例，则无需读取
        return true;

    int aArraySize = aDefSize * theArray->mArrayCount;
    void* pData = DefinitionAlloc(aArraySize);  // 申请内存并初始化填充为 0
    theArray->mArrayData = pData;
    SMemR(theReadPtr, pData, aArraySize);  // 仍然是粗略读取全部数据，然后再根据 theDefMap 的结构字段数组修复指针
    for (int i = 0; i < theArray->mArrayCount; i++)
        if (!DefMapReadFromCache(theReadPtr, theDefMap, (void*)((intptr_t)pData + theDefMap->mDefSize * i)))  // 最后一个参数表示 pData[i]
            return false;
    return true;
}

//0x4440B0
bool DefReadFromCacheFloatTrack(void*& theReadPtr, FloatParameterTrack* theTrack)
{
    int& aCountNodes = theTrack->mCountNodes;
    SMemR(theReadPtr, &aCountNodes, sizeof(int));
    if (aCountNodes > 0)
    {
        int aSize = aCountNodes * sizeof(FloatParameterTrackNode);
        FloatParameterTrackNode* aPtr = (FloatParameterTrackNode*)DefinitionAlloc(aSize);
        theTrack->mNodes = aPtr;
        SMemR(theReadPtr, aPtr, aSize);
    }
    return true;
}

//0x444110
bool DefReadFromCacheString(void*& theReadPtr, char** theString)
{
    int aLen;
    SMemR(theReadPtr, &aLen, sizeof(int));
    TOD_ASSERT(aLen >= 0 && aLen <= 100000);
    if (aLen == 0)
        *theString = (char*)"";
    else
    {
        char* aPtr = (char*)DefinitionAlloc(aLen + 1);
        *theString = aPtr;
        SMemR(theReadPtr, aPtr, aLen);
        aPtr[aLen] = '\0';
    }
    return true;
}

//0x444180
bool DefReadFromCacheImage(void*& theReadPtr, Image** theImage)
{
    int aLen;
    SMemR(theReadPtr, &aLen, sizeof(int));  // 读取贴图标签字符数组的长度
    char* aImageName = (char*)_alloca(aLen + 1);  // 在栈上分配贴图标签字符数组的内存空间
    SMemR(theReadPtr, aImageName, aLen);  // 读取贴图标签字符数组
    aImageName[aLen] = '\0';

    *theImage = nullptr;
    return aImageName[0] == '\0' || DefinitionLoadImage(theImage, aImageName);
}

//0x444220
bool DefReadFromCacheFont(void*& theReadPtr, _Font** theFont)
{
    int aLen;
    SMemR(theReadPtr, &aLen, sizeof(int));  // 读取字体标签字符数组的长度
    char* aFontName = (char*)_alloca(aLen + 1);  // 在栈上分配字体标签字符数组的内存空间
    SMemR(theReadPtr, aFontName, aLen);  // 读取字体标签字符数组
    aFontName[aLen] = '\0';

    *theFont = nullptr;
    return aFontName[0] == '\0' || DefinitionLoadFont(theFont, aFontName);
}

//0x4442C0
bool DefMapReadFromCache(void*& theReadPtr, DefMap* theDefMap, void* theDefinition)
{
    // 分别确认每一个成员变量，并修复其中的指针类型和标志类型的变量
    for (DefField* aField = theDefMap->mMapFields; *aField->mFieldName != '\0'; aField++)
    {
        bool aSucceed = true;
        void* aDest = (void*)((intptr_t)theDefinition + aField->mFieldOffset);  // 指向该成员变量的指针
        switch (aField->mFieldType)
        {
        case DefFieldType::DT_STRING:
            aSucceed = DefReadFromCacheString(theReadPtr, (char**)aDest);
            break;
        case DefFieldType::DT_ARRAY:
            aSucceed = DefReadFromCacheArray(theReadPtr, (DefinitionArrayDef*)aDest, (DefMap*)aField->mExtraData);
            break;
        case DefFieldType::DT_IMAGE:
            aSucceed = DefReadFromCacheImage(theReadPtr, (Image**)aDest);
            break;
        case DefFieldType::DT_FONT:
            aSucceed = DefReadFromCacheFont(theReadPtr, (_Font**)aDest);
            break;
        case DefFieldType::DT_TRACK_FLOAT:
            aSucceed = DefReadFromCacheFloatTrack(theReadPtr, (FloatParameterTrack*)aDest);
            break;
        default:
            break;
        }

        if (!aSucceed)
            return false;
    }
    return true;
}

//0x444380
uint DefinitionCalcHashSymbolMap(int aSchemaHash, DefSymbol* theSymbolMap)
{
    while (theSymbolMap->mSymbolName != nullptr)
    {
        aSchemaHash = crc32(aSchemaHash, (const Bytef*)theSymbolMap->mSymbolName, strlen(theSymbolMap->mSymbolName));
        aSchemaHash = crc32(aSchemaHash, (const Bytef*)&theSymbolMap->mSymbolValue, sizeof(int));
        theSymbolMap++;
    }
    return aSchemaHash;
}

//0x4443D0
uint DefinitionCalcHashDefMap(int aSchemaHash, DefMap* theDefMap, TodList<DefMap*>& theProgressMaps)
{
    for (TodListNode<DefMap*>* aNode = theProgressMaps.mHead; aNode != nullptr; aNode = aNode->mNext)
        if (aNode->mValue == theDefMap)
            return aSchemaHash;
    theProgressMaps.AddTail(theDefMap);

    aSchemaHash = crc32(aSchemaHash, (Bytef*)&theDefMap->mDefSize, sizeof(int));
    for (DefField* aField = theDefMap->mMapFields; *aField->mFieldName != '\0'; aField++)
    {
        aSchemaHash = crc32(aSchemaHash, (Bytef*)&aField->mFieldType, sizeof(DefFieldType));
        aSchemaHash = crc32(aSchemaHash, (Bytef*)&aField->mFieldOffset, sizeof(int));
        switch (aField->mFieldType)
        {
        case DefFieldType::DT_ENUM:
        case DefFieldType::DT_FLAGS:
            aSchemaHash = DefinitionCalcHashSymbolMap(aSchemaHash, (DefSymbol*)aField->mExtraData);
            break;
        case DefFieldType::DT_ARRAY:
            aSchemaHash = DefinitionCalcHashDefMap(aSchemaHash, (DefMap*)aField->mExtraData, theProgressMaps);
            break;
        default:
            break;
        }
    }
    return aSchemaHash;
}

//0x444490
uint DefinitionCalcHash(DefMap* theDefMap)
{
    TodList<DefMap*> aProgressMaps;
    uint aResult = DefinitionCalcHashDefMap(crc32(0L, (Bytef*)Z_NULL, NULL) + 1, theDefMap, aProgressMaps);
    aProgressMaps.RemoveAll();
    return aResult;
}

//0x444500 : UnCompress(&theUncompressedSize, theCompressedBufferSize, esi = *theCompressedBuffer)
void* DefinitionUncompressCompiledBuffer(void* theCompressedBuffer, size_t theCompressedBufferSize, size_t& theUncompressedSize, const SexyString& theCompiledFilePath)
{
    // theCompressedBuffer 的前两个四字节存有特殊数据，此处检测其长度是否足够 8 字节（即 2 个四字节）
    if (theCompressedBufferSize < 8)
    {
        TodTrace("Compile def too small", theCompiledFilePath.c_str());
        return nullptr;
    }
    // 将 theCompressedBuffer 的前两个四字节视为一个 CompressedDefinitionHeader
    CompressedDefinitionHeader* aHeader = (CompressedDefinitionHeader*)theCompressedBuffer;
    if (aHeader->mCookie != 0xDEADFED4L)
    {
        TodTrace("Compiled fire cookie wrong: %s\n", theCompiledFilePath.c_str());
        return nullptr;
    }
    
    Bytef* aUncompressedBuffer = (Bytef*)DefinitionAlloc(aHeader->mUncompressedSize);
    Bytef* aSrc = (Bytef*)((intptr_t)theCompressedBuffer + sizeof(CompressedDefinitionHeader));  // 实际解压数据从第 3 个四字节开始
    ulong aUncompressedSizeResult;  // 用作出参的未压缩数据实际长度
    int aResult = uncompress(aUncompressedBuffer, &aUncompressedSizeResult, aSrc, theCompressedBufferSize - sizeof(CompressedDefinitionHeader));
    (void)aResult; // Compiler can't work out that this is used in the Debug build
    TOD_ASSERT(aResult == Z_OK);
    TOD_ASSERT(aUncompressedSizeResult == aHeader->mUncompressedSize);
    theUncompressedSize = aHeader->mUncompressedSize;
    return aUncompressedBuffer;
}

//0x444560 : (void* def, *defMap, eax = string& compiledFilePath)  //esp -= 8
bool DefinitionReadCompiledFile(const SexyString& theCompiledFilePath, DefMap* theDefMap, void* theDefinition)
{
    PerfTimer aTimer;
    aTimer.Start();
    PFILE* pFile = p_fopen(theCompiledFilePath.c_str(), _S("rb"));
    if (pFile)
    {
        p_fseek(pFile, 0, 2);  // 将读取位置的指针移动至文件末尾
        size_t aCompressedSize = p_ftell(pFile);  // 此时获取到的偏移量即为整个文件的大小
        p_fseek(pFile, 0, 0);  // 再把读取位置的指针移回文件开头
        void* aCompressedBuffer = DefinitionAlloc(aCompressedSize);
        // 读取文件，并判断实际读取的大小是否为完整的文件大小，若不等则判断为读取失败
        bool aReadCompressedFailed = p_fread(aCompressedBuffer, sizeof(char), aCompressedSize, pFile) != aCompressedSize;
        p_fclose(pFile);  // 关闭资源文件流并释放 pFile 占用的内存
        if (aReadCompressedFailed)  // 判断是否读取成功
        {
            TodTrace(_S("Failed to read compiled file: %s\n"), theCompiledFilePath.c_str());
            free(aCompressedBuffer);
        }
        else
        {
            size_t aUncompressedSize;
            void* aUncompressedBuffer = DefinitionUncompressCompiledBuffer(aCompressedBuffer, aCompressedSize, aUncompressedSize, theCompiledFilePath);
            delete[] (char *)aCompressedBuffer;
            if (aUncompressedBuffer)
            {
                uint aDefHash = DefinitionCalcHash(theDefMap);  // 计算 CRC 校验值，后将用于检测数据的完整性
                if (aUncompressedSize < theDefMap->mDefSize + sizeof(uint))  // 检测解压数据的长度是否足够“定义数据 + 一个校验值记录数据”的长度
                    TodTrace(_S("Compiled file size too small: %s\n"), theCompiledFilePath.c_str());
                else
                {
                    // 复制一份解压数据的指针用于读取时移动，原指针后续要用于计算读取区域大小及 delete[] 操作
                    void* aBufferPtr = aUncompressedBuffer;
                    uint aCashHash;
                    SMemR(aBufferPtr, &aCashHash, sizeof(uint));  // 读取记录的 CRC 校验值
                    if (aCashHash != aDefHash)  // 判断校验值是否一致，若不一致则说明数据发生错误
                        TodTrace(_S("Compiled file schema wrong: %s\n"), theCompiledFilePath.c_str());
                    else
                    {
                        // ☆ 正式开始读取定义数据 ☆
                        // 初次粗略读取 theDefinition 原类型的定义数据，囫囵吞枣地将所有记录的数据全部读入到 theDefinition 中
                        // 此时 theDefinition 原本的非指针类型的数据将全部被正确读取，但其指针类型的变量会被读取并赋值为野指针
                        // 这些野指针的问题后续将会在 DefMapReadFromCache() 中借助相应 DefField 的 mExtraData 进行修复
                        SMemR(aBufferPtr, theDefinition, theDefMap->mDefSize);
                        // 修复野指针及标志型数据，并保存是否成功的结果，后续作为返回值
                        bool aResult = DefMapReadFromCache(aBufferPtr, theDefMap, theDefinition);
                        size_t aReadMemSize = (uintptr_t)aBufferPtr - (uintptr_t)aUncompressedBuffer;
                        delete[] (char *)aUncompressedBuffer;
                        if (aResult && aReadMemSize != aUncompressedSize)
                            TodTrace(_S("Compiled file wrong size: %s\n"), theCompiledFilePath.c_str());
                        return aResult;
                    }
                }
            }
            delete[] (char *)aUncompressedBuffer;
        }
    }
    return false;
}

//0x444770
SexyString DefinitionGetCompiledFilePathFromXMLFilePath(const SexyString& theXMLFilePath)
{
    return _S("compiled\\") + theXMLFilePath + _S(".compiled");
}

bool IsFileInPakFile(const SexyString& theFilePath)
{
    PFILE* pFile = p_fopen(theFilePath.c_str(), _S("rb"));
    bool aIsInPak = pFile && !pFile->mFP;  // 通过 mPakRecordMap.find 找到并打开的文件，其 mFP 为空指针（因为不是从实际文件中打开的）
    if (pFile)
    {
        p_fclose(pFile);
    }
    return aIsInPak;
}

bool DefinitionIsCompiled(const SexyString& theXMLFilePath)
{
    SexyString aCompiledFilePath = DefinitionGetCompiledFilePathFromXMLFilePath(theXMLFilePath);
    if (IsFileInPakFile(aCompiledFilePath))
        return true;

    _WIN32_FILE_ATTRIBUTE_DATA lpFileData;
    _FILETIME aCompiledFileTime;
    bool aSucceed = GetFileAttributesEx(aCompiledFilePath.c_str(), _GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &lpFileData);
    if (aSucceed)
        aCompiledFileTime = lpFileData.ftLastWriteTime;
    
    if (!GetFileAttributesEx(theXMLFilePath.c_str(), _GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &lpFileData))
    {
        TodTrace(_S("Can't file source file to compile '%s'"), theXMLFilePath.c_str());
        return false;
    }
    else
        return aSucceed && CompareFileTime(&aCompiledFileTime, &lpFileData.ftLastWriteTime) == 1;
}

void DefinitionFillWithDefaults(DefMap* theDefMap, void* theDefinition)
{
    memset(theDefinition, NULL, theDefMap->mDefSize);  // 将 theDefinition 初始化填充为 0
    for (DefField* aField = theDefMap->mMapFields; *aField->mFieldName != '\0'; aField++)  // 遍历 theDefinition 的每一个成员变量
        if (aField->mFieldType == DefFieldType::DT_STRING)
            *(char**)((uintptr_t)theDefinition + aField->mFieldOffset) = (char *)"";  // 将所有 char* 类型的成员变量赋值为空字符数组的指针
}

void DefinitionXmlError(XMLParser* theXmlParser, const char* theFormat, ...)
{
    va_list argList;
    va_start(argList, theFormat);
    std::string aFormattedMessage = SexyStringToString(vformat(theFormat, argList));
    va_end(argList);

    int aLine = theXmlParser->GetCurrentLineNum();
    std::string aFileName = theXmlParser->GetFileName();
    TodTraceAndLog("%s(%d): XML Definition Error: %s\n", aFileName.c_str(), aLine, aFormattedMessage.c_str());
}

bool DefinitionReadXMLString(XMLParser* theXmlParser, SexyString& theValue)
{
    XMLElement aXMLElement;
    if (!theXmlParser->NextElement(&aXMLElement))  // 读取下一个 XML 元素
    {
        DefinitionXmlError(theXmlParser, "Missing element value");
        return false;
    }
    if (aXMLElement.mType == XMLElement::TYPE_END)  // 读取到结束标签则结束处理
        return true;
    else if (aXMLElement.mType != XMLElement::TYPE_ELEMENT)  // 除结束标签外，正常情况下，此处读取到的应是定义的正片内容
    {
        DefinitionXmlError(theXmlParser, "unknown element type");
        return false;
    }

    theValue = aXMLElement.mValue;  // ☆ 赋值出参

    if (!theXmlParser->NextElement(&aXMLElement))  // 继续读取下一个 XML 元素
    {
        DefinitionXmlError(theXmlParser, "Can't read element end");
        return false;
    }
    if (aXMLElement.mType != XMLElement::TYPE_END)  // 正常情况下，此处读取到的应是结束标签
    {
        DefinitionXmlError(theXmlParser, "Missing element end");
        return false;
    }
    return true;
}

bool DefSymbolValueFromString(DefSymbol* theSymbolMap, const char* theName, int* theResultValue)
{
    while (theSymbolMap->mSymbolName != nullptr)
    {
        if (stricmp(theName, theSymbolMap->mSymbolName) == 0)
        {
            *theResultValue = theSymbolMap->mSymbolValue;
            return true;
        }
        theSymbolMap++;
    }
    return false;
}

bool DefinitionReadIntField(XMLParser* theXmlParser, int* theValue)
{
    SexyString aStringValue;
    if (!DefinitionReadXMLString(theXmlParser, aStringValue))
        return false;

    if (sexysscanf(aStringValue.c_str(), _S("%d"), theValue) == 1)
        return true;

    DefinitionXmlError(theXmlParser, "Can't parse int value '%s'", aStringValue.c_str());
    return false;
}

bool DefinitionReadFloatField(XMLParser* theXmlParser, float* theValue)
{
    SexyString aStringValue;
    if (!DefinitionReadXMLString(theXmlParser, aStringValue))
        return false;

    if (sexysscanf(aStringValue.c_str(), _S("%f"), theValue) == 1)
        return true;

    DefinitionXmlError(theXmlParser, "Can't parse float value '%s'", aStringValue.c_str());
    return false;
}

bool DefinitionReadStringField(XMLParser* theXmlParser, char** theValue)
{
    SexyString aStringValue;
    if (!DefinitionReadXMLString(theXmlParser, aStringValue))
        return false;

    if (aStringValue.size() == 0)
    {
        *theValue = (char *)"";
    }
    else
    {
        *theValue = (char*)DefinitionAlloc(aStringValue.size());
        strcpy(*theValue, aStringValue.c_str());
    }
    return true;
}

bool DefinitionReadEnumField(XMLParser* theXmlParser, int* theValue, DefSymbol* theSymbolMap)
{
    SexyString aStringValue;
    if (!DefinitionReadXMLString(theXmlParser, aStringValue))
        return false;

    if (DefSymbolValueFromString(theSymbolMap, aStringValue.c_str(), theValue))
        return true;

    DefinitionXmlError(theXmlParser, "Can't parse enum value '%s'", aStringValue.c_str());
    return false;
}

bool DefinitionReadVector2Field(XMLParser* theXmlParser, SexyVector2* theValue)
{
    SexyString aStringValue;
    if (!DefinitionReadXMLString(theXmlParser, aStringValue))
        return false;

    if (sexysscanf(aStringValue.c_str(), _S("%f %f"), theValue) == 1)
        return true;

    DefinitionXmlError(theXmlParser, "Can't parse vector2 value '%s'", aStringValue.c_str());
    return false;
}

bool DefinitionReadArrayField(XMLParser* theXmlParser, DefinitionArrayDef* theArray, DefField* theField)
{
    DefMap* aDefMap = (DefMap*)theField->mExtraData;

    if (theArray->mArrayCount == 0)
    {
        theArray->mArrayCount = 1;
        theArray->mArrayData = DefinitionAlloc(aDefMap->mDefSize);
    }
    else
    {
        // 当 theArray 中已存在元素，且元素的个数为 2 的整数次幂时
        // TODO Potential error with the bracketing for the &
        if (theArray->mArrayCount >= 1 && (theArray->mArrayCount == 1 || ((theArray->mArrayCount & (theArray->mArrayCount - 1)) == 0)))
        {
            void* anOldData = theArray->mArrayData;
            theArray->mArrayData = DefinitionAlloc(2 * theArray->mArrayCount * aDefMap->mDefSize);
            memcpy(theArray->mArrayData, anOldData, theArray->mArrayCount * aDefMap->mDefSize);
            delete[] (char *)anOldData;
        }
        theArray->mArrayCount++;
    }

    if (DefinitionLoadMap(theXmlParser, aDefMap, (unsigned char*)theArray->mArrayData + aDefMap->mDefSize * (theArray->mArrayCount - 1)))
        return true;

    DefinitionXmlError(theXmlParser, "failed to read sub def");
    return false;
}

bool DefinitionReadFloatTrackField(XMLParser* theXmlParser, FloatParameterTrack* theTrack)
{
    /*
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    */
    return true;
}

bool DefinitionReadFlagField(XMLParser* theXmlParser, const SexyString& theElementName, uint* theResultValue, DefSymbol* theSymbolMap)
{
    int aValue;
    if (!DefSymbolValueFromString(theSymbolMap, theElementName.c_str(), &aValue))
        return false;

    SexyString aStringValue;
    if (!DefinitionReadXMLString(theXmlParser, aStringValue))
        return false;

    int aFlag;
    if (sexysscanf(aStringValue.c_str(), _S("%f %f"), &aFlag) != 1)
    {
        DefinitionXmlError(theXmlParser, "Can't parse int value '%s'", aStringValue.c_str());
        return false;
    }

    if (aFlag)
    {
        *theResultValue |= 1 << aValue;
    }
    else
    {
        *theResultValue &= ~(1 << aValue);
    }
    return true;
}

bool DefinitionReadImageField(XMLParser* theXmlParser, Image** theImage)
{
    SexyString aStringValue;
    if (!DefinitionReadXMLString(theXmlParser, aStringValue))
        return false;

    if (DefinitionLoadImage(theImage, aStringValue))
        return true;

    std::string aMessgae = StrFormat("Failed to find image '%s' in %s", SexyStringToStringFast(aStringValue).c_str(), theXmlParser->GetFileName().c_str());
    TodErrorMessageBox(aMessgae.c_str(), "Missing image");

    return false;
}

bool DefinitionReadFontField(XMLParser* theXmlParser, _Font** theFont)
{
    SexyString aStringValue;
    if (!DefinitionReadXMLString(theXmlParser, aStringValue))
        return false;

    if (DefinitionLoadFont(theFont, aStringValue))
        return true;

    std::string aMessgae = StrFormat("Failed to find font '%s' in %s", SexyStringToStringFast(aStringValue).c_str(), theXmlParser->GetFileName().c_str());
    TodErrorMessageBox(aMessgae.c_str(), "Missing font");

    return false;
}

bool DefinitionReadField(XMLParser* theXmlParser, DefMap* theDefMap, void* theDefinition, bool* theDone)
{
    if (theXmlParser->HasFailed())
        return false;

    XMLElement aXMLElement;
    if (!theXmlParser->NextElement(&aXMLElement) || aXMLElement.mType == XMLElement::TYPE_END)  // 读取下一个 XML 元素
    {
        *theDone = true;  // 没有下一个元素则表示读取完成
        return true;
    }
    if (aXMLElement.mType != XMLElement::TYPE_START)  // 正常情况下，此处读取到的应是开始标签，而其他内容在后续的相应函数中读取
    {
        DefinitionXmlError(theXmlParser, "Missing element start");
        return false;
    }

    for (DefField* aField = theDefMap->mMapFields; *aField->mFieldName != '\0'; aField++)
    {
        void* pVar = (void*)((uintptr_t)theDefinition + aField->mFieldOffset);
        if (aField->mFieldType == DefFieldType::DT_FLAGS && DefinitionReadFlagField(theXmlParser, aXMLElement.mValue, nullptr, (DefSymbol*)aField->mExtraData))
            return true;
        
        if (stricmp(aXMLElement.mValue.c_str(), aField->mFieldName) == 0)  // 判断 aXMLElement 定义的是否为该成员变量
        {
            bool aSuccess;
            switch (aField->mFieldType)
            {
            case DefFieldType::DT_INT:
                aSuccess = DefinitionReadIntField(theXmlParser, (int*)pVar);
                break;
            case DefFieldType::DT_FLOAT:
                aSuccess = DefinitionReadFloatField(theXmlParser, (float*)pVar);
                break;
            case DefFieldType::DT_STRING:
                aSuccess = DefinitionReadStringField(theXmlParser, (char**)pVar);
                break;
            case DefFieldType::DT_ENUM:
                aSuccess = DefinitionReadEnumField(theXmlParser, (int*)pVar, (DefSymbol*)aField->mExtraData);
                break;
            case DefFieldType::DT_VECTOR2:
                aSuccess = DefinitionReadVector2Field(theXmlParser, (SexyVector2*)pVar);
                break;
            case DefFieldType::DT_ARRAY:
                aSuccess = DefinitionReadArrayField(theXmlParser, (DefinitionArrayDef*)pVar, aField);
                break;
            case DefFieldType::DT_TRACK_FLOAT:
                aSuccess = DefinitionReadFloatTrackField(theXmlParser, (FloatParameterTrack*)pVar);
                break;
            case DefFieldType::DT_IMAGE:
                aSuccess = DefinitionReadImageField(theXmlParser, (Image**)pVar);
                break;
            case DefFieldType::DT_FONT:
                aSuccess = DefinitionReadFontField(theXmlParser, (_Font**)pVar);
                break;
            default:
                aSuccess = false;
                TOD_ASSERT(false);
                break;
            }
            if (aSuccess)
                return true;

            DefinitionXmlError(theXmlParser, "Failed to read '%s' field", aXMLElement.mValue.c_str());
            return false;
        }
    }
    DefinitionXmlError(theXmlParser, "Ignoring unknown element '%s'", aXMLElement.mValue.c_str());  // aXMLElement 未定义任何成员变量时
    return false;
}

bool DefinitionLoadMap(XMLParser* theXmlParser, DefMap* theDefMap, void* theDefinition)
{
    if (theDefMap->mConstructorFunc)
        theDefMap->mConstructorFunc(theDefinition);  // 利用构造函数构造 theDefinition
    else
        DefinitionFillWithDefaults(theDefMap, theDefinition);  // 以默认值填充 theDefinition

    bool aDone = false;
    while (!aDone)
        if (!DefinitionReadField(theXmlParser, theDefMap, theDefinition, &aDone))
            return false;
    return true;
}

bool DefinitionWriteCompiledFile(const SexyString& theCompiledFilePath, DefMap* theDefMap, void* theDefinition)
{
    /*
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    ####################################################################################################
    */
    return true;
}

bool DefinitionCompileFile(const SexyString theXMLFilePath, const SexyString& theCompiledFilePath, DefMap* theDefMap, void* theDefinition)
{
    XMLParser aXMLParser;
    if (!aXMLParser.OpenFile(theXMLFilePath))
    {
        TodTrace(_S("XML file not found: %s\n"), theXMLFilePath.c_str());
        return false;
    }
    else if (!DefinitionLoadMap(&aXMLParser, theDefMap, theDefinition))
        return false;
    
    DefinitionWriteCompiledFile(theCompiledFilePath, theDefMap, theDefinition);
    return true;
}

//0x4447F0 : (void* def, *defMap, string& xmlFilePath)  //esp -= 0xC
bool DefinitionCompileAndLoad(const SexyString& theXMLFilePath, DefMap* theDefMap, void* theDefinition)
{
#ifdef _DEBUG  // 内测版执行的内容

    TodHesitationTrace(_S("predef"));
    SexyString aCompiledFilePath = DefinitionGetCompiledFilePathFromXMLFilePath(theXMLFilePath);
    if (DefinitionIsCompiled(theXMLFilePath) && DefinitionReadCompiledFile(aCompiledFilePath, theDefMap, theDefinition))
    {
        TodHesitationTrace(_S("loaded %s"), aCompiledFilePath.c_str());
        return true;
    }
    else
    {
        PerfTimer aTimer;
        aTimer.Start();
        bool aResult = DefinitionCompileFile(theXMLFilePath, aCompiledFilePath, theDefMap, theDefinition);
        TodTrace(_S("compile %d ms:'%s'"), (int)aTimer.GetDuration(), aCompiledFilePath.c_str());
        TodHesitationTrace(_S("compiled %s"), aCompiledFilePath.c_str());
        return aResult;
    }

#else  // 原版执行的内容

    SexyString aCompiledFilePath = DefinitionGetCompiledFilePathFromXMLFilePath(theXMLFilePath);
    if (DefinitionReadCompiledFile(aCompiledFilePath, theDefMap, theDefinition))
        return true;

    TodErrorMessageBox(StrFormat(_S("missing resource %s"), aCompiledFilePath.c_str()).c_str(), _S("Error"));
    exit(0);
    
#endif
}

//0x4448E0
float FloatTrackEvaluate(FloatParameterTrack& theTrack, float theTimeValue, float theInterp)
{
    if (theTrack.mCountNodes == 0)
        return 0.0f;

    if (theTimeValue < theTrack.mNodes[0].mTime)  // 如果当前时间小于第一个节点的开始时间
        return TodCurveEvaluate(theInterp, theTrack.mNodes[0].mLowValue, theTrack.mNodes[0].mHighValue, theTrack.mNodes[0].mDistribution);

    for (int i = 1; i < theTrack.mCountNodes; i++)
    {
        FloatParameterTrackNode* aNodeNxt = &theTrack.mNodes[i];
        if (theTimeValue <= aNodeNxt->mTime)  // 寻找首个开始时间大于当前时间的节点
        {
            FloatParameterTrackNode* aNodeCur = &theTrack.mNodes[i - 1];
            // 计算当前时间在〔当前节点至下一节点〕的过程中的进度
            float aTimeFraction = (theTimeValue - aNodeCur->mTime) / (aNodeNxt->mTime - aNodeCur->mTime);
            float aLeftValue = TodCurveEvaluate(theInterp, aNodeCur->mLowValue, aNodeCur->mHighValue, aNodeCur->mDistribution);
            float aRightValue = TodCurveEvaluate(theInterp, aNodeNxt->mLowValue, aNodeNxt->mHighValue, aNodeNxt->mDistribution);
            return TodCurveEvaluate(aTimeFraction, aLeftValue, aRightValue, aNodeCur->mCurveType);
        }
    }

    FloatParameterTrackNode* aLastNode = &theTrack.mNodes[theTrack.mCountNodes - 1];  // 如果当前时间大于最后一个节点的开始时间
    return TodCurveEvaluate(theInterp, aLastNode->mLowValue, aLastNode->mHighValue, aLastNode->mDistribution);
}

//0x4449F0
void FloatTrackSetDefault(FloatParameterTrack& theTrack, float theValue)
{
    if (theTrack.mNodes == nullptr && theValue != 0.0f)  // 确保该参数轨道无节点（未被赋值过）且给定的默认值不为 0
    {
        theTrack.mCountNodes = 1;  // 默认参数轨道有且仅有 1 个节点
        FloatParameterTrackNode* aNode = (FloatParameterTrackNode*)DefinitionAlloc(sizeof(FloatParameterTrackNode));
        theTrack.mNodes = aNode;
        aNode->mTime = 0.0f;
        aNode->mLowValue = theValue;
        aNode->mHighValue = theValue;
        aNode->mCurveType = TodCurves::CURVE_CONSTANT;
        aNode->mDistribution = TodCurves::CURVE_LINEAR;
    }
}

bool FloatTrackIsSet(const FloatParameterTrack& theTrack)
{
    return theTrack.mCountNodes != 0 && theTrack.mNodes[0].mCurveType != TodCurves::CURVE_CONSTANT;
}

bool FloatTrackIsConstantZero(FloatParameterTrack& theTrack)
{
    // 当轨道无节点，或仅存在一个节点且该节点的最大、最小值均为 0 时，认为该轨道上的值恒为零
    return theTrack.mCountNodes == 0 || (theTrack.mCountNodes == 1 && theTrack.mNodes[0].mLowValue == 0.0f && theTrack.mNodes[0].mHighValue == 0.0f);
}

//0x5167F0
float FloatTrackEvaluateFromLastTime(FloatParameterTrack& theTrack, float theTimeValue, float theInterp)
{
    return theTimeValue < 0.0f ? 0.0f : FloatTrackEvaluate(theTrack, theTimeValue, theInterp);
}

//0x444A50
void DefinitionFreeArrayField(DefinitionArrayDef* theArray, DefMap* theDefMap)
{
    for (int i = 0; i < theArray->mArrayCount; i++)
        DefinitionFreeMap(theDefMap, (void*)((intptr_t)theArray->mArrayData + theDefMap->mDefSize * i));  // 最后一个参数表示 pData[i]
    delete[] (char *)theArray->mArrayData;
    theArray->mArrayData = nullptr;
}

//0x444A90
void DefinitionFreeMap(DefMap* theDefMap, void* theDefinition)
{
    // 根据 theDefMap 遍历 theDefinition 的每个成员变量
    for (DefField* aField = theDefMap->mMapFields; *aField->mFieldName != '\0'; aField++)
    {
        void* aVar = (void*)((intptr_t)theDefinition + aField->mFieldOffset);  // 指向该成员变量的指针
        switch (aField->mFieldType)
        {
        case DefFieldType::DT_STRING:
            if (**(char**)aVar != '\0')
                delete[] *(char**)aVar;  // 释放字符数组
            *(char**)aVar = nullptr;
            break;
        case DefFieldType::DT_ARRAY:
            DefinitionFreeArrayField((DefinitionArrayDef*)aVar, (DefMap*)aField->mExtraData);
            break;
        case DefFieldType::DT_TRACK_FLOAT:
            if (((FloatParameterTrack*)aVar)->mCountNodes != 0)
                delete[]((FloatParameterTrack*)aVar)->mNodes;  // 释放浮点参数轨道的节点
            ((FloatParameterTrack*)aVar)->mNodes = nullptr;
            break;
        default:
            break;
        }
    }
}
