enum DefFieldType {
    Invalid,
    Int,
    Float,
    String,
    FlagEnum(DefSymbol),
    Vector2,
    Array(&'static DefMap),
    TrackFloat,
    Flags(&'static [DefSymbol]),
    Image,
    Font,
}

struct DefSymbol {
    symbol_value: u32,
    symbol_name: &'static str,
}

struct DefField {
    field_name: &'static str,
    field_type: DefFieldType,
}

struct DefMap {
    map_fields: &'static [DefField],
}

const TRAIL_FLAG_DEF_SYMBOLS: &'static [DefSymbol] = {
    &[DefSymbol {
        symbol_value: 0,
        symbol_name: "Loops",
    }]
};

#[rustfmt::skip]
const TRAIL_DEF_FIELDS: &'static[DefField]  = {
    type T = DefFieldType;
    &[
        DefField{ field_name: "Image", field_type: T::Image },
        DefField{ field_name: "MaxPoints", field_type: T::Int },
        DefField{ field_name: "MinPointDistance", field_type: T::Float },
        DefField{ field_name: "TrailFlags", field_type: T::Flags(TRAIL_FLAG_DEF_SYMBOLS) },
        DefField{ field_name: "WidthOverLength", field_type: T::Image },
        DefField{ field_name: "WidthOverTime", field_type: T::Image },
        DefField{ field_name: "AlphaOverLength", field_type: T::Image },
        DefField{ field_name: "AlphaOverTime", field_type: T::Image },
        DefField{ field_name: "TrailDuration", field_type: T::Image },
        DefField{ field_name: "", field_type: T::Invalid}
    ]
};

#[rustfmt::skip]
const PARTICLE_FLAG_SYMBOLS: &'static [DefSymbol] = {  //0x69E290
    &[
    DefSymbol{ symbol_value: 0, symbol_name:"RandomLaunchSpin" },
    DefSymbol{ symbol_value: 1, symbol_name: "AlignLaunchSpin" },
    DefSymbol{ symbol_value: 2, symbol_name: "AlignToPixel" },
    DefSymbol{ symbol_value: 4, symbol_name: "ParticleLoops" },
    DefSymbol{ symbol_value: 3, symbol_name: "SystemLoops" },
    DefSymbol{ symbol_value: 5, symbol_name:"ParticlesDontFollow" },
    DefSymbol{ symbol_value: 6, symbol_name: "RandomStartTime" },
    DefSymbol{ symbol_value: 7, symbol_name: "DieIfOverloaded" },
    DefSymbol{ symbol_value: 8, symbol_name: "Additive" },
    DefSymbol{ symbol_value: 9, symbol_name: "FullScreen" },
    DefSymbol{ symbol_value:10, symbol_name:"SoftwareOnly" },
    DefSymbol{ symbol_value:11, symbol_name: "HardwareOnly" },
    DefSymbol{ symbol_value:u32::MAX, symbol_name: "" }
    ]
};

#[rustfmt::skip]
const EMMITTER_TYPE_SYMBOLS: &'static [DefSymbol] = {  //0x69E290
    &[
    DefSymbol{ symbol_value: 0, symbol_name: "Circle" },
    DefSymbol{ symbol_value: 1, symbol_name: "Box" },
    DefSymbol{ symbol_value: 2, symbol_name: "BoxPath" },
    DefSymbol{ symbol_value: 3, symbol_name: "CirclePath" },
    DefSymbol{ symbol_value: 4, symbol_name: "CircleEvenSpacing" },
    DefSymbol{ symbol_value:u32::MAX, symbol_name: "" }
    ]
};

#[rustfmt::skip]
const PARTICLE_TYPE_SYMBOLS: &'static [DefSymbol] = {  //0x69E290
    &[
    DefSymbol{ symbol_value: 0, symbol_name: "Circle" },
    DefSymbol{ symbol_value: 1, symbol_name: "Box" },
    DefSymbol{ symbol_value: 2, symbol_name: "BoxPath" },
    DefSymbol{ symbol_value: 4, symbol_name: "CirclePath" },
    DefSymbol{ symbol_value: 3, symbol_name: "CircleEvenSpacing" },
    DefSymbol{ symbol_value:u32::MAX, symbol_name: "" }
    ]
};

/*
DefSymbol gParticleFlagSymbols[] = {  //0x69E290
    {  0, "RandomLaunchSpin" },     {  1, "AlignLaunchSpin" },  {  2, "AlignToPixel" },     {  4, "ParticleLoops" },    {  3, "SystemLoops" },
    {  5, "ParticlesDontFollow" },  {  6, "RandomStartTime" },  {  7, "DieIfOverloaded" },  {  8, "Additive" },         {  9, "FullScreen" },
    { 10, "SoftwareOnly" },         { 11, "HardwareOnly" },     { -1, nullptr }
};

DefSymbol gEmitterTypeSymbols[] = {  //0x69E260
    {  0, "Circle" },               {  1, "Box" },              {  2, "BoxPath" },          {  3, "CirclePath" },       {  4, "CircleEvenSpacing" },
    { -1, nullptr }
};

DefMap gParticleFieldDefMap = { gParticleFieldDefFields, sizeof(ParticleField), ParticleFieldConstructor };

DefField gParticleFieldDefFields[] = {  //0x69E2F8
    { "FieldType",          0x0,        DefFieldType::DT_ENUM,          gParticleTypeSymbols },
    { "x",                  0x4,        DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "y",                  0xC,        DefFieldType::DT_TRACK_FLOAT,   nullptr },
    { "",                   0x0,        DefFieldType::DT_INVALID,       nullptr },
};
*/

/*
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
*/

fn main() {
    println!("Hello, world!");
}
