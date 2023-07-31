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
					unsigned int trb, tg;
					#if !defined(LINEAR_BLEND)
					{
						trb = (((tex&0xff00ff) * alpha) >> 8) & 0xff00ff;
						tg  = (((tex&0x00ff00) * alpha) >> 8) & 0x00ff00;
					}
					#else
					{
						trb = tex&0xff00ff;
						tg = tex&0x00ff00;
					}
					#endif

					tex = *pix;
					alpha = 0xff - alpha;
					unsigned int	prb = (((tex&0xff00ff) * alpha) >> 8) & 0xff00ff;
					unsigned int	pg  = (((tex&0x00ff00) * alpha) >> 8) & 0x00ff00;
					*pix = 0xFF000000 | ((trb|tg) + (prb|pg));
				}
				else
				{
					*pix = 0xFF000000 | tex;
				}
			}
			#else
			{
				*pix = 0xFF000000 | tex;
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
			unsigned int	alpha = a >> 16;
			unsigned int	_rb = ((((r&0xff0000) | (b>>16)) * alpha)>> 8)&0xff00ff;
			unsigned int	_g  =  (((g&0xff0000)            * alpha)>>16)&0x00ff00;
			unsigned int	p = *pix;
			alpha = 0xff - alpha;
			unsigned int	prb = (((p&0xff00ff) * alpha) >> 8) & 0xff00ff;
			unsigned int	pg  = (((p&0x00ff00) * alpha) >> 8) & 0x00ff00;
			*pix = 0xFF000000 | (_rb|_g)+(prb|pg);
		}
	}
	#endif
