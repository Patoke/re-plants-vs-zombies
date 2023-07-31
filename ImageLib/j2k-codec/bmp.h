
/*/////////////////////////////////////////////////////////////////////////////////////////
// BMP Saving
/////////////////////////////////////////////////////////////////////////////////////////*/

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>

int SaveAsBMP(char *filename, unsigned char *buffer, int width, int height, int bpp)
{
 int w, w2, h, tmp, color_map_size; unsigned char *row; FILE *f; 

 BITMAPFILEHEADER bmpHeader; BITMAPINFOHEADER bmpInfoHeader; 

 color_map_size=(bpp==1 ? 256*4 : 0);

 bmpHeader.bfType='MB';
 bmpHeader.bfSize=14+40;//sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
 bmpHeader.bfSize+=color_map_size;
 bmpHeader.bfOffBits=bmpHeader.bfSize;
 bmpHeader.bfSize+=width*height*bpp;
 bmpHeader.bfReserved1=bmpHeader.bfReserved2=0;

 bmpInfoHeader.biSize=40;//sizeof(BITMAPINFOHEADER); 
 bmpInfoHeader.biWidth=width;
 bmpInfoHeader.biHeight=height; 
 bmpInfoHeader.biPlanes=1; 
 bmpInfoHeader.biBitCount=(bpp==1) ? 8 : ((bpp==3) ? 24 : 32);
 bmpInfoHeader.biCompression=BI_RGB;
 bmpInfoHeader.biSizeImage=0; 
 bmpInfoHeader.biXPelsPerMeter=0; 
 bmpInfoHeader.biYPelsPerMeter=0; 
 bmpInfoHeader.biClrUsed=(bpp==1 ? 256 : 0); 
 bmpInfoHeader.biClrImportant=0; 

 f=fopen(filename, "wb"); if(!f) return 0;

// fwrite(&bmpHeader, 14, 1, f);
 fwrite(&bmpHeader.bfType, 2, 1, f);
 fwrite(&bmpHeader.bfSize, 4, 1, f);
 fwrite(&bmpHeader.bfReserved1, 2, 1, f);
 fwrite(&bmpHeader.bfReserved2, 2, 1, f);
 fwrite(&bmpHeader.bfOffBits, 4, 1, f);

// fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);

 fwrite(&bmpInfoHeader.biSize, 4, 1, f);
 fwrite(&bmpInfoHeader.biWidth, 4, 1, f);
 fwrite(&bmpInfoHeader.biHeight, 4, 1, f);
 fwrite(&bmpInfoHeader.biPlanes, 2, 1, f);
 fwrite(&bmpInfoHeader.biBitCount, 2, 1, f);
 fwrite(&bmpInfoHeader.biCompression, 4, 1, f);
 fwrite(&bmpInfoHeader.biSizeImage, 4, 1, f);
 fwrite(&bmpInfoHeader.biXPelsPerMeter, 4, 1, f);
 fwrite(&bmpInfoHeader.biYPelsPerMeter, 4, 1, f);
 fwrite(&bmpInfoHeader.biClrUsed, 4, 1, f);
 fwrite(&bmpInfoHeader.biClrImportant, 4, 1, f);

 if(bpp==1)
 {
  for(w=0; w<256; w++){ tmp=(w<<16)|(w<<8)|w; fwrite(&tmp, 4, 1, f); }

  for(h=0; h<height; h++)
  {
   row=buffer+width*(height-h-1);
   for(w=0; w<width; w++) fputc(row[w], f);
   if(width&3) for(w=0; w<4-(width&3); w++) fputc(0, f);
  }
 }
 else
 {
  for(h=0; h<height; h++)
  {
   row=buffer+width*(height-h-1)*bpp;
   for(w=0; w<width; w++, row+=bpp) fwrite(row, bpp, 1, f);
   w2=width*bpp; if(w2&3) for(w=0; w<4-(w2&3); w++) fputc(0, f);
  }
 }

 fclose(f);
 return 1;
}
