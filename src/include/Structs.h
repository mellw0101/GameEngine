/// Base structures for the engine
#pragma once

#include "Global.hpp"

namespace Structs
{
    typedef class __VECTOR_2D_T__
    {
    public:
        f32 x, y;

        // Constructors
        __VECTOR_2D_T__ ()
                : x(0), y(0)
        {}

        __VECTOR_2D_T__ ( f32 x, f32 y )
                : x(x), y(y)
        {}

        __VECTOR_2D_T__ ( s32 x, s32 y )
                : x(static_cast<f32>(x)), y(static_cast<f32>(y)) {}

        __VECTOR_2D_T__ ( pair<f32, f32> xY ) // Initialize the vector with a pair of floats
                : x(xY.first), y(xY.second) {}

        __VECTOR_2D_T__ ( pair<s32, s32> xY ) // Initialize the vector with a pair of integers
                : x(static_cast<f32>(xY.first)), y(static_cast<f32>(xY.second)) {}

        // Add another vector to this one
        __VECTOR_2D_T__& operator += ( const __VECTOR_2D_T__& other ) {
            x += other.x;
            y += other.y;
            return *this;
        }

        // Multiply this vector by a scalar
        __VECTOR_2D_T__& operator *= ( f32 scalar ) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        SDL_FPoint toSDL_FPoint () const {
            return {x, y};
        }

        // Retrieve the integer x component
        s32 getXInt () const {
            return static_cast<s32>(x);
        }

        // Retrieve the integer y component
        s32 getYInt () const {
            return static_cast<s32>(y);
        }

        operator pair<f32, f32> () const {
            return {x, y};
        }
    } __VECTOR_2D_T_;

    // Non-member function to add two vectors
    FORCE_INLINE __VECTOR_2D_T_ operator + ( __VECTOR_2D_T_ lhs, const __VECTOR_2D_T_& rhs ) {
        lhs += rhs;
        return lhs;
    }

    // Non-member function to multiply a vector by a scalar
    FORCE_INLINE __VECTOR_2D_T_ operator * ( __VECTOR_2D_T_ vec, f32 scalar ) {
        vec *= scalar;
        return vec;
    }

    FORCE_INLINE bool PointInRect(const __VECTOR_2D_T_* p, const SDL_Rect* r) {
        return ( (p->x >= r->x) && (p->x < (r->x + r->w)) &&
                 (p->y >= r->y) && (p->y < (r->y + r->h)) ) ? true : false;
    }

    typedef struct __OBJECT_DATA_2D_T__ {
        __VECTOR_2D_T_ position;         // position,  Coordinate Of The Object
        s32 w;
        s32 h;
        f32 speed;
        u32 state;
    } __OBJECT_DATA_2D_T_;

    typedef struct __OBJECT_2D_T__ {
        __OBJECT_DATA_2D_T_ data;

        auto init ( const __OBJECT_DATA_2D_T_& data ) -> void;

        auto move(s32 Direction, f32 speed_override = 0.0) -> void;

        auto move( __VECTOR_2D_T_ vel) -> void;

        auto draw(SDL_Renderer *renderer) const -> void;

        auto isStatic() const -> bool;

        auto rect() const -> SDL_Rect;

        auto frect() const -> SDL_FRect;

        auto state() const -> u32;
    } __OBJECT_2D_T_;
}

// Export Names to Global Namespace for Ease of Use
using Vector2D      = Structs::__VECTOR_2D_T_;
using ObjectData2D  = Structs::__OBJECT_DATA_2D_T_;
using Object2D      = Structs::__OBJECT_2D_T_;


FORCE_INLINE bool obj2DPointInObj2DRect( Object2D const& object, Object2D const& other ) {
    return ( object.data.position.x < other.data.position.x + other.data.w  &&
             object.data.position.x + object.data.w > other.data.position.x &&
             object.data.position.y < other.data.position.y + other.data.h  &&
             object.data.position.y + object.data.h > other.data.position.y );
}
