#include "PoolEffect.h"
#include "../../LawnApp.h"
#include "../../Resources.h"
#include "../../GameConstants.h"
#include "../../Sexy.TodLib/TodDebug.h"
#include "graphics/DDImage.h"
#include "graphics/Graphics.h"
#include "graphics/DDInterface.h"
#include "graphics/D3DInterface.h"

//0x469A60
void PoolEffect::PoolEffectInitialize()
{
    TodHesitationBracket aHesitation("PoolEffectInitialize");

    mApp = gLawnApp;

    mCausticImage = new MemoryImage(gSexyAppBase);
    mCausticImage->mWidth = CAUSTIC_IMAGE_WIDTH;
    mCausticImage->mHeight = CAUSTIC_IMAGE_HEIGHT;
    mCausticImage->mBits = new unsigned long[CAUSTIC_IMAGE_WIDTH * CAUSTIC_IMAGE_HEIGHT + 1];
    mCausticImage->mHasTrans = true;
    mCausticImage->mHasAlpha = true;
    memset(mCausticImage->mBits, 0xFF, CAUSTIC_IMAGE_WIDTH * CAUSTIC_IMAGE_HEIGHT * 4);
    mCausticImage->mBits[CAUSTIC_IMAGE_WIDTH * CAUSTIC_IMAGE_HEIGHT] = MEMORYCHECK_ID;

    mCausticGrayscaleImage = new unsigned char[256 * 256];
    MemoryImage* aCausticGrayscaleImage = (MemoryImage*)IMAGE_POOL_CAUSTIC_EFFECT;
    int index = 0;
    for (int x = 0; x < 256; x++)
    {
        for (int y = 0; y < 256; y++)
        {
            mCausticGrayscaleImage[index] = (unsigned char)aCausticGrayscaleImage->mBits[index];
            index++;
        }
    }
}

void PoolEffect::PoolEffectDispose()
{
    delete mCausticImage;
    delete[] mCausticGrayscaleImage;
}

//0x469BC0
unsigned int PoolEffect::BilinearLookupFixedPoint(unsigned int u, unsigned int v)
{
    unsigned int timeU = u & 0xFFFF0000;
    unsigned int timeV = v & 0xFFFF0000;
    unsigned int factorU1 = ((u - timeU) & 0x0000FFFE) + 1;
    unsigned int factorV1 = ((v - timeV) & 0x0000FFFE) + 1;
    unsigned int factorU0 = 65536 - factorU1;
    unsigned int factorV0 = 65536 - factorV1;
    unsigned int indexU0 = (timeU >> 16) % 256;
    unsigned int indexU1 = ((timeU >> 16) + 1) % 256;
    unsigned int indexV0 = (timeV >> 16) % 256;
    unsigned int indexV1 = ((timeV >> 16) + 1) % 256;

    return
        ((((factorU0 * factorV1) / 65536) * mCausticGrayscaleImage[indexV1 * 256 + indexU0]) / 65536) +
        ((((factorU1 * factorV1) / 65536) * mCausticGrayscaleImage[indexV1 * 256 + indexU1]) / 65536) +
        ((((factorU0 * factorV0) / 65536) * mCausticGrayscaleImage[indexV0 * 256 + indexU0]) / 65536) +
        ((((factorU1 * factorV0) / 65536) * mCausticGrayscaleImage[indexV0 * 256 + indexU1]) / 65536);
}

//0x469CA0
void PoolEffect::UpdateWaterEffect()
{
    int idx = 0;
    for (int y = 0; y < CAUSTIC_IMAGE_HEIGHT; y++)
    {
        int timeV1 = (256 - y) << 17;
        int timeV0 = y << 17;

        for (int x = 0; x < CAUSTIC_IMAGE_WIDTH; x++)
        {
            unsigned long* pix = &mCausticImage->mBits[idx];

            int timeU = x << 17;
            int timePool0 = mPoolCounter << 16;
            int timePool1 = ((mPoolCounter & 65535) + 1) << 16;
            int a1 = (unsigned char)BilinearLookupFixedPoint(timeU - timePool1 / 6, timeV1 + timePool0 / 8);
            int a0 = (unsigned char)BilinearLookupFixedPoint(timeU + timePool0 / 10, timeV0);
            unsigned char a = (unsigned char)((a0 + a1) / 2);

            unsigned char alpha;
            if (a >= 160U)
            {
                alpha = 255 - 2 * (a - 160U);
            }
            else if (a >= 128U)
            {
                alpha = 5 * (a - 128U);
            }
            else
            {
                alpha = 0;
            }

            *pix = (*pix & 0x00FFFFFF) + (((int)alpha / 3) << 24);
            idx++;
        }
    }

    ++mCausticImage->mBitsChangedCount;
}

//0x469DE0
void PoolEffect::PoolEffectDraw(Sexy::Graphics* g, bool theIsNight)
{
    if (!mApp->Is3DAccelerated())
    {
        if (theIsNight)
        {
            g->DrawImage(IMAGE_POOL_NIGHT, 34, 278);
        }
        else
        {
            g->DrawImage(IMAGE_POOL, 34, 278);
        }
        return;
    }

    float aGridSquareX = IMAGE_POOL->GetWidth() / 15.0f;
    float aGridSquareY = IMAGE_POOL->GetHeight() / 5.0f;
    float aOffsetArray[3][16][6][2] = {{{{ 0 }}}};
    for (int x = 0; x <= 15; x++)
    {
        for (int y = 0; y <= 5; y++)
        {
            aOffsetArray[2][x][y][0] = x / 15.0f;
            aOffsetArray[2][x][y][1] = y / 5.0f;
            if (x != 0 && x != 15 && y != 0 && y != 5)
            {
                float aPoolPhase = mPoolCounter * 2 * PI;
                float aWaveTime1 = aPoolPhase / 800.0;
                float aWaveTime2 = aPoolPhase / 150.0;
                float aWaveTime3 = aPoolPhase / 900.0;
                float aWaveTime4 = aPoolPhase / 800.0;
                float aWaveTime5 = aPoolPhase / 110.0;
                float xPhase = x * 3.0f * 2 * PI / 15.0f;
                float yPhase = y * 3.0f * 2 * PI / 5.0f;

                aOffsetArray[0][x][y][0] = sin(yPhase + aWaveTime2) * 0.002f + sin(yPhase + aWaveTime1) * 0.005f;
                aOffsetArray[0][x][y][1] = sin(xPhase + aWaveTime5) * 0.01f + sin(xPhase + aWaveTime3) * 0.015f + sin(xPhase + aWaveTime4) * 0.005f;
                aOffsetArray[1][x][y][0] = sin(yPhase * 0.2f + aWaveTime2) * 0.015f + sin(yPhase * 0.2f + aWaveTime1) * 0.012f;
                aOffsetArray[1][x][y][1] = sin(xPhase * 0.2f + aWaveTime5) * 0.005f + sin(xPhase * 0.2f + aWaveTime3) * 0.015f + sin(xPhase * 0.2f + aWaveTime4) * 0.02f;
                aOffsetArray[2][x][y][0] += sin(yPhase + aWaveTime1 * 1.5f) * 0.004f + sin(yPhase + aWaveTime2 * 1.5f) * 0.005f;
                aOffsetArray[2][x][y][1] += sin(xPhase * 4.0f + aWaveTime5 * 2.5f) * 0.005f + sin(xPhase * 2.0f + aWaveTime3 * 2.5f) * 0.04f + sin(xPhase * 3.0f + aWaveTime4 * 2.5f) * 0.02f;
            }
            else
            {
                aOffsetArray[0][x][y][0] = 0.0f;
                aOffsetArray[0][x][y][1] = 0.0f;
                aOffsetArray[1][x][y][0] = 0.0f;
                aOffsetArray[1][x][y][1] = 0.0f;
            }
        }
    }

    int aIndexOffsetX[6] = { 0, 0, 1, 0, 1, 1 };
    int aIndexOffsetY[6] = { 0, 1, 1, 0, 1, 0 };
    TriVertex aVertArray[3][150][3];

    for (int x = 0; x < 15; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            for (int aLayer = 0; aLayer < 3; aLayer++)
            {
                TriVertex* pVert = &aVertArray[aLayer][x * 10 + y * 2][0];
                for (int aVertIndex = 0; aVertIndex < 6; aVertIndex++, pVert++)
                {
                    int aIndexX = x + aIndexOffsetX[aVertIndex];
                    int aIndexY = y + aIndexOffsetY[aVertIndex];
                    if (aLayer == 2)
                    {
                        pVert->x = (704.0f / 15.0f) * aIndexX + 45.0f;
                        pVert->y = 30.0f * aIndexY + 288.0f;
                        pVert->u = aOffsetArray[2][aIndexX][aIndexY][0] + aIndexX / 15.0f;
                        pVert->v = aOffsetArray[2][aIndexX][aIndexY][1] + aIndexY / 5.0f;

                        if (!g->mClipRect.Contains(pVert->x, pVert->y))
                        {
                            pVert->color = 0x00FFFFFFUL;
                        }
                        else if (aIndexX == 0 || aIndexX == 15 || aIndexY == 0)
                        {
                            pVert->color = 0x20FFFFFFUL;
                        }
                        else if (theIsNight)
                        {
                            pVert->color = 0x30FFFFFFUL;
                        }
                        else
                        {
                            pVert->color = aIndexX <= 7 ? 0xC0FFFFFFUL : 0x80FFFFFFUL;
                        }
                    }
                    else
                    {
                        pVert->color = 0xFFFFFFFFUL;
                        pVert->x = aIndexX * aGridSquareX + 35.0f;
                        pVert->y = aIndexY * aGridSquareY + 279.0f;
                        pVert->u = aOffsetArray[aLayer][aIndexX][aIndexY][0] + aIndexX / 15.0f;
                        pVert->v = aOffsetArray[aLayer][aIndexX][aIndexY][1] + aIndexY / 5.0f;
                        if (!g->mClipRect.Contains(pVert->x, pVert->y))
                        {
                            pVert->color = 0x00FFFFFFUL;
                        }
                    }
                }
            }
        }
    }

    if (theIsNight)
    {
        g->DrawTrianglesTex(IMAGE_POOL_BASE_NIGHT, aVertArray[0], 150);
        g->DrawTrianglesTex(IMAGE_POOL_SHADING_NIGHT, aVertArray[1], 150);
    }
    else
    {
        g->DrawTrianglesTex(IMAGE_POOL_BASE, aVertArray[0], 150);
        g->DrawTrianglesTex(IMAGE_POOL_SHADING, aVertArray[1], 150);
    }

    UpdateWaterEffect();
    D3DInterface* anInterface = ((DDImage*)g->mDestImage)->mDDInterface->mD3DInterface;
    anInterface->CheckDXError(anInterface->mD3DDevice->SetTextureStageState(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ADDRESSU, D3DTEXTUREADDRESS::D3DTADDRESS_WRAP), "DrawPool");
    anInterface->CheckDXError(anInterface->mD3DDevice->SetTextureStageState(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ADDRESSV, D3DTEXTUREADDRESS::D3DTADDRESS_WRAP), "DrawPool");
    g->DrawTrianglesTex(mCausticImage, aVertArray[2], 150);
    anInterface->CheckDXError(anInterface->mD3DDevice->SetTextureStageState(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ADDRESSU, D3DTEXTUREADDRESS::D3DTADDRESS_CLAMP), "DrawPool");
    anInterface->CheckDXError(anInterface->mD3DDevice->SetTextureStageState(0, D3DTEXTURESTAGESTATETYPE::D3DTSS_ADDRESSV, D3DTEXTUREADDRESS::D3DTADDRESS_CLAMP), "DrawPool");
}

void PoolEffect::PoolEffectUpdate()
{
    ++mPoolCounter;
}