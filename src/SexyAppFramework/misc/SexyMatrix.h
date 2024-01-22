#ifndef __SEXYMATRIX_H__
#define __SEXYMATRIX_H__

#include "SexyVector.h"

namespace Sexy
{

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class SexyMatrix3
{
public:
	union
    {
        float m[3][3];
        struct
        {
            float m00, m01, m02;
            float m10, m11, m12;
            float m20, m21, m22;
        };
    };

public:
	SexyMatrix3();
	void ZeroMatrix();
	void LoadIdentity();

	SexyVector2 operator*(const SexyVector2 &theVec) const;
	SexyVector3 operator*(const SexyVector3 &theVec) const;
	SexyMatrix3 operator*(const SexyMatrix3 &theMat) const;
	const SexyMatrix3& operator*=(const SexyMatrix3 &theMat);
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class SexyTransform2D : public SexyMatrix3
{
public:
	SexyTransform2D();
	SexyTransform2D(bool loadIdentity);
	SexyTransform2D(const SexyMatrix3& theMatrix);

	const SexyTransform2D& operator=(const SexyMatrix3 &theMat);


	void Translate(float tx, float ty);

	// Rotate has been replaced by RotateRad.  
	// NOTE:  If you had Rotate(angle) you should now use RotateRad(-angle).  
	// This is to make positive rotations go counter-clockwise when using screen coordinates.
	void RotateRad(float rot); 
	void RotateDeg(float rot);
	void Scale(float sx, float sy);
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Transform
{
protected:
	mutable SexyTransform2D mMatrix;
	mutable bool mNeedCalcMatrix;
	void MakeComplex();
	void CalcMatrix() const;

public:
	bool mComplex, mHaveRot, mHaveScale;
	float mTransX1, mTransY1, mTransX2, mTransY2;
	float mScaleX, mScaleY;
	float mRot;

public:
	Transform();

	void Reset();

	void Translate(float tx, float ty);
	void RotateRad(float rot); 
	void RotateDeg(float rot);
	void Scale(float sx, float sy);

	const SexyTransform2D& GetMatrix() const;
};


} // namespace Sexy

#endif