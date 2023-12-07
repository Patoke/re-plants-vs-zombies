// This file is included by SWTri.cpp and should not be built directly by the project.
{
	int premult; (void)premult;
	#if defined(MOD_ARGB) && defined(GLOBAL_ARGB)
	{
		premult = ((globalDiffuse.a*a)>>24);
		alpha = (alpha * premult) >> 8;
		tex =	((((tex&0xff0000)*((globalDiffuse.r*r)>>24))>>8)&0xff0000)|
			((((tex&0x00ff00)*((globalDiffuse.g*g)>>24))>>8)&0x00ff00)|
			((((tex&0x0000ff)*((globalDiffuse.b*b)>>24))>>8)&0x0000ff);
	}
	#elif !defined(MOD_ARGB) && defined(GLOBAL_ARGB)
	{
		premult = globalDiffuse.a;
		alpha = (alpha * premult) >> 8;
		tex =	((((tex&0xff0000)*globalDiffuse.r)>>8)&0xff0000)|
			((((tex&0x00ff00)*globalDiffuse.g)>>8)&0x00ff00)|
			((((tex&0x0000ff)*globalDiffuse.b)>>8)&0x0000ff);
	}
	#elif defined(MOD_ARGB) && !defined(GLOBAL_ARGB)
	{
		premult = a>>16;
		alpha = (alpha * premult) >> 8;
		tex =	((((tex&0xff0000)*(r>>16))>>8)&0xff0000)|
			((((tex&0x00ff00)*(g>>16))>>8)&0x00ff00)|
			((((tex&0x0000ff)*(b>>16))>>8)&0x0000ff);
	}
	#endif

	// linear blend expects pixel to already be premultiplied by alpha
	#if defined(LINEAR_BLEND) && (defined(MOD_ARGB) || defined(GLOBAL_ARGB))
	{
		int r = (((tex&0xff0000)*premult)>>8)&0xff0000;
		int g = (((tex&0x00ff00)*premult)>>8)&0x00ff00;
		int b = (((tex&0x0000ff)*premult)>>8)&0x0000ff;
		tex = r|g|b;
	}	
	#endif
}