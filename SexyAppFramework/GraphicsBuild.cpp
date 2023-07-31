#include "DDInterface.cpp" // build first because this defines INITGUID at the top

#include "Color.cpp"
#include "D3DInterface.cpp"
#include "D3DTester.cpp"
#include "DDImage.cpp"
#include "Font.cpp"
#include "Graphics.cpp"
#include "NativeDisplay.cpp"
#include "Image.cpp"
#include "ImageFont.cpp"
#include "MemoryImage.cpp"
#include "SysFont.cpp"
#include "Quantize.cpp"
#include "SharedImage.cpp"

// Leave this at the bottom because it undefs DIRECT3D_VERSION
#include "D3D8Helper.cpp"
