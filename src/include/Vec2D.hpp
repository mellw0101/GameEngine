#pragma once

#include "Global.hpp"

class Vec2D
{
public:
    f32 x, y;

    // Constructors
    Vec2D()
    : x(0), y(0)
    {}
    
    Vec2D(f32 x, f32 y)
    : x(x), y(y)
    {}

    // Add another vector to this one
    Vec2D& operator += ( const Vec2D& other )
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Multiply this vector by a scalar
    Vec2D& operator *= ( float scalar )
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
};

// Non-member function to add two vectors
inline Vec2D operator + ( Vec2D lhs, const Vec2D& rhs )
{
    lhs += rhs;
    return lhs;
}

// Non-member function to multiply a vector by a scalar
inline Vec2D operator * ( Vec2D vec, f32 scalar )
{
    vec *= scalar;
    return vec;
}