#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

using namespace std;

#pragma region 'Macros'
    #define SDL2_ERROR_CHECK( x ) if (x < 0) { return -1; }

#pragma endregion
#pragma region 'Typedefs'
    template <typename T0, typename T1>
    using umap = unordered_map<T0, T1>;

    template <typename T>
    using vec = vector<T>;

    template <typename T>
    using func = function<T>;

    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    using s8 = int8_t;
    using s16 = int16_t;
    using s32 = int32_t;
    using s64 = int64_t;
    
    using f32 = float;
    using f64 = double;
#pragma endregion

namespace Engine
{
    #pragma region 'Constants Assigneble Variables'
        inline s32 SCREEN_WIDTH;
        inline s32 SCREEN_HEIGHT;
    #pragma endregion
    #pragma region 'Enums'
        class  Direction { public: enum
        {
            UP      = 1 << 0,
            DOWN    = 1 << 1,
            LEFT    = 1 << 2,
            RIGHT   = 1 << 3,
        };};
    #pragma endregion
    #pragma region 'Forward Declarations'
        typedef class __SDL2_KEY_OBJECT_T__ KeyObject;
        static KeyObject* KeyObjectInstance = nullptr;
    #pragma endregion
}