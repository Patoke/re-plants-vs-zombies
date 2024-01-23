#ifndef __TRIVERTEX_H__
#define __TRIVERTEX_H__

namespace Sexy
{

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class TriVertex
{
public:
	float x,y,u,v;
	unsigned long color; //ARGB (0 = use color specified in function call)

public:
	TriVertex() { color = 0; }
	TriVertex(float theX, float theY) : x(theX), y(theY) { color = 0; }
	TriVertex(float theX, float theY, float theU, float theV) : x(theX), y(theY), u(theU), v(theV) { color = 0; }
	TriVertex(float theX, float theY, float theU, float theV, unsigned long theColor) : x(theX), y(theY), u(theU), v(theV), color(theColor) { }
};

} // namespace Sexy


#endif