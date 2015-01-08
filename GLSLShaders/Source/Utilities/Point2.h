#ifndef POINT2_H_
#define POINT2_H_

class Point2
{
public:

    Point2();
    Point2(float aX, float aY);
    ~Point2();

    Point2& operator=(const Point2 &aP2);
    operator const float*() const;
    operator float*();

    Point2 operator+(const Point2 &aRHS) const;
    Point2 operator-(const Point2 &aRHS) const;
    Point2 operator*(float aRHS) const; 
    Point2 operator/(float aRHS) const;
    Point2 operator-() const;

    Point2& operator+=(const Point2 &aRHS);
    Point2& operator-=(const Point2 &aRHS);
    Point2& operator*=(float aRHS);
    Point2& operator/=(float aRHS);
	
    const float X() const;
    float &X();

    const float Y() const;
    float &Y();

protected:

    union
    {
        float mComponents[2];
        struct
        {
            float mX;
            float mY;
        };
    };
};

Point2 operator*(float aLHS, const Point2& aRHS);

inline Point2 &Point2::operator=(const Point2 &aP2)
{
    mX = aP2.mX;
    mY = aP2.mY;

    return *this;
}

inline Point2::operator const float*() const
{
    return mComponents;
}

inline Point2::operator float*()
{
    return mComponents;
}

inline const float Point2::X() const
{
    return mX;
}

inline const float Point2::Y() const
{
    return mY;
}

inline float &Point2::X()
{
    return mX;
}

inline float &Point2::Y()
{
    return mY;
}

#endif //POINT2_H_
