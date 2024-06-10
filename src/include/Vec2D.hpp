#pragma once

#include "Global.hpp"

typedef class __VEC_2D_T__
{
public:
    f32 x, y;

    // Constructors
    __VEC_2D_T__ ()
    : x(0), y(0)
    {}
    
    __VEC_2D_T__ ( f32 x, f32 y )
    : x(x), y(y)
    {}

    __VEC_2D_T__ ( s32 x, s32 y )
    : x(static_cast<f32>(x)), y(static_cast<f32>(y))
    {}

    // Add another vector to this one
    __VEC_2D_T__& operator += ( const __VEC_2D_T__& other )
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Multiply this vector by a scalar
    __VEC_2D_T__& operator *= ( float scalar )
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Retrieve the integer x component
    s32 getXInt() const
    {
        return static_cast<s32>(x);
    }

    // Retrieve the integer y component
    s32 getYInt() const
    {
        return static_cast<s32>(y);
    }
}Vec2D;

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