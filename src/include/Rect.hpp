#pragma once

#include "Global.hpp"

typedef class __RECT_T__ Rect;

typedef class __RECT_T__
{
public:
    s32 x, y, w, h;

    // Constructors
    __RECT_T__ ()
    : x(0), y(0), w(0), h(0)
    {}
    
    __RECT_T__ ( s32 x, s32 y, s32 w, s32 h )
    : x(x), y(y), w(w), h(h)
    {}

    __RECT_T__ ( pair<f32, f32> xY, s32 w, s32 h )
    : x(xY.first), y(xY.second), w(w), h(h)
    {}

    s32 getXInt () const // Retrieve the integer x component
    {
        return x;
    }
    s32 getYInt () const // Retrieve the integer y component
    {
        return y;
    }
    s32 getWInt () const // Retrieve the integer w component
    {
        return w;
    }
    s32 getHInt () const // Retrieve the integer h component
    {
        return h;
    }
    f32 getXFloat () const              // Retrieve the float x component
    {
        return static_cast<f32>(x);
    }
    f32 getYFloat () const              // Retrieve the float y component
    {
        return static_cast<f32>(y);
    }
    f32 getWFloat () const              // Retrieve the float w component
    {
        return static_cast<f32>(w);
    }
    f32 getHFloat () const              // Retrieve the float h component
    {
        return static_cast<f32>(h);
    }
    SDL_Rect toSDL_Rect () const        // Retrieve the SDL_Rect representation of this rect
    {
        return {x, y, w, h};
    }
    SDL_FRect toSDL_FRect () const      // Retrieve the SDL_FRect representation of this rect
    {
        return {static_cast<f32>(x), static_cast<f32>(y), static_cast<f32>(w), static_cast<f32>(h)};
    }

    FORCE_INLINE bool PointInRect(const Pair<f32>* p, const Rect* r)
    {
        return ( (p->x >= r->x) && (p->x < (r->x + r->w)) &&
                (p->y >= r->y) && (p->y < (r->y + r->h)) ) ? true : false;
    }

    /**
     * Returns true if the rectangle has no area.
     */
    FORCE_INLINE bool Empty(const Rect* r)
    {
        return ((!r) || (r->w <= 0) || (r->h <= 0)) ? true : false;
    }

    /**
     * Returns true if the two rectangles are equal.
     */
    FORCE_INLINE bool Equals(const Rect* a, const Rect* b)
    {
        return (a && b && (a->x == b->x) && (a->y == b->y) &&
                (a->w == b->w) && (a->h == b->h)) ? true : false;
    }
}
Rect;