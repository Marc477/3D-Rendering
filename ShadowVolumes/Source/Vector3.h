#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <cassert>

class Vector3
{
public:

    Vector3();
    Vector3(const Vector3 &aRHS);
    Vector3(float aX, float aY, float aZ);

	//Helpers
    float	Length() const;
    float	SquaredLength() const;
    bool	IsZeroLength();

    float	Normalise();
    Vector3 NormalisedCopy() const;
	
    Vector3 Reflect(const Vector3 &aNormal) const;
    Vector3 Perpendicular() const;

	//Assignement operations
    Vector3& operator=(const Vector3 &aRHS);
    operator const float*() const;
    operator float*();
    float &operator[](int i);
    float X() const;
    float& X();
    float Y() const;
    float& Y();
    float Z() const;
    float& Z();

	//Arithmetic operations
    Vector3 operator+(const Vector3 &aRHS) const;
    Vector3 operator-(const Vector3 &aRHS) const;
    Vector3 operator*(float aRHS) const; 
    Vector3 operator/(float aRHS) const;
    Vector3 operator-() const;

    float DotProduct(const Vector3 &aRHS) const;
    Vector3 CrossProduct(const Vector3 &aRHS) const;

	//Arithmetic updates
    Vector3& operator+=(const Vector3 &aRHS);
    Vector3& operator-=(const Vector3 &aRHS);
    Vector3& operator*=(float aRHS);
    Vector3& operator/=(float aRHS);

    static const Vector3 Zero;
    static const Vector3 UnitX;
    static const Vector3 UnitY;
    static const Vector3 UnitZ;
    static const Vector3 NegativeUnitX;
    static const Vector3 NegativeUnitY;
    static const Vector3 NegativeUnitZ;
    static const Vector3 UnitScale;
	
protected:

    union
    {
        float mComponents[3];
        struct
        {
            float mX;
            float mY;
            float mZ;
        };
    };

};

Vector3 operator*(float aLHS, const Vector3& aRHS);

inline Vector3& Vector3::operator=(const Vector3 &aRHS)
{
    mX = aRHS.mX;
    mY = aRHS.mY;
    mZ = aRHS.mZ;

    return *this;
}

inline Vector3::operator const float*() const
{
    return mComponents;
}

inline Vector3::operator float*()
{
    return mComponents;
}

inline float &Vector3::operator[](int i)
{
    assert(i >= 0 && i < 3);
    return mComponents[i];
}

inline float Vector3::X() const
{
    return mX;
}

inline float& Vector3::X()
{
    return mX;
}

inline float Vector3::Y() const
{
    return mY;
}

inline float& Vector3::Y()
{
    return mY;
}

inline float Vector3::Z() const
{
    return mZ;
}

inline float& Vector3::Z()
{
    return mZ;
}

#endif //VECTOR3_H_
