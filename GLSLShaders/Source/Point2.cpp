#include "Point2.h"

Point2::Point2()
: mX(0.0f)
, mY(0.0f)
{}

Point2::Point2(float aX, float aY)
: mX(aX)
, mY(aY)
{}

Point2 Point2::operator+(const Point2 &aRHS) const
{
    return Point2(mX+aRHS.mX,mY + aRHS.mY);
}

Point2 Point2::operator-(const Point2 &aRHS) const
{
    return Point2(mX - aRHS.mX,mY - aRHS.mY);
}

Point2 Point2::operator*(float aRHS) const
{
    return Point2(aRHS * mX,aRHS * mY);
}

Point2 Point2::operator/(float aRHS) const
{
    return Point2(mX/aRHS,mY/aRHS);
}

Point2 Point2::operator-() const
{
    return Point2(-mX,-mY);
}

Point2 operator*(float aLHS, const Point2& aRHS)
{
    return Point2(aLHS * aRHS.X(),aLHS * aRHS.Y());
}

Point2& Point2::operator+=(const Point2 &aRHS)
{
    mX += aRHS.mX;
    mY += aRHS.mY;

    return *this;
}

Point2& Point2::operator-=(const Point2 &aRHS)
{
    mX -= aRHS.mX;
    mY -= aRHS.mY;

    return *this;
}

Point2& Point2::operator*=(float aRHS)
{
    mX *= aRHS;
    mY *= aRHS;

    return *this;
}

Point2& Point2::operator/=(float aRHS)
{
    mX /= aRHS;
    mY /= aRHS;

    return *this;
}

Point2::~Point2()
{}
