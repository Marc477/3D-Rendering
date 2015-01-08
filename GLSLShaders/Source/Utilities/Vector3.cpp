#include "Vector3.h"

#include <cmath>

const Vector3 Vector3::Zero(0.0f,0.0f,0.0f);
const Vector3 Vector3::UnitX(1.0f,0.0f,0.0f);
const Vector3 Vector3::UnitY(0.0f,1.0f,0.0f);
const Vector3 Vector3::UnitZ(0.0f,0.0f,1.0f);
const Vector3 Vector3::NegativeUnitX(-1.0f, 0.0f,0.0f);
const Vector3 Vector3::NegativeUnitY(0.0f,-1.0f, 0.0f);
const Vector3 Vector3::NegativeUnitZ(0.0f,0.0f,-1.0f);
const Vector3 Vector3::UnitScale(1.0f,1.0f,1.0f);

Vector3::Vector3()
: mX(0.0f)
, mY(0.0f)
, mZ(0.0f)
{}

Vector3::Vector3(const Vector3& aRHS)
: mX(aRHS.mX)
, mY(aRHS.mY)
, mZ(aRHS.mZ)
{}

Vector3::Vector3(float aX, float aY, float aZ)
: mX(aX)
, mY(aY)
, mZ(aZ)
{}

float Vector3::Length() const
{
    return sqrt((mX*mX) + (mY*mY) + (mZ*mZ));
}

float Vector3::SquaredLength() const
{
    return (mX*mX)+(mY*mY)+(mZ*mZ);
}

bool Vector3::IsZeroLength()
{
    return mX == 0.0f && mY == 0.0f && mZ == 0.0f;
}

float Vector3::Normalise()
{
    float TmpLength = sqrt((mX*mX) + (mY*mY) + (mZ*mZ));

    if(TmpLength > 1e-08)
    {
        float InvLength = 1.0f / TmpLength; 
        //1 division and 3 multiplication is
        //faster than 3 divisions.

        mX *= InvLength;
        mY *= InvLength;
        mZ *= InvLength;
    }

    return TmpLength;
}

Vector3 Vector3::NormalisedCopy() const
{
    Vector3 RetVec(*this);
    RetVec.Normalise();
    return RetVec;
}

Vector3 Vector3::Reflect(const Vector3 &aNormal) const
{
    //R = 2(N*L)N-L
    return Vector3(2*((aNormal.DotProduct(*this))*aNormal)-*this);
}

Vector3 Vector3::Perpendicular()const
{
    static const double SquareZero = 1e-06 * 1e-06;
    
    Vector3 TmpPerpVec = this->CrossProduct( Vector3::UnitX);

    if(TmpPerpVec.SquaredLength() < SquareZero)
    {
        //This vector is the Y axis multiplied by a scalar, so we have
        //to use another axis.
        TmpPerpVec = this->CrossProduct(Vector3::UnitY);
    }

    TmpPerpVec.Normalise();

    return TmpPerpVec;
}

Vector3 Vector3::operator+(const Vector3 &aRHS) const
{
    return Vector3(
                   mX + aRHS.mX,
                   mY + aRHS.mY,
                   mZ + aRHS.mZ
                   );
}

Vector3 Vector3::operator-(const Vector3 &aRHS) const
{
    return Vector3(
                   mX - aRHS.mX,
                   mY - aRHS.mY,
                   mZ - aRHS.mZ
                   );
}

Vector3 Vector3::operator*(float aRHS) const
{
    return Vector3(
                   mX * aRHS,
                   mY * aRHS,
                   mZ * aRHS
                   );
}


Vector3 Vector3::operator/(float aRHS) const
{
    assert( aRHS != 0.0f);
    return Vector3(
                   mX/aRHS,
                   mY/aRHS,
                   mZ/aRHS
                   );
}

Vector3 Vector3::operator-() const
{
    return Vector3(*this) *= -1.0f;
}

float Vector3::DotProduct(const Vector3 &aRHS) const
{
    return mX * aRHS.mX + mY * aRHS.mY + mZ * aRHS.mZ;
}

Vector3 Vector3::CrossProduct(const Vector3 &aRHS) const
{
    return Vector3(
                   mY * aRHS.mZ - mZ * aRHS.mY,
                   mZ * aRHS.mX - mX * aRHS.mZ,
                   mX * aRHS.mY - mY * aRHS.mX
                   );
}

Vector3& Vector3::operator+=(const Vector3 &aRHS)
{
    mX += aRHS.mX;
    mY += aRHS.mY;
    mZ += aRHS.mZ;

    return *this;
}

Vector3& Vector3::operator-=(const Vector3 &aRHS)
{
    mX -= aRHS.mX;
    mY -= aRHS.mY;
    mZ -= aRHS.mZ;

    return *this;
}

Vector3& Vector3::operator*=(float aRHS)
{
    mX *= aRHS;
    mY *= aRHS;
    mZ *= aRHS;

    return *this;
}

Vector3& Vector3::operator/=(float aRHS)
{
    //assert(aRHS != 0.0f);
    mX /= aRHS;
    mY /= aRHS;
    mZ /= aRHS;

    return *this;
}

Vector3 operator*(float aLHS, const Vector3& aRHS)
{
    return Vector3(aLHS * aRHS.X(),aLHS * aRHS.Y(), aLHS * aRHS.Z());
}
