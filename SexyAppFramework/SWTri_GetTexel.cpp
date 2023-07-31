// This file is included by SWTri.cpp and should not be built directly by the project.

	unsigned int tex;
	#if !defined(LINEAR_BLEND)
	{
		unsigned int	t_pos = ((v)>>16)*tex_pitch + ((u)>>16);
		tex = t_pos<tex_endpos?pTexture[t_pos]:0;
	}
	#else
	{
		int umid = u-0x8000;
		int vmid = v-0x8000;
		int umidfloor = FixedFloor(umid);
		int vmidfloor = FixedFloor(vmid);

		unsigned int	t_pos = (vmidfloor>>16)*tex_pitch + (umidfloor>>16);

		unsigned int	t00 = t_pos<tex_endpos?pTexture[t_pos]:0;
		unsigned int	t10 = t_pos+1<tex_endpos?pTexture[t_pos+1]:0;
		unsigned int	t01 = t_pos+tex_pitch<tex_endpos?pTexture[t_pos+tex_pitch]:0;
		unsigned int	t11 = t_pos+tex_pitch+1<tex_endpos?pTexture[t_pos+tex_pitch+1]:0;

		int aUFactor = ((umid-umidfloor) & 0xFFFE) + 1; // aUFactor needs to be between 1 and 0xFFFF to avoid overflow
		int aVFactor = ((vmid-vmidfloor) & 0xFFFE) + 1; // ditto for aVFactor
		int a00 = ((t00 >> 24) * ((ulong) ((0x10000  - aUFactor) * (0x10000  - aVFactor)) >> 16)) >> 16;
		int a10 = ((t10 >> 24) * ((ulong) ((           aUFactor) * (0x10000  - aVFactor)) >> 16)) >> 16;
		int a01 = ((t01 >> 24) * ((ulong) ((0x10000  - aUFactor) * (           aVFactor)) >> 16)) >> 16;
		int a11 = ((t11 >> 24) * ((ulong) ((           aUFactor) * (           aVFactor)) >> 16)) >> 16;
		unsigned int r = (((t00&0x00FF0000)*a00 + (t10&0x00FF0000)*a10 + (t01&0x00FF0000)*a01 + (t11&0x00FF0000)*a11)>>8)&0xFF0000;
		unsigned int g = (((t00&0x0000FF00)*a00 + (t10&0x0000FF00)*a10 + (t01&0x0000FF00)*a01 + (t11&0x0000FF00)*a11)>>8)&0x00FF00;
		unsigned int b = (((t00&0x000000FF)*a00 + (t10&0x000000FF)*a10 + (t01&0x000000FF)*a01 + (t11&0x000000FF)*a11)>>8)&0x0000FF;
		unsigned int a = ((a00 + a10 + a01 + a11)<<24)&0xFF000000;

		tex = a|r|g|b;
	}
	#endif

	unsigned int	alpha;
	#if defined (TEX_ALPHA)
		alpha = tex>>24;
	#else
		alpha = 0xFF;
	#endif

