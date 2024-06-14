#pragma once

#include <SDL2/SDL.h>
#include <chrono>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

using namespace std;
class Direction
{
public:
    enum
    {
        UP    = 1 << 0,
        DOWN  = 1 << 1,
        LEFT  = 1 << 2,
        RIGHT = 1 << 3,
    };
};

class State
{
public:
    enum
    {
        STATIC    = 1 << 0,
        ON_GROUND = 1 << 1,
        IS_PLAYER = 1 << 2
    };
};

class Log;
static Log* LogInstance = nullptr;

inline bool DEBUG = false;

#pragma region 'Macros'
#define SDL2_ERROR_CHECK(x)                                                                                            \
    if (x < 0)                                                                                                         \
    {                                                                                                                  \
        return -1;                                                                                                     \
    }
#define FORCE_INLINE __attribute__((always_inline)) static __inline__
#define FORCE_CONSTEXPR constexpr __attribute__((const)) static __inline__
#define FORCE_NO_INLINE __attribute__((noinline))
#define FORCE_CONSTEXPR_NO_INLINE constexpr __attribute__((noinline)) static __inline__
#define FORCE_CONSTEXPR_INLINE constexpr __attribute__((always_inline)) static __inline__

#define FORCE_ACON __attribute__((const)) __attribute__((always_inline)) static __inline__ constexpr auto

// Visibility For Shared Libraries
#define PUBLIC __attribute__((visibility("default")))
#define PRIVATE __attribute__((visibility("hidden")))
#define PROTECTED __attribute__((visibility("protected")))
//
#pragma endregion
#pragma region 'Typedefs'
template <typename T0, typename T1>
using umap = unordered_map<T0, T1>;

template <typename T>
using vec = vector<T>;

template <typename T>
using func = function<T>;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using s8  = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using f32 = float;
using f64 = double;

#pragma endregion
#pragma region 'Constants'
f32 constexpr GRAVITY   = 3.0f;
f32 constexpr FPS       = 120.0f;
f32 constexpr FRAMETIME = 1000.0f / FPS;

#pragma endregion
#pragma region 'helpers'
template <typename T>
FORCE_INLINE void
swap(T& a, T& b)
{
    T temp = a;
    a      = b;
    b      = temp;
}

namespace FLOAT {
    FORCE_CONSTEXPR f32
    normalize(f32 value, f32 period)
    {
        return (value * (period / period)); // 1000 / period = 1 frame
    }
    FORCE_CONSTEXPR f32
    distance_constant_velocity(f32 velocity, f32 time)
    {
        return velocity * time;
    }
    FORCE_CONSTEXPR f32
    calcGravityAcel(f32 time_ms)
    {
        return GRAVITY * (time_ms / 1000.0f);
    }
    FORCE_CONSTEXPR f32
    distance_constant_acceleration(f32 initial_velocity, f32 acceleration, f32 time)
    {
        return initial_velocity * time + 0.5f * acceleration * time * time;
    }
    FORCE_CONSTEXPR f32
    distance_from_final_velocity(f32 initial_velocity, f32 final_velocity, f32 acceleration)
    {
        return (final_velocity * final_velocity - initial_velocity * initial_velocity) / (2 * acceleration);
    }
    FORCE_CONSTEXPR f32
    distance_uniform_circular(f32 radius, f32 angle_radians)
    {
        return radius * angle_radians;
    }
    FORCE_INLINE f32
    clamp(f32 value, f32 min, f32 max)
    {
        return (value < min) ? min : (value > max) ? max : value;
    }
} // namespace FLOAT
#pragma endregion

namespace Engine {
#pragma region 'Forward Declarations'
    typedef class __KEY_OBJECT_T__ KeyObject;
    static KeyObject*              KeyObjectInstance = nullptr;

    typedef class __FYSICS_MAP_OBJECT_T__ FysicsMapObject;
    static FysicsMapObject*               FysicsMapObjectInstanse = nullptr;

    class Core;
    static Core* CoreInstance = nullptr;
#pragma endregion
} // namespace Engine

#pragma region 'Variables'
inline s32 SCREEN_WIDTH;
inline s32 SCREEN_HEIGHT;

#pragma endregion
