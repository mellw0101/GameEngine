#pragma once

#include "Global.hpp"
#include "Rect.hpp"

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

    __VEC_2D_T__ ( pair<f32, f32> xY ) // Initialize the vector with a pair of floats
    : x(xY.first), y(xY.second)
    {}

    __VEC_2D_T__ ( pair<s32, s32> xY ) // Initialize the vector with a pair of integers
    : x(static_cast<f32>(xY.first)), y(static_cast<f32>(xY.second))
    {}

    // Add another vector to this one
    __VEC_2D_T__& operator += ( const __VEC_2D_T__& other )
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Multiply this vector by a scalar
    __VEC_2D_T__& operator *= ( f32 scalar )
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    SDL_FPoint toSDL_FPoint () const
    {
        return {x, y};
    }

    // Retrieve the integer x component
    s32 getXInt () const
    {
        return static_cast<s32>(x);
    }

    // Retrieve the integer y component
    s32 getYInt () const
    {
        return static_cast<s32>(y);
    }

    operator pair<f32, f32> () const
    {
        return {x, y};
    }     
}
Vec2D;

FORCE_INLINE bool Vec2DInObj ( Vec2D const* pos, Rect const rect )
{
    return (pos->x >= rect.x && pos->x <= rect.x + rect.w && pos->y >= rect.y && pos->y <= rect.y + rect.h);
}

// Non-member function to add two vectors
FORCE_INLINE Vec2D operator + ( Vec2D lhs, const Vec2D& rhs )
{
    lhs += rhs;
    return lhs;
}

// Non-member function to multiply a vector by a scalar
FORCE_INLINE Vec2D operator * ( Vec2D vec, f32 scalar )
{
    vec *= scalar;
    return vec;
}

FORCE_INLINE bool PointInRect(const Vec2D* p, const SDL_Rect* r)
{
    return ( (p->x >= r->x) && (p->x < (r->x + r->w)) &&
            (p->y >= r->y) && (p->y < (r->y + r->h)) ) ? true : false;
}

FORCE_INLINE bool Empty(const Rect* r)
{
    return ((!r) || (r->w <= 0) || (r->h <= 0)) ? true : false;
}

FORCE_INLINE bool Equals(const Rect* a, const Rect* b)
{
    return (a && b && (a->x == b->x) && (a->y == b->y) &&
            (a->w == b->w) && (a->h == b->h)) ? true : false;
}