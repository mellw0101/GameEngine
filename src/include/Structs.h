/// Base structures for the engine
#pragma once

#include "Global.hpp"

namespace Engine::Structs {
    class Vector2D {
    public:
        f32 x;
        f32 y;

        Vector2D ( f32 x, f32 y )
        : x(x), y(y) {}

        Vector2D& ( const Vector2D& other ) {
            x = other.x;
            y = other.y;
            return *this;
        }

        Vector2D& operator =+ ( const Vector2D& other ) {
            x =+ other.x;
            y =+ other.y;
            return *this;
        }

        // Multiply this vector by a scalar
        Vector2D& operator *= ( f32 scalar ) {
            if (DEBUG_MODE) {
                if (scalar == 0) {
                    throw std::invalid_argument("Cannot multiply a vector by 0");
                }
            }
            x *= scalar;
            y *= scalar;
            return *this;
        }
    }
}