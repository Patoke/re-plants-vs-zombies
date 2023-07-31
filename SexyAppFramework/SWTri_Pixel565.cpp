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
					
					trb = ((trb>>8)&0xf800)|((trb>>3)&0x001f);
					tg = ((tg>>5)&0x07e0);
					tex = *pix;
					alpha = (0xff - alpha)>>3;
					unsigned int	prb = (((tex&0xf81f) * alpha) >> 5) & 0xf81f;
					unsigned int	pg  = (((tex&0x07e0) * alpha) >> 5) & 0x07e0;
					*pix = (trb|tg) + (prb|pg);
				}
				else
				{
					*pix = ((tex>>8)&0xf800)|((tex>>5)&0x07e0)|((tex>>3)&0x001f);
				}
			}
			#else
			{
				*pix = ((tex>>8)&0xf800)|((tex>>5)&0x07e0)|((tex>>3)&0x001f);
			}
			#endif
		}
	}
	#elif defined(MOD_ARGB)
	{
		if (a > 0xf00000)
		{
			*pix = ((r>>8)&0xf800)|((g>>13)&0x07e0)|((b>>19)&0x001f);
		}
		else if (a > 0x080000)
		{
			unsigned int	alpha = a >> 16;
			unsigned int	_rb = ((((r&0xff0000) | (b>>16)) * alpha)>> 8)&0xff00ff;
			unsigned int	_g  =  (((g&0xff0000)            * alpha)>>16)&0x00ff00;
					_rb = ((_rb>>8)&0xf800)|((_rb>>3)&0x001f);
					_g = ((_g>>5)&0x07e0);
			unsigned int	p = *pix;
					alpha = (0xff - alpha)>>3;
			unsigned int	prb = (((p&0xf81f) * alpha) >> 5) & 0xf81f;
			unsigned int	pg  = (((p&0x07e0) * alpha) >> 5) & 0x07e0;
			*pix = (_rb|_g)+(prb|pg);
		}
	}
	#endif
