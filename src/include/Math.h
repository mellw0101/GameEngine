#pragma once

#include "Global.hpp"

namespace Math {
    namespace FLOAT {
        FORCE_INLINE f64 abs ( f64 x ) {
            return x < 0 ? -x : x;
        }

        FORCE_INLINE f64 sqrt ( f64 x ) {
            f64 xhalf = 0.5f * x;
            i64 i = *(i64*)&x;
            i = 0x5fe6ec85e7de30da - (i >> 1);
            x = *(f64*)&i;
            x = x * (1.5f - xhalf * x * x);
            return 1.0f / x;
        }

        FORCE_INLINE bool PointInRect( f32 pX, f32 pY, f32 rX, f32 rY, f32 rW, f32 rH ) {
            return ( (pX >= rX) && (pX < (rX + rW)) && (pY >= rY) && (pY < (rY + rH)) ) ? true : false;
        }

        FORCE_INLINE f32 between ( f32 value, f32 min, f32 max ) {
            return ( value >= min ) ? (( value <= max ) ? value : max ) : min;
        }
    }
}