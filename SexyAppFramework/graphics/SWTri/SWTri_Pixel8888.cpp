// This file is included by SWTri.cpp and should not be built directly by the project.

	#if defined(TEXTURED)
	{
		#include "SWTri_GetTexel.cpp"
		
		if (alpha > 0x08)
		{
			#include "SWTri_TexelARGB.cpp"
				
			#if defined(GLOBAL_ARGB) || defined (TEX_ALPHA) || defined(MOD_ARGB)
			{
				if (alpha < 0xf0)
				{
					unsigned int p = *pix;
					unsigned int da = p >> 24;
					
					unsigned int tr,tg,tb;
					#if !defined(LINEAR_BLEND)
					{
						tr = ((tex&0xFF0000)*alpha)&0xFF000000;
						tg = ((tex&0x00FF00)*alpha)&0x00FF0000;
						tb = ((tex&0x0000FF)*alpha)&0x0000FF00;
					}
					#else
					{
						tr = (tex&0xFF0000)<<8;
						tg = (tex&0x00FF00)<<8;
						tb = (tex&0x0000FF)<<8;
					}
					#endif
					
					unsigned int dr = (((p&0xFF0000)*da)>>8) & 0xFF0000;
					unsigned int dg = (((p&0x00FF00)*da)>>8) & 0x00FF00;
					unsigned int db = (((p&0x0000FF)*da)>>8) & 0x0000FF;
					
					int finalAlpha = 256 - (((256 - alpha)*(256 - da))>>8);
					tr = ((tr + (256-alpha)*dr)/finalAlpha) & 0xFF0000;
					tg = ((tg + (256-alpha)*dg)/finalAlpha) & 0x00FF00;
					tb = ((tb + (256-alpha)*db)/finalAlpha) & 0x0000FF;
					
					*pix = ((finalAlpha-1)<<24) | tr | tg | tb; 
				}
				else
				{
					*pix = tex | 0xFF000000;
				}
			}
			#else
			{
				*pix = tex | 0xFF000000;
			}
			#endif
		}
	}
	#elif defined(MOD_ARGB)
	{
		if (a > 0xf00000)
		{
			*pix = 0xFF000000 | ((r)&0xff0000)|((g>>8)&0xff00)|((b>>16)&0xff);
		}
		else if (a > 0x080000)
		{
			unsigned int p = *pix;
			unsigned int da = p >> 24;
			
			int alpha = a>>16;

			unsigned int tr = ((r)*(alpha))&0xFF000000;
			unsigned int tg = ((g>>8)*(alpha))&0x00FF0000;
			unsigned int tb = ((b>>16)*(alpha))&0x0000FF00;
			
			unsigned int dr = (((p&0xFF0000)*da)>>8) & 0xFF0000;
			unsigned int dg = (((p&0x00FF00)*da)>>8) & 0x00FF00;
			unsigned int db = (((p&0x0000FF)*da)>>8) & 0x0000FF;
			
			int finalAlpha = 256 - (((256 - alpha)*(256 - da))>>8);
			tr = ((tr + (256-alpha)*dr)/finalAlpha) & 0xFF0000;
			tg = ((tg + (256-alpha)*dg)/finalAlpha) & 0x00FF00;
			tb = ((tb + (256-alpha)*db)/finalAlpha) & 0x0000FF;
			
			*pix = ((finalAlpha-1)<<24) | tr | tg | tb;
		}
		// Not sure  of use case
		/*
		else
			_asm nop;
		*/
	}
	#endif
